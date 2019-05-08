//----------------------------------------------------------------------------
// Anti-Grain Geometry (AGG) - Version 2.5
// A high quality rendering engine for C++
// Copyright (C) 2002-2006 Maxim Shemanarev
// Copyright (C) 2004 Mauricio Piacentini (SDL Support)
// Contact: mcseem@antigrain.com
//          mcseemagg@yahoo.com
//          http://antigrain.com
// 
// AGG is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
// 
// AGG is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with AGG; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, 
// MA 02110-1301, USA.
//----------------------------------------------------------------------------

#include <string.h>
#include "platform/agg_platform_support.h"
#include "SDL.h"
#include "SDL_surface.h"
#include "SDL_endian.h"


#if __ANDROID__
extern "C"
{
extern int Android_ScreenWidth;
extern int Android_ScreenHeight;
}

#ifdef NATIVE_LOG
#define LOG_TAG "NATIVE.LOG"

#include <android/log.h>

#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN,LOG_TAG,__VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)
#define LOGF(...) __android_log_print(ANDROID_LOG_FATAL,LOG_TAG,__VA_ARGS__)
#else
#define LOGD(...) do{}while(0)
#define LOGI(...) do{}while(0)
#define LOGW(...) do{}while(0)
#define LOGE(...) do{}while(0)
#define LOGF(...) do{}while(0)
#endif
#define DEBUG_PRINT(...) LOGD(__VA_ARGS__)
#define ERROR_PRINT(...) LOGE(__VA_ARGS__)
#else //!__ANDROID__
#define DEBUG_PRINT(...) do{fprintf(stderr, __VA_ARGS__ ); } while (false)
#define ERROR_PRINT(...) do{fprintf(stderr, __VA_ARGS__ ); } while (false)
#endif

namespace agg {

    //------------------------------------------------------------------------
    class platform_specific {

    public:
        platform_specific(pix_format_e format, bool flip_y);

        ~platform_specific();

        pix_format_e m_format;
        pix_format_e m_sys_format;
        bool m_flip_y;
        unsigned m_bpp;
        unsigned m_pformat;
        unsigned m_sys_bpp;
        unsigned m_rmask;
        unsigned m_gmask;
        unsigned m_bmask;
        unsigned m_amask;
        bool m_update_flag;
        bool m_resize_flag;
        bool m_initialized;
        SDL_Window *m_mwindow;
        SDL_Surface *m_surface;
        SDL_Texture *m_texture;
        SDL_Renderer *m_renderer;
        SDL_Surface *m_surf_img[platform_support::max_images];
        int m_cur_x;
        int m_cur_y;
        int m_sw_start;
    };


    //------------------------------------------------------------------------
    platform_specific::platform_specific(pix_format_e format, bool flip_y) :
            m_format(format),
            m_sys_format(pix_format_undefined),
            m_flip_y(flip_y),
            m_bpp(0),
            m_sys_bpp(0),
            m_update_flag(true),
            m_resize_flag(true),
            m_initialized(false),
            m_mwindow(0),
            m_surface(0),
            m_texture(0),
            m_renderer(0),
            m_cur_x(0),
            m_cur_y(0) {

        memset(m_surf_img, 0, sizeof(m_surf_img));
        m_pformat = 0;

        switch (m_format) {
            case pix_format_gray8:
                m_bpp = 8;
                break;
            case pix_format_rgb565:
                m_rmask = 0xF800;
                m_gmask = 0x7E0;
                m_bmask = 0x1F;
                m_amask = 0;
                m_bpp = 16;
                break;
            case pix_format_rgb555:
                m_rmask = 0x7C00;
                m_gmask = 0x3E0;
                m_bmask = 0x1F;
                m_amask = 0;
                m_bpp = 16;
                break;
            case pix_format_rgb24:
                m_rmask = 0xFF;
                m_gmask = 0xFF00;
                m_bmask = 0xFF0000;
                m_amask = 0;
                m_bpp = 24;
                m_pformat = SDL_PIXELFORMAT_RGB24;
                break;
            case pix_format_bgr24:
                m_rmask = 0xFF0000;
                m_gmask = 0xFF00;
                m_bmask = 0xFF;
                m_amask = 0;
                m_bpp = 24;
                m_pformat = SDL_PIXELFORMAT_BGR24;
                break;
            case pix_format_bgra32:
                m_rmask = 0xFF0000;
                m_gmask = 0xFF00;
                m_bmask = 0xFF;
                m_amask = 0xFF000000;
                m_bpp = 32;
                m_pformat = SDL_PIXELFORMAT_BGRA8888;
                break;
            case pix_format_abgr32:
                m_rmask = 0xFF000000;
                m_gmask = 0xFF0000;
                m_bmask = 0xFF00;
                m_amask = 0xFF;
                m_bpp = 32;
                m_pformat = SDL_PIXELFORMAT_ABGR8888;
                break;
            case pix_format_argb32:
                m_rmask = 0xFF00;
                m_gmask = 0xFF0000;
                m_bmask = 0xFF000000;
                m_amask = 0xFF;
                m_bpp = 32;
                m_pformat = SDL_PIXELFORMAT_ARGB8888;
                break;
            case pix_format_rgba32:
                m_rmask = 0xFF;
                m_gmask = 0xFF00;
                m_bmask = 0xFF0000;
                m_amask = 0xFF000000;
                m_pformat = SDL_PIXELFORMAT_RGBA8888;
                m_bpp = 32;
                break;
        }
    }


