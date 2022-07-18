## Build

### Linux Only

To ensure that you have the proper packages on your machine, run the following script to automatically install them:

```
$ sudo build_support/packages.sh
```

Then run the following commands to build the system:

```
$ mkdir build
$ cd build
$ cmake ..
$ make
```

If you want to compile the system in debug mode, pass in the following flag to cmake:
Debug mode:

```
$ cmake -DCMAKE_BUILD_TYPE=Debug ..
$ make
```
This enables [AddressSanitizer](https://github.com/google/sanitizers), which can generate false positives for overflow on STL containers. If you encounter this, define the environment variable `ASAN_OPTIONS=detect_container_overflow=0`.


Instead of using printf statements for debugging, use the LOG_* macros for logging information like this:
```
LOG_INFO("# Pages: %d", num_pages);
LOG_DEBUG("Fetching page %d", page_id);
```

NewsqlKV's code follows the Google C++ Style Guide. 
We use Clang to automatically check the quality of your source code.

Execute the following commands to check your syntax. 
The format target will automatically correct your code. 
The check-lint and check-clang-tidy targets will print errors and instruct you how to fix it to conform to Google C++ style guide.

```
make format
make check-lint
make check-clang-tidy
```



## Testing

build and run all test
```
$ cd build
$ make check-tests
```
buidl and run single test
```
$ make smaple_test
$ ./test/smaple_test
```

## Docker

First, make sure that you have docker installed:


From the repository directory, run these commands to create a Docker image and container:

```
$ docker build . -t newsql_kv
$ docker create -t -i --name newsql_kv -v $(pwd):/newsql_kv newsql_kv bash
```

This will create a Docker image and container. To run it, type:

```
$ docker start -a -i newsql_kv
```

to open a shell within the box. You can find Newsql_KV's code mounted at `/newsql_kv` and run the commands mentioned above like normal.