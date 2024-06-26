cmake_minimum_required(VERSION 3.1)

project(3d_file_importer)

set(CMAKE_CXX_STANDARD  17)

find_package(gRPC CONFIG REQUIRED)
find_package(Protobuf REQUIRED)

# OpenCascade
find_package(OpenCASCADE)
include_directories(SYSTEM ${OpenCASCADE_INCLUDE_DIR})

file(GLOB_RECURSE src_files src/*.cpp src/*.h)
set(proto_files proto/file_importer.grpc.pb.cc proto/file_importer.pb.cc
        src/file_importer_factory.cpp
        src/file_importer_factory.h)

include_directories(proto)
add_executable(3d_file_importer ${src_files} ${proto_files})

target_link_libraries(3d_file_importer PRIVATE gRPC::grpc++ gRPC::grpc++_reflection gRPC::gpr gRPC::grpc gRPC::grpc++ protobuf::libprotoc protobuf::libprotobuf protobuf::libprotobuf-lite)

# Add linker options
foreach (LIB ${OpenCASCADE_LIBRARIES})
    target_link_libraries(3d_file_importer PRIVATE ${OpenCASCADE_LIBRARY_DIR}/${LIB}.lib)
endforeach ()