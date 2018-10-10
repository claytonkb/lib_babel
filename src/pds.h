// pds.h
// 

#ifndef PDS_H
#define PDS_H

mword pds_la_lookup(mword *sap, mword *key, access_size asize);
mword pds_la_lookup_scaled(mword *sap, mword size, mword *key, access_size asize);
void  pds_la_update(mword *sap, mword *key, mword *payload, access_size asize);
void  pds_la_update_scaled(mword *sap, mword size, mword *key, mword *payload, access_size asize);
mword pds_la_find_index(mword *key, mword sap_size);


#endif //PDS_H

// Clayton Bauman 2018

