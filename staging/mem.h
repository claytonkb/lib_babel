// mem.h
// 

#ifndef MEM_H
#define MEM_H

#define mem_new_valz(pyr,size) mem_new_val(pyr, size, 0) // mem_new_valz#
#define mem_new_cons(x) (mem_new_ptr(x, 2)) // mem_new_cons#

void *mem_sys_alloc(int size);
void  mem_sys_free(void *p);
bstruct mem_alloc(babel_env *be, mword alloc_sfield);

void *mem_new_val(babel_env *be, mword size, mword init);
void *mem_new_ptr(babel_env *be, mword size);
void *mem_new_tptr(babel_env *be, const mword *hash, mword *bs);
void *_val(babel_env *be, mword value);
void *_ptr(babel_env *be, mword *unsafe_ptr);
mword *mem_new_str(babel_env *be, mword size8, char set_char);
//mword *_newbits(babel_env *be, mword size1);
mword *list_cons(babel_env *be, mword *car, mword *cdr);
mword *list_dcons(babel_env *be, mword *car, mword *cdr, mword *cpr);
void *_mkval(babel_env *be, mword array_size, ...);
void *_mkptr(babel_env *be, mword array_size, ...);
//void *_mk_aop(babel_env *be, mword array_size, ...);

#endif //MEM_H

// Clayton Bauman 2018

