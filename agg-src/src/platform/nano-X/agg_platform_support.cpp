//----------------------------------------------------------------------------
// Anti-Grain Geometry - Version 2.4
// Copyright (C) 2002-2005 Maxim Shemanarev (http://www.antigrain.com)
// Copyright (C) 2007-2019 Greg Haerr <greg@censoft.com>
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
// class platform_support for Nano-X with direct framebuffer by Greg Haerr
//
//----------------------------------------------------------------------------
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include "nano-X.h"
#include "agg_basics.h"
#include "util/agg_color_conv_rgb8.h"
#include "platform/agg_platform_support.h"

#define LSBFirst	0
#define MSBFirst	1

namespace agg
{
    //------------------------------------------------------------------------
    class platform_specific
    {
    public:
        platform_specific(pix_format_e format, bool flip_y);
        ~platform_specific();
        
        void caption(const char* capt);
        void put_image(const rendering_buffer* src);
       
        pix_format_e         m_format;
        pix_format_e         m_sys_format;
        int                  m_byte_order;
        bool                 m_flip_y;
        unsigned             m_bpp;
        unsigned             m_sys_bpp;
        int                  m_depth;
        GR_WINDOW_ID         m_window;
        GR_GC_ID             m_gc;
	unsigned             m_window_flags;	// FIXME
        unsigned char*       m_buf_window;
        unsigned char*       m_buf_img[platform_support::max_images];
	unsigned char *      m_winpixels;
	unsigned int         m_pitch;
        unsigned             m_keymap[256];
       
        bool m_update_flag;
        bool m_resize_flag;
        bool m_initialized;
        //bool m_wait_mode;
        clock_t m_sw_start;
    };



    //------------------------------------------------------------------------
    platform_specific::platform_specific(pix_format_e format, bool flip_y) :
        m_format(format),
        m_sys_format(pix_format_undefined),
        m_byte_order(LSBFirst),
        m_flip_y(flip_y),
        m_bpp(0),
        m_sys_bpp(0),
        m_depth(0),
        m_window(0),
        m_gc(0),

        m_buf_window(0),
	m_winpixels(0),
	m_pitch(0),
        
        m_update_flag(true), 
        m_resize_flag(true),
        m_initialized(false)
        //m_wait_mode(true)
    {
        memset(m_buf_img, 0, sizeof(m_buf_img));

        unsigned i;
        for(i = 0; i < 256; i++)
        {
            m_keymap[i] = i;
        }

        m_keymap[MWKEY_PAUSE&0xFF] = key_pause;
        //m_keymap[XK_Clear&0xFF] = key_clear;

        m_keymap[MWKEY_KP0&0xFF] = key_kp0;
        m_keymap[MWKEY_KP1&0xFF] = key_kp1;
        m_keymap[MWKEY_KP2&0xFF] = key_kp2;
        m_keymap[MWKEY_KP3&0xFF] = key_kp3;
        m_keymap[MWKEY_KP4&0xFF] = key_kp4;
        m_keymap[MWKEY_KP5&0xFF] = key_kp5;
        m_keymap[MWKEY_KP6&0xFF] = key_kp6;
        m_keymap[MWKEY_KP7&0xFF] = key_kp7;
        m_keymap[MWKEY_KP8&0xFF] = key_kp8;
        m_keymap[MWKEY_KP9&0xFF] = key_kp9;

        m_keymap[MWKEY_KP_PERIOD&0xFF]    = key_kp_period;
        m_keymap[MWKEY_KP_DIVIDE&0xFF]    = key_kp_divide;
        m_keymap[MWKEY_KP_MULTIPLY&0xFF]  = key_kp_multiply;
        m_keymap[MWKEY_KP_MINUS&0xFF]     = key_kp_minus;
        m_keymap[MWKEY_KP_PLUS&0xFF]      = key_kp_plus;
        m_keymap[MWKEY_KP_ENTER&0xFF]     = key_kp_enter;
        m_keymap[MWKEY_KP_EQUALS&0xFF]    = key_kp_equals;

        m_keymap[MWKEY_UP&0xFF]           = key_up;
        m_keymap[MWKEY_DOWN&0xFF]         = key_down;
        m_keymap[MWKEY_RIGHT&0xFF]        = key_right;
        m_keymap[MWKEY_LEFT&0xFF]         = key_left;
        m_keymap[MWKEY_INSERT&0xFF]       = key_insert;
        m_keymap[MWKEY_DELETE&0xFF]       = key_delete;
        m_keymap[MWKEY_END&0xFF]          = key_end;
        m_keymap[MWKEY_PAGEUP&0xFF]       = key_page_up;
        m_keymap[MWKEY_PAGEDOWN&0xFF]     = key_page_down;

        m_keymap[MWKEY_F1&0xFF]           = key_f1;
        m_keymap[MWKEY_F2&0xFF]           = key_f2;
        m_keymap[MWKEY_F3&0xFF]           = key_f3;
        m_keymap[MWKEY_F4&0xFF]           = key_f4;
        m_keymap[MWKEY_F5&0xFF]           = key_f5;
        m_keymap[MWKEY_F6&0xFF]           = key_f6;
        m_keymap[MWKEY_F7&0xFF]           = key_f7;
        m_keymap[MWKEY_F8&0xFF]           = key_f8;
        m_keymap[MWKEY_F9&0xFF]           = key_f9;
        m_keymap[MWKEY_F10&0xFF]          = key_f10;
        m_keymap[MWKEY_F11&0xFF]          = key_f11;
        m_keymap[MWKEY_F12&0xFF]          = key_f12;

        m_keymap[MWKEY_NUMLOCK&0xFF]    = key_numlock;
        m_keymap[MWKEY_CAPSLOCK&0xFF]   = key_capslock;
        m_keymap[MWKEY_SCROLLOCK&0xFF]  = key_scrollock;

        switch(m_format)
        {
        default: break;
        case pix_format_gray8:
            m_bpp = 8;
            break;

        case pix_format_rgb565:
        case pix_format_rgb555:
            m_bpp = 16;
            break;

        case pix_format_rgb24:
        case pix_format_bgr24:
            m_bpp = 24;
            break;

        case pix_format_bgra32:
        case pix_format_abgr32:
        case pix_format_argb32:
        case pix_format_rgba32:
            m_bpp = 32;
            break;
        }
        m_sw_start = clock();
    }

