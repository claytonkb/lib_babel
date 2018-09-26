// pearson.h

#ifndef PEARSON_H
#define PEARSON_H

#include <stdint.h>

#define PERM_TABLE_SIZE 256

void pearson128(hash result, const bstruct init, const char *key, const unsigned strlen);
hash pearson_hash8(babel_env *be, const bstruct init, const char *key, const unsigned strlen);
static const char pearson_perm[PERM_TABLE_SIZE];

#endif //PEARSON_H

//Clayton Bauman 2018

