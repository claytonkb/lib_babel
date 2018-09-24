// babel.h
//
// Every .c file in the Pyramid project should include this file

#ifndef PYRAMID_H
#define PYRAMID_H

// XXX BEGIN AUTOMATION, DO NOT REMOVE XXX //

/*****************************************************************************
 *                                                                           *
 *                         PREPROCESSOR SWITCHES                             *
 *                                                                           *
 ****************************************************************************/

#define DEV_MODE
//#define MEM_DEBUG
//#define PROF_MODE
//#define CHK_MODE


/*****************************************************************************
 *                                                                           *
 *                              INCLUDES                                     *
 *                                                                           *
 ****************************************************************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stddef.h>
#include <assert.h>
#include <setjmp.h>
#include <stdarg.h>
#include <time.h>
#include <stdint.h>
#include <inttypes.h>
#include <time.h>
#include <unistd.h>
#include <math.h>


    /*XXX********************************************************XXX*
     *XXX                                                        XXX*
     *XXX  NOTICE: see about splitting babel.h up into a bunch XXX*
     *XXX     of smaller header files...                         XXX*
     *XXX                                                        XXX*
     *XXX********************************************************XXX*/

    // constants.h
    // typedefs.h
    // primitives.h
    //   accessors
    //   predicates
    //   utilities
    // devtools.h

/*****************************************************************************
 *                                                                           *
 *                         CONSTANTS AND UNITS                               *
 *                                                                           *
 ****************************************************************************/

  ///////////////////
 // CODES          //
//                 ///////////////////////////////////////////////////////////
#define NORMAL_EXIT_CODE    0                                   // NORMAL_EXIT_CODE#
#define DIE_EXIT_CODE       1                                   // DIE_EXIT_CODE#
#define CAT_EXCEPT          1                                   // CAT_EXCEPT#
#define INTERP_RESET        2                                   // INTERP_RESET#
#define OP_RESTART          4                                   // OP_RESTART#

  ///////////////////
 // DIMENSIONS     //
//                 ///////////////////////////////////////////////////////////
#define BITS_PER_BYTE 8                                         // BITS_PER_BYTE#
#define BYTE_MAX 255                                            // BYTE_MAX#

  ///////////////////
 // MWORD          //
//                 ///////////////////////////////////////////////////////////
#define MWORD_SIZE sizeof(mword)                                // MWORD_SIZE#
#define MWORD_BIT_SIZE (MWORD_SIZE * BITS_PER_BYTE)             // MWORD_BIT_SIZE#
#define HALF_MWORD_BIT_SIZE (MWORD_BIT_SIZE/2)                  // HALF_MWORD_SIZE#
#define MWORD_MSB (MWORD_BIT_SIZE-1)                            // MWORD_MSB#
#define MSB_MASK ((mword)1<<MWORD_MSB)                          // MSB_MASK#
#define MWORD_LSB (mword)0                                      // MWORD_LSB#

#define NEG_ONE ((mword)-1)                                     // NEG_ONE#
#define FMAX    NEG_ONE                                         // FMAX#

  ///////////////////
 // UNITS          //
//                 ///////////////////////////////////////////////////////////
#define UNITS_MTO8(x) ((x)*MWORD_SIZE)                          // UNITS_MTO8#
#define UNITS_8TOM(x) ((x)/MWORD_SIZE)                          // UNITS_8TOM#

#define UNITS_8TO1(x) ((x)*BITS_PER_BYTE)                       // UNITS_8TO1#
#define UNITS_1TO8(x) ((x)/BITS_PER_BYTE)                       // UNITS_1TO8#

#define UNITS_32TO8(x) ((x)*4)                                  // UNITS_32TO8#
#define UNITS_8TO32(x) ((x)/4)                                  // UNITS_8TO32#

#define UNITS_MTO1(x) ((x)*MWORD_BIT_SIZE)                      // UNITS_MTO1#
#define UNITS_1TOM(x) ((x)/MWORD_BIT_SIZE)                      // UNITS_1TOM#

#define MODULO_MTO1(x) ((x)%MWORD_BIT_SIZE)                     // MODULO_MTO1#
#define MODULO_MTO8(x) ((x)%MWORD_SIZE)                         // MODULO_MTO8#
#define MODULO_8TO1(x) ((x)%BITS_PER_BYTE)                      // MODULO_8TO1#