    //------------------------------------------------------------------------
    platform_specific::~platform_specific()
    {
    }

    //------------------------------------------------------------------------
    void platform_specific::caption(const char* capt)
    {
	GR_WM_PROPERTIES	props;
	char			buf[128];

	strcpy(buf, capt);
	props.flags = GR_WM_FLAGS_TITLE;
	props.title = buf;
	GrSetWMProperties(m_window, &props);
    }

    
    //------------------------------------------------------------------------
    void platform_specific::put_image(const rendering_buffer* src)
    {    
//printf("Format %d sys format %d\n", m_format, m_sys_format);
	if(m_winpixels != 0 && m_format == m_sys_format)
	{
		/* no conversion direct framebuffer memcpy*/
		int y;
		unsigned char *s = m_buf_window;
		unsigned char *d = m_winpixels;
            	int slen = src->width() * m_sys_bpp / 8;

		for (y=src->height(); y>0; --y)
		{
			memcpy(d, s, slen);
			s += slen;
			d += m_pitch;
		}
		//printf("NO CONVERSION memcpy\n");
        }
	else if(m_format == m_sys_format)
        {
	    /* no conversion pass image through GrArea*/
            GrArea(m_window, 
                      m_gc, 
                      0, 0,
                      src->width(), 
                      src->height(),
		      m_buf_window,
		      MWPF_HWPIXELVAL);
	    printf("NO CONVERSION GRAREA\n");
        }
        else
        {
	    /* color conversion required*/
            int row_len = src->width() * m_sys_bpp / 8;
            unsigned char* buf_tmp = 
                new unsigned char[row_len * src->height()];
            
            rendering_buffer rbuf_tmp;
            rbuf_tmp.attach(buf_tmp, 
                            src->width(), 
                            src->height(), 
                            m_flip_y ? -row_len : row_len);

//printf("converting from %d to %d\n", m_format, m_sys_format);
            switch(m_sys_format)            
            {
                default: break;
                case pix_format_rgb555:
                    switch(m_format)
                    {
                        default: break;
                        case pix_format_rgb555: color_conv(&rbuf_tmp, src, color_conv_rgb555_to_rgb555()); break;
                        case pix_format_rgb565: color_conv(&rbuf_tmp, src, color_conv_rgb565_to_rgb555()); break;
                        case pix_format_rgb24:  color_conv(&rbuf_tmp, src, color_conv_rgb24_to_rgb555());  break;
                        case pix_format_bgr24:  color_conv(&rbuf_tmp, src, color_conv_bgr24_to_rgb555());  break;
                        case pix_format_rgba32: color_conv(&rbuf_tmp, src, color_conv_rgba32_to_rgb555()); break;
                        case pix_format_argb32: color_conv(&rbuf_tmp, src, color_conv_argb32_to_rgb555()); break;
                        case pix_format_bgra32: color_conv(&rbuf_tmp, src, color_conv_bgra32_to_rgb555()); break;
                        case pix_format_abgr32: color_conv(&rbuf_tmp, src, color_conv_abgr32_to_rgb555()); break;
                    }
                    break;
                    
                case pix_format_rgb565:
                    switch(m_format)
                    {
                        default: break;
                        case pix_format_rgb555: color_conv(&rbuf_tmp, src, color_conv_rgb555_to_rgb565()); break;
                        case pix_format_rgb565: color_conv(&rbuf_tmp, src, color_conv_rgb565_to_rgb565()); break;
                        case pix_format_rgb24:  color_conv(&rbuf_tmp, src, color_conv_rgb24_to_rgb565());  break;
                        case pix_format_bgr24:  color_conv(&rbuf_tmp, src, color_conv_bgr24_to_rgb565());  break;
                        case pix_format_rgba32: color_conv(&rbuf_tmp, src, color_conv_rgba32_to_rgb565()); break;
                        case pix_format_argb32: color_conv(&rbuf_tmp, src, color_conv_argb32_to_rgb565()); break;
                        case pix_format_bgra32: color_conv(&rbuf_tmp, src, color_conv_bgra32_to_rgb565()); break;
                        case pix_format_abgr32: color_conv(&rbuf_tmp, src, color_conv_abgr32_to_rgb565()); break;
                    }
                    break;
                    
                case pix_format_rgba32:
                    switch(m_format)
                    {
                        default: break;
                        case pix_format_rgb555: color_conv(&rbuf_tmp, src, color_conv_rgb555_to_rgba32()); break;
                        case pix_format_rgb565: color_conv(&rbuf_tmp, src, color_conv_rgb565_to_rgba32()); break;
                        case pix_format_rgb24:  color_conv(&rbuf_tmp, src, color_conv_rgb24_to_rgba32());  break;
                        case pix_format_bgr24:  color_conv(&rbuf_tmp, src, color_conv_bgr24_to_rgba32());  break;
                        case pix_format_rgba32: color_conv(&rbuf_tmp, src, color_conv_rgba32_to_rgba32()); break;
                        case pix_format_argb32: color_conv(&rbuf_tmp, src, color_conv_argb32_to_rgba32()); break;
                        case pix_format_bgra32: color_conv(&rbuf_tmp, src, color_conv_bgra32_to_rgba32()); break;
                        case pix_format_abgr32: color_conv(&rbuf_tmp, src, color_conv_abgr32_to_rgba32()); break;
                    }
                    break;
                    
                case pix_format_abgr32:
                    switch(m_format)
                    {
                        default: break;
                        case pix_format_rgb555: color_conv(&rbuf_tmp, src, color_conv_rgb555_to_abgr32()); break;
                        case pix_format_rgb565: color_conv(&rbuf_tmp, src, color_conv_rgb565_to_abgr32()); break;
                        case pix_format_rgb24:  color_conv(&rbuf_tmp, src, color_conv_rgb24_to_abgr32());  break;
                        case pix_format_bgr24:  color_conv(&rbuf_tmp, src, color_conv_bgr24_to_abgr32());  break;
                        case pix_format_abgr32: color_conv(&rbuf_tmp, src, color_conv_abgr32_to_abgr32()); break;
                        case pix_format_rgba32: color_conv(&rbuf_tmp, src, color_conv_rgba32_to_abgr32()); break;
                        case pix_format_argb32: color_conv(&rbuf_tmp, src, color_conv_argb32_to_abgr32()); break;
                        case pix_format_bgra32: color_conv(&rbuf_tmp, src, color_conv_bgra32_to_abgr32()); break;
                    }
                    break;
                    
                case pix_format_argb32:
                    switch(m_format)
                    {
                        default: break;
                        case pix_format_rgb555: color_conv(&rbuf_tmp, src, color_conv_rgb555_to_argb32()); break;
                        case pix_format_rgb565: color_conv(&rbuf_tmp, src, color_conv_rgb565_to_argb32()); break;
                        case pix_format_rgb24:  color_conv(&rbuf_tmp, src, color_conv_rgb24_to_argb32());  break;
                        case pix_format_bgr24:  color_conv(&rbuf_tmp, src, color_conv_bgr24_to_argb32());  break;
                        case pix_format_rgba32: color_conv(&rbuf_tmp, src, color_conv_rgba32_to_argb32()); break;
                        case pix_format_argb32: color_conv(&rbuf_tmp, src, color_conv_argb32_to_argb32()); break;
                        case pix_format_abgr32: color_conv(&rbuf_tmp, src, color_conv_abgr32_to_argb32()); break;
                        case pix_format_bgra32: color_conv(&rbuf_tmp, src, color_conv_bgra32_to_argb32()); break;
                    }
                    break;
                    
                case pix_format_bgra32:
                    switch(m_format)
                    {
                        default: break;
                        case pix_format_rgb555: color_conv(&rbuf_tmp, src, color_conv_rgb555_to_bgra32()); break;
                        case pix_format_rgb565: color_conv(&rbuf_tmp, src, color_conv_rgb565_to_bgra32()); break;
                        case pix_format_rgb24:  color_conv(&rbuf_tmp, src, color_conv_rgb24_to_bgra32());  break;
                        case pix_format_bgr24:  color_conv(&rbuf_tmp, src, color_conv_bgr24_to_bgra32());  break;
                        case pix_format_rgba32: color_conv(&rbuf_tmp, src, color_conv_rgba32_to_bgra32()); break;
                        case pix_format_argb32: color_conv(&rbuf_tmp, src, color_conv_argb32_to_bgra32()); break;
                        case pix_format_abgr32: color_conv(&rbuf_tmp, src, color_conv_abgr32_to_bgra32()); break;
                        case pix_format_bgra32: color_conv(&rbuf_tmp, src, color_conv_bgra32_to_bgra32()); break;
                    }
                    break;
            }
            
	    if (m_winpixels)
	    {
		/* color converted direct framebuffer memcpy*/
		int y;
		unsigned char *s = buf_tmp;
		unsigned char *d = m_winpixels;
            	int slen = src->width() * m_sys_bpp / 8;

		for (y=src->height(); y>0; --y) {
			memcpy(d, s, slen);
			s += slen;
			d += m_pitch;
		}
		//printf("CONVERSION memcpy\n");
	    } else {
		/* color converted pass image through GrArea*/
	    	GrArea(m_window, 
                      m_gc, 
                      0, 0,
                      src->width(), 
                      src->height(),
		      buf_tmp,
		      MWPF_HWPIXELVAL);
		printf("CONVERSION GRAREA\n");
            }
            delete [] buf_tmp;
        }
	GrFlushWindow(m_window);
    }
    

