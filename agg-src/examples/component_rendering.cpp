#include "agg_basics.h"
#include "agg_rendering_buffer.h"
#include "agg_rasterizer_scanline_aa.h"
#include "agg_scanline_p.h"
#include "agg_renderer_scanline.h"
#include "agg_ellipse.h"
#include "agg_pixfmt_gray.h"
#include "agg_pixfmt_rgb.h"
#include "ctrl/agg_slider_ctrl.h"
#include "platform/agg_platform_support.h"

#define AGG_BGR24
//#define AGG_BGR48
//#define AGG_BGR96
#include "pixel_formats.h"

using gray_blender = agg::blender_gray<gray_type>;

enum flip_y_e { flip_y = static_cast<unsigned int>(true) };


class the_application : public agg::platform_support
{
    agg::slider_ctrl<color_type> m_alpha;

public:
    the_application(agg::pix_format_e format, bool flip_y) :
        agg::platform_support(format, flip_y),
        m_alpha(5, 5, 320-5, 10+5, !flip_y)
    {
        m_alpha.label("Alpha=%1.0f");
        m_alpha.range(0, 255);
        m_alpha.value(255);
        add_ctrl(m_alpha);
    }

    void on_draw() override
    {
        pixfmt pf(rbuf_window());

        using pixfmt_r = agg::pixfmt_alpha_blend_gray<gray_blender, agg::rendering_buffer, 3, 2>;
        using pixfmt_g = agg::pixfmt_alpha_blend_gray<gray_blender, agg::rendering_buffer, 3, 1>;
        using pixfmt_b = agg::pixfmt_alpha_blend_gray<gray_blender, agg::rendering_buffer, 3, 0>;

        pixfmt_r pfr(rbuf_window());
        pixfmt_g pfg(rbuf_window());
        pixfmt_b pfb(rbuf_window());

        agg::renderer_base<pixfmt>   rbase(pf);
        agg::renderer_base<pixfmt_r> rbr(pfr);
        agg::renderer_base<pixfmt_g> rbg(pfg);
        agg::renderer_base<pixfmt_b> rbb(pfb);

        agg::rasterizer_scanline_aa<> ras;
        agg::scanline_p8 sl;

        rbase.clear(agg::rgba(1,1,1));

        agg::ellipse er(width() / 2 - 0.87*50, height() / 2 - 0.5*50, 100, 100, 100);
        ras.add_path(er);
        agg::render_scanlines_aa_solid(ras, sl, rbr,
                                       gray_type(0, unsigned(m_alpha.value())));
        
        agg::ellipse eg(width() / 2 + 0.87*50, height() / 2 - 0.5*50, 100, 100, 100);
        ras.add_path(eg);
        agg::render_scanlines_aa_solid(ras, sl, rbg, 
                                       gray_type(0, unsigned(m_alpha.value())));

        agg::ellipse eb(width() / 2, height() / 2 + 50, 100, 100, 100);
        ras.add_path(eb);
        agg::render_scanlines_aa_solid(ras, sl, rbb,
                                       gray_type(0, unsigned(m_alpha.value())));

        agg::render_ctrl(ras, sl, rbase, m_alpha);
    }

};


int agg_main(int /*argc*/, char * /*argv*/[])
{
  the_application app(pix_format, flip_y != 0u);
  app.caption("AGG Example. Component Rendering");

  if (app.init(320, 320, 0)) {
    return app.run();
    }
    return 1;
}


