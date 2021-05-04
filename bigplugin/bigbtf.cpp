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

#include <mitsuba/core/properties.h>
#include <mitsuba/core/spectrum.h>
#include <mitsuba/core/string.h>
#include <mitsuba/render/bsdf.h>
#include <mitsuba/render/texture.h>
#include <mitsuba/core/logger.h>
#include <mitsuba/core/warp.h>

#include "big_render.cpp"
#include <ostream>

//1 to generate mipmap levels image instead render texture
#define MIPMAPMAP 0;

NAMESPACE_BEGIN(mitsuba)



template <typename Float, typename Spectrum>
class BigBTF final : public BSDF<Float, Spectrum> {
public:
    MTS_IMPORT_BASE(BSDF, m_flags, m_components)

    BigBTF(const Properties &props) : Base(props) {
        int bsdf_index = 0;
        m_flags = BSDFFlags::DiffuseReflection | BSDFFlags::FrontSide;
        std::string filename = props.string("filepath");
        Log(Info, "Loading file \"%s\" ..", filename);
        bool memory = false;
        if (props.has_property("memory")) {
            memory = props.bool_("memory");
        }

        if (props.has_property("cubemap_path")) {
            std::string pathToCubeMap = props.string("cubemap_path");
            if (pathToCubeMap.compare(pathToCubeMap.size()-1,1,"/")) {
                pathToCubeMap += "/";
            }
            try {
                big_render = new BigRender(filename, memory, pathToCubeMap);
            }
            catch (const char* str) {
                Log(Error, "BigRender init error: %s", str);
            }
        } else {
            try {
                big_render = new BigRender(filename, memory);
            }
            catch (const char* str) {
                Log(Error, "BigRender init error: %s", str);
            }
            
        }

        if (props.has_property("filtering")) {
            std::string filter = props.string("filtering");
            big_render->setFilter(filter);
            if (big_render->filter != Filtering::none) {
                if (big_render->mip.getIsotropicItems().size() > 0 || big_render->mip.getAnisotropicItems().size()>0) {
                    m_flags = BSDFFlags::DiffuseReflection | BSDFFlags::FrontSide | BSDFFlags::NeedsDifferentials;
                    Log(Info, "Renderer using %s.", filter);
                } else {
                    Log(Error, "BigRender Mif file %s don't have mipmaps/anizotropy maps and you want use mipmapping/anizotropy filtering.", filename);
                }
                if ((big_render->filter == Filtering::ANIZO_1x || big_render->filter == Filtering::ANIZO_4x) && big_render->mip.getAnisotropicItems().size() <= 0) {
                    Log(Error, "BigRender Mif file %s don't have anizotropy maps and you want use anizotropy filtering.", filename);
                }
            }
        }

        if (props.has_property("scale")) {
            float scale = props.float_("scale");
            big_render->setScale(scale);
        }
        if (props.has_property("level")) {
           filter_level = props.int_("level");
        }

        m_components.push_back(m_flags);

    }

