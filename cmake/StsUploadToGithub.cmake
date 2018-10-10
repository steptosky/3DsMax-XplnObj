#----------------------------------------------------------------------------------#
#//////////////////////////////////////////////////////////////////////////////////#
#----------------------------------------------------------------------------------#
#
#  Copyright (C) 2018, StepToSky
#  All rights reserved.
#
#  Redistribution and use in source and binary forms, with or without
#  modification, are permitted provided that the following conditions are met:
#
#  1.Redistributions of source code must retain the above copyright notice, this
#    list of conditions and the following disclaimer.
#  2.Redistributions in binary form must reproduce the above copyright notice,
#    this list of conditions and the following disclaimer in the documentation
#    and / or other materials provided with the distribution.
#  3.Neither the name of StepToSky nor the names of its
#    contributors may be used to endorse or promote products derived from
#    this software without specific prior written permission.
#
#  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
#  ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
#  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
#  DISCLAIMED.IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
#  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
#  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
#  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
#  ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
#  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
#  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#
#  Contacts: www.steptosky.com
#
#----------------------------------------------------------------------------------#
#
# Version 1.0.0
#
#----------------------------------------------------------------------------------#
#//////////////////////////////////////////////////////////////////////////////////#
#----------------------------------------------------------------------------------#

set(REPOSITORY_NAME "3DsMax-XplnObj")
set(REPOSITORY_USER_NAME "steptosky")

#----------------------------------------------------------------------------------#
#//////////////////////////////////////////////////////////////////////////////////#
#----------------------------------------------------------------------------------#

message(STATUS "==============================================")
message(STATUS "Uploading artifacts ")

if (NOT WORKING_DIR)
    message(FATAL_ERROR "Working dir isn't specified. Usually it should be the root repository folder.")
endif()

include(StsProjectDesc.cmake)

if (ProjectReleaseType)
    set(ProjectVersionTag "${ProjectVersion}-${ProjectReleaseType}")
else()
    set(ProjectVersionTag "${ProjectVersion}")
endif()

#----------------------------------------------------------------------------------#

message(STATUS "==============================================")
message(STATUS "          Working dir = ${WORKING_DIR}")
message(STATUS "                  Tag = ${ProjectVersionTag}")
message(STATUS "      Repository name = ${REPOSITORY_NAME}")
message(STATUS " Repository user name = ${REPOSITORY_USER_NAME}")
message(STATUS "    Organization name = ${ProjectOrganizationName}")
message(STATUS "         Project name = ${ProjectName}")
message(STATUS "          Description = ${ProjectDescription}")
message(STATUS "==============================================")

#----------------------------------------------------------------------------------#

function (check_result result error output)
    string(LENGTH "${error}" ERROR_VAR_LENGTH)
    string(LENGTH "${output}" OUTPUT_VAR_LENGTH)

    if (NOT ${result} EQUAL 0 OR ${ERROR_VAR_LENGTH} GREATER 3)
        message(STATUS "FAILED")
        message(STATUS "==============================================")
        message(FATAL_ERROR "Result: ${result} Error: ${error}")
    endif()
    
    if (NOT ${OUTPUT_VAR_LENGTH} EQUAL 0)
        message(STATUS "Output: ${output}")
        message(STATUS "OK")
    else()
        message(STATUS "OK")
    endif()    
endfunction ()

function (create_tag)
    message(STATUS "Running 'github-release release'")
    execute_process(
        COMMAND github-release release 
                    --user ${REPOSITORY_USER_NAME}  
                    --repo ${REPOSITORY_NAME} 
                    --tag ${ProjectVersionTag} 
                    --name "${ProjectOrganizationName} ${ProjectName}" 
                    --description "${ProjectDescription}"
            RESULT_VARIABLE RESULT_VAR
            OUTPUT_VARIABLE OUTPUT_VAR
            ERROR_VARIABLE ERROR_VAR
    )
    check_result("${RESULT_VAR}" "${ERROR_VAR}" "${OUTPUT_VAR}")
endfunction ()

function (upload_doc DOC_FILE DOC_FILE_NAME)
    message(STATUS "Running 'github-release upload' uploading '${DOC_FILE}'")
    execute_process(
        COMMAND github-release upload 
                    --user ${REPOSITORY_USER_NAME}  
                    --repo ${REPOSITORY_NAME} 
                    --tag ${ProjectVersionTag} 
                    --name "${DOC_FILE_NAME}" 
                    --file "${WORKING_DIR}/doc/${DOC_FILE}"
            RESULT_VARIABLE RESULT_VAR
            OUTPUT_VARIABLE OUTPUT_VAR
            ERROR_VARIABLE ERROR_VAR
    )
    check_result("${RESULT_VAR}" "${ERROR_VAR}" "${OUTPUT_VAR}")
endfunction ()

function (upload_plugin PLUGIN_NAME)
    message(STATUS "Running 'github-release upload' uploading '${PLUGIN_NAME}'")
    execute_process(
        COMMAND github-release upload 
                    --user ${REPOSITORY_USER_NAME}  
                    --repo ${REPOSITORY_NAME} 
                    --tag ${ProjectVersionTag} 
                    --name "${PLUGIN_NAME}" 
                    --file "${WORKING_DIR}/output/Release/${PLUGIN_NAME}"
            RESULT_VARIABLE RESULT_VAR
            OUTPUT_VARIABLE OUTPUT_VAR
            ERROR_VARIABLE ERROR_VAR
    )
    check_result("${RESULT_VAR}" "${ERROR_VAR}" "${OUTPUT_VAR}")
endfunction ()

#----------------------------------------------------------------------------------#

create_tag()
upload_doc("changelog.rst" "changelog.txt")
upload_doc("install.txt" "install.txt")
upload_plugin("3DsMax9-XplnObj-x64.dlu")
upload_plugin("3DsMax2008-XplnObj-x64.dlu")
upload_plugin("3DsMax2009-XplnObj-x64.dlu")
upload_plugin("3DsMax2010-XplnObj-x64.dlu")
upload_plugin("3DsMax2011-XplnObj-x64.dlu")
upload_plugin("3DsMax2012-XplnObj-x64.dlu")
upload_plugin("3DsMax2013-XplnObj-x64.dlu")
upload_plugin("3DsMax2014-XplnObj-x64.dlu")
upload_plugin("3DsMax2015-XplnObj-x64.dlu")
upload_plugin("3DsMax2016-XplnObj-x64.dlu")
upload_plugin("3DsMax2017-XplnObj-x64.dlu")
upload_plugin("3DsMax2018-XplnObj-x64.dlu")
upload_plugin("3DsMax2019-XplnObj-x64.dlu")

message(STATUS "==============================================")

#----------------------------------------------------------------------------------#
#//////////////////////////////////////////////////////////////////////////////////#
#----------------------------------------------------------------------------------#
