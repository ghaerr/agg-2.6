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

#ifndef AGG_PIXFMT_GRAY_INCLUDED
#define AGG_PIXFMT_GRAY_INCLUDED

#include <string.h>
#include "agg_pixfmt_base.h"
#include "agg_rendering_buffer.h"

namespace agg
{
 
    //============================================================blender_gray
    template<class ColorT> struct blender_gray : blender_base<ColorT>
    {
        // Blend pixels using the non-premultiplied form of Alvy-Ray Smith's
        // compositing function. Since the render buffer is opaque we skip the
        // initial premultiply and final demultiply.

        static AGG_INLINE void blend_pix(value_type* p, 
            value_type cv, value_type alpha, cover_type cover)
        {
            blend_pix(p, cv, mult_cover(alpha, cover));
        }

        static AGG_INLINE void blend_pix(value_type* p, 
            value_type cv, value_type alpha)
        {
            *p = lerp(*p, cv, alpha);
        }
    };


    //======================================================blender_gray_pre
    template<class ColorT> struct blender_gray_pre : blender_base<ColorT>
    {
        // Blend pixels using the premultiplied form of Alvy-Ray Smith's
        // compositing function. 

        static AGG_INLINE void blend_pix(value_type* p, 
            value_type cv, value_type alpha, cover_type cover)
        {
            blend_pix(p, mult_cover(cv, cover), mult_cover(alpha, cover));
        }

        static AGG_INLINE void blend_pix(value_type* p, 
            value_type cv, value_type alpha)
        {
            *p = prelerp(*p, cv, alpha);
        }
    };
    


    //=====================================================apply_gamma_dir_gray
    template<class ColorT, class GammaLut> class apply_gamma_dir_gray
    {
    public:
        typedef typename ColorT::value_type value_type;

        apply_gamma_dir_gray(const GammaLut& gamma) : m_gamma(gamma) {}

        AGG_INLINE void operator () (value_type* p)
        {
            *p = m_gamma.dir(*p);
        }

    private:
        const GammaLut& m_gamma;
    };



    //=====================================================apply_gamma_inv_gray
    template<class ColorT, class GammaLut> class apply_gamma_inv_gray
    {
    public:
        typedef typename ColorT::value_type value_type;

        apply_gamma_inv_gray(const GammaLut& gamma) : m_gamma(gamma) {}

        AGG_INLINE void operator () (value_type* p)
        {
            *p = m_gamma.inv(*p);
        }

    private:
        const GammaLut& m_gamma;
    };



    //=================================================pixfmt_alpha_blend_gray
    template<class Blender, class RenBuf, unsigned Step = 1, unsigned Offset = 0>
    class pixfmt_alpha_blend_gray
    {
    public:
        typedef pixfmt_gray_tag pixfmt_category;
        typedef RenBuf   rbuf_type;
        typedef typename rbuf_type::row_data row_data;
        typedef Blender  blender_type;
        typedef typename blender_type::color_type color_type;
        typedef int                               order_type; // A fake one
        typedef typename color_type::value_type   value_type;
        typedef typename color_type::calc_type    calc_type;
        enum 
        {
            num_components = 1,
            pix_width = sizeof(value_type) * Step,
            pix_step = Step,
            pix_offset = Offset,
        };
        struct pixel_type
        {
            value_type c[num_components];
        };

    private:
        //--------------------------------------------------------------------
        static AGG_INLINE void copy_or_blend_pix(value_type* p, 
                                                 const color_type& c, 
                                                 unsigned cover)
        {
            if (!c.is_transparent())
            {
                if (c.is_opaque() && cover == cover_mask)
                {
                    *p = c.v;
                }
                else
                {
                    Blender::blend_pix(p, c.v, c.a, cover);
                }
            }
        }


        static AGG_INLINE void copy_or_blend_pix(value_type* p, 
                                                 const color_type& c)
        {
            if (!c.is_transparent())
            {
                if (c.is_opaque())
                {
                    *p = c.v;
                }
                else
                {
                    Blender::blend_pix(p, c.v, c.a);
                }
            }
        }


    public:
        //--------------------------------------------------------------------
        explicit pixfmt_alpha_blend_gray(rbuf_type& rb) :
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
        AGG_INLINE unsigned width()  const { return m_rbuf->width();  }
        AGG_INLINE unsigned height() const { return m_rbuf->height(); }
        AGG_INLINE int      stride() const { return m_rbuf->stride(); }

        //--------------------------------------------------------------------
        int8u* row_ptr(int y)       { return m_rbuf->row_ptr(y); }
        const int8u* row_ptr(int y) const { return m_rbuf->row_ptr(y); }
        row_data     row(int y)     const { return m_rbuf->row(y); }

        const int8u* pix_ptr(int x, int y) const
        {
            return m_rbuf->row_ptr(y) + x * pix_step + pix_offset;
        }

        int8u* pix_ptr(int x, int y)
        {
            return m_rbuf->row_ptr(y) + x * pix_step + pix_offset;
        }

