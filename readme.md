# The X-Plane .obj exporter for the 3DsMax.
- The plug-in is distributed under [BSD (3-Clause)](http://opensource.org/licenses/BSD-3-Clause) license.
  For more information read the [license](license.txt) file.
- The X-Plane [official website](http://www.x-plane.com/).
- The X-Plane [obj specification](http://developer.x-plane.com/?article=obj8-file-format-specification).
- The plug-in supports `9-2019` 3DsMaxs (x64 only).
- The plug-in requires C++ 17 or higher standard.

### Warning 
Importing is still under developing and doesn't work properly!

## Dependencies
- [CMake [3.10.0+]](https://cmake.org/) building tool.
- [Conan [1.6+]](https://www.conan.io) package manager.  
- [XplnObj](https://github.com/steptosky/XplnObj) is used via conan.
- [sts-signals](https://github.com/steptosky/sts-signals) is used via conan.
- [sts-semver](https://github.com/steptosky/sts-semver) is used via conan.
- [jsonformoderncpp](https://github.com/nlohmann/json) is used via conan.
- 3Ds Max SDK. Usually you can find it on 3Ds Max installation CD or image.
- **Since 3Ds Max 2018 the plug-in uses Qt for building some GUI elements.**   
  The QT isn't used via conan so you have to install it manually.  
    - `3Ds Max 2018` - requires [QT [5.6.2]](https://www1.qt.io/offline-installers/?hsLang=en)
    - `3Ds Max 2019` - requires [QT [5.6.3]](https://www1.qt.io/offline-installers/?hsLang=en)
      (which is binary compatible with [QT 5.6.2])

## Memo for the library developers
- [release-checklist](doc/release-checklist.md) see this file when you are making a release.
- [change log](doc/changelog.txt) this file has to be filled during the release process and contains information about changes.

## Developing and Building
- You have to install `conan` and [add](https://docs.conan.io/en/latest/reference/commands/misc/remote.html):  
    - If it isn't available [conan-center](https://bintray.com/conan/conan-center) remote the link can be `https://conan.bintray.com`.
    - [bincrafters bintray](https://bintray.com/bincrafters/public-conan) remote.
    - [steptosky bintray](https://bintray.com/steptosky/conan-open-source) remote.
- You have to get the 3DsMax SDK and then put it into your conan repository or your conan local cache.  
  See [Conan 3dsmax SDK recipes](https://github.com/steptosky/conan-3dsmax-sdk-recipes).
- Adjust the file [conanfile.txt](conanfile.txt). You may comment unnecessary 3DsMax SDK version with the `#` symbol. It also turns off corresponding build targets.
``` bash
...
#3DsMaxSdk2009/last@steptosky/stable   isn't used
3DsMaxSdk2010/last@steptosky/stable    is used
...
```
- If you are going to compile the plug-in for **3Ds Max version greater than 2017** you have to install certain versions of QT.
  See `dependencies` section above.  
  Then you have to specify environment variables with path to QT something like this `C:\Qt\Qt5.6.3\5.6.3\msvc2017_64`:
    - `3DSMAX_2018_QT`=path to certain QT version where `bin` and `include` folders are located.
    - `3DSMAX_2019_QT`=path to certain QT version where `bin` and `include` folders are located.
- If you are going to develop you may generate the `Visual Studio 2017` project with the file `msvc-2017.bat` in the root repository folder.  
  You may copy that file and make necessary changes as you wish using the origin script as the example.  
  You may copy `conanfile.txt` and name it as `dev-conanfile.txt` then this file will be used during project generation. 
    It is useful when you are developing only for certain 3Ds Max versions but you want all the versions to be built with your CI.  
    **Don't forget to update data** in the `dev-conanfile.txt` when you pull new changes from the repository 
    or you changed commit or branch because dependencies' versions could also be changed.
- If you just want to build the plugins you may use the scripts inside the `ci` folder.  
  Pay attention those scripts must be run from the root repository folder `ci/build-vs2017.bat`.
    - `ci/build-release-vs2017.bat` - Builds and installs release target only. This script is usually used for making public release versions.
    - `ci/build-vs2017.bat` - Builds and installs release and debug targets. This script is usually used in CI jobs for checking your work.
- If you are going to upload built plugins to github:
  - You have to build and install [github-release](https://github.com/aktau/github-release). And make it accessible via your `PATH` environment variable.
  - Adjust `REPOSITORY_NAME` and `REPOSITORY_USER_NAME` variables in the file [StsUploadToGithub.cmake](cmake/StsUploadToGithub.cmake) if necessary.
  - Set the environment variable `GITHUB_TOKEN` - your access token from github account.
  - Run `ci/upload-to-github.bat` the release targets of the project must be built and installed for all supported 3DsMax versions.  
    Release tag must not exist in github repository otherwise the script stops with the error that the tag already exists.

#### Cmake variables
| Variables | Type | Description |
|----------:|:----:|:------------|
| **ADD_3DMAXS_EXEC** | _ON/OFF_  | If enabled then the default 3DsMaxs' path to `3dsmax.exe` will be added as the command for debugging. It is adjusted to be used with the `Visual Studio 2017`, it may not work with other ones. |

## Copyright
Copyright (c) 2018, StepToSky team. All rights reserved.  
[www.steptosky.com](http://www.steptosky.com/)