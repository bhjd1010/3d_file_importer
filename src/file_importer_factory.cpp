//
// Created by bhjd1010 on 2024/6/20.
//

#include "file_importer_factory.h"
#include "step_importer.h"
#include "stl_importer.h"
#include "obj_importer.h"
#include "gltf_importer.h"

using namespace std;

std::unique_ptr<IFileImporter>
FileImporterFactory::CreateFileImporter(EFileTypes type) {
    switch (type) {
        case EFileTypes::Step:
            return make_unique<StepImporter>();
        case EFileTypes::Stl:
            return make_unique<StlImporter>();
        case EFileTypes::Obj:
            return make_unique<ObjImporter>();
        case EFileTypes::Gltf:
            return make_unique<GltfImporter>();
        default:
            return nullptr;
    }
}
