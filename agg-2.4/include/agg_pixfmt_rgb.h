//----------------------------------------------------------------------------
// Anti-Grain Geometry - Version 2.4
// Copyright (C) 2002-2005 Maxim Shemanarev (http://www.antigrain.com)
//
// Permission to copy, use, modify, sell and distribute this software 
// is granted provided this copyright notice appears in all copies. 
// This software is provided "as is" without express or implied
// warranty, and with no claim as to its suitability for any purpose.
//
//----------------------------------------------------------------------------
// Contact: mcseem@antigrain.com
//          mcseemagg@yahoo.com
//          http://www.antigrain.com
//----------------------------------------------------------------------------
//
// Adaptation for high precision colors has been sponsored by 
// Liberty Technology Systems, Inc., visit http://lib-sys.com
//
// Liberty Technology Systems, Inc. is the provider of
// PostScript and PDF technology for software developers.
// 
//----------------------------------------------------------------------------

#ifndef AGG_PIXFMT_RGB_INCLUDED
#define AGG_PIXFMT_RGB_INCLUDED

#include <string.h>
#include "agg_pixfmt_base.h"
#include "agg_rendering_buffer.h"

namespace agg
{

    //=====================================================apply_gamma_dir_rgb
    template<class ColorT, class Order, class GammaLut> class apply_gamma_dir_rgb
    {
    public:
        typedef typename ColorT::value_type value_type;

        apply_gamma_dir_rgb(const GammaLut& gamma) : m_gamma(gamma) {}

        AGG_INLINE void operator () (value_type* p)
        {
            p[Order::R] = m_gamma.dir(p[Order::R]);
            p[Order::G] = m_gamma.dir(p[Order::G]);
            p[Order::B] = m_gamma.dir(p[Order::B]);
        }

    private:
        const GammaLut& m_gamma;
    };



    //=====================================================apply_gamma_inv_rgb
    template<class ColorT, class Order, class GammaLut> class apply_gamma_inv_rgb
    {
    public:
        typedef typename ColorT::value_type value_type;

        apply_gamma_inv_rgb(const GammaLut& gamma) : m_gamma(gamma) {}

        AGG_INLINE void operator () (value_type* p)
        {
            p[Order::R] = m_gamma.inv(p[Order::R]);
            p[Order::G] = m_gamma.inv(p[Order::G]);
            p[Order::B] = m_gamma.inv(p[Order::B]);
        }

    private:
        const GammaLut& m_gamma;
    };


    //=========================================================blender_rgb
    template<class ColorT, class Order> 
    struct blender_rgb : blender_base<ColorT, Order>
    {
        // Blend pixels using the non-premultiplied form of Alvy-Ray Smith's
        // compositing function. Since the render buffer is opaque we skip the
        // initial premultiply and final demultiply.

        //--------------------------------------------------------------------
        static AGG_INLINE void blend_pix(value_type* p, 
            value_type cr, value_type cg, value_type cb, value_type alpha, cover_type cover)
        {
            blend_pix(p, cr, cg, cb, mult_cover(alpha, cover));
        }
        
        //--------------------------------------------------------------------
        static AGG_INLINE void blend_pix(value_type* p, 
            value_type cr, value_type cg, value_type cb, value_type alpha)
        {
            p[Order::R] = lerp(p[Order::R], cr, alpha);
            p[Order::G] = lerp(p[Order::G], cg, alpha);
            p[Order::B] = lerp(p[Order::B], cb, alpha);
        }
    };

    //======================================================blender_rgb_pre
    template<class ColorT, class Order> 
    struct blender_rgb_pre : blender_base<ColorT, Order>
    {
        // Blend pixels using the premultiplied form of Alvy-Ray Smith's
        // compositing function. 

        //--------------------------------------------------------------------
        static AGG_INLINE void blend_pix(value_type* p, 
            value_type cr, value_type cg, value_type cb, value_type alpha, cover_type cover)
        {
            blend_pix(p, 
                mult_cover(cr, cover), 
                mult_cover(cg, cover), 
                mult_cover(cb, cover), 
                mult_cover(alpha, cover));
        }

