#include "IndexBuffer.h"

#include <gl/glew.h>

using namespace dss;

IndexBuffer::IndexBuffer(IMesh *mesh) {
    glGenBuffers(1, &_indexBufferHandle);

    // Hard-coding static usage of the buffer. An obvious extension point to this
    // class would be the ability to choose another usage type, like dynamic
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indexBufferHandle);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->GetNumFacesBytes(), mesh->GetFaceData(), GL_STATIC_DRAW);
}

void IndexBuffer::Bind() const {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indexBufferHandle);
}