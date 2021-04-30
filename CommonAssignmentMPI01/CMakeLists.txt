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
# This file is part of CommonAssignmentMPI01.
#
# CommonAssignmentMPI01 is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# CommonAssignmentMPI01 is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with CommonAssignmentMPI01.  If not, see <http://www.gnu.org/licenses/>.
#

cmake_minimum_required(VERSION 3.9)
project(CommonAssignmentMPI01
	DESCRIPTION "Matrix dot Vector implementation with MPICH"
	VERSION 0.1
	LANGUAGES C
	)

# Generate comile_commands.json for semantic completion in YCM
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# DEBUG flag
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DDEBUG")

#enable_testing()

add_executable(program_O3_V1 src/main.c src/utils.c include/utils.h)
add_executable(program_O3_V2 src/main.c src/utils.c include/utils.h)
add_executable(program_O3_V3 src/main.c src/utils.c include/utils.h)
add_executable(program_O3_V4 src/main.c src/utils.c include/utils.h)
add_executable(program_O3_V5 src/main.c src/utils.c include/utils.h)
add_executable(program_O3_V6 src/main.c src/utils.c include/utils.h)
add_executable(program_seq_O3 src/main_seq.c src/utils.c include/utils.h)
add_executable(program_test_O3 src/main_test.c src/utils.c include/utils.h)
add_executable(initMatrices src/initMatrices.c src/utils.c include/utils.h)

target_include_directories(program_O3_V1 PRIVATE include)
target_include_directories(program_O3_V2 PRIVATE include)
target_include_directories(program_O3_V3 PRIVATE include)
target_include_directories(program_O3_V4 PRIVATE include)
target_include_directories(program_O3_V5 PRIVATE include)
target_include_directories(program_O3_V6 PRIVATE include)
target_include_directories(program_seq_O3 PRIVATE include)
target_include_directories(program_test_O3 PRIVATE include)
target_include_directories(initMatrices PRIVATE include)

find_package(MPI REQUIRED)
if(MPI_C_FOUND)
	message(STATUS "Run: ${MPIEXEC} ${MPIEXEC_NUMPROC_FLAG} ${MPIEXEC_MAX_NUMPROCS} ${MPIEXEC_PREFLAGS} EXECUTABLE ${MPIEXEC_POSTFLAGS} ARGS")
	target_link_libraries(program_O3_V1 PUBLIC MPI::MPI_C)
	target_link_libraries(program_O3_V2 PUBLIC MPI::MPI_C)
	target_link_libraries(program_O3_V3 PUBLIC MPI::MPI_C)
	target_link_libraries(program_O3_V4 PUBLIC MPI::MPI_C)
	target_link_libraries(program_O3_V5 PUBLIC MPI::MPI_C)
	target_link_libraries(program_O3_V6 PUBLIC MPI::MPI_C)
	target_link_libraries(program_test_O3 PUBLIC MPI::MPI_C)
endif()

target_compile_options(program_O3_V1 PRIVATE -O3)
target_compile_options(program_O3_V2 PRIVATE -O3)
target_compile_options(program_O3_V3 PRIVATE -O3)
target_compile_options(program_O3_V4 PRIVATE -O3)
target_compile_options(program_O3_V5 PRIVATE -O3)
target_compile_options(program_O3_V6 PRIVATE -O3)
target_compile_options(program_seq_O3 PRIVATE -O3)
target_compile_options(program_test_O3 PRIVATE -O3)
target_compile_options(initMatrices PRIVATE -O3)

target_compile_features(program_O3_V1 PRIVATE c_std_11)
target_compile_features(program_O3_V2 PRIVATE c_std_11)
target_compile_features(program_O3_V3 PRIVATE c_std_11)
target_compile_features(program_O3_V4 PRIVATE c_std_11)
target_compile_features(program_O3_V5 PRIVATE c_std_11)
target_compile_features(program_O3_V6 PRIVATE c_std_11)
target_compile_features(program_seq_O3 PRIVATE c_std_11)
target_compile_features(program_test_O3 PRIVATE c_std_11)
target_compile_features(initMatrices PRIVATE c_std_11)

target_compile_definitions(program_O3_V1 PRIVATE -DVERSION=1)
target_compile_definitions(program_O3_V2 PRIVATE -DVERSION=2)
target_compile_definitions(program_O3_V3 PRIVATE -DVERSION=3)
target_compile_definitions(program_O3_V4 PRIVATE -DVERSION=4)
target_compile_definitions(program_O3_V5 PRIVATE -DVERSION=5)
target_compile_definitions(program_O3_V6 PRIVATE -DVERSION=6)

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
add_custom_target(
	 generate_measures
	 COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/measures/measures.bash ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Generating measures [${CMAKE_CURRENT_SOURCE_DIR}/measures/measures.bash ${CMAKE_CURRENT_BINARY_DIR}]"
    DEPENDS	
		initMatrices
		program_O3_V1
		program_O3_V2
		program_O3_V3
		program_O3_V4
		program_O3_V5
		program_O3_V6
		program_seq_O3
)
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
