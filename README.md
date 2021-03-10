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
and use a simple platform_support class currently implemented for Win32, X11, SDL, Nano-X, MacOS, AmigaOS and BeOS.
One of the examples is an SVG Viewer.

###
Cloned with history on May 6, 2019 (rev r132) from the official Sourceforge AGG project at
https://svn.code.sf.net/p/agg/svn.

## License

AGG 2.6, along with the official Sourceforge AGG project, is based off of AGG 2.4, which
is dual licensed by either a Modified BSD License, or an Anti-Grain Geometry Public License.
These licenses allow for free use in commercial software.

There exists an AGG 2.5 that was created with a GNU GPL License, with no other changes from 2.4.
The AGG 2.5 version is not included here nor in the Sourceforge repositories.

There remains a seperate license for the General Polygon Clipper (GPC) code, which
is free only in non-commercial software. However, GPC is not an obligatory part of AGG
and can be easily removed.

Refer to [License HTML](https://github.com/ghaerr/agg-2.6/blob/master/agg-web/license/index.html)
for license texts and GPC details.

## Building

The library examples show the amazing capabilities of the AGG C++ template library. There are scripts
for autogen and configure in the source directory, but haven't found these to work well, and
they're not needed to build the demos anyways. X11, SDL2, Nano-X and Win32 are supported
for the demo builds, which visually show various complex rendering pipelines in the library.
These should build on OSX and Linux (OSX is currently tested).
There is additional untested platform support for MacOS Carbon, BeOS and AmigaOS.

To build the demos for X11, first check the Makefile in `agg-2.6/agg-src/Makefile.in.$(uname)`
corresponding to your system's `uname` and set the location of the X11 -Iinclude and -Llib paths.
Then:

```
cd agg-2.6/agg-src/examples/X11
make
(if have Freetype2 installed, 'make all' will build further examples)
```

Likewise, to build the demos for SDL2, first check the Makefile in `agg-2.6/agg-src/Makefile.in.$(uname).SDL2`
corresponding to your system's `uname` and set the location of the SDL2 -Iinclude and -Llib paths.
Then:

```
cd agg-2.6/agg-src/examples/SDL2
make
```

To build the demos for Nano-X, first check the Makefile in
`agg-2.6/agg-src/Makefile.in.$(uname).nano-X` corresponding to your system's
`uname` and set the location of the nano-X -Iinclude and -Llib paths.
For instance, `/path/to/microwindows/src/include` for include, and
`/path/to/microwindows/src/lib` for the lib directory.
Then:

```
cd agg-2.6/agg-src/examples/nano-X
make
(if have Freetype2 installed, 'make all' will build further examples)
./runapp line_patterns (to run demo app and nano-X server)
```
## Roadmap

The official AGG site remains on Sourceforge, but doesn't see much activity.
With www.antigrain.com recently inoperational, I thought a GitHub repo was in order,
as the project demos stopped building with the site down. The demos now build without
the website.

I have created PDFs of all of Maxim's articles from the previous website and may put
those up as well, as they greatly aid in understanding the library.
