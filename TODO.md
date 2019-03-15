TODO
====

*BUGS AT BOTTOM*

Migration
---------

*TOS*

    break everything!

    get make.pl up to snuff
        get .last_built working
        get entr working while we're at it...

    sexpr --> aexpr
        Need to convert s-expressions into nested aop (not sap)
        Temporary mem usage... :/
        Babel/bpdl.c --> [bs ...]

    val-based predicates
        is_val_pred(x) (x > 0)
        is_ptr_pred(x) (x < 0)
        etc.

    relative-offset accessors
        rdval_rel(bs,x) bs[x]
        access-size-specific versions
        etc.

    sap
        add an empty-pair to babel_env to allow sorting of sparse sap's
        augment is_nil() test in sap_find_index* to include is_empty_pair()

        empty_pair --> [ptr be->zero_hash 0]

    flat heap
        implemented as a ptr-array of size 2*N for heap of size N. Flat binary tree
        indexing used to perform operations on heap. Entries in the ptr-array alternate
        between priority and payload (default payload is a pair).

    pds
        linear probing (lossless)
        lossy array
        bloom filter
        cms

    all functions must have the following modes:
        non-allocating supervisor (REQUIRED)
        allocating supervisor (optional)
        non-allocating user
        allocating user

    re-architect the memory manager (ugh!)
        Don't get carried away
        Flat pyramid
        Flat allocator, page acquire/release, stop+copy GC
        Portable mem_env (encapsulate mem_env to allow context-switching)

    round-trippable bstructs
        bstruct_load_str() --> implementation can wait, but stub it in  

    basic graph support

    canonical bstructs
        Rivest's s-expression format:
        people.csail.mit.edu/rivest/Sexp.txt

        Convert bstruct to canonical s-expression, then convert to Rivest s-exp

    type-format checking
        get tagged bstruct and dispatch to relevant format-checker
        checker does not *certify* type, only detects type format violations

        test whether quotes are working in sexpr
        sort out the tags issue (tags.h vs dynamic tags)
        diff ai_ml/pyramid and pyramid; may still need to migrate some functions

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

    NOTE: THIS COMPLICATES CONTEXT-SWITCHING!!!


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



