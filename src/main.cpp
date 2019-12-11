#include <iostream>
#include "httplib.h"
#include "json.hpp"
#include <GLFW/glfw3.h>
#include "gl/GL.h"
#include "glm/vec3.hpp"
#include "Window.h"
#include "Application.h"

using json = nlohmann::json;
using namespace dss;

int main() {
    Application application;
    application.Run();
}