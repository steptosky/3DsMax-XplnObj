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

#include "Mesh2Cpp.h"
#include <fstream>
#include "converters/ConverterMesh.h"
#include "converters/ExportParams.h"
#include "additional/utils/StringUtils.h"

/**************************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////
/**************************************************************************************************/

void Mesh2Cpp::saveSelected(const std::string & folderPath, const std::string & name) {
    ExportParams params;
    if (params.mCoreInterface->GetSelNodeCount() == 0) {
        return;
    }
    INode * node = params.mCoreInterface->GetSelNode(0);
    Mesh * m = ConverterMesh::extractMesh(node, params);
    if (m) {
        save(*m, name.empty() ? sts::toMbString(node->GetName()) : name, folderPath);
    }
}

void Mesh2Cpp::save(const Mesh & mesh, const std::string & name, const std::string & folderPath) {
    writeHeader(name, sts::MbStrUtils::joinStr(folderPath, "/", name, "-gen.h"));
    writeCpp(mesh, name, sts::MbStrUtils::joinStr(folderPath, "/", name, "-gen.cpp"));
}

/**************************************************************************************************/
//////////////////////////////////////////* Functions */////////////////////////////////////////////
/**************************************************************************************************/

void Mesh2Cpp::writeHeader(const std::string & name, const std::string & filePath) {
    std::ofstream out(filePath);
    if (out) {
        out << std::fixed;
        out.precision(6);

        out << "#pragma once" << std::endl;
        out << std::endl;
        out << "/* Auto-generated file */" << std::endl;
        out << std::endl;
        out << "class Mesh;" << std::endl;
        out << std::endl;
        out << "class " << name << " { " << std::endl;
        out << "public:" << std::endl;
        out << std::endl;
        out << "    static void fillMesh(Mesh & outMesh, float scale);" << std::endl;
        out << std::endl;
        out << "};" << std::endl;
    }
    out.close();
}

void Mesh2Cpp::writeCpp(const Mesh & mesh, const std::string & name, const std::string & filePath) {
    std::ofstream out(filePath);
    if (out) {
        out << std::fixed;
        out.precision(6);

        out << "#include \"" << name << "-gen.h\"" << std::endl;
        out << std::endl;
        out << "#pragma warning(push, 0)" << std::endl;
        out << "#include <max.h>" << std::endl;
        out << "#pragma warning(pop)" << std::endl;
        out << std::endl;
        out << "/* Auto-generated file */" << std::endl;
        out << std::endl;
        out << "void " << name << "::fillMesh(Mesh & outMesh, const float scale) {" << std::endl;
        out << std::endl;
        out << "\toutMesh.setNumVerts(" << mesh.numVerts << ");" << std::endl;
        out << "\toutMesh.setNumFaces(" << mesh.numFaces << ");" << std::endl;
        out << std::endl;

        for (size_t i = 0; i < mesh.numVerts; ++i) {
            out << "\toutMesh.verts[" << i << "].x = scale * " << mesh.verts[i].x << "f;" << std::endl;
            out << "\toutMesh.verts[" << i << "].y = scale * " << mesh.verts[i].y << "f;" << std::endl;
            out << "\toutMesh.verts[" << i << "].z = scale * " << mesh.verts[i].z << "f;" << std::endl;
        }

        for (size_t i = 0; i < mesh.numFaces; ++i) {
            out << "\toutMesh.faces[" << i << "].setVerts("
                    << mesh.faces[i].getVert(0) << ", "
                    << mesh.faces[i].getVert(1) << ", "
                    << mesh.faces[i].getVert(2) << ");"
                    << std::endl;

            out << "\toutMesh.faces[" << i << "].setEdgeVisFlags("
                    << mesh.faces[i].getEdgeVis(0) << ", "
                    << mesh.faces[i].getEdgeVis(1) << ", "
                    << mesh.faces[i].getEdgeVis(2) << ");"
                    << std::endl;
        }
        out << std::endl;
        out << "\toutMesh.InvalidateGeomCache();" << std::endl;
        out << "}" << std::endl;
    }
    out.close();
}

/**************************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////
/**************************************************************************************************/
