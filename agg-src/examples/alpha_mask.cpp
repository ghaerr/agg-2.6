#include <cstdlib>
#include <cctype>
#include <cstdio>
#include "agg_basics.h"
#include "agg_rendering_buffer.h"
#include "agg_rasterizer_scanline_aa.h"
#include "agg_path_storage.h"
#include "agg_conv_transform.h"
#include "agg_bounding_rect.h"
#include "agg_renderer_scanline.h"
#include "agg_pixfmt_rgb.h"
#include "agg_pixfmt_gray.h"
#include "agg_alpha_mask_u8.h"
#include "agg_scanline_u.h"
#include "agg_scanline_p.h"
#include "agg_ellipse.h"
#include "platform/agg_platform_support.h"

#define AGG_BGR24
//#define AGG_BGR48
//#define AGG_BGR96
#include "pixel_formats.h"

enum flip_y_e { flip_y = static_cast<unsigned int>(true) };

agg::path_storage g_path;
agg::srgba8        g_colors[100];
unsigned          g_path_idx[100];
unsigned          g_npaths = 0;
double            g_x1 = 0;
double            g_y1 = 0;
double            g_x2 = 0;
double            g_y2 = 0;
double            g_base_dx = 0;
double            g_base_dy = 0;
double            g_angle = 0;
double            g_scale = 1.0;
double            g_skew_x = 0;
double            g_skew_y = 0;
int               g_nclick = 0;


unsigned parse_lion(agg::path_storage& ps, agg::srgba8* colors, unsigned* path_idx);
void parse_lion()
{
    g_npaths = parse_lion(g_path, g_colors, g_path_idx);
    agg::pod_array_adaptor<unsigned> path_idx(g_path_idx, 100);
    agg::bounding_rect(g_path, path_idx, 0, g_npaths, &g_x1, &g_y1, &g_x2, &g_y2);
    g_base_dx = (g_x2 - g_x1) / 2.0;
    g_base_dy = (g_y2 - g_y1) / 2.0;
}


agg::rendering_buffer g_alpha_mask_rbuf;
agg::alpha_mask_gray8 g_alpha_mask(g_alpha_mask_rbuf);
agg::rasterizer_scanline_aa<> g_rasterizer;


class the_application : public agg::platform_support
{
    unsigned char* m_alpha_buf;
    agg::rendering_buffer m_alpha_rbuf;

public:
  ~the_application() override
  {
    delete[] m_alpha_buf;
    }

    the_application(agg::pix_format_e format, bool flip_y) : agg::platform_support(format, flip_y),
                                                             m_alpha_buf(nullptr)
    {
        parse_lion();
    }


    void generate_alpha_mask(int cx, int cy)
    {
        delete [] m_alpha_buf;
        m_alpha_buf = new unsigned char[cx * cy];
        g_alpha_mask_rbuf.attach(m_alpha_buf, cx, cy, cx);

        using ren_base = agg::renderer_base<agg::pixfmt_sgray8>;
        using renderer = agg::renderer_scanline_aa_solid<ren_base>;

        agg::pixfmt_sgray8 pixf(g_alpha_mask_rbuf);
        ren_base rb(pixf);
        renderer r(rb);
        agg::scanline_p8 sl;

        rb.clear(agg::sgray8(0));

        agg::ellipse ell;

        int i = 0;
        for(i = 0; i < 10; i++)
        {
            ell.init(rand() % cx, 
                     rand() % cy, 
                     rand() % 100 + 20, 
                     rand() % 100 + 20,
                     100);

            g_rasterizer.add_path(ell);
            r.color(agg::sgray8(rand() & 0xFF, rand() & 0xFF));
            agg::render_scanlines(g_rasterizer, sl, r);
        }
    }


    void on_resize(int cx, int cy) override
    {
        generate_alpha_mask(cx, cy);
    }

    void on_draw() override
    {
        int width = rbuf_window().width();
        int height = rbuf_window().height();

        using scanline_type = agg::scanline_u8_am<agg::alpha_mask_gray8>;
        using ren_base = agg::renderer_base<pixfmt>;
        using renderer = agg::renderer_scanline_aa_solid<ren_base>;

        pixfmt pixf(rbuf_window());
        ren_base rb(pixf);
        renderer r(rb);

        scanline_type sl(g_alpha_mask);
        rb.clear(agg::srgba8(255, 255, 255));

        agg::trans_affine mtx;
        mtx *= agg::trans_affine_translation(-g_base_dx, -g_base_dy);
        mtx *= agg::trans_affine_scaling(g_scale, g_scale);
        mtx *= agg::trans_affine_rotation(g_angle + agg::pi);
        mtx *= agg::trans_affine_skewing(g_skew_x/1000.0, g_skew_y/1000.0);
        mtx *= agg::trans_affine_translation(width/2, height/2);

        agg::conv_transform<agg::path_storage, agg::trans_affine> trans(g_path, mtx);

        agg::render_all_paths(g_rasterizer, sl, r, trans, g_colors, g_path_idx, g_npaths);
    }


    static void transform(double width, double height, double x, double y)
    {
        x -= width / 2;
        y -= height / 2;
        g_angle = atan2(y, x);
        g_scale = sqrt(y * y + x * x) / 100.0;
    }


    void on_mouse_button_down(int x, int y, unsigned flags) override
    {
      if ((flags & agg::mouse_left) != 0u) {
        int width = rbuf_window().width();
        int height = rbuf_window().height();
        transform(width, height, x, y);
        force_redraw();
      }

      if ((flags & agg::mouse_right) != 0u) {
        g_skew_x = x;
        g_skew_y = y;
        force_redraw();
      }
    }


    void on_mouse_move(int x, int y, unsigned flags) override
    {
        on_mouse_button_down(x, y, flags);
    }


};


int agg_main(int /*argc*/, char * /*argv*/[])
{
  the_application app(pix_format, flip_y != 0u);
  app.caption("AGG Example. Lion with Alpha-Masking");

  if (app.init(512, 400, agg::window_resize)) {
    return app.run();
    }
    return 1;
}






