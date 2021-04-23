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
    // soft transfer on shadow boundaries, not use in final implementation in Mitsuba, but could help in normal renderer
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


};//--- RenderBIG -------------------------------------------------------