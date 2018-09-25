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

//src/sexpr.c:38:46: warning: implicit declaration of function ‘array_slice’ [-Wimplicit-function-declaration]
//src/sexpr.c:310:38: warning: implicit declaration of function ‘string_to_ul’ [-Wimplicit-function-declaration]
//src/sexpr.c:310:52: warning: implicit declaration of function ‘array_to_string’ [-Wimplicit-function-declaration]
//src/sexpr.c:367:30: warning: implicit declaration of function ‘string_to_array’ [-Wimplicit-function-declaration]

#define append_sexpr(x)                                     \
    if(is_nil(curr_sexpr)){                                 \
        curr_sexpr = list_cons(be, (x), be->nil);           \
    }                                                       \
    else{                                                   \
        list_unshift(be,                                    \
            curr_sexpr,                                     \
            (x));                                           \
    }


#define capture_token                                       \
    capture_length = j-i;                                   \
    if(capture_length){                                     \
        captured_token = array8_slice(be, (mword*)bstring, UNITS_8TO32(i), UNITS_8TO32(j));     \
        append_sexpr(captured_token);                       \
    }                                                       \
    else{                                                   \
        parse_error;                                        \
    }                                                       \


#define capture_dquote                                      \
    capture_length = j-i;                                   \
    if(capture_length){                                     \
        captured_token = sexpr_unescape( be, (str)array8_slice(be, (mword*)bstring, UNITS_8TO32(i), UNITS_8TO32(j)), '\\');    \
        append_sexpr(captured_token);                       \
    }                                                       \
    else{                                                   \
        parse_error;                                        \
    }                                                       \


#define capture_squote                                      \
    capture_length = j-i;                                   \
    if(capture_length){                                     \
        captured_token = sexpr_unescape( be, (str)array8_slice(be, (mword*)bstring, UNITS_8TO32(i), UNITS_8TO32(j)), '/');    \
        append_sexpr(captured_token);                       \
    }                                                       \
    else{                                                   \
        parse_error;                                        \
    }                                                       \


#define  adv(x) j++;  goto x;     //advance
#define  ret(x) j--;  goto x;     //retard
#define  esc(x) j+=2; goto x;     //escape
#define advi(x) i=j; j++; goto x; //advance, set i
#define parse_error do{ _trace; return be->nil; } while(0)

#define chkdone    if(!(j<length)){goto done;}




