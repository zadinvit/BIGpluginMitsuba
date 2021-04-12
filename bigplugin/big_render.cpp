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
enum class Distribution { uniform, UBO, BTFthtd, BTFthph, none };
static std::unordered_map<std::string, Distribution > const table = { {"uniform",Distribution::uniform}, {"UBO81x81",Distribution::UBO}, {"CoatingRegular", Distribution::BTFthtd}, {"CoatingSpecial", Distribution::BTFthph} };
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
    ///another name nth
    int ntv;
    ///another name ntd
    int nti;
    ///another names: step th
    float step_t;
    ///another names: step ph, step td
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

    ///methods to work with half vector codes from Jiri Filip
    //Half vector convert from Jiri Filip
    void ConvertThetaPhiToHalfDiff(float theta_in, float fi_in, float theta_out, float fi_out, float& theta_half, float& fi_half, float& theta_diff, float& fi_diff);
    void normalize(float* v);
    void cross_product(float* v1, float* v2, float* out);
    void rotate_vector(float* vector, float* axis, float angle, float* out);
    //transform UV coordinates to image coordinates and apply mipmap level
    void getCoordinates(const float &u, const float &v, float& level, int& row, int& col);
   
    

public:
    //constructor for uniform data
    BigRender(std::string, bool cache = false, uint64_t cache_size = 0);
    //constructor for cubemaps
    BigRender(std::string bigname, bool cache, uint64_t cache_size, std::string path_to_cube_maps);
    ~BigRender();
    //get pixel from BIG file
    void getPixel(const float &u, const float &v, float &theta_i, float &phi_i, float &theta_v, float &phi_v, float &level, float RGB[]);
    //get pixel from BIG file
    void getPixelUniform(const float& u, const float &v, float &theta_i, float& phi_i, float& theta_v, float& phi_v, float& level, float RGB[]);
    //get pixel from BIG file
    void getPixelCubeMaps(const float &u, const float &v, float& theta_i, float& phi_i, float &theta_v, float& phi_v, float& level, float RGB[]);
    //get pixel from BTFthph mif file
    void getPixelBTFthph(const float& u, const float& v, float& theta_i, float& phi_i, float& theta_v, float& phi_v, float& level, float RGB[]);
    //get pixel from BTFthtd mif file
    void getPixelBTFthtd(const float& u, const float& v, float& theta_i, float& phi_i, float& theta_v, float& phi_v, float& level, float RGB[]);
    //Convert XYZ to sRGB 0-1 format
    void XYZtoRGB(float XYZ[]);
    // soft transfer on shadow boundaries
    void attenuateElevations(float theta_i, float RGB[]);
    //set tiling scale of UV coordinates
    void setScale(float scale);
    //turn on mipmapping 
    void turnOnMipmapping();
    //information about mipmaps
    Mipmap mip;
    // use render mipmapping
    bool mipmapping = false;
    // max level of mipmap
    int maxMipLevel;
   

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
    mip =  mif.getBtfMipmap("btf0");
    maxMipLevel = mip.isotropic.size()-1;
    if (mip.isotropic.size() > 0) {
        this->nc = mip.isotropic[0].cols;
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
    switch (dist)
    {
    case Distribution::uniform:
        this->ntv = 5;
        this->nti = 6;
        this->step_t = 15.f;
        this->step_p = 30.f;
        this->np = (int)(360.f / step_p);//kroky na otoèku
        this->ni = 1 + np * (nti - 1);//množství smìrù osvìtlení 
        this->nv = np * ntv;//pro kamery
        break;
    case Distribution::UBO:
        throw "MIF file is UBO file and no path to cubemaps added, add in scene file path to file - <string name=\"cubemap_path\" value=\"cubemaps / 081 / 0256\"/> ";
        break;
    case Distribution::BTFthtd:
        this->step_t = 5.f;
        this->step_p = 15.f;
        this->nti = 90.f / step_t;
        this->ntv = 90.f / step_p;
        break;
    case Distribution::BTFthph:
        this->step_t = 5.f; 
        this->step_p = 30.f; 
        this->np = (int)(360.f / step_p);
        this->nv = 90 / step_t;
        break;
    case Distribution::none:
        break;
    default:
        break;
    }
    
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

void BigRender::turnOnMipmapping() {
    mipmapping = true;
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

void BigRender::getCoordinates(const float &u, const float &v, float& level, int& row, int &col) {
    if (level <= 0) {
        //abs u and v because mitsuba scene matpreview.xml have negative UV coordinates.
        row = (int)(floor(abs(u) * (float)nr * uv_scale)) % nr;
        col = (int)(floor(abs(v) * (float)nc * uv_scale)) % nc;
    } else {
        Item lvl = mip.isotropic[(int)level];
        int tmprow = (int)(floor(abs(u) * (float)lvl.rows * uv_scale)) % lvl.rows;
        int tmpcol = (int)(floor(abs(v) * (float)lvl.cols * uv_scale)) % lvl.cols;
        row = tmprow + lvl.y;
        col = tmpcol + lvl.x;
    }
}

void BigRender::getPixel(const float &u, const float &v, float &theta_i, float &phi_i,
    float &theta_v, float& phi_v, float &level, float RGB[]) {
    switch (dist)
    {
    case Distribution::uniform:
        getPixelUniform(u, v, theta_i, phi_i, theta_v, phi_v, level, RGB);
        break;
    case Distribution::UBO:
        getPixelCubeMaps(u, v, theta_i, phi_i, theta_v, phi_v, level, RGB);
        break;
    case Distribution::BTFthtd:
        getPixelBTFthtd(u, v, theta_i, phi_i, theta_v, phi_v, level, RGB);
        break;
    case Distribution::BTFthph:
        getPixelBTFthph(u, v, theta_i, phi_i, theta_v, phi_v, level, RGB);
        break;
    default:
        break;
    }

}

//need to rework y,x are float we need recompute size base on texture image size
void BigRender::getPixelUniform(const float& u, const float& v, float &theta_i, float &phi_i,
                         float& theta_v, float& phi_v,float& level, float RGB[]) {
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
    int irow = 0;
    int jcol = 0;
    getCoordinates(u, v, level, irow, jcol);
    for (int isp = 0; isp < planes; isp++)
        RGB[isp] = 0.f;
    float aux2[3];
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
                        disk->getPixel(idx, irow, jcol, aux2);
                        for (int isp = 0; isp < planes; isp++)
                        {
                            RGB[isp] += aux2[isp] * wti[i] * wtv[j] * wpi[k] * wpv[l];;
                        }
                    }
                }

    attenuateElevations(theta_i_BKP, RGB);
    XYZtoRGB(RGB); //covert XYZ data in RGB to sRGB data
    //clamp RGB values, measure data could be negative, need clamp this data to zero
    clampToZero(RGB, 3);

    return;
}

