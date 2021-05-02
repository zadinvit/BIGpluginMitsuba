/**
* BigBTF plugin - plugin for Mitsuba renderer - version 1.0
* --------------------------------------------------------
* Copyright (C) 2021, Vít Zadina (vitek.zadina@gmail.com)
* Report bugs and download new versions at https://github.com/zadinvit/BIGpluginMitsuba
*
* This library is distributed under the 3-clause BSD license. See notice at the end
* of this file.
*
* This library uses parts of MIF Library, which are:
* Copyright (C) 2021, by  Radomir Vavra (vavra.radomir@gmail.com) and Jiri Filip.
*/

#define _USE_MATH_DEFINES
#include <math.h>
#include "big_core_read.hpp"
//include cubemaps indexing class provide by Jiri Filip
#include "TCubeMap.cpp"
#include "MIFbtf.hpp"
#include "pugixml.hpp"
using namespace mif;

enum class Distribution { uniform, UBO, BTFthtd, BTFthph, none };
static std::unordered_map<std::string, Distribution > const table = { {"Uniform",Distribution::uniform}, {"UBO81x81",Distribution::UBO}, {"CoatingRegular", Distribution::BTFthtd}, {"CoatingSpecial", Distribution::BTFthph} };
Distribution parse(std::string str) {
    auto it = table.find(str);
    if (it != table.end())
        return it->second;
    else
        return Distribution::none;
}

enum class Filtering { MIPMAP_LINEAR, MIPMAP_WEIGHTED, ANIZO_1x, ANIZO_4x, none };
static std::unordered_map<std::string, Filtering> const filtering = { {"MIPMAP_LINEAR",Filtering::MIPMAP_LINEAR}, {"MIPMAP_WEIGHTED",Filtering::MIPMAP_WEIGHTED}, {"ANIZO_1x", Filtering::ANIZO_1x}, {"ANIZO_4x", Filtering::ANIZO_4x} };
Filtering parseFilter(std::string str) {
    auto it = filtering.find(str);
    if (it != filtering.end())
        return it->second;
    else
        return Filtering::none;
}

class BigRender
{
public:
    //for mipmaping use only levelx, for anisotripic levelx a levely
    struct MipLvl
    {
        float levelx = 0;
        float levely = 0;
    };
private:
    //Level struct  use to interpolate with filtering
    struct Level
    {
        float weight;
        int row;
        int col;
    };
    int nr, nc, planes, nimg;
    int ni, nv;
    ///another name nth
    int ntv;
    ///another name ntd
    int nti;
    ///another names: step th
    float step_t;
    ///another names: step ph, step td
    float step_p;
    //! number of azimuths
    int np; 
    //radians to degrees
    float r2d; 
    //scale of texture
    float uv_scale = 7;
    //float 2D array 
    float** anglesUBO = NULL; //! array of angles: index,theta,phi,[x,y,z] coord. of individual directions

    TCubeMap* CM = NULL;      // cubemaps
    //big::BigCoreRead * bigR; //BIG read structure
    MIFbtf mif;
    //cache for read from memory
    MIFbtf::CacheWholeSame* cache = NULL;
    //cache for read from disk
    MIFbtf::CacheNoneSame* disk = NULL;
    //data distribution
    Distribution dist;
    //load big file, and set default parameters
    void init(std::string& bigname, bool& cache);
    //generate angles from cubemaps
    void generateDirectionsUBO(float** anglesUBO);
    //UTIA have some invalid data (negative), we can't have negative value in this data, so we clamp negative values to zero
    void clampToZero(float array[], int size);
    //helper functions
    template<typename T> static T max(const T& a, const T& b) { return a > b ? a : b; }
    //memory init and clear
    float** allocation2(int nrl, int nrh, int ncl, int nch);
    //clear memorz
    void freemem2(float** m, int nrl, int nrh, int ncl, int nch);

