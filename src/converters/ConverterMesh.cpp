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
#include "MeshNormalSpec.h"
#include "common/String.h"
#include <cassert>
#include "ConverterUtils.h"

/**************************************************************************************************/
///////////////////////////////////////////* Functions *////////////////////////////////////////////
/**************************************************************************************************/

INode * ConverterMesh::toMax(const xobj::ObjAbstract * inXObj) {
    if (inXObj->objType() != xobj::OBJ_MESH) {
        return nullptr;
    }
    const xobj::ObjMesh & inXMesh = *static_cast<const xobj::ObjMesh*>(inXObj);

    TriObject * mobj = CreateNewTriObject();
    if (mobj == nullptr) {
        return nullptr;
    }

    INode * pnode = GetCOREInterface()->CreateObjectNode(mobj);
    if (pnode == nullptr) {
        return nullptr;
    }

    size_t numVert = inXMesh.pVertices.size();
    size_t numFace = inXMesh.pFaces.size();

    mobj->mesh.setNumVerts(static_cast<int>(numVert));
    mobj->mesh.setNumTVerts(static_cast<int>(numVert));
    mobj->mesh.setNumFaces(static_cast<int>(numFace));
    mobj->mesh.setNumTVFaces(static_cast<int>(numFace));

    int counter = 0;

    // vertex
    Point3 vert;
    for (auto & xVert : inXMesh.pVertices) {
        vert = Point3(xVert.pPosition.x, xVert.pPosition.y, xVert.pPosition.z);
        mobj->mesh.setVert(counter, vert);
        mobj->mesh.setTVert(counter, xVert.pTexture.x, xVert.pTexture.y, 0.0f);
        ++counter;
    }

    counter = 0;

    // Faces
    for (auto & xFace : inXMesh.pFaces) {
        mobj->mesh.faces[counter].setVerts(static_cast<int>(xFace.pV2),
                                           static_cast<int>(xFace.pV1),
                                           static_cast<int>(xFace.pV0));
        mobj->mesh.faces[counter].setEdgeVisFlags(1, 1, 1);
        mobj->mesh.tvFace[counter].setTVerts(static_cast<int>(xFace.pV2),
                                             static_cast<int>(xFace.pV1),
                                             static_cast<int>(xFace.pV0));
        ++counter;
    }

    counter = 0;

    // normal
    mobj->mesh.SpecifyNormals();
    MeshNormalSpec * meshNorm = mobj->mesh.GetSpecifiedNormals();
    if (meshNorm) {
        meshNorm->CheckNormals();
        for (auto & xFace : inXMesh.pFaces) {
            auto & xVert1 = inXMesh.pVertices.at(xFace.pV2);
            Point3 p1(xVert1.pNormal.x, xVert1.pNormal.y, xVert1.pNormal.z);
            meshNorm->SetNormal(counter, 0, p1);

            auto & xVert2 = inXMesh.pVertices.at(xFace.pV1);
            Point3 p2(xVert2.pNormal.x, xVert2.pNormal.y, xVert2.pNormal.z);
            meshNorm->SetNormal(counter, 1, p2);

            auto & xVert3 = inXMesh.pVertices.at(xFace.pV0);
            Point3 p3(xVert3.pNormal.x, xVert3.pNormal.y, xVert3.pNormal.z);
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

xobj::ObjMesh * ConverterMesh::toXpln(INode * inNode) {
    Mesh * mesh = extractMesh(inNode);
    if (mesh == nullptr) {
        return nullptr;
    }

    mesh->checkNormals(true);

    FaceList faces;
    VertList vertices;

    xobj::ObjMesh * xMesh = new xobj::ObjMesh;
    xMesh->setObjectName(sts::toMbString(inNode->GetName()));

    for (int i = 0; i < mesh->numFaces; ++i) {
        addFace(faces, vertices, i, mesh);
    }

    saveVerts(vertices, xMesh, mesh);
    saveFaces(faces, xMesh);
    //-----------------------------------------------------
    // Correct position if pivot is chenged

    Matrix3 objTm = ConverterUtils::offsetMatrix(inNode);
    if (!objTm.IsIdentity()) {
        xMesh->applyTransform(ConverterUtils::toXTMatrix(objTm));
    }
    //-----------------------------------------------------

    return xMesh;
}

/**************************************************************************************************/
///////////////////////////////////////////* Functions *////////////////////////////////////////////
/**************************************************************************************************/

bool ConverterMesh::isBone(INode * inNode) {
    ObjectState os = inNode->EvalWorldState(GetCOREInterface()->GetTime());
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

Mesh * ConverterMesh::extractMesh(INode * inNode) {
    assert(inNode);
    if (isBone(inNode)) {
        return nullptr;
    }

    ObjectState os = inNode->EvalWorldState(GetCOREInterface()->GetTime());
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
        tobj = static_cast<TriObject *>(os.obj->ConvertToType(GetCOREInterface()->GetTime(), triObjectClassID));
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
        int numNormals = rv->rFlags & NORCT_MASK;
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
    size_t num = vertices.size();
    for (size_t i = 0; i < num; ++i) {
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
    int numtv = inMesh->getNumTVerts();
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
    size_t numf = faces.size();
    for (int i = 0; i < numf; ++i) {
        MFace & f = faces[i];
        fList.emplace_back(xobj::ObjMesh::Face(f.vertices[2], f.vertices[1], f.vertices[0]));
    }
    inXMesh->pFaces.swap(fList);
}

//-------------------------------------------------------------------------

void ConverterMesh::saveVerts(VertList & vertices, xobj::ObjMesh * inXMesh, Mesh * inMesh) {
    xobj::ObjMesh::VertexList vList;
    size_t numv = vertices.size();
    UVVert UVzero(0, 0, 0);
    for (int i = 0; i < numv; ++i) {
        MPoint & point = vertices[i];
        Point3 mp = inMesh->verts[point.vertexIdx];
        Point3 nn = point.normal;
        UVVert uv = point.TextureVertexIdx == -1 ? UVzero : inMesh->tVerts[point.TextureVertexIdx];
        vList.emplace_back(xobj::ObjMesh::Vertex(xobj::Point3(mp.x, mp.y, mp.z),
                                                 xobj::Point3(nn.x, nn.y, nn.z),
                                                 xobj::Point2(uv.x, uv.y)));
    }
    inXMesh->pVertices.swap(vList);
}

/**************************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////
/**************************************************************************************************/
