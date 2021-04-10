#include <mitsuba/core/properties.h>
#include <mitsuba/core/spectrum.h>
#include <mitsuba/core/string.h>
#include <mitsuba/render/bsdf.h>
#include <mitsuba/render/texture.h>
#include <mitsuba/core/logger.h>
#include <mitsuba/core/warp.h>

#include "big_render.cpp"
#include <ostream>


NAMESPACE_BEGIN(mitsuba)



template <typename Float, typename Spectrum>
class BigBTF final : public BSDF<Float, Spectrum> {
public:
    MTS_IMPORT_BASE(BSDF, m_flags, m_components)

    BigBTF(const Properties &props) : Base(props) {
        int bsdf_index = 0;

        std::string filename = props.string("big_filepath");
        Log(Info, "Loading file \"%s\" ..", filename);
        uint64_t memory = 0;
        bool useMemory = false;
        if (props.has_property("memory")) {
            memory = props.int_("memory");
            memory = memory * 1024 * 1024; //convert from MB to B 
            if (memory >= 0) {
                useMemory = true;
            }
        }
       
        if (props.has_property("cubemap_path")) {
            std::string pathToCubeMap = props.string("cubemap_path");
            if (pathToCubeMap.compare(pathToCubeMap.size()-1,1,"/")) {
                pathToCubeMap += "/";
            }
            big_render = new BigRender(filename, useMemory, memory, pathToCubeMap);
        } else {
            big_render = new BigRender(filename, useMemory, memory);
        }

        if (props.has_property("scale")) {
            float scale = props.float_("scale");
            big_render->setScale(scale);
        }


       
      
        m_flags    = BSDFFlags::DiffuseReflection | BSDFFlags::FrontSide | BSDFFlags::NeedsDifferentials;
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
        Vector2f duv_dx = si.duv_dx;
        Vector2f duv_dy = si.duv_dy;
        float width = max(max(duv_dx[0], duv_dx[1]), max(duv_dy[0], duv_dy[1]));
        //(Info, "width \"%d\" ", width);
        //Log(Info, "duv_DX \"%d\", \"%d\" duv_DY \"%d\", \"%d\" ", duv_dx[0], duv_dx[1], duv_dy[0], duv_dy[1]);
        float RGB[3];
        big_render->getPixel(si.uv[0], si.uv[1], theta_i, phi_i, theta_o, phi_o,  RGB); // get RGB value from BIG file,  UV coordinate
        spect = Color3f(RGB[0], RGB[1],RGB[2]); //M_PI *  / Frame3f::cos_theta(bs.wo)* M_PI /cos_theta_o, možná bude fungovat s dìlením a v eval s násobením tímto úhelm cosine term (musím toto konzultovat)
        return { bs, select(active,spect,0.0f) }; // 
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
            float_t theta_i      = acos(si.wi[2]);
            float_t theta_o      = acos(wo[2]);
            float_t phi_i        = atan2(si.wi[1], si.wi[0]);
            float_t phi_o        = atan2(wo[1], wo[0]);
            float RGB[3];
            big_render->getPixel(
                si.uv[0], si.uv[1], theta_i, phi_i, theta_o, phi_o,
                RGB); // get RGB value from BIG file,  UV coordinate
            //invPI for darken lights spots and cost thete to darken light part
            spect = Color3f(RGB[0], RGB[1], RGB[2]) * cos_theta_o * math::InvPi<float>; //* math::InvPi<float>  * cos_theta_o cosine term. 
        }
        return spect;
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
};

MTS_IMPLEMENT_CLASS_VARIANT(BigBTF, BSDF)
MTS_EXPORT_PLUGIN(BigBTF, "BlendBSDF material")
NAMESPACE_END(mitsuba)