# 3d_file_importer
Build a server application based on OCCT to provide 3D model import capabilities, and communicate with the higher-level application using gRPC
# Build
opencascade: 7.8.0  
grpc: 1.51.1, protobuf: 3.21.12  
cmake options example: -DCMAKE_TOOLCHAIN_FILE=E:/xxx/vcpkg/scripts/buildsystems/vcpkg.cmake -DOpenCASCADE_DIR:PATH=E:/occt-vc143-64/cmake
# Support format
step  
obj  
stl  