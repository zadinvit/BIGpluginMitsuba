/*!**************************************************************************
\file    TAlloc.cpp
\author  Jiri Filip
\date    April 2004
\version 1.00

  \brief Routines for data allocation and deallocation          

</em>
******************************************************************************/


#include <stdio.h>
#include <stdlib.h> 

//################################################################################
//# ALLOCATIONS                                                                 #
//################################################################################

float* allocation1(int nrl,int nrh){
  //! memory allocation, float vector [nrl..nrh] 
  float *m;
  if(nrh-nrl+1<=0) return NULL;

  m=(float*)calloc((unsigned)(nrh-nrl+1),sizeof(float));
  m -=nrl;

  return m;
} // allocation1 --------------------------------------------- 

int* iallocation1(int nrl,int nrh){
  //! memory allocation, float vector [nrl..nrh] 
  int *m;
  if(nrh-nrl+1<=0) return NULL;

  m=(int*)calloc((unsigned)(nrh-nrl+1),sizeof(int));
  m -=nrl;

  return m;
} // iallocation1 --------------------------------------------- 

char* callocation1(int nrl,int nrh){
  //! memory allocation, float vector [nrl..nrh] 
  char *m;
  if(nrh-nrl+1<=0) return NULL;

  m=(char*)calloc((unsigned)(nrh-nrl+1),sizeof(char));
  m -=nrl;

  return m;
} // callocation1 --------------------------------------------- 

unsigned char* ballocation1(int nrl,int nrh){
  //! memory allocation, float vector [nrl..nrh] 
  unsigned char *m;
  if(nrh-nrl+1<=0) return NULL;

  m=(unsigned char*)calloc((unsigned)(nrh-nrl+1),sizeof(unsigned char));
  m -=nrl;

  return m;
} // ballocation1 --------------------------------------------- 

float** allocation2(int nrl,int nrh,int ncl,int nch){
  //! memory allocation, float matrix [nrl..nrh][ncl..nch 
  int j;
  float **m;
  if((nrh-nrl+1<=0)||(nch-ncl+1<=0)) return NULL;

  m=(float**)calloc((unsigned)(nrh-nrl+1),sizeof(float*));
  if (!m) {printf(" allocation failure 1 in allocation2() \n");}
  m -=nrl;

  for(j=nrl;j<=nrh;j++){
    m[j]=(float*)calloc((unsigned)(nch-ncl+1),sizeof(float));
    if (!m[j]) {printf(" allocation failure 2 in allocation2() \n");}
    m[j] -=ncl;
  }
  return m;
} // allocation2 ----------------------------------------------- 

int** iallocation2(int nrl,int nrh,int ncl,int nch){
  //! memory allocation, int matrix [nrl..nrh][ncl..nch] 
  int j;
  int **m;
  if((nrh-nrl+1<=0)||(nch-ncl+1<=0)) return NULL;

  nch=nch;
  m=(int**)calloc((unsigned)(nrh-nrl+1),sizeof(int*));
  if (!m) {printf(" allocation failure 1 in iallocation2\n");}
  m -=nrl;

  for(j=nrl;j<=nrh;j++){
    m[j]=(int*)calloc((unsigned)(nch-ncl+1),sizeof(int));
    if (!m[j]) {printf(" allocation failure 2 in iallocation2\n");}
    m[j] -=ncl;
  }
  return m;
} // iallocation2 ----------------------------------------------- 

unsigned short** sallocation2(int nrl,int nrh,int ncl,int nch){
  //! memory allocation, int matrix [nrl..nrh][ncl..nch] 
  int j;
  unsigned short **m;
  if((nrh-nrl+1<=0)||(nch-ncl+1<=0)) return NULL;

  nch=nch;
  m=(unsigned short**)calloc((unsigned)(nrh-nrl+1),sizeof(unsigned short*));
  if (!m) {printf(" allocation failure 1 in sallocation2\n");}
  m -=nrl;

  for(j=nrl;j<=nrh;j++){
    m[j]=(unsigned short*)calloc((unsigned)(nch-ncl+1),sizeof(unsigned short));
    if (!m[j]) {printf(" allocation failure 2 in sallocation2\n");}
    m[j] -=ncl;
  }
  return m;
} // sallocation2 ----------------------------------------------- 

