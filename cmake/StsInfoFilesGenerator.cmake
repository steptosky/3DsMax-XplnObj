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
# This module uses the StsProjectDesc module for generate ".h" file with information
#
# Version 1.3.1 (26.09.2018)
#   - Improved logic for getting git branch and revision
# Version 1.3.0 (30.08.2018)
#   - Reading GIT_COMMIT and GIT_BRANCH environment variables
#     if data isn't retrieved from the git directly.
#   - Fixed full name printing.
# Version 1.2.2 (01.06.2017)
#   - Generates hidden from doxygen file 
# Version 1.2.1 (16.05.2017)
#   - Fixed vcs detection
# Version 1.2.0 (21.04.2017)
#   - List of open source libraries
#   - Fixed for generation Contributors
# Version 1.1.0 (19.04.2017)
# Version 1.1.0 (19.03.2017)
# Version 1.0.1
#
#----------------------------------------------------------------------------------#
#
#   if vcs_revision, vcs_branch variables are not empty 
#       then they will not be overwritten.
#       It can be useful when the project is not in the git repository,
#           for example it is in the conan repository, 
#           so we can define those variables outside.
#
#   Usage example:
#
#       # specify the folder where this module is.
#       list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
#       
#       # include the modules
#       include(StsProjectDesk)
#       include(StsInfoFilesGenerator)
#
#       # generate file
#       genInfoFile(StsProjectDesk "${CMAKE_SOURCE_DIR}/include/Info.h")
#
#----------------------------------------------------------------------------------#
#//////////////////////////////////////////////////////////////////////////////////#
#----------------------------------------------------------------------------------#

