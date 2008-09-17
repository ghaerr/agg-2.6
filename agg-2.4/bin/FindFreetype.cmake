# \file bin/FindFreetype.cmake
# \author Erik van der Wal
#
#     Copyright: 2004 (c) Erik van der Wal
#
#     Licence: wxWidgets Licence
#
#     RCS-ID: $Id: FindFreetype.cmake,v 1.7 2008/07/20 12:27:37 titato Exp $

# This module finds if freetype is installed and finds its settings.
# This code may set/use the following variables:
#
# FT2_ROOT_DIR      - Directory where freetype is installed
# FT2_DEFINITIONS   - Definitions to use when compiling code
# FT2_INCLUDE_DIR   - Directories to include to use freetype
# FT2_LIBRARIES     - Files to link against to use freetype (and linker flags on unix)
# FT2_LINK_DIR      - Directories to link to for using freetype
# FT2_FOUND         - If false, don't try to use freetype
#
# Description:
# First, I want to give credits to Marcelo Magallon, whose code I used
# as a base for this detection routine. (freetype2.m4)
#
# The settings for unix(like) systems are found by using freetype-config
# This script is found, in the following order:
# 1) file in environment variable FT2_CONFIG
# 2) path in environment variable FT2
# 3) path in environment variable FREETYPE
# 4) path in cmake variable FT2_ROOT_DIR
# 5) Search path
# If the wrong file is found, it can allway be overrided in the gui.
# 
# To Do:
# a) Version detection.
#    Tested with freetype 2.1.8 (=9.6.3) on mingw
# b) PC-version of the freetype detection is very difficult. When
# examining the visualc dsw files, it looks like they generate a lot
# of possible libraries again, including library version, which makes it
# very hard to make a descent detection scheme.
# Possible library names for version 2.1.8
# - freetype218.lib
# - freetype218_D.lib
# - freetype200b8_D.lib
# - freetype218MT_D.lib
# - freetype200b8.lib
# - freetype218MT.lib
# - freetype206.lib
# - freetype218ST.lib
# - freetype206_D.lib
# - freetype218ST_D.lib
# And this list is only for one version of freetype.
# Therefore, for now, I only implemented a test routine. This assumes,
# that the user entered by hand, at least the following variables:
#    FT2_INCLUDE_DIR: directory where the ft2build is located
#    FT2_LINK_DIR: directory where the freetype*.lib is located
#    FT2_LIBRARIES: the library name itself.

OPTION( FT2_NO_TEST "Do not try to compile and run a test Freetype program" OFF )

# Mark advanced settings
MARK_AS_ADVANCED( FORCE FT2_INCLUDE_DIR )
MARK_AS_ADVANCED( FORCE FT2_LINK_DIR )
MARK_AS_ADVANCED( FORCE FT2_LIBRARIES )
MARK_AS_ADVANCED( FORCE FT2_FOUND )
MARK_AS_ADVANCED( FORCE FT2_NO_TEST )

# Freetype test macro. This actually compiles and runs a piece of
# test software.
MACRO(FT2_TEST IS_OK)
    IF( FT2_NO_TEST )
        SET( ${IS_OK} TRUE )
    ELSE( FT2_NO_TEST )

        SET( TESTCODE 
            "#include <ft2build.h>\n#include FT_FREETYPE_H\n#include <stdio.h>\n#include <stdlib.h>\nint main()\n{\n\tFT_Library library;\n\tFT_Error  error;\n\terror = FT_Init_FreeType(&library);\n\tif (error)\n\t\treturn 1;\n\telse\n\t{\n\t\tFT_Done_FreeType(library);\n\t\treturn 0;\n\t}\n}"
        )
        CONFIGURE_FILE( ${CMAKE_SOURCE_DIR}/bin/testcode.c
            ${CMAKE_BINARY_DIR}/fttest.c
            IMMEDIATE @ONLY
        )

        SET( ${FT2_TEST_RESULT} "" CACHE INTERNAL abc FORCE)
        TRY_RUN( FT2_TEST_RESULT FT2_COMPILE_RESULT
            ${CMAKE_BINARY_DIR}/CMakeTmp
            ${CMAKE_BINARY_DIR}/fttest.c
            CMAKE_FLAGS
                "-DINCLUDE_DIRECTORIES:STRING=${FT2_INCLUDE_DIR}"
                "-DLINK_LIBRARIES:STRING=${FT2_LIBRARIES}"
                "-DLINK_DIRECTORIES:STRING=${FT2_LINK_DIR}"
            COMPILE_DEFINITIONS
            "${FT2_DEFINITIONS}"
        )

        IF( FT2_TEST_RESULT MATCHES "0")
            SET( ${IS_OK} TRUE )
        ELSE( FT2_TEST_RESULT MATCHES "0")
            SET( ${IS_OK} FALSE )
        ENDIF( FT2_TEST_RESULT MATCHES "0")
    ENDIF( FT2_NO_TEST )
ENDMACRO(FT2_TEST)

# Detection routine starts here:
IF( UNIX )
    FIND_PROGRAM( FT2_CONFIG
        NAMES
            $ENV{FT2_CONFIG}
            "freetype-config"
        PATHS
            $ENV{FT2}
            $ENV{FREETYPE}
            ${FT2_ROOT_DIR}
    )
    EXEC_PROGRAM(sh
        ARGS "${FT2_CONFIG} --cflags"
        OUTPUT_VARIABLE FT2_DEFINITIONS
        RETURN_VALUE RET
    )
    EXEC_PROGRAM(sh
        ARGS "${FT2_CONFIG} --libs"
        OUTPUT_VARIABLE FT2_LIBRARIES
        RETURN_VALUE RET
    )

    # Test sanity of library
    FT2_TEST( FT2_FOUND )

ELSE( UNIX )
    #else WIN32
    
    # Only a test routine.
    FT2_TEST( FT2_FOUND )

ENDIF( UNIX )

#MESSAGE( "FT2_ROOT_DIR = ${FT2_ROOT_DIR}" )
#MESSAGE( "FT2_INCLUDE_DIR = ${FT2_INCLUDE_DIR}" )
#MESSAGE( "FT2_DEFINITIONS = ${FT2_DEFINITIONS}" )
#MESSAGE( "FT2_LIBRARIES = ${FT2_LIBRARIES}" )
#MESSAGE( "FT2_LINK_DIR = ${FT2_LINK_DIR}" )
#MESSAGE( "FT2_FOUND = ${FT2_FOUND}" )
