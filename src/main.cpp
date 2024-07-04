

#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <filesystem>

#include <grpcpp/ext/proto_server_reflection_plugin.h>
#include <grpcpp/grpcpp.h>
#include <grpcpp/health_check_service_interface.h>

#include "file_importer.grpc.pb.h"
#include "file_importer_factory.h"
#include "file_importer.h"

using file_importer::FileImporter;
using file_importer::ImportFileRequest;
using file_importer::ImportFileResponse;
using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;

class FileImporterServiceImpl final : public FileImporter::Service {
    Status Import3dFile(ServerContext *context, const ImportFileRequest *request,
                        ImportFileResponse *response) override {
        fileImportFactory = std::make_unique<FileImporterFactory>();
        std::filesystem::path filePath(request->filepath());
        auto extension = filePath.extension();
        std::vector<MeshData> meshDataArr;
        if (extension.string() == ".step" || extension.string() == ".STEP" || extension.string() == ".stp" || extension.string() == ".STP") {
            meshDataArr = fileImportFactory->CreateFileImporter(EFileTypes::Step)->Import(request->filepath());
        } else if (extension.string() == ".stl" || extension.string() == ".STL") {
            meshDataArr = fileImportFactory->CreateFileImporter(EFileTypes::Stl)->Import(request->filepath());
        } else if (extension.string() == ".obj" || extension.string() == ".OBJ") {
            meshDataArr = fileImportFactory->CreateFileImporter(EFileTypes::Obj)->Import(request->filepath());
        } else if (extension.string() == ".glb" || extension.string() == ".gltf") {
            meshDataArr = fileImportFactory->CreateFileImporter(EFileTypes::Gltf)->Import(request->filepath());
        } else {
            return Status::CANCELLED;
        }
        for(const auto& meshData : meshDataArr){
            file_importer::MeshData* _meshData = response->add_meshdataarr();
            _meshData->mutable_points()->Add(meshData.points.begin(), meshData.points.end());
            _meshData->mutable_indices()->Add(meshData.indices.begin(), meshData.indices.end());
            _meshData->mutable_uvs()->Add(meshData.uvs.begin(), meshData.uvs.end());
            _meshData->mutable_normals()->Add(meshData.normals.begin(), meshData.normals.end());

        }
        return Status::OK;
    }

private:
    std::unique_ptr<FileImporterFactory> fileImportFactory;
};

void RunServer() {
    std::string server_address("0.0.0.0:50051");
    FileImporterServiceImpl service;

    grpc::EnableDefaultHealthCheckService(true);
    grpc::reflection::InitProtoReflectionServerBuilderPlugin();
    ServerBuilder builder;
    builder.SetMaxReceiveMessageSize(1024 * 1024 * 1024);
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    builder.RegisterService(&service);
    std::unique_ptr<Server> server(builder.BuildAndStart());
    std::cout << "Server listening on " << server_address << std::endl;
    server->Wait();
}

int main(int argc, char **argv) {
    RunServer();
    return 0;
}
