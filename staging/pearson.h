// pearson.h

#ifndef PEARSON_H
#define PEARSON_H

#define PERM_TABLE_WIDTH 256
#define PERM_TABLE_BYTE_SIZE 4096
#define PERM_TABLE_MWORD_SIZE (PERM_TABLE_BYTE_SIZE/MWORD_SIZE)

//extern const char pearson16_byte_perm[4096];
extern const char pearson16_byte_perm[PERM_TABLE_BYTE_SIZE];

mword *pearson_hash8(babel_env *be, mword *key);
mword *pearson_hash(babel_env *be, mword *key);
mword *pearson_hash8p(babel_env *be, char *base_hash, mword *key);

void pearson16(char *result, char *init, char *key, mword strlen);
void pearson_marsaglia16(char *result, char* init, char *key, mword strlen);

mword *pearson16a(babel_env *be, char *init, char *key, mword strlen);
mword *pearson_marsaglia16a(babel_env *be, char *init, char *key, mword strlen);

mword  pearson_marsaglia_fuzz(mword reset);
void pearson_empty_hash(char *result);

//mword *pearson_rand_val(babel_env *be, mword num_mwords);

#endif //PEARSON_H

//Clayton Bauman 2016

