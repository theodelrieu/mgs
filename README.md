MGS
===

`mgs` is a C++14 codec library.

Its main design goals are:

* Ease of use: Regular usage must be a no-brainer.
* Extensibility: New codecs should be easy to add in future versions.
* API genericity: Emulating concepts to define generic APIs.
* Package manager friendliness: Modular architecture, each codec is a single entity.

`mgs` defines a common interface for all supported codecs, that is both generic and customizable:

```cpp
   #include <mgs/base64.hpp>

   #include <array>
   #include <forward_list>
   #include <string>
   #include <vector>

   using namespace mgs;

   int main() {
     std::string const a = base64::encode("Hello, World!");
     std::vector<unsigned char> const b = base64::decode(a);

     // Default return types can be overriden
     auto const c = base64::encode<std::forward_list<char>>(b);
     auto const d = base64::decode<std::array<char, 13>>(c);

     // Iterator ranges are supported
     auto const e = base64::encode(d.begin(), d.end());
     auto const f = base64::decode(e.begin(), e.end());
   }
```

You can find the full documentation [here](https://theodelrieu.github.io/mgs-docs/).

How to build
------------

__Requirements__:

* Python3
* Conan (latest version)
* CMake (>= 3.3)

Then run:

```shell
$ mkdir build && cd build
$ conan workspace install ..
$ cmake -G Ninja .. && ninja
``` 