void BigRender::getPixelCubeMaps(const float& u, const float& v, float &theta_i, float &phi_i,
    float &theta_v, float& phi_v, float &level, float RGB[]) {
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
    int irow = 0; 
    int jcol = 0; 
    getCoordinates(u, v, level, irow, jcol);
    for (int isp = 0; isp < 3; isp++)
        RGB[isp] = 0.f;

    float aux2[3];
    for (int i = 0; i < 9; i++)
    {
        int idx = ni * viewPos[i] + illuPos[i];
        if (cache) {
            const float* aux = cache->getPixel(idx, irow, jcol);
            for (int isp = 0; isp < 3; isp++)
                RGB[isp] += texWeights[i] * aux[isp];
        }
        else {
            disk->getPixel(idx, irow, jcol, aux2);
            for (int isp = 0; isp < 3; isp++)
                RGB[isp] += texWeights[i] * aux2[isp];
        }
      
    }
    // soft transfer on shadow boundaries
    attenuateElevations(theta_i_BKP, RGB);

    XYZtoRGB(RGB); //covert XYZ data in RGB to sRGB data
    //clamp RGB values, measure data could be negative, need clamp this data to zero
    clampToZero(RGB, 3);
}

//*****BTFthph********
void BigRender::normalize(float* v)
{
    float vn = sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
    v[0] /= vn;
    v[1] /= vn;
    v[2] /= vn;
}

void BigRender::cross_product(float* v1, float* v2, float* out)
{
    out[0] = v1[1] * v2[2] - v1[2] * v2[1];
    out[1] = v1[2] * v2[0] - v1[0] * v2[2];
    out[2] = v1[0] * v2[1] - v1[1] * v2[0];
}

