// constants.h
//

/*****************************************************************************
 *                                                                           *
 *                              CONSTANTS                                    *
 *                                                                           *
 ****************************************************************************/

// CODES
//
#define NORMAL_EXIT_CODE    0
#define DIE_EXIT_CODE       1
#define CAT_EXCEPT          1
#define INTERP_RESET        2
#define OP_RESTART          4

// DIMENSIONS/LIMITS
//
#define BITS_PER_BYTE 8
#define BYTE_MAX 255

#define NEG_ONE ((mword)-1)
#define FMAX    NEG_ONE

// MEM & PAGING
//
#define PA_LEVEL2_MSB 36
#define PA_LEVEL2_LSB 29
#define PA_LEVEL1_MSB 28
#define PA_LEVEL1_LSB 21
#define PA_LEVEL0_MSB 20
#define PA_LEVEL0_LSB 0

#define SMALL_PAGE_SIZE   (1<<12)
#define LARGE_PAGE_SIZE   (1<<PA_LEVEL1_LSB)
//#define LARGE_PAGE_SIZE   (128)
#define PA_LO_THRESH (LARGE_PAGE_SIZE/2)
#define PA_DIR_SIZE 256
//#define PA_DIR_SIZE 16
#define LEVEL0_PAGE_SIZE LARGE_PAGE_SIZE

#define MEM_SUGGEST_INIT_ALLOC (1<<21)

// MWORD
//
#define MWORD_SIZE sizeof(void*)
#define MWORD_BIT_SIZE (MWORD_SIZE * BITS_PER_BYTE)
#define MWORD_MSB (MWORD_BIT_SIZE-1)
#define MSB_MASK ((mword)1<<MWORD_MSB)
#define MWORD_LSB (mword)0


// MASKS
//
#define MASK_1_BIT   (mword)0x01
#define MASK_1_BYTE  (mword)0xff
#define MASK_1_WORD  (mword)0xffff
#define MASK_1_DWORD (mword)0xffffffff
#define MASK_1_QWORD (mword)0xffffffffffffffff
#define MASK_1_MWORD (mword)(-1)

#define CTL_MASK (MWORD_SIZE-1)

// HASHES
//
#define HASH_BIT_SIZE (mword)128
#define HASH_BYTE_SIZE (mword)(HASH_BIT_SIZE/BITS_PER_BYTE)
#define HASH_SIZE (mword)UNITS_1TOM(HASH_BIT_SIZE)
#define HASH_ALLOC_SIZE (HASH_SIZE+1)

// TPTRS
//
#define TAG_SIZE (HASH_SIZE*MWORD_SIZE)
#define INTERP_TAG_SIZE (mword)1

// NB: Can use INTERP_TAG_SIZE only when certain we are operating on an
// interpreter-generated tag

// +3 -> s-field + s-field + car
#define TPTR_SIZE (HASH_SIZE+3)
#define TPTR_ALLOC_SIZE TPTR_SIZE

#define TPTR_TAG_OFFSET  (mword)0
#define TPTR_PTR_OFFSET  (HASH_SIZE+1)

#define TPTR_SFIELD (mword)0

// MISC
//
#define UNINIT_VAL 0xdbadbadbadbadbad
#define UNINIT_PTR (be->nil)

#define EMPTY_CSTRING ""