    //------------------------------------------------------------------------
    platform_specific::~platform_specific() {
        int i;
        for (i = platform_support::max_images - 1; i >= 0; --i) {
            if (m_surf_img[i]) SDL_FreeSurface(m_surf_img[i]);
        }
        if (m_texture) SDL_DestroyTexture(m_texture);
        if (m_renderer) SDL_DestroyRenderer(m_renderer);
        if (m_surface) SDL_FreeSurface(m_surface);
        if (m_mwindow) SDL_DestroyWindow(m_mwindow);
    }


    //------------------------------------------------------------------------
    platform_support::platform_support(pix_format_e format, bool flip_y) :
            m_specific(new platform_specific(format, flip_y)),
            m_format(format),
            m_bpp(m_specific->m_bpp),
            m_window_flags(0),
            m_wait_mode(true),
            m_flip_y(flip_y) {
        SDL_Init(SDL_INIT_VIDEO);
        strcpy(m_caption, "Anti-Grain Geometry Application");
    }


    //------------------------------------------------------------------------
    platform_support::~platform_support() {
        delete m_specific;
    }


    //------------------------------------------------------------------------
    void platform_support::caption(const char *cap) {
        strcpy(m_caption, cap);
        if (m_specific->m_initialized) {
            SDL_SetWindowTitle(m_specific->m_mwindow, cap);
        }
        //DEBUG_PRINT("Caption: %s", cap);
    }


    //------------------------------------------------------------------------
    bool platform_support::init(unsigned width, unsigned height, unsigned flags) {
        m_window_flags = flags;
        if (m_specific->m_texture) SDL_DestroyTexture(m_specific->m_texture);
        if (m_specific->m_renderer) SDL_DestroyRenderer(m_specific->m_renderer);
        if (m_specific->m_surface) SDL_FreeSurface(m_specific->m_surface);
        m_specific->m_texture = 0;
        m_specific->m_renderer = 0;
        m_specific->m_surface = 0;

#ifdef __ANDROID__
        width = (unsigned int) Android_ScreenWidth;
        height = (unsigned int) Android_ScreenHeight;
        unsigned int wflags = SDL_WINDOW_FULLSCREEN;
#else
        unsigned int wflags = 0;
#endif

        if (m_window_flags & window_resize) {
            wflags |= SDL_WINDOW_RESIZABLE;
        }
        //DEBUG_PRINT("platform_support::init %dx%d,%d", width, height, wflags);

        if (!m_specific->m_mwindow) {
            m_specific->m_mwindow =
                    SDL_CreateWindow(m_caption,
                                     SDL_WINDOWPOS_UNDEFINED,
                                     SDL_WINDOWPOS_UNDEFINED,
                                     width, height, wflags);
        }
        if (m_specific->m_mwindow == 0) {
            ERROR_PRINT("Unable to create %dx%d %d bpp window: %s\n",
                        width, height, m_bpp, ::SDL_GetError());
            return false;
        }
        m_specific->m_surface =
                SDL_CreateRGBSurface(0, width, height, m_bpp,
                                     m_specific->m_rmask,
                                     m_specific->m_gmask,
                                     m_specific->m_bmask,
                                     m_specific->m_amask);
        //DEBUG_PRINT("surface at %p", m_specific->m_surface);
        if (m_specific->m_surface == 0) {
            ERROR_PRINT("Unable to create image buffer %dx%d %d bpp: %s\n",
                        width, height, m_bpp, SDL_GetError());
            return false;
        }

        m_rbuf_window.attach((unsigned char *) m_specific->m_surface->pixels,
                             (unsigned int) m_specific->m_surface->w,
                             (unsigned int) m_specific->m_surface->h,
                             m_flip_y ? -m_specific->m_surface->pitch :
                             m_specific->m_surface->pitch);

        m_specific->m_renderer = SDL_CreateRenderer(m_specific->m_mwindow, -1, 0);
        if (m_specific->m_renderer == 0) {
            ERROR_PRINT("Unable to create renderer: %s\n", SDL_GetError());
            return false;
        }

        m_specific->m_texture =
                SDL_CreateTexture(m_specific->m_renderer,
                                  m_specific->m_pformat,
                                  SDL_TEXTUREACCESS_STREAMING, width, height);
        if (m_specific->m_renderer == 0) {
            ERROR_PRINT("Unable to create texture: %s\n", SDL_GetError());
            return false;
        }

        if (!m_specific->m_initialized) {
            m_initial_width = width;
            m_initial_height = height;
            on_init();
            m_specific->m_initialized = true;
        }
        on_resize(m_rbuf_window.width(), m_rbuf_window.height());
        m_specific->m_update_flag = true;

		SDL_PumpEvents();	/* SDL bug: must call before output or black window*/

        return true;
    }


