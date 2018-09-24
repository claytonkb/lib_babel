// string.c
//

#include "babel.h"
#include "string.h"
#include "array.h"
#include "mem.h"
#include "utf8.h"


//
//
mword *string_c2b(babel_env *be, char *string, mword max_safe_length){ // string_c2b#

    mword char_length;

    //strlen is unsafe, use memchr instead:
    void *null_term = memchr(string, 0, (size_t)max_safe_length);

    if(null_term == NULL)
        char_length = max_safe_length;
    else
        char_length = (mword)((char*)null_term - string);

    mword *result = mem_new_str(be, char_length, ' ');

    memcpy(result, string, char_length); // XXX WAIVER(memcpy) XXX //

    return result;

}

#if 0
// Trims trailing whitespace in a val8, similar to trunc() but different
// Intended for internal-use ...
// XXX Won't array_shrink() work after identifying the trim point? XXX
void wstrim(babel_env *be, mword *bs){ // wstrim#

    #define WS_CHAR ' '

    mword size8 = array8_size(bs);

    //find last white-space character
    int i = size8-1;
    char *char_bs = (char*)bs;

    while((i >= 0) && (char_bs[i] == ' ')){
        i--;
    }

    //get new mword-size
    mword msize = array8_mword_size(i);

    //calculate, set new alignment-word
    mword alignment_word = array8_enc_align(i);
    ldv(bs,msize-1) = alignment_word;

    //clean-up next-to-last mword
    ldv(bs,msize-2) = rdv(bs,msize-2) & ~alignment_word;

    //trunc the s-field
    sfield(bs) = UNITS_MTO8(msize);

}

#endif

// babel-ized sprintf()
//
int bsprintf(babel_env *be, mword *buf, mword *offset, const char *format, ... ){ // bsprintf#

    int printed_length;

    int buf_length = array8_size(buf) 
                            - *offset 
                            + 1; // +1 because vsnprintf() discounts for the null-terminator

    if(buf_length <= 0)
        return -1;

    va_list args;
    va_start(args,format);

    printed_length = vsnprintf((char*)buf+*offset, buf_length, format, args);

    if(printed_length < 0) // encoding error
        return -1;

    *offset = *offset + printed_length;

    va_end(args);

    return printed_length;

}

#if 0

// string_to_ul# --> string_to_ul
//
mword *string_to_ul(babel_env *be, mword *string, mword radix){ // string_to_ul#

    unsigned long *result = 
        (unsigned long *)
            mem_new_val(
                    be, 
                    (sizeof(unsigned long) / sizeof(mword)),
                    0);

    *result = strtoul((char*)string,NULL,radix);

    return (mword*)result;

}


//
//
mword *string_to_array(babel_env *be, mword *string){ // string_to_array#

    mword length8    = array8_size(string);
    mword u8_length  = (mword)u8_strlen((char*)string, length8);

//    mword *result = mem_new_val(be, u8_length+1, 0);
    mword *result = mem_new_str(be, (u8_length*4), 0);

    mword length = u8_toucs((uint32_t *)result, u8_length+1, (char *)string, length8);

    array_shrink(be, result, 0, (length*4), BYTE_ASIZE);

    return result;

}

/* Footnotes:
    [1] bstr-array is 32-bits per entry (UCS4)
*/
#endif


// Clayton Bauman 2018