    //------------------------------------------------------------------------
    platform_support::platform_support(pix_format_e format, bool flip_y) :
        m_specific(new platform_specific(format, flip_y)),
        m_format(format),
        m_bpp(m_specific->m_bpp),
        m_window_flags(0),
        m_wait_mode(true),
        m_flip_y(flip_y),
        m_initial_width(10),
        m_initial_height(10)
    {
        strcpy(m_caption, "AGG Application");
    }

    //------------------------------------------------------------------------
    platform_support::~platform_support()
    {
        delete m_specific;
    }



    //------------------------------------------------------------------------
    void platform_support::caption(const char* cap)
    {
        strcpy(m_caption, cap);
        if(m_specific->m_initialized)
        {
            m_specific->caption(cap);
        }
    }

   
    //------------------------------------------------------------------------
    enum xevent_mask_e
    { 
        xevent_mask =
            GR_EVENT_MASK_MOUSE_POSITION|
            GR_EVENT_MASK_BUTTON_DOWN|
            GR_EVENT_MASK_BUTTON_UP|
            GR_EVENT_MASK_EXPOSURE|
			GR_EVENT_MASK_KEY_DOWN|
			GR_EVENT_MASK_UPDATE|
	    GR_EVENT_MASK_CLOSE_REQ
    };