        //--------------------------------------------------------------------
        static AGG_INLINE void blend_pix(value_type* p, 
            value_type cr, value_type cg, value_type cb, value_type alpha)
        {
            p[Order::R] = prelerp(p[Order::R], cr, alpha);
            p[Order::G] = prelerp(p[Order::G], cg, alpha);
            p[Order::B] = prelerp(p[Order::B], cb, alpha);
        }
    };

    //===================================================blender_rgb_gamma
    template<class ColorT, class Order, class Gamma> 
    class blender_rgb_gamma : public blender_base<ColorT, Order>
    {
    public:
        typedef Gamma gamma_type;

        //--------------------------------------------------------------------
        blender_rgb_gamma() : m_gamma(0) {}
        void gamma(const gamma_type& g) { m_gamma = &g; }

        //--------------------------------------------------------------------
        AGG_INLINE void blend_pix(value_type* p, 
            value_type cr, value_type cg, value_type cb, value_type alpha, cover_type cover)
        {
            blend_pix(p, cr, cg, cb, mult_cover(alpha, cover));
        }
        
        //--------------------------------------------------------------------
        AGG_INLINE void blend_pix(value_type* p, 
            value_type cr, value_type cg, value_type cb, value_type alpha)
        {
            calc_type r = m_gamma->dir(p[Order::R]);
            calc_type g = m_gamma->dir(p[Order::G]);
            calc_type b = m_gamma->dir(p[Order::B]);
            p[Order::R] = m_gamma->inv(lerp(r, m_gamma->dir(cr), alpha));
            p[Order::G] = m_gamma->inv(lerp(g, m_gamma->dir(cg), alpha));
            p[Order::B] = m_gamma->inv(lerp(b, m_gamma->dir(cb), alpha));
        }
        
    private:
        const gamma_type* m_gamma;
    };


    //==================================================pixfmt_alpha_blend_rgb
    template<class Blender, class RenBuf, unsigned Step, unsigned Offset = 0> 
    class pixfmt_alpha_blend_rgb
    {
    public:
        typedef pixfmt_rgb_tag pixfmt_category;
        typedef RenBuf   rbuf_type;
        typedef Blender  blender_type;
        typedef typename rbuf_type::row_data row_data;
        typedef typename blender_type::color_type color_type;
        typedef typename blender_type::order_type order_type;
        typedef typename color_type::value_type value_type;
        typedef typename color_type::calc_type calc_type;
        enum 
        {
            num_components = 3,
            pix_step = Step,
            pix_offset = Offset,
            pix_width = sizeof(value_type) * pix_step
        };
        struct pixel_type
        {
            value_type c[num_components];
        };

    private:
        //--------------------------------------------------------------------
        AGG_INLINE void copy_or_blend_pix(value_type* p, 
                                          const color_type& c, 
                                          unsigned cover)
        {
            if (!c.is_transparent())
            {
                if (c.is_opaque() && cover == cover_mask)
                {
                    p[order_type::R] = c.r;
                    p[order_type::G] = c.g;
                    p[order_type::B] = c.b;
                }
                else
                {
                    m_blender.blend_pix(p, c.r, c.g, c.b, c.a, cover);
                }
            }
        }

        //--------------------------------------------------------------------
        AGG_INLINE void copy_or_blend_pix(value_type* p, 
                                          const color_type& c)
        {
            if (!c.is_transparent())
            {
                if (c.is_opaque())
                {
                    p[order_type::R] = c.r;
                    p[order_type::G] = c.g;
                    p[order_type::B] = c.b;
                }
                else
                {
                    m_blender.blend_pix(p, c.r, c.g, c.b, c.a);
                }
            }
        }

    public:
        //--------------------------------------------------------------------
        explicit pixfmt_alpha_blend_rgb(rbuf_type& rb) :
            m_rbuf(&rb)
        {}
        void attach(rbuf_type& rb) { m_rbuf = &rb; }

