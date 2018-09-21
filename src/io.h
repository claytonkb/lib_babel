// io.h
// 

#ifndef IO_H
#define IO_H

//FILE *io_open_file(babel_env *be, mword *filename);
FILE *io_open_file(babel_env *be, mword *filename, char *attr);
mword io_file_size(babel_env *be, FILE *file);
babel_env *io_close_file(babel_env *be, FILE *file);
mword *io_slurp8(babel_env *be, char *filename);
mword *io_slurp(babel_env *be, char *filename);
void io_spit(babel_env *be, char *filename, mword *fileout, access_size asize, fileout_type ft);

#endif //IO_H

// Clayton Bauman 2017

