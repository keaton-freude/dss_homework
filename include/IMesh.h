#pragma once

namespace dss
{

/**
 *  Represents the minimal interface a derived Mesh should implement.
 *  Components which need access to a Mesh can accept pointers to an IMesh
 *  to do their work, and we can abstract the details of that mesh away
 * 
 *  NOTE: In this interface we do not know about the underlying layout of
 *  vertex data, nor how the faces are organized. We want to be able to
 *  consume this interface in order to create Index & Vertex buffers
 */
class IMesh {
public:
    virtual ~IMesh() = default;

    // The amount of bytes consumed by the Vertex Data of the Mesh
    virtual const size_t GetNumVertexBytes() const = 0;

    // Pointer to the beginning of the Vertex Data
    // TODO: Can we get rid of the void* ?? It's not pretty, but its also
    // what OpenGL expects (impl details leak?)
    virtual void *GetVertexData() const = 0;

    // Number of faces in this mesh
    virtual const size_t GetNumFaces() const = 0;

    // The amount of bytes consumed by the Index Data of the Mesh
    virtual const size_t GetNumFacesBytes() const = 0;

    // Pointer to the beginning of the Vertex Data
    virtual const void *GetFaceData() const = 0;
};

}