    //------------------------------------------------------------------------
    bool platform_support::init(unsigned width, unsigned height, unsigned flags)
    {
        m_window_flags = flags;
        
        if(GrOpen() < 0)
        {
            fprintf(stderr, "Unable to open graphics!\n");
            return false;
        }
	GR_SCREEN_INFO si;
	GrGetScreenInfo(&si);
        
        unsigned long r_mask = si.rmask;
        unsigned long g_mask = si.gmask;
        unsigned long b_mask = si.bmask;
	if(r_mask == 0x7C00 && g_mask == 0x3E0 && b_mask == 0x1F)
		m_specific->m_depth = 15;
        else m_specific->m_depth  = si.bpp;
                
        if(m_specific->m_depth < 15 || r_mask == 0 || g_mask == 0 || b_mask == 0)
        {
            fprintf(stderr,
                   "Screen not compatible with minimal AGG requirements:\n"
                   "At least 15-bit color depth and Truecolor.\n\n");
            GrClose();
            return false;
        }
        
        int t = 1;
        int hw_byte_order = LSBFirst;
        if(*(char*)&t == 0) hw_byte_order = MSBFirst;
        
        // Perceive SYS-format by mask
        switch(m_specific->m_depth)
        {
            case 15:
                m_specific->m_sys_bpp = 16;
                if(r_mask == 0x7C00 && g_mask == 0x3E0 && b_mask == 0x1F)
                {
                    m_specific->m_sys_format = pix_format_rgb555;
                    m_specific->m_byte_order = hw_byte_order;
                }
                break;
                
            case 16:
                m_specific->m_sys_bpp = 16;
                if(r_mask == 0xF800 && g_mask == 0x7E0 && b_mask == 0x1F)
                {
                    m_specific->m_sys_format = pix_format_rgb565;
                    m_specific->m_byte_order = hw_byte_order;
                }
                break;
                
            case 24:
            case 32:
                m_specific->m_sys_bpp = 32;
                if(g_mask == 0xFF00)
                {
                    if(r_mask == 0xFF && b_mask == 0xFF0000)
                    {
                        switch(m_specific->m_format)
                        {
                            case pix_format_rgba32:
                                m_specific->m_sys_format = pix_format_rgba32;
                                m_specific->m_byte_order = LSBFirst;
                                break;
                                
                            case pix_format_abgr32:
                                m_specific->m_sys_format = pix_format_abgr32;
                                m_specific->m_byte_order = MSBFirst;
                                break;

                            default:                            
                                m_specific->m_byte_order = hw_byte_order;
                                m_specific->m_sys_format = 
                                    (hw_byte_order == LSBFirst) ?
                                    pix_format_rgba32 :
                                    pix_format_abgr32;
                                break;
                        }
                    }
                    
                    if(r_mask == 0xFF0000 && b_mask == 0xFF)
                    {
                        switch(m_specific->m_format)
                        {
                            case pix_format_argb32:
                                m_specific->m_sys_format = pix_format_argb32;
                                m_specific->m_byte_order = MSBFirst;
                                break;
                                
                            case pix_format_bgra32:
                                m_specific->m_sys_format = pix_format_bgra32;
                                m_specific->m_byte_order = LSBFirst;
                                break;

                            default:                            
                                m_specific->m_byte_order = hw_byte_order;
                                m_specific->m_sys_format = 
                                    (hw_byte_order == MSBFirst) ?
                                    pix_format_argb32 :
                                    pix_format_bgra32;
                                break;
                        }
                    }
                }
                break;
        }
        
        if(m_specific->m_sys_format == pix_format_undefined)
        {
            fprintf(stderr,
                   "RGB masks are not compatible with AGG pixel formats:\n"
                   "R=%08lx, R=%08lx, B=%08lx\n", r_mask, g_mask, b_mask);
            GrClose();
            return false;
        }

        m_specific->m_window = GrNewBufferedWindow(GR_WM_PROPS_BUFFER_MMAP|GR_WM_PROPS_BUFFER_MWPF,
                          NULL, GR_ROOT_WINDOW_ID, 0, 0, width, height, GR_RGB(255,255,255));

        m_specific->m_gc = GrNewGC();
	GrSetGCUseBackground(m_specific->m_gc, GR_TRUE);

//printf("format %d sys format %d\n", m_format, m_specific->m_sys_format);
        m_specific->m_winpixels = GrOpenClientFramebuffer(m_specific->m_window);
	m_specific->m_pitch = width * m_specific->m_sys_bpp / 8;
        
        m_specific->m_buf_window = new unsigned char[width * height * (m_bpp / 8)];
        memset(m_specific->m_buf_window, 255, width * height * (m_bpp / 8));
        
        m_rbuf_window.attach(m_specific->m_buf_window,
                             width,
                             height,
                             m_flip_y ? -width * (m_bpp / 8) : width * (m_bpp / 8));
            
        m_specific->caption(m_caption); 
        m_initial_width = width;
        m_initial_height = height;
        
        if(!m_specific->m_initialized)
        {
            on_init();
            m_specific->m_initialized = true;
        }

        trans_affine_resizing(width, height);
        on_resize(width, height);
        m_specific->m_update_flag = true;

        GrMapWindow(m_specific->m_window);

        GrSelectEvents(m_specific->m_window, xevent_mask);

        return true;
    }