        //--------------------------------------------------------------------
        template<class PixFmt>
        bool attach(PixFmt& pixf, int x1, int y1, int x2, int y2)
        {
            rect_i r(x1, y1, x2, y2);
            if (r.clip(rect_i(0, 0, pixf.width()-1, pixf.height()-1)))
            {
                int stride = pixf.stride();
                m_rbuf->attach(pixf.pix_ptr(r.x1, stride < 0 ? r.y2 : r.y1), 
                               (r.x2 - r.x1) + 1,
                               (r.y2 - r.y1) + 1,
                               stride);
                return true;
            }
            return false;
        }

        //--------------------------------------------------------------------
        Blender& blender() { return m_blender; }

        //--------------------------------------------------------------------
        AGG_INLINE unsigned width()  const { return m_rbuf->width();  }
        AGG_INLINE unsigned height() const { return m_rbuf->height(); }
        AGG_INLINE int      stride() const { return m_rbuf->stride(); }

        //--------------------------------------------------------------------
        AGG_INLINE       int8u* row_ptr(int y)       { return m_rbuf->row_ptr(y); }
        AGG_INLINE const int8u* row_ptr(int y) const { return m_rbuf->row_ptr(y); }
        AGG_INLINE row_data     row(int y)     const { return m_rbuf->row(y); }

        //--------------------------------------------------------------------
        AGG_INLINE int8u* pix_ptr(int x, int y) 
        { 
            return m_rbuf->row_ptr(y) + x * pix_width; 
        }

        AGG_INLINE const int8u* pix_ptr(int x, int y) const 
        { 
            return m_rbuf->row_ptr(y) + x * pix_width; 
        }

        // Return pointer to pixel value, or null if row not allocated.
        AGG_INLINE const value_type* pix_value_ptr(int x, int y) const 
        {
            return (const value_type*)m_rbuf->row_ptr(y) + x * pix_step;
        }

        // Return pointer to pixel value, forcing row to be allocated.
        AGG_INLINE value_type* pix_value_ptr(int x, int y, unsigned len) 
        {
            return (value_type*)m_rbuf->row_ptr(x, y, len) + x * pix_step;
        }

        //--------------------------------------------------------------------
        AGG_INLINE static void set_plain_color(value_type* p, color_type c)
        {
            // RGB formats are implicitly premultiplied.
            c.premultiply();
            p[order_type::R] = c.r;
            p[order_type::G] = c.g;
            p[order_type::B] = c.b;
        }

        //--------------------------------------------------------------------
        AGG_INLINE static color_type get_plain_color(const value_type* p)
        {
            return color_type(
                p[order_type::R],
                p[order_type::G],
                p[order_type::B]);
        }

        //--------------------------------------------------------------------
        AGG_INLINE static void make_pix(value_type* p, const color_type& c)
        {
            p[order_type::R] = c.r;
            p[order_type::G] = c.g;
            p[order_type::B] = c.b;
        }

        //--------------------------------------------------------------------
        AGG_INLINE static color_type make_color(const value_type* p)
        {
            return color_type(
                p[order_type::R], 
                p[order_type::G], 
                p[order_type::B]);
        }

        //--------------------------------------------------------------------
        AGG_INLINE color_type pixel(int x, int y) const
        {
            if (const value_type* p = pix_value_ptr(x, y))
            {
                return make_color(p);
            }
            return color_type::no_color();
        }

        //--------------------------------------------------------------------
        AGG_INLINE void copy_pixel(int x, int y, const color_type& c)
        {
            make_pix(pix_value_ptr(x, y, 1), c);
        }

        //--------------------------------------------------------------------
        AGG_INLINE void blend_pixel(int x, int y, const color_type& c, int8u cover)
        {
            copy_or_blend_pix(pix_value_ptr(x, y, 1), c, cover);
        }

        //--------------------------------------------------------------------
        AGG_INLINE void copy_hline(int x, int y, 
                                   unsigned len, 
                                   const color_type& c)
        {
            value_type* p = pix_value_ptr(x, y, len);
            do
            {
                p[order_type::R] = c.r; 
                p[order_type::G] = c.g; 
                p[order_type::B] = c.b;
                p += pix_step;
            }
            while(--len);
        }