#define COMPLEMENT_MTO1(x) (MWORD_BIT_SIZE-(x))                 // COMPLEMENT_MTO1#

  ///////////////////
 // MASKS          //
//                 ///////////////////////////////////////////////////////////
#define MASK_1_BIT   (mword)0x01                                // MASK_1_BIT#
#define MASK_1_BYTE  (mword)0xff                                // MASK_1_BYTE#
#define MASK_1_WORD  (mword)0xffff                              // MASK_1_WORD#
#define MASK_1_DWORD (mword)0xffffffff                          // MASK_1_DWORD#
#define MASK_1_QWORD (mword)0xffffffffffffffff                  // MASK_1_QWORD#
#define MASK_1_MWORD (mword)(-1)                                // MASK_1_MWORD#

#define CTL_MASK (MWORD_SIZE-1)                                 // CTL_MASK#

  ///////////////////
 // BIT TOOLS      //
//                 ///////////////////////////////////////////////////////////
#define BIT_RANGE(hi,lo) ((FMAX >> (MWORD_MSB-(hi))) & (FMAX << (lo)))  // BIT_RANGE#
#define BIT_MASK(val, hi, lo) (val & BIT_RANGE(hi, lo))                 // BIT_MASK#
#define BIT_SELECT(val, hi, lo) (BIT_MASK(val, hi, lo) >> lo)           // BIT_SELECT#
#define HI_BITS(val, index) BIT_SELECT(val, MWORD_MSB, index)           // HI_BITS#
#define LO_BITS(val, index) BIT_SELECT(val, index, 0)                   // LO_BITS#

#define NBIT_HI_MASK(n) (((n)==0) ? 0 : (FMAX << (MWORD_BIT_SIZE-(n)))) // NBIT_HI_MASK#
#define NBIT_LO_MASK(n) (((n)==0) ? 0 : (FMAX >> (MWORD_BIT_SIZE-(n)))) // NBIT_LO_MASK#

#define MWORD_MUX(A, B, sel) (((A) & sel) | ((B) & (~sel)))             // MWORD_MUX#
#define BIT_MERGE(A, B, sel) ldv(A,0) = MWORD_MUX(B, rdv(A,0), sel);    // BIT_MERGE#

  ///////////////////
 // ALU            //
//                 ///////////////////////////////////////////////////////////
#define BYTE_SELECT(val, index) (val \
                                    &  (MASK_1_BYTE << (BITS_PER_BYTE*index)) \
                                    >> (BITS_PER_BYTE*index))           // BYTE_SELECT#

#define WORD_SELECT(val, index) (val \
                                    &  (MASK_1_WORD << (2*BITS_PER_BYTE*index)) \
                                    >> (2*BITS_PER_BYTE*index))         // WORD_SELECT#

#define DWORD_SELECT(val, index) (val \
                                    &  (MASK_1_DWORD << (4*BITS_PER_BYTE*index)) \
                                    >> (4*BITS_PER_BYTE*index))         // DWORD_SELECT#

#define MWORD_SHIFT(A, n) ((n>0) ? (A << (n)) : (A >> (abs(n))))        // MWORD_SHIFT#
#define MWORD_ROTATE(A, n) ((A << n) | (A >> (MWORD_MSB-n)))            // MWORD_ROTATE#

  ///////////////////
 // HASHES         //
//                 ///////////////////////////////////////////////////////////
#define HASH_BIT_SIZE (mword)128                                        // HASH_BIT_SIZE#
#define HASH_BYTE_SIZE (mword)(HASH_BIT_SIZE/BITS_PER_BYTE)             // HASH_BYTE_SIZE#
#define HASH_SIZE (mword)UNITS_1TOM(HASH_BIT_SIZE)                      // HASH_SIZE#
#define HASH_ALLOC_SIZE (HASH_SIZE+1)                                   // HASH_ALLOC_SIZE#

  ///////////////////
 // TPTRS          //
//                 ///////////////////////////////////////////////////////////
#define TAG_SIZE (HASH_SIZE*MWORD_SIZE)                         // TAG_SIZE#
#define INTERP_TAG_SIZE (mword)1                                // INTERP_TAG_SIZE#

