# Visual LLDB
A GUI frontend for LLDB. This code has been hacked together in a few days and isn't in a usable state. These are just some of the things I'm considering:
* Expose more LLDB functionality
* Support core dumps
* Make it pretty
* Keyboard driven interface
## Dependencies
* LibLLDB
* Qt6
## Build
```
$ mkdir build/ && cd build/
$ cmake -G Ninja ..
$ ninja
```
## Usage
```
$ visual-lldb <target>
```
