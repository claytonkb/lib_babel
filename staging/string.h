// string.h
// 

#ifndef STRING_H
#define STRING_H

mword *string_c2b(babel_env *be, char *string, mword max_safe_length);
int bsprintf( babel_env *be, mword *buf, mword *offset, const char *format, ... );
void wstrim(babel_env *be, mword *bs);
mword *string_to_ul(babel_env *be, mword *string, mword radix);
mword *string_to_array(babel_env *be, mword *string);

#endif //STRING_H

// Clayton Bauman 2018

