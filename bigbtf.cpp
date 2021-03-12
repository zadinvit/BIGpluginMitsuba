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
        std::cout << "hello sd" << std::endl;
        Log(Info, "Start");
        for (auto &[name, obj] : props.objects(false)) {
            auto *bsdf = dynamic_cast<Base *>(obj.get());
            if (bsdf) {
                if (bsdf_index == 2)
                    Throw(
                        "BlendBSDF: Cannot specify more than two child BSDFs");
                m_nested_bsdf[bsdf_index++] = bsdf;
                props.mark_queried(name);
            }
        }
        std::string filename = props.string("big_filepath");
        Log(Info, "Loading file \"%s\" ..", filename);
        big_render = new BigRender(filename, false, 0);

    }

    std::pair<BSDFSample3f, Spectrum>
    sample(const BSDFContext &ctx, const SurfaceInteraction3f &si,
           Float sample1, const Point2f &sample2, Mask active) const override {
        Log(Info, "sample");
        Log(Info, "UV Coordinats u \"%d\" v \"%d\" ", si.uv[0], si.uv[1]);
        float cos_theta_i = Frame3f::cos_theta(si.wi);
        Spectrum spect;
        BSDFSample3f bs = BSDFSample3f();
        if (!(active & BSDFFlags::DiffuseReflection) || cos_theta_i <= 0)
            spect = Spectrum(0.0f);
        else {
            bs.wo                = warp::square_to_cosine_hemisphere(sample2);
            bs.pdf               = warp::square_to_cosine_hemisphere_pdf(bs.wo);
            bs.eta               = 1.0;
            bs.sampled_type      = +BSDFFlags::DiffuseReflection;
            bs.sampled_component = 0;
            float_t r2d          = 180.0 / M_PI;
            float_t theta_i      = r2d * acos(float_t(si.wi[2]));
            float_t theta_o      = r2d * acos(bs.wo[2]);
            float_t phi_i        = r2d * atan2(si.wi[1], si.wi[0]);
            float_t phi_o        = r2d * atan2(bs.wo[1], bs.wo[0]);
            float *RGB;
            big_render->getPixel(
                si.uv[0], si.uv[1], theta_i, phi_i, theta_o, phi_o,
                RGB); // get RGB value from BIG file,  UV coordinate
            spect = M_PI * Color3f(RGB[0], RGB[1], RGB[2]);
        }
        
        return { bs, spect }; // 
    }


    Spectrum eval(const BSDFContext &ctx, const SurfaceInteraction3f &si,
                  const Vector3f &wo, Mask active) const override {
      
        return Spectrum(0.0f);
    }

    Float pdf(const BSDFContext &ctx, const SurfaceInteraction3f &si,
              const Vector3f &wo, Mask active) const override {
        MTS_MASKED_FUNCTION(ProfilerPhase::BSDFEvaluate, active);
        Log(Info, "pdf");
        
        return 0;
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