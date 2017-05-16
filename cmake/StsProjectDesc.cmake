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
# StepToSky project description.
#
# Version 1.2.0 (21.04.2017) 
#	- List of open source libraries
#
# Version 1.1.0 (19.03.2017)
# Version 1.0.0
#
#----------------------------------------------------------------------------------#
#
# This file describes the project.
# Use StsInfoFilesGenerator module for generation c++ .h file.
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
# Helpers

string(TIMESTAMP _curr_y_ "%Y" UTC)

#----------------------------------------------------------------------------------#
# Organization Info	

set(ProjectOrganizationName "StepToSky")
set(ProjectOrganizationWebLink "www.steptosky.com")

#----------------------------------------------------------------------------------#
# Project info

set(ProjectName "3DsMax X-Plane Obj Exporter")
set(ProjectShortName "3DsMax-XplnObj")
set(ProjectDescription "This plugin is for the 3DsMax that allows you to import or export X-Plane's obj format.")
set(ProjectWebLink "www.steptosky.com")
set(ProjectSourcesWebLink "https://github.com/steptosky/3DsMax-XplnObj")

#----------------------------------------------------------------------------------#
# Version / Dependency's system

set(ProjectVersionMajor 2)
set(ProjectVersionMinor 1)
set(ProjectVersionPatch 0)
set(ProjectVersion "${ProjectVersionMajor}.${ProjectVersionMinor}.${ProjectVersionPatch}")
set(ProjectReleaseType "") # You can use any string you wish

set(ProjectGroupId "steptosky")
set(ProjectId ${ProjectShortName})

#----------------------------------------------------------------------------------#
# License

set(ProjectAuthor "StepToSky")
set(ProjectCopyRight "Copyright ${ProjectAuthor} ${_curr_y_}")
set(ProjectLicenseType "BSD 3-Clause")

#----------------------------------------------------------------------------------#
# Contributors

set(ProjectContributors "")
list(APPEND ProjectContributors "StepToSky <info@steptosky.com>")

#----------------------------------------------------------------------------------#
# Open source libraries
# Use the following format. Symbol | is the separator
# For unspecified fields use nothing but don't forget the separator
# libname|lib url|copyright|copyrighter url|license|license url
# example, the urls are empty: "XplnObj||Copyright (c) 2017 StepToSky||BSD|"

set(ProjectLibraries "")
list(APPEND ProjectLibraries "JSON for Modern C++|https://github.com/nlohmann/json|Copyright 2013-2017 Niels Lohmann|http://nlohmann.me|MIT|")
                                                                                  
#----------------------------------------------------------------------------------#
# Other

# VCS type this project uses. Can be either "git" or empty "".
set(ProjectVcsType "git") 

# Prefix for the defines.
set(ProjectDefPrefix "XIO_") 

#----------------------------------------------------------------------------------#
# Checking

if(NOT ProjectId)
	message(FATAL_ERROR "ProjectId is not specified")
endif()

#----------------------------------------------------------------------------------#
#//////////////////////////////////////////////////////////////////////////////////#
#----------------------------------------------------------------------------------#
