#include "ContentTile.h"
#include "QuadMesh.h"
#include "Utility.h"

using namespace dss;

ContentTile::ContentTile(std::shared_ptr<ShaderProgram> shader)
:   _shader(shader),
    _indexBuffer(&QuadMesh()),
    _vertexBuffer(&QuadMesh()),
    _texture("textures/example_content.jpg"),
    _transform()
{
    QuadMesh mesh;
    _indexBuffer = IndexBuffer(&mesh);
    _vertexBuffer = VertexBuffer(&mesh);
}

ContentTile::ContentTile(glm::vec3 position, std::shared_ptr<ShaderProgram> shader) 
:   _shader(shader),
    _indexBuffer(&QuadMesh()),
    _vertexBuffer(&QuadMesh()),
    _texture("textures/example_content.jpg"),
    _transform(position, glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f))
{
}