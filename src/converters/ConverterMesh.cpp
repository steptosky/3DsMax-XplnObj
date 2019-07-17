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

#include "ConverterMesh.h"

#pragma warning(push, 0)
#include <max.h>
#pragma warning(pop)

#include <cassert>
#include <xpln/obj/ObjMesh.h>
#include "MeshNormalSpec.h"
#include "common/String.h"
#include "ConverterUtils.h"
#include "ExportParams.h"
#include "ImportParams.h"

/**************************************************************************************************/
///////////////////////////////////////////* Functions *////////////////////////////////////////////
/**************************************************************************************************/

INode * ConverterMesh::toMax(const xobj::ObjAbstract * inXObj, const ImportParams & params) {
    if (inXObj->objType() != xobj::OBJ_MESH) {
        return nullptr;
    }
    const xobj::ObjMesh & inXMesh = *static_cast<const xobj::ObjMesh*>(inXObj);

    const auto mobj = CreateNewTriObject();
    if (mobj == nullptr) {
        return nullptr;
    }

    const auto pnode = params.mCoreInterface->CreateObjectNode(mobj);
    if (pnode == nullptr) {
        return nullptr;
    }

    const auto numVert = inXMesh.mVertices.size();
    const auto numFace = inXMesh.mFaces.size();

    mobj->mesh.setNumVerts(static_cast<int>(numVert));
    mobj->mesh.setNumTVerts(static_cast<int>(numVert));
    mobj->mesh.setNumFaces(static_cast<int>(numFace));
    mobj->mesh.setNumTVFaces(static_cast<int>(numFace));

    int counter = 0;

    // vertex
    Point3 vert;
    for (auto & xVert : inXMesh.mVertices) {
        vert = Point3(xVert.mPosition.x, xVert.mPosition.y, xVert.mPosition.z);
        mobj->mesh.setVert(counter, vert);
        mobj->mesh.setTVert(counter, xVert.mTexture.x, xVert.mTexture.y, 0.0f);
        ++counter;
    }

    counter = 0;

    // Faces
    for (auto & xFace : inXMesh.mFaces) {
        mobj->mesh.faces[counter].setVerts(static_cast<int>(xFace.mV2),
                                           static_cast<int>(xFace.mV1),
                                           static_cast<int>(xFace.mV0));
        mobj->mesh.faces[counter].setEdgeVisFlags(1, 1, 1);
        mobj->mesh.tvFace[counter].setTVerts(static_cast<int>(xFace.mV2),
                                             static_cast<int>(xFace.mV1),
                                             static_cast<int>(xFace.mV0));
        ++counter;
    }

    counter = 0;

    // normal
    mobj->mesh.SpecifyNormals();
    MeshNormalSpec * meshNorm = mobj->mesh.GetSpecifiedNormals();
    if (meshNorm) {
        meshNorm->CheckNormals();
        for (auto & xFace : inXMesh.mFaces) {
            const auto & xVert1 = inXMesh.mVertices.at(xFace.mV2);
            Point3 p1(xVert1.mNormal.x, xVert1.mNormal.y, xVert1.mNormal.z);
            meshNorm->SetNormal(counter, 0, p1);

            const auto & xVert2 = inXMesh.mVertices.at(xFace.mV1);
            Point3 p2(xVert2.mNormal.x, xVert2.mNormal.y, xVert2.mNormal.z);
            meshNorm->SetNormal(counter, 1, p2);

            const auto & xVert3 = inXMesh.mVertices.at(xFace.mV0);
            Point3 p3(xVert3.mNormal.x, xVert3.mNormal.y, xVert3.mNormal.z);
            meshNorm->SetNormal(counter, 2, p3);
            ++counter;
        }
    }

    mobj->mesh.buildBoundingBox();
    mobj->mesh.InvalidateEdgeList();
    return pnode;
}

/**************************************************************************************************/
//////////////////////////////////////////* Functions */////////////////////////////////////////////
/**************************************************************************************************/

xobj::ObjMesh * ConverterMesh::toXpln(INode * inNode, const ExportParams & params) {
    Mesh * mesh = extractMesh(inNode, params);
    if (mesh == nullptr) {
        return nullptr;
    }

    mesh->checkNormals(true);

    FaceList faces;
    VertList vertices;

    const auto xMesh = new xobj::ObjMesh;
    xMesh->setObjectName(sts::toMbString(inNode->GetName()));

    for (int i = 0; i < mesh->numFaces; ++i) {
        addFace(faces, vertices, i, mesh);
    }

    saveVerts(vertices, xMesh, mesh);
    saveFaces(faces, xMesh);
    //-----------------------------------------------------
    // Correct position if pivot is chenged

    const auto objTm = ConverterUtils::offsetMatrix(inNode);
    if (!objTm.IsIdentity()) {
        xMesh->applyTransform(ConverterUtils::toXTMatrix(objTm));
    }
    //-----------------------------------------------------

    return xMesh;
}

/**************************************************************************************************/
///////////////////////////////////////////* Functions *////////////////////////////////////////////
/**************************************************************************************************/

bool ConverterMesh::isBone(INode * inNode, const ExportParams & params) {
    ObjectState os = inNode->EvalWorldState(params.mCurrTime);
    if (os.obj->SuperClassID() == BONE_CLASS_ID || os.obj->ClassID() == BONE_OBJ_CLASSID) {
        return true;
    }

    Object * objRef = inNode->GetObjectRef();
    if (objRef) {
        objRef = objRef->FindBaseObject();
        if (objRef) {
            if (objRef->SuperClassID() == BONE_CLASS_ID || objRef->ClassID() == BONE_OBJ_CLASSID) {
                return true;
            }
        }
    }
    return false;
}