unsigned int** uallocation2(int nrl,int nrh,int ncl,int nch){
  //! memory allocation, int matrix [nrl..nrh][ncl..nch] 
  int j;
  unsigned int **m;
  if((nrh-nrl+1<=0)||(nch-ncl+1<=0)) return NULL;

  nch=nch;
  m=(unsigned int**)calloc((unsigned)(nrh-nrl+1),sizeof(unsigned int*));
  if (!m) {printf(" allocation failure 1 in uallocation2\n");}
  m -=nrl;

  for(j=nrl;j<=nrh;j++){
    m[j]=(unsigned int*)calloc((unsigned)(nch-ncl+1),sizeof(unsigned int));
    if (!m[j]) {printf(" allocation failure 2 in uallocation2\n");}
    m[j] -=ncl;
  }
  return m;
} // uallocation2 ----------------------------------------------- 

char** callocation2(int nrl,int nrh,int ncl,int nch){
  //! memory allocation, float matrix [nrl..nrh][ncl..nch 
  int j;
  char **m;
  if((nrh-nrl+1<=0)||(nch-ncl+1<=0)) return NULL;

  m=(char**)calloc((unsigned)(nrh-nrl+1),sizeof(char*));
  if (!m) {printf(" allocation failure 1 in callocation2() \n");}
  m -=nrl;

  for(j=nrl;j<=nrh;j++){
    m[j]=(char*)calloc((unsigned)(nch-ncl+1),sizeof(char));
    if (!m[j]) {printf(" allocation failure 2 in callocation2() \n");}
    m[j] -=ncl;
  }
  return m;
} // callocation2 ----------------------------------------------- 

unsigned char** ballocation2(int nrl,int nrh,int ncl,int nch){
  //! memory allocation, float matrix [nrl..nrh][ncl..nch 
  int j;
  unsigned char **m;
  if((nrh-nrl+1<=0)||(nch-ncl+1<=0)) return NULL;

  m=(unsigned char**)calloc((unsigned)(nrh-nrl+1),sizeof(unsigned char*));
  if (!m) {printf(" allocation failure 1 in ballocation2() \n");}
  m -=nrl;

  for(j=nrl;j<=nrh;j++){
    m[j]=(unsigned char*)calloc((unsigned)(nch-ncl+1),sizeof(unsigned char));
    if (!m[j]) {printf(" allocation failure 2 in ballocation2() \n");}
    m[j] -=ncl;
  }
  return m;
} // ballocation2 ----------------------------------------------- 

long** lallocation2(int nrl,int nrh,int ncl,int nch){
  //! memory allocation, float matrix [nrl..nrh][ncl..nch 
  int j;
  long **m;
  if((nrh-nrl+1<=0)||(nch-ncl+1<=0)) return NULL;

  m=(long**)calloc((unsigned)(nrh-nrl+1),sizeof(long*));
  if (!m) {printf(" allocation failure 1 in lallocation2() \n");}
  m -=nrl;

  for(j=nrl;j<=nrh;j++){
    m[j]=(long*)calloc((unsigned)(nch-ncl+1),sizeof(long));
    if (!m[j]) {printf(" allocation failure 2 in lallocation2() \n");}
    m[j] -=ncl;
  }
  return m;
} // lallocation2 ----------------------------------------------- 

float*** allocation3(int nmf,int nml,int nrl,int nrh,int ncl,int nch){
  //! memory allocation for 3 float matrices [nmf..nml][nrl..nrh][ncl..nch] 
  int i,j;
  float ***m;
  if((nml-nmf+1<=0)||(nrh-nrl+1<=0)||(nch-ncl+1<=0)) return NULL;
  // allocate pointers to matrixs 
  m=(float***)calloc((unsigned)(nml-nmf+1),sizeof(float**));
  if (!m) {printf(" allocation failure 1 in allocation3() \n");}
  m -=nmf;

  // allocate pointers to rows 
  for(j=nmf;j<=nml;j++){
    m[j]=(float**)calloc((unsigned)(nrh-nrl+1),sizeof(float*));
    if (!m[j]) {printf(" allocation failure 2 in allocation3() \n");}
    m[j] -=nrl;

    // allocate rows and set pointers to them 
    for (i=nrl;i<=nrh;i++){
      m[j][i]=(float*) calloc((unsigned)(nch-ncl+1),sizeof(float));
      if (!m[j][i]) {printf(" allocation failure 3 in allocation3() \n");}
      m[j][i] -=ncl;
    }
  }
  return m;
} //-- allocation3 --------------------------------------------- 

