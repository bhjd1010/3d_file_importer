//
// Created by bhjd1010 on 2024/6/20.
//

#pragma once

#include "file_importer.pb.h"
#include <vector>
#include <string>

struct MeshData {
    std::vector<file_importer::Pt3d> points;
    std::vector<file_importer::Pt2d> uvs;
    std::vector<file_importer::Pt3d> normals;
    std::vector<int> indices;
};

class IFileImporter {
public:
    virtual ~IFileImporter() = default;

    virtual std::vector<MeshData> Import(const std::string &path) = 0;
};
