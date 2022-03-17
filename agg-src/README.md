# The Anti-Grain Geometry Project

A high quality rendering engine for C++
http://antigrain.com

Anti-Grain Geometry - Version 2.4
Copyright (C) 2002-2005 Maxim Shemanarev (McSeem)

Permission to copy, use, modify, sell and distribute this software
is granted provided this copyright notice appears in all copies.
This software is provided "as is" without express or implied
warranty, and with no claim as to its suitability for any purpose.

---------------------------------

requires: 
- a modern cmake installation >= 3.13
- a conan installation (if not yet installed: try pip install conan)


Use cmake and conan to build the library.

1. create a build folder
2. enter build folder
3. run "conan install .."
4. run "conan build .."

conan will install all necessary dependancies and trigger the cmake build process

To use GPC library, add -DUSE_GPC to the definitions, for example in the cmake file.
Caution! GPC licence only allows non-commercial use.
---------------------------------
