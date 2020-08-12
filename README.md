# Coil Engine
Base on the Game Engine series by the Cherno ([YouTube](https://www.youtube.com/channel/UCQ-W1KE9EYfdxhL6S4twUNw)) and his Hazel Engine ([GitHub](https://github.com/TheCherno/Hazel)) under [Apache License 2.0](https://github.com/TheCherno/Hazel/blob/master/LICENSE).

### Goal
The purpose of the Coil Engine is to be a Breakable Toy, allowing me to learn about game engines and improve my programming capability.
Most of the code closely follows the Game Engine series for future compatibility purposes (especially rendering). However, to further increase learning potential, I try to personalize the source code and implement my own solutions.
I have implemented my own: String libraries, Logging, GUI interface, low overhead Profiling (improved version of the one in the series), Time library, and Doxygen comments.

### Requirements
Currently, Coil Engine is Windows only and uses C++17.

### Setup
Coil Engine uses submodules for third party code and is it advised to clone it recursively
```sh
git clone --recursive https://github.com/ikold/Coil [...Destination]
```

To generated Project for Visual Studio 2019, run `./Scripts/GenrateProject_vs2019.bat`.
If you want to generate Project for different environment ([Premake Environments](https://github.com/premake/premake-core/wiki/Using-Premake)) call:
```sh
./vendor/binary/premake/premake5.exe [Environment]
```

### Third-Party Acknowledgment

| Library | License |
| ------ | ------ |
| Premake5 | [BSD-3-Clause](https://github.com/premake/premake-core/blob/master/LICENSE.txt) |
| glad | [MIT](https://github.com/Dav1dde/glad/blob/master/LICENSE) |
| GLFW | [Zlib](https://github.com/glfw/glfw/blob/master/LICENSE.md) |
| ImGui | [MIT](https://github.com/ocornut/imgui/blob/master/LICENSE.txt) |
| glm | [The Happy Bunny (Modified MIT)](https://github.com/g-truc/glm/blob/master/copying.txt) |
| Ryu | [Apache 2.0](https://github.com/ulfjack/ryu/blob/master/LICENSE-Apache2) |
| stb | [MIT/Public Domain](https://github.com/nothings/stb/blob/master/LICENSE) |