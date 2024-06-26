//
// Created by bhjd1010 on 2024/6/20.
//

#include "stl_importer.h"
#include <RWStl.hxx>

using namespace std;

std::vector<MeshData> StlImporter::Import(const std::string &path) {
    std::vector<MeshData> meshDataArr;
    NCollection_Sequence<Handle(Poly_Triangulation) > triangleList;
    RWStl::ReadFile(path.c_str(), M_PI / 2.0, triangleList);
    for (auto &mesh: triangleList) {
        if (!mesh) {
            continue;
        }
        MeshData meshData;
        meshData.indices.reserve(mesh->NbTriangles() * 3);
        meshData.points.reserve(mesh->NbNodes());
        for (int i = 1; i <= mesh->NbTriangles(); ++i) {
            const auto &tri = mesh->Triangle(i);
            int _indices[3];
            tri.Get(_indices[0], _indices[1], _indices[2]);
            meshData.indices.push_back(_indices[0] - 1);
            meshData.indices.push_back(_indices[1] - 1);
            meshData.indices.push_back(_indices[2] - 1);
        }
        for (int i = 1; i <= mesh->NbNodes(); ++i) {
            auto node = mesh->Node(i);
            file_importer::Pt3d Point;
            Point.set_x(node.X());
            Point.set_y(node.Y());
            Point.set_z(node.Z());
            meshData.points.emplace_back(std::move(Point));
        }
        if (mesh->HasNormals()) {
            meshData.normals.reserve(mesh->NbNodes());
            for (int i = 1; i <= mesh->NbNodes(); ++i) {
                auto normal = mesh->Normal(i);
                file_importer::Pt3d Point;
                Point.set_x(normal.X());
                Point.set_y(normal.Y());
                Point.set_z(normal.Z());
                meshData.normals.emplace_back(std::move(Point));
            }
        }
        if (mesh->HasUVNodes()) {
            meshData.uvs.reserve(mesh->NbNodes());
            for (int i = 1; i <= mesh->NbNodes(); ++i) {
                auto uv = mesh->UVNode(i);
                file_importer::Pt2d Point;
                Point.set_x(uv.X());
                Point.set_y(uv.Y());
                meshData.uvs.emplace_back(std::move(Point));
            }
        }
        meshDataArr.emplace_back(std::move(meshData));
    }
    return meshDataArr;
}
