// tptr.h
// 

#ifndef TPTR_H
#define TPTR_H

tptr   tptr_new(babel_env *be, const mword *hash, mword *bs);
val    tptr_extract_hash(babel_env *be, mword *tptr);
mword *tptr_detag(babel_env *be, tptr t);
void   tptr_update_hash(babel_env *be, mword *tptr, mword *hash);
mword *tptr_extract_ptr(babel_env *be, mword *tptr);
mword *tptr_hard_detag(babel_env *be, mword *tptr);
tptr   tptr_uniq(babel_env *be, mword *bs);

mword *tptr_find_tag_unsafe(babel_env *be, mword *bs, mword *tag);
mword *tptr_find_tag(babel_env *be, mword *search_bs, mword *tag);

#endif //TPTR_H

// Clayton Bauman 2016

