/*!**************************************************************************
\file    TCubeMap.h
\author  Jiri Filip
\date    February 2008
\version 1.00

  The header file for the:
  - cubemap indices lookup

******************************************************************************/
#ifndef TCubeMap_c
#define TCubeMap_c

//#################################################################################
//# class TCubeMap
//#################################################################################

class TCubeMap{
 private:
  int ndir;
  float ***cubeWeights[6];    //! cube-map texture with interpolation weights
  float ***cubeIndices[6];    //! cube-map texture with direction indices
  int CMsize;                 //! size of cube-map texture

public:
  TCubeMap(char* appDir, int ndir, int size);
  TCubeMap(char* appDir);
  ~TCubeMap();
  void loadCubemaps(char *path, int CMsize);
  void interpolateCM(float theta, float phi, float *weights, int *index);
}; //--- TCubeMap --------------------------------------------


#endif