void BigRender::rotate_vector(float* vector, float* axis, float angle, float* out)
{
    float temp;
    float cross[3];
    float cos_ang = cos(angle);
    float sin_ang = sin(angle);

    out[0] = vector[0] * cos_ang;
    out[1] = vector[1] * cos_ang;
    out[2] = vector[2] * cos_ang;

    temp = axis[0] * vector[0] + axis[1] * vector[1] + axis[2] * vector[2];
    temp = temp * (1.0 - cos_ang);

    out[0] += axis[0] * temp;
    out[1] += axis[1] * temp;
    out[2] += axis[2] * temp;

    cross_product(axis, vector, cross);

    out[0] += cross[0] * sin_ang;
    out[1] += cross[1] * sin_ang;
    out[2] += cross[2] * sin_ang;
}
//code from Jiri Filip 
void BigRender::ConvertThetaPhiToHalfDiff(float theta_in, float fi_in, float theta_out, float fi_out,
    float& theta_half, float& fi_half, float& theta_diff, float& fi_diff)
{
    // compute in vector
    float in_vec_z = cos(theta_in);
    float proj_in_vec = sin(theta_in);
    float in_vec_x = proj_in_vec * cos(fi_in);
    float in_vec_y = proj_in_vec * sin(fi_in);
    float in[3] = { in_vec_x,in_vec_y,in_vec_z };
    normalize(in);

    // compute out vector
    float out_vec_z = cos(theta_out);
    float proj_out_vec = sin(theta_out);
    float out_vec_x = proj_out_vec * cos(fi_out);
    float out_vec_y = proj_out_vec * sin(fi_out);
    float out[3] = { out_vec_x,out_vec_y,out_vec_z };
    normalize(out);

    // compute halfway vector
    float half_x = (in[0] + out[0]) / 2.0f;
    float half_y = (in[1] + out[1]) / 2.0f;
    float half_z = (in[2] + out[2]) / 2.0f;
    float half[3] = { half_x,half_y,half_z };
    normalize(half);

    // compute  theta_half, fi_half
    theta_half = acos(half[2]);
    fi_half = atan2(half[1], half[0]);

    float bi_normal[3] = { 0.0, 1.0, 0.0 };
    float normal[3] = { 0.0, 0.0, 1.0 };
    float temp[3];
    float diff[3];

    // compute diff vector
    rotate_vector(in, normal, -fi_half, temp);
    rotate_vector(temp, bi_normal, -theta_half, diff);
    normalize(diff);

    // compute  theta_diff, fi_diff
    theta_diff = acos(diff[2]);
    fi_diff = atan2(diff[1], diff[0]);

    if (fi_half < 0.f)
        fi_half += 2.f * PI;
    if (fi_half >= 2.f * PI)
        fi_half -= 2.f * PI;

    if (fi_diff < 0.f)
        fi_diff += 2.f * PI;
    if (fi_diff >= 2.f * PI)
        fi_diff -= 2.f * PI;
}

void BigRender::getPixelBTFthph(const float& u, const float& v, float& theta_i, float& phi_i,
    float& theta_v, float& phi_v, float& level, float RGB[]) {
    float th, ph, td, pd;
    ConvertThetaPhiToHalfDiff(theta_i, phi_i, theta_v, phi_v, th, ph, td, pd);

    int nth = 90.f / step_t;
    int nph = 360.f / step_p;

    int ith[2] = { 0,0 }, iph[2] = { 0,0 };

    int nlth = 1;
    float aux;
    if (nlth) // quadratic mapping of theta_h
    {
        for (int i = 0; i < nth; i++)
        {
            aux = (float)(i * step_t);
            if (r2d * th > aux * aux / 90.f)
                ith[0] = i;
        }
    } 	else // linear mapping of theta_h
        ith[0] = (int)floor((r2d * th) / step_t);

    if (ith[0] > nth - 2)
        ith[0] = nth - 2;
    ith[1] = ith[0] + 1;

    iph[0] = (int)floor((r2d * ph) / step_p);
    iph[1] = iph[0] + 1;

    // compute weights ----------------------------------------------
    float wth[2], wph[2], sum;
    if (nlth)
    {
        aux = (float)(ith[0] * step_t);
        wth[1] = r2d * th - aux * aux / 90.f;
        aux = (float)(ith[1] * step_t);
        wth[0] = aux * aux / 90.f - r2d * th;
    } 	else
    {
        wth[1] = r2d * th - step_t * ith[0];
        wth[0] = step_t * ith[1] - r2d * th;
    }

    sum = wth[0] + wth[1];
    wth[0] /= sum;
    wth[1] /= sum;

    wph[1] = r2d * ph - step_p * iph[0];
    wph[0] = step_p * iph[1] - r2d * ph;

    sum = wph[0] + wph[1];
    wph[0] /= sum;
    wph[1] /= sum;

    iph[0] += 4;  // rotating anisotropy axis
    iph[1] = iph[0] + 1;

    if (iph[0] >= nph)
        iph[0] -= nph;
    if (iph[1] >= nph)
        iph[1] -= nph;

    int irow = 0;
    int jcol = 0;
    getCoordinates(u, v, level, irow, jcol);
    for (int isp = 0; isp < planes; isp++)
        RGB[isp] = 0.f;
    float aux2[3];
    for (int i = 0; i < 2; i++)
        for (int k = 0; k < 2; k++)
        {
            int idx = ith[i] * nph + iph[k];
            float w = wth[i] * wph[k];
            if (cache) {
                const float* aux = cache->getPixel(idx, irow, jcol);
                for (int isp = 0; isp < planes; isp++)
                    RGB[isp] += w * aux[isp];
            } else {
                disk->getPixel(idx, irow, jcol, aux2);
                for (int isp = 0; isp < planes; isp++)
                    RGB[isp] += w * aux2[isp];
            }
            
        }

    attenuateElevations(theta_i, RGB);
    XYZtoRGB(RGB); //covert XYZ data in RGB to sRGB data
    //clamp RGB values, measure data could be negative, need clamp this data to zero
    clampToZero(RGB, 3);
}

