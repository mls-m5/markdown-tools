
cmake_minimum_required(VERSION 3.22)

project(markdown_tools)


add_executable(
    markdown_download_images
    src/main.cpp
    )

target_compile_features(
    markdown_download_images
    PUBLIC
    cxx_std_17
    )
