//
// Created by bhjd1010 on 2024/6/20.
//

#pragma once

#include "occ_importer.h"
#include <string>
#include <TopoDS_Shape.hxx>

class StepImporter : public OccImporter {
public:
    std::vector<MeshData> Import(const std::string &path) override;

};
