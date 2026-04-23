set(PREFIX $ENV{PREFIX})
set(HOST $ENV{HOST})

set(TARGET_GLIBC "2.13")
set(TOOLCHAIN_PREFIX "$arm-unknown-linux-gnueabi")
set(TOOLCHAIN_PATH "$ENV{PREFIX}")

set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_C_COMPILER   ${CC})
set(CMAKE_CXX_COMPILER ${CXX})

set(CMAKE_FIND_ROOT_PATH  ${PREFIX} ${PREFIX}/arm-unknown-linux-gnueabi)
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

if(ERRATA_1)
	add_compile_options(-std=c99)
endif()

if(ERRATA_2)
	#add_link_options(-lstdc++ -static-libstdc++ -static-libgcc -ftls-model=initial-exec)
	#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-threadsafe-statics -ftls-model=global-dynamic")
	#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_GLIBCXX_USE_CXX11_ABI=0")
	#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fvisibility=hidden")
	#set(CMAKE_EXE_LINKER_FLAGS "-static-libstdc++ -static-libgcc")
endif()

if(ERRATA_3)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
	set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,-z,relro")
endif()

add_definitions(-include ${PREFIX}/include/prolin_printf.h)

if(SDL_1)
	set(SDL_INCLUDE_DIR ${PREFIX}/include/SDL)
	set(SDL_INCLUDE_DIRS ${PREFIX}/include/SDL)
	link_directories(${PREFIX}/lib)
	set(SDL_LIBRARIES SDL)
endif()
