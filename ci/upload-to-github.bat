::==========================================================
set CURR_WD=%cd%
cd cmake
call cmake -DWORKING_DIR="%CURR_WD%" -P StsUploadToGithub.cmake
IF %ERRORLEVEL% NEQ 0 EXIT /B %ERRORLEVEL%
cd ../
::==========================================================