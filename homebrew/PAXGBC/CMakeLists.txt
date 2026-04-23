cmake_minimum_required(VERSION 3.12)

set(TOOLCHAIN_PREFIX "arm-unknown-linux-gnueabi")
set(TOOLCHAIN_PATH "$ENV{PAXPATH}/toolchain")
set(BUILD_SHARED_LIBS OFF)
set(CMAKE_CXX_STANDARD 20)

project(paxgbc)

# Add the libpax library as a dependency
set(BUILD_SHARED_LIBS OFF)
include(FetchContent)
FetchContent_Declare(
    libpax
    GIT_REPOSITORY https://github.com/paxdevs/libpax.git
    GIT_TAG main
)
FetchContent_MakeAvailable(libpax)

# Add the fbg library as a dependency
FetchContent_Declare(
    fbg
    GIT_REPOSITORY https://github.com/rhgndf/fbg.git
    GIT_TAG paxs920
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
)
FetchContent_MakeAvailable(fbg)

# Create the shared library target
add_library(paxgbc SHARED
    main.cpp
    emu.c
    state.c
    cpu.c
    mmu.c
    disassembler.c
    lcd.c
    audio.c
    fileio.c
    debugger-dummy.c

    ${fbg_SOURCE_DIR}/src/fbgraphics.c
    ${fbg_SOURCE_DIR}/src/lodepng/lodepng.c
    ${fbg_SOURCE_DIR}/src/nanojpeg/nanojpeg.c
    ${fbg_SOURCE_DIR}/custom_backend/fbdev/fbg_fbdev.c
)

target_include_directories(paxgbc PRIVATE
    ${fbg_SOURCE_DIR}/src
    ${fbg_SOURCE_DIR}/custom_backend/fbdev
    )

# Set the optimization level
target_compile_options(paxgbc PRIVATE -O3 -Wall -Wextra -flto)

# Set the link options
target_link_options(paxgbc PRIVATE -Wl,--no-as-needed -static-libstdc++ -static-libgcc -flto)
target_link_options(paxgbc PRIVATE )
target_link_libraries(paxgbc PRIVATE libpax)

# Link against the libpax library
target_link_libraries(paxgbc PUBLIC libpax -Wl,--whole-archive libpax -Wl,--no-whole-archive)

# Set the include directories for the libpax library
target_include_directories(paxgbc PRIVATE ${libpax_SOURCE_DIR}/include)
