#define _USE_MATH_DEFINES
#include <math.h>
#include "big_core_read.hpp"
//include cubemaps indexing class provide by Jiri Filip
#include "TCubeMap.cpp"
#include "pugixml.cpp"
#include "MIFbtf.hpp"
using namespace mif;
using namespace mif::directions;
using namespace mif::mipmap;
enum class Distribution { uniform, UBO, none};
static std::unordered_map<std::string, Distribution > const table = { {"uniform",Distribution::uniform}, {"UBO81x81",Distribution::UBO} };
Distribution parse(std::string str) {
    auto it = table.find(str);
    if (it != table.end())
        return it->second;
    else
        return Distribution::none;
}

class BigRender
{
private:
    int nr, nc, planes, nimg;
    int ni, nv;
    int ntv;
    int nti;
    float step_t;
    float step_p;
    int np;               //! number of azimuths
    float r2d;
    float uv_scale = 7;
    float** anglesUBO = NULL; //! array of angles: index,theta,phi,[x,y,z] coord. of individual directions



    TCubeMap* CM = NULL;      // cubemaps
    //big::BigCoreRead * bigR; //BIG read structure
    MIFbtf mif;
    MIFbtf::MIFcacheWhole * cache = NULL;
    MIFbtf::MIFcacheNone * disk = NULL;
    Distribution dist;
    //xml document
    pugi::xml_document doc;
    //load big file, and set default parameters
    void init(std::string &bigname, bool cache, uint64_t cache_size);
    //generate angles from cubemaps
    void generateDirectionsUBO(float** anglesUBO);
    //UTIA have some invalid data (negative), we can't have negative value in this data, so we clamp negative values to zero
    void clampToZero(float array[], int size);
    //helper functions
    template<typename T> static T max(const T& a, const T& b) { return a > b ? a : b; }
    //memory init and clear
    float** allocation2(int nrl, int nrh, int ncl, int nch);
    void freemem2(float** m, int nrl, int nrh, int ncl, int nch);
   
    

public:
    //constructor for uniform data
    BigRender(std::string, bool cache = false, uint64_t cache_size = 0);
    //constructor for cubemaps
    BigRender(std::string bigname, bool cache, uint64_t cache_size, std::string path_to_cube_maps);
    ~BigRender();
    //get pixel from BIG file
    void getPixel(float u, float v, float theta_i, float phi_i, float theta_v, float phi_v, float RGB[]);
    //get pixel from BIG file
    void getPixelUniform(float& u, float &v, float &theta_i, float& phi_i, float& theta_v, float& phi_v, float RGB[]);
    //get pixel from BIG file
    void getPixelCubeMaps(float &u, float &v, float& theta_i, float& phi_i, float &theta_v, float& phi_v, float RGB[]);
    //Convert XYZ to sRGB 0-1 format
    void XYZtoRGB(float XYZ[]);
    // soft transfer on shadow boundaries
    void attenuateElevations(float theta_i, float RGB[]);
    //set tiling scale of UV coordinates
    void setScale(float scale);
    //maximal mipmap level
    int mipmapLevel;
   

};//--- RenderBIG -------------------------------------------------------

float** BigRender::allocation2(int nrl, int nrh, int ncl, int nch) {
    //! memory allocation, float matrix [nrl..nrh][ncl..nch 
    int j;
    float** m;
    if ((nrh - nrl + 1 <= 0) || (nch - ncl + 1 <= 0)) return NULL;

    m = (float**)calloc((unsigned)(nrh - nrl + 1), sizeof(float*));
    if (!m) { printf(" allocation failure 1 in allocation2() \n"); }
    m -= nrl;

    for (j = nrl; j <= nrh; j++) {
        m[j] = (float*)calloc((unsigned)(nch - ncl + 1), sizeof(float));
        if (!m[j]) { printf(" allocation failure 2 in allocation2() \n"); }
        m[j] -= ncl;
    }
    return m;
}

void  BigRender::freemem2(float** m, int nrl, int nrh, int ncl, int nch) {
    //! frees memory allocated by allocation2 
    int i;
    for (i = nrh; i >= nrl; i--) free((float*)(m[i] + ncl));
    free((float**)(m + nrl));
}

