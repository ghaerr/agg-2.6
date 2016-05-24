#include <stdio.h>
#include <string.h>
#include "agg_pixfmt_rgb24.h"
#include "agg_renderer_base.h"
#include "agg_renderer_scanline.h"
#include "agg_scanline_u.h"
#include "agg_rasterizer_scanline_aa.h"
#include "agg_ellipse.h"
#include "agg_span_gradient.h"
#include "agg_span_interpolator_linear.h"

enum
{
    frame_width = 320,
    frame_height = 200
};

// Writing the buffer to a .PPM file, assuming it has 
// RGB-structure, one byte per color component
//--------------------------------------------------
bool write_ppm(const unsigned char* buf, 
               unsigned width, 
               unsigned height, 
               const char* file_name)
{
    FILE* fd = fopen(file_name, "wb");
    if(fd)
    {
        fprintf(fd, "P6 %d %d 255 ", width, height);
        fwrite(buf, 1, width * height * 3, fd);
        fclose(fd);
        return true;
    }
    return false;
}



// A simple function to form the gradient color array 
// consisting of 3 colors, "begin", "middle", "end"
//---------------------------------------------------
void fill_color_array(agg::rgba8* array, 
                      agg::rgba8 begin, 
                      agg::rgba8 middle, 
                      agg::rgba8 end)
{
    unsigned i;
    for(i = 0; i < 128; ++i)
    {
        array[i] = begin.gradient(middle, i / 128.0);
    }
    for(; i < 256; ++i)
    {
        array[i] = middle.gradient(end, (i - 128) / 128.0);
    }
}






int main()
{
    unsigned char* buffer = new unsigned char[frame_width * frame_height * 3];

    agg::rendering_buffer rbuf(buffer, 
                               frame_width, 
                               frame_height, 
                               -frame_width * 3);

    // Pixel format and basic renderers.
    //-----------------
    typedef agg::pixfmt_rgb24 pixfmt_type;
    typedef agg::renderer_base<pixfmt_type> renderer_base_type;


    // Gradient shape function (linear, radial, custom, etc)
    //-----------------
    typedef agg::gradient_x gradient_func_type;   


    // Span interpolator. This object is used in all span generators 
    // that operate with transformations during iterating of the spans,
    // for example, image transformers use the interpolator too.
    //-----------------
    typedef agg::span_interpolator_linear<> interpolator_type;


    // Span allocator is an object that allocates memory for 
    // the array of colors that will be used to render the 
    // color spans. One object can be shared between different 
    // span generators.
    //-----------------
    typedef agg::span_allocator<agg::rgba8> span_allocator_type;


    // Finally, the gradient span generator working with the agg::rgba8 
    // color type. 
    // The 4-th argument is the color function that should have 
    // the [] operator returning the color in range of [0...255].
    // In our case it will be a simple look-up table of 256 colors.
    //-----------------
    typedef agg::span_gradient<agg::rgba8, 
                               interpolator_type, 
                               gradient_func_type, 
                               const agg::rgba8*,
                               span_allocator_type> span_gradient_type;


    // The gradient scanline renderer type
    //-----------------
    typedef agg::renderer_scanline_aa<renderer_base_type, 
                                      span_gradient_type> renderer_gradient_type;


    // Common declarations (pixel format and basic renderer).
    //----------------
    pixfmt_type pixf(rbuf);
    renderer_base_type rbase(pixf);


    // The gradient objects declarations
    //----------------
    gradient_func_type  gradient_func;                   // The gradient function
    agg::trans_affine   gradient_mtx;                    // Affine transformer
    interpolator_type   span_interpolator(gradient_mtx); // Span interpolator
    span_allocator_type span_allocator;                  // Span Allocator
    agg::rgba8          color_array[256];                // The gradient colors

    // Declare the gradient span itself. 
    // The last two arguments are so called "d1" and "d2" 
    // defining two distances in pixels, where the gradient starts
    // and where it ends. The actual meaning of "d1" and "d2" depands
    // on the gradient function.
    //----------------
    span_gradient_type span_gradient(span_allocator, 
                                     span_interpolator, 
                                     gradient_func, 
                                     color_array, 
                                     0, 100);

    // The gradient renderer
    //----------------
    renderer_gradient_type ren_gradient(rbase, span_gradient);


    // The rasterizing/scanline stuff
    //----------------
    agg::rasterizer_scanline_aa<> ras;
    agg::scanline_u8 sl;


    // Finally we can draw a circle.
    //----------------
    rbase.clear(agg::rgba8(255, 255, 255));

    fill_color_array(color_array, 
                     agg::rgba8(0,50,50), 
                     agg::rgba8(240, 255, 100), 
                     agg::rgba8(80, 0, 0));

    agg::ellipse ell(50, 50, 50, 50, 100);
    ras.add_path(ell);

    agg::render(ras, sl, ren_gradient);

    write_ppm(buffer, frame_width, frame_height, "agg_test.ppm");

    delete [] buffer;
    return 0;
}




