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
# This file is part of CommonAssignment2.
#
# CommonAssignment2 is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# CommonAssignment2 is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with CommonAssignment2.  If not, see <http://www.gnu.org/licenses/>.
#

cmake_minimum_required(VERSION 3.9)
project(CommonAssignment2
	DESCRIPTION "Matrix dot Vector implementation with MPICH"
	VERSION 0.1
	LANGUAGES C
	)

# Generate comile_commands.json for semantic completion in YCM
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

#enable_testing()

add_executable(program src/main.c src/utils.c include/utils.h)

target_include_directories(program PRIVATE include)

find_package(MPI REQUIRED)
if(MPI_C_FOUND)
	message(STATUS "Run: ${MPIEXEC} ${MPIEXEC_NUMPROC_FLAG} ${MPIEXEC_MAX_NUMPROCS} ${MPIEXEC_PREFLAGS} EXECUTABLE ${MPIEXEC_POSTFLAGS} ARGS")
	target_link_libraries(program PUBLIC MPI::MPI_C)
endif()

target_compile_options(program PRIVATE -O0)

target_compile_features(program PRIVATE c_std_11)

set(VERSION 1 CACHE STRING "Read procedure used.")
target_compile_definitions(program PRIVATE -DVERSION=${VERSION})
# ---------------------------------------- TEST -----------------------------------------
#add_executable(dotprod_test test/test.c src/dotprod.c)
#target_include_directories(dotprod_test PRIVATE include)
#target_compile_definitions(dotprod_test PRIVATE -DELEMENT_TYPE=${ELEMENT_TYPE})
#
#add_test(NAME dotprod_test
#	COMMAND dotprod_test )

# ------------------------------------ DOCUMENTATION ------------------------------------
#find_package(Doxygen
#             REQUIRED dot)
#
#if(DOXYGEN_FOUND)
#	set(DOXYGEN_GENERATE_XML YES)
#	doxygen_add_docs(
#		docs
#		./src/dotprod.c ./include/dotprod.h ./test/test.c 
#		)
#endif()

# ---------------------------------- GENERATE MEASURES ----------------------------------
#add_custom_target(
#	 generate_measures
#	 COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/measures/measures.bash ${CMAKE_CURRENT_BINARY_DIR}
#    COMMENT "Generating measures [${CMAKE_CURRENT_SOURCE_DIR}/measures/measures.bash ${CMAKE_CURRENT_BINARY_DIR}]"
#    DEPENDS	
#		program_O1
#		program_O2
#		program_O3
#		program_seq_O0
#		program_seq_O1
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
# --------------------------------- MEASURES EXTRACTION ---------------------------------
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