        //--------------------------------------------------------------------
        AGG_INLINE void copy_vline(int x, int y,
                                   unsigned len, 
                                   const color_type& c)
        {
            do
            {
                value_type* p = pix_value_ptr(x, y++, 1);
                p[order_type::R] = c.r; 
                p[order_type::G] = c.g; 
                p[order_type::B] = c.b;
            }
            while (--len);
        }

        //--------------------------------------------------------------------
        void blend_hline(int x, int y,
                         unsigned len, 
                         const color_type& c,
                         int8u cover)
        {
            if (!c.is_transparent())
            {
                value_type* p = pix_value_ptr(x, y, len);

                if (c.is_opaque() && cover == cover_mask)
                {
                    do
                    {
                        p[order_type::R] = c.r; 
                        p[order_type::G] = c.g; 
                        p[order_type::B] = c.b;
                        p += pix_step;
                    }
                    while (--len);
                }
                else
                {
                    do
                    {
                        m_blender.blend_pix(p, c.r, c.g, c.b, c.a, cover);
                        p += pix_step;
                    }
                    while (--len);
                }
            }
        }


        //--------------------------------------------------------------------
        void blend_vline(int x, int y,
                         unsigned len, 
                         const color_type& c,
                         int8u cover)
        {
            if (!c.is_transparent())
            {
                if (c.is_opaque() && cover == cover_mask)
                {
                    do
                    {
                        value_type* p = pix_value_ptr(x, y++, 1);

                        p[order_type::R] = c.r; 
                        p[order_type::G] = c.g; 
                        p[order_type::B] = c.b;
                    }
                    while (--len);
                }
                else
                {
                    do
                    {
                        value_type* p = pix_value_ptr(x, y++, 1);

                        m_blender.blend_pix(p, c.r, c.g, c.b, c.a, cover);
                    }
                    while (--len);
                }
            }
        }

        //--------------------------------------------------------------------
        void blend_solid_hspan(int x, int y,
                               unsigned len, 
                               const color_type& c,
                               const int8u* covers)
        {
            if (!c.is_transparent())
            {
                value_type* p = pix_value_ptr(x, y, len);

                do 
                {
                    if (c.is_opaque() && *covers == cover_mask)
                    {
                        p[order_type::R] = c.r;
                        p[order_type::G] = c.g;
                        p[order_type::B] = c.b;
                    }
                    else
                    {
                        m_blender.blend_pix(p, c.r, c.g, c.b, c.a, *covers);
                    }
                    p += pix_step;
                    ++covers;
                }
                while (--len);
            }
        }


        //--------------------------------------------------------------------
        void blend_solid_vspan(int x, int y,
                               unsigned len, 
                               const color_type& c,
                               const int8u* covers)
        {
            if (!c.is_transparent())
            {
                do 
                {
                    value_type* p = pix_value_ptr(x, y++, 1);

                    if (c.is_opaque() && *covers == cover_mask)
                    {
                        p[order_type::R] = c.r;
                        p[order_type::G] = c.g;
                        p[order_type::B] = c.b;
                    }
                    else
                    {
                        m_blender.blend_pix(p, c.r, c.g, c.b, c.a, *covers);
                    }
                    ++covers;
                }
                while (--len);
            }
        }

        //--------------------------------------------------------------------
        void copy_color_hspan(int x, int y,
                              unsigned len, 
                              const color_type* colors)
        {
            value_type* p = pix_value_ptr(x, y, len);

            do 
            {
                p[order_type::R] = colors->r;
                p[order_type::G] = colors->g;
                p[order_type::B] = colors->b;
                ++colors;
                p += pix_step;
            }
            while (--len);
        }


        //--------------------------------------------------------------------
        void copy_color_vspan(int x, int y,
                              unsigned len, 
                              const color_type* colors)
        {
            do 
            {
                value_type* p = pix_value_ptr(x, y++, 1);
                p[order_type::R] = colors->r;
                p[order_type::G] = colors->g;
                p[order_type::B] = colors->b;
                ++colors;
            }
            while (--len);
        }

