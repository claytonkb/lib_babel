// io.c
//

#include "babel.h"
#include "io.h"
#include "mem.h"
#include "array.h"

// If file-size < MMAP_FILE_THRESH
//      slurp file
// Else
//      mmap() the file
//
// MMAP_FILE_THRESH should probably default to GC_FAST_ALLOC_THRESH
//      In other words, a file big enough that we would have to stop using
//      regular GC to manage it is too big to slurp. In that case, we should
//      just memory-map the file and let the user interact with it as they
//      would any dynamic memory allocation.
//

//
//
FILE *io_open_file(babel_env *be, mword *filename, char *attr){ // io_open_file#

    FILE*  file;

    file = fopen((char*)filename, attr);

    // FIXME(_fatal)
    if(file==NULL)
        _fatal((char*)filename);

    return file;

}


//
//
mword io_file_size(babel_env *be, FILE *file){ // io_file_size#

    fseek(file, 0L, SEEK_END);
    mword size = ftell(file);
    rewind(file);

    return size;

}


//
//
babel_env *io_close_file(babel_env *be, FILE *file){ // io_close_file#

    fclose(file); // Return value can be checked to see if there were any issues closing the file...

    // XXX un-register the opened file in global_irt

    return be;

}


//
//
mword *io_slurp8(babel_env *be, char *filename){ // io_slurp8#

    FILE *f = io_open_file(be, (mword*)filename, "rb");
    mword file_size = io_file_size(be, f);

    mword *file_buffer = mem_new_str(be, file_size, ' ');
    fread((char*)file_buffer, 1, file_size, f);

    io_close_file(be, f);

    return file_buffer;

}


// FIXME: array_truc() commented out
//
mword *io_slurp(babel_env *be, char *filename){ // io_slurp#

    mword *result = io_slurp8(be, filename);
//    array_trunc(be, result, size(result)-1);
    array_shrink(be, result, 0, size(result)-1, MWORD_ASIZE);

    return result;

}


//
//
void io_spit(babel_env *be, char *filename, mword *fileout, access_size asize, fileout_type ft){ // io_spit#

    FILE *pFile;

    mword filesize;

    if(asize == MWORD_ASIZE){
        filesize = UNITS_MTO8(size(fileout));
    }
    else{ //asize == BYTE_ASIZE
        filesize = array8_size(fileout);
    }

    if(ft == OVERWRITE){
        pFile = io_open_file(be, (mword*)filename, "wb");
    }
    else{ // ft = APPEND
        pFile = io_open_file(be, (mword*)filename, "ab");
    }

    fwrite(fileout, 1, filesize, pFile);

    fclose(pFile);

}


// stdin, stdout, stderr, stdscr (requires linking with curses)
// Babelized fopen, read, seek, etc. (memory-mapped files)

////
////
//mword *_stdinln(bvm_cache *this_bvm, FILE *stream){ // _stdinln#
//
//    int c, i=0;
//    char buffer[(1<<16)]; //FIXME: make buffer size an argument to this function
//
//    while(1){ //FIXME unsafe, wrong
//        c = fgetc(stream);
//        if(c == EOF || c == '\n'){
//            break;
//        }
//        buffer[i] = c;
//        i++;
//    }
//
//    mword file_mword_size = _array8_size(this_bvm,i);
//
//    mword *result = _newlfi(this_bvm, file_mword_size, 0);
//
//    memcpy(result, buffer, i); // XXX WAIVER(memcpy) XXX //
//
//    lcl(result, file_mword_size-1) = _alignment_word8(this_bvm, i);
//
//    return result;
//
//}
//
//
////
////
//void _output8(bvm_cache *this_bvm, mword *string, FILE *stream){ // _output8#
//
//    if(!is_nil(string)){
//        fprintf(stream, (char*)string);
//    }
//
//}
//
//
////
////
//void _journal(bvm_cache *this_bvm, mword *filename, mword *fileout, mword access_size){ // _journal#
//
//    FILE *pFile;
//
//    mword filesize;
//
//    if(access_size == ACCESS_SIZE_MWORD){
//        filesize = size(fileout) * MWORD_SIZE;
//    }
//    else{ //access_size == ACCESS_SIZE_BYTE
//        filesize = _arlen8(this_bvm, fileout);
//    }
//
//    pFile = fopen((char*)filename, "ab");
//
//    if(pFile==NULL){
//        _fatal("file error");
//    }
//
//    fwrite(fileout, 1, filesize, pFile);
//
//    fclose(pFile);
//
//}
//
//
//

// Clayton Bauman 2018

