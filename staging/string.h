// string.h
// 

#if 0
#ifndef STRING_H
#define STRING_H

#include "babel.h"

mword *string_c2b(babel_env *be, char *string, mword max_safe_length);
void wstrim(babel_env *be, mword *bs);
void bsprintf( babel_env *be, mword *buf, mword *offset, const char *format, ... );
mword *_radix2cu(babel_env *be, mword *string, mword radix);
mword *string_to_array(babel_env *be, mword *string);

#endif //STRING_H
#endif

// Clayton Bauman 2017

