
add_executable( aa_demo WIN32 aa_demo.cpp)
target_link_libraries(aa_demo PRIVATE antigrain controls platform)

add_executable( aa_test WIN32    aa_test.cpp)
target_link_libraries(aa_test PRIVATE antigrain controls platform)

add_executable( alpha_gradient WIN32    alpha_gradient.cpp)
target_link_libraries(alpha_gradient PRIVATE antigrain controls platform)

add_executable( alpha_mask WIN32    alpha_mask.cpp    parse_lion.cpp)
target_link_libraries(alpha_mask PRIVATE antigrain controls platform)

add_executable( alpha_mask2 WIN32    alpha_mask.cpp    parse_lion.cpp)
target_link_libraries(alpha_mask2 PRIVATE antigrain controls platform)

add_executable( alpha_mask3 WIN32
    alpha_mask.cpp
    make_arrows.cpp
    make_gb_poly.cpp
    parse_lion.cpp
)
target_link_libraries(alpha_mask3 PRIVATE antigrain controls platform)

add_executable( bezier_div WIN32
    bezier_div.cpp
    interactive_polygon.cpp
)
target_link_libraries(bezier_div PRIVATE antigrain controls platform)

add_executable( blend_color WIN32
    blend_color.cpp
)
target_link_libraries(blend_color PRIVATE antigrain controls platform)

add_executable( blur WIN32
    blur.cpp
)
target_link_libraries(blur PRIVATE antigrain controls platform)

add_executable( bspline WIN32
    bspline.cpp
    interactive_polygon.cpp
)
target_link_libraries(bspline PRIVATE antigrain controls platform)

add_executable( circles WIN32
    circles.cpp
)
target_link_libraries(circles PRIVATE antigrain controls platform)

add_executable( component_rendering WIN32
    component_rendering.cpp
)
target_link_libraries(component_rendering PRIVATE antigrain controls platform)

add_executable( compositing WIN32
    compositing.cpp
)
target_link_libraries(compositing PRIVATE antigrain controls platform)

add_executable( compositing2 WIN32
    compositing2.cpp
)
target_link_libraries(compositing2 PRIVATE antigrain controls platform)

add_executable( conv_contour WIN32
    conv_contour.cpp
)
target_link_libraries(conv_contour PRIVATE antigrain controls platform)

add_executable( conv_dash_marker WIN32
    conv_dash_marker.cpp
)
target_link_libraries(conv_dash_marker PRIVATE antigrain controls platform)

add_executable( conv_stroke WIN32
    conv_stroke.cpp
)
target_link_libraries(conv_stroke PRIVATE antigrain controls platform)

add_executable( distortions WIN32
    distortions.cpp
)
target_link_libraries(distortions PRIVATE antigrain controls platform)

add_executable( flash_rasterizer WIN32
    flash_rasterizer.cpp
)
target_link_libraries(flash_rasterizer PRIVATE antigrain controls platform)

add_executable( flash_rasterizer2 WIN32
    flash_rasterizer2.cpp
)
target_link_libraries(flash_rasterizer2 PRIVATE antigrain controls platform)

if ( WIN32 )
    add_executable( freetype_test WIN32
        freetype_test.cpp
        make_arrows.cpp
        make_gb_poly.cpp
    )
    target_link_libraries(freetype_test PRIVATE antigrain controls platform)
endif ()

add_executable( gamma_correction WIN32
    gamma_correction.cpp
)
target_link_libraries(gamma_correction PRIVATE antigrain controls platform)

add_executable( gamma_ctrl WIN32
    gamma_ctrl.cpp
)
target_link_libraries(gamma_ctrl PRIVATE antigrain controls platform)

add_executable( gamma_tuner WIN32
    gamma_tuner.cpp
)
target_link_libraries(gamma_tuner PRIVATE antigrain controls platform)

add_executable( gouraud WIN32
    gouraud.cpp
)
target_link_libraries(gouraud PRIVATE antigrain controls platform)

