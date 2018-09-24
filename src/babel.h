// babel.h
//

#ifndef BABEL_H
#define BABEL_H


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

#include "constants.h"
#include "typedefs.h"
#include "primitives.h"
#include "cutils.h"


/*****************************************************************************
 *                                                                           *
 *                                  DEV                                      *
 *                                                                           *
 ****************************************************************************/

#ifdef DEV_MODE
int dev_i;
#endif


/*****************************************************************************
 *                                                                           *
 *                             GLOBAL NIL                                    *
 *                                                                           *
 ****************************************************************************/
#define gnil (be->nil)

    // XXX
    //      This #define is utilized as an internal short-hand since every
    // NOTE instance of a babel_env is named "be" in lib_babel. Use of this
    //      #define in other code may have undefined/unpredictable consequences.
    // XXX


/*****************************************************************************
 *                                                                           *
 *                        FUNCTION DECLARATIONS                              *
 *                                                                           *
 ****************************************************************************/
babel_env *babel_env_new(int init_mem_size, int max_mem_size);


#endif //BABEL_H


// Clayton Bauman 2018

