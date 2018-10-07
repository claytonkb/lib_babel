// aop.h
// 

#ifndef AOP_H
#define AOP_H

aop  aop_from_trie(babel_env *be, trie t);
trie aop_to_trie(babel_env *be, aop a);
aop aop_from_trie_keys(babel_env *be, trie t);
trie aop_to_trie_keys(babel_env *be, aop a);

bstruct aop_to_array(babel_env *be, aop a);
aop aop_from_array(babel_env *be, arr a, arr b);

aop aop_from_list(babel_env *be, ls a, ls b);
bstruct aop_to_list(babel_env *be, aop a);

aop aop_from_sap(babel_env *be, sap a);
sap aop_to_sap(babel_env *be, aop a, sort_type st);

void *aop_select_cmp_fn(sort_type st);

#endif //AOP_H

// Clayton Bauman 2018