// NB: Can use INTERP_TAG_SIZE only when certain we are operating on an
// interpreter-generated tag

// +3 -> s-field + s-field + car
#define TPTR_SIZE (HASH_SIZE+3)                                 // TPTR_SIZE#
#define TPTR_ALLOC_SIZE TPTR_SIZE                               // TPTR_ALLOC_SIZE#

#define TPTR_TAG_OFFSET  (mword)0                               // TPTR_TAG_OFFSET#
#define TPTR_PTR_OFFSET  (HASH_SIZE+1)                          // TPTR_PTR_OFFSET#

#define TPTR_SFIELD (mword)0                                    // TPTR_SFIELD#

  ///////////////////
 // MISC           //
//                 ///////////////////////////////////////////////////////////
#define UNINIT_VAL 0xdbadbadbadbadbad                           // UNINIT_VAL#
#define UNINIT_PTR nil                                          // UNINIT_PTR#

#define EMPTY_CSTRING ""                                        // EMPTY_CSTRING#


/*****************************************************************************
 *                                                                           *
 *                             TYPEDEFS                                      *
 *                                                                           *
 ****************************************************************************/

typedef uint64_t mword; // mword#
typedef uint32_t hword; // hword# h=half-mword

typedef mword* bstruct; // bstruct#

// bstruct type-synonyms
typedef bstruct ptr;    // ptr-array ptr#
typedef bstruct val;    // val-array val#
typedef bstruct arr;    // mword array (ptr or val) arr#
typedef bstruct val8;   // byte val-array val8#
typedef bstruct val1;   // byte val-array val1#
typedef bstruct tptr;   // tagged-pointer tptr#
typedef bstruct cptr;   // C-style (unsafe) pointer cptr#
typedef bstruct ls;     // linked-list ls#
typedef bstruct dls;    // doubly linked-list dls#
typedef bstruct pyr;    // nested ptr-array pyr#
typedef bstruct mat;    // ls-based matrix mat#
typedef bstruct aop;    // array-of-pairs aop#
typedef bstruct sap;    // sorted aop sap#
typedef bstruct tens;   // tensor tens#
typedef bstruct str;    // utf-8 encoded byte-string u8str#
typedef hword*  bstr;   // Babel-string (mword) bstr#
typedef bstruct hash;   // HASH_SIZE hash value hash#
typedef bstruct bsig;   // bstruct-signature bsig#
typedef bstruct map;    // hash-table map#
typedef bstruct hist;   // map-based histogram hist#
typedef bstruct hista;  // aop-based histogram hista#
typedef bstruct lut;    // map-based lookup-table lut#
typedef bstruct luta;   // aop-based lookup-table luta#
typedef bstruct tmap;   // tag-only map tmap#
typedef bstruct ns;     // namespace ns#
typedef bstruct dir;    // directory dir#
typedef bstruct sexpr;  // array-based s-expression sexpr#
typedef bstruct enuma;  // aop-based enumeration enuma#
typedef bstruct tree;   // binary tree data-structre tree#
typedef bstruct ntree;  // aop-based n-ary tree data-structure ntree#
typedef bstruct graph;  // undirected graph data-structure graph#
typedef bstruct digraph; // directed graph data-structure digraph#
typedef bstruct hygraph; // hyper-graph data-structure hygraph#
typedef bstruct adjmat; // adjacency-matrix adjmat#
typedef bstruct sym;    // general-purpose symbol sym#
typedef bstruct fn;     // applicable function fn#
typedef bstruct args;   // argument list args#
typedef bstruct qu;     // quoted expression qu#
typedef bstruct qq;     // quasi-quoted expression qq#
typedef bstruct mac;    // macro form mac#
typedef bstruct thunk;  // delayed-eval thunk#
    // list of thunks:
    //      - loadable
    //      - unloadable
    //      - evalable
    //      - stdlib (load)
    //      ...


typedef struct { // pyr_context#

//    mem_thread_base         *sys_mem;
//    mem_context             *gc_mem;

    aop     static_env;
    aop     dyn_env;

    tptr    zero_hash; // zero_hash#
    tptr    nil;            // nil#

    mword   *empty_string;

//    struct tm               *utc_epoch;
//    mword                   *epoch_ms;
//    mword                   *srand;
//    mword                   *dispatch_table;

//    int                      argc;
//    char                   **argv;
//    mword                   *interp_argv;
//    char                   **envp;

} pyr_context;

  ///////////////////
 // enums          //
