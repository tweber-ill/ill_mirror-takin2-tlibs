#
# tlibs
# @author Tobias Weber <tobias.weber@tum.de>
# @date 2012-2017
# @license GPLv2 or GPLv3
#
# ----------------------------------------------------------------------------
# tlibs -- a physical-mathematical C++ template library
# Copyright (C) 2017-2021  Tobias WEBER (Institut Laue-Langevin (ILL),
#                          Grenoble, France).
# Copyright (C) 2015-2017  Tobias WEBER (Technische Universitaet Muenchen
#                          (TUM), Garching, Germany).
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 2 of the License, or
# (at your option) version 3.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
# ----------------------------------------------------------------------------
#

project(tlibs)
cmake_minimum_required(VERSION 3.0)

#set(CMAKE_VERBOSE_MAKEFILE TRUE)
set(CMAKE_BUILD_TYPE Release)


list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}" ".")
set(CMAKE_BINARY_DIR "${CMAKE_SOURCE_DIR}")
#set(EXECUTABLE_OUTPUT_PATH "${CMAKE_BINARY_DIR}/bin")

find_package(Boost REQUIRED COMPONENTS iostreams system filesystem REQUIRED)
find_package(Threads REQUIRED)
find_package(Freetype REQUIRED)
find_package(Minuit2 REQUIRED)
find_package(OpenGL REQUIRED)


set(CMAKE_AUTOMOC OFF)
set(CMAKE_AUTOUIC OFF)


#message("Project source dir: ${PROJECT_SOURCE_DIR}")
#message("CMake source dir: ${CMAKE_SOURCE_DIR}")
#message("Binary dir: ${CMAKE_BINARY_DIR}")
message("Install path prefix: ${CMAKE_INSTALL_PREFIX}")
message("OpenGL include dir: \"${OPENGL_INCLUDE_DIR}\".")


add_definitions(-DNDEBUG)
add_definitions(-DUSE_IOSTR)
add_definitions(-DNO_LAPACK)
#add_definitions(-DUSE_FFTW)
add_definitions(-DINSTALL_PREFIX="${CMAKE_INSTALL_PREFIX}")
add_definitions(${Boost_CXX_FLAGS})


include_directories("${PROJECT_SOURCE_DIR}" ".")
include_directories("${Boost_INCLUDE_DIRS}/..")
include_directories("${FREETYPE_INCLUDE_DIRS}")
include_directories("${Minuit2_INCLUDE_DIRS}")
include_directories("${OPENGL_INCLUDE_DIR}" 
	"${OPENGL_INCLUDE_DIR}/GL" 
	"${OPENGL_INCLUDE_DIR}/OpenGL"
	"${OPENGL_INCLUDE_DIR}/Headers")


add_definitions(-std=c++11)
#add_definitions(-march=native -s)



# -----------------------------------------------------------------------------
# build
# -----------------------------------------------------------------------------
add_library(tlibs SHARED
	file/tmp.cpp  file/loadinstr.cpp  file/x3d.cpp
	log/log.cpp log/debug.cpp
	helper/flags.cpp
	math/linalg2.cpp  math/rand.cpp
	net/tcp.cpp
	string/spec_char.cpp  string/eval.cpp
	gfx/gl.cpp  gfx/gnuplot.cpp
	version.cpp
)


target_link_libraries(tlibs ${Boost_LIBRARIES} Threads::Threads
	${OPENGL_LIBRARIES} ${FREETYPE_LIBRARY} ${Minuit2_LIBRARIES}
)


#if(CMAKE_BUILD_TYPE STREQUAL "Release")
#	add_custom_command(TARGET tlibs POST_BUILD
#		COMMAND strip -v $<TARGET_FILE:tlibs>
#		MAIN_DEPENDENCY tlibs
#	)
#endif()
# -----------------------------------------------------------------------------




# -----------------------------------------------------------------------------
# install
# -----------------------------------------------------------------------------
install(TARGETS tlibs  DESTINATION lib)

file(GLOB tlibs_rt  ${PROJECT_SOURCE_DIR}/*.h)
file(GLOB tlibs_file  ${PROJECT_SOURCE_DIR}/file/*.h)
file(GLOB tlibs_helper  ${PROJECT_SOURCE_DIR}/helper/*.h)
file(GLOB tlibs_math  ${PROJECT_SOURCE_DIR}/math/*.h)
file(GLOB tlibs_phys  ${PROJECT_SOURCE_DIR}/phys/*.h)
file(GLOB tlibs_net  ${PROJECT_SOURCE_DIR}/net/*.h)
file(GLOB tlibs_string  ${PROJECT_SOURCE_DIR}/string/*.h)
file(GLOB tlibs_gfx  ${PROJECT_SOURCE_DIR}/gfx/*.h)
file(GLOB tlibs_time  ${PROJECT_SOURCE_DIR}/time/*.h)
file(GLOB tlibs_cl  ${PROJECT_SOURCE_DIR}/cl/*.h)
file(GLOB tlibs_log  ${PROJECT_SOURCE_DIR}/log/*.h)
file(GLOB tlibs_fit  ${PROJECT_SOURCE_DIR}/fit/*.h)
file(GLOB tlibs_ext  ${PROJECT_SOURCE_DIR}/ext/*.h)

install(FILES ${tlibs_rt}  DESTINATION include/tlibs)
install(FILES ${tlibs_file}  DESTINATION include/tlibs/file)
install(FILES ${tlibs_helper}  DESTINATION include/tlibs/helper)
install(FILES ${tlibs_math}  DESTINATION include/tlibs/math)
install(FILES ${tlibs_phys}  DESTINATION include/tlibs/phys)
install(FILES ${tlibs_net}  DESTINATION include/tlibs/net)
install(FILES ${tlibs_string}  DESTINATION include/tlibs/string)
install(FILES ${tlibs_gfx}  DESTINATION include/tlibs/gfx)
install(FILES ${tlibs_time}  DESTINATION include/tlibs/time)
install(FILES ${tlibs_cl}  DESTINATION include/tlibs/cl)
install(FILES ${tlibs_log}  DESTINATION include/tlibs/log)
install(FILES ${tlibs_fit}  DESTINATION include/tlibs/fit)
install(FILES ${tlibs_ext}  DESTINATION include/tlibs/ext)

install(FILES ${PROJECT_SOURCE_DIR}/FindMinuit2.cmake DESTINATION share/tlibs)

install(FILES ${PROJECT_SOURCE_DIR}/COPYING.GPLv2 ${PROJECT_SOURCE_DIR}/COPYING.GPLv3
	${PROJECT_SOURCE_DIR}/AUTHORS ${PROJECT_SOURCE_DIR}/LICENSES
        DESTINATION share/tlibs
)
# -----------------------------------------------------------------------------
