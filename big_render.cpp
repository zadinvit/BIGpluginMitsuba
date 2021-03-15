#define _USE_MATH_DEFINES
#include <math.h>

#include "big_core_read.hpp"

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
    big::BigCoreRead * bigR; //BIG read structure

public:
    BigRender(std::string, bool cache = false, int cache_size = 0);
    ~BigRender();
    //get pixel from BIG file
    void getPixel(int y, int x, float theta_i, float phi_i, float theta_v, float phi_v, float RGB[]);

};//--- RenderBIG -------------------------------------------------------


BigRender::BigRender(std::string bigname, bool cache, int cache_size) {
    printf("loading BIG...    \n");
    try {
        bigR = &big::BigCoreRead(bigname, cache, cache_size);
    } catch (const char *msg) {
        std::cout << msg << std::endl;
    }
   
 

    /* read info from BIG file */
    this->nr = bigR->getImageHeight(); 
    this->nc     = bigR->getImageWidth();
    this->nimg   = bigR->getNumberOfImages();
    this->planes = bigR->getNumberOfPlanes(); // number of specters
            
    //int status = load(bigname, true); // zde je treba nacist BIG soubor
    //report();
    //if (status < 0)
    //    exit(0);
   // report();
   // printf("loading BIG...done\n");

    //this->nr = get_height(); // zde je treba z nej vytahnout vysku textury
    //this->nc = get_width(); // zde je treba z nej vytahnout sirku textury
    //this->planes = get_spectra(); // zde je treba z nej vytahnout pocet spekter
    //this->nimg = get_images(); // zde je treba z nej vytahnout pocet obrazku

    this->ntv = 5;
    this->nti = 6;
    this->step_t = 15.f;
    this->step_p = 30.f;
    this->np = (int)(360.f / step_p);//kroky na oto�ku
    this->ni = 1 + np * (nti - 1);//mno�stv� sm�r� osv�tlen� 
    this->nv = np * ntv;//pro kamery
    this->r2d = 180.f / M_PI;

}//--- RenderBIG --------------------------------------------------

BigRender::~BigRender() {
}//--- ~RenderBIG --------------------------------------------------


//need to rework y,x are float we need recompute size base on texture image size
void BigRender::getPixel(int y, int x, float theta_i, float phi_i,
                         float theta_v, float phi_v, float RGB[]) {
    float theta_i_BKP = theta_i;
    float theta_v_BKP = theta_v;
    float phi_i_BKP = phi_i;
    float phi_v_BKP = phi_v;
    theta_i *= r2d;
    theta_v *= r2d;
    phi_i *= r2d;
    phi_v *= r2d;

    int iti[2] = { 0,0 }, itv[2] = { 0,0 }, ipi[2] = { 0,0 }, ipv[2] = { 0,0 };//dva nejbli��� indexy mezi �hly
    float wti[2] = { 0.f,0.f }, wtv[2] = { 0.f,0.f }, wpi[2] = { 0.f,0.f }, wpv[2] = { 0.f,0.f };  //v�hy mezi dv�ma indexy
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
    int irow = y % nr;
    int jcol = x % nc;

    float aux[3];
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

                    //get_pixel(0, irow, jcol, idx, aux); // zde je treba zavolat funkci co mi da hodnotu pixelu pro obrazek s indexem idx

                    for (int isp = 0; isp < planes; isp++)
                    {
                        aux[isp] *= wti[i] * wtv[j] * wpi[k] * wpv[l];//p�id�n� na z�klad� vaha (v�hy 0...1) v�hy v�echy �ty� sm�r�
                        RGB[isp] += aux[isp];
                    }

                }

    // attenuation below elev. 75 deg. ----------------------               
    float thLim = 1.3089969389957471826927680763665; //75.f*(PI/180.f)
    float cosThLim = cos(thLim);
    if (theta_i_BKP > thLim)
    {
        for (int isp = 0; isp < 3; isp++)
            RGB[isp] *= cos(theta_i_BKP) / cosThLim;
    }
    return;
}