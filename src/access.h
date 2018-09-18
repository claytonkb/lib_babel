// access.h
//

#ifndef ACCESS_H
#define ACCESS_H

void access_api_rd(babel_env *be, bstruct b, mword offset, cptr result, access_size asize);
void access_api_wr(babel_env *be, bstruct b, mword offset, bstruct payload, access_size asize);
mword access_api_rd_val(babel_env *be, val v, mword offset, access_size asize);
void access_api_wr_val(babel_env *be, val v, mword offset, cptr payload, access_size asize);
mword *access_api_rd_ptr(ptr p, mword offset);
void access_api_wr_ptr(ptr p, mword offset, cptr payload);

#endif // ACCESS_H

