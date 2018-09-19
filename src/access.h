// access.h
//

#ifndef ACCESS_H
#define ACCESS_H

// lib-babel accessors:
//      Low-level accessors (rdp, ldp, rdv, ldv)
//      Paged-array accessors
//      API accessors
//
// Accessors vs. built-ins
//      Accessors allow look-up in primitive bstructs. A primitive bstruct is
//      "lower-level" than a built-in. Built-ins are recognized by tag, and
//      operations (read, write, remove, exists, etc.) are dispatched to the
//      appropriate function based on tag and applied.
//
//      Paged-array accessors are synthetic accessors that assist in breaking
//      up large primitive bstructs -- a 1GB val-array, for example, will be
//      stored in a paged-array. The PA_LO_THRESH value is what differentiates
//      between large and small arrays. Note that PA's are only used for
//      in-memory storage. A large array is converted to a segmented-array
//      built-in for mass/persistent storage.
//
//      See std.* for segmented-array functions (was called paged-array)

// general accessors:
// use these when you don't know what kind of bstruct you're accessing
void access_api_rd(babel_env *be, bstruct b, mword offset, cptr result,     access_size asize);
void access_api_wr(babel_env *be, bstruct b, mword offset, bstruct payload, access_size asize);

// specific accessors:
mword  access_api_rd_val(babel_env *be, val v, mword offset, access_size asize);
void   access_api_wr_val(babel_env *be, val v, mword offset, cptr payload, access_size asize);
mword *access_api_rd_ptr(ptr p, mword offset);
void   access_api_wr_ptr(ptr p, mword offset, cptr payload);

// paged-array general accessors:
void access_pa_rd(babel_env *be, bstruct b, mword offset, cptr result, access_size asize);
void access_pa_wr(babel_env *be, bstruct b, mword offset, bstruct payload, access_size asize);

// paged-array specific accessors:
mword access_pa_rd_val(babel_env *be, bstruct b, mword offset, cptr result, access_size asize);
void access_pa_wr_val(babel_env *be, bstruct b, mword offset, bstruct payload, access_size asize);
mword *access_pa_rd_ptr(bstruct b, mword offset);
void access_pa_wr_ptr(babel_env *be, bstruct b, mword offset, bstruct payload, access_size asize);

#endif // ACCESS_H

