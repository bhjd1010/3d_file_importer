//
// Created by bhjd1010 on 2024/6/20.
//
#include <iostream>
#include <optional>
#include "step_importer.h"
#include <BRepMesh_IncrementalMesh.hxx>
#include <BRep_Tool.hxx>
#include <STEPControl_Reader.hxx>
#include <TopExp_Explorer.hxx>
#include <TopoDS.hxx>
#include <BRepAdaptor_Surface.hxx>
#include "XCAFDoc_DocumentTool.hxx"

#include "timer/PerformanceTimer.h"
#include "spdlog/spdlog.h"
using namespace std;

std::vector<MeshData> StepImporter::Import(const std::string &path) {
    std::vector<MeshData> meshDataArr;
    STEPControl_Reader stepReader;
    auto readResult = stepReader.ReadFile(path.c_str());
    stepReader.TransferRoots();
    if (readResult != IFSelect_ReturnStatus::IFSelect_RetDone) {
        return meshDataArr;
    }
    std::vector<TopoDS_Shape> shapes;
    for (int shapeIdx = 1; shapeIdx <= stepReader.NbShapes(); ++shapeIdx) {
        auto shape = stepReader.Shape(shapeIdx);
        AddShape(shape, shapes);
    }
    PerformanceTimer getShapeMeshDataTimer(true);
    for (auto &shape: shapes) {
        meshDataArr.emplace_back(GetShapeMeshData(shape));
    }
    getShapeMeshDataTimer.End();
    spdlog::info("file: {} get shape mesh data spend: {}", path, getShapeMeshDataTimer.Duration());
    return meshDataArr;
}