//
//
mword *sexpr_prelude(babel_env *be, str bstring, mword *index){ // sexpr_prelude#

    mword j;
    j=*index;

    mword length = bstrlen((mword*)bstring);

    if(!length){ goto done; }

//    mword SEXPR_LIST_SYMBOL[] = { 0x10, 0x6c, 0x69, 0x73, 0x74 };
//    mword SEXPR_CODE_SYMBOL[] = { 0x10, 0x63, 0x6f, 0x64, 0x65 };

//// file.c
//char* myArray[] = { "str1", "str2", ... "str100" };
//const size_t sizeof_myArray = sizeof myArray;

//char test='[';
//_d(bstring[j]);
//_d(test);

null_context:
    chkdone;
    switch(bstring[j]){
        case ' ' :
        case 0x0a:
        case 0x0d: adv(null_context);
        case '{' : 
                *index = j+1;
//                return sexpr_body(be, string, index, bstruct_cp(be, global_irt->symbols->SEXPR_CODE_SYMBOL));
                return sexpr_body(be, bstring, index, bstruct_cp(be, be->zero_hash));
        case '(' : 
                *index = j+1;
//                return sexpr_body(be, string, index, bstruct_cp(be, global_irt->symbols->SEXPR_LIST_SYMBOL) );
                return sexpr_body(be, bstring, index, bstruct_cp(be, be->zero_hash));
        case '[' : 
                *index = j+1;
                return sexpr_body(be, bstring, index, be->nil);
        case '-' : adv(comment_required);
        default  : parse_error;
    }


comment_required:
//_trace;
    chkdone;
    switch(bstring[j]){
        case '-': adv(comment);
        default : parse_error;
    }


comment:
//_trace;
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
mword *sexpr_body(babel_env *be, str bstring, mword *index, mword *sexpr_type){ // sexpr_body#

    mword i,j;
    i=j=*index;

//    mword length = size(string);
//    mword length = UNITS_8TO32(array8_size(bstring));
    mword length = bstrlen((mword*)bstring);

//    str bstring = (str)string;

    if(!length){ goto done; }

    mword *curr_sexpr     = be->nil;
    mword *captured_token = be->nil;
    mword capture_length  = 0;

    if(!is_nil(sexpr_type)){
        append_sexpr(sexpr_type);
    }

list_context:
//_trace;
    chkdone;
    switch(bstring[j]){
        case ' ' :
        case 0x0a:
        case 0x0d: adv(list_context);
        case '{' :
                *index = ++j;
//                append_sexpr(sexpr_body(be, string, index, bstruct_cp(be, global_irt->symbols->SEXPR_CODE_SYMBOL)));
                append_sexpr(sexpr_body(be, bstring, index, bstruct_cp(be, be->zero_hash)));
                j = *index;
                goto list_context;
        case '(' : 
                *index = ++j;
//                append_sexpr(sexpr_body(be, string, index, bstruct_cp(be, global_irt->symbols->SEXPR_LIST_SYMBOL)));
                append_sexpr(sexpr_body(be, bstring, index, bstruct_cp(be, be->zero_hash)));
                j = *index;
                goto list_context;
        case '[' :
                // add a state to check for nil braces []
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
//_trace;
    switch(bstring[j]){
        case '"' : advi(dquote);
        case '\'': advi(squote);
        default  : advi(non_quote);
    }


dquote:
//_trace;
    chkdone;
    switch(bstring[j]){
        case '"' : j++;
                   capture_dquote;
                   goto list_context;
        case '\\': esc(dquote);
        default  : adv(dquote);
    }


squote:        
//_trace;
    chkdone;
    switch(bstring[j]){
        case '\'': j++;
                   capture_squote;
                   goto list_context;
        case '/' : esc(squote);
        default  : adv(squote);
    }


non_quote:
//_trace;
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
//_trace;
    chkdone;
    switch(bstring[j]){
        case '-': adv(comment);
        default : ret(token);
    }


comment:
//_trace;
    chkdone;
    switch(bstring[j]){
        case 0x0d: 
        case 0x0a: adv(list_context);
        default  : adv(comment);
    }


done:
//_trace;
    *index = j;

//    if((list_len(be, curr_sexpr) == 1) && (array_cmp_lex(be, pcar(curr_sexpr), global_irt->symbols->SEXPR_LIST_SYMBOL, MWORD_ASIZE) == 0)){
//        curr_sexpr = _ptr(be, bstruct_cp(be, global_irt->symbols->SEXPR_NIL_SYMBOL));
//    }

    if((list_len(be, curr_sexpr) == 1) && (array_cmp_lex(pcar(curr_sexpr), be->zero_hash, MWORD_ASIZE) == 0)){
        curr_sexpr = _ptr(be, bstruct_cp(be, be->zero_hash));
    }

    return curr_sexpr;

}



// input string: array-8 string ... INCLUDING THE QUOTES
// returns: standard Babel-string
//
mword *sexpr_unescape(babel_env *be, str bstring, mword escape_char){ // sexpr_unescape#

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


//    mword *bstring = _str2ar(be, oi0.data);
//    result0 = sexpr_op2(be, bstring);
//
//
mword *sexpr_from_string(babel_env *be, mword *bstring){ // sexpr_from_string#

    mword index=0;
    return sexpr_prelude(be, (str)string_to_array(be, (mword*)bstring), &index);

}


////
////
//mword *sexpr_array_from_string(babel_env *be, mword *bstring){ // sexpr_array_from_string#
//
//    mword index=0;
//    mword *sexpr_body = sexpr_prelude(be, string_to_array(be, bstring), &index);
//
//    return sexpr_array_from_string_r(be, sexpr_body, nil, nil);
//
//}
//
//
//mword *sexpr_array_from_string_r(babel_env *be, mword *sexpr_body, mword *sexpr_array, mword *sexpr_array_head){ // sexpr_array_from_string_r#
//
//    if(is_nil(sexpr_body))
//        return list_to_ptr_array(be, sexpr_array_head);
//    
//    if(is_nil(sexpr_array_head)){
//        mword *new_sexpr_array = list_cons(be, pearson_hash(be, bstruct_unload(be, pcar(sexpr_body)), nil));
//        return sexpr_array_from_string_r(be,
//                pcdr(sexpr_body),
//                new_sexpr_array,
//                new_sexpr_array);
//    }
//    else{
//        if(is_ptr(pcar(sexpr_body))){ // nested list
//            return sexpr_array_from_string_r(be, pcar(sexpr_body), nil, nil);
//        }
//        else{
//            sexpr_array = list_cons(be, sexpr_array, pcar(sexpr_body));
//            return sexpr_array_from_string_r(be, pcdr(sexpr_body), sexpr_array, sexpr_array_head);
//        }
//    }
//
//}



// Clayton Bauman 2018

