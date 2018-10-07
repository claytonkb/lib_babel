// sap.h
// 

#ifndef SAP_H
#define SAP_H

#define SAP_LINEAR_THRESH  128
#define SAP_LINEAR_HOLDOFF 3

void   sap_overwrite(babel_env *be, mword *sap, mword *key, mword *payload, search_type s, sort_type st);
mword *sap_lookup(babel_env *be, mword *sap, mword *key, search_type s, sort_type st);
mword  sap_remove(babel_env *be, mword *sap, mword *key, search_type s, sort_type st);
mword *sap_entries(babel_env *be, mword *sap);
mword  sap_find_index(babel_env *be, mword *sap, mword *key, search_type s, sort_type st);
mword  sap_find_index_linear(babel_env *be, mword *sap, mword start, mword end, mword *key, sort_type st);
mword  sap_find_index_binary(babel_env *be, mword *sap, mword *key, sort_type st);
mword  sap_find_index_probe(babel_env *be, mword *sap, mword *key);


#endif //SAP_H

// Clayton Bauman 2018

