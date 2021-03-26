/*!**************************************************************************
\file    TCubeMap.cpp
\author  Jiri Filip
\date    February 2008
\version 1.00

  The main file for the:
  - cubemap indices lookup
      
******************************************************************************/

#include <cstdio>
#include <cmath>
#include <cstdlib>
#include <string.h>
#include <cassert>

#include "TCubeMap.h"

#include "TAlloc.h"
#include "readpng.h"

#define PI 3.14159265358979323846

//#########################################################################
//######## TCubeMap ##################################################JF##
//#########################################################################
//!\brief cube-maps loading and indexing

TCubeMap::TCubeMap(char* appDir, int ndir, int size)
{
  this->ndir = ndir;
  for(int i=0;i<6;i++)
   {
     cubeIndices[i] = NULL;
     cubeWeights[i] = NULL;
   }
  this->CMsize = size; //1024; //256;

  char aux[100];
  if(ndir==81 || ndir==151)
  {
	sprintf(aux,"%s/../data/cubemaps/%03d/%04d/",appDir,ndir,CMsize);      // ZDE UPRAVIT CESTU. JF
	loadCubemaps(aux, CMsize);
  }
  else
  {
	   printf("Unsupported number of direnctions (%d). Missing cubemaps! Exiting...\n",ndir);
	   exit(0);
  }
}//--- TCubeMap ----------------------------------------------

TCubeMap::TCubeMap(char* appDir)
{
  for(int i=0;i<6;i++)
   {
     cubeIndices[i] = NULL;
     cubeWeights[i] = NULL;
   }
  this->CMsize = 256; // 1024; //256;

  char aux[100];
  sprintf(aux,"%s/../data/cubemaps/081/%04d/",appDir,CMsize);
  loadCubemaps(aux, CMsize);

}//--- TCubeMap ----------------------------------------------

TCubeMap::~TCubeMap()
{
	for(int i=0;i<6;i++)
		if(cubeIndices[i]!=NULL)
			freemem3(cubeIndices[i], 0,2, 0,CMsize-1, 0,CMsize-1);
	for(int i=0;i<6;i++)
		if(cubeWeights[i]!=NULL)
			freemem3(cubeWeights[i], 0,2, 0,CMsize-1, 0,CMsize-1);
}//--- TCubeMap -----------------------------------------------

void 
TCubeMap::loadCubemaps(char *path, int CMsize)
{
  printf("Loading cubemaps...    \r");fflush(stdout);

  this->CMsize = CMsize;

  for(int i=0;i<6;i++)
	  if(cubeIndices[i]==NULL)
		  cubeIndices[i] = allocation3(0,2, 0,CMsize-1, 0,CMsize-1);
  for(int i=0;i<6;i++)
	  if(cubeWeights[i]==NULL)
		  cubeWeights[i] = allocation3(0,2, 0,CMsize-1, 0,CMsize-1);

  char fileName[1000];
  for(int i=0;i<6;i++)
    {
      // reading PNG of cubemap face
      sprintf(fileName,"%scubeIndices_%d.png",path,i);
      int readStatus = read_PNG_file3D (fileName,cubeIndices[i],3,CMsize,CMsize,1);
      printf("Reading cubeIndices - face %d (%d)\n",i, readStatus);
      if(readStatus!=1)
        {
          printf("Error reading of '%s'\nExiting...\n",fileName);
          exit(-1);
        }
      sprintf(fileName,"%scubeWeights_%d.png",path,i);
      readStatus = read_PNG_file3D (fileName,cubeWeights[i],3,CMsize,CMsize,1);
      printf("Reading cubeWeights - face %d (%d)\n",i, readStatus);      
      if(readStatus!=1)
        {
          printf("Error reading of '%s'\nExiting...\n",fileName);
          exit(-1);
        }
    }
  printf("Loading cubemaps...done\n");fflush(stdout);
  return;
}//--- loadCubemaps ------------------------------------------------------

void 
TCubeMap::interpolateCM(float theta, float phi, float *weights, int *index)
{  
  float v_actual[3];
  v_actual[0] =  sin(theta)*cos(phi);
  //v_actual[1] =  sin(theta)*sin(phi);
  v_actual[1] = -sin(theta)*sin(phi); // - fixes shadowing error
  v_actual[2] =  cos(theta);

  int mainAxis;
  float s,t;
  float abs_v[3];
  abs_v[0] = fabs(v_actual[0]);
  abs_v[1] = fabs(v_actual[1]);
  abs_v[2] = fabs(v_actual[2]);

  if(abs_v[0]>=abs_v[1] && abs_v[0]>=abs_v[2])
    if(v_actual[0]>=0.f)
      {
        mainAxis = 0;
        t = -v_actual[1]/abs_v[0];
        s = -v_actual[2]/abs_v[0];//-
      }
    else
      {      
        mainAxis = 1;
        t = -v_actual[1]/abs_v[0];
        s =  v_actual[2]/abs_v[0];
      }
  if(abs_v[1]>=abs_v[2] && abs_v[1]>=abs_v[0])
    if(v_actual[1]>=0.f)
      {     
        mainAxis = 2;
        s =  v_actual[0]/abs_v[1];
        t =  v_actual[2]/abs_v[1];//-
      }
    else
      {     
        mainAxis = 3;
        s =  v_actual[0]/abs_v[1];
        t = -v_actual[2]/abs_v[1];
      }
  if(abs_v[2]>=abs_v[1] && abs_v[2]>=abs_v[0])
    if(v_actual[2]>=0.f)
      {     
        mainAxis = 4;
        s =  v_actual[0]/abs_v[2];
        t = -v_actual[1]/abs_v[2];
      }
    else
      {
        mainAxis = 5;
        s = -v_actual[0]/abs_v[2];//-
        t = -v_actual[1]/abs_v[2];
      }

  int is = (int)floor((CMsize-1)*((s+1.f)*0.5));
  int it = (int)floor((CMsize-1)*((t+1.f)*0.5));
  for(int i=0;i<3;i++)
    {
		/*assert(mainAxis<0 || mainAxis>5);
		assert(i>=0 && i<=3);
		assert(it>=0 && it<CMsize);
		assert(is>=0 && is<CMsize);*/
		index[i]   = (int)cubeIndices[mainAxis][i][it][is];
		weights[i] = cubeWeights[mainAxis][i][it][is]/255.f;
    }

  return;
}//--- interpolateCM ------------------------------------------------------



 