    //------------------------------------------------------------------------
    void platform_support::update_window()
    {
        m_specific->put_image(&m_rbuf_window);
        
        // When m_wait_mode is true we can discard all the events 
        // came while the image is being drawn. In this case 
        // the X server does not accumulate mouse motion events.
        // When m_wait_mode is false, i.e. we have some idle drawing
        // we cannot afford to miss any events
        //XSync(m_specific->m_display, m_wait_mode);
    }


    //------------------------------------------------------------------------
    int platform_support::run()
    {
        bool quit = false;
        unsigned flags;
        int cur_x;
        int cur_y;

        while(!quit)
        {
            if(m_specific->m_update_flag)
            {
                on_draw();
                update_window();
                m_specific->m_update_flag = false;
            }

            GR_EVENT nx_event;
            if(!m_wait_mode)
            {
                if(GrPeekEvent(&nx_event) == 0)
                {
                    on_idle();
                    continue;
                }
            }

            GrGetNextEvent(&nx_event);
#if 0        
            // In the Idle mode discard all intermediate MotionNotify events
            if(!m_wait_mode && nx_event.type == GR_EVENT_TYPE_MOUSE_MOTION)
            {
                GR_EVENT te = nx_event;
                for(;;)
                {
                    if(GrPeekEvent(&nx_event) == 0) break;
                    GrGetNextEvent(&te);
                    if(te.type != GR_EVENT_TYPE_MOUSE_MOTION) break;
                }
                nx_event = te;
            }
#endif
            switch(nx_event.type) 
            {
            case GR_EVENT_TYPE_UPDATE: 
                {
                    if(nx_event.update.utype == GR_UPDATE_SIZE &&
		       (nx_event.update.width != int(m_rbuf_window.width()) ||
                       nx_event.update.height != int(m_rbuf_window.height())))
                    {
                        int width  = nx_event.update.width;
                        int height = nx_event.update.height;

			GrCloseClientFramebuffer(m_specific->m_window);
			m_specific->m_winpixels = GrOpenClientFramebuffer(m_specific->m_window);
			m_specific->m_pitch = width * m_specific->m_sys_bpp / 8;

                        delete [] m_specific->m_buf_window;

                        m_specific->m_buf_window = new unsigned char[width * height * (m_bpp / 8)];

                        m_rbuf_window.attach(m_specific->m_buf_window,
                                             width,
                                             height,
                                             m_flip_y ? 
                                             -width * (m_bpp / 8) : 
                                             width * (m_bpp / 8));
            
                        trans_affine_resizing(width, height);
                        on_resize(width, height);
                        on_draw();
                        update_window();
                    }
                }
                break;

            case GR_EVENT_TYPE_EXPOSURE:
                m_specific->put_image(&m_rbuf_window);
                break;

            case GR_EVENT_TYPE_KEY_DOWN:
                {
                    MWKEY key = nx_event.keystroke.ch;
                    flags = 0;
                    if(nx_event.keystroke.buttons & GR_BUTTON_L) flags |= mouse_left;
                    if(nx_event.keystroke.buttons & GR_BUTTON_R) flags |= mouse_right;
                    if(nx_event.keystroke.modifiers & MWKMOD_SHIFT)   flags |= kbd_shift;
                    if(nx_event.keystroke.modifiers & MWKMOD_CTRL) flags |= kbd_ctrl;

                    bool left  = false;
                    bool up    = false;
                    bool right = false;
                    bool down  = false;

                    if (key >= MWKEY_FIRST)
		       switch(m_specific->m_keymap[key & 0xFF])
                    {
                    case key_left:
                        left = true;
                        break;

                    case key_up:
                        up = true;
                        break;

                    case key_right:
                        right = true;
                        break;

                    case key_down:
                        down = true;
                        break;

                    case key_f2:                        
                        copy_window_to_img(max_images - 1);
                        save_img(max_images - 1, "screenshot");
                        break;
                    }

                    if(m_ctrls.on_arrow_keys(left, right, down, up))
                    {
                        on_ctrl_change();
                        force_redraw();
                    }
                    else
                    {
                        on_key(nx_event.keystroke.x, 
                               m_flip_y ? 
                                   m_rbuf_window.height() - nx_event.keystroke.y :
                                   nx_event.keystroke.y,
                               m_specific->m_keymap[key & 0xFF],
                               flags);
                    }
                }
                break;


            case GR_EVENT_TYPE_BUTTON_DOWN:
                {
                    flags = 0;
                    if(nx_event.button.buttons & GR_BUTTON_L) flags |= mouse_left;
                    if(nx_event.button.buttons & GR_BUTTON_R) flags |= mouse_right;
                    if(nx_event.button.modifiers & MWKMOD_SHIFT) flags |= kbd_shift;
                    if(nx_event.button.modifiers & MWKMOD_CTRL) flags |= kbd_ctrl;

                    cur_x = nx_event.button.x;
                    cur_y = m_flip_y ? m_rbuf_window.height() - nx_event.button.y : nx_event.button.y;

                    if(flags & mouse_left)
                    {
                        if(m_ctrls.on_mouse_button_down(cur_x, cur_y))
                        {
                            m_ctrls.set_cur(cur_x, cur_y);
                            on_ctrl_change();
                            force_redraw();
                        }
                        else
                        {
                            if(m_ctrls.in_rect(cur_x, cur_y))
                            {
                                if(m_ctrls.set_cur(cur_x, cur_y))
                                {
                                    on_ctrl_change();
                                    force_redraw();
                                }
                            }
                            else
                            {
                                on_mouse_button_down(cur_x, cur_y, flags);
                            }
                        }
                    }
                    if(flags & mouse_right)
                    {
                        on_mouse_button_down(cur_x, cur_y, flags);
                    }
                    //m_specific->m_wait_mode = m_wait_mode;
                    //m_wait_mode = true;
                }
                break;

                
            case GR_EVENT_TYPE_MOUSE_POSITION:
                {
                    flags = 0;
                    if(nx_event.mouse.buttons & GR_BUTTON_L) flags |= mouse_left;
                    if(nx_event.mouse.buttons & GR_BUTTON_R) flags |= mouse_right;
                    if(nx_event.mouse.modifiers & MWKMOD_SHIFT) flags |= kbd_shift;
                    if(nx_event.mouse.modifiers & MWKMOD_CTRL) flags |= kbd_ctrl;

                    cur_x = nx_event.mouse.x;
                    cur_y = m_flip_y ? m_rbuf_window.height() - nx_event.mouse.y : nx_event.mouse.y;

                    if(m_ctrls.on_mouse_move(cur_x, cur_y, (flags & mouse_left) != 0))
                    {
                        on_ctrl_change();
                        force_redraw();
                    }
                    else
                    {
                        if(!m_ctrls.in_rect(cur_x, cur_y))
                        {
                            on_mouse_move(cur_x, cur_y, flags);
                        }
                    }
                }
                break;
                
            case GR_EVENT_TYPE_BUTTON_UP:
                {
                    flags = 0;
                    if(nx_event.button.changebuttons & GR_BUTTON_L) flags |= mouse_left;
                    if(nx_event.button.changebuttons & GR_BUTTON_R) flags |= mouse_right;
                    if(nx_event.button.modifiers & MWKMOD_SHIFT) flags |= kbd_shift;
                    if(nx_event.button.modifiers & MWKMOD_CTRL) flags |= kbd_ctrl;

                    cur_x = nx_event.button.x;
                    cur_y = m_flip_y ? m_rbuf_window.height() - nx_event.button.y : nx_event.button.y;

                    if(flags & mouse_left)
                    {
                        if(m_ctrls.on_mouse_button_up(cur_x, cur_y))
                        {
                            on_ctrl_change();
                            force_redraw();
                        }
                    }
                    if(flags & (mouse_left | mouse_right))
                    {
                        on_mouse_button_up(cur_x, cur_y, flags);
                    }
                }
                //m_wait_mode = m_specific->m_wait_mode;
                break;

            case GR_EVENT_TYPE_CLOSE_REQ:
                quit = true;
                break;
            }           
        }


        unsigned i = platform_support::max_images;
        while(i--)
        {
            if(m_specific->m_buf_img[i]) 
            {
                delete [] m_specific->m_buf_img[i];
            }
        }

        delete [] m_specific->m_buf_window;
	GrCloseClientFramebuffer(m_specific->m_window);
        GrDestroyGC(m_specific->m_gc);
        GrDestroyWindow(m_specific->m_window);
        GrClose();
        
        return 0;
    }



