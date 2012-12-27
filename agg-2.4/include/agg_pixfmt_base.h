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

#ifndef AGG_PIXFMT_BASE_INCLUDED
#define AGG_PIXFMT_BASE_INCLUDED

#include "agg_basics.h"
#include "agg_color_gray.h"
#include "agg_color_rgba.h"

namespace agg
{
    //--------------------------------------------------------------blender_base
    template<class ColorT, class Order = void> 
    struct blender_base
    {
        typedef ColorT color_type;
        typedef Order order_type;
        typedef typename color_type::value_type value_type;
        typedef typename color_type::calc_type calc_type;
        typedef typename color_type::long_type long_type;

        static value_type invert(value_type a)
        {
            return color_type::invert(a);
        }

        static value_type multiply(value_type a, value_type b)
        {
            return color_type::multiply(a, b);
        }

        static value_type demultiply(value_type a, value_type b)
        {
            return color_type::demultiply(a, b);
        }

        static value_type mult_cover(value_type a, cover_type b) 
        {
            return color_type::mult_cover(a, b);
        }

        static value_type prelerp(value_type p, value_type q, value_type a) 
        {
            return color_type::prelerp(p, q, a);
        }
        
        static value_type lerp(value_type p, value_type q, value_type a) 
        {
            return color_type::lerp(p, q, a);
        }

        static value_type empty_value()
        {
            return color_type::empty_value();
        }

        static value_type full_value()
        {
            return color_type::full_value();
        }

        static bool is_empty(value_type a)
        {
            return a <= empty_value();
        }

        static bool is_full(value_type a)
        {
            return a >= full_value();
        }

        static double to_double(value_type a)
        {
            return color_type::to_double(a);
        }

        static value_type from_double(double a)
        {
            return color_type::from_double(a);
        }

        static rgba get(value_type r, value_type g, value_type b, value_type a, cover_type cover = cover_full)
        {
            if (cover > cover_none)
            {
                rgba c(
                    to_double(r), 
                    to_double(g), 
                    to_double(b), 
                    to_double(a));

                if (cover < cover_full)
                {
                    double x = double(cover) / cover_full;
                    c.r *= x;
                    c.g *= x;
                    c.b *= x;
                    c.a *= x;
                }

                return c;
            }
            else return rgba::no_color();
        }

        static rgba get(const value_type* p, cover_type cover = cover_full)
        {
            return get(
                p[order_type::R], 
                p[order_type::G], 
                p[order_type::B], 
                p[order_type::A], 
                cover);
        }

        static void set(value_type* p, value_type r, value_type g, value_type b, value_type a)
        {
            p[Order::R] = r;
            p[Order::G] = g;
            p[Order::B] = b;
            p[Order::A] = a;
        }

        static void set(value_type* p, const rgba& c)
        {
            p[order_type::R] = from_double(c.r);
            p[order_type::G] = from_double(c.g);
            p[order_type::B] = from_double(c.b);
            p[order_type::A] = from_double(c.a);
        }
    };
}

#endif
