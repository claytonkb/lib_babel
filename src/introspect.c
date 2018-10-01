// introspect.c
//

#include "babel.h"
#include "introspect.h"
#include "bstruct.h"
#include "mem.h"
#include "bstring.h"
#include "array.h"


// introspect_gv  --> use for live, in-memory snapshot of bstruct; use Graphviz 
//                      dot tool to construct a graphical image from the string
//                      returned by this function
//
// introspect_str --> use for "round-trippable" string-representation of bstruct
//
// introspect_svg --> use for visual representation of byte-arrays; choose 
//                      any height/width suitable for a given array


// bs must be an unloaded bstruct
//
str introspect_str(babel_env *be, mword *bs){ // introspect_str#

    if(is_nil_tag(be,bs+1)){
        return C2B("[bs s0 nil]\n"); // FIXME: Probably wrong, see ^^^
    }

    mword result_string_length = 
                introspect_calc_buf_length(be, bs, (1<<16), 65,  23,  33,  68,   140);
              //introspect_calc_buf_length(be, bs, const,   nlf, nva, nin, nptr, ntag)

    str result = mem_new_str(be, result_string_length, 0);

    mword bs_size = size(bs);

    mword current_bs_size;
    mword current_ptr;

    mword *bs_base; // XXX bs_base usually contains UNSAFE pointers
    mword bs_offset  = 0;

    mword str_offset = 0;

    int curr_sfield = 0; // int so we can use abs() to get array-size
    int i;

//    char *offset_format = (char*)mem_new_str(be, 7, 0);
    char *offset_format;

    if(bs_size < (1<<4)){
        offset_format = "s0%x ";
    }
    else if(bs_size < (1<<8)){
        offset_format = "s0%02x ";
    }
    else if(bs_size < (1<<16)){
        offset_format = "s0%04x ";
    }
    else if(bs_size < (1UL<<32)){
        offset_format = "s0%08x ";
    }
    else{
        offset_format = "s0%016lx ";
    }

    bsprintf(be, result, &str_offset, "[bs ");

    while(bs_offset < bs_size){

        curr_sfield = (int)ldv(bs,bs_offset);
        bs_offset++;
        bs_base = (bs + bs_offset);

        if(is_nil_tag(be,bs_base)){ // skip nil
            bs_offset += TPTR_SIZE-1;
            continue;
        }

        current_bs_size = UNITS_8TOM(abs(curr_sfield));

        bsprintf(be, result, &str_offset, "\n  ");
        bsprintf(be, result, &str_offset, offset_format, bs_offset);

        if(curr_sfield == 0){ // tptr

            bs_offset += TPTR_SIZE-1;

            bsprintf(be, result, &str_offset, "[tptr ");

            // print tag
            for(i=0; i < HASH_SIZE; i++){            
//                bsprintf(be, result, &str_offset, "0x%08x ", (unsigned)rdv(bs_base,i));
                bsprintf(be, result, &str_offset, "0x%016lx ", (unsigned)rdv(bs_base,i));
            }

            // print inte ptr
            bsprintf(be, result, &str_offset, "[ptr ");

            if(is_nil_tag(be,(mword*)(bs+bs_offset))){
                bsprintf(be, result, &str_offset, "nil "); 
            }
            else{
                bsprintf(be, result, &str_offset, offset_format, (unsigned)UNITS_8TOM(rdv(bs_base,HASH_SIZE+1)));
            }

            bsprintf(be, result, &str_offset, "] ");

        }
        else{

            bs_offset += current_bs_size;

            if(curr_sfield > 0){ // -- leaf

                bsprintf(be, result, &str_offset, "[val ");

                for(i=0; i < current_bs_size; i++){
                    bsprintf(be, result, &str_offset, INTROSPECT_VAL_FORMAT, (mword)rdv(bs_base,i));
                }
            }
            else{ // curr_sfield < 0 -- inte

                bsprintf(be, result, &str_offset, "[ptr ");

                for(i=0; i < current_bs_size; i++){

                    current_ptr = UNITS_8TOM(rdv(bs_base,i));

                    if(is_nil_tag(be,(mword*)(bs+current_ptr))){
                        bsprintf(be, result, &str_offset, "nil "); 
                    }
                    else{
                        bsprintf(be, result, &str_offset, offset_format, (unsigned)UNITS_8TOM(rdv(bs_base,i))); 
                    }
                }

            }
        }

        bsprintf(be, result, &str_offset, "]");

    }

    bsprintf(be, result, &str_offset, " ]\n");

    array_shrink(be,result,0,str_offset-1,U8_ASIZE);

    return result;

}


////
////
//mword is_nil_tag(babel_env *be, mword *tag){ // is_nil_tag#
//
//    return (memcmp(tag, be->nil, HASH_BYTE_SIZE) == 0);
//
//}


