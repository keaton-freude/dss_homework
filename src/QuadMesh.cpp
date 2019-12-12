#include "QuadMesh.h"

using namespace dss;

/**
 *  View of the QuadMesh vertices
 *      v0    v1
 * 
 *
 *      v2    v3 
 * 
 *  Faces are defined as: [{v0, v2, v1}, {v1, v2, v3}]
 * 
 *  Quad will be a unit quad: width == 1.0f, height == 1.0f, top-left corner at 0.0f, 0.0f
 */
QuadMesh::QuadMesh()
    : _indices({0, 1, 2, 0, 1, 3}), _vertices({
        {glm::vec3(0.0f, 0.0f, 0.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)},
        {glm::vec3(1.0f, 0.0f, 0.0f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f)},
        {glm::vec3(0.0f, 0.0f, 1.0f), glm::vec4(0.0f, 1.0f, 0.0f, 1.0f)},
        {glm::vec3(1.0f, 0.0f, 1.0f), glm::vec4(0.0f, 0.0f, 1.0f, 1.0f)}
    }), _numFaces(2)
{
}

const size_t QuadMesh::GetNumVertexBytes() const {
    return _vertices.size() * PositionColor::SizeInBytes();
}

void *QuadMesh::GetVertexData() const {
    return (void *)&_vertices.front();
}

const size_t QuadMesh::GetNumFaces() const {
    return _numFaces;
}

const size_t QuadMesh::GetNumFacesBytes() const {
    return _indices.size() * sizeof(decltype(_indices)::value_type);
}

const void *QuadMesh::GetFaceData() const {
    return (void *)&_indices.front();
}