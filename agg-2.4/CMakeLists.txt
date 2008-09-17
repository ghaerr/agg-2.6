PROJECT( antigrain )

CMAKE_MINIMUM_REQUIRED( VERSION 2.4.8 )

SET (LIBRARY_OUTPUT_PATH ${antigrain_BINARY_DIR}/lib/ CACHE PATH "Single output directory for building all libraries." FORCE )
#SET (EXECUTABLE_OUTPUT_PATH ${antigrain_BINARY_DIR}/exe/ CACHE PATH "Single output directory for building all executables.")
#MARK_AS_ADVANCED(LIBRARY_OUTPUT_PATH EXECUTABLE_OUTPUT_PATH)

LINK_DIRECTORIES(  ${antigrain_BINARY_DIR}/lib )

OPTION( agg_USE_GPC "Use Gpc Boolean library" ON)
OPTION( agg_USE_FREETYPE "Use Freetype library" OFF)
OPTION( agg_USE_EXPAT "Use Expat library" OFF)
OPTION( agg_USE_SDL_PLATFORM "Use SDL as platform" OFF)

IF ( agg_USE_GPC )
    INCLUDE_DIRECTORIES( ${antigrain_SOURCE_DIR}/gpc )
ENDIF ( agg_USE_GPC )

# for the moment this decides the platfoirm code.
IF(WIN32)
    ADD_DEFINITIONS( -D_MSWVC_ -D_CRT_SECURE_NO_WARNINGS -D_CRT_SECURE_NO_DEPRECATE )
    SET( WIN32GUI WIN32 )    
    INCLUDE_DIRECTORIES( ${antigrain_SOURCE_DIR}/font_win32_tt )
    INCLUDE_DIRECTORIES( ${antigrain_SOURCE_DIR}/font_freetype )
ENDIF(WIN32)

IF(UNIX)
    ADD_DEFINITIONS( -D__UNIX__  )
    SET( WIN32GUI "" )
    INCLUDE_DIRECTORIES( ${antigrain_SOURCE_DIR}/font_freetype )

    FIND_PACKAGE(X11)
    IF(X11_FOUND)  
        INCLUDE_DIRECTORIES(${X11_INCLUDE_DIRS})
        LINK_LIBRARIES(${X11_LIBRARIES})
    ENDIF(X11_FOUND)

ENDIF(UNIX)

# more specific set platform code part to use for different compilers/tool sets
IF ( ${CMAKE_GENERATOR} STREQUAL "MSYS Makefiles" )
	SET (CMAKE_CXX_FLAGS_RELEASE "-DNDEBUG" CACHE STRING
		"Flags used by the compiler during release builds" FORCE)
	SET (CMAKE_CX_FLAGS_RELEASE "-DNDEBUG" CACHE STRING
		"Flags used by the compiler during release builds" FORCE)		
ENDIF ( ${CMAKE_GENERATOR} STREQUAL "MSYS Makefiles" )

IF ( ${CMAKE_GENERATOR} STREQUAL "MinGW Makefiles" )
	
ENDIF ( ${CMAKE_GENERATOR} STREQUAL "MinGW Makefiles" )

IF ( ${CMAKE_GENERATOR} STREQUAL "Unix Makefiles" )
    IF( CYGWIN OR MINGW )
		SET (CMAKE_CXX_FLAGS_RELEASE "-DNDEBUG" CACHE STRING
			"Flags used by the compiler during release builds" FORCE)
		SET (CMAKE_C_FLAGS_RELEASE "-DNDEBUG" CACHE STRING
			"Flags used by the compiler during release builds" FORCE)
    ENDIF( CYGWIN OR MINGW )	
ENDIF ( ${CMAKE_GENERATOR} STREQUAL "Unix Makefiles" )

IF ( ${CMAKE_GENERATOR} MATCHES "Visual Studio.*" )
	
ENDIF ( ${CMAKE_GENERATOR} MATCHES "Visual Studio.*" )

IF ( ${CMAKE_GENERATOR} MATCHES "Borland Makefiles" )

ENDIF ( ${CMAKE_GENERATOR} MATCHES "Borland Makefiles" )

##################################################
# Set all includes, flags, libraries, related to expat
##################################################

IF( agg_USE_EXPAT )

    FIND_PACKAGE( EXPAT )                    

    IF(EXPAT_FOUND)  
        INCLUDE_DIRECTORIES(${EXPAT_INCLUDE_DIRS})
        LINK_LIBRARIES(${EXPAT_LIBRARIES})
    ELSE(EXPAT_FOUND)  
        MESSAGE(SEND_ERROR "expat not found")
    ENDIF(EXPAT_FOUND)
ENDIF( agg_USE_EXPAT )
   
##################################################
# Set all includes, flags, libraries, related to freetype
##################################################

#FindFreetype: Locate FreeType library
# FREETYPE_LIBRARIES, the library to link against  
# FREETYPE_FOUND, if false, do not try to link to 
# FREETYPE  FREETYPE_INCLUDE_DIRS, where to find #headers.  
# This is the concatenation of the paths:  FREETYPE_INCLUDE_DIR_ft2build  FREETYPE_INCLUDE_DIR_freetype2

IF( agg_USE_FREETYPE )
    FIND_PACKAGE( Freetype )                    
    IF( FREETYPE_FOUND )
        INCLUDE_DIRECTORIES( ${FREETYPE_INCLUDE_DIRS} )
        LINK_LIBRARIES( ${FREETYPE_LIBRARIES} )
        LINK_DIRECTORIES( ${FREETYPE_LINK_DIR} )
    ELSE( FREETYPE_FOUND )
        MESSAGE(SEND_ERROR "freetype not found")
    ENDIF( FREETYPE_FOUND )
ENDIF( agg_USE_FREETYPE )

##################################################
# Set all includes, flags, libraries, related to SDL
##################################################

FIND_PACKAGE( SDL QUIET )
IF( SDL_FOUND )
    IF ( agg_USE_SDL_PLATFORM )
        INCLUDE_DIRECTORIES(${SDL_INCLUDE_DIR})
        LINK_LIBRARIES(${SDL_LIBRARY})
    ENDIF ( agg_USE_SDL_PLATFORM )
ELSE( SDL_FOUND )
    IF ( agg_USE_SDL_PLATFORM )
        MESSAGE( "SDL libray was not found, disable agg_USE_SDL_PLATFORM please" )
    ENDIF ( agg_USE_SDL_PLATFORM )
ENDIF( SDL_FOUND )

INCLUDE_DIRECTORIES( ${antigrain_SOURCE_DIR}/include )

ADD_SUBDIRECTORY( src )

IF( agg_USE_FREETYPE )
    LINK_LIBRARIES( freetypefont )
ENDIF( agg_USE_FREETYPE )

IF ( agg_USE_GPC )
    LINK_LIBRARIES( gpcbool )
ENDIF ( agg_USE_GPC )

IF( SDL_FOUND AND agg_USE_SDL_PLATFORM )
    LINK_LIBRARIES( controls sdlplatform antigrain )
ELSE( SDL_FOUND AND agg_USE_SDL_PLATFORM )
    LINK_LIBRARIES( controls platform antigrain )
ENDIF( SDL_FOUND AND agg_USE_SDL_PLATFORM )

SUBDIRS( examples )
 
