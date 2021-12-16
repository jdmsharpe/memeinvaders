cmake_minimum_required(VERSION 3.10)

# set the project name
project(memeinvaders VERSION 0.01)

set(CMAKE_CXX_STANDARD 17)

include(FindPkgConfig)

PKG_SEARCH_MODULE(SDL2 REQUIRED sdl2)
PKG_SEARCH_MODULE(SDL2IMAGE REQUIRED SDL2_image>=2.0.0)

include_directories(${PROJECT_SOURCE_DIR}/inc ${SDL2_INCLUDE_DIRS} ${SDL2IMAGE_INCLUDE_DIRS})

file(GLOB SRC
     "src/*.cpp"
)

# add the executable
add_executable(MemeInvaders ${SRC})

target_link_libraries(MemeInvaders ${SDL2_LIBRARIES} ${SDL2IMAGE_LIBRARIES} ${OPENGL_LIBRARIES} ${GLUT_LIBRARY})