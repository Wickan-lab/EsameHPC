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
