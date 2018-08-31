::==========================================================
:: Sometimes conan asks you about to login to private repositories
:: if you have added ones. So you can put into the root repository folder
:: the file called 'set-conan-user.bat' and write there something like this:
::      call conan user userName -r RemoteAlias -p Password
if exist set-conan-user.bat call set-conan-user.bat
::==========================================================
:: Creating building DIR and use it as working one.
set dir="build-msvc-2017-release"
if not exist %dir% mkdir %dir%
cd %dir%
::==========================================================
:: Generating Visual Studio project.
call cmake -G "Visual Studio 15 Win64" ../ -DCMAKE_INSTALL_PREFIX=../output
IF %ERRORLEVEL% NEQ 0 EXIT /B %ERRORLEVEL%

set CTEST_OUTPUT_ON_FAILURE=1
call cmake --build . --target install --config Release
IF %ERRORLEVEL% NEQ 0 EXIT /B %ERRORLEVEL%

::==========================================================
cd ../
::==========================================================