    ~BigBTF() {
        delete big_render;
    }
    //sample function, sample texture data 
    std::pair<BSDFSample3f, Spectrum>
    sample(const BSDFContext &ctx, const SurfaceInteraction3f &si,
           Float sample1, const Point2f &sample2, Mask active) const override {
       // MTS_MASKED_FUNCTION(ProfilerPhase::BSDFSample, active);// lower perfonmance 
        //Log(Info, "UV Coordinats u \"%d\" v \"%d\" ", si.uv[0], si.uv[1]);
        float cos_theta_i = Frame3f::cos_theta(si.wi);
        Spectrum spect;
        BSDFSample3f bs = zero<BSDFSample3f>();
        active &= cos_theta_i > 0.f;
        if (unlikely(none_or<false>(active) || !ctx.is_enabled(BSDFFlags::DiffuseReflection)) || cos_theta_i <= 0)
            return { bs, 0.f };
        bs.wo                = warp::square_to_cosine_hemisphere(sample2);
        bs.pdf               = warp::square_to_cosine_hemisphere_pdf(bs.wo);
        bs.eta               = 1.0f;
        bs.sampled_type      = +BSDFFlags::DiffuseReflection;
        bs.sampled_component = 0;
        //angles in radians
        float_t theta_i = acos(si.wi[2]);
        float_t theta_o = acos(bs.wo[2]);
        float_t phi_i = atan2(si.wi[1], si.wi[0]);
        float_t phi_o = atan2(bs.wo[1], bs.wo[0]);
        BigRender::MipLvl level;
        if (big_render->filter == Filtering::MIPMAP_LINEAR || big_render->filter == Filtering::MIPMAP_WEIGHTED) {
            int cols, rows;
            if (big_render->mip.isIsotropic()) {
                cols = big_render->mip.getIsotropicItems()[0].getCols();
                rows = big_render->mip.getIsotropicItems()[0].getRows();
            } else {
                cols = big_render->mip.getAnisotropicItems()[0].getCols();
                rows = big_render->mip.getAnisotropicItems()[0].getRows();
            }
            float width = max(max(si.duv_dx[0] * cols, si.duv_dx[1] * cols), max(si.duv_dy[0] * rows, si.duv_dy[1] * rows));
            if (width == 0.0f) //log 0 is inf, so we need check it 
                level.levelx = 0.0f;
            else
                level.levelx = min(float(big_render->maxMipLevel), float(big_render->maxMipLevel) + log2(width) + filter_level);
            //std::cout << level << " log width"<< log2(width) << std::endl;
        } else if (big_render->filter == Filtering::ANIZO_1x || big_render->filter == Filtering::ANIZO_4x) {
            int cols = big_render->mip.getAnisotropicItems()[0].getCols();
            int rows = big_render->mip.getAnisotropicItems()[0].getRows();
            float widthx = max(si.duv_dx[0] * cols, si.duv_dx[1] * cols);
            float widthy = max(si.duv_dy[0] * rows, si.duv_dy[1] * rows);
            if (widthx == 0.0f) //log 0 is inf, so we need check it 
                level.levelx = 0.0f;
            else 
                level.levelx = min(float(big_render->mip.getAnisotropicItems().width() - 1), float(big_render->mip.getAnisotropicItems().width() - 1) + log2(widthx) + filter_level);
            if(widthy == 0.0f)
                level.levely = 0.0f;
            else
                level.levely = min(float(big_render->mip.getAnisotropicItems().height() - 1), float(big_render->mip.getAnisotropicItems().height() - 1) + log2(widthy) + filter_level);
        }
        //(Info, "width \"%d\" ", width);
        //Log(Info, "duv_DX \"%d\", \"%d\" duv_DY \"%d\", \"%d\" ", duv_dx[0], duv_dx[1], duv_dy[0], duv_dy[1]);
        float RGB[3];
#if MIPMAPMAP == 1
        if ((int)level.levelx <= 0)
            return{ bs, Color3f(0.0f,0.f,0.f) };
        else if ((int)level.levelx % 3 == 0)
            return{ bs, Color3f(0.0f,0.0f,1.0f) };
        else if ((int)level.levelx % 3 == 1)
            return{ bs, Color3f(0.0f,1.0f,0.0f) };
        else if ((int)level.levelx % 3 == 2)
            return{ bs, Color3f(1.0f,0.0f,0.0f) };
        else {
            return{ bs, Color3f(1.0f,1.0f,1.0f) };
        }
        /*float one = 1.0f / big_render->maxMipLevel;
        float color = (int)level * one;*/
       
#else // MIPMAPMAP

        big_render->getPixel(si.uv[0], si.uv[1], theta_i, phi_i, theta_o, phi_o, level, RGB ); // get RGB value from BIG file,  UV coordinate
        spect = Color3f(RGB[0], RGB[1],RGB[2]); //M_PI *  / Frame3f::cos_theta(bs.wo)* M_PI /cos_theta_o, možná bude fungovat s dìlením a v eval s násobením tímto úhelm cosine term (musím toto konzultovat)
        return { bs, select(active,spect,0.0f) }; // 
#endif
    }

