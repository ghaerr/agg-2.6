#include <cstdlib>
#include <cctype>
#include <cstdio>
#include "agg_basics.h"
#include "agg_rendering_buffer.h"
#include "agg_rasterizer_scanline_aa.h"
#include "agg_scanline_u.h"
#include "agg_renderer_scanline.h"
#include "agg_path_storage.h"
#include "agg_conv_transform.h"
#include "agg_trans_affine.h"
#include "agg_trans_bilinear.h"
#include "agg_trans_perspective.h"
#include "agg_span_interpolator_linear.h"
#include "agg_span_interpolator_trans.h"
#include "agg_span_allocator.h"
#include "agg_image_accessors.h"
#include "ctrl/agg_rbox_ctrl.h"
#include "platform/agg_platform_support.h"
#include "interactive_polygon.h"

#define AGG_BGR24
#include "pixel_formats.h"

#include "agg_span_image_filter_rgb.h"
#define span_image_filter_2x2 agg::span_image_filter_rgb_2x2
#define span_image_filter_nn  agg::span_image_filter_rgb_nn

enum flip_y_e { flip_y = static_cast<unsigned int>(true) };

agg::rasterizer_scanline_aa<> g_rasterizer;
agg::scanline_u8  g_scanline;
double            g_x1 = 0;
double            g_y1 = 0;
double            g_x2 = 0;
double            g_y2 = 0;


class the_application : public agg::platform_support
{
public:
  using renderer_base = agg::renderer_base<pixfmt>;
  using renderer_solid = agg::renderer_scanline_aa_solid<renderer_base>;
  using renderer_base_pre = agg::renderer_base<pixfmt_pre>;

  agg::interactive_polygon m_quad;
  agg::rbox_ctrl<agg::rgba> m_trans_type;
  bool m_test_flag;

  the_application(agg::pix_format_e format, bool flip_y) : agg::platform_support(format, flip_y),
                                                           m_quad(4, 5.0),
                                                           m_trans_type(460, 5.0, 420 + 170.0, 60.0, !flip_y),
                                                           m_test_flag(false)
  {
    m_trans_type.text_size(8);
    m_trans_type.text_thickness(1);
    m_trans_type.add_item("Affine");
    m_trans_type.add_item("Bilinear");
    m_trans_type.add_item("Perspective");
    m_trans_type.cur_item(2);
    add_ctrl(m_trans_type);
    }

    void on_init() override
    {
        g_x1 = -150;
        g_y1 = -150;
        g_x2 =  150;
        g_y2 =  150;

        double trans_x1 = -200;
        double trans_y1 = -200;
        double trans_x2 =  200;
        double trans_y2 =  200;

        double dx = width()  / 2.0 - (trans_x2 + trans_x1) / 2.0;
        double dy = height() / 2.0 - (trans_y2 + trans_y1) / 2.0;
        m_quad.xn(0) = floor(trans_x1 + dx);
        m_quad.yn(0) = floor(trans_y1 + dy);
        m_quad.xn(1) = floor(trans_x2 + dx);
        m_quad.yn(1) = floor(trans_y1 + dy);
        m_quad.xn(2) = floor(trans_x2 + dx);
        m_quad.yn(2) = floor(trans_y2 + dy);
        m_quad.xn(3) = floor(trans_x1 + dx);
        m_quad.yn(3) = floor(trans_y2 + dy);
    }

