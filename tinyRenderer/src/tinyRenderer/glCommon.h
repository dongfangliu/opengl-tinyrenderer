#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <learnopengl/filesystem.h>
#include <learnopengl/shader_m.h>
#include <learnopengl/camera.h>

#include <iostream>

const float cube_vertices[] = {-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
                               0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
                               0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
                               0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
                               -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
                               -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,

                               -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
                               0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
                               0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
                               0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
                               -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
                               -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,

                               -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f,
                               -0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
                               -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
                               -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
                               -0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f,
                               -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f,

                               0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f,
                               0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
                               0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
                               0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
                               0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f,
                               0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f,

                               -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,
                               0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,
                               0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f,
                               0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f,
                               -0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f,
                               -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,

                               -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
                               0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
                               0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
                               0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
                               -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
                               -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f};