//                 ///////////////////////////////////////////////////////////
//
typedef enum flag_val_enum // flag_val# 
        {CLR, SET, IGN} flag_val;

typedef enum access_size_sel_enum // access_size#  
        {BIT_ASIZE,   BYTE_ASIZE,  WORD_ASIZE,
         DWORD_ASIZE, QWORD_ASIZE, MWORD_ASIZE} access_size;

typedef enum sort_type_enum // sort_type# 
        {UNSIGNED,  SIGNED,   ALPHA_MWORD, ALPHA_BYTE, 
         LEX_MWORD, LEX_BYTE, VAL,         CUSTOM} sort_type;

typedef enum fileout_type_enum 
        {OVERWRITE, APPEND} fileout_type; // fileout_type_enum#


  ///////////////////
 // generic_fn     //
//                 ///////////////////////////////////////////////////////////
//
//    generic_fn is used for general-purpose function-call with a function pointer-
//    any function call can be performed with generic_fn pointer by creating a
//    warpper function, packaging the function arguments and return value(s) into a
//    struct and then passing this struct by pointer (void*). The "in" argument is
//    for input arguments to the function and the "out" argument is for return
//    value(s).
//
typedef void generic_fn(void* out, void* in); // generic_fn#


  ///////////////////
 // RANGES         //
//                 ///////////////////////////////////////////////////////////
//
//    crange   -> use this to specify a byte-aligned "start/stop" range in memory
//
//    mrange   -> use this to specify an mword-aligned "start/stop" range in memory
//
//    bsrange   -> use this to specify an mword-aligned "start/stop" range in memory
//                 while tracking the base-ptr of the bstruct
//
typedef struct {  // crange#
    char *start;
    char *end;
} crange;

typedef struct {  // mrange#
    mword *start;
    mword *end;
} mrange;

typedef struct {  // bsrange#
    mword *base;
    union {
        mrange malign;
        crange calign;
    } range;
} bsrange;


  ////////////////////
 // adesc           //
//                  //////////////////////////////////////////////////////////
//
// adesc    -> use this descriptor to specify all information related to a 
//              particular array; this is the "Cadillac struct" for working 
//              with Babel arrays
//
// Ex.
//      adesc a;
//      a.bs.range.malign.start = (mword*)foo;
//
typedef struct {
    tptr            tag;
    access_size asize;
    sort_type       st;
    generic_fn     *cmp;
    bsrange         bs;
} adesc;


  ///////////////////
 // loadable_thunk //
//                 ///////////////////////////////////////////////////////////
//
//    Use loadable_thunk for objects that cannot be saved/restored across invocations
//    of the interpreter (cannot save restore live pointers). If the is_loaded flag
//    is CLR, the loaded_object pointer is not valid. The function that can load the
//    object must be looked up using loader tag and then invoked and given type_tag
//    and unloaded_object. The return value is stored in loaded_object and the 
//    is_loaded flag is changed to SET.
//
//    Note: this thunk is specifically designed for user-defined objects; built-ins
//    (e.g. Pyramid library API call-sites) are automatically loaded/unloaded as 
//    appropriate.
//
typedef struct { // loadable_thunk#
    tptr      type_tag;
    tptr      loader_tag;
    flag_val  is_loaded;
    mword    *unloaded_object;
    mword    *loaded_object;
} loadable_thunk;


  ///////////////////
 // lazy_list      //
//                 ///////////////////////////////////////////////////////////
//
//
//    A lazy list looks like:
//
//        [ptr elem_A [ptr elem_B [ptr ... [tag "lazy_list" val <lazy_list_thunk_ptr> ]]]]
//
//    Note: list_length() returns -1 when invoked on a lazy list
//
//    When a list operator needs to access a list element beyond the end of the
//    lazy list, the list_extender function is called and whatever it returns is
//    appended to the end of the list.
//
typedef struct { // lazy_list#
    ls          list_head;
    ls          list_tail;
    generic_fn *list_extender;
    ls          in;
    ls          out;
} lazy_list;



