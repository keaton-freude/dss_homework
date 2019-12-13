#include "Background.h"
#include "QuadMesh.h"

using namespace dss;

Background::Background() : _mesh(std::make_unique<QuadMesh>()), _indexBuffer(_mesh.get()), _vertexBuffer(_mesh.get()) {
}