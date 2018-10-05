// constants.h
//

/*****************************************************************************
 *                                                                           *
 *                         CONSTANTS AND UNITS                               *
 *                                                                           *
 ****************************************************************************/


// CODES
//
#define NORMAL_EXIT_CODE    0                                   // NORMAL_EXIT_CODE#
#define DIE_EXIT_CODE       1                                   // DIE_EXIT_CODE#
#define CAT_EXCEPT          1                                   // CAT_EXCEPT#
#define INTERP_RESET        2                                   // INTERP_RESET#
#define OP_RESTART          4                                   // OP_RESTART#


// DIMENSIONS/LIMITS
//
#define BITS_PER_BYTE 8                                         // BITS_PER_BYTE#
#define BYTE_MAX 255                                            // BYTE_MAX#

#define NEG_ONE ((mword)-1)                                     // NEG_ONE#
#define FMAX    NEG_ONE                                         // FMAX#

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

#define MEM_SUGGEST_INIT_ALLOC (1<<21)

// MWORD
//
#define MWORD_SIZE sizeof(void*)                                // MWORD_SIZE#
#define MWORD_BIT_SIZE (MWORD_SIZE * BITS_PER_BYTE)             // MWORD_BIT_SIZE#
#define MWORD_MSB (MWORD_BIT_SIZE-1)                            // MWORD_MSB#
#define MSB_MASK ((mword)1<<MWORD_MSB)                          // MSB_MASK#
#define MWORD_LSB (mword)0                                      // MWORD_LSB#


// UNITS
//
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

#define VAL_TO_PTR(x) (-1*(x))

// MASKS
//
#define MASK_1_BIT   (mword)0x01                                // MASK_1_BIT#
#define MASK_1_BYTE  (mword)0xff                                // MASK_1_BYTE#
#define MASK_1_WORD  (mword)0xffff                              // MASK_1_WORD#
#define MASK_1_DWORD (mword)0xffffffff                          // MASK_1_DWORD#
#define MASK_1_QWORD (mword)0xffffffffffffffff                  // MASK_1_QWORD#
#define MASK_1_MWORD (mword)(-1)                                // MASK_1_MWORD#

#define CTL_MASK (MWORD_SIZE-1)                                 // CTL_MASK#

// BIT TOOLS
//
#define BIT_RANGE(hi,lo) ((FMAX >> (MWORD_MSB-(hi))) & (FMAX << (lo)))  // BIT_RANGE#
#define BIT_MASK(val, hi, lo) (val & BIT_RANGE(hi, lo))                 // BIT_MASK#
#define BIT_SELECT(val, hi, lo) (BIT_MASK(val, hi, lo) >> lo)           // BIT_SELECT#
#define HI_BITS(val, index) BIT_SELECT(val, MWORD_MSB, index)           // HI_BITS#
#define LO_BITS(val, index) BIT_SELECT(val, index, 0)                   // LO_BITS#

#define NBIT_HI_MASK(n) (((n)==0) ? 0 : (FMAX << (MWORD_BIT_SIZE-(n)))) // NBIT_HI_MASK#
#define NBIT_LO_MASK(n) (((n)==0) ? 0 : (FMAX >> (MWORD_BIT_SIZE-(n)))) // NBIT_LO_MASK#

#define MWORD_MUX(A, B, sel) (((A) & sel) | ((B) & (~sel)))             // MWORD_MUX#
#define BIT_MERGE(A, B, sel) ldv(A,0) = MWORD_MUX(B, rdv(A,0), sel);    // BIT_MERGE#

// ALU
//
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

// HASHES
//
#define HASH_BIT_SIZE (mword)128                                        // HASH_BIT_SIZE#
#define HASH_BYTE_SIZE (mword)(HASH_BIT_SIZE/BITS_PER_BYTE)             // HASH_BYTE_SIZE#
#define HASH_SIZE (mword)UNITS_1TOM(HASH_BIT_SIZE)                      // HASH_SIZE#
#define HASH_ALLOC_SIZE (HASH_SIZE+1)                                   // HASH_ALLOC_SIZE#

// TPTRS
//
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

// MISC
//
#define UNINIT_VAL 0xdbadbadbadbadbad                           // UNINIT_VAL#
#define UNINIT_PTR (be->nil)

#define EMPTY_CSTRING ""                                        // EMPTY_CSTRING#