int*** iallocation3(int l1,int h1,int l2,int h2,int l3,int h3){
  //! memory allocation, int matrix [l1..h1][l2..h2][l3..h3] 
  int j,i;
  int ***m;
  if((h1-l1+1<=0)||(h2-l2+1<=0)||(h3-l3+1<=0)) return NULL;

  m=(int***)calloc((unsigned)(h1-l1+1),sizeof(int**));
  if (!m) {printf(" allocation failure 1 in iallocation3\n");}
  m -=l1;
  for(j=l1;j<=h1;j++){
    m[j]=(int**)calloc((unsigned)(h2-l2+1),sizeof(int*));
    if (!m[j]) {printf(" allocation failure 2 in iallocation3\n");}
    m[j] -=l2;
    for(i=l2;i<=h2;i++){
      m[j][i]=(int*)calloc((unsigned)(h3-l3+1),sizeof(int));
      if (!m[j][i]) {printf(" allocation failure 3 in iallocation3\n");}
      m[j][i] -=l3;
    }
  }
  return m;
} //-- iallocation3 ----------------------------------------------- 

unsigned char*** ballocation3(int l1,int h1,int l2,int h2,int l3,int h3){
  //! memory allocation, int matrix [l1..h1][l2..h2][l3..h3] 
  int j,i;
  unsigned char ***m;
  if((h1-l1+1<=0)||(h2-l2+1<=0)||(h3-l3+1<=0)) return NULL;

  m=(unsigned char***)calloc((unsigned)(h1-l1+1),sizeof(unsigned char**));
  if (!m) {printf(" allocation failure 1 in ballocation3\n");}
  m -=l1;
  for(j=l1;j<=h1;j++){
    m[j]=(unsigned char**)calloc((unsigned)(h2-l2+1),sizeof(unsigned char*));
    if (!m[j]) {printf(" allocation failure 2 in ballocation3\n");}
    m[j] -=l2;
    for(i=l2;i<=h2;i++){
      m[j][i]=(unsigned char*)calloc((unsigned)(h3-l3+1),sizeof(unsigned char));
      if (!m[j][i]) {printf(" allocation failure 3 in ballocation3\n");}
      m[j][i] -=l3;
    }
  }
  return m;
} //-- ballocation3 ----------------------------------------------- 

float**** allocation4(int ntf, int ntl, int nmf,int nml,int nrl,int nrh,int ncl,int nch){
  //! memory allocation for 4 float matrices [ntf .. ntl] [nmf..nml][nrl..nrh][ncl..nch] 

  int i,j,k;
  float ****m;
  if((ntl-ntf+1<=0)||(nml-nmf+1<=0)||(nrh-nrl+1<=0)||(nch-ncl+1<=0)) return NULL;

  // allocate pointers to matrices (time frames) 
  m=(float****)calloc((unsigned)(ntl-ntf+1),sizeof(float***));
  if (!m) {printf(" allocation failure 1 in allocation4() \n");}
  m -=nmf;

  // allocate pointers to spectral planes  
  for(k=ntf;k<=ntl;k++){
    m[k]=(float***)calloc((unsigned)(nml-nmf+1),sizeof(float**));
    if (!m[k]) {printf(" allocation failure 2 in allocation4() \n");}
    m[k] -=nmf;

    // allocate pointers to rows 
    for(j=nmf;j<=nml;j++){
      m[k][j]=(float**)calloc((unsigned)(nrh-nrl+1),sizeof(float*));
      if (!m[k][j]) {printf(" allocation failure 3 in allocation4() \n");}
      m[k][j] -=nrl;

      // allocate rows and set pointers to them 
      for (i=nrl;i<=nrh;i++){
        m[k][j][i]=(float*) calloc((unsigned)(nch-ncl+1),sizeof(float));
        if (!m[k][j][i]) {printf(" allocation failure 4 in allocation4() \n");}
        m[k][j][i] -=ncl;
      }
    }
  }
  return m;
} //-- allocation4 --------------------------------------------- 