/*****************************************************************************
 *                                                                           *
 *                              GLOBALS                                      *
 *                                                                           *
 ****************************************************************************/


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
#define getp(x,y)      (*((mword**)(x)+(y)))  // getp#
#define getv(x,y)      (*((mword* )(x)+(y)))  // getv#
#define setp(x,y,z) do{ *((mword**)(x)+(y))=(z); }while(0) // setp#
#define setv(x,y,z) do{ *((mword* )(x)+(y))=(z); }while(0) // setv#


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
#define rd(pc,x,y)              access_rd_api(pc,x,y,  MWORD_ASIZE) // rd#
#define wr(pc,x,y,z)            access_wr_api(pc,x,y,z,MWORD_ASIZE) // wr#

#define rd1(pc,x,y)             access_rd_api(pc,x,y,  BIT_ASIZE)  // rd8#
#define wr1(pc,x,y,z)           access_wr_api(pc,x,y,z,BIT_ASIZE)  // wr8#

#define rd8(pc,x,y)             access_rd_api(pc,x,y,  BYTE_ASIZE)  // rd8#
#define wr8(pc,x,y,z)           access_wr_api(pc,x,y,z,BYTE_ASIZE)  // wr8#

#define rd16(pc,x,y)            access_rd_api(pc,x,y,  WORD_ASIZE)  // rd16#
#define wr16(pc,x,y,z)          access_wr_api(pc,x,y,z,WORD_ASIZE)  // wr16#

#define rd32(pc,x,y)            access_rd_api(pc,x,y,  DWORD_ASIZE) // rd32#
#define wr32(pc,x,y,z)          access_wr_api(pc,x,y,z,DWORD_ASIZE) // wr32#

#define rd_svc(pc,x,y,  asize)  access_rd_svc(pc,x,y,  asize)       // rd_svc#
#define wr_svc(pc,x,y,z,asize)  access_wr_svc(pc,x,y,z,asize)       // wr_svc#

#define rd_res(pc,x,y,  asize)  access_rd_res(pc,x,y,  asize)       // rd_res#
#define wr_res(pc,x,y,z,asize)  access_wr_res(pc,x,y,z,asize)       // wr_res#

#define rd_sys(x,y,  asize)     access_rd_sys(x,y,  asize)          // rd_sys#
#define wr_sys(x,y,z,asize)     access_wr_sys(x,y,z,asize)          // wr_sys#


/*****************************************************************************
 *                                                                           *
 *                            PREDICATES                                     *
 *                                                                           *
 ****************************************************************************/

#define is_nil(x)        ( tageq(x,pc->nil,TAG_SIZE) )                   // is_nil#
#define is_nil_fast(x)   ( itageq(x,nil) )                               // is_nil_fast#
#define is_nil_tag(pc,x) ( memcmp((x), pc->nil, HASH_BYTE_SIZE) == 0)    // is_nil_tag#

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

#define HASHI(pc,res,str) do{    pearson128( (str)res, (bstruct)pc->zero_hash, (char*)str, STRLEN(str) ) } while(0)
#define HASHC(pc,str)     do{ pearson_hash8( pc      , (bstruct)pc->zero_hash, (char*)str, STRLEN(str) ) } while(0)

//#define HASHI(result,str) (pearson_marsaglia16( (char*)result, (char*)global_irt->tags->PYR_TAG_ZERO_HASH, (char*)str, STRLEN(str) ))
//#define HASHC(pyr,str)    HASHA_FORM(pearson_marsaglia16a, pyr, str, STRLEN(str) )
//#define HASH(pyr,str)     HASHA_FORM(pearson_marsaglia16a, pyr, str, array8_size(pyr,str) )
//#define HASHM(pyr,str)    HASHA_FORM(pearson_marsaglia16a, pyr, str, UNITS_MTO8(array_size(pyr,str) )
//
//#define HASH8(pyr,str) (pearson_hash8(pyr,string_c2b(this_pyr, str, STRLEN(str)))) // HASH8#

// signature is HASHM of unloaded bstruct

#define C2B(str) (string_c2b(pc, str, STRLEN(str))) // C2B#