        //--------------------------------------------------------------------
        void blend_color_hspan(int x, int y,
                               unsigned len, 
                               const color_type* colors,
                               const int8u* covers,
                               int8u cover)
        {
            value_type* p = pix_value_ptr(x, y, len);

            if (covers)
            {
                do 
                {
                    copy_or_blend_pix(p, *colors++, *covers++);
                    p += pix_step;
                }
                while (--len);
            }
            else
            {
                if (cover == cover_mask)
                {
                    do 
                    {
                        copy_or_blend_pix(p, *colors++);
                        p += pix_step;
                    }
                    while (--len);
                }
                else
                {
                    do 
                    {
                        copy_or_blend_pix(p, *colors++, cover);
                        p += pix_step;
                    }
                    while (--len);
                }
            }
        }

        //--------------------------------------------------------------------
        void blend_color_vspan(int x, int y,
                               unsigned len, 
                               const color_type* colors,
                               const int8u* covers,
                               int8u cover)
        {
            if (covers)
            {
                do 
                {
                    copy_or_blend_pix(pix_value_ptr(x, y++, 1), *colors++, *covers++);
                }
                while (--len);
            }
            else
            {
                if (cover == cover_mask)
                {
                    do 
                    {
                        copy_or_blend_pix(pix_value_ptr(x, y++, 1), *colors++);
                    }
                    while (--len);
                }
                else
                {
                    do 
                    {
                        copy_or_blend_pix(pix_value_ptr(x, y++, 1), *colors++, cover);
                    }
                    while (--len);
                }
            }
        }

        //--------------------------------------------------------------------
        template<class Function> void for_each_pixel(Function f)
        {
            for (unsigned y = 0; y < height(); ++y)
            {
                row_data r = m_rbuf->row(y);
                if (r.ptr)
                {
                    unsigned len = r.x2 - r.x1 + 1;
                    value_type* p = pix_value_ptr(r.x1, y, len);
                    do
                    {
                        f(p);
                        p += pix_step;
                    }
                    while (--len);
                }
            }
        }

        //--------------------------------------------------------------------
        template<class GammaLut> void apply_gamma_dir(const GammaLut& g)
        {
            for_each_pixel(apply_gamma_dir_rgb<color_type, order_type, GammaLut>(g));
        }

        //--------------------------------------------------------------------
        template<class GammaLut> void apply_gamma_inv(const GammaLut& g)
        {
            for_each_pixel(apply_gamma_inv_rgb<color_type, order_type, GammaLut>(g));
        }

        //--------------------------------------------------------------------
        template<class RenBuf2>
        void copy_from(const RenBuf2& from, 
                       int xdst, int ydst,
                       int xsrc, int ysrc,
                       unsigned len)
        {
            if (const int8u* p = from.row_ptr(ysrc))
            {
                memmove(m_rbuf->row_ptr(xdst, ydst, len) + xdst * pix_width, 
                        p + xsrc * pix_width, 
                        len * pix_width);
            }
        }

        //--------------------------------------------------------------------
        // Blend from an RGBA surface.
        template<class SrcPixelFormatRenderer>
        void blend_from(const SrcPixelFormatRenderer& from, 
                        int xdst, int ydst,
                        int xsrc, int ysrc,
                        unsigned len,
                        int8u cover)
        {
            typedef typename SrcPixelFormatRenderer::value_type src_value_type;
            typedef typename SrcPixelFormatRenderer::order_type src_order;

            if (const src_value_type* psrc = from.pix_value_ptr(xsrc, ysrc))
            {
                value_type* pdst = pix_value_ptr(xdst, ydst, len);
                int srcinc = SrcPixelFormatRenderer::pix_step;
                int dstinc = pix_step;

                if (cover == cover_mask)
                {
                    do 
                    {
                        value_type alpha = psrc[src_order::A];
                        if (!is_empty(alpha))
                        {
                            if (is_full(alpha))
                            {
                                pdst[order_type::R] = psrc[src_order::R];
                                pdst[order_type::G] = psrc[src_order::G];
                                pdst[order_type::B] = psrc[src_order::B];
                            }
                            else
                            {
                                m_blender.blend_pix(pdst, 
                                    psrc[src_order::R],
                                    psrc[src_order::G],
                                    psrc[src_order::B],
                                    alpha);
                            }
                        }
                        psrc += srcinc;
                        pdst += dstinc;
                    }
                    while(--len);
                }
                else
                {
                    color_type color;
                    do 
                    {
                        color.r = psrc[src_order::R];
                        color.g = psrc[src_order::G];
                        color.b = psrc[src_order::B];
                        color.a = psrc[src_order::A];
                        copy_or_blend_pix(pdst, color, cover);
                        psrc += srcinc;
                        pdst += dstinc;
                    }
                    while (--len);
                }
            }
        }