    //------------------------------------------------------------------------
    void platform_support::update_window() {
        SDL_UpdateTexture(m_specific->m_texture, NULL,
                          m_specific->m_surface->pixels,
                          m_specific->m_surface->pitch);
        SDL_RenderClear(m_specific->m_renderer);
        SDL_RenderCopy(m_specific->m_renderer, m_specific->m_texture, NULL, NULL);
        SDL_RenderPresent(m_specific->m_renderer);
    }


    //------------------------------------------------------------------------
    int platform_support::run() {
        SDL_Event event;
        bool ev_flag;

        for (;;) {
            if (m_specific->m_update_flag) {
                on_draw();
                update_window();
                m_specific->m_update_flag = false;
            }
            ev_flag = false;
            if (m_wait_mode) {
                SDL_WaitEvent(&event);
                ev_flag = true;
            } else {
                if (SDL_PollEvent(&event)) {
                    ev_flag = true;
                } else {
                    on_idle();
                }
            }

            if (ev_flag) {
                if (event.type == SDL_QUIT) {
                    break;
                }
                int y;
                unsigned flags = 0;

                switch (event.type) {
                    case SDL_WINDOWEVENT:
                        switch (event.window.event) {
                            case SDL_WINDOWEVENT_RESIZED:
                                if (!init((unsigned int) event.window.data1,
                                          (unsigned int) event.window.data2,
                                          m_window_flags)) {
                                    return false;
                                }
                                on_resize(m_rbuf_window.width(), m_rbuf_window.height());
                                trans_affine_resizing(event.window.data1, event.window.data2);
                                m_specific->m_update_flag = true;
                                break;
                            default:
                                break;
                        }
                        break;
                    case SDL_APP_DIDENTERFOREGROUND:
                        m_specific->m_update_flag = true;
                        on_ctrl_change();
                        break;
                    case SDL_APP_WILLENTERBACKGROUND:
                        m_wait_mode = true;
                        m_specific->m_update_flag = false;
                        break;
                    case SDL_KEYDOWN: {
                        flags = 0;
                        if (event.key.keysym.mod & KMOD_SHIFT) flags |= kbd_shift;
                        if (event.key.keysym.mod & KMOD_CTRL) flags |= kbd_ctrl;

                        int keyCode = event.key.keysym.sym;
                        bool left = keyCode == key_left;
                        bool up = keyCode == key_up;
                        bool right = keyCode == key_right;
                        bool down = keyCode == key_down;

                        if (m_ctrls.on_arrow_keys(left, right, down, up)) {
                            on_ctrl_change();
                            force_redraw();
                        } else {
                            on_key(m_specific->m_cur_x, m_specific->m_cur_y,
                                   (unsigned int) event.key.keysym.sym, flags);
                        }
                    }
                        break;
                    case SDL_MOUSEMOTION:
                        y = m_flip_y ? m_rbuf_window.height() - event.motion.y :
                            (unsigned int) event.motion.y;
                        m_specific->m_cur_x = event.motion.x;
                        m_specific->m_cur_y = y;
                        flags = 0;
                        if (event.motion.state & SDL_BUTTON_LMASK) flags |= mouse_left;
                        if (event.motion.state & SDL_BUTTON_RMASK) flags |= mouse_right;

                        if (m_ctrls.on_mouse_move(m_specific->m_cur_x, m_specific->m_cur_y,
                                                  (flags & mouse_left) != 0)) {
                            on_ctrl_change();
                            force_redraw();
                        } else {
                            on_mouse_move(m_specific->m_cur_x, m_specific->m_cur_y, flags);
                        }
                        SDL_Event eventTrash;
                        while (SDL_PeepEvents(&eventTrash, 1, SDL_GETEVENT, SDL_MOUSEMOTION,
                                              SDL_MOUSEMOTION) != 0) {
                        }
                        //DEBUG_PRINT("Mouse Motion: [%dx%d, %d]", m_specific->m_cur_x, m_specific->m_cur_y, flags);
                        break;
                    case SDL_MOUSEBUTTONDOWN:
                        y = m_flip_y ? m_rbuf_window.height() - event.button.y
                                     : (unsigned int) event.button.y;
                        m_specific->m_cur_x = event.button.x;
                        m_specific->m_cur_y = y;
                        switch (event.button.button) {
                            case SDL_BUTTON_LEFT: {
                                flags = mouse_left;
                                if (m_ctrls.on_mouse_button_down(m_specific->m_cur_x,
                                                                 m_specific->m_cur_y)) {
                                    m_ctrls.set_cur(m_specific->m_cur_x, m_specific->m_cur_y);
                                    on_ctrl_change();
                                    force_redraw();
                                } else {
                                    if (m_ctrls.in_rect(m_specific->m_cur_x, m_specific->m_cur_y)) {
                                        if (m_ctrls.set_cur(m_specific->m_cur_x,
                                                            m_specific->m_cur_y)) {
                                            on_ctrl_change();
                                            force_redraw();
                                        }
                                    } else {
                                        on_mouse_button_down(m_specific->m_cur_x,
                                                             m_specific->m_cur_y, flags);
                                    }
                                }
                            }
                                break;
                            case SDL_BUTTON_RIGHT:
                                flags = mouse_right;
                                on_mouse_button_down(m_specific->m_cur_x, m_specific->m_cur_y,
                                                     flags);
                                break;
                        }
                        //DEBUG_PRINT("Mouse Down: [%dx%d, %d]", m_specific->m_cur_x, m_specific->m_cur_y, flags);
                        break;
                    case SDL_MOUSEBUTTONUP:
                        y = m_flip_y ? m_rbuf_window.height() - event.button.y
                                     : (unsigned int) event.button.y;
                        m_specific->m_cur_x = event.button.x;
                        m_specific->m_cur_y = y;
                        flags = 0;
                        if (m_ctrls.on_mouse_button_up(m_specific->m_cur_x, m_specific->m_cur_y)) {
                            on_ctrl_change();
                            force_redraw();
                        }
                        on_mouse_button_up(m_specific->m_cur_x, m_specific->m_cur_y, flags);
                        //DEBUG_PRINT("Mouse Up: [%dx%d, %d]", m_specific->m_cur_x, m_specific->m_cur_y, flags);
                        break;
                    case SDL_FINGERDOWN:
                        //DEBUG_PRINT("Finger Down(%f, %f)", event.mgesture.x, event.mgesture.y);
                        break;
                    case SDL_FINGERUP:
                        //DEBUG_PRINT("Finger Up(%f, %f)", event.mgesture.x, event.mgesture.y);
                        break;
                    case SDL_FINGERMOTION:
                        //DEBUG_PRINT("Finger Motion(%f, %f)", event.mgesture.x, event.mgesture.y);
                        break;
                    case SDL_MULTIGESTURE:
                        //DEBUG_PRINT(
                                //"Multi Gesture: x = %f, y = %f, dAng = %f, dR = %f num = %i\n",
                                //event.mgesture.x,
                                //event.mgesture.y,
                                //event.mgesture.dTheta,
                                //event.mgesture.dDist,
                                //event.mgesture.numFingers);
                        break;
                    case 769:
                        //DEBUG_PRINT("769: call init");
                        if (!init(300, 400, m_window_flags)) {
                            DEBUG_PRINT("init failed");
                            return false;
                        }
                        break;
                    default:
                        DEBUG_PRINT("Unknown Event %d\n", event.type);
                        break;
                }
            }
        }
        return 0;
    }


