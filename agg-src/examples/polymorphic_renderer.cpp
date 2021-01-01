#include <cmath>
#include <cstdio>
#include <ctime>
#include "agg_basics.h"
#include "agg_rendering_buffer.h"
#include "agg_rasterizer_scanline_aa.h"
#include "agg_rasterizer_outline.h"
#include "agg_scanline_p.h"
#include "agg_path_storage.h"
#include "agg_renderer_scanline.h"
#include "platform/agg_platform_support.h"

#include "agg_pixfmt_rgb.h"
#include "agg_pixfmt_rgb_packed.h"
#include "agg_pixfmt_rgba.h"

static int pix_fmt = agg::pix_format_rgb555;
//static int pix_fmt = agg::pix_format_rgb565;
//static int pix_fmt = agg::pix_format_rgb24; 
//static int pix_fmt = agg::pix_format_bgr24; 
//static int pix_fmt = agg::pix_format_rgba32;
//static int pix_fmt = agg::pix_format_argb32;
//static int pix_fmt = agg::pix_format_abgr32;
//static int pix_fmt = agg::pix_format_bgra32;


enum flip_y_e { flip_y = static_cast<unsigned int>(true) };

namespace agg
{
    //========================================================================
    class polymorphic_renderer_solid_rgba8_base
    {
    public:
      using color_type = srgba8;
      using scanline_type = scanline_p8;

      virtual ~polymorphic_renderer_solid_rgba8_base() = default;

      //--------------------------------------------------------------------
      virtual void clear(const color_type &c) = 0;
      virtual void color(const color_type &c) = 0;
      virtual color_type color() const = 0;
      virtual void prepare() = 0;
      virtual void render(const scanline_type &) = 0;
    };



    //========================================================================
    template<class PixFmt> class polymorphic_renderer_solid_rgba8_adaptor :
    public polymorphic_renderer_solid_rgba8_base
    {
    public:
      explicit polymorphic_renderer_solid_rgba8_adaptor(rendering_buffer &rbuf) : m_pixfmt(rbuf),
                                                                                  m_ren_base(m_pixfmt),
                                                                                  m_ren(m_ren_base)
      {}

      //--------------------------------------------------------------------
      void clear(const color_type &c) override
      {
        m_ren_base.clear(c);
        }

        void color(const color_type &c) override
        {
            m_ren.color(c);
        }

        color_type color() const override
        {
            return m_ren.color();
        }

        void prepare() override
        {
            m_ren.prepare();
        }

        void render(const scanline_type &sl) override
        {
            m_ren.render(sl);
        }

    private:
        PixFmt                m_pixfmt;
        renderer_base<PixFmt> m_ren_base;
        renderer_scanline_aa_solid<renderer_base<PixFmt> > m_ren;
    };


}







class the_application : public agg::platform_support
{
  double m_x[3]{};
  double m_y[3]{};

public:
    the_application(int format, bool flip_y) :
        agg::platform_support((agg::pix_format_e)format, flip_y)
    {
        m_x[0] = 100; m_y[0] = 60;
        m_x[1] = 369; m_y[1] = 170;
        m_x[2] = 143; m_y[2] = 310;
    }

    void on_draw() override
    {
        agg::path_storage path;
        path.move_to(m_x[0], m_y[0]);
        path.line_to(m_x[1], m_y[1]);
        path.line_to(m_x[2], m_y[2]);
        path.close_polygon();

        agg::polymorphic_renderer_solid_rgba8_base *ren = nullptr;

        //-- Polymorphic renderer class factory
        switch(pix_fmt)
        {
        case agg::pix_format_rgb555 : ren = new agg::polymorphic_renderer_solid_rgba8_adaptor<agg::pixfmt_rgb555>(rbuf_window()); break;
        case agg::pix_format_rgb565 : ren = new agg::polymorphic_renderer_solid_rgba8_adaptor<agg::pixfmt_rgb565>(rbuf_window()); break;
        case agg::pix_format_rgb24  : ren = new agg::polymorphic_renderer_solid_rgba8_adaptor<agg::pixfmt_rgb24 >(rbuf_window()); break;
        case agg::pix_format_bgr24  : ren = new agg::polymorphic_renderer_solid_rgba8_adaptor<agg::pixfmt_bgr24 >(rbuf_window()); break;
        case agg::pix_format_rgba32 : ren = new agg::polymorphic_renderer_solid_rgba8_adaptor<agg::pixfmt_rgba32>(rbuf_window()); break;
        case agg::pix_format_argb32 : ren = new agg::polymorphic_renderer_solid_rgba8_adaptor<agg::pixfmt_argb32>(rbuf_window()); break;
        case agg::pix_format_abgr32 : ren = new agg::polymorphic_renderer_solid_rgba8_adaptor<agg::pixfmt_abgr32>(rbuf_window()); break;
        case agg::pix_format_bgra32 : ren = new agg::polymorphic_renderer_solid_rgba8_adaptor<agg::pixfmt_bgra32>(rbuf_window()); break;
        }

        agg::rasterizer_scanline_aa<> ras;
        agg::scanline_p8 sl;
        if (ren != nullptr) {
          ren->clear(agg::srgba8(255, 255, 255));
          ren->color(agg::srgba8(80, 30, 20));
          ras.add_path(path);
          agg::render_scanlines(ras, sl, *ren);
        }
        delete ren;
    }

};


int agg_main(int /*argc*/, char * /*argv*/[])
{
  the_application app(pix_fmt, flip_y != 0U);
  app.caption("AGG Example. Polymorphic Renderers");

  if (app.init(400, 330, agg::window_resize)) {
    return app.run();
    }
    return 1;
}


