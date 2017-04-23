## Jenkins CI item example
- Generate enivironment variables from the project description
```
cd cmake
cmake -DCONTENT_PREFIX="Sts" -P StsProjectDescCiEnvGenerator.cmake 
```
- Use inject environment variables
```
cmake/variables.txt
```
```
GITHUB_TOKEN=token here
```
The *GITHUB_TOKEN* is needed if you want to auto-upload the artifacts to github
- Make build dir and export environment variables
```
export -p
# uncomment it if you want to remove the dir first
#if [[ -d "build" ]]; then
#    rm -r "build"
#fi
[ -d "build" ] || mkdir "build"
```
- Set conan user if it is needed
``` 
conan user username -r remote -p password 
```
- Build
```
cd build
conan install .. --build=outdated
cmake -G "Visual Studio 14 Win64" ../ -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=../output -DUSE_CONAN_CMAKE_MULTI=OFF 
cmake --build . --target install --config Release
```
- Use the [github-release](https://github.com/aktau/github-release)
    for uploading your artifacts to github
- Create the release tag
```
github-release release --user steptosky --repo 3DsMax-XplnObj \
    --tag ${StsProjectVersionTag} \
    --name "${StsProjectOrganizationName} ${StsProjectName}" \
    --description "${StsProjectDescription}"
```
- Upload the artifacts
```
github-release upload --user steptosky --repo 3DsMax-XplnObj --tag ${StsProjectVersionTag} \
    --name "3DsMax9-XplnObj-x64.dlu" --file "${WORKSPACE}/output/Release/3DsMax9-XplnObj-x64.dlu"
    
github-release upload --user steptosky --repo 3DsMax-XplnObj --tag ${StsProjectVersionTag} \
    --name "3DsMax2008-XplnObj-x64.dlu" --file "${WORKSPACE}/output/Release/3DsMax2008-XplnObj-x64.dlu"
    
github-release upload --user steptosky --repo 3DsMax-XplnObj --tag ${StsProjectVersionTag} \
    --name "3DsMax2009-XplnObj-x64.dlu" --file "${WORKSPACE}/output/Release/3DsMax2009-XplnObj-x64.dlu"
    
github-release upload --user steptosky --repo 3DsMax-XplnObj --tag ${StsProjectVersionTag} \
    --name "3DsMax2010-XplnObj-x64.dlu" --file "${WORKSPACE}/output/Release/3DsMax2010-XplnObj-x64.dlu"
    
github-release upload --user steptosky --repo 3DsMax-XplnObj --tag ${StsProjectVersionTag} \
    --name "3DsMax2011-XplnObj-x64.dlu" --file "${WORKSPACE}/output/Release/3DsMax2011-XplnObj-x64.dlu"
    
github-release upload --user steptosky --repo 3DsMax-XplnObj --tag ${StsProjectVersionTag} \
    --name "3DsMax2012-XplnObj-x64.dlu" --file "${WORKSPACE}/output/Release/3DsMax2012-XplnObj-x64.dlu"
    
github-release upload --user steptosky --repo 3DsMax-XplnObj --tag ${StsProjectVersionTag} \
    --name "3DsMax2013-XplnObj-x64.dlu" --file "${WORKSPACE}/output/Release/3DsMax2013-XplnObj-x64.dlu"
    
github-release upload --user steptosky --repo 3DsMax-XplnObj --tag ${StsProjectVersionTag} \
    --name "3DsMax2014-XplnObj-x64.dlu" --file "${WORKSPACE}/output/Release/3DsMax2014-XplnObj-x64.dlu"
    
github-release upload --user steptosky --repo 3DsMax-XplnObj --tag ${StsProjectVersionTag} \
    --name "3DsMax2015-XplnObj-x64.dlu" --file "${WORKSPACE}/output/Release/3DsMax2015-XplnObj-x64.dlu"
    
github-release upload --user steptosky --repo 3DsMax-XplnObj --tag ${StsProjectVersionTag} \
    --name "3DsMax2016-XplnObj-x64.dlu" --file "${WORKSPACE}/output/Release/3DsMax2016-XplnObj-x64.dlu"
    
github-release upload --user steptosky --repo 3DsMax-XplnObj --tag ${StsProjectVersionTag} \
    --name "3DsMax2017-XplnObj-x64.dlu" --file "${WORKSPACE}/output/Release/3DsMax2017-XplnObj-x64.dlu"
	
github-release upload --user steptosky --repo 3DsMax-XplnObj --tag ${StsProjectVersionTag} \
    --name "changelog.txt" --file "${WORKSPACE}/doc/changelog.txt"
	
github-release upload --user steptosky --repo 3DsMax-XplnObj --tag ${StsProjectVersionTag} \
    --name "install.txt" --file "${WORKSPACE}/doc/install.txt"
```