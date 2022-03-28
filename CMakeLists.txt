

add_executable(
    markdown_download_images
    src/main.cpp
    )

target_compile_features(
    markdown_download_images
    PUBLIC
    cxx_std_17
    )