    ///methods to work with half vector codes from Jiri Filip
    //Half vector convert from Jiri Filip
    void ConvertThetaPhiToHalfDiff(float theta_in, float fi_in, float theta_out, float fi_out, float& theta_half, float& fi_half, float& theta_diff, float& fi_diff);
    void normalize(float* v);
    void cross_product(float* v1, float* v2, float* out);
    void rotate_vector(float* vector, float* axis, float angle, float* out);
    //get coordinates for Mipmap
    Level getCoordinatesMip(const float& u, const float& v, const int level);
    //get coordinates for anizotropic filtering
    Level getCoordinatesAnizo(const float& u, const float& v, int levelx, int levely);
    //return vector of filtering levels
    std::vector<Level> getLevels(const float& u, const float& v, MipLvl& level);
    //return levels for MIPMAP_LINEAR
    std::vector<Level> getMipLinLevels(const float& u, const float& v, float& level);
    //return levels for MIPMAP_WEIGHTED
    std::vector<Level> getMipWeightLevels(const float& u, const float& v, float& level);
    //return level for classic rendering
    std::vector<Level> getNormalLevels(const float& u, const float& v, float& level);
    //return level for ANIZO_1x
    std::vector<Level> getMipAnizoLevels(const float& u, const float& v, MipLvl& level);
    //return levels for ANIZO_4x
    std::vector<Level> getMipAnizo4Levels(const float& u, const float& v, MipLvl& level);
    //normalize weight of levels (cnt weights == 1)
    void normalizeLevelsWeight(std::vector<Level>& levels);



public:
    //constructor for uniform data
    BigRender(std::string bigname, bool cache = false);
    //constructor for cubemaps
    BigRender(std::string bigname, bool& cache, std::string& path_to_cube_maps);
    ~BigRender();
    //get pixel from BIG file
    void getPixel(const float& u, const float& v, float& theta_i, float& phi_i, float& theta_v, float& phi_v, MipLvl& level, float RGB[]);
    //get pixel from BIG file
    void getPixelUniform(const float& u, const float& v, float& theta_i, float& phi_i, float& theta_v, float& phi_v, MipLvl& level, float RGB[]);
    //get pixel from BIG file
    void getPixelCubeMaps(const float& u, const float& v, float& theta_i, float& phi_i, float& theta_v, float& phi_v, MipLvl& level, float RGB[]);
    //get pixel from BTFthph mif file
    void getPixelBTFthph(const float& u, const float& v, float& theta_i, float& phi_i, float& theta_v, float& phi_v, MipLvl& level, float RGB[]);
    //get pixel from BTFthtd mif file
    void getPixelBTFthtd(const float& u, const float& v, float& theta_i, float& phi_i, float& theta_v, float& phi_v, MipLvl& level, float RGB[]);
    //Convert XYZ to sRGB 0-1 format
    void XYZtoRGB(float XYZ[]);
    // soft transfer on shadow boundaries, theta in radians, not use in final implementation in Mitsuba, but could help in normal renderer
    void attenuateElevations(float theta_i, float RGB[]);
    //set tiling scale of UV coordinates
    void setScale(float scale);
    //set type of filter
    void setFilter(std::string& filter);
    //information about mipmaps
    ElementMipmap mip;
    // max level of mipmap
    int maxMipLevel;
    //Filter type
    Filtering filter = Filtering::none;


};

/**
* 3-Clause BSD License
*
* Copyright (c) 2021, Vít Zadina (vitek.zadina@gmail.com).
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*
* * Redistributions of source code must retain the above copyright notice, this
*   list of conditions and the following disclaimer.
*
* * Redistributions in binary form must reproduce the above copyright notice,
*   this list of conditions and the following disclaimer in the documentation
*   and/or other materials provided with the distribution.
*
* * Neither the name of the copyright holder nor the names of its
*   contributors may be used to endorse or promote products derived from
*   this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
* AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
* FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
* DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
* SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
* CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
* OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
* OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/