        // Return pointer to pixel value, or null if row not allocated.
        AGG_INLINE const value_type* pix_value_ptr(int x, int y) const 
        {
            return (const value_type*)m_rbuf->row_ptr(y) + x * pix_step + pix_offset;
        }

        // Return pointer to pixel value, forcing row to be allocated.
        AGG_INLINE value_type* pix_value_ptr(int x, int y, unsigned len) 
        {
            return (value_type*)m_rbuf->row_ptr(x, y, len) + x * pix_step + pix_offset;
        }

        //--------------------------------------------------------------------
        AGG_INLINE static void set_plain_color(value_type* p, color_type c)
        {
            // Grayscale formats are implicitly premultiplied.
            c.premultiply();
            *p = c.v;
        }

        //--------------------------------------------------------------------
        AGG_INLINE static color_type get_plain_color(const value_type* p)
        {
            return color_type(*p);
        }

        //--------------------------------------------------------------------
        AGG_INLINE static void make_pix(value_type* p, const color_type& c)
        {
            *p = c.v;
        }

        //--------------------------------------------------------------------
        AGG_INLINE static color_type make_color(const value_type* p)
        {
            return color_type(*p);
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
            *pix_value_ptr(x, y, 1) = c.v;
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
                *p = c.v; 
                p += pix_step;
            }
            while (--len);
        }


        //--------------------------------------------------------------------
        AGG_INLINE void copy_vline(int x, int y,
                                   unsigned len, 
                                   const color_type& c)
        {
            do
            {
                value_type* p = pix_value_ptr(x, y++, 1);
                *p = c.v;
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
                        *p = c.v; 
                        p += pix_step;
                    }
                    while (--len);
                }
                else
                {
                    do
                    {
                        Blender::blend_pix(p, c.v, c.a, cover);
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
                        *pix_value_ptr(x, y++, 1) = c.v; 
                    }
                    while (--len);
                }
                else
                {
                    do
                    {
                        Blender::blend_pix(pix_value_ptr(x, y++, 1), c.v, c.a, cover);
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
                        *p = c.v;
                    }
                    else
                    {
                        Blender::blend_pix(p, c.v, c.a, *covers);
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
                        *p = c.v;
                    }
                    else
                    {
                        Blender::blend_pix(p, c.v, c.a, *covers);
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
                *p = colors->v;
                p += pix_step;
                ++colors;
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
                *p = colors->v;
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
                        if (colors->is_opaque())
                        {
                            *p = colors->v;
                        }
                        else
                        {
                            copy_or_blend_pix(p, *colors);
                        }
                        p += pix_step;
                        ++colors;
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
                        value_type* p = pix_value_ptr(x, y++, 1);

                        if (colors->is_opaque())
                        {
                            *p = colors->v;
                        }
                        else
                        {
                            copy_or_blend_pix(p, *colors);
                        }
                        ++colors;
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
            unsigned y;
            for (y = 0; y < height(); ++y)
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
            for_each_pixel(apply_gamma_dir_gray<color_type, GammaLut>(g));
        }

        //--------------------------------------------------------------------
        template<class GammaLut> void apply_gamma_inv(const GammaLut& g)
        {
            for_each_pixel(apply_gamma_inv_gray<color_type, GammaLut>(g));
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

                do 
                {
                    copy_or_blend_pix(pdst, color, src_color_type::scale_cover(cover, *psrc));
                    psrc += SrcPixelFormatRenderer::pix_step;
                    pdst += pix_step;
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

                do 
                {
                    copy_or_blend_pix(pdst, color_lut[*psrc], cover);
                    psrc += SrcPixelFormatRenderer::pix_step;
                    pdst += pix_step;
                }
                while (--len);
            }
        }

    private:
        rbuf_type* m_rbuf;
    };

    typedef blender_gray<gray8> blender_gray8;
    typedef blender_gray<gray16> blender_gray16;
    typedef blender_gray<gray32> blender_gray32;

    typedef blender_gray_pre<gray8> blender_gray8_pre;
    typedef blender_gray_pre<gray16> blender_gray16_pre;
    typedef blender_gray_pre<gray32> blender_gray32_pre;

    typedef pixfmt_alpha_blend_gray<blender_gray8, rendering_buffer> pixfmt_gray8;
    typedef pixfmt_alpha_blend_gray<blender_gray16, rendering_buffer> pixfmt_gray16;
    typedef pixfmt_alpha_blend_gray<blender_gray32, rendering_buffer> pixfmt_gray32;

    typedef pixfmt_alpha_blend_gray<blender_gray8_pre, rendering_buffer> pixfmt_gray8_pre;
    typedef pixfmt_alpha_blend_gray<blender_gray16_pre, rendering_buffer> pixfmt_gray16_pre;
    typedef pixfmt_alpha_blend_gray<blender_gray32_pre, rendering_buffer> pixfmt_gray32_pre;
}

#endif

