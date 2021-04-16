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
cmake_minimum_required(VERSION 3.9)
project(CommonAssignment1
	DESCRIPTION "First common assignment of the HPC course"
	VERSION 0.1
	LANGUAGES C
	)

enable_testing()

add_executable(program_O0 src/main.c src/dotprod.c include/dotprod.h)
add_executable(program_O1 src/main.c src/dotprod.c include/dotprod.h)
add_executable(program_O2 src/main.c src/dotprod.c include/dotprod.h)
add_executable(program_O3 src/main.c src/dotprod.c include/dotprod.h)
add_executable(program_seq_O0 src/main.c src/dotprod.c include/dotprod.h)
add_executable(program_seq_O1 src/main.c src/dotprod.c include/dotprod.h)
add_executable(program_seq_O2 src/main.c src/dotprod.c include/dotprod.h)
add_executable(program_seq_O3 src/main.c src/dotprod.c include/dotprod.h)

target_include_directories(program_O0 PRIVATE include)
target_include_directories(program_O1 PRIVATE include)
target_include_directories(program_O2 PRIVATE include)
target_include_directories(program_O3 PRIVATE include)
target_include_directories(program_seq_O0 PRIVATE include)
target_include_directories(program_seq_O1 PRIVATE include)
target_include_directories(program_seq_O2 PRIVATE include)
target_include_directories(program_seq_O3 PRIVATE include)

find_package(OpenMP)
if(OpenMP_C_FOUND)
        target_link_libraries(program_O0 PRIVATE OpenMP::OpenMP_C)
        target_link_libraries(program_O1 PRIVATE OpenMP::OpenMP_C)
        target_link_libraries(program_O2 PRIVATE OpenMP::OpenMP_C)
        target_link_libraries(program_O3 PRIVATE OpenMP::OpenMP_C)
endif()

target_compile_options(program_O0 PRIVATE -O0)
target_compile_options(program_O1 PRIVATE -O1)
target_compile_options(program_O2 PRIVATE -O2)
target_compile_options(program_O3 PRIVATE -O3)
target_compile_options(program_seq_O0 PRIVATE -O0)
target_compile_options(program_seq_O1 PRIVATE -O1)
target_compile_options(program_seq_O2 PRIVATE -O2)
target_compile_options(program_seq_O3 PRIVATE -O3)

target_compile_features(program_O0 PRIVATE c_std_11)
target_compile_features(program_O1 PRIVATE c_std_11)
target_compile_features(program_O2 PRIVATE c_std_11)
target_compile_features(program_O3 PRIVATE c_std_11)
target_compile_features(program_seq_O0 PRIVATE c_std_11)
target_compile_features(program_seq_O1 PRIVATE c_std_11)
target_compile_features(program_seq_O2 PRIVATE c_std_11)
target_compile_features(program_seq_O3 PRIVATE c_std_11)

set(ELEMENT_TYPE double CACHE STRING "Type of the elements in matrices and arrays.")
target_compile_definitions(program_O0 PRIVATE -DELEMENT_TYPE=${ELEMENT_TYPE})
target_compile_definitions(program_O1 PRIVATE -DELEMENT_TYPE=${ELEMENT_TYPE})
target_compile_definitions(program_O2 PRIVATE -DELEMENT_TYPE=${ELEMENT_TYPE})
target_compile_definitions(program_O3 PRIVATE -DELEMENT_TYPE=${ELEMENT_TYPE})
target_compile_definitions(program_seq_O0 PRIVATE -DELEMENT_TYPE=${ELEMENT_TYPE})
target_compile_definitions(program_seq_O1 PRIVATE -DELEMENT_TYPE=${ELEMENT_TYPE})
target_compile_definitions(program_seq_O2 PRIVATE -DELEMENT_TYPE=${ELEMENT_TYPE})
target_compile_definitions(program_seq_O3 PRIVATE -DELEMENT_TYPE=${ELEMENT_TYPE})

add_executable(dotprod_test test/test.c src/dotprod.c)
target_include_directories(dotprod_test PRIVATE include)
target_compile_definitions(dotprod_test PRIVATE -DELEMENT_TYPE=${ELEMENT_TYPE})

add_test(NAME dotprod_test
	COMMAND dotprod_test )

# Search fo doxygen
find_package(Doxygen
             REQUIRED dot)

if(DOXYGEN_FOUND)
	set(DOXYGEN_GENERATE_XML YES)
	doxygen_add_docs(
		docs
		./src/dotprod.c ./include/dotprod.h ./test/test.c 
		)
endif()
