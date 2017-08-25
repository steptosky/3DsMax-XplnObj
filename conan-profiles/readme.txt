Conan profiles
https://www.conan.io/

When you develop your library on Windows usually you need to have multiconfiguration project 'Release' and 'Debug'
but conan has one default global configuration. 
The debug and the release modes have the different runtime flags Md/MDd MT/MTd and you can't specify 
more then one flag without changing the global conan configuration but you can use the conan profiles.
Of Course you can create and use your own profiles for different operating systems and compilers.
See the conan documentation for more information.