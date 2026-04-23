#
#  CMakeLists.txt
#
#  Copyright 2016 Dale Whinham
#
#  This file is part of MilkyTracker.
#
#  MilkyTracker is free software: you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation, either version 3 of the License, or
#  (at your option) any later version.
#
#  MilkyTracker is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with MilkyTracker.  If not, see <http://www.gnu.org/licenses/>.
#

cmake_minimum_required(VERSION 2.6)
project(MilkyTracker)

# Lowercase project name for binaries and packaging
string(TOLOWER ${PROJECT_NAME} PROJECT_NAME_LOWER)

# Additional CMake modules
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR}/cmake)

# Version number in format X.YY.ZZ
set(VER_X  0)
set(VER_YY 90)
set(VER_ZZ 87)
set(VER_FULL "${VER_X}.${VER_YY}.${VER_ZZ}")

if(POLICY CMP0014)
  cmake_policy(SET CMP0014 NEW)
endif()


# Generate version header from the above
configure_file(
    ${PROJECT_SOURCE_DIR}/src/tracker/version.h.in
    ${PROJECT_BINARY_DIR}/src/tracker/version.h
)

#find_package(SDL REQUIRED)


add_subdirectory(src/compression)
add_subdirectory(src/fx)
add_subdirectory(src/milkyplay)
add_subdirectory(src/ppui)
add_subdirectory(src/tracker)
