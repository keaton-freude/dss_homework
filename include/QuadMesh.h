#pragma once

#include <vector>

#include "IMesh.h"
#include "VertexFormat.h"

/**
 *  A hand-written Quad Mesh built of 2 triangles
 */

namespace dss
{

class QuadMesh : public IMesh {
private:
    std::vector<uint32_t> _indices;
    std::vector<PositionColor> _vertices;
    size_t _numFaces;
public:
    QuadMesh();

    const size_t GetNumVertexBytes() const override;
    void *GetVertexData() const override;
    const size_t GetNumFaces() const override;
    const size_t GetNumFacesBytes() const override;
    const void *GetFaceData() const override;
};
    
}