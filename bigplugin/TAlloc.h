/*  TAlloc.h
 *  Header file with functions for data allocations and deallocations
 *  Jiri Filip, UTIA CAS CR, filipj"at"utia.cas.cz
 */

#ifndef TAlloc_c
#define TAlloc_c

//allocations
float* allocation1(int nrl,int nrh);
int* iallocation1(int nrl,int nrh);
char* callocation1(int nrl,int nrh);
unsigned char* uallocation1(int nrl,int nrh);
unsigned char* ballocation1(int nrl,int nrh);
float** allocation2(int nrl,int nrh,int ncl,int nch);
int** iallocation2(int nrl,int nrh,int ncl,int nch);
unsigned short** sallocation2(int nrl,int nrh,int ncl,int nch);
unsigned int** uallocation2(int nrl,int nrh,int ncl,int nch);
char** callocation2(int nrl,int nrh,int ncl,int nch);
unsigned char** ballocation2(int nrl,int nrh,int ncl,int nch);
long** lallocation2(int nrl,int nrh,int ncl,int nch);
float*** allocation3(int nmf,int nml,int nrl,int nrh,int ncl,int nch);
int*** iallocation3(int l1,int h1,int l2,int h2,int l3,int h3);
unsigned char*** ballocation3(int l1,int h1,int l2,int h2,int l3,int h3);
float**** allocation4(int ntf, int ntl, int nmf,int nml,int nrl,int nrh,int ncl,int nch);
unsigned char**** ballocation4(int ntf, int ntl, int nmf,int nml,int nrl,int nrh,int ncl,int nch);
//deallocations
void freemem1(float *m,int nrl,int nrh);
void freeimem1(int *m,int nrl,int nrh);
void freecmem1(char *m,int nrl,int nrh);
void freeumem1(unsigned char *m,int nrl,int nrh);
void freebmem1(unsigned char *m,int nrl,int nrh);
void freemem2(float **m,int nrl,int nrh,int ncl,int nch);
void freeimem2(int **m,int nrl,int nrh,int ncl,int nch);
void freesmem2(unsigned short **m,int nrl,int nrh,int ncl,int nch);
void freeumem2(unsigned int **m,int nrl,int nrh,int ncl,int nch);
void freecmem2(char **m,int nrl,int nrh,int ncl,int nch);
void freebmem2(unsigned char **m,int nrl,int nrh,int ncl,int nch);
void freelmem2(long **m,int nrl,int nrh,int ncl,int nch);
void freemem3(float ***m,int nmf,int nml,int nrl,int nrh,int ncl,int nch);
void freeimem3(int ***m,int l1,int h1,int l2,int h2,int l3,int h3);
void freebmem3(unsigned char ***m,int l1,int h1,int l2,int h2,int l3,int h3);
void freemem4(float ****m,int ntf, int ntl, int nmf,int nml,int nrl,int nrh,int ncl,int nch);
void freebmem4(unsigned char ****m,int ntf, int ntl, int nmf,int nml,int nrl,int nrh,int ncl,int nch);

#endif