    void on_draw() override
    {
        pixfmt            pixf(rbuf_window());
        pixfmt_pre        pixf_pre(rbuf_window());
        renderer_base     rb(pixf);
        renderer_base_pre rb_pre(pixf_pre);

        if(!m_test_flag)
        {
            rb.clear(agg::rgba(1, 1, 1));
        }

        if(m_trans_type.cur_item() == 0)
        {
            // For the affine parallelogram transformations we
            // calculate the 4-th (implicit) point of the parallelogram
            m_quad.xn(3) = m_quad.xn(0) + (m_quad.xn(2) - m_quad.xn(1));
            m_quad.yn(3) = m_quad.yn(0) + (m_quad.yn(2) - m_quad.yn(1));
        }

        if(!m_test_flag)
        {
            //--------------------------
            // Render the "quad" tool and controls
            g_rasterizer.add_path(m_quad);
            agg::render_scanlines_aa_solid(g_rasterizer, g_scanline, rb, agg::rgba(0, 0.3, 0.5, 0.6));

            //--------------------------
            agg::render_ctrl(g_rasterizer, g_scanline, rb, m_trans_type);
        }

        // Prepare the polygon to rasterize. Here we need to fill
        // the destination (transformed) polygon.
        g_rasterizer.clip_box(0, 0, width(), height());
        g_rasterizer.reset();
        g_rasterizer.move_to_d(m_quad.xn(0), m_quad.yn(0));
        g_rasterizer.line_to_d(m_quad.xn(1), m_quad.yn(1));
        g_rasterizer.line_to_d(m_quad.xn(2), m_quad.yn(2));
        g_rasterizer.line_to_d(m_quad.xn(3), m_quad.yn(3));


        using span_alloc_type = agg::span_allocator<color_type>;
        span_alloc_type sa;
        agg::image_filter<agg::image_filter_hanning> filter;

        using remainder_type = agg::wrap_mode_reflect_auto_pow2;
        using img_source_type = agg::image_accessor_wrap<pixfmt, remainder_type, remainder_type>;

        pixfmt img_pixf(rbuf_img(0));
        img_source_type img_src(img_pixf);

        enum subdiv_shift_e { subdiv_shift = 2 };
         
        switch(m_trans_type.cur_item())
        {
            case 0:
            {
                // Note that we consruct an affine matrix that transforms
                // a parallelogram to a rectangle, i.e., it's inverted.
                // It's actually the same as:
                // tr(g_x1, g_y1, g_x2, g_y2, m_triangle.polygon());
                // tr.invert();
                agg::trans_affine tr(m_quad.polygon(), g_x1, g_y1, g_x2, g_y2);

                // Also note that we can use the linear interpolator instead of 
                // arbitrary span_interpolator_trans. It works much faster, 
                // but the transformations must be linear and parellel.
                using interpolator_type = agg::span_interpolator_linear<>;
                interpolator_type interpolator(tr);

                using span_gen_type = agg::span_image_filter_rgb_2x2<img_source_type, interpolator_type>;
                span_gen_type sg(img_src, interpolator, filter);
                agg::render_scanlines_aa(g_rasterizer, g_scanline, rb_pre, sa, sg);
                break;
            }

            case 1:
            {
                agg::trans_bilinear tr(m_quad.polygon(), g_x1, g_y1, g_x2, g_y2);
                if(tr.is_valid())
                {
                  using interpolator_type = agg::span_interpolator_linear<agg::trans_bilinear>;
                  interpolator_type interpolator(tr);

                  using span_gen_type = agg::span_image_filter_rgb_2x2<img_source_type, interpolator_type>;
                  span_gen_type sg(img_src, interpolator, filter);
                  agg::render_scanlines_aa(g_rasterizer, g_scanline, rb_pre, sa, sg);
                }
                break;
            }

            case 2:
            {
                agg::trans_perspective tr(m_quad.polygon(), g_x1, g_y1, g_x2, g_y2);
                if(tr.is_valid())
                {
                  using interpolator_type = agg::span_interpolator_linear_subdiv<agg::trans_perspective, 8>;
                  interpolator_type interpolator(tr);

                  using span_gen_type = agg::span_image_filter_rgb_2x2<img_source_type, interpolator_type>;
                  span_gen_type sg(img_src, interpolator, filter);
                  agg::render_scanlines_aa(g_rasterizer, g_scanline, rb_pre, sa, sg);
                }
                break;
            }
        }
    }


    void on_mouse_button_down(int x, int y, unsigned flags) override
    {
      if ((flags & agg::mouse_left) != 0U) {
        if (m_quad.on_mouse_button_down(x, y)) {
          force_redraw();
        } else {
          start_timer();
          m_test_flag = true;
          on_draw();
          on_draw();
          on_draw();
          on_draw();
          char buf[100];
          sprintf(buf, "time=%.3f", elapsed_time());
          m_test_flag = false;
          force_redraw();
          message(buf);
        }
      }
    }


    void on_mouse_move(int x, int y, unsigned flags) override
    {
      if ((flags & agg::mouse_left) != 0U) {
        if (m_quad.on_mouse_move(x, y)) {
          force_redraw();
        }
      }
        if((flags & agg::mouse_left) == 0)
        {
            on_mouse_button_up(x, y, flags);
        }
    }


    void on_mouse_button_up(int x, int y, unsigned /*flags*/) override
    {
        if(m_quad.on_mouse_button_up(x, y))
        {
            force_redraw();
        }
    }

};






int agg_main(int argc, char* argv[])
{
  the_application app(pix_format, flip_y != 0U);
  app.caption("AGG Example. Pattern Perspective Transformations");

  const char *img_name = "agg";
  if (argc >= 2) {
    img_name = argv[1];
  }
    if(!app.load_img(0, img_name)) 
    {
        char buf[256];
        if(strcmp(img_name, "agg") == 0)
        {
          sprintf(buf,
            "File not found: %s%s. Download http://www.antigrain.com/%s%s\n"
            "or copy it from another directory if available.",
            img_name,
            the_application::img_ext(),
            img_name,
            the_application::img_ext());
        }
        else
        {
          sprintf(buf, "File not found: %s%s", img_name, the_application::img_ext());
        }
        the_application::message(buf);
        return 1;
    }
    
    if(app.init(600, 600, agg::window_resize))
    {
        return app.run();
    }
    return 1;
}






