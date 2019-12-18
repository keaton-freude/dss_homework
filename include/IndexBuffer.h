#pragma once

#include <stdint.h>

#include "IMesh.h"

/**
 *  Support for an OpenGL Index Buffer
 * 
 *  Not strictly necessary, especially as we are likely only drawing Quad's
 *  but it's the way I'd normally do rendering
 */

namespace dss
{

/**
 *  General idea is to wrap an OpenGL Index Buffer Handle
 *  which includes providing an easy way to create an Index buffer
 *  for a given Mesh.
 * 
 *  Design Considerations:
 *      -   Not keeping a copy of the index data. This might be useful in some
 *          contexts, especially Dynamic index data. But specifically not doing it here
 *          as it will not be needed and save a few bytes
 *        
 *
 */
class IndexBuffer {
private:
    // Handle OpenGL gives us to reference the managed index buffer
    uint32_t _indexBufferHandle;
    
    // How many faces does this Index Buffer represent
    size_t _numFaces;
public:
    // Want to enforce a pre-condition of this class, which is we immediately wrap
    // data and do not allow an IndexBuffer in an invalid/unwanted state
    IndexBuffer() = delete;

    // Build an IndexBuffer from a provided mesh
    IndexBuffer(IMesh *mesh);

    size_t GetNumFaces() const {
        return _numFaces;
    }

    // Bind this Index Buffer to the pipeline
    void Bind() const;
};

}