TODO
====

*BUGS AT BOTTOM*

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

Migration
---------

*Edits*

    %s/pyr_cache \*this_pyr/babel_env *be/g

* There is no global_irt
* nil?
* mem_new_\* ?

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
  
Bstruct  
Array  
List  
Trie  
...  

DONEISH
=======

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



