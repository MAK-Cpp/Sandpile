add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} bmppicture -static)
target_link_libraries(${PROJECT_NAME} sandpile -static)
target_link_libraries(${PROJECT_NAME} gif_animation -static)
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/lib)
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/giflib)