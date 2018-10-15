// primitives.h
//

/*****************************************************************************
 *                                                                           *
 *                       LOW-LEVEL ACCESSORS                                 *
 *                                                                           *
 ****************************************************************************/

/////// sfield accessor ///////
#define sfield(x) (*((mword*)x-1))      // sfield#

/////// pointer-deref accessors ///////

//RHS dereference for ptr array
#define rdp(x,y) (*((mword**)x+y))      // rdp#

//RHS dereference for val array
#define rdv(x,y) (*((mword*)x+y))       // rdv#

//LHS dereference for ptr array
#define ldp(x,y) (*((mword**)x+y))      // ldp#

//LHS dereference for val array
#define ldv(x,y) (*((mword*)x+y))       // ldv#


// mutators
#define bgetp(x,y)      (*((mword**)(x)+(y)))  // bgetp#
#define bgetv(x,y)      (*((mword* )(x)+(y)))  // bgetv#
#define bsetp(x,y,z) do{ *((mword**)(x)+(y))=(z); }while(0) // bsetp#
#define bsetv(x,y,z) do{ *((mword* )(x)+(y))=(z); }while(0) // bsetv#


/////// Lisp-style accessors ///////
#define vcar(x) ((mword)rdv(x,0))       // vcar#
#define vcdr(x) ((mword)rdv(x,1))       // vcdr#
#define vcpr(x) ((mword)rdv(x,2))       // vcpr#

#define pcar(x) ((mword*)rdp(x,0))      // pcar#
#define pcdr(x) ((mword*)rdp(x,1))      // pcdr#
#define pcpr(x) ((mword*)rdp(x,2))      // pcpr#

//list-safe car/cdr (not tptr safe):
#define lcar(x)     (is_nil(x) ? nil : pcar(x)) // lcar#
#define lcdr(x)     (is_nil(x) ? nil : pcdr(x)) // lcdr#

/////// tptr accessors ///////
#define tcar(x) ((mword*)rdp(x,TPTR_PTR_OFFSET))                    // tcar#
#define tptr_set_tag(dest,src) tagcpy(dest,src)                     // tptr_set_tag#
#define tptr_set_ptr(dest,src) (ldp(dest,TPTR_PTR_OFFSET) = src)    // tptr_set_ptr#

/////// generic bstruct accessors ///////
#define rbs(x,y,z) (sfield(x)>0) ? ((z)=rdv(x,y)) : ((sfield(x)<0) ? ((z)=rdp(x,y)) : ((z)=read_thunk(this_pyr,x,y)))
#define wbs(x,y,z) (sfield(x)>0) ? (ldv(x,y)=(z)) : ((sfield(x)<0) ? (ldp(x,y)=(z)) : (write_thunk(this_pyr,x,y,z)))

/////// relative offset accessors ///////
#define rel_to_abs(base,rel) (mword*)(base+UNITS_8TOM((mword)rel))
#define abs_to_rel(base,abs) (mword*)(base-UNITS_8TOM((mword)abs))

#define rrdp(base,x,y)  (*((mword*)(base+(UNITS_8TOM(x)))+y))
#define rldp(base,x,y)  (*((mword*)(base+(UNITS_8TOM(x)))+y))

#define rrdv(base,x,y)  (*((mword*)(base+(UNITS_8TOM(x)))+y))
#define rldv(base,x,y)  (*((mword*)(base+(UNITS_8TOM(x)))+y))

#define rtcar(base,x)  ((mword*)rrdp(base,x,TPTR_PTR_OFFSET))

/////// aop accessors ///////
#define  key_aopv(x,y) vcar(pcar(rdp(x,y)))
#define data_aopv(x,y) vcar(pcar(rdp(x,y)))

#define  key_aop(x,y) pcar(rdp(x,y))
#define data_aop(x,y) pcar(rdp(x,y))


/////// proper accessors ///////
#define rd(be,x,y)              access_rd_api(be,x,y,  MWORD_ASIZE) // rd#
#define wr(be,x,y,z)            access_wr_api(be,x,y,z,MWORD_ASIZE) // wr#

#define rd1(be,x,y)             access_rd_api(be,x,y,  BIT_ASIZE)  // rd8#
#define wr1(be,x,y,z)           access_wr_api(be,x,y,z,BIT_ASIZE)  // wr8#

#define rd8(be,x,y)             access_rd_api(be,x,y,  BYTE_ASIZE)  // rd8#
#define wr8(be,x,y,z)           access_wr_api(be,x,y,z,BYTE_ASIZE)  // wr8#

#define rd16(be,x,y)            access_rd_api(be,x,y,  WORD_ASIZE)  // rd16#
#define wr16(be,x,y,z)          access_wr_api(be,x,y,z,WORD_ASIZE)  // wr16#

