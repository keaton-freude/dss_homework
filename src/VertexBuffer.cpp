#include "VertexBuffer.h"

#include <gl/glew.h>

using namespace dss;

VertexBuffer::VertexBuffer(IMesh *mesh) {
    glGenBuffers(1, &_vertexBufferHandle);

    // Hard-coding static usage of the buffer. A more full-featured VertexBuffer class
    // would support different usage types, but not necessary in our scope-limited app
    // TODO: Is this comment relevant still?
    glBindBuffer(GL_ARRAY_BUFFER, _vertexBufferHandle);
    glBufferData(GL_ARRAY_BUFFER, mesh->GetNumVertexBytes(), mesh->GetVertexData(), GL_STATIC_DRAW);
}

void VertexBuffer::Bind() const {
    glBindBuffer(GL_ARRAY_BUFFER, _vertexBufferHandle);
}