void BigRender::init(std::string &bigname, bool cache, uint64_t cache_size) {
    try {
        /* read info from BIG file */
        mif.open(bigname, true);
        //bigR = new big::BigCoreRead(bigname, cache, cache_size);
    }
    catch (const char* msg) {
        std::cout << msg << std::endl;
    }
    if (cache) {
        this->cache = new MIFbtf::MIFcacheWhole(mif,"btf0");
    } else {
        this->disk = new MIFbtf::MIFcacheNone(mif, "btf0");
    }
    this->r2d = 180.f / M_PI;
    if (cache) {
        this->nr = this->cache->getImageHeight();
        this->nc = this->cache->getImageWidth();
        this->nimg = this->cache->getNumberOfImages();
        this->planes = this->cache->getImageNumberOfPlanes(); // number of specters
    } else {
        this->nr = this->disk->getImageHeight(0);
        this->nc = this->disk->getImageWidth(0);
        this->nimg = this->disk->getNumberOfImages();
        this->planes = this->disk->getImageNumberOfPlanes(0);
    }
    Directions directions = mif.getBtfDirections("btf0");
    this->dist = parse(directions.name);
}


BigRender::BigRender(std::string bigname, bool cache, uint64_t cache_size, std::string path_to_cube_maps) {
   
    init(bigname, cache, cache_size);
    this->ni = 81;
    this->nv = 81;
    // generating list of angles 
    anglesUBO = allocation2(0, 80, 0, 8); // position [8] for an ideal mirroring direction
    generateDirectionsUBO(anglesUBO);


    // cubemaps creation
    CM = new TCubeMap(path_to_cube_maps.data());
}


BigRender::BigRender(std::string bigname, bool cache, uint64_t cache_size) {
    init(bigname,cache, cache_size);

    this->ntv = 5;
    this->nti = 6;
    this->step_t = 15.f;
    this->step_p = 30.f;
    this->np = (int)(360.f / step_p);//kroky na otoèku
    this->ni = 1 + np * (nti - 1);//množství smìrù osvìtlení 
    this->nv = np * ntv;//pro kamery
}


BigRender::~BigRender() {
    //delete bigR;
    if(cache!=NULL)
        delete cache;
    if (anglesUBO != NULL)
        freemem2(anglesUBO, 0, 80, 0, 8);

    if (CM) {
        delete CM;
        CM = 0;
    }
}


void BigRender::setScale(float scale) {
    uv_scale = scale;
}


void BigRender::clampToZero(float array[], int size) {
    for (int i = 0; i < size; i++) {
        array[i] = max(0.0f, array[i]);
    }
}


void BigRender::XYZtoRGB(float XYZ[]) {
    double x = XYZ[0] / 100.0;
    double y = XYZ[1] / 100.0;
    double z = XYZ[2] / 100.0;

    double r = x * 3.2404542 + y * -1.5371385 + z * -0.4985314;
    double g = x * -0.9692660 + y * 1.8760108 + z * 0.0415560;
    double b = x * 0.0556434 + y * -0.2040259 + z * 1.0572252;

    XYZ[0] = r;
    XYZ[1] = g;
    XYZ[2] = b;

}

void BigRender::generateDirectionsUBO(float** anglesUBO)
{
    int theta = 15;
    int phi = 0;
    int phi_increments[] = { 60, 30, 20, 18, 15 };

    anglesUBO[0][0] = 0.f;
    anglesUBO[0][1] = 0.f;
    anglesUBO[0][2] = 0.f;
    anglesUBO[0][8] = 0.f;
    int index = 1;
    int ic = 1;
    for (int i = 0; i < 5; ++i, theta += 15)
    {
        int elc = 0;
        for (phi = 0; phi < 360; phi += phi_increments[i], ++index)
        {
            anglesUBO[index][0] = index;
            anglesUBO[index][1] = theta;
            anglesUBO[index][2] = phi;

            int na = (int)360 / phi_increments[i];
            int aux = elc + na / 2;
            if (aux >= na)
                aux -= na;
            anglesUBO[index][8] = ic + aux;
            elc++;
        }
        ic += elc;
    }
    return;
}



