include(FetchContent)
set(BUILD_SHARED_LIBS OFF)
enable_testing()

cmake_minimum_required(VERSION 3.26)
project(sea_battle)

set(CMAKE_CXX_STANDARD 17)

add_library(player player.h player.cpp)
add_executable(sea_battle main.cpp)
target_link_libraries(sea_battle PRIVATE player)

add_executable(player_test test.cpp)
target_link_libraries(player_test PRIVATE player)
add_test(NAME player_test COMMAND player_test)

find_package(Doxygen)

set(DOXYGEN_GENERATE_HTML YES)
set(DOXYGEN_USE_MATHJAX YES)
set(DOXYGEN_EXTRACT_PRIVATE YES)
set(DOXYGEN_OUTPUT_LANGUAGE Russian)

doxygen_add_docs(
        doc ALL
        ${PROJECT_SOURCE_DIR}/player.h
        COMMENT "Generate HTML documentation"
)

FetchContent_Declare(
        sfml
        GIT_REPOSITORY "https://github.com/SFML/SFML"
        GIT_TAG 2.6.0
)
set(SFML_BUILD_AUDIO FALSE)
set(SFML_BUILD_NETWORK FALSE)
FetchContent_MakeAvailable(sfml)

target_link_libraries(sea_battle PUBLIC sfml-graphics)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/textures DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

set(CMAKE_INSTALL_DOCDIR ${CMAKE_INSTALL_DATAROOTDIR}/doc/${PROJECT_NAME})
install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/doc/html/
        DESTINATION ${CMAKE_INSTALL_DOCDIR})
install(TARGETS player
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)