//
//
mword introspect_calc_buf_length(
        babel_env *be,
        mword *bs,
        mword const_padding,
        mword nlf_scaling,
        mword nva_scaling,
        mword nin_scaling,
        mword nptr_scaling,
        mword ntag_scaling){

     return
        ( const_padding
            + bstruct_nlf(be, bs)  * nlf_scaling
            + bstruct_nva(be, bs)  * nva_scaling
            + bstruct_nin(be, bs)  * nin_scaling
            + bstruct_nptr(be, bs) * nptr_scaling
            + bstruct_ntag(be, bs) * ntag_scaling );

}


// Returns a string containing the Graphviz text file
// FIXME: use mem_sys_alloc() and memcpy to avoid wasted space
//
str introspect_gv(babel_env *be, mword *bs){ // introspect_gv#

    mword result_string_length = 
                introspect_calc_buf_length(be, bs, (1<<10), 65, 23, 33, 68, 140);

    mword *result = mem_new_str(be, result_string_length, 0);

    mword str_offset  = 0;

    bsprintf(be, result, &str_offset, "digraph babel {\nnode [shape=record];\n");
    bsprintf(be, result, &str_offset, "graph [rankdir = \"LR\"];\n");

    introspect_gv_r(be, bs, result, &str_offset, 1);

    bstruct_clean(be, bs);

    bsprintf(be, result, &str_offset, "}\n");

//    wstrim(be, result);
    array_shrink(be,result,0,str_offset-1,U8_ASIZE);

    return result;

}



//
//
void introspect_gv_r(babel_env *be, mword *bs, mword *result, mword *str_offset, int initial){ // introspect_gv_r#

    int i;
    int clipped=0;

    if( is_traversed_U(bs) ){
        return;
    }

    int num_entries = size(bs);

    if(is_ptr(bs)){ // XXX 33 * NIN

        mark_traversed_U(bs);

        bsprintf(be, result, str_offset, "\"s%08x\" [shape=record,label=\"", (mword)bs); // XXX 33

        for(i=0; i<num_entries; i++){ // XXX 25

            bsprintf(be, result, str_offset, "<f%d> %d", i, i); // XXX 24

            if(i<(num_entries-1)){
                bsprintf(be, result, str_offset, "|"); // XXX 1
            }
        }

        if(initial){ // XXX constant-padding
            bsprintf(be, result, str_offset, "\",style=\"dashed\"];\n");
        }
        else{
            bsprintf(be, result, str_offset, "\"];\n");
        }

        for(i=0; i<num_entries; i++){
#ifndef PYR_INTROSPECT_SHOW_NIL
            if(is_nil(pcar(bs+i))){
                continue;
            }
#endif
            bsprintf(be, result, str_offset, "\"s%08x\":f%d -> \"s%08x\":f0;\n", (mword)bs, i, *(mword *)(bs+i)); // XXX 43
            introspect_gv_r(be, (mword *)*(bs+i), result, str_offset, 0);

        }

        // XXX (25 + 43) * NPT

    }
    else if(is_val(bs)){  // XXX 65 * NLF

        if(num_entries > 8){
            num_entries=8;
            clipped=1;
        }
        else{
            clipped=0;
        }

        bsprintf(be, result, str_offset, "s%08x [style=bold,shape=record,label=\"", (mword)bs); // XXX 43

        for(i=0; i<num_entries; i++){ // XXX 23 * NVA

//            bsprintf(be, result, str_offset, "<f%d> %x", i, *(mword *)(bs+i));              // XXX 22
            bsprintf(be, result, str_offset, "<f%d> %lx", i, *(mword *)(bs+i));              // XXX 22

            if(i<(num_entries-1)){
                bsprintf(be, result, str_offset, "|");              // XXX 1
            }

        }
        if(clipped){

            bsprintf(be, result, str_offset, "|");              // XXX 1
            bsprintf(be, result, str_offset, "<f%d> ...", i);   // XXX <=17

        }

        bsprintf(be, result, str_offset, "\"];\n");             // XXX 4

    }
    else{ //if(is_tptr(bs)){ // is_tptr

        if(!is_nil(bs)){ // XXX 140 * NTAG

//            mword *builtin_tag = be->nil;//xbar_tag_to_string(be, bs);

//            if(is_cptr(bs)){ // we should search all known tags and print corresponding label
//
//                bsprintf(be, result, str_offset, "s%08x [shape=record,label=\"", (mword)bs); // XXX 31
//                bsprintf(be, result, str_offset, "<f0> /pyramid/tag/cptr");
//                bsprintf(be, result, str_offset, "\"];\n"); // XXX 4
//
//                mark_traversed_U(bs);
//
//            }
//            else if(!is_nil(builtin_tag)){
//
//                bsprintf(be, result, str_offset, "s%08x [shape=record,label=\"", (mword)bs); // XXX 31
//                bsprintf(be, result, str_offset, "<f0> %s", builtin_tag);
//                bsprintf(be, result, str_offset, "\"];\n"); // XXX 4
//                
//                if(!is_nil(tcar(bs))){
//                    bsprintf(be, result, str_offset,
//                                "\"s%08x\":f0 -> \"s%08x\":f0 [arrowhead=\"none\"];\n", // XXX 53
//                                (mword)bs, 
//                                (mword)tcar(bs));
//                }
//
//                mark_traversed_U(bs);
//                
////                introspect_gv_r(be, (mword *)(bs+HASH_SIZE+1), result, str_offset, 0);
//                introspect_gv_r(be, tcar(bs), result, str_offset, 0);
//
//            }
//            else{

                bsprintf(be, result, str_offset, "s%08x [shape=record,label=\"", (mword)bs); // XXX 31

                for(i=0; i<HASH_SIZE; i++){ // XXX 13 * 4 = 52

                    bsprintf(be, result, str_offset, "<f%d> %016lx", i, *(mword *)(bs+i)); // XXX 12

                    if(i<(HASH_SIZE-1)){
                        bsprintf(be, result, str_offset, "|"); // XXX 1
                    }

                }

                bsprintf(be, result, str_offset, "\"];\n"); // XXX 4

                bsprintf(be, result, str_offset,
                            "\"s%08x\":f0 -> \"s%08x\":f0 [arrowhead=\"none\"];\n", // XXX 53
                            (mword)bs, 
                            (mword)(bs+HASH_SIZE+1));

                mark_traversed_U(bs);

                introspect_gv_r(be, (mword *)(bs+HASH_SIZE+1), result, str_offset, 0);

//            }

        }
#ifdef PYR_INTROSPECT_SHOW_NIL
        else{

            bsprintf(be, result, str_offset, "s%08x [style=bold,shape=record,label=\"", (mword)bs);
            bsprintf(be, result, str_offset, "<f0> nil (%x)", (mword)bs);
            bsprintf(be, result, str_offset, "\"];\n");

        }
#endif

    }


    mark_traversed_U(bs);

}