unsigned char**** ballocation4(int ntf, int ntl, int nmf,int nml,int nrl,int nrh,int ncl,int nch){
  //! memory allocation for 4 float matrices [ntf .. ntl] [nmf..nml][nrl..nrh][ncl..nch] 

  int i,j,k;
  unsigned char ****m;
  if((ntl-ntf+1<=0)||(nml-nmf+1<=0)||(nrh-nrl+1<=0)||(nch-ncl+1<=0)) return NULL;

  // allocate pointers to matrices (time frames) 
  m=(unsigned char****)calloc((unsigned)(ntl-ntf+1),sizeof(unsigned char***));
  if (!m) {printf(" allocation failure 1 in allocation4() \n");}
  m -=nmf;

  // allocate pointers to spectral planes  
  for(k=ntf;k<=ntl;k++){
    m[k]=(unsigned char***)calloc((unsigned)(nml-nmf+1),sizeof(unsigned char**));
    if (!m[k]) {printf(" allocation failure 2 in allocation4() \n");}
    m[k] -=nmf;

    // allocate pointers to rows 
    for(j=nmf;j<=nml;j++){
      m[k][j]=(unsigned char**)calloc((unsigned)(nrh-nrl+1),sizeof(unsigned char*));
      if (!m[k][j]) {printf(" allocation failure 3 in allocation4() \n");}
      m[k][j] -=nrl;

      // allocate rows and set pointers to them 
      for (i=nrl;i<=nrh;i++){
        m[k][j][i]=(unsigned char*) calloc((unsigned)(nch-ncl+1),sizeof(unsigned char));
        if (!m[k][j][i]) {printf(" allocation failure 4 in allocation4() \n");}
        m[k][j][i] -=ncl;
      }
    }
  }
  return m;
} //-- ballocation4 --------------------------------------------- 

//################################################################################
//# DEALLOCATIONS                                                                 #
//################################################################################

void freemem1(float *m,int nrl,int nrh){
  //! This method frees memory allocated by allocation1 
  nrh=nrh;
  free((float*)(m+nrl));
} // freemem1 ---------------------------------------------------- 

void freeimem1(int *m,int nrl,int nrh){
  //! This method frees memory allocated by allocation1 
  nrh=nrh;
  free((int*)(m+nrl));
} // freemem1 ---------------------------------------------------- 

void freecmem1(char *m,int nrl,int nrh){
  //! This method frees memory allocated by allocation1 
  nrh=nrh;
  free((char*)(m+nrl));
} // freecmem1 ---------------------------------------------------- 

void freebmem1(unsigned char *m,int nrl,int nrh){
  //! This method frees memory allocated by allocation1 
  nrh=nrh;
  free((unsigned char*)(m+nrl));
} // freebmem1 ---------------------------------------------------- 

void freemem2(float **m,int nrl,int nrh,int ncl,int nch){
  //! frees memory allocated by allocation2 
  int i;
  //  nch=nch;
  //  nrh=nrh;

  for (i=nrh;i>=nrl;i--) free((float*)(m[i]+ncl));
  free((float**)(m+nrl));
} // freemem2 --------------------------------------------------- 

void freeimem2(int **m,int nrl,int nrh,int ncl,int nch){
  //! This method frees memory iallocated by iallocation2 
  int i;

  nch=nch;
  for (i=nrh;i>=nrl;i--) free((int*)(m[i]+ncl));
  free((int**)(m+nrl));
} // freeimem2 ---------------------------------------------------

void freesmem2(unsigned short **m,int nrl,int nrh,int ncl,int nch){
  //! This method frees memory iallocated by iallocation2 
  int i;

  nch=nch;
  for (i=nrh;i>=nrl;i--) free((unsigned short*)(m[i]+ncl));
  free((unsigned short**)(m+nrl));
} // freesmem2 ---------------------------------------------------

