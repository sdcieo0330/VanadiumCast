# VanadiumCast
VanadiumCast is a simple and opensource video streaming tool

## Install
### Windows
#### Pre-built binaries
Download the installer from the latest release: https://github.com/sdcieo0330/VanadiumCast/releases. Please build the release version.

#### Build from sources
Follow the instructions to build QtAV on https://github.com/wang-bin/QtAV/wiki/Build-QtAV
You have to have the Visual Studio Compiler 2019, the Windows SDK 10 and the Universal C Runtime installed.

After that, clone the VanadiumCast-Repository:
```
git clone https://github.com/sdcieo0330/VanadiumCast
```
Then you have to build the application. Please use the PowerShell for the following commands, because the CMD doesn't supports every command.
Jom is an multithreaded nmake, that is shipped with Qt Creator. You will probably find it under ``<QtDir>\Tools\QtCreator\bin\jom.exe``

```
mkdir build
cd build
Path\To\qmake.exe ..\VanadiumCast.pro
Path\To\jom.exe -j4
cp backend\backend.dll VanadiumCast\
```

### Linux
#### Flatpak
_//Todo_

#### Build from sources
##### QtAV is available in as a package
Simply install the QtAV package.

IMPORTANT: If you are using a debian- or ubuntu-based distribution, install the *-dev and the QtAV-Widgets packages too, because the library is split into multiple packages.

##### QtAV is not available as a package
Follow the instructions to build QtAV on https://github.com/wang-bin/QtAV/wiki/Build-QtAV. Please build the release version.

After that, clone the VanadiumCast-Repository
```
git clone https://github.com/sdcieo0330/VanadiumCast
```
Then you are able to build the application:

```
mkdir build
cd build
qmake ../VanadiumCast.pro
make -j4
cp backend/libbackend.so.1.0.0 VanadiumCast/
cd VanadiumCast
ln -s libbackend.so.1.0.0 libbackend.so.1.0
ln -s libbackend.so.1.0.0 libbackend.so.1
ln -s libbackend.so.1.0.0 libbackend.so
```

_//Todo: Create an (un-)install target for make to install/remove VanadiumCast in/from the system_
