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

#pragma warning(push, 0)
#include <max.h>
#pragma warning(pop)

#include <xpln/obj/ObjMesh.h>

/**************************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////
/**************************************************************************************************/

class ConverterMesh {
public:

    static INode * toMax(const xobj::ObjAbstract * xObj);
    static xobj::ObjMesh * toXpln(INode * node);

    static Mesh * extractMesh(INode * inNode);

    ConverterMesh() = default;
    ~ConverterMesh() = default;

private:

    class MPoint {
    public:

        int vertexIdx;
        int TextureVertexIdx;
        Point3 normal;

        MPoint(int inVertextIdx, int inTextureVertexIdx) {
            vertexIdx = inVertextIdx;
            TextureVertexIdx = inTextureVertexIdx;
        }

        ~MPoint() { }
    };

    struct MFace {

        int vertices[3];
        int smGroupIdx;
        int materialIdx;

    };

    typedef std::vector<MFace> FaceList;
    typedef std::vector<MPoint> VertList;

    static Point3 vertexNormal(Mesh * mesh, int faceNo, RVertex * rv);
    static void addFace(FaceList & faces, VertList & vertices, int inFaceIdx, Mesh * inMesh);
    static size_t idx(VertList & vertices, const MPoint & p);
    static void saveVerts(VertList & vertices, xobj::ObjMesh * inXMesh, Mesh * inMesh);
    static void saveFaces(FaceList & faces, xobj::ObjMesh * objMesh);

    static bool isBone(INode * node);

};

/**************************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////
/**************************************************************************************************/
