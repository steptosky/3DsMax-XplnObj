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

namespace xobj {
class ObjAbstract;
class ObjMain;
class Transform;
}

class MainObjParamsWrapper;

class INode;
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

class ExportParams;
class ImportParams;

/**************************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////
/**************************************************************************************************/

class Converterer {
public:

    bool toXpln(MainObjParamsWrapper * mainNode, xobj::ObjMain & xObjMain);
    static bool toMax(xobj::ObjMain & xObjMain);

    Converterer();
    ~Converterer();
private:

    //-------------------------------------------------------------------------

    static bool processXTransformHierarchy(INode * parent, xobj::Transform * xTransform, const ImportParams & params);
    static void processXTransformObjects(INode * parent, xobj::Transform * xTransform, const ImportParams & params);
    static INode * processXObjects(const xobj::ObjAbstract & xObj, const ImportParams & params);

    //-------------------------------------------------------------------------

    bool processNode(INode * node, xobj::Transform * xTransform, const ExportParams & params) const;

    typedef std::vector<xobj::ObjAbstract*> ObjAbstractList;
    void toXpln(INode * inxNode, const Matrix3 & baseTm,
                ObjAbstractList & outList, const ExportParams & params) const;

    static bool collectLods(INode * ownerNode, INode * currNode, std::vector<INode*> & outLods);

    //-------------------------------------------------------------------------

    MainObjParamsWrapper * mMainObj;
    xobj::ObjMain * mXObjMain;
    Interface * mIp;
    std::vector<INode *> mLods;

    //-------------------------------------------------------------------------

};

/**************************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////
/**************************************************************************************************/
