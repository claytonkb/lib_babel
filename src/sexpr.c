// sexpr.c
//

#include "babel.h"
#include "sexpr.h"
#include "bstring.h"
#include "array.h"
#include "list.h"
#include "bstruct.h"
#include "mem.h"
#include "tptr.h"

#define append_sexpr(x)                                     \
    do{                                                     \
        if(is_nil(curr_sexpr)){                                 \
            curr_sexpr = list_cons(be, (x), be->nil);           \
        }                                                       \
        else{                                                   \
            list_unshift(be,                                    \
                curr_sexpr,                                     \
                (x));                                           \
        }                                                       \
    } while(0)

#define capture_token                                       \
    do{                                                     \
        capture_length = j-i;                                   \
        if(capture_length){                                     \
            captured_token = array8_slice(be, (mword*)bstring, UNITS_32TO8(i), UNITS_32TO8(j));     \
            append_sexpr(captured_token);                       \
        }                                                       \
        else{                                                   \
            parse_error;                                        \
        }                                                       \
    } while(0)


#define capture_dquote                                      \
    do{                                                     \
        capture_length = j-i;                                   \
        if(capture_length){                                     \
            captured_token = sexpr_unescape( be, (ucs4)array8_slice(be, (mword*)bstring, UNITS_32TO8(i), UNITS_32TO8(j)), '\\');    \
            append_sexpr(captured_token);                       \
        }                                                       \
        else{                                                   \
            parse_error;                                        \
        }                                                       \
    } while(0)


#define capture_squote                                      \
    do{                                                     \
        capture_length = j-i;                                   \
        if(capture_length){                                     \
            captured_token = sexpr_unescape( be, (ucs4)array8_slice(be, (mword*)bstring, UNITS_32TO8(i), UNITS_32TO8(j)), '/');    \
            append_sexpr(captured_token);                       \
        }                                                       \
        else{                                                   \
            parse_error;                                        \
        }                                                       \
    } while(0)

#define  adv(x) do{ j++;  goto x; } while(0)
#define  ret(x) do{ j--;  goto x; } while(0)
#define  esc(x) do{ j+=2; goto x; } while(0)
#define advi(x) do{ i=j; j++; goto x; } while(0)
#define parse_error do{ _warn("parse error"); return be->nil; } while(0)

#define chkdone    if(!(j<length)){goto done;}


//
//
mword *sexpr_prelude(babel_env *be, ucs4 bstring, mword *index){

    mword j;
    j=*index;

    mword length = bstrlen((mword*)bstring);

    if(!length)
        goto done;

null_context:
    chkdone;
    switch(bstring[j]){
        case ' ' :
        case 0x0a:
        case 0x0d: adv(null_context);
        case '{' : 
                *index = j+1;
                return sexpr_body(be, bstring, index, bstruct_cp(be, be->zero_hash));
        case '(' : 
                *index = j+1;
                return sexpr_body(be, bstring, index, bstruct_cp(be, be->zero_hash));
        case '[' : 
                *index = j+1;
                return sexpr_body(be, bstring, index, be->nil);
        case '-' : adv(comment_required);
        default  : parse_error;
    }


comment_required:
    chkdone;
    switch(bstring[j]){
        case '-': adv(comment);
        default : parse_error;
    }


comment:
    chkdone;
    switch(bstring[j]){
        case 0x0d: 
        case 0x0a: adv(null_context);
        default  : adv(comment);
    }    

done:
    return be->nil;

}


