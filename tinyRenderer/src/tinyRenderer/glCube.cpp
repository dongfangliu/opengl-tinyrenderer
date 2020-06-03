#include "glCube.h"
    void glCube::loadGLData(){
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);

    glBindVertexArray(VAO);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);


    };
    void glCube::updateGLData(){
    
    };
    void glCube::removeGLData(){
glDeleteVertexArrays(1, &VAO);

    glDeleteBuffers(1, &VBO);
    };
    void glCube::draw(Shader& shader){
        shader.use();
        shader.setMat4("model", this->modelMatrix);

        // render the cube
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        		glBindVertexArray(0);
    };