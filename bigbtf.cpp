#include <mitsuba/core/properties.h>
#include <mitsuba/core/spectrum.h>
#include <mitsuba/core/string.h>
#include <mitsuba/render/bsdf.h>
#include <mitsuba/render/texture.h>
#include <mitsuba/core/logger.h>

#include "big_render.cpp"
#include <ostream>


NAMESPACE_BEGIN(mitsuba)



template <typename Float, typename Spectrum>
class BigBTF final : public BSDF<Float, Spectrum> {
public:
    MTS_IMPORT_BASE(BSDF, m_flags, m_components)
    MTS_IMPORT_TYPES(Texture)

    BigBTF(const Properties &props) : Base(props) {
        int bsdf_index = 0;
        std::cout << "hello" << std::endl;
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
        BigRender::BigRender(filename, false, 0);

    }

    std::pair<BSDFSample3f, Spectrum>
    sample(const BSDFContext &ctx, const SurfaceInteraction3f &si,
           Float sample1, const Point2f &sample2, Mask active) const override {
        MTS_MASKED_FUNCTION(ProfilerPhase::BSDFSample, active);
        
        Float weight = 0.5f;
        if (unlikely(ctx.component != (uint32_t) -1)) {
            bool sample_first =
                ctx.component < m_nested_bsdf[0]->component_count();
            BSDFContext ctx2(ctx);
            if (!sample_first)
                ctx2.component -=
                    (uint32_t) m_nested_bsdf[0]->component_count();
            else
                weight = 1.f - weight;
            auto [bs, result] = m_nested_bsdf[sample_first ? 0 : 1]->sample(
                ctx2, si, sample1, sample2, active);
            result *= weight;
            return { bs, result };
        }

        BSDFSample3f bs = zero<BSDFSample3f>();
        Spectrum result(0.f);

        Mask m0 = active && sample1 > weight, m1 = active && sample1 <= weight;

        if (any_or<true>(m0)) {
            auto [bs0, result0] = m_nested_bsdf[0]->sample(
                ctx, si, (sample1 - weight) / (1 - weight), sample2, m0);
            masked(bs, m0)     = bs0;
            masked(result, m0) = result0;
        }

        if (any_or<true>(m1)) {
            auto [bs1, result1] = m_nested_bsdf[1]->sample(
                ctx, si, sample1 / weight, sample2, m1);
            masked(bs, m1)     = bs1;
            masked(result, m1) = result1;
        }

        return { bs, result };
    }


    Spectrum eval(const BSDFContext &ctx, const SurfaceInteraction3f &si,
                  const Vector3f &wo, Mask active) const override {
        MTS_MASKED_FUNCTION(ProfilerPhase::BSDFEvaluate, active);

        Float weight = 0.5f;
        if (unlikely(ctx.component != (uint32_t) -1)) {
            bool sample_first =
                ctx.component < m_nested_bsdf[0]->component_count();
            BSDFContext ctx2(ctx);
            if (!sample_first)
                ctx2.component -=
                    (uint32_t) m_nested_bsdf[0]->component_count();
            else
                weight = 1.f - weight;
            return weight * m_nested_bsdf[sample_first ? 0 : 1]->eval(
                                ctx2, si, wo, active);
        }

        return m_nested_bsdf[0]->eval(ctx, si, wo, active) * (1 - weight) +
               m_nested_bsdf[1]->eval(ctx, si, wo, active) * weight;
    }

    Float pdf(const BSDFContext &ctx, const SurfaceInteraction3f &si,
              const Vector3f &wo, Mask active) const override {
        MTS_MASKED_FUNCTION(ProfilerPhase::BSDFEvaluate, active);
        if (unlikely(ctx.component != (uint32_t) -1)) {
            bool sample_first =
                ctx.component < m_nested_bsdf[0]->component_count();
            BSDFContext ctx2(ctx);
            if (!sample_first)
                ctx2.component -=
                    (uint32_t) m_nested_bsdf[0]->component_count();
            return m_nested_bsdf[sample_first ? 0 : 1]->pdf(ctx2, si, wo,
                                                            active);
        }
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
};

MTS_IMPLEMENT_CLASS_VARIANT(BigBTF, BSDF)
MTS_EXPORT_PLUGIN(BigBTF, "BlendBSDF material")
NAMESPACE_END(mitsuba)