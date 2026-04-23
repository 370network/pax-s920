cmake_minimum_required(VERSION 3.11)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_C_FLAGS "-O3 -flto")
set(CMAKE_CXX_FLAGS "-O3 -flto")

project(paxdemo)

set(BUILD_SHARED_LIBS OFF)
include(FetchContent)
FetchContent_Declare(
    libpax
    GIT_REPOSITORY https://github.com/paxdevs/libpax.git
    GIT_TAG main
    OVERRIDE_FIND_PACKAGE
)
FetchContent_MakeAvailable(libpax)

set(OPUS_FIXED_POINT ON)
FetchContent_Declare(
    opus
    GIT_REPOSITORY https://github.com/xiph/opus.git
    GIT_TAG ddbe48383984d56acd9e1ab6a090c54ca6b735a6
    OVERRIDE_FIND_PACKAGE
)
FetchContent_MakeAvailable(opus)

FetchContent_Declare(
    ogg
    GIT_REPOSITORY https://github.com/xiph/ogg.git
    GIT_TAG db5c7a49ce7ebda47b15b78471e78fb7f2483e22
    OVERRIDE_FIND_PACKAGE
)
FetchContent_MakeAvailable(ogg)

set(OP_DISABLE_HTTP ON)
set(OP_DISABLE_DOCS ON)
set(OP_FIXED_POINT ON)
set(OP_DISABLE_EXAMPLES ON)
FetchContent_Declare(
    opusfile
    GIT_REPOSITORY https://github.com/xiph/opusfile.git
    GIT_TAG master
    OVERRIDE_FIND_PACKAGE
)
FetchContent_MakeAvailable(opusfile)

# Create the shared library target
add_library(paxdemo SHARED main.cpp audio.cpp)

# Set the optimization level
target_compile_options(paxdemo PRIVATE -O3 -Wall -Wextra -flto)

# Set the link options
target_link_libraries(paxdemo
    PUBLIC
        -Wl,--whole-archive libpax -Wl,--no-whole-archive
        -Wl,--whole-archive opusfile -Wl,--no-whole-archive
        -Wl,--whole-archive opus -Wl,--no-whole-archive
        -Wl,--whole-archive ogg -Wl,--no-whole-archive
)
target_link_options(paxdemo PRIVATE -Wl,--no-as-needed -static-libstdc++ -static-libgcc -flto)


# Set the include directories for the libpax library
target_include_directories(paxdemo PRIVATE ${libpax_SOURCE_DIR}/include)