add_executable( gouraud_mesh WIN32
    gouraud_mesh.cpp
)
target_link_libraries(gouraud_mesh PRIVATE antigrain controls platform)

if(USE_GPC)
    add_executable( gpc_test WIN32
        gpc_test.cpp
        make_arrows.cpp
        make_gb_poly.cpp
    )
    target_link_libraries(gpc_test PRIVATE antigrain controls platform gpcbool)    
endif()

add_executable( gradients WIN32
    gradients.cpp
)
target_link_libraries(gradients PRIVATE antigrain controls platform)

add_executable( gradient_focal WIN32
    gradient_focal.cpp
)
target_link_libraries(gradient_focal PRIVATE antigrain controls platform)

add_executable( gradients_contour WIN32
    gradients_contour.cpp
    make_arrows.cpp
    make_gb_poly.cpp
    parse_lion.cpp
)
target_link_libraries(gradients_contour PRIVATE antigrain controls platform)

add_executable( graph_test WIN32
    graph_test.cpp
)
target_link_libraries(graph_test PRIVATE antigrain controls platform)

add_executable( idea WIN32
    idea.cpp
)
target_link_libraries(idea PRIVATE antigrain controls platform)

add_executable( image1 WIN32
    image1.cpp
)
target_link_libraries(image1 PRIVATE antigrain controls platform)

add_executable( image_alpha WIN32
    image_alpha.cpp
)
target_link_libraries(image_alpha PRIVATE antigrain controls platform)

add_executable( image_filters WIN32
    image_filters.cpp
)
target_link_libraries(image_filters PRIVATE antigrain controls platform)

add_executable( image_filters2 WIN32
    image_filters2.cpp
)
target_link_libraries(image_filters2 PRIVATE antigrain controls platform)

add_executable( image_fltr_graph WIN32
    image_fltr_graph.cpp
)
target_link_libraries(image_fltr_graph PRIVATE antigrain controls platform)

add_executable( image_perspective WIN32
    image_perspective.cpp
    interactive_polygon.cpp
)
target_link_libraries(image_perspective PRIVATE antigrain controls platform)

add_executable( image_resample WIN32
    image_resample.cpp
    interactive_polygon.cpp
)
target_link_libraries(image_resample PRIVATE antigrain controls platform)

add_executable( image_transforms WIN32
    image_transforms.cpp
)
target_link_libraries(image_transforms PRIVATE antigrain controls platform)

add_executable( line_patterns WIN32
    line_patterns.cpp
)
target_link_libraries(line_patterns PRIVATE antigrain controls platform)

add_executable( line_patterns_clip WIN32
    line_patterns_clip.cpp
)
target_link_libraries(line_patterns_clip PRIVATE antigrain controls platform)

add_executable( lion WIN32
    lion.cpp
    parse_lion.cpp
)
target_link_libraries(lion PRIVATE antigrain controls platform)

add_executable( lion_lens WIN32
    lion_lens.cpp
    parse_lion.cpp
)
target_link_libraries(lion_lens PRIVATE antigrain controls platform)

add_executable( lion_outline WIN32
    lion_outline.cpp
    parse_lion.cpp
)
target_link_libraries(lion_outline PRIVATE antigrain controls platform)

add_executable( mol_view WIN32
    mol_view.cpp
)
target_link_libraries(mol_view PRIVATE antigrain controls platform)

add_executable( multi_clip WIN32
    multi_clip.cpp
    parse_lion.cpp
)
target_link_libraries(multi_clip PRIVATE antigrain controls platform)

add_executable( pattern_fill WIN32
    pattern_fill.cpp
)
target_link_libraries(pattern_fill PRIVATE antigrain controls platform)

add_executable( pattern_perspective WIN32
    pattern_perspective.cpp
    interactive_polygon.cpp
)
target_link_libraries(pattern_perspective PRIVATE antigrain controls platform)

