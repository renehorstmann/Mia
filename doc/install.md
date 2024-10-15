# Install Mia
Back to [README](../README.md)

Mia can be compiled and run on multiple platforms!

Mia is cmake based and can also be setup for IDE's to debug the code (Except for the Android app version)

Have a look at the following options to install all needed libraries to compile and run Mia.

## Options
- Desktop
    - Ubuntu
      - [Install](#install-for-ubuntu-or-wsl-ubuntu)
      - [Run](#run-hello-world)
    - Windows 11 WSL Ubuntu
      - [Install](#install-for-windows-11-wsl-ubuntu)
      - [Run](#run-hello-world)
    - Windows Msys2
      - [Install](#install-windows-msys2)
      - [Run](#run-hello-world)
    - Windows MSVC
      - [Install](#install-windows-msvc)
      - [Run for MSVC](#run-hello-world-with-msvc)
- [Compiled in the Android App CxxDroid](#install-compile-and-run-directly-on-android)
- [Android App](#android-app-with-androidstudio)
- [WebApp](#compiling-for-web)

### Install for Ubuntu or WSL Ubuntu
- Install the needed libraries in the shell:
```sh
#   update the system
sudo apt update && sudo apt upgrade
#   install basic stuff for c coding
sudo apt install build-essential gdb git cmake 
#   install sdl stuff 
# 	  net, ttf are optional, see MIA_OPTION_SOCKET and MIA_OPTION_TTF
sudo apt install libsdl2-dev libsdl2-image-dev libsdl2-net-dev libsdl2-ttf-dev
#   install curl (optional) for MIA_OPTION_FETCH 
sudo apt install libcurl4-openssl-dev
#   install zenity for sfd file dialogs / picker
sudo apt install zenity
```

- [now run the hello world demo](#run-hello-world)


### Install for Windows 11 WSL Ubuntu
With WSL (Windows Subsystem Linux) you can use all the awesome unix tools in Windows with an Ubuntu terminal.
But you can not distribute your compiled programs (to Windows), the users would have to have WSL installed.
See Windows MSYS2 or MSVC below, if you want to create an .exe to distribute for Windows

- run Powershell as admin
```
wsl --install
```
- see [Ubuntu](#install-for-ubuntu-or-wsl-ubuntu) with the Windows Ubuntu shell


### Install Windows MSYS2
MSYS2 is a package manager for Windows, that can install the compiler Mingw-w64,
which is a port of the GCC-Compiler with wrapper POSIX headers for Windows.

- install [MSYS2](https://www.msys2.org/)
- update the package manager (as the site says: `pacman -Syu` ; restart ; `pacman -Su`)
- In "MSYS2 MSYS" Terminal
```sh
#   basic stuff for c coding
pacman -S --needed base-devel mingw-w64-x86_64-toolchain
pacman -S git mingw-w64-x86_64-cmake mingw-w64-x86_64-ninja
#   install sdl stuff
# 	  net, ttf are optional, see MIA_OPTION_SOCKET and MIA_OPTION_TTF
pacman -S mingw-w64-x86_64-glew mingw-w64-x86_64-SDL2 mingw-w64-x86_64-SDL2_image mingw-w64-x86_64-SDL2_net mingw-w64-x86_64-SDL2_ttf
#   install curl (optional) for MIA_OPTION_FETCH 
pacman -S mingw-w64-x86_64-curl
```
- add the following line at the end of the file .bashrc (runs on terminal startup) (run `nano ~/.bashrc`)
```sh
# add mingw32 lib to the path, in this dir should be the opengl libraries (dll)
export PATH=$PATH:/c/msys64/mingw64/x86_64-w64-mingw32/lib
```
- Close the terminal
- If you use an IDE, you may need to add the PATH to ...mingw32/lib (see above) into the windows system variables

- [now run the hello world demo](#run-hello-world)


### Run Hello World
In Ubuntu, WSL Ubuntu or "MSYS2 MinGW x64" Terminal:
```sh
#   clone the repo
git clone git@github.com:renehorstmann/Mia.git
#   mv and make a build dir
cd Mia && mkdir build && cd build
#   run cmake debug
cmake .. -DCMAKE_BUILD_TYPE=Debug
#   run cmake release
#cmake .. -DCMAKE_BUILD_TYPE=Release
#
#   compile with 16 threads
cmake --build . -- -j16
#   compile single threaded
#cmake --build .
#
#   run
./mia
```

### Install Windows MSVC
MSVC is the Microsoft C(++) Compiler, that comes with Visual Studio.
The compiler can also be installed without the IDE (without needing the Visual Studio License...).

- install MSVC [Buildtools](https://visualstudio.microsoft.com/de/downloads/)
    - scroll down to "Tools for Visual Studio"
    - or install Visual Studio IDE directly if you want to use it
    - you need at least MSVC Version 2019 16.8
    - in the installer, check C++ development and the MSVC built tools + Windows SDK
- install a [git client](https://git-scm.com/download/win)
- install [cmake](https://cmake.org/download/) (add to system PATH)
- install [vcpkg](https://vcpkg.io/en/getting-started.html)
    - To install vcpk in PowerShell:
```sh
cd C:\
mkdir dev
cd dev
git clone https://github.com/Microsoft/vcpkg.git
.\vcpkg\bootstrap-vcpkg.bat -disableMetrics
cd vcpkg
```
- Install packages in Powershell
```sh
cd C:\dev\vcpkg
#   install sdl stuff
# 	  net, ttf are optional, see MIA_OPTION_SOCKET and MIS_OPTION_TTF
.\vcpkg install glew:x64-windows sdl2:x64-windows sdl2-image:x64-windows sdl2-net:x64-windows sdl2-ttf:x64-windows
#   install curl (optional) for MIA_OPTION_FETCH 
.\vcpkg install curl:x64-windows
```

- [now run the hello world demo with MSVC](#run-hello-world-with-msvc)

### Run Hello World with MSVC
In the Powershell Terminal:
```sh
#   clone the repo
git clone git@github.com:renehorstmann/Mia.git
#   mv and make a build dir
cd mia 
mkdir build
cd build
#
#   run cmake debug
#   Note: you may have to edit the toolchain path for vcpkg
cmake -DCMAKE_TOOLCHAIN_FILE=C:/dev/vcpkg/scripts/buildsystems/vcpkg.cmake .. -DCMAKE_BUILD_TYPE=Debug
#   run cmake release
#cmake -DCMAKE_TOOLCHAIN_FILE=C:/dev/vcpkg/scripts/buildsystems/vcpkg.cmake .. -DCMAKE_BUILD_TYPE=Release
#
#   compile
cmake --build .
#   run
.\Debug\mia.exe
```
> An IDE may also need the cmake option for the toolchain file



### Install, compile and run directly on Android
Useful to test and code on the go.

In fact, most of Mias's code, my games and apps were developed that way!

I recommend using a good Touchscreen-Keyboard-App with the right settings.
In my case its the Samsung default keyboard (Samsung Galaxy Note 8, btw.).
My recommended settings for the Samsung Keyboard:
- Smart typing / Keyboard swipe controls: **Cursor controls**
    - so its easy to navigate to the code with swipes instead of tipping on the line and position you want to editor
- Keyboard layout and feedback / Keyboard layout: **Number keys** checked; **Alternative characters** checked
    - so you can insert glyphs with a long press on the key, instead of switching to the alt. chars. site

I used the full/paid version of [CxxDroid](https://play.google.com/store/apps/details?id=ru.iiec.cxxdroid) (from Google Play), other c compilers with SDL support may work, too

- Install F-Droid: https://f-droid.org/
- In F-Droid, install Termux (https://termux.com/)
- In Termux, clone the project
```sh
#   update the system
pkg upgrade
#   install basic stuff
pkg install git
#   get access to the local file system of your android device from termux (needs the storage permission)
termux-setup-storage
# c  d to your file system
cd ~/storage/shared
#   optional, create a c projects folder
mkdir cprojects && cd cprojects
#   clone the repo
git clone git@github.com:renehorstmann/Mia.git
```
- In CxxDroid, install the following libraries (Menu/Install Libraries)
    - SDL2
    - SDL2-image
    - SDL2-net (optional with MIA_OPTION_SOCKET)
    - SDL2-ttf (optional with MIA_OPTION_TTF)
    - libcurl (optional with MIA_OPTION_FETCH)
- Open a file from Mia in CxxDroid
- You should now be able to compile and run directly on Android



### Android App with AndroidStudio
Install [Android Studio](https://developer.android.com/studio) and the latest sdk tools.
The install path must not contain a whitespace or other special symbols.

Create the Android project with the template generator:
```sh
cd android
# Use your own package name like:
python3 template.py de horsimann tea
```
The project will be generated in the out dir.
> The generated project path must not include a whitespace or other special symbols!

Open the project in Android Studio and have fun.



### Compiling for Web
Mia compiles fine as website with wasm and webgl, that can also be used on mobile browsers.

Using the [Emscripten compiler](https://emscripten.org/docs/getting_started/downloads.html)

Tested under Ubuntu and WSL Ubuntu (may work on Windows too, if the compile calls are changed).

You should have already cloned the project and `cd` to that dir:

```sh
#   create the build directory
mkdir build_web && cd build_web
#
#   run cmake debug
#   Note: you may have to edit the toolchain path for emsdk / emscripten
cmake -DCMAKE_TOOLCHAIN_FILE=$EMSDK/upstream/emscripten/cmake/Modules/Platform/Emscripten.cmake .. -DCMAKE_BUILD_TYPE=Debug
#   run cmake release
#cmake -DCMAKE_TOOLCHAIN_FILE=$EMSDK/upstream/emscripten/cmake/Modules/Platform/Emscripten.cmake .. -DCMAKE_BUILD_TYPE=Release
#
#   compile
cmake --build . -- -j16
#   compile single threaded
#cmake --build .  
#
#   run (debug version) 
#		 may requires an "sudo apt install firefox"
emrun index.html --browser=firefox
```

- Test the website (open a browser and call localhost:8000)
```sh
python3 -m http.server --bind localhost  # [port]
```

- Clear the makefiles
```sh
# clear all but the webapp
rm -r !(debug.html|favicon.ico|icon*.png|index*)
```


Back to [README](../README.md)