        //--------------------------------------------------------------------
        // Blend from single color, using grayscale surface as alpha channel.
        template<class SrcPixelFormatRenderer>
        void blend_from_color(const SrcPixelFormatRenderer& from, 
                              const color_type& color,
                              int xdst, int ydst,
                              int xsrc, int ysrc,
                              unsigned len,
                              int8u cover)
        {
            typedef typename SrcPixelFormatRenderer::value_type src_value_type;
            typedef typename SrcPixelFormatRenderer::color_type src_color_type;

            if (const src_value_type* psrc = from.pix_value_ptr(xsrc, ysrc))
            {
                value_type* pdst = pix_value_ptr(xdst, ydst, len);
                int srcinc = SrcPixelFormatRenderer::pix_step;
                int dstinc = pix_step;

                do 
                {
                    copy_or_blend_pix(pdst, color, src_color_type::scale_cover(cover, *psrc));
                    psrc += srcinc;
                    pdst += dstinc;
                }
                while (--len);
            }
        }

        //--------------------------------------------------------------------
        // Blend from color table, using grayscale surface as indexes into table. 
        // Obviously, this only works for integer value types.
        template<class SrcPixelFormatRenderer>
        void blend_from_lut(const SrcPixelFormatRenderer& from, 
                            const color_type* color_lut,
                            int xdst, int ydst,
                            int xsrc, int ysrc,
                            unsigned len,
                            int8u cover)
        {
            typedef typename SrcPixelFormatRenderer::value_type src_value_type;

            if (const src_value_type* psrc = from.pix_value_ptr(xsrc, ysrc))
            {
                value_type* pdst = pix_value_ptr(xdst, ydst, len);
                int srcinc = SrcPixelFormatRenderer::pix_step;
                int dstinc = pix_step;

                if (cover == cover_mask)
                {
                    do 
                    {
                        const color_type& color = color_lut[*psrc];
                        m_blender.blend_pix(pdst, color.r, color.g, color.b, color.a);
                        psrc += srcinc;
                        pdst += dstinc;
                    }
                    while(--len);
                }
                else
                {
                    do 
                    {
                        copy_or_blend_pix(pdst, color_lut[*psrc], cover);
                        psrc += srcinc;
                        pdst += dstinc;
                    }
                    while(--len);
                }
            }
        }

    private:
        rbuf_type* m_rbuf;
        Blender    m_blender;
    };
    
    //-----------------------------------------------------------------------
    typedef blender_rgb<rgba8, order_rgb> blender_rgb24;
    typedef blender_rgb<rgba8, order_bgr> blender_bgr24;
    typedef blender_rgb<rgba16, order_rgb> blender_rgb48;
    typedef blender_rgb<rgba16, order_bgr> blender_bgr48;
    typedef blender_rgb<rgba32, order_rgb> blender_rgb96;
    typedef blender_rgb<rgba32, order_bgr> blender_bgr96;

    typedef blender_rgb_pre<rgba8, order_rgb> blender_rgb24_pre;
    typedef blender_rgb_pre<rgba8, order_bgr> blender_bgr24_pre;
    typedef blender_rgb_pre<rgba16, order_rgb> blender_rgb48_pre;
    typedef blender_rgb_pre<rgba16, order_bgr> blender_bgr48_pre;
    typedef blender_rgb_pre<rgba32, order_rgb> blender_rgb96_pre;
    typedef blender_rgb_pre<rgba32, order_bgr> blender_bgr96_pre;

