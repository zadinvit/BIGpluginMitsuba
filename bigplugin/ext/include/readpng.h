/*  readpng.cpp
 *  This program contains PNG-reading functions 
 *  Jiri Filip, 18.2.2004
 */

#ifndef readpng_c
#define readpng_c

//#define IMGIO_FILE_NOT_OPENED -1
//#define IMGIO_NOT_A_PNG -2
//#define IMGIO_ALLOCATION_PROBLEM -3
//#define IMGIO_FILE_READ_PROBLEM -4
//#define IMGIO_FILE_WRITE_PROBLEM -5
//#define IMGIO_FILE_CONTENTS_PROBLEM -6
//#define IMGIO_WRONG_PARAMETER -7

int read_PNG_header(char *filename, int *planes, int *nr, int *nc);
int read_PNG_file (char * filename,unsigned char *arr,int planes, int nr, int nc, int oneplane);

int read_PNG_file3D (char * filename,float ***arr,int planes, int nr, int nc, int oneplane);

int write_PNG_file (char *filename,float ***arr,int planes, int nr, int nc, int oneplane, int sixteenbit);
int write_PNG_file_1D (char *filename,unsigned char *arr,int planes, int nr, int nc, int oneplane);

#endif