    //Evaluate the BSDF f(wi, wo) or its adjoint version f ^{ * }(wi, wo) and multiply by the cosine foreshortening term.
    //emitter sampling
    Spectrum eval(const BSDFContext &ctx, const SurfaceInteraction3f &si,
                  const Vector3f &wo, Mask active) const override {
       // MTS_MASKED_FUNCTION(ProfilerPhase::BSDFEvaluate, active);
        float cos_theta_i = Frame3f::cos_theta(si.wi);
        float cos_theta_o = Frame3f::cos_theta(wo);
        Spectrum spect;
        // !ctx.is_enabled(BSDFFlags::DiffuseReflection) newest version off !(active & BSDFFlags::DiffuseReflection)
        if (!ctx.is_enabled(BSDFFlags::DiffuseReflection) || cos_theta_i <= 0 ||
            cos_theta_o <= 0)
            return Spectrum(0.0f);
        else {
            float_t theta_i = acos(si.wi[2]);
            float_t theta_o = acos(wo[2]);
            if (std::isnan(theta_o)) { //sometimes pathtracer for light return wo[2] > 1.0000 and this cause errror
                return Spectrum(0.0f);
            }
            float_t phi_i = atan2(si.wi[1], si.wi[0]);
            float_t phi_o = atan2(wo[1], wo[0]);
            BigRender::MipLvl level;
            if (big_render->filter == Filtering::MIPMAP_LINEAR || big_render->filter == Filtering::MIPMAP_WEIGHTED) {
                int cols, rows;
                if (big_render->mip.isIsotropic()) {
                    cols = big_render->mip.getIsotropicItems()[0].getCols();
                    rows = big_render->mip.getIsotropicItems()[0].getRows();
                } else {
                    cols = big_render->mip.getAnisotropicItems()[0].getCols();
                    rows = big_render->mip.getAnisotropicItems()[0].getRows();
                }
                float width = max(max(si.duv_dx[0] * cols, si.duv_dx[1] * cols), max(si.duv_dy[0] * rows, si.duv_dy[1] * rows));
                if (width == 0.0f) //log 0 is inf, so we need check it 
                    level.levelx = 0.0f;
                else
                    level.levelx = min(float(big_render->maxMipLevel), float(big_render->maxMipLevel) + log2(width) + filter_level);
                //std::cout << level << " log width"<< log2(width) << std::endl;
            } else if (big_render->filter == Filtering::ANIZO_1x || big_render->filter == Filtering::ANIZO_4x) {
                int cols = big_render->mip.getAnisotropicItems()[0].getCols();
                int rows = big_render->mip.getAnisotropicItems()[0].getRows();
                float widthx = max(si.duv_dx[0] * cols, si.duv_dx[1] * cols);
                float widthy = max(si.duv_dy[0] * rows, si.duv_dy[1] * rows);
                if (widthx == 0.0f) //log 0 is inf, so we need check it 
                    level.levelx = 0.0f;
                else
                    level.levelx = min(float(big_render->mip.getAnisotropicItems().width() - 1), float(big_render->mip.getAnisotropicItems().width() - 1) + log2(widthx) + filter_level);
                if (widthy == 0.0f)
                    level.levely = 0.0f;
                else
                    level.levely = min(float(big_render->mip.getAnisotropicItems().height() - 1), float(big_render->mip.getAnisotropicItems().height() - 1) + log2(widthy) + filter_level);
            }
#if MIPMAPMAP == 1
            if ((int)level.levelx <= 0)
                return Color3f(0.0f,0.f,0.f) ;
            else if ((int)level.levelx % 3 == 0)
                return Color3f(0.0f,0.0f,1.0f) ;
            else if ((int)level.levelx % 3 == 1)
                return Color3f(0.0f,1.0f,0.0f) ;
            else if ((int)level.levelx % 3 == 2)
                return Color3f(1.0f,0.0f,0.0f) ;
            else {
                return Color3f(1.0f,1.0f,1.0f) ;
            }
            /*if (level < 0)
                level = 0;
            float one = 1.0f / big_render->maxMipLevel;
            float color =  (int)level * 0.1f;
            return Color3f(0.f, color, color);*/
        }
#else // MIPMAPMAP
            float RGB[3];
            big_render->getPixel(si.uv[0], si.uv[1], theta_i, phi_i, theta_o, phi_o, level, RGB); // get RGB value from BIG file,  UV coordinate
            //invPI for darken lights spots and cost thete to darken light part
            spect = Color3f(RGB[0], RGB[1], RGB[2]) * cos_theta_o * math::InvPi<float>; //* math::InvPi<float>  * cos_theta_o cosine term. 
        }
        return spect;
#endif 

    }

    //Compute the probability per unit solid angle of sampling a given direction
    Float pdf(const BSDFContext &ctx, const SurfaceInteraction3f &si,
              const Vector3f &wo, Mask active) const override {
       // MTS_MASKED_FUNCTION(ProfilerPhase::BSDFEvaluate, active);
        float cos_theta_i = Frame3f::cos_theta(si.wi);
        float cos_theta_o = Frame3f::cos_theta(wo);
        //!ctx.is_enabled(BSDFFlags::DiffuseReflection) newest version off !(active & BSDFFlags::DiffuseReflection)
        if (!ctx.is_enabled(BSDFFlags::DiffuseReflection) || cos_theta_i <= 0 ||
            cos_theta_o <= 0)
            return 0.0f;
        
            return warp::square_to_cosine_hemisphere_pdf(wo);
    }

    void traverse(TraversalCallback *callback) override {
        callback->put_object("bsdf_0", m_nested_bsdf[0].get());
        callback->put_object("bsdf_1", m_nested_bsdf[1].get());
    }

    std::string to_string() const override {
        std::ostringstream oss;
        oss << "BlendBSDF[" << std::endl << "," << std::endl
            << "  nested_bsdf[0] = " << string::indent(m_nested_bsdf[0]) << ","
            << std::endl
            << "  nested_bsdf[1] = " << string::indent(m_nested_bsdf[1])
            << std::endl
            << "]";
        return oss.str();
    }

    MTS_DECLARE_CLASS()
protected:
    ref<Base> m_nested_bsdf[2];
    BigRender *big_render;
    int filter_level = 0;
};

MTS_IMPLEMENT_CLASS_VARIANT(BigBTF, BSDF)
MTS_EXPORT_PLUGIN(BigBTF, "bigbtf plugin")
NAMESPACE_END(mitsuba)

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