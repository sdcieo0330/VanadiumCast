# VanadiumCast

## Requirements

This application uses
- Qt5: https://qt.io
- QtAV plugin by wang-bin: https://github.com/wang-bin/QtAV

### Install build-tools
#### Arch Linux
```
pacman -S cmake base-devel
```

#### Debian
```
apt install cmake build-essentials
```

### Install Qt5 and QtAV
#### Arch Linux
```
pacman -S qt5-base qt5-quickcontrols2 qtav
```

#### Debian
```
apt install qt5-default
```

To build and install QtAV follow the steps on https://github.com/wang-bin/QtAV/wiki/Build-QtAV.

Please build QtAV with cmake instead of qmake. Because the cmake build is using ``ninja``, you have to ensure it is installed:
```
apt install ninja
```

## Building VanadiumCast
```
git clone https://github.com/sdcieo0330/VanadiumCast.git
cd VanadiumCast
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
cmake --build . --target VanadiumCast -j4
```

The executable is now in the subdirectory ``src`` of the build directory
