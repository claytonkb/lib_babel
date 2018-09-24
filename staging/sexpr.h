// sexpr.h
// 

#ifndef SEXPR_H
#define SEXPR_H

//mword *sexpr_prelude(babel_env *be, mword *string, mword *index);
mword *sexpr_prelude(babel_env *be, bstr bstring, mword *index);
//mword *sexpr_body(babel_env *be, mword *string, mword *index, mword *sexpr_type);
mword *sexpr_body(babel_env *be, bstr bstring, mword *index, mword *sexpr_type);
//mword *sexpr_unescape(babel_env *be, mword *string, mword escape_char);
mword *sexpr_unescape(babel_env *be, bstr bstring, mword escape_char);
mword *sexpr_from_string(babel_env *be, mword *bstring);

#endif //SEXPR_H

// Clayton Bauman 2018