Mesh * ConverterMesh::extractMesh(INode * inNode, const ExportParams & params) {
    assert(inNode);
    if (isBone(inNode, params)) {
        return nullptr;
    }

    ObjectState os = inNode->EvalWorldState(params.mCurrTime);
    if (os.obj->SuperClassID() == BONE_CLASS_ID || os.obj->ClassID() == BONE_OBJ_CLASSID) {
        return nullptr;
    }
    if (os.obj->SuperClassID() == SHAPE_CLASS_ID) {
        return nullptr;
    }

    TriObject * tobj = nullptr;
    if (os.obj->IsSubClassOf(triObjectClassID)) {
        tobj = static_cast<TriObject *>(os.obj);
    }
    else if (os.obj->CanConvertToType(triObjectClassID)) {
        tobj = static_cast<TriObject *>(os.obj->ConvertToType(params.mCurrTime, triObjectClassID));
    }

    if (tobj == nullptr) {
        return nullptr;
    }

    Mesh * mesh = &tobj->GetMesh();
    if (mesh == nullptr) {
        return nullptr;
    }
    return mesh;
}

//-------------------------------------------------------------------------

Point3 ConverterMesh::vertexNormal(Mesh * mesh, int faceNo, RVertex * rv) {
    Face * f = &mesh->faces[faceNo];
    Point3 outVertNormal;
    if (!rv->rFlags) {
        outVertNormal = mesh->getFaceNormal(faceNo);
    }
    else if (rv->rFlags & SPECIFIED_NORMAL) {
        outVertNormal = rv->rn.getNormal();
    }
    else {
        const int numNormals = rv->rFlags & NORCT_MASK;
        if (numNormals && f->smGroup) {
            if (numNormals == 1) {
                outVertNormal = rv->rn.getNormal();
            }
            else {
                for (int i = 0; i < numNormals; i++) {
                    if (rv->ern[i].getSmGroup() & f->smGroup) {
                        outVertNormal = rv->ern[i].getNormal();
                    }
                }
            }
        }
        else {
            outVertNormal = mesh->getFaceNormal(faceNo);
        }
    }
    return outVertNormal;
}

//-------------------------------------------------------------------------

size_t ConverterMesh::idx(VertList & vertices, const MPoint & inMPoint) {
    const auto num = vertices.size();
    for (std::size_t i = 0; i < num; ++i) {
        if (vertices[i].vertexIdx == inMPoint.vertexIdx &&
            vertices[i].TextureVertexIdx == inMPoint.TextureVertexIdx &&
            vertices[i].normal == inMPoint.normal) {
            return i;
        }
    }
    vertices.emplace_back(inMPoint);
    return num;
}

//-------------------------------------------------------------------------

void ConverterMesh::addFace(FaceList & faces, VertList & vertices, int inFaceIdx, Mesh * inMesh) {
    MFace mf;
    Face * f = &inMesh->faces[inFaceIdx];
    TVFace * tvFace = &inMesh->tvFace[inFaceIdx];
    const auto numtv = inMesh->getNumTVerts();
    mf.smGroupIdx = f->smGroup;
    mf.materialIdx = f->getMatID();
    for (int i = 0; i < 3; ++i) {
        if (tvFace && numtv != 0) {
            MPoint p(f->v[i], tvFace->t[i]);
            p.normal = vertexNormal(inMesh, inFaceIdx, &inMesh->getRVert(f->v[i]));
            mf.vertices[i] = static_cast<int>(idx(vertices, p));
        }
        else {
            MPoint p(f->v[i], -1);
            p.normal = vertexNormal(inMesh, inFaceIdx, &inMesh->getRVert(f->v[i]));
            mf.vertices[i] = static_cast<int>(idx(vertices, p));
        }
    }
    faces.emplace_back(mf);
}

//-------------------------------------------------------------------------

void ConverterMesh::saveFaces(FaceList & faces, xobj::ObjMesh * inXMesh) {
    xobj::ObjMesh::FaceList fList;
    const auto numF = faces.size();
    for (std::size_t i = 0; i < numF; ++i) {
        const MFace & f = faces[i];
        fList.emplace_back(xobj::ObjMesh::Face(f.vertices[2], f.vertices[1], f.vertices[0]));
    }
    inXMesh->mFaces.swap(fList);
}

//-------------------------------------------------------------------------

void ConverterMesh::saveVerts(VertList & vertices, xobj::ObjMesh * inXMesh, Mesh * inMesh) {
    xobj::ObjMesh::VertexList vList;
    const auto numv = vertices.size();
    UVVert UVzero(0, 0, 0);
    for (std::size_t i = 0; i < numv; ++i) {
        MPoint & point = vertices[i];
        const Point3 mp = inMesh->verts[point.vertexIdx];
        const Point3 nn = point.normal;
        const UVVert uv = point.TextureVertexIdx == -1 ? UVzero : inMesh->tVerts[point.TextureVertexIdx];
        vList.emplace_back(xobj::ObjMesh::Vertex(xobj::Point3(mp.x, mp.y, mp.z),
                                                 xobj::Point3(nn.x, nn.y, nn.z),
                                                 xobj::Point2(uv.x, uv.y)));
    }
    inXMesh->mVertices.swap(vList);
}

/**************************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////
/**************************************************************************************************/
