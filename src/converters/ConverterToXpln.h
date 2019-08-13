/*
**  Copyright(C) 2017, StepToSky
**
**  Redistribution and use in source and binary forms, with or without
**  modification, are permitted provided that the following conditions are met:
**
**  1.Redistributions of source code must retain the above copyright notice, this
**    list of conditions and the following disclaimer.
**  2.Redistributions in binary form must reproduce the above copyright notice,
**    this list of conditions and the following disclaimer in the documentation
**    and / or other materials provided with the distribution.
**  3.Neither the name of StepToSky nor the names of its contributors
**    may be used to endorse or promote products derived from this software
**    without specific prior written permission.
**
**  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
**  ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
**  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
**  DISCLAIMED.IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
**  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
**  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
**  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
**  ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
**  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
**  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
**
**  Contacts: www.steptosky.com
*/

#pragma once

#include <vector>
#include <unordered_set>

#include "ExportParams.h"

namespace xobj {
class ObjAbstract;
class ObjMain;
class Transform;
}

class MainObjParamsWrapper;

class INode;
class ILayer;
class Interface;
class Matrix3;
class StdMat;
class RawExpOption;
class Log;

class ConverterMesh;
class ConverterLine;
class ConverterDummy;
class ConverterLight;
class ConverterAttr;
class ConverterAnim;

/**************************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////
/**************************************************************************************************/

class ConverterToXpln {
public:

    ConverterToXpln();
    ~ConverterToXpln() = default;

    bool toXpln(MainObjParamsWrapper * mainNode, xobj::ObjMain & xObjMain);

private:

    bool byLayer(MainObjParamsWrapper * mainNode, const std::unordered_set<ILayer*> & layers, xobj::ObjMain & xObjMain);
    bool byLink(MainObjParamsWrapper * mainNode, xobj::ObjMain & xObjMain);

    //-------------------------------------------------------------------------

    bool processNode(INode * node, xobj::Transform * xTransform,
                     const ExportParams & params, const std::unordered_set<ILayer*> & layers) const;

    typedef std::vector<xobj::ObjAbstract*> ObjAbstractList;
    ObjAbstractList xplnObj(INode * inNode, const ExportParams & params) const;

    static bool collectLods(INode * ownerNode, INode * currNode, std::vector<INode*> & outLods);
    static std::unordered_set<INode*> rootNodesFromLayers(const std::unordered_set<ILayer*> & layers);
    static ILayer * layerFromNode(INode * node);

    //-------------------------------------------------------------------------

    ExportParams mExportOptions;
    MainObjParamsWrapper * mMainObj;
    xobj::ObjMain * mXObjMain;
    std::vector<INode *> mLods;

    //-------------------------------------------------------------------------

};

/**************************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////
/**************************************************************************************************/
