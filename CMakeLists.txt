cmake_minimum_required(VERSION 3.10)

# Can increment version later if desired
project(memeinvaders VERSION 0.01)
# C++20 because new tech!!!
set(CMAKE_CXX_STANDARD 20)
# Set to optimized debug, comment out to disable
# Unsure if there's a performance impact...
set(CMAKE_BUILD_TYPE RelWithDebInfo)

include(FindPkgConfig)
PKG_SEARCH_MODULE(SDL2 REQUIRED sdl2)
PKG_SEARCH_MODULE(SDL2IMAGE REQUIRED SDL2_image>=2.0.0)
PKG_SEARCH_MODULE(SDL2TTF REQUIRED SDL2_ttf>=2.0.0)

# Include all .h files and library headers
include_directories(
                    ${PROJECT_SOURCE_DIR}/inc 
                    ${SDL2_INCLUDE_DIRS} 
                    ${SDL2IMAGE_INCLUDE_DIRS} 
                    ${SDL2TTF_INCLUDE_DIRS}
                    )

# Include all .cpp files
file(GLOB SRC "src/*.cpp")

add_executable(MemeInvaders ${SRC})

target_link_libraries(MemeInvaders 
                      ${SDL2_LIBRARIES} 
                      ${SDL2IMAGE_LIBRARIES} 
                      ${SDL2TTF_LIBRARIES} 
                      ${OPENGL_LIBRARIES} 
                      ${GLUT_LIBRARY}
                      )
