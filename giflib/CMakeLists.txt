add_library(gif_animation STATIC gif_animation.h)

target_link_libraries(gif_animation bmppicture -static)
# link_libraries(${PROJECT_NAME} bmppicture)

target_include_directories(gif_animation PUBLIC ${PROJECT_SOURCE_DIR}/lib)
