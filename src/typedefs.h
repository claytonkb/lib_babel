// typedefs.h
//

/*****************************************************************************
 *                                                                           *
 *                             TYPEDEFS                                      *
 *                                                                           *
 ****************************************************************************/

typedef uint64_t mword;

typedef mword* bstruct;
typedef mword* tstruct;
typedef uint32_t* ucs4; // used in sexpr.*

// bstruct type-synonyms
//
typedef bstruct ptr;    // ptr-array
typedef bstruct pair;   // ptr-array of size 2 (synonymous with cons-box)
typedef bstruct val;    // val-array
typedef bstruct arr;    // mword array (ptr or val)
typedef bstruct tarr;   // tag array
typedef bstruct val8;   // byte val-array
typedef bstruct val1;   // byte val-array
typedef bstruct tptr;   // tagged-pointer
typedef bstruct cptr;   // C-style (unsafe) pointer (MWORD-aligned)
typedef bstruct cptr8;   // C-style (unsafe) pointer (byte-aligned)
typedef bstruct ls;     // linked-list
typedef bstruct dls;    // doubly linked-list
typedef bstruct tens;   // tensor
typedef bstruct pyr;    // pyramidal array (fixed depth/width + variadic)
typedef bstruct mat;    // ls-based matrix
typedef bstruct aop;    // array-of-pairs
typedef bstruct sap;    // sorted aop
typedef bstruct tens;   // tensor
typedef bstruct str;    // utf-8 encoded byte-string
typedef bstruct hash;   // HASH_SIZE hash value
typedef bstruct bsig;   // bstruct-signature
typedef bstruct map;    // hash-table
typedef bstruct tmap;   // tag-only map
typedef bstruct hist;   // histogram
typedef bstruct lut;    // lookup-table
typedef bstruct ns;     // namespace
typedef bstruct dir;    // directory
typedef bstruct sexpr;  // s-expression
typedef bstruct enuma;  // aop-based enumeration
typedef bstruct tree;   // binary tree data-structre
typedef bstruct ntree;  // aop-based n-ary tree data-structure
typedef bstruct graph;  // undirected graph data-structure
typedef bstruct digraph; // directed graph data-structure
typedef bstruct hygraph; // hyper-graph data-structure
typedef bstruct adjmat; // adjacency-matrix
typedef bstruct sym;    // general-purpose symbol

// static environment --> implemented with sap+linear probing

// enums
//
typedef enum flag_val_enum
        {CLR, SET, IGN} flag_val;

typedef enum access_size_enum
        {U1_ASIZE,   U8_ASIZE,  U16_ASIZE,
         U32_ASIZE,  U64_ASIZE, U128_ASIZE, 
         MWORD_ASIZE} access_size;

typedef enum sort_type_enum
        {UNSIGNED_ST,  SIGNED_ST,   ALPHA_MWORD_ST, ALPHA_BYTE_ST, 
         LEX_MWORD_ST, LEX_BYTE_ST, VAL_ST,         CUSTOM_ST} sort_type;

typedef enum fileout_type_enum 
        {OVERWRITE, APPEND} fileout_type;

// generic_fn
//
typedef void generic_fn(void* out, void* in); // generic_fn#


// ranges
//
//    crange   -> use this to specify a byte-aligned "start/stop" range in memory
//
//    mrange   -> use this to specify an mword-aligned "start/stop" range in memory
//
//    bsrange   -> use this to specify an mword-aligned "start/stop" range in memory
//                 while tracking the base-ptr of the bstruct
//
typedef struct{
    char *start;
    char *end;
} crange;

typedef struct{
    mword *start;
    mword *end;
} mrange;

typedef struct{
    mword *base;
    union {
        mrange malign;
        crange calign;
    } range;
} bsrange;


// adesc
//
// adesc    -> use this descriptor to specify all information related to a 
//              particular array; this is the "Cadillac struct" for working 
//              with Babel arrays
//
// Ex.
//      adesc a;
//      a.bs.range.malign.start = (mword*)foo;
//
typedef struct{
    tptr            tag;
    access_size     asize;
    sort_type       st;
    generic_fn     *cmp;
    bsrange         bs;
} adesc;


typedef struct{
    bstruct paging_base;
    // GC flags
    // nested context list
    mword sys_alloc_count;
    mword sys_free_count;

} mem_context;


typedef struct{
    mword *zero_hash;
    mword *nil;
    mem_context *mem;
    sap static_env;
    map dynamic_env;
} babel_env;



// Clayton Bauman 2018

