//
// Created by bhjd1010 on 2024/6/20.
//
#pragma once

#include "file_types.h"
#include <memory>

class IFileImporter;

class FileImporterFactory {
public:
    std::unique_ptr<IFileImporter> CreateFileImporter(EFileTypes type);

private:
};
