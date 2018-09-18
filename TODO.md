TODO
====

*BUGS AT BOTTOM*

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

Babel Context
-------------

    struct{
        // mem_context
        // static_env (sap)
        // dynamic_env (map)
    } babel_env;

Paging/GC
---------

    #define PA_LO_THRESH (1<<20)

PA_LO_THRESH sets lower threshold of array size which might cross paging
boundary(ies). All arrays below this threshold can use normal accessors.
Above this threshold, must use paged-array accessors.

Set up pyramidal array for mem_context

Migration
---------

Bstruct
Array
List
Trie
...

BUGS
====



