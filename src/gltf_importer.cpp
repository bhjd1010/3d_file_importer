//
// Created by bhjd1010 on 2024/6/20.
//
#include <iostream>
#include <optional>
#include "gltf_importer.h"
#include <BRepMesh_IncrementalMesh.hxx>
#include <BRep_Tool.hxx>
#include <STEPControl_Reader.hxx>
#include <TopExp_Explorer.hxx>
#include <TopoDS.hxx>
#include <BRepAdaptor_Surface.hxx>
#include <TDocStd_Document.hxx>
#include <RWGltf_CafReader.hxx>
#include "XCAFDoc_DocumentTool.hxx"
#include <TopExp.hxx>
using namespace std;

std::vector<MeshData> GltfImporter::Import(const std::string &path) {
    std::vector<MeshData> meshDataArr;
    auto doc = new TDocStd_Document("gltf_doc");
    auto gltf_reader = RWGltf_CafReader();
    gltf_reader.SetDocument(doc);
//gltf_reader.SetParallel(True)
//gltf_reader.SetDoublePrecision(True)
//gltf_reader.SetToSkipLateDataLoading(True)
//gltf_reader.SetToKeepLateData(True)
//gltf_reader.SetToPrintDebugMessages(True)
//gltf_reader.SetLoadAllScenes(True)
    auto status = gltf_reader.Perform(path.c_str(), Message_ProgressRange());

    if(status != IFSelect_RetDone){
        return meshDataArr;
    }
    auto shp = gltf_reader.SingleShape();
    std::vector<TopoDS_Shape> shapes;
    AddShape(shp, shapes);
    for (auto &shape: shapes) {
        meshDataArr.emplace_back(GetShapeMeshData(shape));
    }
    return meshDataArr;
}