    //------------------------------------------------------------------------
    const char* platform_support::img_ext() const { return ".ppm"; }

    //------------------------------------------------------------------------
    const char* platform_support::full_file_name(const char* file_name)
    {
        return file_name;
    }

    //------------------------------------------------------------------------
    bool platform_support::load_img(unsigned idx, const char* file)
    {
        if(idx < max_images)
        {
            char buf[1024];
            strcpy(buf, file);
            int len = strlen(buf);
            if(len < 4 || strcasecmp(buf + len - 4, ".ppm") != 0)
            {
                strcat(buf, ".ppm");
            }
            
            FILE* fd = fopen(buf, "rb");
            if(fd == 0) return false;

            if((len = fread(buf, 1, 1022, fd)) == 0)
            {
                fclose(fd);
                return false;
            }
            buf[len] = 0;
            
            if(buf[0] != 'P' && buf[1] != '6')
            {
                fclose(fd);
                return false;
            }
            
            char* ptr = buf + 2;
            
            while(*ptr && !isdigit(*ptr)) ptr++;
            if(*ptr == 0)
            {
                fclose(fd);
                return false;
            }
            
            unsigned width = atoi(ptr);
            if(width == 0 || width > 4096)
            {
                fclose(fd);
                return false;
            }
            while(*ptr && isdigit(*ptr)) ptr++;
            while(*ptr && !isdigit(*ptr)) ptr++;
            if(*ptr == 0)
            {
                fclose(fd);
                return false;
            }
            unsigned height = atoi(ptr);
            if(height == 0 || height > 4096)
            {
                fclose(fd);
                return false;
            }
            while(*ptr && isdigit(*ptr)) ptr++;
            while(*ptr && !isdigit(*ptr)) ptr++;
            if(atoi(ptr) != 255)
            {
                fclose(fd);
                return false;
            }
            while(*ptr && isdigit(*ptr)) ptr++;
            if(*ptr == 0)
            {
                fclose(fd);
                return false;
            }
            ptr++;
            fseek(fd, long(ptr - buf), SEEK_SET);
            
            create_img(idx, width, height);
            bool ret = true;
            
            if(m_format == pix_format_rgb24)
            {
                fread(m_specific->m_buf_img[idx], 1, width * height * 3, fd);
            }
            else
            {
                unsigned char* buf_img = new unsigned char [width * height * 3];
                rendering_buffer rbuf_img;
                rbuf_img.attach(buf_img,
                                width,
                                height,
                                m_flip_y ?
                                  -width * 3 :
                                   width * 3);
                
                fread(buf_img, 1, width * height * 3, fd);
                
                switch(m_format)
                {
                    case pix_format_rgb555:
                        color_conv(m_rbuf_img+idx, &rbuf_img, color_conv_rgb24_to_rgb555());
                        break;
                        
                    case pix_format_rgb565:
                        color_conv(m_rbuf_img+idx, &rbuf_img, color_conv_rgb24_to_rgb565());
                        break;
                        
                    case pix_format_bgr24:
                        color_conv(m_rbuf_img+idx, &rbuf_img, color_conv_rgb24_to_bgr24());
                        break;
                        
                    case pix_format_rgba32:
                        color_conv(m_rbuf_img+idx, &rbuf_img, color_conv_rgb24_to_rgba32());
                        break;
                        
                    case pix_format_argb32:
                        color_conv(m_rbuf_img+idx, &rbuf_img, color_conv_rgb24_to_argb32());
                        break;
                        
                    case pix_format_bgra32:
                        color_conv(m_rbuf_img+idx, &rbuf_img, color_conv_rgb24_to_bgra32());
                        break;
                        
                    case pix_format_abgr32:
                        color_conv(m_rbuf_img+idx, &rbuf_img, color_conv_rgb24_to_abgr32());
                        break;
                        
                    default:
                        ret = false;
                }
                delete [] buf_img;
            }
                        
            fclose(fd);
            return ret;
        }
        return false;
    }




