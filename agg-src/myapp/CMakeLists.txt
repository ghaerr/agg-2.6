add_executable(my_demo WIN32 my_demo.cpp)
target_link_libraries(my_demo PRIVATE antigrain controls platform)

if(WIN32)
    add_executable(agg2d_demo agg2d_demo.cpp)
    target_link_libraries(agg2d_demo PRIVATE antigrain controls platform)
endif()


