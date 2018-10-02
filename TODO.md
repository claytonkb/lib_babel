TODO
====

*BUGS AT BOTTOM*

Migration
---------

**NOTE**: \*.c files are from '16 pyramid; update to ai_ml/pyramid ('18)  
**NOTE**: will need to re-migrate accordingly

*TOS*

    un-crapify mem_alloc so that it uses be->paging_base
        mem_context_expand() implemented -- TEST it
        implement frame_list 
        when closing a mem_frame, allow to specify a pointer to
            save from deallocation
        this pointer gets "bstruct_to_array()" called to ensure that none of
            its children get de-allocated (wait, this is circular!)
        de-allocation proceeds backward toward the snapshot point
    aop/sap  
    lossy
    vvvvv mid priority
        basic graph support
        test whether quotes are working in sexpr
        sort out the tags issue (tags.h vs dynamic tags)
        diff ai_ml/pyramid and pyramid; may still need to migrate some functions
        bstruct_load_str() --> implementation can wait, but stub it in  
            round-trippable bstructs

Will call this "lib_babel 0.5"; ready for external use  

*Edits*

    %s/pyr_cache \*this_pyr/babel_env *be/g
    %s/\<pc\>/be/g
    %s/\<this_pyr\>/be/g

    %s/access_size/asize/g
    %s/access_size_sel/access_size/g

    sed -i -e 's/\baccess_size\b/asize/g' array.h
    sed -i -e 's/\baccess_size_sel\b/access_size/g' array.h

    sed -i -e 's/pyr_cache \*this_pyr/babel_env *be/g' *


DONEISH
=======

sort re-work
------------

        switch to qsort (factor out merge_sort cruft)

        cmp_aop_*
            array_cmp_*_bstruct()
                array_cmp_*_range()

            int array_cmp_num(mword *left, mword *right);
            int array_cmp_num_range(mword *left, mword *left_end, mword *right, mword *right_end);

            int array_ncmp(babel_env *be, mword *left, mword left_offset, mword *right, mword length, access_size asize);

        
        array_cmp_lex_range(left_begin,left_end,right_begin,right_end)
            left  range: |---****-------|
            right range: |--****----|

        array_cmp_lex_bstruct(bstruct left, bstruct right)
            array_cmp_lex_range(left,left+size(left)-1,right,right+size(right)-1)

        NOTE: bake in reuse of *cmp* fn's for linear probing


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

    introspect_gv not rendering tags correctly