//
//
//<svg width="120" height="120" viewBox="0 0 120 120"
//    xmlns="http://www.w3.org/2000/svg">
//
//  <rect x="10" y="10" width="100" height="100"/>
//</svg>
// Returns a string containing an SVG file depicting an array8 as a grayscale
// image (8-bit depth) with dimensions width,height
//
str introspect_svg(babel_env *be, val arr, mword width, mword height, access_size asize){ // introspect_svg#

    #define SVG_RECT_SCALE 20

    mword array_length;

    if(asize==U8_ASIZE)
        array_length = array8_size(arr);
    else
        array_length = UNITS_MTO8(size(arr));

    if(height == 0){
        height = (array_length / width)+1;
    }

    mword svg_height = height*SVG_RECT_SCALE;
    mword svg_width  = width *SVG_RECT_SCALE;
    mword str_offset=0;
    unsigned char *carr = (unsigned char*)arr;

    int i,j,ctr=0;

    mword result_length = 128 + height*width*92; // See [1] for constants

    str result = mem_new_str(be, result_length, 0);

    //<svg width="120" height="120" viewBox="0 0 120 120" xmlns="http://www.w3.org/2000/svg">
    bsprintf(be, result, &str_offset,
        "<svg width=\"%d\" height=\"%d\" viewBox=\"0 0 %d %d\" xmlns=\"http://www.w3.org/2000/svg\">\n",
        svg_width,
        svg_height,
        svg_width,
        svg_height);

    //<rect x="10" y="80" width="10" height="10" stroke="none" fill="#6f6f6f"/>
    for(i=0;i<height;i++){
        for(j=0;j<width;j++){

            if(ctr++ >= array_length) break;

            unsigned char byte = carr[i*width+j];

            bsprintf(be, result, &str_offset,
            "<rect x=\"%d\" y=\"%d\" width=\"%d\" height=\"%d\" stroke=\"none\" fill=\"#%02x%02x%02x\"/>\n",
            (SVG_RECT_SCALE*j), (SVG_RECT_SCALE*i),
             SVG_RECT_SCALE,     SVG_RECT_SCALE,
            byte, byte, byte);

        }
    }

    bsprintf(be, result, &str_offset, "</svg>\n\n");

//    wstrim(be, result);
//mword *array_shrink(babel_env *be, mword *array, mword new_begin, mword new_end, access_size asize);

    array_shrink(be,result,0,str_offset-1,U8_ASIZE);

    return result;

}

/*
Footnotes:
[1] - 128 chosen to leave room for header with large width and height, and
        to leave room for the footer. 92 chosen similarly, but for each
        rectangle. 
*/


// Clayton Bauman 2018