#define rd32(be,x,y)            access_rd_api(be,x,y,  DWORD_ASIZE) // rd32#
#define wr32(be,x,y,z)          access_wr_api(be,x,y,z,DWORD_ASIZE) // wr32#

#define rd_svc(be,x,y,  asize)  access_rd_svc(be,x,y,  asize)       // rd_svc#
#define wr_svc(be,x,y,z,asize)  access_wr_svc(be,x,y,z,asize)       // wr_svc#

#define rd_res(be,x,y,  asize)  access_rd_res(be,x,y,  asize)       // rd_res#
#define wr_res(be,x,y,z,asize)  access_wr_res(be,x,y,z,asize)       // wr_res#

#define rd_sys(x,y,  asize)     access_rd_sys(x,y,  asize)          // rd_sys#
#define wr_sys(x,y,z,asize)     access_wr_sys(x,y,z,asize)          // wr_sys#


/*****************************************************************************
 *                                                                           *
 *                            PREDICATES                                     *
 *                                                                           *
 ****************************************************************************/

#define is_nil(x)        ( tageq(x,be->nil,TAG_SIZE) )                   // is_nil#
#define is_nil_fast(x)   ( itageq(x,nil) )                               // is_nil_fast#
#define is_nil_tag(be,x) ( memcmp((x), be->nil, HASH_BYTE_SIZE) == 0)    // is_nil_tag#

#define is_val(x)    ((int)sfield((mword*)x) >  0)                  // is_val#
#define is_ptr(x)    ((int)sfield((mword*)x) <  0)                  // is_ptr#
#define is_tptr(x)   ((int)sfield((mword*)x) == 0)                  // is_tptr#
//#define is_cptr(x)   tageq((x), global_irt->tags->PYR_TAG_CPTR, TAG_SIZE) // is_cptr#

#define is_val_sfield(x)    (((int)x) > 0) // is_val_sfield#
//#define is_val(x)    is_val_sfield(sfield((mword*)x))

#define is_tptr_spec(x)   (!is_val(x) && !is_ptr && !is_nil(x))     // is_tptr_spec#

#define is_conslike(x) (is_ptr(x) && size(x) == 2)                  // is_conslike#

#define is_traversed_U(x)       (!((sfield(x) & 0x1)==0))           // is_traversed_U#
#define is_traversed_V(x)       (!((sfield(x) & 0x2)==0))           // is_traversed_V#
#define is_traversed_U_or_V(x)  (!((sfield(x) & CTL_MASK)==0))      // is_traversed_U_or_V#
#define is_traversed_U_and_V(x) (!((sfield(x) & 0x3)==0))           // is_traversed_U_and_V#

#define is_val_masked(x)     ((~CTL_MASK & (int)sfield((mword*)x)) >  0) // is_val_masked#
#define is_ptr_masked(x)     ((~CTL_MASK & (int)sfield((mword*)x)) <  0) // is_ptr_masked#
#define is_tptr_masked(x)    ((~CTL_MASK & (int)sfield((mword*)x)) == 0) // is_tptr_masked#

#define is_false_tptr(tptr)                                 \
           (tageq(tptr, PYR_TAG_FALSE     , TAG_SIZE)       \
        ||  tageq(tptr, PYR_TAG_REJECT    , TAG_SIZE)       \
        ||  tageq(tptr, PYR_TAG_UNEXIST   , TAG_SIZE)       \
        ||  tageq(tptr, PYR_TAG_INTERP_NIL, TAG_SIZE))


#define is_large_arr(x) (size(x) > PA_LO_THRESH)

// place-holder for compilation; belongs in tptr.*
#define is_builtin(x) (0)

#define flag_set(x) ((x) == SET_FLAG)
#define flag_clr(x) ((x) == CLR_FLAG)
#define flag_ign(x) ((x) == IGN_FLAG)
#define flag_def(x) (flag_set(x) || flag_clr(x) || flag_ign(x))


/*****************************************************************************
 *                                                                           *
 *                              PRIMITIVES                                   *
 *                                                                           *
 ****************************************************************************/

// size is in units of MWORDS
#define cpy(dest,src,size) memcpy(dest,src,UNITS_MTO8(size)) // cpy#

#define val_gt(left,right) (array_cmp_num(left, right) >  0)
#define val_ge(left,right) (array_cmp_num(left, right) >= 0)
#define val_lt(left,right) (array_cmp_num(left, right) <  0)
#define val_le(left,right) (array_cmp_num(left, right) <= 0)
#define val_eq(left,right) (array_cmp_num(left, right) == 0)
#define val_ne(left,right) (array_cmp_num(left, right) != 0)

#define ptr2val(x) sfield(x) = abs(sfield(x))

// FIXME sed -i to choose_max(), choose_min()
#define MAX(a,b) ((a>=b) ? (a) : (b)) // MAX#
#define MIN(a,b) ((a<=b) ? (a) : (b)) // MIN#

