
# The X-Plane .obj exporter for the 3DsMax.
- The plugin is distributed under [BSD (3-Clause)](http://opensource.org/licenses/BSD-3-Clause) license.
  For more information read the [license](license.txt) file.
- The X-Plane [official website](http://www.x-plane.com/).
- The X-Plane [obj specification](http://developer.x-plane.com/?article=obj8-file-format-specification).
- The plugin supports `9-2017` 3DsMaxs (x64 only).
- The plugin requires C++ 14 or higher standard.

### Warning 
Importing is still under developing and doesn't work properly!

## Dependencies
- [CMake 3.7.0+](https://cmake.org/) building tool.
- [Conan 1.6+](https://www.conan.io) package manager.
- [StepToSky X-Plane obj library](https://github.com/steptosky/XplnObj) used through the conan.
- 3Ds Max SDK. Usually you can find it on 3Ds Max installation CD or image.

## Memo for the library developers
- [release-checklist](doc/release-checklist.md) see this file when you are making the release.
- [change log](doc/changelog.txt) this file has to be filled during the release process and contains information about changes.

## Developing and Building
- You have to install `conan` and [add](https://docs.conan.io/en/latest/reference/commands/misc/remote.html) 
  our [bintray remote](https://bintray.com/steptosky/conan-open-source).
- You have to get the 3DsMax SDK and then put it into your conan repository or your conan local cache.  
  See [Conan 3dsmax SDK recipes](https://github.com/steptosky/conan-3dsmax-sdk-recipes).
- Adjust the file [conanfile.txt](conanfile.txt). You may comment unnecessary 3DsMax SDK version with the `#` symbol. It also turns off corresponding build targets.
``` bash
...
#3DsMaxSdk2009/last@steptosky/stable   isn't used
3DsMaxSdk2010/last@steptosky/stable    is used
...
```
- If you are going to develop you may generate the `Visual Studio 2017` project with the file `msvc-2017.bat` in the root repository folder.  
  You also may copy that file and make necessary changes as you wish using the origin script as the example.  
- If you just want to build the plugins you may use the scripts inside the `ci` folder.  
  Pay attention those scripts must be run from the root repository folder `ci/build-vs2017.bat`.
    - `ci/build-release-vs2017.bat` - Builds and installs release target only. This script is usually used for making public release versions.
    - `ci/build-vs2017.bat` - Builds and installs release and debug targets. This script is usually used in CI jobs for checking your work.
- If you want to upload built plugins to github:
  - You have to build and install [github-release](https://github.com/aktau/github-release). And make it accessible via your `PATH` environment variable.
  - Adjust `REPOSITORY_NAME` and `REPOSITORY_USER_NAME` variables in the file [StsUploadToGithub.cmake](cmake/StsUploadToGithub.cmake) if necessary.
  - Set the environment variable `GITHUB_TOKEN` - your access token from github account.
  - Run `ci/upload-to-github.bat` the release targets of the project must be built and installed for all supported 3DsMax versions.

## Copyright
Copyright (c) 2017, StepToSky team. All rights reserved.  
[www.steptosky.com](http://www.steptosky.com/)