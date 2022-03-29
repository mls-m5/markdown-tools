
cmake_minimum_required(VERSION 3.22)

project(markdown_tools)

add_executable(
    markdown_download_images
    src/download_images.cpp
    )

target_compile_features(
    markdown_download_images
    PUBLIC
    cxx_std_17
    )

add_executable(
    strip_extra_blanklines
    src/strip_extra_blanklines.cpp
    )

target_compile_features(
    strip_extra_blanklines
    PUBLIC
    cxx_std_17
    )
