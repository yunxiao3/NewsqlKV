
```asm
sudo ./build_support/packages.sh

mkdir build
cd build
cmake ..
make -j
```


Instead of using printf statements for debugging, use the LOG_* macros for logging information like this:


```asm
LOG_INFO("# Pages: %d", num_pages);
LOG_DEBUG("Fetching page %d", page_id);
```

To enable logging in your project, you will need to reconfigure it like this:

```asm
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=DEBUG ..
make
```

Your code must follow the Google C++ Style Guide. We use Clang to automatically check the quality of your source code. 
Your project grade will be zero if your submission fails any of these checks.

Execute the following commands to check your syntax. The format target will automatically correct your code.
The check-lint and check-clang-tidy targets will print errors and instruct you how to fix it to conform to our style guide.
```asm
make format
make check-lint
make check-clang-tidy
```