function(genInfoFile descriptionFile destinationFile)

    message(STATUS "Info generated ${descriptionFile} -> ${destinationFile}")
    include("${descriptionFile}")

    set(__prfix__ ${ProjectDefinePrefix})
    set(__DECOR__ "//===========================================================================//")

    #-------------------------------------------------------------------#

    set(CONTENT "#pragma once\n\n")

    # hides content for doxygen
    set(CONTENT "${CONTENT}/// @cond sts_info_gen\n\n") 

    set(CONTENT "${CONTENT}/* This is the auto-generated file with the cmake script */\n\n")
    set(CONTENT "${CONTENT}${__DECOR__}\n\n")

    #-------------------------------------------------------------------#
    # Organization Info	

    set(CONTENT "${CONTENT}/* Organization Info */\n")
    if(IdentificationName)
        set(CONTENT "${CONTENT}#define ${__prfix__}IDENTIFICATION_NAME \"${IdentificationName}\"\n")
        message(WARNING "IdentificationName is depricated use ProjectId instead")
    endif()
    set(CONTENT "${CONTENT}#define ${__prfix__}ORGANIZATION_NAME \"${ProjectOrganizationName}\"\n")
    set(CONTENT "${CONTENT}#define ${__prfix__}ORGANIZATION_WEBLINK \"${ProjectOrganizationWebLink}\"\n\n")

    #-------------------------------------------------------------------#
    # Project info

    set(CONTENT "${CONTENT}/* Project info */\n")
    set(CONTENT "${CONTENT}#define ${__prfix__}PROJECT_NAME \"${ProjectName}\"\n")
    if(ProjectShortName)
        set(CONTENT "${CONTENT}#define ${__prfix__}PROJECT_SHORT_NAME \"deprecated\"\n")
        message(WARNING "ProjectShortName is depricated use ProjectName and ProjectFullName instead")
    endif()
    set(CONTENT "${CONTENT}#define ${__prfix__}PROJECT_FULL_NAME \"${ProjectFullName}\"\n")
    set(CONTENT "${CONTENT}#define ${__prfix__}PROJECT_DESCRIPTION \"${ProjectDescription}\"\n")
    set(CONTENT "${CONTENT}#define ${__prfix__}PROJECT_WEBLINK \"${ProjectWebLink}\"\n")
    set(CONTENT "${CONTENT}#define ${__prfix__}PROJECT_SOURCES_WEBLINK \"${ProjectSourcesWebLink}\"\n\n")

    set(CONTENT "${CONTENT}#define ${__prfix__}PROJECT_GROUP_ID \"${ProjectGroupId}\"\n")
    set(CONTENT "${CONTENT}#define ${__prfix__}PROJECT_ID \"${ProjectId}\"\n\n")

    #-------------------------------------------------------------------#
    # Version

    set(CONTENT "${CONTENT}/* Version Info */\n")
    set(CONTENT "${CONTENT}#define ${__prfix__}VERSION_MAJOR  ${ProjectVersionMajor}\n")
    set(CONTENT "${CONTENT}#define ${__prfix__}VERSION_MINOR  ${ProjectVersionMinor}\n")
    set(CONTENT "${CONTENT}#define ${__prfix__}VERSION_PATCH  ${ProjectVersionPatch}\n")
    set(CONTENT "${CONTENT}#define ${__prfix__}VERSION_STRING \"${ProjectVersion}\"\n")
    set(CONTENT "${CONTENT}#define ${__prfix__}VERSION ((${ProjectVersionMajor} << 16) + (${ProjectVersionMinor} << 8) + ${ProjectVersionPatch})\n")
    set(CONTENT "${CONTENT}#define ${__prfix__}RELEASE_TYPE \"${ProjectReleaseType}\"\n\n")

    #-------------------------------------------------------------------#
    # License

    set(CONTENT "${CONTENT}/* License Info */\n")
    set(CONTENT "${CONTENT}#define ${__prfix__}COPYRIGHT \"${ProjectCopyRight}\"\n")
    set(CONTENT "${CONTENT}#define ${__prfix__}AUTHOR \"${ProjectAuthor}\"\n")
    set(CONTENT "${CONTENT}#define ${__prfix__}LICENSE_TYPE \"${ProjectLicenseType}\"\n\n")

    #-------------------------------------------------------------------#
    # License

    set(CONTENT "${CONTENT}/* Helpers */\n")
    set(CONTENT "${CONTENT}#define ${__prfix__}ARRAY_LENGTH(a)(sizeof(a)/sizeof(*a))\n\n")

    #-------------------------------------------------------------------#
    # Contributors

    set(CONTENT "${CONTENT}/* Contributors Info */\n")	
    list(LENGTH ProjectContributors ProjectContributorsSize)

    if (NOT ${ProjectContributorsSize} EQUAL 0)
        set(CONTENT "${CONTENT}static const char * ${__prfix__}CONTRIBUTORS[] = {\n")
        foreach(Contributor ${ProjectContributors})
            set(CONTENT "${CONTENT} \t\"${Contributor}\",\n")
        endforeach()
        set(CONTENT "${CONTENT}};\n\n")
    else()
        set(CONTENT "${CONTENT}\n\n")
    endif()

    #-------------------------------------------------------------------#
    # Open source libraries

    set(CONTENT "${CONTENT}/* Open source libraries info*/\n") 
    set(CONTENT "${CONTENT}struct ${__prfix__}LIBRARIES_DATA {\n")

    set(CONTENT "${CONTENT}\t${__prfix__}LIBRARIES_DATA(\n")
    set(CONTENT "${CONTENT}\t\tconst char *inLibName,   const char *inLibUrl,\n")
    set(CONTENT "${CONTENT}\t\tconst char *inCopyright, const char *inCopyrightUrl,\n")
    set(CONTENT "${CONTENT}\t\tconst char *inLicense,   const char *inLicenseUrl)\n")

    set(CONTENT "${CONTENT}\t\t\t: libName(inLibName),     libUrl(inLibUrl),\n")
    set(CONTENT "${CONTENT}\t\t\t  copyright(inCopyright), copyrightUrl(inCopyrightUrl),\n")
    set(CONTENT "${CONTENT}\t\t\t  license(inLicense),     licenseUrl(inLicenseUrl) {}\n\n")

    set(CONTENT "${CONTENT}\tconst char * libName = nullptr;\n")
    set(CONTENT "${CONTENT}\tconst char * libUrl = nullptr;\n")
    set(CONTENT "${CONTENT}\tconst char * copyright = nullptr;\n")
    set(CONTENT "${CONTENT}\tconst char * copyrightUrl = nullptr;\n")
    set(CONTENT "${CONTENT}\tconst char * license = nullptr;\n")
    set(CONTENT "${CONTENT}\tconst char * licenseUrl = nullptr;\n")
    set(CONTENT "${CONTENT}};\n")
    list(LENGTH ProjectLibraries LibraryListSize)

    if (NOT ${LibraryListSize} EQUAL 0)
        set(CONTENT "${CONTENT}static const ${__prfix__}LIBRARIES_DATA ${__prfix__}LIBRARIES[] = {\n")
        foreach(Library ${ProjectLibraries})
            string(REPLACE "|" ";" LibraryList ${Library})
            list(LENGTH LibraryList LibraryListSize)
            if (${LibraryListSize} EQUAL 6)
                list(GET LibraryList "0" L_0)
                list(GET LibraryList "1" L_1)
                list(GET LibraryList "2" L_2)
                list(GET LibraryList "3" L_3)
                list(GET LibraryList "4" L_4)
                list(GET LibraryList "5" L_5)
                if(NOT ${L_0} STREQUAL "")
                    set(L_0 "\"${L_0}\"")
                else()
                    set(L_0 "nullptr")
                endif()
                if(NOT ${L_1} STREQUAL "")
                    set(L_1 "\"${L_1}\"")
                else()
                    set(L_1 "nullptr")
                endif()
                if(NOT ${L_2} STREQUAL "")
                    set(L_2 "\"${L_2}\"")
                else()
                    set(L_2 "nullptr")
                endif()
                if(NOT ${L_3} STREQUAL "")
                    set(L_3 "\"${L_3}\"")
                else()
                    set(L_3 "nullptr")
                endif()
                if(NOT ${L_4} STREQUAL "")
                    set(L_4 "\"${L_4}\"")
                else()
                    set(L_4 "nullptr")
                endif()
                if(NOT ${L_5} STREQUAL "")
                    set(L_5 "\"${L_5}\"")
                else()
                    set(L_5 "nullptr")
                endif()
                set(CONTENT "${CONTENT} \t${__prfix__}LIBRARIES_DATA(${L_0}, ${L_1}, ${L_2}, ${L_3}, ${L_4}, ${L_5}),\n")
            else()
                message(FATAL_ERROR "Library description has incorrect number of variables: ${LibraryListSize}\n\t${Library}")
            endif()
        endforeach()
        set(CONTENT "${CONTENT}};\n\n")
    else()
        set(CONTENT "${CONTENT}\n\n")	
    endif()
    
    #-------------------------------------------------------------------#
    # Other

    set(CONTENT "${CONTENT}/* Other Info */\n\n")

    set(CONTENT "${CONTENT}/*! \\warning For internal use only */\n")
    set(CONTENT "${CONTENT}#define ${__prfix__}COMPILE_DATE __DATE__\n")
    set(CONTENT "${CONTENT}/*! \\warning For internal use only */\n")
    set(CONTENT "${CONTENT}#define ${__prfix__}COMPILE_TIME __TIME__\n\n")

    set(CONTENT "${CONTENT}#define ${__prfix__}COMPILER_NAME \"${CMAKE_CXX_COMPILER_ID}\" \n")
    set(CONTENT "${CONTENT}#define ${__prfix__}COMPILER_VERSION \"${CMAKE_CXX_COMPILER_VERSION}\" \n\n")

    if(NOT vcs_revision OR NOT vcs_branch)
        if (${ProjectVcsType} STREQUAL git)
            if (EXISTS "${CMAKE_SOURCE_DIR}/.git")
                if (NOT vcs_revision)
                    execute_process(
                            COMMAND "git" "log" "-1" "--pretty=format:%h"
                            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
                            RESULT_VARIABLE vcs_result
                            OUTPUT_VARIABLE vcs_revision
                    )
                endif()
                if (NOT vcs_branch)
                    execute_process(
                            COMMAND "git" "rev-parse" "--abbrev-ref" "HEAD"
                            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
                            RESULT_VARIABLE vcs_result
                            OUTPUT_VARIABLE vcs_branch
                    )
                    if(vcs_branch)
                        string(STRIP ${vcs_branch} vcs_branch)
                        set(CONTENT "${CONTENT}/* git */\n")
                    endif()
                endif()
            else()
                message(WARNING "<${CMAKE_SOURCE_DIR}> isn't a git repository")
            endif()
        else()
            message(STATUS "VCS <ProjectVcsType> variable isn't specified")
        endif()
    else()
        message(STATUS "<vcs_revision> and <vcs_branch> has already been set from outside")
    endif()
    #-------------------------------#
    if(NOT vcs_revision)
        if (NOT $ENV{GIT_COMMIT} STREQUAL "")
            set(vcs_revision $ENV{GIT_COMMIT})
            message(STATUS "Got GIT commit/revision <$ENV{GIT_COMMIT}> from <GIT_COMMIT> environment variable")
        else()
            set(vcs_revision "undefined")
        endif()
    endif()
    if(NOT vcs_branch OR "${vcs_branch}" STREQUAL "HEAD")
        if (NOT $ENV{GIT_BRANCH} STREQUAL "")
            set(vcs_branch $ENV{GIT_BRANCH})
            message(STATUS "Got GIT branch name <$ENV{GIT_BRANCH}> from <GIT_BRANCH> environment variable")
        else()
            if(NOT vcs_branch)
                set(vcs_branch "undefined")
            endif()
            message(WARNING "Current branch name is <${vcs_branch}> and <GIT_BRANCH> env variable isn't set.")
        endif()
    endif()

    message(STATUS "VCS Revision = ${vcs_revision}")
    message(STATUS "VCS branch = ${vcs_branch}")
    #-------------------------------#

    set(CONTENT "${CONTENT}#define ${__prfix__}VCS_REVISION \"${vcs_revision}\" \n")
    set(CONTENT "${CONTENT}#define ${__prfix__}VCS_BRANCH \"${vcs_branch}\" \n\n")

    #-------------------------------------------------------------------#

    # hides content for doxygen
    set(CONTENT "${CONTENT}/// @endcond\n\n")

    set(CONTENT "${CONTENT}\n${__DECOR__}")
    file(WRITE ${destinationFile} "${CONTENT}")

    #-------------------------------------------------------------------#

endfunction()

#----------------------------------------------------------------------------------#
#//////////////////////////////////////////////////////////////////////////////////#
#----------------------------------------------------------------------------------#