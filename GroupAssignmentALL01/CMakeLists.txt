#
# Course: High Performance Computing 2020/2021
# 
# Lecturer: Francesco Moscato	fmoscato@unisa.it
#
# Group:
# Capitani	Giuseppe	0622701085	g.capitani@studenti.unisa.it               
# Falanga	Armando	0622701140  a.falanga13@studenti.unisa.it 
# Terrone	Luigi		0622701071  l.terrone2@studenti.unisa.it
#
# This file is part of EsameHPC.
#
# CommonAssignment1 is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# CommonAssignment1 is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with CommonAssignment1.  If not, see <http://www.gnu.org/licenses/>.
#

cmake_minimum_required(VERSION 3.9)
project(GroupAssignmentALL01
	DESCRIPTION "Matrix dot Vector implementation with OpenMP"
	VERSION 0.1
	LANGUAGES C
	)

enable_testing()

add_executable(program_seq src/main_omp.c src/knn.c include/knn.h)
add_executable(program_omp src/main_omp.c src/knn.c include/knn.h)

target_include_directories(program_seq PRIVATE include)
target_include_directories(program_omp PRIVATE include)
target_include_directories(program_omp PRIVATE /usr/lib/gcc/x86_64-linux-gnu/4.8/include/)

find_package(OpenMP REQUIRED)
if(OpenMP_C_FOUND)
		  target_link_libraries(program_omp PRIVATE OpenMP::OpenMP_C)
endif()

target_compile_options(program_omp PRIVATE -O3)

target_compile_features(program_seq PRIVATE c_std_11)
target_compile_features(program_omp PRIVATE c_std_11)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# ---------------------------------------- TEST -----------------------------------------
#add_executable(dotprod_test test/test.c src/dotprod.c)
#target_include_directories(dotprod_test PRIVATE include)
#target_compile_definitions(dotprod_test PRIVATE -DELEMENT_TYPE=${ELEMENT_TYPE})
#
#add_test(NAME dotprod_test
#	COMMAND dotprod_test )
#
# ------------------------------------ DOCUMENTATION ------------------------------------
find_package(Doxygen
             REQUIRED dot)

if(DOXYGEN_FOUND)
	set(DOXYGEN_GENERATE_XML YES)
	doxygen_add_docs(
		docs
		./src/dotprod.c ./include/dotprod.h ./test/test.c 
		)
endif()

# ---------------------------------- GENERATE MEASURES ----------------------------------
#add_custom_target(
#	 generate_measures
#	 COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/measures/measures.bash ${CMAKE_CURRENT_BINARY_DIR}
#    COMMENT "Generating measures [${CMAKE_CURRENT_SOURCE_DIR}/measures/measures.bash ${CMAKE_CURRENT_BINARY_DIR}]"
#    DEPENDS	
#		program_omp
#		program_O2
#		program_O3
#		program_seq_O0
#		program_seq_omp
#		program_seq_O2
#		program_seq_O3
#)
#
#add_custom_target(
#	 change_ref
#	 COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/measures/measures.bash -p
#	 COMMENT "Change graph ref [${CMAKE_CURRENT_SOURCE_DIR}/measures/measures.bash -p]"
#	)
#
## --------------------------------- MEASURES EXTRACTION ---------------------------------
#find_package(Python3 COMPONENTS Interpreter REQUIRED)
#find_program(PipEnv pipenv REQUIRED)
#
#add_custom_target(
#	 install_pipenv
#	 COMMAND PIPENV_PIPFILE=${CMAKE_CURRENT_SOURCE_DIR}/Pipfile ${PipEnv} install --ignore-pipfile
#    COMMENT "Installing pipenv dependencies [${PipEnv} install]"
#    DEPENDS	${CMAKE_CURRENT_SOURCE_DIR}/Pipfile.lock
#)
#
#add_custom_target(
#    extract_measures
#    COMMAND PIPENV_PIPFILE=${CMAKE_CURRENT_SOURCE_DIR}/Pipfile ${PipEnv} run python ${CMAKE_CURRENT_SOURCE_DIR}/measures/extract.py
#    COMMENT "Generating version header [${PipEnv} run python ${CMAKE_CURRENT_SOURCE_DIR}/measures/extract.py]"
#    DEPENDS	
#		install_pipenv
#		${CMAKE_CURRENT_SOURCE_DIR}/measures/extract.py
#)
#
