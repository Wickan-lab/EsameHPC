#
# Course: High Performance Computing 2020/2021
# 
# Lecturer: Francesco Moscato	fmoscato@unisa.it
#
# Group:
# Capitani	Giuseppe	0622701085	g.capitani@studenti.unisa.it               
# Falanga	Armando		0622701140  a.falanga13@studenti.unisa.it 
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
	DESCRIPTION "K-Nearest Neighbours implementation in OpenMP, MPI & CUDA"
	VERSION 0.1
	LANGUAGES C
	)

# Generate compile_commands.json for semantic completion in YCM
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

#DEBUG flag
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DDEBUG")

enable_testing()

add_executable(program_seq src/main_omp.c 
						src/knn_omp.c src/point.c src/sort.c 
						include/knn_omp.h include/point.h include/sort.h)

add_executable(program_omp_selection src/main_omp.c 
						src/knn_omp.c src/point.c src/sort.c 
						include/knn_omp.h include/point.h include/sort.h)

add_executable(program_omp_bitonic src/main_omp.c 
						src/knn_omp.c src/point.c src/sort.c 
						include/knn_omp.h include/point.h include/sort.h)

add_executable(program_omp_bubble src/main_omp.c
						src/knn_omp.c src/point.c src/sort.c 
						include/knn_omp.h include/point.h include/sort.h)

add_executable(program_mpi src/main_mpi.c  
						src/knn_mpi.c src/point.c src/sort.c 
						include/knn_mpi.h include/point.h include/sort.h)

target_include_directories(program_seq				PRIVATE include)
target_include_directories(program_omp_selection	PRIVATE include)
target_include_directories(program_omp_bitonic		PRIVATE include)
target_include_directories(program_omp_bubble		PRIVATE include)
target_include_directories(program_mpi				PRIVATE include)

find_package(OpenMP REQUIRED)
if(OpenMP_C_FOUND)
		  target_link_libraries(program_omp_selection	PRIVATE OpenMP::OpenMP_C)
		  target_link_libraries(program_omp_bitonic		PRIVATE OpenMP::OpenMP_C)
		  target_link_libraries(program_omp_bubble		PRIVATE OpenMP::OpenMP_C)
endif()

find_package(MPI REQUIRED)
if(MPI_C_FOUND)
	message(STATUS "Run: ${MPIEXEC} ${MPIEXEC_NUMPROC_FLAG} ${MPIEXEC_MAX_NUMPROCS} ${MPIEXEC_PREFLAGS} EXECUTABLE ${MPIEXEC_POSTFLAGS} ARGS")
	
	target_link_libraries(program_mpi PUBLIC MPI::MPI_C)
	
	target_compile_definitions(program_mpi PRIVATE -D_MPI)
endif()

target_compile_options(program_omp_selection	PRIVATE -O3)
target_compile_options(program_omp_bitonic		PRIVATE -O3)
target_compile_options(program_omp_bubble		PRIVATE -O3)
target_compile_options(program_mpi				PRIVATE -O3)

target_compile_features(program_seq				PRIVATE c_std_11)
target_compile_features(program_omp_selection	PRIVATE c_std_11)
target_compile_features(program_omp_bitonic		PRIVATE c_std_11)
target_compile_features(program_omp_bubble		PRIVATE c_std_11)
target_compile_features(program_mpi				PRIVATE c_std_11)

target_compile_definitions(program_seq				PRIVATE	-DALG=BITONIC)
target_compile_definitions(program_omp_selection	PRIVATE	-DALG=SELECTION)
target_compile_definitions(program_omp_bitonic		PRIVATE	-DALG=BITONIC)
target_compile_definitions(program_omp_bubble		PRIVATE	-DALG=BUBBLE)


# ---------------------------------------- TEST -----------------------------------------
add_executable(sqrt_test test/compare_sqrt_algos.c src/point.c include/point.h)
add_executable(sort_test test/SortTest.c test/TestLib.c src/point.c src/knn_omp.c src/sort.c include/point.h include/sort.h include/knn_omp.h include/TestLib.h)

target_include_directories(sqrt_test PRIVATE include)
target_include_directories(sort_test PRIVATE include)

target_link_libraries(sqrt_test PRIVATE m)

add_test(NAME sqrt_test COMMAND sqrt_test 262144 0.18 )
add_test(NAME sort_test COMMAND sort_test 4 4 )

# ------------------------------------ DOCUMENTATION ------------------------------------
find_package(Doxygen
             REQUIRED dot)

if(DOXYGEN_FOUND)
	set(DOXYGEN_GENERATE_XML YES)
	doxygen_add_docs(
		docs
		./src/knn_mpi.c ./src/knn_omp.c ./src/point.c ./include/point.h ./test/TestLib.c
		)
endif()

# ---------------------------------- GENERATE MEASURES ----------------------------------
add_custom_target(
	generate_measures_omp
	COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/measures/omp/measures.bash ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Generating measures [${CMAKE_CURRENT_SOURCE_DIR}/measures/omp/measures.bash ${CMAKE_CURRENT_BINARY_DIR}]"
    DEPENDS	
		program_omp_bitonic
		program_seq
)

add_custom_target(
	generate_measures_mpi
	COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/measures/mpi/measures.bash ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Generating measures [${CMAKE_CURRENT_SOURCE_DIR}/measures/mpi/measures.bash ${CMAKE_CURRENT_BINARY_DIR}]"
    DEPENDS	
		program_omp_bitonic
		program_seq
)
# --------------------------------- MEASURES EXTRACTION ---------------------------------
find_package(Python3 COMPONENTS Interpreter REQUIRED)
find_program(PipEnv pipenv REQUIRED)

add_custom_target(
	install_pipenv
	COMMAND PIPENV_PIPFILE=${CMAKE_CURRENT_SOURCE_DIR}/Pipfile ${PipEnv} install --ignore-pipfile
    COMMENT "Installing pipenv dependencies [${PipEnv} install]"
    DEPENDS	${CMAKE_CURRENT_SOURCE_DIR}/Pipfile.lock
)

add_custom_target(
    extract_measures_omp
    COMMAND PIPENV_PIPFILE=${CMAKE_CURRENT_SOURCE_DIR}/Pipfile ${PipEnv} run python ${CMAKE_CURRENT_SOURCE_DIR}/measures/omp/extract.py
    COMMENT "Generating version header [${PipEnv} run python ${CMAKE_CURRENT_SOURCE_DIR}/measures/omp/extract.py]"
    DEPENDS	
		install_pipenv
		${CMAKE_CURRENT_SOURCE_DIR}/measures/omp/extract.py
)
add_custom_target(
    extract_measures_mpi
    COMMAND PIPENV_PIPFILE=${CMAKE_CURRENT_SOURCE_DIR}/Pipfile ${PipEnv} run python ${CMAKE_CURRENT_SOURCE_DIR}/measures/mpi/extract.py
    COMMENT "Generating version header [${PipEnv} run python ${CMAKE_CURRENT_SOURCE_DIR}/measures/mpi/extract.py]"
    DEPENDS	
		install_pipenv
		${CMAKE_CURRENT_SOURCE_DIR}/measures/mpi/extract.py
)

