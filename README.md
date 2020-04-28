# MyROOTData: A simple example of custom data storage

Motivated by a Slack discussion on storing custom C++ `struct`s in
ROOT. This provides a minimum example of how to bind, write, and
read a custom data type to ROOT Trees.

## Requirements
- CMake 3.8 or newer
- ROOT 6.18 or newer
- C++ compiler

In SuperNEMO, these can be provided by the [SuperNEMO brew](https://github.com/SuperNEMO-DBD/brew] `brew snemo-shell` environment.

Alternately, if you are on a CentOS7 system with CVMFS available, then the
LCG 97 release can be used. Just set this up with

```
$ . /cvmfs/sft.cern.ch/lcg/views/LCG_97python3/x86_64-centos7-gcc9-opt/setup.sh
```

Whilst the docs below and code provide an outline, they should be used
together with the core ROOT documentation on https://root.cern.ch. There's
also an active, searchable, and helpful [forum](https://root-forum.cern.ch).

## Building the Example
```
$ mkdir build && cd build
$ cmake ..
...
$ cmake --build .
```

Assuming you have CMake, ROOT and a compiler correctly set up, the above
should run without error and create the following binaries/files in the
current directory:

- `MyDataRW` an application to read/write our custom data
- `libMyData.{dylib.so}` the library implementing data model and ROOT binding
- `MyData_dict.cxx` code generated by ROOT to allow our data to be read/written
- `libMyData.rootmap` `libMyData_rdict.pcm` ROOT-generated support files
  that help us to use of data in ROOT automatically

## Basic use of the Example
Data can be written via:

```
$ ./MyDataRW write test.root
```

and read directly by

```
$ ./MyDataRW read test.root
```

or interactively in ROOT's interpreter:

```
$ root
...

root [0] TFile f("test.root")
root [1] f.ls()
... any other operations ...
```

## How it works
1. The data we want to be able to write/read in ROOT is the struct `MyData` in `MyData.h`
2. To allow ROOT to read/write custom C++ types into TTrees, it requires
   a "dictionary" for the class.
3. ROOT generates this automatically for us via:
   - A "LinkDef" file. This is the `MyData_linkdef.h` file. It just needs
     to `#include` headers for the classes we want dictionaries to
     be generated for, plus `#pragma` directives for those classes.
     Here, we have one simple class, so a simple `link` suffices.
     ROOT's [User Guide](https://root.cern.ch/root/htmldoc/guides/users-guide/ROOTUsersGuide.html) has a [dedicated section on LinkDerf directives](https://root.cern.ch/root/htmldoc/guides/users-guide/ROOTUsersGuide.html#adding-a-class-with-a-shared-library).
   - Running the `rootcling` program over the linkdef and class headers to
     output a file of C++ code which comprises the dictionary. ROOT
     helpfully provide a CMake function to automate this for us, and
     that's shown in the `CMakeLists.txt` file here.
4. We compile the class headers and dictionary into a library. Even in
   this simple case we create a separate library as this allows us to
   read/write data in our own programs _and_ in interactive ROOT.
5. We use and link this library into the `MyDataRW` executable. We can
   then store `MyData` objects into `TTree` branches just as we would
   for any other ROOT or type with dictionary available.

The code of `MyDataRW` should cover the basics of writing and reading
data, and are equally valid in interactive ROOT or ROOT scripts.


