// main.c
//

#include "babel.h"
//#include "access.h"
#include "mem.h"
#include "pearson.h"

#include "cutils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void dev_get_line(char *buffer, FILE *stream);
void dev_menu(void);
void dev_prompt(void);


//
//
int main(void){

    dev_prompt();

}

//
//
void dev_prompt(void){

    char *cmd_code_str;
    int   cmd_code=0;

    char buffer[256];

    int i;
//    ptr   x;
//    mword y;

    bstruct ACC;
    bstruct temp;

    mem_context *mc;
    babel_env *be;
   
    _say("type 0 for menu");

    while(1){

        _prn("% ");

        dev_get_line(buffer, stdin);

        cmd_code_str = strtok(buffer, " ");
        if(cmd_code_str == NULL) continue;
        cmd_code = atoi(cmd_code_str);

        switch(cmd_code){
            case 0:
                dev_menu();
                break;

            case 1:
                be = malloc(sizeof(babel_env));

                be->zero_hash = malloc(UNITS_MTO8(HASH_ALLOC_SIZE));
                ldv(be->zero_hash,0) = UNITS_MTO8(HASH_SIZE);
                be->zero_hash++;
                memset((char*)be->zero_hash, 0, UNITS_MTO8(HASH_SIZE));

                be->nil = malloc(UNITS_MTO8(TPTR_ALLOC_SIZE));
                be->nil++;

                sfield(be->nil) = 0;
                sfield(be->nil+TPTR_PTR_OFFSET) = VAL_TO_PTR(MWORD_SIZE);

                pearson128(be->nil, be->zero_hash, "/babel/tag/nil", STRLEN("/babel/tag/nil") );

                tptr_set_ptr(be->nil, be->nil);

                mc = mem_context_new(be,2048);
                ACC = mc->paging_base;

                break;

            case 2:
                _say("exiting");
                return;

            case 3:
//                mem_context_destroy(mc);
                fprintf(stderr,"%s\n",(char*)ACC);
                break;

            case 4:
                _d((mword)ACC);
                break;

            case 5:
                _d(sfield(ACC));
                break;

            case 6:
                _mem(ACC);
                break;

            case 7:
                ACC = be->nil;
                _say("ACC <== nil");
                break;

            case 8:
                cmd_code_str = strtok(NULL, " ");
                if(cmd_code_str == NULL){ continue; }
                ACC = (mword*)strtoul((char*)cmd_code_str,NULL,16);
                _say("ACC <== p");
                break;

            case 9:
                cmd_code_str = strtok(NULL, " ");
                if(cmd_code_str == NULL){ continue; }
                temp = rdp(ACC, atoi((char*)cmd_code_str));
                ACC = temp;
                _say("ACC <== rdp(ACC,n)");
                break;

            case 10:
                cmd_code_str = strtok(NULL, " ");
                if(cmd_code_str == NULL){ continue; }
                temp = (mword*)rdv(ACC, atoi((char*)cmd_code_str));
                ACC = temp;
                _say("ACC <== rdv(ACC,n)");
                break;

            case 11:
//                tempc = cmd_code_str + strlen(cmd_code_str) + 1;
//                _say((char*)tempc);
//                ACC = sexpr_from_string(be, string_c2b(be, tempc, 300));
                break;

            case 12:
//                temp = introspect_str(be, bstruct_unload(be,ACC));
//                _say((char*)temp);
                break;

            case 13:
//                temp = introspect_gv(be, ACC);
//                io_spit(be, "test.dot", temp, BYTE_ASIZE, OVERWRITE);
//                _say("introspect_gv(ACC) ==> test.dot");
                break;

            case 14:
//                temp = introspect_svg(be, ACC, 10, 10);
//                io_spit(be, "intro.svg", temp, BYTE_ASIZE, OVERWRITE);
//                _say("introspect_svg(ACC) ==> intro.svg");
                break;

            default:
                _say("unrecognized cmd_code");
                dev_menu();
                break;
        }

        for(i=0;i<256;i++){ buffer[i]=0; } // zero out the buffer

    }

}


//
//
void dev_get_line(char *buffer, FILE *stream){

    int c, i=0;

    while(1){ //FIXME unsafe, wrong
        c = fgetc(stream);
        if(c == EOF || c == '\n'){
            break;
        }
        buffer[i] = c;
        i++;
    }

    buffer[i] = '\0';

}


//
//
void dev_menu(void){

    _say(   "0     .....    list command codes\n"
            "1     .....    dev one-off\n"
            "2     .....    exit bare metal\n"
            "3     .....    print(\"%s\",ACC)\n"
            "4     .....    _d(ACC)\n"
            "5     .....    _d(sfield(ACC))\n"
            "6     .....    _mem(ACC)\n"
            "7     .....    ACC <== nil\n"
            "8  p  .....    ACC <== p\n"
            "9  n  .....    ACC <== rdp(ACC,n)\n"
            "10 n  .....    ACC <== rdv(ACC,n)\n");

//            "11 S  .....    ACC <== sexpr(S)\n"
//            "12    .....    introspect_str(ACC)\n"
//            "13    .....    introspect_gv(ACC)\n"
//            "14    .....    introspect_svg(ACC)\n"

}


// Clayton Bauman 2018