//
//
mword *sexpr_body(babel_env *be, ucs4 bstring, mword *index, mword *sexpr_type){

    mword i,j;
    i=j=*index;

    mword length = bstrlen((mword*)bstring);

    if(!length)
        goto done;

    mword *curr_sexpr     = be->nil;
    mword *captured_token = be->nil;
    mword  capture_length = 0;

    if(!is_nil(sexpr_type)){
        append_sexpr(sexpr_type);
    }

list_context:
    chkdone;
    switch(bstring[j]){
        case ' ' :
        case 0x0a:
        case 0x0d: adv(list_context);
        case '{' :
                *index = ++j;
                append_sexpr(sexpr_body(be, bstring, index, bstruct_cp(be, be->zero_hash)));
                j = *index;
                goto list_context;
        case '(' : 
                *index = ++j;
                append_sexpr(sexpr_body(be, bstring, index, bstruct_cp(be, be->zero_hash)));
                j = *index;
                goto list_context;
        case '[' :
                *index = ++j;
                append_sexpr(sexpr_body(be, bstring, index, be->nil));
                j = *index;
                goto list_context;
        case '-' : adv(comment_or_token);
        case '}' : 
        case ')' : 
        case ']' : adv(done);
        default  : goto token;
    }


token:
    switch(bstring[j]){
        case '"' : advi(dquote);
        case '\'': advi(squote);
        default  : advi(non_quote);
    }


dquote:
    chkdone;
    switch(bstring[j]){
        case '"' : j++;
                   capture_dquote;
                   goto list_context;
        case '\\': esc(dquote);
        default  : adv(dquote);
    }


squote:        
    chkdone;
    switch(bstring[j]){
        case '\'': j++;
                   capture_squote;
                   goto list_context;
        case '/' : esc(squote);
        default  : adv(squote);
    }


non_quote:
    chkdone;
    switch(bstring[j]){
        case ' ' :
        case 0x0a:
        case 0x0d:
        case '{' :
        case '}' :
        case '(' :
        case ')' :
        case '[' :
        case ']' :
            capture_token;
            goto list_context;
        case '\\': esc(non_quote);
        default  : adv(non_quote);
    }


comment_or_token:
    chkdone;
    switch(bstring[j]){
        case '-': adv(comment);
        default : ret(token);
    }


comment:
    chkdone;
    switch(bstring[j]){
        case 0x0d: 
        case 0x0a: adv(list_context);
        default  : adv(comment);
    }


done:
    *index = j;

    if((list_len(be, curr_sexpr) == 1) && (array_cmp_lex(pcar(curr_sexpr), be->zero_hash, MWORD_ASIZE) == 0)){
        curr_sexpr = _ptr(be, bstruct_cp(be, be->zero_hash));
    }

    return curr_sexpr;

}


// input string: array-8 string ... INCLUDING THE QUOTES
// returns: standard Babel-string
//
mword *sexpr_unescape(babel_env *be, ucs4 bstring, mword escape_char){

    int i,j;
    mword *temp_string;
    mword *final_string;
//    mword string_length = size(string);
    mword string_length = bstrlen((mword*)bstring);

    if(!string_length){ return mem_new_val(be, 1, 0); } //return the empty string

    temp_string = mem_new_val(be, string_length, 0);

    for(i=0,j=0;i<string_length;i++,j++){

        if(rdv(bstring,i) == escape_char){
            mword character = rdv(bstring,i+1);
            if(character == 'n'){
                rdv(temp_string,j) = 0x0a;
                i++;
            }
            else if(character == 'r'){
                rdv(temp_string,j) = 0x0d;
                i++;
            }
            else if(character == 't'){
                rdv(temp_string,j) = 0x09;
                i++;
            }
            else if(character == '0'){

                #define ASCII_CODE_CHAR_LENGTH 2
                mword *ascii_code = mem_new_val(be, ASCII_CODE_CHAR_LENGTH, 0);

                ldv(ascii_code,0) = rdv(bstring,i+2);
                ldv(ascii_code,1) = rdv(bstring,i+3);

                mword *ascii_value = string_to_ul(be, array_to_string(be, ascii_code), 16);

                ldv(temp_string,j) = rdv(ascii_value,0);

                i+=3;

            }
            else if(character == 'u'){

                #define UNICODE_CHAR_LENGTH 4
                mword *unicode = mem_new_val(be, UNICODE_CHAR_LENGTH, 0);

                ldv(unicode,0) = rdv(bstring,i+2);
                ldv(unicode,1) = rdv(bstring,i+3);
                ldv(unicode,2) = rdv(bstring,i+4);
                ldv(unicode,3) = rdv(bstring,i+5);

                mword *unicode_value = string_to_ul(be, array_to_string(be, unicode), 16);

                ldv(temp_string,j) = rdv(unicode_value,0);

                i+=5;

            }
            else{
                ldv(temp_string,j) = character;
                i++;
            }

        }
        else{ //Not an escape sequence

            ldv(temp_string,j) = rdv(bstring,i);

        }

    }

    if(j != string_length){
        final_string = array_slice(be, temp_string, 0, j);
    }
    else{
        final_string = temp_string;
    }

    return final_string;

}


//
//
mword *sexpr_from_string(babel_env *be, mword *bstring){

    mword index=0;
    return sexpr_prelude(be, string_to_array(be, (mword*)bstring), &index);

}


// Clayton Bauman 2018

