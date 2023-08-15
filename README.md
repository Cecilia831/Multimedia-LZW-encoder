# JPEG Multimedia Project



## Assignment
Rather than copy and paste the less interesting parts, please refer to the programs description in `PA2.pdf`.

## VS Code
To use GoogleTests you'll need to have the proper directories compiled with the source files. From what I can tell, there is also a need for a file like `gtest_main.cc` because it drives the testing program. Moreover, the directory `include` should be in the project's workspace due to the dependencies within the file `gtest/gtest.h`. To integrate these tests with VS Code a `launch.json` file must be created. Go to the test section and you should be prompted with the Command Palette (`⇧ ⌘ P`) from there select the CPP launch config and name the project in the json field. I changed the `name` to `debug` and I still wonder if the `cwd` should be up a directory out of `.vscode`. Anyways, also hope for a file `c_cpp_properties.json` to be automatically created because idk how that was generated but it's needed. From there testing should be as easy as clicking the play button in the test section.

## Testing
Assuming you have the files necessary for the testing library mentioned in the section on VS Code, simply click the play button. Otherwise, tests could have been written by hand. I was quite interested in making this a full-fledged software project, hence the testing framework from Google.

Here's how you compile the tests (you may need to create the `build` directory):
```
cd build/
cmake ..
make all
```

When compiling the tests, we can also compile with debug mode on:
```
cd build/
rm -rf *
cmake -DCMAKE_BUILD_TYPE=Debug ..
make all
```
After you've compiled the code, you can run either the test executable or the program itself.
```
./build/tst/jpeg_multimedia_project_tst
./build/src/jpeg_multimedia_project_run
```

I played around with directories and paths and stuff with very little success. This is the command I used in the beginning to compile tests.
```
g++ -std=c++11 -I ../include/ -L ../ gtest_main.cc ../src/CompressedImageData/ImageData.cpp CompressedImageDataTests.cpp -lgtest -lpthread -o CompressedImageDataTests
```

As it turns out, this article led me down [a much better path.](https://raymii.org/s/tutorials/Cpp_project_setup_with_cmake_and_unit_tests.html#toc_10)

### VS Code stuff
So there is a step by step debugger in VS Code that is quite nice. Do note that it takes a bit of work to install. On macOS you'll need the plugin CodeLLDB 
so that you can see the contents of containers like Vectors. Configuring the debugger is done in the file `.vscode/launch.json`. That is also where you would
specify arguments to the debugger.