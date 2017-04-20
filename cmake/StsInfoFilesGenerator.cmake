#----------------------------------------------------------------------------------#
#//////////////////////////////////////////////////////////////////////////////////#
#----------------------------------------------------------------------------------#
#
#  Copyright (C) 2017, StepToSky
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
# Version 1.1.1 (19.04.2017)
# Version 1.1.0 (19.03.2017)
# Version 1.0.1
#
#----------------------------------------------------------------------------------#
#
#	if vcs_revision, vcs_branch variables are not empty 
#		then they will not be overwritten.
#		It can be useful when the project is not in the git repository,
#			for example it is in the conan repository, 
#			so we can define those variables outside.
#
#	Usage example:
#
#		# specify the folder where this module is.
#		list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
#		
# 		# include the modules
#		include(StsProjectDesk)
#		include(StsInfoFilesGenerator)
#
#		# generate file
#		genInfoFile(StsProjectDesk "${CMAKE_SOURCE_DIR}/include/Info.h")
#
#----------------------------------------------------------------------------------#
#//////////////////////////////////////////////////////////////////////////////////#
#----------------------------------------------------------------------------------#

function(genInfoFile descriptionFile destinationFile)

	message(STATUS "Info generated ${descriptionFile} -> ${destinationFile}")
	include("${descriptionFile}")
	
	set(__prfix__ ${ProjectDefPrefix})
	set(__DECOR__ "//===========================================================================//")

	#-------------------------------------------------------------------#

	set(CONTENT "#pragma once\n\n")
	set(CONTENT "${CONTENT}/* This is the auto-genetared file with the cmake script */\n\n")
	set(CONTENT "${CONTENT}${__DECOR__}\n\n")

	#-------------------------------------------------------------------#
	# Organization Info	

	set(CONTENT "${CONTENT}/* Organization Info */\n")
	if(IdentificationName)
		set(CONTENT "${CONTENT}#define ${__prfix__}IDENTIFICATION_NAME \"${IdentificationName}\"\n")
		message(WARNING "IdentificationName is depricated use ArtifactId instead")
	endif()
	set(CONTENT "${CONTENT}#define ${__prfix__}ORGANIZATION_NAME \"${ProjectOrganizationName}\"\n")
	set(CONTENT "${CONTENT}#define ${__prfix__}ORGANIZATION_WEBLINK \"${ProjectOrganizationWebLink}\"\n\n")

	#-------------------------------------------------------------------#
	# Project info

	set(CONTENT "${CONTENT}/* Project info */\n")
	set(CONTENT "${CONTENT}#define ${__prfix__}PROJECT_NAME \"${ProjectName}\"\n")
	set(CONTENT "${CONTENT}#define ${__prfix__}PROJECT_SHORT_NAME \"${ProjectShortName}\"\n")
	set(CONTENT "${CONTENT}#define ${__prfix__}PROJECT_DESCRIPTION \"${ProjectDescription}\"\n")
	set(CONTENT "${CONTENT}#define ${__prfix__}PROJECT_WEBLINK \"${ProjectWebLink}\"\n")
	set(CONTENT "${CONTENT}#define ${__prfix__}PROJECT_SOURCES_WEBLINK \"${ProjectSourcesWebLink}\"\n\n")

	#-------------------------------------------------------------------#
	# Dependency's system

	set(CONTENT "${CONTENT}/* Dependency's system */\n")
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
	# Contributors
	
	set(CONTENT "${CONTENT}/* Contributors Info */\n") 
	set(CONTENT "${CONTENT}#define ${__prfix__}ARRAY_LENGTH(a)(sizeof(a)/sizeof(*a))\n") 
	set(CONTENT "${CONTENT}static const char * ${__prfix__}CONTRIBUTORS[] = {\n")
	foreach(Contributor ${ProjectContributors})
		set(CONTENT "${CONTENT} \t\"${Contributor}\",\n")
	endforeach()
	set(CONTENT "${CONTENT}};\n\n")
	
	#-------------------------------------------------------------------#
	# Other

	set(CONTENT "${CONTENT}/* Other Info */\n\n")

	set(CONTENT "${CONTENT}/*! \\warning For internal use only */\n")
	set(CONTENT "${CONTENT}#define ${__prfix__}COMPILE_DATE __DATE__\n")
	set(CONTENT "${CONTENT}/*! \\warning For internal use only */\n")
	set(CONTENT "${CONTENT}#define ${__prfix__}COMPILE_TIME __TIME__\n\n")
	
	set(CONTENT "${CONTENT}#define ${__prfix__}COMPILER_NAME \"${CMAKE_CXX_COMPILER_ID}\" \n")
	set(CONTENT "${CONTENT}#define ${__prfix__}COMPILER_VERSION \"${CMAKE_CXX_COMPILER_VERSION}\" \n\n")

	if (${ProjectVcsType} AND ${ProjectVcsType} STREQUAL git)
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
		message(STATUS "VCS is not specified")
	endif()
	
	#-------------------------------#
	if(NOT vcs_revision)
		set(vcs_revision "undefined")
	endif()
	if(NOT vcs_branch)
		set(vcs_branch "undefined")
	endif()
	
	message(STATUS "VCS Revision = ${vcs_revision}")
	message(STATUS "VCS branch = ${vcs_branch}")
	#-------------------------------#
	
	set(CONTENT "${CONTENT}#define ${__prfix__}VCS_REVISION \"${vcs_revision}\" \n")
	set(CONTENT "${CONTENT}#define ${__prfix__}VCS_BRANCH \"${vcs_branch}\" \n")

	#-------------------------------------------------------------------#

	set(CONTENT "${CONTENT}\n${__DECOR__}")
	file(WRITE ${destinationFile} "${CONTENT}")

	#-------------------------------------------------------------------#

endfunction()

#----------------------------------------------------------------------------------#
#//////////////////////////////////////////////////////////////////////////////////#
#----------------------------------------------------------------------------------#