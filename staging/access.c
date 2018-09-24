// access.c
//

#include "babel.h"
#include "access.h"

// access_[rd|wr]_api:
//      if tptr
//          return get_tptr_ptr
//      else if ptr-array
//          sys-level?
//              literal lookup
//          svc_level?
//              if size<threshold
//                  literal lookup
//              else
//                  paged lookup
//          res_level?
//              "
//          api_level
//      else if val-array
//          ASIZE=1,8,16,32,MWORD
//          sys-level?
//              literal lookup
//          svc_level?
//              if size<threshold
//                  literal lookup
//              else
//                  paged lookup
//          res_level?
//              "
//          api_level?

//rd(pc,x,y)              access_rd_api(pc,x,y,  MWORD_ASIZE) // rd#
//wr(pc,x,y,z)            access_wr_api(pc,x,y,z,MWORD_ASIZE) // wr#
//
//rd8(pc,x,y)             access_rd_api(pc,x,y,  BYTE_ASIZE)  // rd8#
//wr8(pc,x,y,z)           access_wr_api(pc,x,y,z,BYTE_ASIZE)  // wr8#
//
//rd16(pc,x,y)            access_rd_api(pc,x,y,  WORD_ASIZE)  // rd16#
//wr16(pc,x,y,z)          access_wr_api(pc,x,y,z,WORD_ASIZE)  // wr16#
//
//rd32(pc,x,y)            access_rd_api(pc,x,y,  DWORD_ASIZE) // rd32#
//wr32(pc,x,y,z)          access_wr_api(pc,x,y,z,DWORD_ASIZE) // wr32#
//
//rd_svc(pc,x,y,  asize)  access_rd_svc(pc,x,y,  asize)       // rd_svc#
//wr_svc(pc,x,y,z,asize)  access_wr_svc(pc,x,y,z,asize)       // wr_svc#
//
//rd_res(pc,x,y,  asize)  access_rd_res(pc,x,y,  asize)       // rd_res#
//wr_res(pc,x,y,z,asize)  access_wr_res(pc,x,y,z,asize)       // wr_res#
//
//rd_sys(x,y,  asize)     access_rd_sys(x,y,  asize)          // rd_sys#
//wr_sys(x,y,z,asize)     access_wr_sys(x,y,z,asize)          // wr_sys#


/*****************************************************************************
 *                                                                           *
 *                          API-LEVEL ACCESSORS                              *
 *                                                                           *
 ****************************************************************************/

// rdp --> "read pointer"
//
mword *access_rdp_api(babel_env *be, array ptr, mword offset){ // access_rdp_api#

    return nil;

}


// wrp --> "write pointer"
//
mword *access_wrp_api(babel_env *be, array ptr, mword offset, mword *payload){ // access_wrp_api#

    return nil;

}


// rdv --> "read value"
//
mword access_rdv_api(babel_env *be, array val, mword offset){ // access_rdv_api#

    return 0;

}


// wrv --> "write value"
//
mword access_wrv_api(babel_env *be, array val, mword offset, mword *payload){ // access_wrv_api#

    // XXX As a rule, payload is UNSAFE XXX //

    return 0; // Return success status

}


//
//
mword access_rdv_api_asize(babel_env *be, array val, mword offset, access_size asize){ // access_rdv_api_asize#

    switch(access_size){
       case BIT_ASIZE:
            break;
        case BYTE_ASIZE:
            break;
        case WORD_ASIZE:
            break;
        case DWORD_ASIZE:
            break;
        case QWORD_ASIZE:
            break;
        case MWORD_ASIZE:
            return access_rdv_api(pc, val, offset);
            break;
         default:
            _pigs_fly;
    }

    return 0;

}


//
//
mword access_wrv_api_asize(babel_env *be, array val, mword offset, mword payload, access_size asize){ // access_wrv_api_asize#

    switch(access_size){
       case BIT_ASIZE:
            break;
        case BYTE_ASIZE:
            break;
        case WORD_ASIZE:
            break;
        case DWORD_ASIZE:
            break;
        case QWORD_ASIZE:
            break;
        case MWORD_ASIZE:
            return access_rdv_api(pc, val, offset);
            break;
         default:
            _pigs_fly;
    }

    return 0; // Return success status

}


/*****************************************************************************
 *                                                                           *
 *                          RES-LEVEL ACCESSORS                              *
 *                                                                           *
 ****************************************************************************/


/*****************************************************************************
 *                                                                           *
 *                          SVC-LEVEL ACCESSORS                              *
 *                                                                           *
 ****************************************************************************/


/*****************************************************************************
 *                                                                           *
 *                          SYS-LEVEL ACCESSORS                              *
 *                                                                           *
 ****************************************************************************/



// Clayton Bauman 2018

