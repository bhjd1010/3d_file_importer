//
// Created by bhjd1010 on 2024/6/20.
//

#pragma once

#include "file_importer.h"
#include <string>
#include <TopoDS_Shape.hxx>

class OccImporter : public IFileImporter {
protected:
    void AddShape(const TopoDS_Shape& inShape, std::vector<TopoDS_Shape>& outShapes);
};