/*
------------------------------
Step 1.

gradients1.png

------------------------------
Step 2.


    // Declare the gradient span itself. 
    // The last two arguments are so called "d1" and "d2" 
    // defining two distances in pixels, where the gradient starts
    // and where it ends. The actual meaning of "d1" and "d2" depands
    // on the gradient function.
    //----------------
    span_gradient_type span_gradient(span_allocator, 
                                     span_interpolator, 
                                     gradient_func, 
                                     color_array, 
                                     50, 100);

gradients2.png

------------------------------
Step 3.

    // Gradient shape function (linear, radial, custom, etc)
    //-----------------
    typedef agg::gradient_circle gradient_func_type;   

    
    // . . .


    // Declare the gradient span itself. 
    // The last two arguments are so called "d1" and "d2" 
    // defining two distances in pixels, where the gradient starts
    // and where it ends. The actual meaning of "d1" and "d2" depands
    // on the gradient function.
    //----------------
    span_gradient_type span_gradient(span_allocator, 
                                     span_interpolator, 
                                     gradient_func, 
                                     color_array, 
                                     0, 100);

    // . . .

    agg::ellipse ell(0, 0, 120, 120, 100);


gradients3.png

------------------------------
Step 4.

    // Declare the gradient span itself. 
    // The last two arguments are so called "d1" and "d2" 
    // defining two distances in pixels, where the gradient starts
    // and where it ends. The actual meaning of "d1" and "d2" depands
    // on the gradient function.
    //----------------
    span_gradient_type span_gradient(span_allocator, 
                                     span_interpolator, 
                                     gradient_func, 
                                     color_array, 
                                     50, 100);

gradients4.png

------------------------------
Step 5.


    gradient_mtx *= agg::trans_affine_scaling(0.75, 1.2);
    gradient_mtx *= agg::trans_affine_rotation(-agg::pi/3.0);
    gradient_mtx *= agg::trans_affine_translation(100.0, 100.0);
    gradient_mtx.invert();

    agg::ellipse ell(100, 100, 120, 120, 100);


gradients5.png

------------------------------
Step 6.


Return to the original code


// Calculate the affine transformation matrix for the linear gradient 
// from (x1, y1) to (x2, y2). gradient_d2 is the "base" to scale the
// gradient. Here d1 must be 0.0, and d2 must equal gradient_d2.
//---------------------------------------------------------------
void calc_linear_gradient_transform(double x1, double y1, double x2, double y2, 
                                    agg::trans_affine& mtx,
                                    double gradient_d2 = 100.0)
{
    double dx = x2 - x1;
    double dy = y2 - y1;
    mtx.reset();
    mtx *= agg::trans_affine_scaling(sqrt(dx * dx + dy * dy) / gradient_d2);
    mtx *= agg::trans_affine_rotation(atan2(dy, dx));
    mtx *= agg::trans_affine_translation(x1, y1);
    mtx.invert();
}



    agg::ellipse ell(100, 100, 120, 120, 100);


gradients6.png

*/