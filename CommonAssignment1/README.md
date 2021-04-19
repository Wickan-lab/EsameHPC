# CommonAssignment1

## Dependencies

* CMake 3.9+
* OpenMP
* Python3
* Pipenv

## How to run

1. Create a build directory and launch cmake

   ```batch
   mkdir build
   cd build
   cmake ..
   ```

2. Generate executables with `make`
3. To generate measures (TAKE A LOT OF TIME! Our measures are already included so you should skip this step) run `make generate_measures`
4. To extract mean times and speedup curves from them run `make extract_measures`

Results can be found in the `measures/measure` directory, divided by problem size and the gcc optimization option used.

By default speedup curves are generated taking the relative `program_seq_Ox` as reference, if you want `program_seq_O0` as reference for all speedup plots launch `make change_ref` before generating them with `make extract_measures`.
