#
# Copyright (C) 2021 - All Rights Reserved 
#
# This file is part of EsameHPC.
#
# EsameHPC is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# EsameHPC is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with EsameHPC.  If not, see <http://www.gnu.org/licenses/>.
#

#[[ 
	Define the version of cmake necessary for executing this 
 ]]
cmake_minimum_required(VERSION 3.10)
project(CommonAssignment1
	DESCRIPTION "First common assignment of the HPC course"
	VERSION 0.1
	LANGUAGES C
	)

enable_testing()

add_executable(program src/main_no_macro.c src/dotprod.c )
add_executable(program_seq src/main_no_macro.c src/dotprod.c )

target_include_directories(program PRIVATE include)
target_include_directories(program_seq PRIVATE include)

find_package(OpenMP)
if(OpenMP_C_FOUND)
	target_link_libraries(program PUBLIC OpenMP::OpenMP_C)
	target_compile_options(program PUBLIC -O2)
endif()

target_compile_features(program PRIVATE c_std_11)
target_compile_features(program_seq PRIVATE c_std_11)

add_executable(dotprod_test test/test.c src/dotprod.c)
target_include_directories(dotprod_test PRIVATE include)

add_test(NAME dotprod_test
	COMMAND dotprod_test )
