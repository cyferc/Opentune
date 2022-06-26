To generate Release project in the build directory:
  cmake -DCMAKE_BUILD_TYPE=Release ..

To generate Debug project in the build directory:
  cmake -DCMAKE_BUILD_TYPE=Debug ..

Generate specific project, see https://cmake.org/cmake/help/v3.11/manual/cmake.1.html:
  cmake -G "MinGW Makefiles" ..
  cmake -G "Visual Studio 8 2005 Win64" ..
  cmake -G "Visual Studio 8 2005 Win32" ..

List all available generators:
  cmake -G

Make and run from the builds directory

To clean this project you simple delete all files in /builds

Build:
  cmake --build build [options]
  Options:
    -v      verbose
    -j N    parallel build, N processors.

  make

Run:
  ./bin/Opentune