void BigRender::attenuateElevations(float theta_i, float RGB[])
{
    // attenuation below elev. 75 deg. ----------------------				
    float thLim = 1.3089969389957471826927680763665; //75.f*(PI/180.f)
    float cosThLim = cos(thLim);
    if (theta_i > thLim)
    {
        for (int isp = 0; isp < 3; isp++)
            RGB[isp] *= cos(theta_i) / cosThLim;
    }
}

void BigRender::getPixel(float u, float v, float theta_i, float phi_i,
    float theta_v, float phi_v, float RGB[]) {
    switch (dist)
    {
    case Distribution::uniform:
        getPixelUniform(u, v, theta_i, phi_i, theta_v, phi_v, RGB);
        break;
    case Distribution::UBO:
        getPixelCubeMaps(u, v, theta_i, phi_i, theta_v, phi_v, RGB);
        break;
    default:
        break;
    }

}

//need to rework y,x are float we need recompute size base on texture image size
void BigRender::getPixelUniform(float& u, float& v, float &theta_i, float &phi_i,
                         float& theta_v, float& phi_v, float RGB[]) {
    //radian versions of angles
    float theta_i_BKP = theta_i; 
    float theta_v_BKP = theta_v;
    float phi_i_BKP = phi_i;
    float phi_v_BKP = phi_v;

    //convert from radians to degree
    theta_v = theta_v * r2d;
    theta_i = theta_i * r2d;
    phi_v = phi_v * r2d;
    phi_i = phi_i * r2d;
    // make sure phi is in [0, 360)
    while (phi_i < 0.0) { phi_i += 360.0; }
    while (phi_v < 0.0) { phi_v += 360.0; }
    while (phi_i >= 360) { phi_i -= 360.0; }
    while (phi_v >= 360) { phi_v -= 360.0; }

    int iti[2] = { 0,0 }, itv[2] = { 0,0 }, ipi[2] = { 0,0 }, ipv[2] = { 0,0 };//dva nejbližší indexy mezi úhly
    float wti[2] = { 0.f,0.f }, wtv[2] = { 0.f,0.f }, wpi[2] = { 0.f,0.f }, wpv[2] = { 0.f,0.f };  //váhy mezi dvìma indexy
    float sum;

    iti[0] = (int)(floor(theta_i / step_t));
    iti[1] = iti[0] + 1;
    itv[0] = (int)(floor(theta_v / step_t)) - 1;
    itv[1] = itv[0] + 1;

    wti[1] = theta_i - (float)(step_t * iti[0]);
    wti[0] = (float)(step_t * iti[1]) - theta_i;
    sum = wti[0] + wti[1];
    wti[0] /= sum;
    wti[1] /= sum;

    wtv[1] = theta_v - (float)(step_t * (itv[0] + 1));
    wtv[0] = (float)(step_t * (itv[1] + 1)) - theta_v;
    sum = wtv[0] + wtv[1];
    wtv[0] /= sum;
    wtv[1] /= sum;

    // low camera elevations (tv<15)---------------------
    if (itv[0] < 0)
    {
        itv[0] = 0;
        itv[1] = 0;
        wtv[0] = 1.f;
        wtv[1] = 0.f;
    }
    // high elevations (ti,tv>75) ---------------------
    if (iti[0] > nti - 2)
    {
        iti[1] = iti[0];
        wti[0] = 1.f;
        wti[1] = 0.f;
    }
    if (itv[0] > ntv - 2)
    {
        itv[1] = itv[0];
        wtv[0] = 1.f;
        wtv[1] = 0.f;
    }
    ipi[0] = (int)(floor(phi_i / step_p));
    ipi[1] = ipi[0] + 1;
    ipv[0] = (int)(floor(phi_v / step_p));
    ipv[1] = ipv[0] + 1;

    wpi[1] = phi_i - (float)(step_p * ipi[0]);
    wpi[0] = (float)(step_p * ipi[1]) - phi_i;
    sum = wpi[0] + wpi[1];
    wpi[0] /= sum;
    wpi[1] /= sum;
    wpv[1] = phi_v - (float)(step_p * ipv[0]);
    wpv[0] = (float)(step_p * ipv[1]) - phi_v;
    sum = wpv[0] + wpv[1];
    wpv[0] /= sum;
    wpv[1] /= sum;

    if (ipi[1] == np)
        ipi[1] = 0;
    if (ipv[1] == np)
        ipv[1] = 0;

    // compute texture mapping
    //abs u and v because mitsuba scene matpreview.xml have negative UV coordinates.
    int irow = (int) (floor(abs(u) * (float) nr * uv_scale)) % nr;
    int jcol = (int) (floor(abs(v) * (float) nc * uv_scale)) % nc;
    for (int isp = 0; isp < planes; isp++)
        RGB[isp] = 0.f;

    for (int j = 0; j < 2; j++)
        for (int l = 0; l < 2; l++)
            for (int i = 0; i < 2; i++)
                for (int k = 0; k < 2; k++)
                {
                    int idx = 0;
                    int idxCam = np * ni * itv[j] + ni * ipv[l];
                    if (iti[i] == 0)
                        idx = idxCam;
                    else
                        idx = idxCam + 1 + np * (iti[i] - 1) + ipi[k]; 
                   
                    if (cache) {
                        const float* aux = cache->getPixel(idx, irow, jcol);
                        for (int isp = 0; isp < planes; isp++)
                        {
                            RGB[isp] += aux[isp] * wti[i] * wtv[j] * wpi[k] * wpv[l];;
                        }
                    }
                    else {
                        float aux[3];
                        disk->getPixel(idx, irow, jcol, aux);
                        for (int isp = 0; isp < planes; isp++)
                        {
                            RGB[isp] += aux[isp] * wti[i] * wtv[j] * wpi[k] * wpv[l];;
                        }
                    }
                }

    attenuateElevations(theta_i_BKP, RGB);
    XYZtoRGB(RGB); //covert XYZ data in RGB to sRGB data
    //clamp RGB values, measure data could be negative, need clamp this data to zero
    clampToZero(RGB, 3);

    return;
}

