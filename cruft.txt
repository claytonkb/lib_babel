
//                util_strtok_dec(cmd_code_str, cmd_code);
//                util_strtok_dec(cmd_code_str, cmd_code2);
//
//                       ACC = _mkval(this_pyr, 4, 0xf0f0f0f0, 0x0f0f0f0f, 0xf0f0f0f0, 0x0f0f0f0f);
//                mword *src = _mkval(this_pyr, 4, 0x77777777, 0x99999999, 0xbbbbbbbb, 0xdddddddd);
//
//                array1_move(this_pyr, 
//                        ACC,
//                        cmd_code, 
//                        src,
//                        cmd_code2);

//                temp = _mkls(this_pyr, 3, _val(this_pyr, 0xdeadbabe), _val(this_pyr, 0xbeefface), _val(this_pyr, 0xabadcafe));
//                ACC = bstruct_cp(this_pyr, temp);

//                ACC = sexpr_from_string(this_pyr, C2B("[val 1 2 3]\n"));

//                ACC = _mk_aop(this_pyr, 2,
//                        _val(this_pyr, 0xdeadbabe),
//                        _val(this_pyr, 0xbeefface),
//                        _val(this_pyr, 0xcafefade),
//                        _val(this_pyr, 0xabaddeed));

//                ACC = global_irt->tags->PYR_TAG_TRIE_ENTRY;
//                ACC = global_irt->tags->PYR_TAG_TRIE;

//                ACC = global_irt->fs;
//                trie_insert(this_pyr, global_irt->fs, HASH8(this_pyr, "foo"), nil, _val(this_pyr, 0x01234567));
//                trie_insert(this_pyr, global_irt->fs, HASH8(this_pyr, "bar"), nil, _val(this_pyr, 0x89abcdef));
//                trie_insert(this_pyr, global_irt->fs, HASH8(this_pyr, "baz"), nil, _val(this_pyr, 0x00112233));
//                trie_insert(this_pyr, global_irt->fs, HASH8(this_pyr, "aaa"), nil, _val(this_pyr, 0x44556677));
//                trie_insert(this_pyr, global_irt->fs, HASH8(this_pyr, "aab"), nil, _val(this_pyr, 0x8899aabb));
//                trie_insert(this_pyr, global_irt->fs, HASH8(this_pyr, "aac"), nil, _val(this_pyr, 0xccddeeff));
//                trie_insert(this_pyr, global_irt->fs, HASH8(this_pyr, "aad"), nil, _val(this_pyr, 0x01012323));
//                trie_insert(this_pyr, global_irt->fs, HASH8(this_pyr, "aae"), nil, _val(this_pyr, 0x45456767));

//                map_insert(global_irt->fs, "foo", _val(this_pyr, 0x01234567));
//                map_insert(global_irt->fs, "bar", _val(this_pyr, 0x89abcdef));
//                map_insert(global_irt->fs, "baz", _val(this_pyr, 0x00112233));
//                map_insert(global_irt->fs, "aaa", _val(this_pyr, 0x44556677));
//                map_insert(global_irt->fs, "aab", _val(this_pyr, 0x8899aabb));
//                map_insert(global_irt->fs, "aac", _val(this_pyr, 0xccddeeff));
//                map_insert(global_irt->fs, "aad", _val(this_pyr, 0x01012323));
//                map_insert(global_irt->fs, "aae", _val(this_pyr, 0x45456767));

//                trie_remove(this_pyr, global_irt->fs, HASH8(this_pyr, "aac"), nil);

//                trie_remove(this_pyr, global_irt->fs, HASH8(this_pyr, "aad"), nil);

//                trie_remove(this_pyr, global_irt->fs, HASH8(this_pyr, "foo"), nil);
//                trie_remove(this_pyr, global_irt->fs, nil,                    string_c2b(this_pyr, "foo", 100));
//                ACC = _val(this_pyr, 
//                        trie_exists(this_pyr, global_irt->fs, HASH8(this_pyr, "bap"), nil));

//#define trie_exists(pyr, trie, key, secondary_key) (!is_nil(trie_lookup_hash(pyr, trie, key, secondary_key))) // trie_exists#

//   result0 = _load(this_bvm, oi0.data, size(oi0.data)); 

//                temp = io_slurp8(this_pyr, "aop.bbl");
//                temp = bstruct_load(this_pyr, temp, size(temp));
//
//                array_sort(this_pyr, temp, LEX_BYTE);

//                ACC = temp;
//
//                ACC = _val(this_pyr,
//                        array_search(this_pyr, temp, C2B("iguana"), LEX_BYTE));

//mword array_search(pyr_cache *this_pyr, mword *array, mword *target, sort_type st){ // array_search#

//                array_build_max_heap(ACC);

//                temp  = _mkval(this_pyr, 4, 3, 1, 0, 0);
//                temp2 = _mkval(this_pyr, 7, 2, 1, 0, 0, 0, 0, 0);
////                temp2 = _val(this_pyr, 3);
//_mem(temp);
//_say("-------");
//_mem(temp2);
//_say("-------");
//                ACC = _val(this_pyr, array_cmp_num(temp, temp2));

//                temp = io_slurp8(this_pyr, "presidents.bbl");
//                temp = bstruct_load(this_pyr, temp, size(temp));
//
////                ACC = bstruct_cp(this_pyr, temp);
//                ACC = list_to_ptr_array(this_pyr, trie_entries(this_pyr, temp));
//                array_sort(this_pyr, ACC, UNSIGNED);

//                ACC = global_irt->symbols->SEXPR_IGNORE_SYMBOL;

