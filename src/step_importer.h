//
// Created by bhjd1010 on 2024/6/20.
//

#pragma once

#include "file_importer.h"
#include <string>
#include <TopoDS_Shape.hxx>

class StepImporter : public IFileImporter {
public:
    std::vector<MeshData> Import(const std::string &path) override;

private:
    void AddShape(const TopoDS_Shape& inShape, std::vector<TopoDS_Shape>& outShapes);

};