#define ROTL64(x,y) ((x << y) | (x >> (64 - y)))


/*****************************************************************************
 *                                                                           *
 *                            MISC UTILITIES                                 *
 *                                                                           *
 ****************************************************************************/

//#define struct_alloc(x,y) mem_sys_alloc( sizeof(x)*(y) )
#define struct_alloc(x,y) malloc( sizeof(x)*(y) )

#define STRLEN(s) (sizeof(s)-1) // STRLEN#

#define _prn(x)         fprintf(stderr, "%s", x); // _prn#
#define _say(x)         fprintf(stderr, "%s\n", x);   // _say#
#define _notify(x)      fprintf(stderr, "PYRAMID: %s\n", x);   // _notify#
#define _msg(x)         fprintf(stderr, "PYRAMID: %s in %s(), %s line %d\n", x, __func__, __FILE__, __LINE__);   // _msg#
#define _trace          fprintf(stderr, "PYRAMID: %s() @ %s line %d\n", __func__, __FILE__, __LINE__);   // _trace#
#define _reset_trace    fprintf(stderr, "RESET_TRACE: %s() in %s line %d\n", __func__, __FILE__, __LINE__); // _reset_trace#

#define QUOTEME(x)      #x
#define _d(x)           fprintf(stderr, "%s %016lx\n", #x, (uint64_t)x); // d#
#define _dx(x)          fprintf(stderr, "%s %x\n",    QUOTEME(x), (unsigned)x); // dx#
#define _dd(x)          fprintf(stderr, "%s %d\n",    QUOTEME(x), (unsigned)x); // dd#
#define _du(x)          fprintf(stderr, "%s %u\n",    QUOTEME(x), (unsigned)x); // du#
#define _dw(x)          fprintf(stderr, "%s %016x ",  QUOTEME(x), (unsigned)x); // dw#

#define _die            fprintf(stderr, "Died at %s line %d\n", __FILE__, __LINE__); exit(1);  // die#

#define _warn(x)        fprintf(stderr, "WARNING: %s in %s() at %s line %d\n", x, __func__, __FILE__, __LINE__);  // warn#
#define _enhance(x)     fprintf(stderr, "ENHANCEMENT: %s in %s at %s line %d\n", x, __func__, __FILE__, __LINE__); // enhance#
#define _error(x)       fprintf(stderr, "ERROR: %s in %s() at %s line %d\n", x, __func__, __FILE__, __LINE__); // error#
#define _fatal(x)       do{ fprintf(stderr, "FATAL: %s in %s()\n", x, __func__); _die; } while(0) // _fatal#
#define _pigs_fly       _fatal("Pigs CAN fly...") // _pigs_fly#
#define _give_up        _fatal("gave up... don't know how to do this") // _give_up#

#define _sys_err_check(x)                                       \
    do{                                                         \
        if(!(x)){                                               \
            fprintf(stderr, "%s:%d: ", __func__, __LINE__);     \
            perror(#x);                                         \
            exit(-1);                                           \
        }                                                       \
    } while(0)

/*****************************************************************************
 *                                                                           *
 *                                  DEV                                      *
 *                                                                           *
 ****************************************************************************/

#ifdef DEV_MODE

int dev_i;     // dev_i#

#define _dump(x)                                                    \
    io_spit(this_pyr, "test.dot",                                   \
        _bs2gv(this_pyr,                                            \
            (x)),                                                   \
            BYTE_ASIZE,                                             \
            OVERWRITE);

#define _mem(x)                                                       \
    fprintf(stderr, "-------- %016lx\n", sfield(x));          \
    for(dev_i=0; dev_i<alloc_size(x)-1; dev_i++){                     \
        if(dev_i>=0){                                                 \
            fprintf(stderr, "%08lx ", (unsigned)dev_i*MWORD_SIZE);    \
        }                                                             \
        fprintf(stderr, "%016lx\n", rdv(x,dev_i));                    \
    }

#endif

// XXX END AUTOMATION, DO NOT REMOVE XXX //

/*****************************************************************************
 *                                                                           *
 *                        FUNCTION DECLARATIONS                              *
 *                                                                           *
 ****************************************************************************/
int pyramid_init(babel_env *be);


#endif //PYRAMID_H

// Clayton Bauman 2018


