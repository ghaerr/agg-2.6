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

#ifndef AGG_GAMMA_LUT_INCLUDED
#define AGG_GAMMA_LUT_INCLUDED

#include <math.h>
#include "agg_basics.h"
#include "agg_gamma_functions.h"

namespace agg
{
    template<class LoResT=int8u, 
             class HiResT=int8u, 
             unsigned GammaShift=8, 
             unsigned HiResShift=8> class gamma_lut
    {
    public:
        typedef gamma_lut<LoResT, HiResT, GammaShift, HiResShift> self_type;

        enum gamma_scale_e
        {
            gamma_shift = GammaShift,
            gamma_size  = 1 << gamma_shift,
            gamma_mask  = gamma_size - 1
        };

        enum hi_res_scale_e
        {
            hi_res_shift = HiResShift,
            hi_res_size  = 1 << hi_res_shift,
            hi_res_mask  = hi_res_size - 1
        };

        ~gamma_lut()
        {
            pod_allocator<LoResT>::deallocate(m_inv_gamma, hi_res_size);
            pod_allocator<HiResT>::deallocate(m_dir_gamma, gamma_size);
        }

        gamma_lut() : 
            m_gamma(1.0), 
            m_dir_gamma(pod_allocator<HiResT>::allocate(gamma_size)),
            m_inv_gamma(pod_allocator<LoResT>::allocate(hi_res_size))
        {
            unsigned i;
            for(i = 0; i < gamma_size; i++)
            {
                m_dir_gamma[i] = HiResT(i << (hi_res_shift - gamma_shift));
            }

            for(i = 0; i < hi_res_size; i++)
            {
                m_inv_gamma[i] = LoResT(i >> (hi_res_shift - gamma_shift));
            }
        }

        gamma_lut(double g) :
            m_gamma(1.0), 
            m_dir_gamma(pod_allocator<HiResT>::allocate(gamma_size)),
            m_inv_gamma(pod_allocator<LoResT>::allocate(hi_res_size))
        {
            gamma(g);
        }

        void gamma(double g) 
        {
            m_gamma = g;

            unsigned i;
            for(i = 0; i < gamma_size; i++)
            {
                m_dir_gamma[i] = (HiResT)
                    uround(pow(i / double(gamma_mask), m_gamma) * double(hi_res_mask));
            }

            double inv_g = 1.0 / g;
            for(i = 0; i < hi_res_size; i++)
            {
                m_inv_gamma[i] = (LoResT)
                    uround(pow(i / double(hi_res_mask), inv_g) * double(gamma_mask));
            }
        }

        double gamma() const
        {
            return m_gamma;
        }

        HiResT dir(LoResT v) const 
        { 
            return m_dir_gamma[unsigned(v)]; 
        }

        LoResT inv(HiResT v) const 
        { 
            return m_inv_gamma[unsigned(v)];
        }

    private:
        gamma_lut(const self_type&);
        const self_type& operator = (const self_type&);

        double m_gamma;
        HiResT* m_dir_gamma;
        LoResT* m_inv_gamma;
    };


    // Optimized sRGB lookup table. The direct conversion (sRGB to linear) 
    // is a straightforward lookup. The inverse conversion (linear to sRGB) 
    // is implemented using binary search.
    template<class HiResT = float>
    class sRGB_lut
    {
    public:
        sRGB_lut()
        {
            // Generate lookup tables.
            m_dir_table[0] = 0;
            m_inv_table[0] = 0;
            for (unsigned i = 1; i <= 255; ++i)
            {
                m_dir_table[i] = HiResT(sRGB_to_linear(i / 255.0));
                m_inv_table[i] = HiResT(sRGB_to_linear((i - 0.5) / 255.0));
            }
        }

        HiResT dir(int8u v) const
        {
            return m_dir_table[v];
        }

        int8u inv(HiResT v) const
        {
            // Unrolled binary search.
            int8u x = 0;
            if (v > m_inv_table[128]) x = 128;
            if (v > m_inv_table[x + 64]) x += 64;
            if (v > m_inv_table[x + 32]) x += 32;
            if (v > m_inv_table[x + 16]) x += 16;
            if (v > m_inv_table[x + 8]) x += 8;
            if (v > m_inv_table[x + 4]) x += 4;
            if (v > m_inv_table[x + 2]) x += 2;
            if (v > m_inv_table[x + 1]) x += 1;
            return x;
        }

    private:
        HiResT m_dir_table[256];
        HiResT m_inv_table[256];
    };

    // Wrapper for sRGB-linear conversion. Overloading is used to automatically 
    // select the direction of conversion, and support is provided for 
    // handling premultiplied RGB values.
    template<class T = float>
    class sRGB
    {
    private:
        static sRGB_lut<> lut;

    public:
        static T conv_rgb(int8u x) // full alpha
        {
            return lut.dir(x);
        }

        static T conv_rgb(int8u x, int8u a) // premultiplied x
        {
            if (a == 0) return 0;
            else if (a == 255) return lut.dir(x);
            else return lut.dir(int8u(0.5 + x * 255.0 / a)) * a / 255;
        }

        static T conv_rgb_demultiply(int8u x, int8u a)
        {
            if (a == 0) return 0;
            else if (a == 255) return lut.dir(x);
            else return lut.dir(int8u(0.5 + x * 255.0 / a));
        }

        static int8u conv_rgb(T x) // full alpha
        {
            return lut.inv(x);
        }

        static int8u conv_rgb(T x, T a) // premultiplied x
        {
            if (a <= 0) return 0;
            else if (a >= 1) return lut.inv(x);
            else return int8u(0.5 + lut.inv(x / a) * a);
        }

        static int8u conv_rgb_demultiply(T x, T a)
        {
            if (a <= 0) return 0;
            else if (a >= 1) return lut.inv(x);
            else return lut.inv(x / a);
        }

        static T conv_alpha(int8u x)
        {
            return T(x / 255.0);
        }

        static int8u conv_alpha(T x)
        {
            if (x <= 0) return 0;
            else if (x >= 1) return 255;
            else return int8(0.5 + x * 255);
        }
    };

    // Definition of sRGB::lut. Due to the fact that this a template, 
    // we don't need to place the definition in a cpp file. Hurrah.
    sRGB_lut<> sRGB<>::lut;

}

#endif
