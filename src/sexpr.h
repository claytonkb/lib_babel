// sexpr.h
// 

#ifndef SEXPR_H
#define SEXPR_H

mword *_pre_sexpr(babel_env *be, mword *string, mword *index);
mword *_sexpr(babel_env *be, mword *string, mword *index, mword *sexpr_type);
mword *_unescape(babel_env *be, mword *string, mword escape_char);
mword *sexpr_from_string(babel_env *be, mword *bstring);

#endif //SEXPR_H

// Clayton Bauman 2017

