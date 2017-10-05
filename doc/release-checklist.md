## Documentation
- Check whether this release-checklist is correct.
- Check whether the [install.txt](install.txt) is correct.
- Check whether the [changelog.txt](changelog.txt) is correct, copy necessary information from the XplnObj library.
- Check whether the variables of the build system (CMake) are described correctly if they were changed, deleted, added.
- Check whether the dependencies and their version are described correctly in the [readme.md](../readme.md) file. 

## Version
- Set the correct version in the [StsProjectDesc.cmake](../cmake/StsProjectDesc.cmake)

## Cmake
- Check whether the information in the [StsProjectDesc.cmake](../cmake/StsProjectDesc.cmake) file is correct.
  - Contributors description
  - Open source libraries description
  - Etc...

## Conan
- Check whether the [conanfile.txt](../conanfile.txt) uses the correct dependencies and their version.  
For the release must not be versions like *last*, *unstable*, *develop* etc...  
Exception is the 3dSMax Sdk, it can have versions *last*
