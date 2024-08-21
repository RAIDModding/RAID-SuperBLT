# RAID-SuperBLT
An open-source Lua hook for RAID: World War II, designed and created for ease of use for both players and modders.

This is an unofficial continuation of the BLT and SuperBLT mod loader for PAYDAY 2, with additional features aimed at allowing things
not possible in standard Lua, such as patching XML files that are loaded directly by the engine or playing
3D sounds.

This is the developer repository and should only be used if you know what you're doing (or running GNU+Linux, as explained below). If you don't, visit the website at [SuperBLT.znix.xyz](https://superblt.znix.xyz/) for an up-to-date drag-drop install.
The Lua component of the BLT, which controls mod loading, can be found in its own repository, [GitHub: RAIDModding/RAID-SuperBLT-Lua]([https://gitlab.com/znixian/payday2-superblt-lua](https://github.com/RAIDModding/RAID-SuperBLT-Lua)).

## Download
Visit [The SuperBLT Site](https://superblt.znix.xyz/) to get the latest stable download for Windows. 
Dribbleondo maintains a Linux download that has compiled loaders for Debian-based distros and Arch-based distros, 
which [can be downloaded from here](https://modworkshop.net/mod/36557). If your distro or Linux OS cannot run the precompiled loader, 
see below for building GNU+Linux binaries for yourself.

If you are on Linux, It is **strongly** advised that you **DO NOT** use the Flatpak version of Steam when running Payday 2 with SuperBLT enabled, as the Flatpak version of Steam imports various library dependencies that improve compatibility between OS, while breaking the SuperBLT loader in the process. **You have been warned.**

## Documentation
Documentation for the BLT can be found on the [GitHub Wiki](https://github.com/JamesWilko/Payday-2-BLT/wiki) for the project.

Documentation for SuperBLT can be found on the [SuperBLT Website](https://superblt.znix.xyz).

## Development

How to contribute to SuperBLT:

SuperBLT uses the CMake build system. This can generate various build files,
including GNU Makefiles and MSVC Project/Solution files, from which the program can
then be built.

SuperBLT depends on two sets of libraries: those commonly available (such as OpenSSL
and ZLib) which you are expected to install via your package manager (GNU+Linux) or
by downloading and compiling (Windows).

It also has libraries that are very small projects, rarely available in package managers,
and that are included as git submodules. CMake automatically adds these targets,
so you don't need to make any special efforts to use them.

### Windows

First, clone this repository and pull all required projects and repositories into one folder (Note: You **NEED** to do this, otherwise you'll get runtime and compile errors):

```
git clone --recursive https://github.com/RAIDModding/RAID-SuperBLT.git
```

Next open it in your IDE of choice. In CLion, select `File -> Open` and the folder created
by git clone. For Visual Studio, select `File -> Open -> CMake` and select the top-level
`CMakeLists.txt` file.

Next, set your IDE to build a 32-bit binary. In CLion, first ensure you have an x86 toolchain
configured in `File | Settings | Build, Execution, Deployment | Toolchains` - add a new
Visual Studio toolchain and set the architecture to `x86`. Ensure this toolchain is selected
in `File | Settings | Build, Execution, Deployment | CMake` in your `Debug` configuration and
inside the toolchain box. Once CMake is done, select `WSOCK32` in the target box.

To enable multithreaded builds in CLion (which massively improves build times), you need to
download [JOM](https://wiki.qt.io/Jom). In the toolchain, you added earlier, set the `Make`
field to the path of the `jom.exe` file you extracted earlier.

In Visual Studio, select the configuration box (at the top of the window, which may, for
example, say `x64-Debug`) and select `Manage Configurations...` if it isn't set to `x86-Debug` already.
Remove the existing configuration then add a new one, and select `x86-Debug`.
Select `Project->Generate Cache` and wait for it to run cmake - this may take some time.

At this point, you can compile your project. In CLion, Click the little green hammer in the top-right
of CLion (the shortcut varies depending on the platform). In Visual Studio, press F7. In either case this
will take some time as it compiles all of SuperBLT's dependencies, and finally, SuperBLT itself.

Finally, you can make PAYDAY use your custom-built version of SBLT instead of having to copy the built
file to the PAYDAY directory each time you change something.
To do this, go to your `PAYDAY 2` directory and open PowerShell. Run:

```
cmd /c mklink WSOCK32.dll <path to SBLT>\out\build\x86-Debug\WSOCK32.dll
```

for Visual Studio, or:

```
cmd /c mklink WSOCK32.dll <path to SBLT>\cmake-build-debug\WSOCK32.dll
```

for CLion.

### Code Conventions
- Avoid `std::shared_ptr` and the likes unless you have a decent reason to use it. If you
need the reference counting, go ahead, but please don't use it when a regular pointer works fine.
- Don't **ever** use CRLF.
- Please ensure there is a linefeed (`\n`) as the last byte of any files you create.
- Please use `git patch.` Don't commit multiple unrelated or loosely related things in a
single commit. Likewise, please don't commit whitespace-only changes. `git blame` is a valuable
tool.
- Please run the source code using `clang-format` to ensure stuff like brace positions and whitespace
are consistent. Later, this will be put into a Continuous Integration task to mark offending
commits and test stuff like compiling in GCC.
- Please ensure your code doesn't cause any compiler warnings (not counting libraries). This is
enforced for GCC; please watch your output if you're using Visual Studio.
