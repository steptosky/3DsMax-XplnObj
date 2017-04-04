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
#
# Version 1.0.0 (04.04.2017)
#
#----------------------------------------------------------------------------------#
#
#	This script is for generation the project description data 
#		as the environment variables list for using in CI systems
#
#	Usage example:
#		Variables:
#			FILE_DESC - path to the StsProjectDesc, default: StsProjectDesc.cmake
#			FILE_OUTPUT - path to the output file default: desc-variables.txt
#			CONTENT_PREFIX - text will be added as the prefix for each variable, default is empty
#
#		Use -DCONTENT_PREFIX="Sts" -P StsProjectDescCiEnvGenerator.cmake in the CI build step
#			it will produce files with environmet variables.
#		Then insert thet variables into your CI
#
#----------------------------------------------------------------------------------#
#//////////////////////////////////////////////////////////////////////////////////#
#----------------------------------------------------------------------------------#

if (NOT FILE_DESC)
	set(FILE_DESC "StsProjectDesc.cmake")
endif()

if (NOT FILE_OUTPUT)
	set(FILE_OUTPUT "variables.txt")
endif()

include(${FILE_DESC})

set(CONTENT "${CONTENT}${CONTENT_PREFIX}ProjectOrganizationName=${ProjectOrganizationName}\n")
set(CONTENT "${CONTENT}${CONTENT_PREFIX}ProjectOrganizationWebLink=${ProjectOrganizationWebLink}\n")

set(CONTENT "${CONTENT}${CONTENT_PREFIX}ProjectName=${ProjectName}\n")
set(CONTENT "${CONTENT}${CONTENT_PREFIX}ProjectShortName=${ProjectShortName}\n")
set(CONTENT "${CONTENT}${CONTENT_PREFIX}ProjectDescription=${ProjectDescription}\n")
set(CONTENT "${CONTENT}${CONTENT_PREFIX}ProjectWebLink=${ProjectWebLink}\n")
set(CONTENT "${CONTENT}${CONTENT_PREFIX}ProjectSourcesWebLink=${ProjectSourcesWebLink}\n")

set(CONTENT "${CONTENT}${CONTENT_PREFIX}ProjectVersionMajor=${ProjectVersionMajor}\n")
set(CONTENT "${CONTENT}${CONTENT_PREFIX}ProjectVersionMinor=${ProjectVersionMinor}\n")
set(CONTENT "${CONTENT}${CONTENT_PREFIX}ProjectVersionPatch=${ProjectVersionPatch}\n")
set(CONTENT "${CONTENT}${CONTENT_PREFIX}ProjectVersion=${ProjectVersion}\n")
set(CONTENT "${CONTENT}${CONTENT_PREFIX}ProjectVersionTag=${ProjectVersion}-${ProjectReleaseType}\n")

set(CONTENT "${CONTENT}${CONTENT_PREFIX}ProjectGroupId=${ProjectGroupId}\n")
set(CONTENT "${CONTENT}${CONTENT_PREFIX}ProjectId=${ProjectId}\n")

set(CONTENT "${CONTENT}${CONTENT_PREFIX}ProjectAuthor=${ProjectAuthor}\n")
set(CONTENT "${CONTENT}${CONTENT_PREFIX}ProjectCopyRight=${ProjectCopyRight}\n")
set(CONTENT "${CONTENT}${CONTENT_PREFIX}ProjectLicenseType=${ProjectLicenseType}\n")

set(CONTENT "${CONTENT}${CONTENT_PREFIX}ProjectContributors=${ProjectContributors}\n")

file(WRITE ${FILE_OUTPUT} "${CONTENT}")

#----------------------------------------------------------------------------------#
#//////////////////////////////////////////////////////////////////////////////////#
#----------------------------------------------------------------------------------#