    typedef blender_rgb<rgba8, order_rgba> blender_rgbx32;
    typedef blender_rgb<rgba8, order_argb> blender_xrgb32;
    typedef blender_rgb<rgba8, order_abgr> blender_xbgr32;
    typedef blender_rgb<rgba8, order_bgra> blender_bgrx32;
    typedef blender_rgb<rgba16, order_rgba> blender_rgbx64;
    typedef blender_rgb<rgba16, order_argb> blender_xrgb64;
    typedef blender_rgb<rgba16, order_abgr> blender_xbgr64;
    typedef blender_rgb<rgba16, order_bgra> blender_bgrx64;

    typedef blender_rgb_pre<rgba8, order_rgba> blender_rgbx32_pre;
    typedef blender_rgb_pre<rgba8, order_argb> blender_xrgb32_pre;
    typedef blender_rgb_pre<rgba8, order_abgr> blender_xbgr32_pre;
    typedef blender_rgb_pre<rgba8, order_bgra> blender_bgrx32_pre;
    typedef blender_rgb_pre<rgba16, order_rgba> blender_rgbx64_pre;
    typedef blender_rgb_pre<rgba16, order_argb> blender_xrgb64_pre;
    typedef blender_rgb_pre<rgba16, order_abgr> blender_xbgr64_pre;
    typedef blender_rgb_pre<rgba16, order_bgra> blender_bgrx64_pre;

    typedef pixfmt_alpha_blend_rgb<blender_rgb24, rendering_buffer, 3> pixfmt_rgb24;
    typedef pixfmt_alpha_blend_rgb<blender_bgr24, rendering_buffer, 3> pixfmt_bgr24;
    typedef pixfmt_alpha_blend_rgb<blender_rgb48, rendering_buffer, 3> pixfmt_rgb48;
    typedef pixfmt_alpha_blend_rgb<blender_bgr48, rendering_buffer, 3> pixfmt_bgr48;
    typedef pixfmt_alpha_blend_rgb<blender_rgb96, rendering_buffer, 3> pixfmt_rgb96;
    typedef pixfmt_alpha_blend_rgb<blender_bgr96, rendering_buffer, 3> pixfmt_bgr96;

    typedef pixfmt_alpha_blend_rgb<blender_rgb24_pre, rendering_buffer, 3> pixfmt_rgb24_pre;
    typedef pixfmt_alpha_blend_rgb<blender_bgr24_pre, rendering_buffer, 3> pixfmt_bgr24_pre;
    typedef pixfmt_alpha_blend_rgb<blender_rgb48_pre, rendering_buffer, 3> pixfmt_rgb48_pre;
    typedef pixfmt_alpha_blend_rgb<blender_bgr48_pre, rendering_buffer, 3> pixfmt_bgr48_pre;
    typedef pixfmt_alpha_blend_rgb<blender_rgb96_pre, rendering_buffer, 3> pixfmt_rgb96_pre;
    typedef pixfmt_alpha_blend_rgb<blender_bgr96_pre, rendering_buffer, 3> pixfmt_bgr96_pre;

    typedef pixfmt_alpha_blend_rgb<blender_rgbx32, rendering_buffer, 4> pixfmt_rgbx32;
    typedef pixfmt_alpha_blend_rgb<blender_xrgb32, rendering_buffer, 4> pixfmt_xrgb32;
    typedef pixfmt_alpha_blend_rgb<blender_xbgr32, rendering_buffer, 4> pixfmt_xbgr32;
    typedef pixfmt_alpha_blend_rgb<blender_bgrx32, rendering_buffer, 4> pixfmt_bgrx32;
    typedef pixfmt_alpha_blend_rgb<blender_rgbx64, rendering_buffer, 4> pixfmt_rgbx64;
    typedef pixfmt_alpha_blend_rgb<blender_xrgb64, rendering_buffer, 4> pixfmt_xrgb64;
    typedef pixfmt_alpha_blend_rgb<blender_xbgr64, rendering_buffer, 4> pixfmt_xbgr64;
    typedef pixfmt_alpha_blend_rgb<blender_bgrx64, rendering_buffer, 4> pixfmt_bgrx64;

