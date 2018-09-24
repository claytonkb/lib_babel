TODO
====

*BUGS AT BOTTOM*

Migration
---------

**NOTE**: \*.c files are from '16 pyramid; update to ai_ml/pyramid ('18)  
**NOTE**: will need to re-migrate accordingly

*TOS*

    introspect_str() dependencies:
    
        is_nil_tag(be,bs+1)
        C2B("[bs s0 nil]\n")
        mem_new_str(be, result_string_length, 0);
        bsprintf(be, result, &str_offset, "[bs ");
        array_shrink(be,result,0,str_offset-1,BYTE_ASIZE);

*Order of migration*

introspect\_\*  
sexpr\*  
bstruct_load_str() --> implementation can wait, but stub it in  
bstruct/array/list/trie  
aop/sap  
array1  

Will call this "lib_babel 0.5"; ready for external use  


*Files*

array.c  
bstruct.c  
introspect.c  
io.c  
list.c  
mem.c  
mt19937ar.c  
pearson.c  
sexpr.c  
string.c  
tptr.c  
trie.c  
utf8.c  


*Edits*

    %s/pyr_cache \*this_pyr/babel_env *be/g
    %s/\<pc\>/be/g

    %s/access_size/asize/g
    %s/access_size_sel/access_size/g

    sed -i -e 's/\baccess_size\b/asize/g' array.h
    sed -i -e 's/\baccess_size_sel\b/access_size/g' array.h

    sed -i -e 's/pyr_cache \*this_pyr/babel_env *be/g' *

* There is no global_irt
* nil?
* mem_new\_\* ?


DONEISH
=======

Paging/GC
---------

    #define PA_LO_THRESH (1<<20)

PA_LO_THRESH sets lower threshold of array size which might cross paging
boundary(ies). All arrays below this threshold can use normal accessors.
Above this threshold, must use paged-array accessors.

Set up pyramidal array for mem_context

    mem_context --> mem_open_nest
                --> mem_close_nest
    
    Use these for semi-automatic memory mgmt



Accessors
---------

Core:  
rd(be,kind,thing,index,result)      --> read  
wr(be,kind,thing,index,payload)     --> write  

Extended:  
rm(be,kind,thing,index)             --> remove  
ex(be,kind,thing,index)             --> test existence  
ls(be,kind,thing)                   --> list (keys)  

Kinds:
- val
- ptr
- tag
- tptr
- can be extended

Examples:

    access_api_rd_val(pc,x,y,MWORD_ASIZE)
    access_api_wr_val(pc,x,y,MWORD_ASIZE)
    access_api_rd_ptr(pc,x,y,MWORD_ASIZE)
    ...

Babel Environment
-----------------

    struct{
        // mem_context
        // static_env (sap)
        // dynamic_env (map)
    } babel_env;



BUGS
====



