implementation of this in c
https://www.youtube.com/watch?v=GXh0Vxg7AnQ

# building
## linux
```
git clone https://github.com/imnethen/softbody-sim --recurse-submodules
cd softbody-sim
cmake -B build -S .
cmake --build build
```
to run:
```
build/softbody
```
it must stay in the build folder because the texture path is hardcoded :3

## windows
no idea
probably something similar
