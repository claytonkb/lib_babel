// approx.h
// 

#ifndef APPROX_H
#define APPROX_H

mword approx_lookup(mword *sap, mword *key, access_size asize);
mword approx_lookup_scaled(mword *sap, mword size, mword *key, access_size asize);
void approx_update(mword *sap, mword *key, mword *payload, access_size asize);
void approx_update_scaled(mword *sap, mword size, mword *key, mword *payload, access_size asize);
mword approx_find_index(mword *key, mword sap_size);


#endif //APPROX_H

// Clayton Bauman 2018