    typedef pixfmt_alpha_blend_rgb<blender_rgbx32_pre, rendering_buffer, 4> pixfmt_rgbx32_pre;
    typedef pixfmt_alpha_blend_rgb<blender_xrgb32_pre, rendering_buffer, 4> pixfmt_xrgb32_pre;
    typedef pixfmt_alpha_blend_rgb<blender_xbgr32_pre, rendering_buffer, 4> pixfmt_xbgr32_pre;
    typedef pixfmt_alpha_blend_rgb<blender_bgrx32_pre, rendering_buffer, 4> pixfmt_bgrx32_pre;
    typedef pixfmt_alpha_blend_rgb<blender_rgbx64_pre, rendering_buffer, 4> pixfmt_rgbx64_pre;
    typedef pixfmt_alpha_blend_rgb<blender_xrgb64_pre, rendering_buffer, 4> pixfmt_xrgb64_pre;
    typedef pixfmt_alpha_blend_rgb<blender_xbgr64_pre, rendering_buffer, 4> pixfmt_xbgr64_pre;
    typedef pixfmt_alpha_blend_rgb<blender_bgrx64_pre, rendering_buffer, 4> pixfmt_bgrx64_pre;
    

    //-----------------------------------------------------pixfmt_rgb24_gamma
    template<class Gamma> class pixfmt_rgb24_gamma : 
    public pixfmt_alpha_blend_rgb<blender_rgb_gamma<rgba8, order_rgb, Gamma>, rendering_buffer, 3>
    {
    public:
        pixfmt_rgb24_gamma(rendering_buffer& rb, const Gamma& g) :
            pixfmt_alpha_blend_rgb<blender_rgb_gamma<rgba8, order_rgb, Gamma>, rendering_buffer, 3>(rb) 
        {
            this->blender().gamma(g);
        }
    };
        
    //-----------------------------------------------------pixfmt_bgr24_gamma
    template<class Gamma> class pixfmt_bgr24_gamma : 
    public pixfmt_alpha_blend_rgb<blender_rgb_gamma<rgba8, order_bgr, Gamma>, rendering_buffer, 3>
    {
    public:
        pixfmt_bgr24_gamma(rendering_buffer& rb, const Gamma& g) :
            pixfmt_alpha_blend_rgb<blender_rgb_gamma<rgba8, order_bgr, Gamma>, rendering_buffer, 3>(rb) 
        {
            this->blender().gamma(g);
        }
    };

    //-----------------------------------------------------pixfmt_rgb48_gamma
    template<class Gamma> class pixfmt_rgb48_gamma : 
    public pixfmt_alpha_blend_rgb<blender_rgb_gamma<rgba16, order_rgb, Gamma>, rendering_buffer, 3>
    {
    public:
        pixfmt_rgb48_gamma(rendering_buffer& rb, const Gamma& g) :
            pixfmt_alpha_blend_rgb<blender_rgb_gamma<rgba16, order_rgb, Gamma>, rendering_buffer, 3>(rb) 
        {
            this->blender().gamma(g);
        }
    };
        
    //-----------------------------------------------------pixfmt_bgr48_gamma
    template<class Gamma> class pixfmt_bgr48_gamma : 
    public pixfmt_alpha_blend_rgb<blender_rgb_gamma<rgba16, order_bgr, Gamma>, rendering_buffer, 3>
    {
    public:
        pixfmt_bgr48_gamma(rendering_buffer& rb, const Gamma& g) :
            pixfmt_alpha_blend_rgb<blender_rgb_gamma<rgba16, order_bgr, Gamma>, rendering_buffer, 3>(rb) 
        {
            this->blender().gamma(g);
        }
    };
    
}

#endif

