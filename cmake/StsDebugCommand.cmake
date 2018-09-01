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
#//////////////////////////////////////////////////////////////////////////////////#
#----------------------------------------------------------------------------------#

cmake_minimum_required (VERSION 3.7.0)

set(SETUP_MAX_TERGET_3DMAX_9_PATH "C:\\Program Files\\Autodesk\\3ds Max 9\\3dsmax.exe")
set(SETUP_MAX_TERGET_3DMAX_2008_PATH "C:\\Program Files\\Autodesk\\3ds Max 2008\\3dsmax.exe")
set(SETUP_MAX_TERGET_3DMAX_2009_PATH "C:\\Program Files\\Autodesk\\3ds Max 2009\\3dsmax.exe")
set(SETUP_MAX_TERGET_3DMAX_2010_PATH "C:\\Program Files\\Autodesk\\3ds Max 2010\\3dsmax.exe")
set(SETUP_MAX_TERGET_3DMAX_2011_PATH "C:\\Program Files\\Autodesk\\3ds Max 2011\\3dsmax.exe")
set(SETUP_MAX_TERGET_3DMAX_2012_PATH "C:\\Program Files\\Autodesk\\3ds Max 2012\\3dsmax.exe")
set(SETUP_MAX_TERGET_3DMAX_2013_PATH "C:\\Program Files\\Autodesk\\3ds Max 2013\\3dsmax.exe")
set(SETUP_MAX_TERGET_3DMAX_2014_PATH "C:\\Program Files\\Autodesk\\3ds Max 2014\\3dsmax.exe")
set(SETUP_MAX_TERGET_3DMAX_2015_PATH "C:\\Program Files\\Autodesk\\3ds Max 2015\\3dsmax.exe")
set(SETUP_MAX_TERGET_3DMAX_2016_PATH "C:\\Program Files\\Autodesk\\3ds Max 2016\\3dsmax.exe")
set(SETUP_MAX_TERGET_3DMAX_2017_PATH "C:\\Program Files\\Autodesk\\3ds Max 2017\\3dsmax.exe")
set(SETUP_MAX_TERGET_3DMAX_2018_PATH "C:\\Program Files\\Autodesk\\3ds Max 2018\\3dsmax.exe")
set(SETUP_MAX_TERGET_3DMAX_2019_PATH "C:\\Program Files\\Autodesk\\3ds Max 2019\\3dsmax.exe")

function(add_3dmax_path project_name path)
    FILE(WRITE "${CMAKE_CURRENT_BINARY_DIR}/${project_name}.vcxproj.user"
        "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
        "<Project ToolsVersion=\"15.0\"  xmlns=\"http://schemas.microsoft.com/developer/msbuild/2003\">\n"
        "  <PropertyGroup Condition=\"'$(Configuration)|$(Platform)'=='Debug|x64'\">\n"
        "    <LocalDebuggerCommand>${path}</LocalDebuggerCommand>\n"
        "    <DebuggerFlavor>WindowsLocalDebugger</DebuggerFlavor>\n"
        "  </PropertyGroup>\n"
        "</Project>")
endfunction(add_3dmax_path)

#----------------------------------------------------------------------------------#
#//////////////////////////////////////////////////////////////////////////////////#
#----------------------------------------------------------------------------------#