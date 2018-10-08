// pearson.c
//

// cite: Fast Hashing of Variable-Length Text Strings, Peter K. Pearson 
// http://epaperpress.com/vbhash/download/p677-pearson.pdf
//
// Let h(i,k) be equivalent to pearson128(r,i,k,len(k)) and let a.b represent
// the concatenation of strings a and b... h(i,k) has the following property:
//
//          h(i,a.b) = h(h(i,a),b)
//
// ... for any i in (0,2^128-1) and for any strings a and b.
//
// This property allows a frequently-used prefix to be hashed once and then
// extended with suffixes by passing the prefix as the init argument to the
// hash function.

#include "babel.h"
#include "pearson.h"
#include "mem.h"


// allocating version of pearson128()
//
hash pearson_hash8(babel_env *be, const bstruct init, const char *key, const unsigned strlen){

    hash result = mem_new_val(be, HASH_SIZE, 0);
    pearson128(result,init,key,strlen);

    return result;

}


// result must be a uint64_t[2] array
// init must be a uint64_t[2] array
//
void pearson128(hash result, const bstruct init, const char *key, const unsigned strlen){ // pearson128#

    int i,j;
    unsigned char temp;
    unsigned char round_key;

    result[0] = init[0];
    result[1] = init[1];

    unsigned char *cresult = (unsigned char*)result;

    for(i=0;i<strlen;i++){ // if strlen==0, result=init

        round_key = key[i];

        cresult[0] = pearson_perm[ cresult[15] ^ round_key ];

        for(j=1;j<HASH_BYTE_SIZE;j++){
            cresult[j] = pearson_perm[ cresult[j-1] ^ (round_key+j) ];
        }

        // xoroshiro128
        result[1] ^= result[0];
        result[0]  = ROTL64(result[0], 55) ^ result[1] ^ (result[1] << 14);
        result[1]  = ROTL64(result[1], 36);

   }

}


//
//
static const char pearson_perm[] = // pearson_perm#
{

    0x01, 0x57, 0x31, 0x0c, 0xb0, 0xb2, 0x66, 0xa6, 0x79, 0xc1, 0x06, 0x54, 0xf9, 0xe6, 0x2c, 0xa3, 
    0x0e, 0xc5, 0xd5, 0xb5, 0xa1, 0x55, 0xda, 0x50, 0x40, 0xef, 0x18, 0xe2, 0xec, 0x8e, 0x26, 0xc8, 
    0x6e, 0xb1, 0x68, 0x67, 0x8d, 0xfd, 0xff, 0x32, 0x4d, 0x65, 0x51, 0x12, 0x2d, 0x60, 0x1f, 0xde, 
    0x19, 0x6b, 0xbe, 0x46, 0x56, 0xed, 0xf0, 0x22, 0x48, 0xf2, 0x14, 0xd6, 0xf4, 0xe3, 0x95, 0xeb, 
    0x61, 0xea, 0x39, 0x16, 0x3c, 0xfa, 0x52, 0xaf, 0xd0, 0x05, 0x7f, 0xc7, 0x6f, 0x3e, 0x87, 0xf8, 
    0xae, 0xa9, 0xd3, 0x3a, 0x42, 0x9a, 0x6a, 0xc3, 0xf5, 0xab, 0x11, 0xbb, 0xb6, 0xb3, 0x00, 0xf3, 
    0x84, 0x38, 0x94, 0x4b, 0x80, 0x85, 0x9e, 0x64, 0x82, 0x7e, 0x5b, 0x0d, 0x99, 0xf6, 0xd8, 0xdb, 
    0x77, 0x44, 0xdf, 0x4e, 0x53, 0x58, 0xc9, 0x63, 0x7a, 0x0b, 0x5c, 0x20, 0x88, 0x72, 0x34, 0x0a, 
    0x8a, 0x1e, 0x30, 0xb7, 0x9c, 0x23, 0x3d, 0x1a, 0x8f, 0x4a, 0xfb, 0x5e, 0x81, 0xa2, 0x3f, 0x98,
    0xaa, 0x07, 0x73, 0xa7, 0xf1, 0xce, 0x03, 0x96, 0x37, 0x3b, 0x97, 0xdc, 0x5a, 0x35, 0x17, 0x83, 
    0x7d, 0xad, 0x0f, 0xee, 0x4f, 0x5f, 0x59, 0x10, 0x69, 0x89, 0xe1, 0xe0, 0xd9, 0xa0, 0x25, 0x7b, 
    0x76, 0x49, 0x02, 0x9d, 0x2e, 0x74, 0x09, 0x91, 0x86, 0xe4, 0xcf, 0xd4, 0xca, 0xd7, 0x45, 0xe5, 
    0x1b, 0xbc, 0x43, 0x7c, 0xa8, 0xfc, 0x2a, 0x04, 0x1d, 0x6c, 0x15, 0xf7, 0x13, 0xcd, 0x27, 0xcb, 
    0xe9, 0x28, 0xba, 0x93, 0xc6, 0xc0, 0x9b, 0x21, 0xa4, 0xbf, 0x62, 0xcc, 0xa5, 0xb4, 0x75, 0x4c, 
    0x8c, 0x24, 0xd2, 0xac, 0x29, 0x36, 0x9f, 0x08, 0xb9, 0xe8, 0x71, 0xc4, 0xe7, 0x2f, 0x92, 0x78, 
    0x33, 0x41, 0x1c, 0x90, 0xfe, 0xdd, 0x5d, 0xbd, 0xc2, 0x8b, 0x70, 0x2b, 0x47, 0x6d, 0xb8, 0xd1,

};


// Clayton Bauman 2018

