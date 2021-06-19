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
	DESCRIPTION "K Nearest Niggas implementation in OpenMP, MPI, CUDA"
	VERSION 0.1
	LANGUAGES C
	)

# Generate compile_commands.json for semantic completion in YCM
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

#DEBUG flag
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DDEBUG")

enable_testing()

add_executable(program_seq src/main_omp.c src/knn.c include/knn.h)
add_executable(program_omp_selection src/main_omp.c src/knn.c include/knn.h)
add_executable(program_omp_merge src/main_omp.c src/knn.c include/knn.h)
add_executable(program_omp_quick src/main_omp.c src/knn.c include/knn.h)
add_executable(program_omp_bitonic src/main_omp.c src/knn.c include/knn.h)
add_executable(program_omp_bubble src/main_omp.c src/knn.c include/knn.h)
add_executable(program_mpi src/main_mpi.c src/knn_mpi.c src/knn.c include/knn.h)

target_include_directories(program_seq PRIVATE include)
target_include_directories(program_omp_selection PRIVATE include)
target_include_directories(program_omp_merge	PRIVATE include)
target_include_directories(program_omp_quick PRIVATE include)
target_include_directories(program_omp_bitonic PRIVATE include)
target_include_directories(program_omp_bubble PRIVATE include)
target_include_directories(program_mpi PRIVATE include)

find_package(OpenMP REQUIRED)
if(OpenMP_C_FOUND)
		  target_link_libraries(program_omp_selection PRIVATE OpenMP::OpenMP_C)
		  target_link_libraries(program_omp_merge PRIVATE OpenMP::OpenMP_C)
		  target_link_libraries(program_omp_quick PRIVATE OpenMP::OpenMP_C)
		  target_link_libraries(program_omp_bitonic PRIVATE OpenMP::OpenMP_C)
		  target_link_libraries(program_omp_bubble PRIVATE OpenMP::OpenMP_C)
endif()

find_package(MPI REQUIRED)
if(MPI_C_FOUND)
	message(STATUS "Run: ${MPIEXEC} ${MPIEXEC_NUMPROC_FLAG} ${MPIEXEC_MAX_NUMPROCS} ${MPIEXEC_PREFLAGS} EXECUTABLE ${MPIEXEC_POSTFLAGS} ARGS")
	target_link_libraries(program_mpi PUBLIC MPI::MPI_C)
endif()

target_compile_options(program_omp_selection PRIVATE -O3)
target_compile_options(program_omp_merge PRIVATE -O3)
target_compile_options(program_omp_quick PRIVATE -O3)
target_compile_options(program_omp_bitonic PRIVATE -O3)
target_compile_options(program_omp_bubble PRIVATE -O3)
target_compile_options(program_mpi PRIVATE -O3)

target_compile_features(program_seq PRIVATE c_std_11)
target_compile_features(program_omp_selection PRIVATE c_std_11)
target_compile_features(program_omp_merge PRIVATE c_std_11)
target_compile_features(program_omp_quick PRIVATE c_std_11)
target_compile_features(program_omp_bitonic PRIVATE c_std_11)
target_compile_features(program_omp_bubble PRIVATE c_std_11)
target_compile_features(program_mpi PRIVATE c_std_11)

target_compile_definitions(program_seq PRIVATE -DFACADE=facade_quicksort)
target_compile_definitions(program_omp_selection PRIVATE -DFACADE=facade_k_selection_sort)
target_compile_definitions(program_omp_merge PRIVATE -DFACADE=facade_mergeSort)
target_compile_definitions(program_omp_quick PRIVATE -DFACADE=facade_quicksort)
target_compile_definitions(program_omp_bitonic PRIVATE -DFACADE=facade_bitonicSort)
target_compile_definitions(program_omp_bubble PRIVATE -DFACADE=facade_bubble_sort)

target_compile_definitions(program_mpi PRIVATE -D_MPI)

# ---------------------------------------- TEST -----------------------------------------
#add_executable(dotprod_test test/test.c src/dotprod.c)
#target_include_directories(dotprod_test PRIVATE include)
#target_compile_definitions(dotprod_test PRIVATE -DELEMENT_TYPE=${ELEMENT_TYPE})
#
#add_test(NAME dotprod_test
#	COMMAND dotprod_test )
#
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

