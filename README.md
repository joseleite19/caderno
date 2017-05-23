# Initialiing
The tests are made using [googletest](https://github.com/google/googletest/). It may be necessary to run
`git submodule update --init --recursive` once after you clone the repository for the first time, if the
folder `test/gtest/` is empty.

# Adding code
Add the code on the `code` folder.

# Adding tests
Add the test on the `test` folder, the file name should be `dir/file.testname.cpp` if you want to test `dir/file.cpp`.
Follow this format closely.

# Testing
Run `make` on the main folder to test all files. Individual executables are created with the same name as the test files.