void BigRender::getPixelBTFthtd(const float& u, const float& v, float& theta_i, float& phi_i,
    float& theta_v, float& phi_v, float& level, float RGB[]) {
    float aux1;

    // convert to HD param ----------------------------------------
    float th, ph, td, pd;
    ConvertThetaPhiToHalfDiff(theta_i, phi_i, theta_v, phi_v, th, ph, td, pd);
    int nth = 90.f / step_t;
    int ntd = 90.f / step_p;

    int ith[2] = { 0,0 }, itd[2] = { 0,0 };

    int nlth = 1;
    if (nlth) // quadratic mapping of theta_h
    {
        for (int i = 0; i < nth; i++)
        {
            aux1 = (float)(i * step_t);
            if (r2d * th > aux1 * aux1 / 90.f)
                ith[0] = i;
        }
    } 	else // linear mapping of theta_h
        ith[0] = (int)floor((r2d * th) / step_t);

    if (ith[0] > nth - 2)
        ith[0] = nth - 2;
    ith[1] = ith[0] + 1;

    itd[0] = (int)floor((r2d * td) / step_p);
    if (itd[0] > ntd - 2)
        itd[0] = ntd - 2;
    itd[1] = itd[0] + 1;

    // compute weights ----------------------------------------------
    float wth[2], wtd[2], sum;
    if (nlth)
    {
        aux1 = (float)(ith[0] * step_t);
        wth[1] = r2d * th - aux1 * aux1 / 90.f;
        aux1 = (float)(ith[1] * step_t);
        wth[0] = aux1 * aux1 / 90.f - r2d * th;
    } 	else
    {
        wth[1] = r2d * th - step_t * ith[0];
        wth[0] = step_t * ith[1] - r2d * th;
    }

    sum = wth[0] + wth[1];
    wth[0] /= sum;
    wth[1] /= sum;

    wtd[1] = r2d * td - step_p * itd[0];
    wtd[0] = step_p * itd[1] - r2d * td;
    sum = wtd[0] + wtd[1];
    wtd[0] /= sum;
    wtd[1] /= sum;

    int irow = 0;
    int jcol = 0;
    getCoordinates(u, v, level, irow, jcol);

    for (int isp = 0; isp < planes; isp++)
        RGB[isp] = 0.f;
    float aux2[3];
    for (int i = 0; i < 2; i++)
        for (int j = 0; j < 2; j++)
        {
            int idx =  ith[i] * (90.f / step_p) + itd[j];
            float w = wth[i] * wtd[j];
            if (cache) {
                const float* aux = cache->getPixel(idx, irow, jcol);
                for (int isp = 0; isp < planes; isp++)
                    RGB[isp] += w * aux[isp];
            } else {
                disk->getPixel(idx, irow, jcol, aux2);
                for (int isp = 0; isp < planes; isp++)
                    RGB[isp] += w * aux2[isp];
            }
        }

    attenuateElevations(theta_i, RGB);
    XYZtoRGB(RGB); //covert XYZ data in RGB to sRGB data
    //clamp RGB values, measure data could be negative, need clamp this data to zero
    clampToZero(RGB, 3);

}