    //------------------------------------------------------------------------
    const char *platform_support::img_ext() const { return ".bmp"; }

    //------------------------------------------------------------------------
    const char *platform_support::full_file_name(const char *file_name) {
        return file_name;
    }

    //------------------------------------------------------------------------
    bool platform_support::load_img(unsigned idx, const char *file) {
        if (idx < max_images) {
            if (m_specific->m_surf_img[idx]) SDL_FreeSurface(m_specific->m_surf_img[idx]);

            char fn[1024];
            strcpy(fn, file);
            size_t len = strlen(fn);
            if (len < 4 || strcmp(fn + len - 4, ".bmp") != 0) {
                strcat(fn, ".bmp");
            }

            SDL_Surface *tmp_surf = SDL_LoadBMP(fn);
            if (tmp_surf == 0) {
                ERROR_PRINT("Couldn't load %s: %s\n", fn, SDL_GetError());
                return false;
            }

            SDL_PixelFormat format;
            format.palette = 0;
            format.BitsPerPixel = (Uint8) m_bpp;
            format.BytesPerPixel = (Uint8) (m_bpp >> 8);
            format.Rmask = m_specific->m_rmask;
            format.Gmask = m_specific->m_gmask;
            format.Bmask = m_specific->m_bmask;
            format.Amask = m_specific->m_amask;
            format.Rshift = 0;
            format.Gshift = 0;
            format.Bshift = 0;
            format.Ashift = 0;
            format.Rloss = 0;
            format.Gloss = 0;
            format.Bloss = 0;
            format.Aloss = 0;

            m_specific->m_surf_img[idx] = SDL_ConvertSurface(tmp_surf, &format, SDL_SWSURFACE);
            SDL_FreeSurface(tmp_surf);
            if (m_specific->m_surf_img[idx] == 0) return false;

            m_rbuf_img[idx].attach((unsigned char *) m_specific->m_surf_img[idx]->pixels,
                                   (unsigned int) m_specific->m_surf_img[idx]->w,
                                   (unsigned int) m_specific->m_surf_img[idx]->h,
                                   m_flip_y ? -m_specific->m_surf_img[idx]->pitch :
                                   m_specific->m_surf_img[idx]->pitch);
            return true;
        }
        return false;
    }


