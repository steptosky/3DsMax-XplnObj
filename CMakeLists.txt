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
#//////////////////////////////////////////////////////////////////////////////////#
#----------------------------------------------------------------------------------#
# init

message(STATUS "==============================================")
cmake_minimum_required (VERSION 3.7.0)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
 
#----------------------------------------------------------------------------------#
#//////////////////////////////////////////////////////////////////////////////////#
#----------------------------------------------------------------------------------#
# Solution

project("3DsMax-XplnObj" LANGUAGES "CXX")

#----------------------------------------------------------------------------------#
# Info file

include(StsProjectDesc)
include(StsInfoFilesGenerator)
genInfoFile(StsProjectDesc "${CMAKE_SOURCE_DIR}/src/Info.h")

#----------------------------------------------------------------------------------#
#//////////////////////////////////////////////////////////////////////////////////#
#----------------------------------------------------------------------------------#
# conan.io

include(StsConan)

#----------------------------------------------------------------------------------#
#//////////////////////////////////////////////////////////////////////////////////#
#----------------------------------------------------------------------------------#
# options 
 
if (NOT CMAKE_BUILD_TYPE)
	set (CMAKE_BUILD_TYPE Release)
endif()

if (NOT TEST_REPORT_DIR)
	set(TEST_REPORT_DIR ${CMAKE_SOURCE_DIR}/reports/tests)
endif()

if (NOT BUILD_SHARED_LIBS)
	set (BUILD_SHARED_LIBS OFF)
endif()

option(BUILD_TESTS "Build all of it's own tests." OFF) 

message(STATUS "==============================================")
message(STATUS "Build type = ${CMAKE_BUILD_TYPE}")
message(STATUS "Build tests = ${BUILD_TESTS}")
message(STATUS "Shared lib = ${BUILD_SHARED_LIBS}")
message(STATUS "Test report dir = ${TEST_REPORT_DIR}")
message(STATUS "Installation prefix = ${CMAKE_INSTALL_PREFIX}")
message(STATUS "==============================================")

#----------------------------------------------------------------------------------#
#//////////////////////////////////////////////////////////////////////////////////#
#----------------------------------------------------------------------------------#		
# help targets

add_custom_target("Clean-Install-Dir"
	COMMAND cmake -E remove_directory ${CMAKE_INSTALL_PREFIX}
)

#add_custom_target("CLEAR-TESTS-REPORT"
#	COMMAND cmake -E remove_directory ${TESTS_REPORTS_DIR}
#)

#----------------------------------------------------------------------------------#
#//////////////////////////////////////////////////////////////////////////////////#
#----------------------------------------------------------------------------------#
# Projects

include(Sts3dmaxTarget)
SETUP_MAX_TERGET(SDK_TARGET "3DsMaxSdk9" 3DMAX_VERSION "9")
SETUP_MAX_TERGET(SDK_TARGET "3DsMaxSdk2008" 3DMAX_VERSION "2008")
SETUP_MAX_TERGET(SDK_TARGET "3DsMaxSdk2009" 3DMAX_VERSION "2009")
SETUP_MAX_TERGET(SDK_TARGET "3DsMaxSdk2010" 3DMAX_VERSION "2010")
SETUP_MAX_TERGET(SDK_TARGET "3DsMaxSdk2011" 3DMAX_VERSION "2011")
SETUP_MAX_TERGET(SDK_TARGET "3DsMaxSdk2012" 3DMAX_VERSION "2012")
SETUP_MAX_TERGET(SDK_TARGET "3DsMaxSdk2013" 3DMAX_VERSION "2013")
SETUP_MAX_TERGET(SDK_TARGET "3DsMaxSdk2014" 3DMAX_VERSION "2014")
SETUP_MAX_TERGET(SDK_TARGET "3DsMaxSdk2015" 3DMAX_VERSION "2015")
SETUP_MAX_TERGET(SDK_TARGET "3DsMaxSdk2016" 3DMAX_VERSION "2016")
SETUP_MAX_TERGET(SDK_TARGET "3DsMaxSdk2017" 3DMAX_VERSION "2017")

if (opt_build_tests)
	enable_testing()
	add_subdirectory(test)
endif ()

message(STATUS "==============================================")
#-----------------------------------------------------------------------#
# //////////////////////////////////////////////////////////////////////#
#-----------------------------------------------------------------------#
