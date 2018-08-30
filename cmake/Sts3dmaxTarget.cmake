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
# StepToSky 3DsMax terget setup.
#
# Version 1.0.0 (03.04.2017)
#
#----------------------------------------------------------------------------------#
#
#   Usage example:
#
#       # specify the folder where this module is.
#       list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
#       
#       include(Sts3dmaxTarget)
#       SETUP_MAX_TERGET(SDK_TARGET "3DsMaxSdk2008" 3DMAX_VERSION "2008")
#       SETUP_MAX_TERGET(SDK_TARGET "3DsMaxSdk2009" 3DMAX_VERSION "2009")
#       SETUP_MAX_TERGET(SDK_TARGET "3DsMaxSdk2010" 3DMAX_VERSION "2010")
#
#----------------------------------------------------------------------------------#
#//////////////////////////////////////////////////////////////////////////////////#
#----------------------------------------------------------------------------------#

cmake_minimum_required (VERSION 3.7.0)
include(CMakeParseArguments)

function(SETUP_MAX_TERGET)

    set(oneValueArgs SDK_TARGET 3DMAX_VERSION)
    cmake_parse_arguments(SETUP_MAX_TERGET "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN} )

    if(TARGET CONAN_PKG::${SETUP_MAX_TERGET_SDK_TARGET})
        message(STATUS "3DsMax + ${SETUP_MAX_TERGET_3DMAX_VERSION}")
        set (PROJECT "3DsMax-${SETUP_MAX_TERGET_3DMAX_VERSION}")
        set (OUTPUT_NAME "3DsMax${SETUP_MAX_TERGET_3DMAX_VERSION}-XplnObj")
        
        #--------------------------------------------------------------------------#
        #//////////////////////////////////////////////////////////////////////////#
        #--------------------------------------------------------------------------#
        # compiler 

        if (NOT ${CMAKE_CXX_COMPILER_ID} STREQUAL MSVC)
            message (FATAL_ERROR "<${PROJECT}> Unsupported compiler")
        endif ()

        #--------------------------------------------------------------------------#
        #//////////////////////////////////////////////////////////////////////////#
        #--------------------------------------------------------------------------#
        # project files

        file(GLOB_RECURSE CM_FILES 
            "${CMAKE_SOURCE_DIR}/src/*.h"
            "${CMAKE_SOURCE_DIR}/src/*.hpp" 
            "${CMAKE_SOURCE_DIR}/src/*.inl" 
            "${CMAKE_SOURCE_DIR}/src/*.cpp"
            "${CMAKE_SOURCE_DIR}/src/*.rc"
            "${CMAKE_SOURCE_DIR}/src/*.txt"
            "${CMAKE_SOURCE_DIR}/include/*.h" 
            "${CMAKE_SOURCE_DIR}/include/*.inl" 
            "${CMAKE_SOURCE_DIR}/include/*.cpp"

            "${CMAKE_SOURCE_DIR}/doc/*"
        )

        include(StsGroupFiles)
        groupFiles("${CM_FILES}")

        list(APPEND CM_FILES "${CMAKE_SOURCE_DIR}/readme.md")
        source_group("doc" FILES "${CMAKE_SOURCE_DIR}/readme.md")

        #--------------------------------------------------------------------------#
        #//////////////////////////////////////////////////////////////////////////#
        #--------------------------------------------------------------------------#
        # include/link directories

        include_directories (${CMAKE_SOURCE_DIR}/include)
        include_directories (${CMAKE_SOURCE_DIR}/src)

        #--------------------------------------------------------------------------#
        #//////////////////////////////////////////////////////////////////////////#
        #--------------------------------------------------------------------------#

        add_library(${PROJECT} SHARED ${CM_FILES})
            
        target_link_libraries(${PROJECT} CONAN_PKG::${SETUP_MAX_TERGET_SDK_TARGET})
        target_link_libraries(${PROJECT} CONAN_PKG::XplnObj)
        target_link_libraries(${PROJECT} CONAN_PKG::sts-signals)
        target_link_libraries(${PROJECT} CONAN_PKG::sts-semver)
        target_link_libraries(${PROJECT} CONAN_PKG::jsonformoderncpp)
        target_link_libraries(${PROJECT} Winhttp)
        
        #--------------------------------------------------------------------------#
        # cxx standard 

        set_target_properties(${PROJECT} PROPERTIES CXX_STANDARD 14 CXX_STANDARD_REQUIRED YES)
        target_compile_features(${PROJECT} PUBLIC cxx_std_14)

        #--------------------------------------------------------------------------#
        # compile options

        # pre-compile headers
        set_source_files_properties(src/stdafx.cpp
            PROPERTIES COMPILE_FLAGS $<$<CXX_COMPILER_ID:MSVC>:/Yc>
        )

        target_compile_options(${PROJECT}
            PRIVATE $<$<CXX_COMPILER_ID:MSVC>:/Yustdafx.h>     # pre-compile headers
            PRIVATE $<$<CXX_COMPILER_ID:MSVC>:/FIstdafx.h>     # pre-compile headers
            PRIVATE $<$<CXX_COMPILER_ID:MSVC>:/MP>
            #PRIVATE $<$<CXX_COMPILER_ID:MSVC>:/WX>             # warnings as error
            PRIVATE $<$<CXX_COMPILER_ID:MSVC>:/W4>
            PRIVATE $<$<CXX_COMPILER_ID:MSVC>:-D_USRDLL>
            PRIVATE $<$<CXX_COMPILER_ID:MSVC>:-D_WIN64>
            PRIVATE $<$<CXX_COMPILER_ID:MSVC>:-D_CRT_SECURE_NO_DEPRECATE>
            PRIVATE $<$<CXX_COMPILER_ID:MSVC>:-DISOLATION_AWARE_ENABLED=1>
        )

        #--------------------------------------------------------------------------#
        #//////////////////////////////////////////////////////////////////////////#
        #--------------------------------------------------------------------------#

        set_target_properties(${PROJECT} PROPERTIES DEBUG_OUTPUT_NAME  ${OUTPUT_NAME})
        set_target_properties(${PROJECT} PROPERTIES RELEASE_OUTPUT_NAME  ${OUTPUT_NAME})
        set_target_properties(${PROJECT} PROPERTIES SUFFIX  ".dlu")
        set_target_properties(${PROJECT} PROPERTIES DEBUG_POSTFIX "-x64")
        set_target_properties(${PROJECT} PROPERTIES RELEASE_POSTFIX "-x64")

        #--------------------------------------------------------------------------#
        #//////////////////////////////////////////////////////////////////////////#
        #--------------------------------------------------------------------------#

        install(TARGETS ${PROJECT} RUNTIME DESTINATION "$<CONFIG>" CONFIGURATIONS "Release")
        install(TARGETS ${PROJECT} RUNTIME DESTINATION "$<CONFIG>" CONFIGURATIONS "Debug")

        #--------------------------------------------------------------------------#
        #//////////////////////////////////////////////////////////////////////////#
        #--------------------------------------------------------------------------#
        
        include(StsDebugCommand)
        set(3DSMAX_EXE_PATH ${SETUP_MAX_TERGET_3DMAX_${SETUP_MAX_TERGET_3DMAX_VERSION}_PATH})
        if (3DSMAX_EXE_PATH)
            add_3dmax_path(${PROJECT} "${3DSMAX_EXE_PATH}")
        endif()
        
        #--------------------------------------------------------------------------#
        #//////////////////////////////////////////////////////////////////////////#
        #--------------------------------------------------------------------------#

    else()
        message(STATUS "3DsMax - ${SETUP_MAX_TERGET_3DMAX_VERSION}")
    endif()

endfunction(SETUP_MAX_TERGET)

#----------------------------------------------------------------------------------#
#//////////////////////////////////////////////////////////////////////////////////#
#----------------------------------------------------------------------------------#