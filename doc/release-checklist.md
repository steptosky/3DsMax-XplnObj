## Documentation
- Check whether the [install.txt](install.txt) is correct.
- Check whether the [changelog.txt](changelog.txt) is correct, copy necessary information from the XplnObj library.
- Check whether the dependencies and their version are described correctly in the [readme.md](../readme.md) file. 
- Check whether all the necessary plugins are adjusted to be uploaded in the file [StsUploadToGithub.cmake](../cmake/StsUploadToGithub.cmake). 

## Version
- Set the correct version in the [StsProjectDesc.cmake](../cmake/StsProjectDesc.cmake)

## Cmake
- Check whether the information in the [StsProjectDesc.cmake](../cmake/StsProjectDesc.cmake) file is correct.
  - Contributors description
  - Open source libraries description
  - Etc...

## Conan
- Check whether the [conanfile.txt](../conanfile.txt) uses the correct dependencies and their versions.  
  For the release must not be versions like `last`, `unstable`, `develop` etc...  
  Exception is the 3dSMax SDK it is allowed to have versions `last`
