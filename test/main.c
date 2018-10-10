// main.c
//

#include "babel.h"
#include "mem.h"
#include "pearson.h"
#include "introspect.h"
#include "bstruct.h"
#include "sexpr.h"
#include "bstring.h"
#include "array.h"
#include "io.h"
#include "trie.h"
#include "tptr.h"
#include "list.h"
#include "aop.h"
#include "sap.h"
#include "pds.h"
//#include "approx.h"

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

    int i,j,k;
//    ptr   x;
//    mword y;

    bstruct ACC;
    bstruct temp;
    char   *tempc=" ";
    mword   tempv=0;
    char *c = malloc(8); 

    mem_context *mc;
//    babel_env *be;

    babel_env *be = babel_env_new(1);
    be->THREADED_ALLOC = SET_FLAG;
    ACC = be->nil;
    temp = be->nil;

//    ACC = mem_new_val(be, 8, 0);
    temp = mem_new_val(be, 32, 0);
    for(i=0;i<100;i++){
//        pds_la_update(temp, pearson_hash8(be, be->zero_hash, _val(be, tempv), 1), _val(be, ++tempv), U8_ASIZE);
        pds_la_update(temp, pearson_hash8(be, be->zero_hash, _val(be, tempv++), 1), _val(be, 0x1), U1_ASIZE);
    }
    ACC=temp;

    thread_context *tc;
    bstruct paging_base;
    mword *level1_page;
    mword *level0_page;
    mword *alloc_ptr;

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
//                pds_la_update(ACC, pearson_hash8(be, be->zero_hash, cmd_code_str, strlen(cmd_code_str)), _val(be, ++tempv), U8_ASIZE);
//                pds_la_update(ACC, pearson_hash8(be, be->zero_hash, cmd_code_str, strlen(cmd_code_str)), _val(be, 1), U1_ASIZE);

//                array8_write(ACC, atoi((char*)cmd_code_str), 0xa5);

//                for(i=0;i<100;i++){
//                    pds_la_update(ACC, pearson_hash8(be, be->zero_hash, _val(be, tempv), 1), _val(be, ++tempv), U8_ASIZE);
//                }

//                cmd_code_str = strtok(NULL, " ");
//                if(cmd_code_str == NULL){ _say("Not enough arguments"); continue; }
//                ACC = pds_la_lookup(temp, pearson_hash8(be, be->zero_hash, _val(be, atoi(cmd_code_str)), 1), U8_ASIZE);

//                cmd_code_str = strtok(NULL, " ");
//                if(cmd_code_str == NULL){ _say("Not enough arguments"); continue; }
//                array1_write(ACC, atoi((char*)cmd_code_str), 0x1);

//                cmd_code_str = strtok(NULL, " ");
//                if(cmd_code_str == NULL){ _say("Not enough arguments"); continue; }
//                ACC = pds_la_lookup(temp, pearson_hash8(be, be->zero_hash, _val(be, atoi(cmd_code_str)), 1), U1_ASIZE);

                break;

            case 2:
                _say("exiting");
                return;

            case 3:
                fprintf(stderr,"%s\n",(char*)ACC);
                break;

            case 4:
                //_dc((mword)ACC);
                //_say("");
                //_d((mword)ACC);
                _dq((mword)ACC);
                //_df((mword)ACC);
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
                if(cmd_code_str == NULL){  continue; }
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
                tempc = cmd_code_str + strlen(cmd_code_str) + 1;
                _say((char*)tempc);
                ACC = sexpr_from_string(be, string_c2b(be, tempc, 300));
                break;

            case 12:
                temp = introspect_str(be, bstruct_unload(be,ACC));
                _say((char*)temp);
                break;

            case 13:
                temp = introspect_gv(be, ACC);
                //_say((char*)temp);
                io_spit(be, "work/test.dot", temp, U8_ASIZE, OVERWRITE);
                _say("introspect_gv(ACC) ==> work/test.dot");
                break;

            case 14:
                temp = introspect_svg(be, ACC, MWORD_SIZE, 0, MWORD_ASIZE);
                //_say((char*)temp);
                io_spit(be, "work/intro.svg", temp, U8_ASIZE, OVERWRITE);
                _say("introspect_svg(ACC) ==> work/intro.svg");
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
            "10 n  .....    ACC <== rdv(ACC,n)\n"
//            "11 S  .....    ACC <== sexpr(S)\n"
            "12    .....    introspect_str(ACC)\n"
            "13    .....    introspect_gv(ACC)\n"
            "14    .....    introspect_svg(ACC)\n");

}


// Clayton Bauman 2018

