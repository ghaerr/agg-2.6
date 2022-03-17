# main lib
add_library( antigrain STATIC

    agg_arc.cpp
    agg_arrowhead.cpp
    agg_bezier_arc.cpp
    agg_bspline.cpp
    agg_curves.cpp
    agg_embedded_raster_fonts.cpp
    agg_gsv_text.cpp
    agg_image_filters.cpp
    agg_line_aa_basics.cpp
    agg_line_profile_aa.cpp
    agg_rounded_rect.cpp
    agg_sqrt_tables.cpp
    agg_trans_affine.cpp
    agg_trans_double_path.cpp
    agg_trans_single_path.cpp
    agg_trans_warp_magnifier.cpp
    agg_vcgen_bspline.cpp
    agg_vcgen_contour.cpp
    agg_vcgen_dash.cpp
    agg_vcgen_markers_term.cpp
    agg_vcgen_smooth_poly1.cpp
    agg_vcgen_stroke.cpp
    agg_vpgen_clip_polygon.cpp
    agg_vpgen_segmentator.cpp
)

target_include_directories(antigrain PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../include)
target_link_libraries(antigrain PUBLIC CONAN_PKG::expat)
if(WIN32)
    target_link_libraries(antigrain PUBLIC agg2d)
    target_compile_definitions(antigrain PUBLIC -D_MSWVC_ -D_CRT_SECURE_NO_WARNINGS -D_CRT_SECURE_NO_DEPRECATE)
    target_compile_definitions(antigrain PUBLIC -DAGG_USE_AGG2D)
elseif(UNIX)
    target_compile_definitions(antigrain PUBLIC -D__UNIX__ -DAGG_USE_FREETYPE)
    target_link_libraries(antigrain PUBLIC CONAN_PKG::xorg freetypefont)
endif()


# control lib
add_library( controls STATIC 

    ctrl/agg_spline_ctrl.cpp
    ctrl/agg_slider_ctrl.cpp
    ctrl/agg_scale_ctrl.cpp
    ctrl/agg_rbox_ctrl.cpp
    ctrl/agg_polygon_ctrl.cpp
    ctrl/agg_gamma_spline.cpp
    ctrl/agg_gamma_ctrl.cpp
    ctrl/agg_cbox_ctrl.cpp
    ctrl/agg_bezier_ctrl.cpp

)
target_link_libraries(controls PUBLIC antigrain)

# freetype support lib
add_library(freetypefont STATIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/../font_freetype/agg_font_freetype.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../font_freetype/agg_font_freetype2.cpp
)
target_include_directories(freetypefont PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../font_freetype)
target_link_libraries(freetypefont PUBLIC CONAN_PKG::freetype antigrain controls)
target_compile_definitions(freetypefont PRIVATE -DAGG_USE_FREETYPE)

# platform specific libs
if(WIN32)
    # win support
    add_library(agg-win STATIC ${CMAKE_CURRENT_SOURCE_DIR}/../font_win32_tt/agg_font_win32_tt.cpp)
    target_include_directories(agg-win PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../font_win_32_tt)

    # 2d support
    add_library(agg2d STATIC ${CMAKE_CURRENT_SOURCE_DIR}/../agg2d/agg2d.cpp)
    target_include_directories(agg2d PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../agg2d} ${CMAKE_CURRENT_SOURCE_DIR}/../include)
    target_compile_definitions(agg2d PRIVATE -DAGG_USE_AGG2D)
    target_link_libraries(agg2d PUBLIC agg-win)

    add_library(platform STATIC 
    
        platform/win32/agg_win32_bmp.cpp
        platform/win32/agg_platform_support.qt_wrap_cpp
        platform/win32/agg_font_win32_tt.cpp
        
    )
    target_link_libraries(platform PUBLIC agg2d)
elseif(UNIX)
    # find x11 on unix systems
    find_package(X11)
    add_library(platform STATIC platform/X11/agg_platform_support.cpp)
    target_include_directories(platform PUBLIC ${X11_INCLUDE_DIRS})
    target_link_libraries(platform PUBLIC freetypefont)
    target_link_libraries(platform PUBLIC ${X11_LIBRARIES})
elseif(APPLE)
    add_library(platform STATIC platform/mac/agg_mac_pmap.cpp platform/mac/agg_platform_support.cpp)
endif()
target_link_libraries(platform PUBLIC controls antigrain)

if(USE_GPC)
    # gpc lib
    add_library(gpcbool STATIC ${CMAKE_CURRENT_SOURCE_DIR}/../gpc/gpc.c)
    target_include_directories(gpcbool PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../gpc)
    target_compile_definitions(gpcbool PRIVATE -DAGG_USE_GPC)
endif()