    //------------------------------------------------------------------------
    bool platform_support::save_img(unsigned idx, const char *file) {
        if (idx < max_images && m_specific->m_surf_img[idx]) {
            char fn[1024];
            strcpy(fn, file);
            size_t len = strlen(fn);
            if (len < 4 || strcmp(fn + len - 4, ".bmp") != 0) {
                strcat(fn, ".bmp");
            }
            return SDL_SaveBMP(m_specific->m_surf_img[idx], fn) == 0;
        }
        return false;
    }


    //------------------------------------------------------------------------
    bool platform_support::create_img(unsigned idx, unsigned width, unsigned height) {
        if (idx < max_images) {
            if (m_specific->m_surf_img[idx]) SDL_FreeSurface(m_specific->m_surf_img[idx]);
            m_specific->m_surf_img[idx] =
                    SDL_CreateRGBSurface(SDL_SWSURFACE,
                                         width,
                                         height,
                                         m_specific->m_surface->format->BitsPerPixel,
                                         m_specific->m_rmask,
                                         m_specific->m_gmask,
                                         m_specific->m_bmask,
                                         m_specific->m_amask);
            if (m_specific->m_surf_img[idx] == 0) {
                ERROR_PRINT("Couldn't create image: %s\n", SDL_GetError());
                return false;
            }

            m_rbuf_img[idx].attach((unsigned char *) m_specific->m_surf_img[idx]->pixels,
                                   (unsigned int) m_specific->m_surf_img[idx]->w,
                                   (unsigned int) m_specific->m_surf_img[idx]->h,
                                   m_flip_y ? -m_specific->m_surf_img[idx]->pitch :
                                   m_specific->m_surf_img[idx]->pitch);
            return true;
        }
        return false;
    }

    //------------------------------------------------------------------------
    void platform_support::start_timer() {
        m_specific->m_sw_start = SDL_GetTicks();
    }

    //------------------------------------------------------------------------
    double platform_support::elapsed_time() const {
        int stop = SDL_GetTicks();
        return double(stop - m_specific->m_sw_start);
    }

    //------------------------------------------------------------------------
    void platform_support::message(const char *msg) {
        DEBUG_PRINT("Message: %s", msg);
    }

    //------------------------------------------------------------------------
    void platform_support::force_redraw() {
        m_specific->m_update_flag = true;
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

    void platform_support::on_post_draw(void *raw_handler) {}
}


int agg_main(int argc, char *argv[]);

#ifdef __ANDROID__
#define MAIN SDL_main
#else
#define MAIN main
#endif

int MAIN(int argc, char *argv[]) {
    return agg_main(argc, argv);
}
