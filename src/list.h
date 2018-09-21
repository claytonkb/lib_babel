// list.h
// 

#ifndef LIST_H
#define LIST_H

#define _new_cons(x)  (_newin(x, 2))             // _new_cons#
#define _new_dcons(x) (_newin(x, 3))             // _new_dcons#

#define CDR_DIRECTION 0
#define CPR_DIRECTION 1

bstruct list_to_map(babel_env *be, bstruct lists);

mword *list_insert(babel_env *be, mword *src_list, mword *dest_list);
mword *list_unshift(babel_env *be, mword *list, mword *bs);
mword *list_shift(babel_env *be, mword *list);
void   list_push(babel_env *be, mword *list, mword *bs);
mword *list_pop(babel_env *be, mword *list);
mword *list_find_end(babel_env *be, mword *list);
mword *list_find_next_to_end(babel_env *be, mword *list);
mword  list_len(babel_env *be, mword *list);
mword *list_to_ptr_array(babel_env *be, mword *list);
mword *list_to_val_array(babel_env *be, mword *list);
mword *list_cdri(babel_env *be, mword *list, mword i);
mword *list_ith(babel_env *be, mword *list, mword i);
mword *list_reverse(babel_env *be, mword *list, mword *new_cdr);
mword *list_split(babel_env *be, mword *list, mword *indices);
mword *list_split_r(babel_env *be, mword *list, mword *indices, mword count);
mword *list_cut(babel_env *be, mword *list, mword index);
mword *list_append(babel_env *be, mword *lists);
mword *list_append_direct(babel_env *be, mword *head_list, mword *tail_list);

mword *dlist_append_direct(babel_env *be, mword *head_list, mword *tail_list);
mword *dlist_insert(babel_env *be, mword *src_list, mword *dest_list);
mword *dlist_insert_prev(babel_env *be, mword *src_list, mword *dest_list);
mword *dlist_find_end(babel_env *be, mword *list);
mword  dlist_len(babel_env *be, mword *list);
mword *dlist_reverse(babel_env *be, mword *list, mword *head, mword direction);
mword *dlist_cut(babel_env *be, mword *list, mword index, mword direction);

// pyr_op's
bstruct list_append_pyr_op(babel_env *be, bstruct lists);

#endif //LIST_H

// Clayton Bauman 2013

