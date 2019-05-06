# Anti-Grain Geometry Library

Anti-Grain Geometry library, written by Maxim Shemanarev in C++. It is an Open Source, 2D vector graphics library.
Agg produces pixel images in memory from vectorial data.

## About the Project
Anti-Grain Geometry (AGG) is an Open Source, free of charge graphic library, written in industrially standard C++. The terms and conditions of use AGG are described on The License page. AGG doesn't depend on any graphic API or technology. Basically, you can think of AGG as of a rendering engine that produces pixel images in memory from some vectorial data. But of course, AGG can do much more than that. The ideas and the philosophy of AGG are:

- **Anti-Aliasing**.
- **Subpixel Accuracy**.
- The highest possible quality.
- High performance.
- Platform independence and compatibility.
- Flexibility and extensibility.
- Lightweight design.
- Reliability and stability (including numerical stability).
- Below there are some key features (but not all of them):
- Rendering of arbitrary polygons with **Anti-Aliasing** and **Subpixel Accuracy**.
- Gradients and Gouraud Shading.
- Fast filtered image affine transformations, including many interpolation filters (bilinear, bicubic, spline16, spline36, sinc, Blackman).
- Strokes with different types of line joins and line caps.
- Dashed line generator.
- Markers, such as arrowheads/arrowtails.
- Fast vectorial polygon clipping to a rectangle.
- Low-level clipping to multiple rectangular regions.
- Alpha-Masking.
- A new, fast **Anti-Alias** line algorithm.
- Using arbitrary images as line patterns.
- Rendering in separate color channels.
- Perspective and bilinear transformations of vector and image data.
- Boolean polygon operations (and, or, xor, sub) based on Alan Murta's **General Polygon Clipper**.

Anti-Grain Geometry contains many interactive Demo examples that are platform independent too,
and use a simple platform_support class currently implemented for Win32, X11, SDL, MacOS, AmigaOS and BeOS.
One of the examples is an SVG Viewer.

###
Cloned on May 6, 2019 (rev r132) from Sourceforge AGG project at https://svn.code.sf.net/p/agg/svn.
