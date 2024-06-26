//
// Created by bhjd1010 on 2024/6/20.
//

#include "step_importer.h"
#include <BRepMesh_IncrementalMesh.hxx>
#include <BRep_Tool.hxx>
#include <STEPControl_Reader.hxx>
#include <TopExp_Explorer.hxx>
#include <TopoDS.hxx>
#include <BRepAdaptor_Surface.hxx>

using namespace std;

std::vector<MeshData> StepImporter::Import(const std::string &path) {
    std::vector<MeshData> meshDataArr;
    STEPControl_Reader readerBase;
    auto readResult = readerBase.ReadFile(path.c_str());
    readerBase.TransferRoots();
    if (readResult != IFSelect_ReturnStatus::IFSelect_RetDone) {
        return meshDataArr;
    }
    for(int shapeIdx = 1; shapeIdx <= readerBase.NbShapes(); ++shapeIdx){
        MeshData meshData;
        auto shape = readerBase.Shape(shapeIdx);
        if (shape.IsNull()) {
            continue;
        }
        BRepMesh_IncrementalMesh mesh(shape, 0.1);
        if (!mesh.IsDone()) {
            continue;
        }
        for (TopExp_Explorer ex(shape, TopAbs_FACE); ex.More(); ex.Next()) {
            const TopoDS_Face &face = TopoDS::Face(ex.Current());
            auto faceOrientation = face.Orientation();
            TopLoc_Location location;
            Handle_Poly_Triangulation triangulation =
                    BRep_Tool::Triangulation(face, location);
            int startPos = static_cast<int>(meshData.points.size()) - 1;
            for (int i = 1; i <= triangulation->NbNodes(); ++i) {
                const gp_Pnt &node = triangulation->Node(i);
                file_importer::Pt3d Point;
                Point.set_x(node.X());
                Point.set_y(node.Y());
                Point.set_z(node.Z());
                meshData.points.emplace_back(std::move(Point));
                meshData.normals.emplace_back();
            }
            for (int i = 1; i <= triangulation->NbTriangles(); ++i) {
                const Poly_Triangle &triangle = triangulation->Triangle(i);
                Standard_Integer index1, index2, index3;
                triangle.Get(index1, index2, index3);
                if(faceOrientation != TopAbs_Orientation::TopAbs_FORWARD){
                    std::swap(index2, index3);
                }
                const gp_Pnt &node0 = triangulation->Node(index1);
                const gp_Pnt &node1 = triangulation->Node(index2);
                const gp_Pnt &node2 = triangulation->Node(index3);
                index1 += startPos;
                index2 += startPos;
                index3 += startPos;
                meshData.indices.push_back(index1);
                meshData.indices.push_back(index2);
                meshData.indices.push_back(index3);
                auto &normal0 = meshData.normals[index1];
                auto &normal1 = meshData.normals[index2];
                auto &normal2 = meshData.normals[index3];
                gp_Vec normal = gp_Vec(node0, node1).Crossed(gp_Vec(node0, node2)).Normalized();
                normal0.set_x(normal.X());
                normal0.set_y(normal.Y());
                normal0.set_z(normal.Z());
                normal1.set_x(normal.X());
                normal1.set_y(normal.Y());
                normal1.set_z(normal.Z());
                normal2.set_x(normal.X());
                normal2.set_y(normal.Y());
                normal2.set_z(normal.Z());
            }
            if (triangulation->HasUVNodes()) {
                for (int i = 1; i <= triangulation->NbNodes(); ++i) {
                    const gp_Pnt2d &uv = triangulation->UVNode(i);
                    file_importer::Pt2d Point2;
                    Point2.set_x(uv.X());
                    Point2.set_y(uv.Y());
                    meshData.uvs.emplace_back(std::move(Point2));
                }
            }
        }
        meshDataArr.emplace_back(std::move(meshData));
    }
    return meshDataArr;
}
