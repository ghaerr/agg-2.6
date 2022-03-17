# agg2d for windows implementation
add_library( agg2d STATIC agg2d.cpp agg2d.h)
target_include_directories(agg2d PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../include)
target_include_directories(agg2d PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../font_win32_tt)