//                ACC = _val(this_pyr,
//                        array_ncmp(this_pyr, C2B("foobaz"), 3, C2B("bar"), 2, BYTE_ASIZE));

//                tempv = _mu(this_pyr, global_irt->env);
//                _prn("_mu: ");
//                _dd(tempv);
//
//                tempv = _nar(this_pyr, global_irt->env);
//                _prn("_nar: ");
//                _dd(tempv);
//
//                tempv = _nin(this_pyr, global_irt->env);
//                _prn("_nin: ");
//                _dd(tempv);
//
//                tempv = _nlf(this_pyr, global_irt->env);
//                _prn("_nlf: ");
//                _dd(tempv);
//
//                tempv = _ntag(this_pyr, global_irt->env);
//                _prn("_ntag: ");
//                _dd(tempv);
//
//                tempv = _nva(this_pyr, global_irt->env);
//                _prn("_nva: ");
//                _dd(tempv);
//
//                tempv = _nptr(this_pyr, global_irt->env);
//                _prn("_nptr: ");
//                _dd(tempv);

//                ACC = bstruct_to_array(this_pyr, global_irt->env);

//                ACC = global_irt->tags->PYR_TAG_CPTR;
//                temp = mem_new_tptr(this_pyr, global_irt->tags->PYR_TAG_CPTR, nil);
//                ACC = _val(this_pyr, is_cptr(temp));

//                ACC = _mk_aop(this_pyr, 2,
//                        mem_new_tptr(this_pyr, global_irt->tags->PYR_TAG_CPTR, _val(this_pyr, 0xdeadbeef)),
//                        _val(this_pyr, 0xdeadbabe),
//                        mem_new_tptr(this_pyr, global_irt->tags->PYR_TAG_OPERAND, nil),
//                        _val(this_pyr, 0xbeefface),
//                        _val(this_pyr, 0xcafefade),
//                        _val(this_pyr, 0xabaddeed));

//                ACC = global_irt->tags_strings;

//                ACC = _val(this_pyr,
//                        array_search(this_pyr, global_irt->tags_strings, global_irt->tags->PYR_TAG_CPTR, LEX_MWORD));

//                tempv = array_search(this_pyr, global_irt->tags_strings, global_irt->tags->PYR_TAG_CPTR, LEX_MWORD);
//                ACC = pcar( rdp(global_irt->tags_strings, tempv) );
//
//                _mem(ACC);
//                _say("------------------------------------");
//                _mem(global_irt->tags->PYR_TAG_CPTR);

//                ACC = xbar_tag_to_string(this_pyr, global_irt->tags->PYR_TAG_CPTR);

//                ACC = global_irt->env;

//                ACC = this_pyr->self;

//                num_tags = size(global_irt->tags);
//
//                for(i=0; i<num_tags; i++){
////                    tempv = array_search_rewrite(this_pyr, global_irt->tags_strings, rdp(global_irt->tags,i), LEX_MWORD);
//                    tempv = array_search(this_pyr, global_irt->tags_strings, rdp(global_irt->tags,i), LEX_MWORD);
////                    _mem(rdp(global_irt->tags,i));
////                    _say("---------------------");
//                    if(tempv!=-1){
////                        _mem(pcar(rdp(global_irt->tags_strings,tempv)));
//                        found_count++;
//                    }
////                    _say("/////////////////////");
//                }
//
//                _dd(num_tags);
//                _dd(found_count);

//                fprintf(stderr, "%d\n", sizeof(unsigned));
//                fprintf(stderr, "%d\n", sizeof(double));
//
//                double foo = 12345.6789;
//                fprintf(stderr, "%lf\n", foo);

//                ACC = global_irt->xbar;

//                ACC = io_slurp8(this_pyr, "code_block.bbl");
//                ACC = bstruct_load(this_pyr, ACC, size(ACC));
//                _say("code_block.bbl loaded");
//
//                pvc_core_interp(this_pyr, tcar(ACC));
//                _say("done");

//                ACC = io_slurp8(this_pyr, "paged_array.bbl");
//                ACC = bstruct_load(this_pyr, ACC, size(ACC));
//                _say("paged_array.bbl loaded");

//                ACC = io_slurp8(this_pyr, "root.bbl");
//                ACC = bstruct_load(this_pyr, ACC, size(ACC));
//                _say("root.bbl loaded");
                ACC = io_slurp8(this_pyr, "tree.bbl");
                ACC = bstruct_load(this_pyr, ACC, size(ACC));
                _say("tree.bbl loaded");

#define sfield_pa(base)  (vcar(rdp(base,0)))
#define pgsize_pa(base)  (vcar(rdp(base,1)))
#define arrays_pa(base)       (rdp(base,2))
#define rdv_pa(base,offset) (rdv(rdp(arrays_pa(base),(offset/pgsize_pa(base))),(offset%pgsize_pa(base))))
#define ldv_pa(base,offset) (ldv(rdp(arrays_pa(base),(offset/pgsize_pa(base))),(offset%pgsize_pa(base))))
#define rdp_pa(base,offset) (rdp(rdp(arrays_pa(base),(offset/pgsize_pa(base))),(offset%pgsize_pa(base))))
#define ldp_pa(base,offset) (ldp(rdp(arrays_pa(base),(offset/pgsize_pa(base))),(offset%pgsize_pa(base))))

// std_new_pa() <-- creates a new paged_array

//_dd(pgsize_pa(ACC));
//_dd(sfield_pa(ACC));



//                ACC = arrays_pa(ACC);

//                tempv = rdv_pa(ACC,9);
//                ACC = _val(this_pyr, tempv);


// Clayton Bauman 2017

