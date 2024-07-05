//
// Created by bhjd1010 on 2024/6/20.
//

#include "occ_importer.h"
#include <RWStl.hxx>
#include <TopExp_Explorer.hxx>
#include <BRepMesh_IncrementalMesh.hxx>
#include <BRepAdaptor_Surface.hxx>
#include <TopoDS.hxx>
#include <BRep_Tool.hxx>
#include "optional"

using namespace std;

void OccImporter::AddShape(const TopoDS_Shape &inShape, vector<TopoDS_Shape> &outShapes) {
    if (inShape.IsNull())
        return;
    if (inShape.ShapeType() == TopAbs_ShapeEnum::TopAbs_COMPOUND) {
        TopExp_Explorer aSolidExplorer(inShape, TopAbs_SOLID);
        bool shapeOrShell = false;
        while (aSolidExplorer.More()) {
            shapeOrShell = true;
            const auto& child = aSolidExplorer.Current();
            AddShape(child, outShapes);
            aSolidExplorer.Next();
        }
        TopExp_Explorer aShellExplorer(inShape, TopAbs_SHELL);
        while (aShellExplorer.More()) {
            shapeOrShell = true;
            const auto& child = aShellExplorer.Current();
            AddShape(child, outShapes);
            aShellExplorer.Next();
        }
        if(!shapeOrShell){
            outShapes.push_back(inShape);
        }
    } else {
        outShapes.push_back(inShape);
    }
}

MeshData OccImporter::GetShapeMeshData(const TopoDS_Shape &shape) {
    MeshData shapeMeshData;
    BRepMesh_IncrementalMesh mesh(shape, 0.1);
    if (!mesh.IsDone()) {
        cerr << "shape triangulation failed" << endl;
        return shapeMeshData;
    }
    for (TopExp_Explorer ex(shape, TopAbs_FACE); ex.More(); ex.Next()) {
        OccMeshData faceMeshData;
        const TopoDS_Face &face = TopoDS::Face(ex.Current());
//        BRepAdaptor_Surface BASur(face);
//        GeomAbs_SurfaceType type = BASur.GetType();
//        if (type != GeomAbs_Plane) {
//            cerr << "face is not plane type, is: " << type << endl;
//        }
        auto faceOrientation = face.Orientation();
        TopLoc_Location location;
        Handle_Poly_Triangulation triangulation =
                BRep_Tool::Triangulation(face, location);
        std::optional<gp_Trsf> trans = std::nullopt;
        if (!location.IsIdentity()) {
            trans = location.Transformation();
        }
        faceMeshData.points.reserve(triangulation->NbNodes());
        faceMeshData.normals.reserve(triangulation->NbNodes());
        for (int i = 1; i <= triangulation->NbNodes(); ++i) {
            faceMeshData.points.emplace_back(triangulation->Node(i));
            faceMeshData.normals.emplace_back();
        }
        int startPos = static_cast<int>(shapeMeshData.points.size()) - 1;
        for (int i = 1; i <= triangulation->NbTriangles(); ++i) {
            const Poly_Triangle &triangle = triangulation->Triangle(i);
            Standard_Integer index1, index2, index3;
            triangle.Get(index1, index2, index3);
            if (faceOrientation != TopAbs_Orientation::TopAbs_FORWARD) {
                std::swap(index2, index3);
            }
            const gp_Pnt &node0 = triangulation->Node(index1);
            const gp_Pnt &node1 = triangulation->Node(index2);
            const gp_Pnt &node2 = triangulation->Node(index3);
            shapeMeshData.indices.push_back(index1 + startPos);
            shapeMeshData.indices.push_back(index2 + startPos);
            shapeMeshData.indices.push_back(index3 + startPos);
            auto &normal0 = faceMeshData.normals[index1 - 1];
            auto &normal1 = faceMeshData.normals[index2 - 1];
            auto &normal2 = faceMeshData.normals[index3 - 1];
            gp_Vec normal = gp_Vec(node0, node1).Crossed(gp_Vec(node0, node2));
            Standard_Real aD = normal.SquareMagnitude();
            if (aD > gp::Resolution()) {
                if(!IsEqual(aD, 1))
                    normal.Normalize();
                normal0 = normal;
                normal1 = normal;
                normal2 = normal;
            }
        }
        if (triangulation->HasUVNodes()) {
            for (int i = 1; i <= triangulation->NbNodes(); ++i) {
                const gp_Pnt2d &uv = triangulation->UVNode(i);
                file_importer::Pt2d Point2;
                Point2.set_x(uv.X());
                Point2.set_y(uv.Y());
                shapeMeshData.uvs.emplace_back(std::move(Point2));
            }
        }

        // transform face mesh data
        shapeMeshData.points.reserve(shapeMeshData.points.size() + faceMeshData.points.size());
        for (auto &pt: faceMeshData.points) {
            if (trans.has_value()) {
                pt.Transform(trans.value());
            }
            file_importer::Pt3d Point;
            Point.set_x(pt.X());
            Point.set_y(pt.Y());
            Point.set_z(pt.Z());
            shapeMeshData.points.emplace_back(std::move(Point));
        }
        shapeMeshData.normals.reserve(shapeMeshData.normals.size() + faceMeshData.normals.size());
        for (auto &dir: faceMeshData.normals) {
            if (trans.has_value()) {
                dir.Transform(trans.value());
            }
            file_importer::Pt3d Point;
            Point.set_x(dir.X());
            Point.set_y(dir.Y());
            Point.set_z(dir.Z());
            shapeMeshData.normals.emplace_back(std::move(Point));
        }
    }
    return shapeMeshData;
}
