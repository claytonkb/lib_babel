// sap.h
// 

#ifndef SAP_H
#define SAP_H

#define SAP_LINEAR_THRESH  128
#define SAP_LINEAR_HOLDOFF 3
#define SAP_PROBE_REGION_DENOM 64

mword *sap_lookup(babel_env *be, mword *sap, mword *key, search_type s, sort_type st);
mword  sap_update(babel_env *be, mword *sap, mword *key, mword *payload, search_type s, sort_type st);
mword  sap_insert(babel_env *be, mword *sap, mword *key, mword *payload);
mword  sap_remove(babel_env *be, mword *sap, mword *key);

mword  sap_find_index(babel_env *be, mword *sap, mword *key, search_type s, sort_type st);
mword  sap_find_index_linear(babel_env *be, mword *sap, mword start, mword end, mword *key, sort_type st);
mword  sap_find_index_binary(babel_env *be, mword *sap, mword *key, sort_type st);
mword  sap_find_index_probe(babel_env *be, mword *sap, mword *key);
mword  sap_find_empty_index(babel_env *be, mword *sap, mword *key);


#endif //SAP_H

// Clayton Bauman 2018

