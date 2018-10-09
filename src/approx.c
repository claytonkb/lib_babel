// approx.c
//

#include "babel.h"
#include "approx.h"
#include "array.h"

// notes: Use approx_*_scaled() for best perf. Wrapper functions provided for
// convenience.

//
//
mword approx_lookup(mword *sap, mword *key, access_size asize){

    mword sap_size;
   
    switch(asize){
        case MWORD_ASIZE:
            sap_size = size(sap);
            break;
        case U8_ASIZE:
            sap_size = array8_size(sap);
            break;
        case U1_ASIZE:
            sap_size = array1_size(sap);
            break;
        default:
            _pigs_fly;
    }

    approx_lookup_scaled(sap, sap_size, key, asize);

}


//
//
mword approx_lookup_scaled(mword *sap, mword size, mword *key, access_size asize){

    mword sap_index = approx_find_index(key, size);

    switch(asize){
        case MWORD_ASIZE:
            return rdv(sap,sap_index);
        case U8_ASIZE:
            return array8_read(sap, sap_index);
        case U1_ASIZE:
            return array1_read(sap, sap_index);
        default:
            _pigs_fly;
    }

    return NEG_ONE; // silence compiler warning

}


// convenience fn for calling approx_update_scaled()
//
void approx_update(mword *sap, mword *key, mword *payload, access_size asize){

    mword sap_size;
   
    switch(asize){
        case MWORD_ASIZE:
            sap_size = size(sap);
            break;
        case U8_ASIZE:
            sap_size = array8_size(sap);
            break;
        case U1_ASIZE:
            sap_size = array1_size(sap);
            break;
        default:
            _pigs_fly;
    }

    approx_update_scaled(sap, sap_size, key, payload, asize);

}


//
//
void approx_update_scaled(mword *sap, mword size, mword *key, mword *payload, access_size asize){

    mword sap_index = approx_find_index(key, size);
_dd(size);
_dd(sap_index);
    switch(asize){
        case MWORD_ASIZE:
            ldv(sap,sap_index) = *payload;
            break;
        case U8_ASIZE:
            array8_write(sap, sap_index, *payload);
            break;
        case U1_ASIZE:
            array1_write(sap, sap_index, *payload);
            break;
        default:
            _pigs_fly;
    }

}


//
//
mword approx_find_index(mword *key, mword sap_size){

    // FIXME: 64-bit specific:
    double partition_num = (double)(key[1]) / (double)ULONG_MAX;

    return (sap_size * partition_num);

}



// Clayton Bauman 2018