    //------------------------------------------------------------------------
    bool platform_support::save_img(unsigned idx, const char* file)
    {
        if(idx < max_images &&  rbuf_img(idx).buf())
        {
            char buf[1024];
            strcpy(buf, file);
            int len = strlen(buf);
            if(len < 4 || strcasecmp(buf + len - 4, ".ppm") != 0)
            {
                strcat(buf, ".ppm");
            }
            
            FILE* fd = fopen(buf, "wb");
            if(fd == 0) return false;
            
            unsigned w = rbuf_img(idx).width();
            unsigned h = rbuf_img(idx).height();
            
            fprintf(fd, "P6\n%d %d\n255\n", w, h);
                
            unsigned y; 
            unsigned char* tmp_buf = new unsigned char [w * 3];
            for(y = 0; y < rbuf_img(idx).height(); y++)
            {
                const unsigned char* src = rbuf_img(idx).row_ptr(m_flip_y ? h - 1 - y : y);
                switch(m_format)
                {
                    default: break;
                    case pix_format_rgb555:
                        color_conv_row(tmp_buf, src, w, color_conv_rgb555_to_rgb24());
                        break;
                        
                    case pix_format_rgb565:
                        color_conv_row(tmp_buf, src, w, color_conv_rgb565_to_rgb24());
                        break;
                        
                    case pix_format_bgr24:
                        color_conv_row(tmp_buf, src, w, color_conv_bgr24_to_rgb24());
                        break;
                        
                    case pix_format_rgb24:
                        color_conv_row(tmp_buf, src, w, color_conv_rgb24_to_rgb24());
                        break;
                       
                    case pix_format_rgba32:
                        color_conv_row(tmp_buf, src, w, color_conv_rgba32_to_rgb24());
                        break;
                        
                    case pix_format_argb32:
                        color_conv_row(tmp_buf, src, w, color_conv_argb32_to_rgb24());
                        break;
                        
                    case pix_format_bgra32:
                        color_conv_row(tmp_buf, src, w, color_conv_bgra32_to_rgb24());
                        break;
                        
                    case pix_format_abgr32:
                        color_conv_row(tmp_buf, src, w, color_conv_abgr32_to_rgb24());
                        break;
                }
                fwrite(tmp_buf, 1, w * 3, fd);
            }
            delete [] tmp_buf;
            fclose(fd);
            return true;
        }
        return false;
    }



