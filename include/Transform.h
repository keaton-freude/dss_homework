#pragma once

#include "glm/glm.hpp"
#include <glm/gtx/transform.hpp>

namespace dss
{

/**
 *  A transform is a combination of a translation, rotation and scale
 * 
 *  It represeents some orientation in 3D space and used to represent the "Model"
 *  matrix in the MVP matrix 
 * 
 *  NOTE: Typically a rotation should be stored as a Quaternion, but not forseeing a 
 *  reason to add the additional complexity as rotations will not be involved in this app.
 */
struct Transform {
    glm::vec3 translation;
    glm::vec3 rotation;
    glm::vec3 scale;

    Transform() {
        translation = glm::vec3(0.0f, 0.0f, 0.0f);
        rotation = glm::vec3(1.0f, 1.0f, 1.0f);
        scale = glm::vec3(1.0f, 1.0f, 1.0f);
    }

    Transform(glm::vec3 translation, glm::vec3 rotation, glm::vec3 scale) 
        : translation(translation), rotation(rotation), scale(scale) {

    }

    inline glm::mat4 GetModelMatrix() const {
        // Translate -> Rotate -> Scale (happens in reverse order)
        return glm::translate(translation) * glm::rotate(0.0f, rotation) * glm::scale(scale);
    }
};

}