void BigRender::getPixelCubeMaps(float& u, float& v, float &theta_i, float &phi_i,
    float &theta_v, float& phi_v, float RGB[]) {
    //radian versions of angles
    float theta_i_BKP = theta_i;

    int i_index[3];
    float i_weight[3];
    int v_index[3];
    float v_weight[3];
    // illum. and view directions indices from cubemaps class
    CM->interpolateCM(theta_i, phi_i, i_weight, i_index);
    CM->interpolateCM(theta_v, phi_v, v_weight, v_index);

    // mixed interp. weights computation
    int illuPos[9];
    int viewPos[9];
    float texWeights[9];
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
        {
            int k = i * 3 + j;
            illuPos[k] = i_index[i];
            viewPos[k] = v_index[j];
            texWeights[k] = i_weight[i] * v_weight[j];
        }

    // compute texture mapping
    //abs u and v because mitsuba scene matpreview.xml have negative UV coordinates.
    int irow = (int)(floor(abs(u) * (float)nr * uv_scale)) % nr;
    int jcol = (int)(floor(abs(v) * (float)nc * uv_scale)) % nc;

    for (int isp = 0; isp < 3; isp++)
        RGB[isp] = 0.f;

    float aux[3];
    for (int i = 0; i < 9; i++)
    {
        int idx = ni * viewPos[i] + illuPos[i];
        if (cache) {
            const float* aux = cache->getPixel(idx, irow, jcol);
            for (int isp = 0; isp < 3; isp++)
                RGB[isp] += texWeights[i] * aux[isp];
        }
        else {
            float aux[3];
            disk->getPixel(idx, irow, jcol, aux);
            for (int isp = 0; isp < 3; isp++)
                RGB[isp] += texWeights[i] * aux[isp];
        }
      
    }
    // soft transfer on shadow boundaries
    attenuateElevations(theta_i_BKP, RGB);

    XYZtoRGB(RGB); //covert XYZ data in RGB to sRGB data
    //clamp RGB values, measure data could be negative, need clamp this data to zero
    clampToZero(RGB, 3);
}