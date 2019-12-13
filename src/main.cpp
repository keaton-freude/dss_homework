#include <iostream>
#include "httplib.h"
#include "json.hpp"
#include "glm/vec3.hpp"
#include "VertexFormat.h"
#include "Application.h"
#include "ShaderProgram.h"
#include "Utility.h"
#include <gl/glew.h>

using json = nlohmann::json;
using namespace dss;

int main() {
    try {
        Application application;
        application.Run();
    } catch (std::exception& exception) {
        std::cerr << "Caught unhandled exception in main: " << exception.what() << std::endl;
    }
}