void freeumem2(unsigned int **m,int nrl,int nrh,int ncl,int nch){
  //! This method frees memory iallocated by iallocation2 
  int i;

  nch=nch;
  for (i=nrh;i>=nrl;i--) free((unsigned int*)(m[i]+ncl));
  free((unsigned int**)(m+nrl));
} // freeumem2 ---------------------------------------------------

void freecmem2(char **m,int nrl,int nrh,int ncl,int nch){
  //! frees memory allocated by allocation2 
  int i;
  nch=nch;
  nrh=nrh;

  for (i=nrh;i>=nrl;i--) free((char*)(m[i]+ncl));
  free((char**)(m+nrl));
} // freecmem2 --------------------------------------------------- 

void freebmem2(unsigned char **m,int nrl,int nrh,int ncl,int nch){
  //! frees memory allocated by allocation2 
  int i;
  nch=nch;
  nrh=nrh;

  for (i=nrh;i>=nrl;i--) free((unsigned char*)(m[i]+ncl));
  free((unsigned char**)(m+nrl));
} // freebmem2 --------------------------------------------------- 

void freelmem2(long **m,int nrl,int nrh,int ncl,int nch){
  //! frees memory allocated by allocation2 
  int i;
  nch=nch;
  nrh=nrh;

  for (i=nrh;i>=nrl;i--) free((long*)(m[i]+ncl));
  free((long**)(m+nrl));
} // freelmem2 --------------------------------------------------- 

void freemem3(float ***m,int nmf,int nml,int nrl,int nrh,int ncl,int nch){
  int i,j;
  nch=nch;

  for (j=nml;j>=nmf;j--){
    for (i=nrh;i>=nrl;i--) free((float*)(m[j][i]+ncl));
    free((float**)(m[j]+nrl));
  }
  free((float***)(m+nmf));
} // freemem3 --------------------------------------------- 

void freeimem3(int ***m,int l1,int h1,int l2,int h2,int l3,int h3){
  //! This method frees memory iallocated by iallocation3 
  int i,j;
  for (i=h1;i>=l1;i--)
  {
  	for (j=h2;j>=l2;j--) free((int*)(m[i][j]+l3));
    	free((int**)(m[i]+l2));
  }
  free((int***)(m+l1));
} // freeimem3 ------------------------------------------------- 

void freebmem3(unsigned char ***m,int l1,int h1,int l2,int h2,int l3,int h3){
  //! This method frees memory iallocated by iallocation3 
  int i,j;
  for (i=h1;i>=l1;i--)
  {
  	for (j=h2;j>=l2;j--) free((unsigned char *)(m[i][j]+l3));
    	free((unsigned char**)(m[i]+l2));
  }
  free((unsigned char***)(m+l1));
} // freebmem3 ------------------------------------------------- 

void freemem4(float ****m,int ntf, int ntl, int nmf,int nml,int nrl,int nrh,int ncl,int nch){

  int i,j,k;
  nch=nch;

  for (k=ntl;k>=ntf;k--)
  {
    for (j=nml;j>=nmf;j--){
      for (i=nrh;i>=nrl;i--) free((float*)(m[k][j][i]+ncl));
      free((float**)(m[k][j]+nrl)); // WARNING: was (char*)(m[j]+nrl) 
    }
    free((float***)(m[k]+nmf)); // WARNING: was not present at all 
  }
  free((float****)(m+nmf));
} // freemem4 --------------------------------------------- 

void freebmem4(unsigned char ****m,int ntf, int ntl, int nmf,int nml,int nrl,int nrh,int ncl,int nch){

  int i,j,k;
  nch=nch;

  for (k=ntl;k>=ntf;k--)
  {
    for (j=nml;j>=nmf;j--){
      for (i=nrh;i>=nrl;i--) free((unsigned char*)(m[k][j][i]+ncl));
      free((unsigned char**)(m[k][j]+nrl)); // WARNING: was (char*)(m[j]+nrl) 
    }
    free((unsigned char***)(m[k]+nmf)); // WARNING: was not present at all 
  }
  free((unsigned char****)(m+nmf));
} // freebmem4 --------------------------------------------- 