#define floor_clamp(x,y) ((x) < (y)) ? (y) : (x); // floor_clamp#
#define ceil_clamp(x,y)  ((x) > (y)) ? (y) : (x); // ceil_clamp#

// returns true if x>=y and x<=z
#define in_bounds(x,y,z) (!(((x) < (y)) || ((x) > (z)))) // in_bounds#

//#define TOGGLE_FLAG(x) (((x) == IGN) ? (x = IGN) : (((x) == SET) ? (x = CLR) : (x = SET))) // TOGGLE_FLAG#

#define tagcmp(x,y,z) ( (is_tptr(x) || (size(x) >= HASH_SIZE)) ? (memcmp((mword*)x, y, z)) : -1 ) // tagcmp#
#define tageq(x,y,z)  ( tagcmp(x,y,z) == 0 ) // tageq#
#define itageq(x,y)   ( tagcmp(x,y,INTERP_TAG_SIZE) == 0 ) // itageq#
#define tagcpy(dest,src) cpy(dest,src,HASH_SIZE) // tagcpy#

#define mark_traversed_U(x) (sfield(x) |= 0x1) // mark_traversedU#
#define mark_traversed_V(x) (sfield(x) |= 0x2) // mark_traversedV#

#define size(x)             (abs(sfield(x))/MWORD_SIZE)                 // size#
#define hsize(x)            (size(x)*2)                                 // hsize#
#define size_special(x)     (sfield(x) == 0 ? HASH_SIZE : size(x))      // size_special#
#define alloc_size(x)       (sfield(x) == 0 ? TPTR_SIZE : size(x)+1)    // alloc_size#
#define mem_alloc_size(x)   (x == 0 ? TPTR_SIZE : (abs(x)/MWORD_SIZE))  // mem_alloc_size#
#define size_masked(x)      (abs(~CTL_MASK & sfield(x))/MWORD_SIZE)     // size_masked#
#define bstrlen(x)          UNITS_8TO32(array8_size(x))                 // bstrlen#

#define _mktptr(pyr,key,bs) mem_new_tptr(pyr,HASHC(pyr,key),bs)         // _mktptr#

#define mem_sys_free_bs(bs,size) mem_sys_free(bs-1,size) // mem_sys_free_bs#

//#define HASHA_FORM(hash_fn,pyr,str,strlen) (hash_fn( pyr, (char*)global_irt->tags->PYR_TAG_ZERO_HASH, (char*)str, strlen ))

// HASHI# --> Hash a constant string (non-allocating)
// HASHC# --> Hash a constant string (allocating)
// HASH#  --> Hash a Babel string    (allocating)
// HASHM# --> Hash a val-array       (allocating)

#define HASHI(be,res,str)    pearson128( (str)res, (bstruct)be->zero_hash, (char*)str, STRLEN(str) )
#define HASHC(be,str)     pearson_hash8( be      , (bstruct)be->zero_hash, (char*)str, STRLEN(str) )
#define HASH8(be,str)    (pearson_hash8( be      , be->zero_hash         , (char*)string_c2b(be, str, STRLEN(str)), STRLEN(str)))

//#define HASHI(result,str) (pearson_marsaglia16( (char*)result, (char*)global_irt->tags->PYR_TAG_ZERO_HASH, (char*)str, STRLEN(str) ))
//#define HASHC(pyr,str)    HASHA_FORM(pearson_marsaglia16a, pyr, str, STRLEN(str) )
//#define HASH(pyr,str)     HASHA_FORM(pearson_marsaglia16a, pyr, str, array8_size(pyr,str) )
//#define HASHM(pyr,str)    HASHA_FORM(pearson_marsaglia16a, pyr, str, UNITS_MTO8(array_size(pyr,str) )

// signature is HASHM of unloaded bstruct

#define C2B(str) (string_c2b(be, str, STRLEN(str))) // C2B#

#define ROTL64(x,y) ((x << y) | (x >> (64 - y)))



/*****************************************************************************
 *                                                                           *
 *                                   DEV                                     *
 *                                                                           *
 ****************************************************************************/

#define _dump(x)        do{ fprintf(stderr, "%s\n", (char*)introspect_str(be, bstruct_unload(be,(x)))); } while(0)

#define _mem(x)                                                       \
    fprintf(stderr, "---------------- %016lx\n", sfield(x));          \
    for(dev_i=0; dev_i<alloc_size(x)-1; dev_i++){                     \
        if(dev_i>=0){                                                 \
            fprintf(stderr, "%016lx ", (uint64_t)dev_i*MWORD_SIZE);   \
        }                                                             \
        fprintf(stderr, "%016lx\n", *((uint64_t*)x+dev_i));           \
    }