add_executable( pattern_resample WIN32
    pattern_resample.cpp
    interactive_polygon.cpp
)
target_link_libraries(pattern_resample PRIVATE antigrain controls platform)

add_executable( perspective WIN32
    perspective.cpp
    interactive_polygon.cpp
    parse_lion.cpp
)
target_link_libraries(perspective PRIVATE antigrain controls platform)

add_executable( polymorphic_renderer WIN32
    polymorphic_renderer.cpp
)
target_link_libraries(polymorphic_renderer PRIVATE antigrain controls platform)

add_executable( rasterizers WIN32
    rasterizers.cpp
)
target_link_libraries(rasterizers PRIVATE antigrain controls platform)

add_executable( rasterizers2 WIN32
    rasterizers2.cpp
)
target_link_libraries(rasterizers2 PRIVATE antigrain controls platform)

add_executable( rasterizer_compound WIN32
    rasterizer_compound.cpp
)
target_link_libraries(rasterizer_compound PRIVATE antigrain controls platform)

add_executable( raster_text WIN32
    raster_text.cpp
)
target_link_libraries(raster_text PRIVATE antigrain controls platform)

add_executable( rounded_rect WIN32
    rounded_rect.cpp
)
target_link_libraries(rounded_rect PRIVATE antigrain controls platform)

add_executable( scanline_boolean WIN32
    scanline_boolean.cpp
    interactive_polygon.cpp
)
target_link_libraries(scanline_boolean PRIVATE antigrain controls platform)

add_executable( scanline_boolean2 WIN32
    scanline_boolean2.cpp
    make_arrows.cpp
    make_gb_poly.cpp
)
target_link_libraries(scanline_boolean2 PRIVATE antigrain controls platform)

add_executable( simple_blur WIN32
    simple_blur.cpp
    parse_lion.cpp
)
target_link_libraries(simple_blur PRIVATE antigrain controls platform)

if(WIN32)

    add_executable( trans_curve1 WIN32
        trans_curve1.cpp
        interactive_polygon.cpp
    )
    target_link_libraries(trans_curve1 PRIVATE antigrain controls platform)


    add_executable( trans_curve2 WIN32
        trans_curve2.cpp
        interactive_polygon.cpp
    )
    target_link_libraries(trans_curve2 PRIVATE antigrain controls platform)

    add_executable( truetype_test WIN32
        truetype_test.cpp
    )
    target_link_libraries(truetype_test PRIVATE antigrain controls platform)

endif()

add_executable( trans_polar WIN32
    trans_polar.cpp
)
target_link_libraries(trans_polar PRIVATE antigrain controls platform)

add_executable( svg_test WIN32
    ./svg_viewer/svg_test.cpp
    ./svg_viewer/agg_svg_exception.h
    ./svg_viewer/agg_svg_parser.cpp
    ./svg_viewer/agg_svg_parser.h
    ./svg_viewer/agg_svg_path_renderer.cpp
    ./svg_viewer/agg_svg_path_renderer.h
    ./svg_viewer/agg_svg_path_tokenizer.cpp
    ./svg_viewer/agg_svg_path_tokenizer.h
)
target_link_libraries(svg_test PRIVATE antigrain controls platform)

if(WIN32)
    add_executable( pure_api WIN32
        ./win32_api/pure_api/pure_api.h
        ./win32_api/pure_api/pure_api.cpp
        ./win32_api/pure_api/resource.h
        ./win32_api/pure_api/StdAfx.h
        ./win32_api/pure_api/StdAfx.cpp
        ./win32_api/pure_api/pure_api.rc
        parse_lion.cpp
            
    )
    target_link_libraries(pure_api PRIVATE antigrain controls platform)

    
    add_executable( agg2_demo WIN32
        agg2d_demo.cpp
    )
    target_link_libraries(agg2_demo PRIVATE antigrain controls platform)
    
endif()    
