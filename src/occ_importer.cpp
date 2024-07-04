//
// Created by bhjd1010 on 2024/6/20.
//

#include "occ_importer.h"
#include <RWStl.hxx>
#include <TopExp_Explorer.hxx>

using namespace std;

void OccImporter::AddShape(const TopoDS_Shape &inShape, vector<TopoDS_Shape> &outShapes) {
    if (inShape.IsNull())
        return;
    if (inShape.ShapeType() == TopAbs_ShapeEnum::TopAbs_COMPOUND) {
        TopExp_Explorer aSolidExplorer(inShape, TopAbs_SOLID);
        while (aSolidExplorer.More()) {
            auto child = aSolidExplorer.Current();
            AddShape(child, outShapes);
            aSolidExplorer.Next();
        }
    } else {
        outShapes.push_back(inShape);
    }
}