    //------------------------------------------------------------------------
    bool platform_support::create_img(unsigned idx, unsigned width, unsigned height)
    {
        if(idx < max_images)
        {
            if(width  == 0) width  = rbuf_window().width();
            if(height == 0) height = rbuf_window().height();
            delete [] m_specific->m_buf_img[idx];
            m_specific->m_buf_img[idx] = 
                new unsigned char[width * height * (m_bpp / 8)];

            m_rbuf_img[idx].attach(m_specific->m_buf_img[idx],
                                   width,
                                   height,
                                   m_flip_y ? 
                                       -width * (m_bpp / 8) : 
                                        width * (m_bpp / 8));
            return true;
        }
        return false;
    }


    //------------------------------------------------------------------------
    void platform_support::force_redraw()
    {
        m_specific->m_update_flag = true;
    }


    //------------------------------------------------------------------------
    void platform_support::message(const char* msg)
    {
        fprintf(stderr, "%s\n", msg);
    }

    //------------------------------------------------------------------------
    void platform_support::start_timer()
    {
        m_specific->m_sw_start = clock();
    }

    //------------------------------------------------------------------------
    double platform_support::elapsed_time() const
    {
        clock_t stop = clock();
        return double(stop - m_specific->m_sw_start) * 1000.0 / CLOCKS_PER_SEC;
    }


    //------------------------------------------------------------------------
    void platform_support::on_init() {}
    void platform_support::on_resize(int sx, int sy) {}
    void platform_support::on_idle() {}
    void platform_support::on_mouse_move(int x, int y, unsigned flags) {}
    void platform_support::on_mouse_button_down(int x, int y, unsigned flags) {}
    void platform_support::on_mouse_button_up(int x, int y, unsigned flags) {}
    void platform_support::on_key(int x, int y, unsigned key, unsigned flags) {}
    void platform_support::on_ctrl_change() {}
    void platform_support::on_draw() {}
    void platform_support::on_post_draw(void* raw_handler) {}



}


int agg_main(int argc, char* argv[]);


int main(int argc, char* argv[])
{
    return agg_main(argc, argv);
}

/* vim: set ts=8: */
