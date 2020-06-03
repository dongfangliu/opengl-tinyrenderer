
#pragma once
#include "glCommon.h"
#include "GLShader.h"
#include "glCamera.h"
#include <string>
#include <vector>
#include "../../cslbm/rigidbody/mlRigidBody.h"
#include "../../cslbm/rigidbody/mlMeshWrapper.h"
#include "../../cslbm/mlUtil/float3.h"
#include "OpenGLWindow/GLInstanceGraphicsShape.h"
#include "BulletCollision/CollisionShapes/btShapeHull.h"
#include "Utils/b3BulletDefaultFileIO.h"
#include "Bullet3Common/b3FileUtils.h"
#include "Importers/ImportObjDemo/LoadMeshFromObj.h"
// Function prototypes

struct renderParam
{
    float3 cameraPos;
    float3 cameraTarget;
    float3 cameraUp;
    float3 lightPos;
    float3 lightColor;
    int width = 1024;
    int height = 768;
};
class model_data
{
public:
    std::vector<GLInstanceVertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<float3> samples;
    btCollisionObject *object;
    float3 color;
    unsigned int meshVAO;
    unsigned int meshVBO;
    unsigned int meshEBO;

    unsigned int sampleVAO, sampleVBO;
    void init(btCollisionObject *object, float3 color, char *filePath = NULL)
    {
        this->object = object;
        this->color = color;
        vertices.clear();
        indices.clear();
        btCollisionShape *shape = object->getCollisionShape();
        initShape(shape, btTransform::getIdentity(), filePath);
    }
    void initShape(btCollisionShape *shape, btTransform localTrans = btTransform::getIdentity(), char *filePath = NULL)
    {
        switch (shape->getShapeType())
        {
        case BroadphaseNativeTypes::STATIC_PLANE_PROXYTYPE:
            break;
        case BroadphaseNativeTypes::COMPOUND_SHAPE_PROXYTYPE:
        {
            btCompoundShape *compound = (btCompoundShape *)shape;
            //std::cout << "childShape Num: " << compound->getNumChildShapes() << std::endl;
            for (int i = 0; i < compound->getNumChildShapes(); i++)
            {
                initShape(compound->getChildShape(i), compound->getChildTransform(i) * localTrans);
            }
            break;
        }
        case BroadphaseNativeTypes::BOX_SHAPE_PROXYTYPE:
        {
            btBoxShape *boxShape = (btBoxShape *)shape;
            init_convex(boxShape, localTrans);
            //init_box(boxShape);
            break;
        }
        case BroadphaseNativeTypes::CAPSULE_SHAPE_PROXYTYPE:
        {
            btCapsuleShapeZ *capsuleShape = (btCapsuleShapeZ *)shape;
            init_convex(capsuleShape, localTrans);
            break;
        }
        case BroadphaseNativeTypes::SPHERE_SHAPE_PROXYTYPE:
        {
            btSphereShape *sphereShape = (btSphereShape *)shape;
            init_convex(sphereShape, localTrans);
            break;
        }
        case BroadphaseNativeTypes::CYLINDER_SHAPE_PROXYTYPE:
        {
            btCylinderShapeZ *cylZShape = (btCylinderShapeZ *)shape;
            init_convex(cylZShape, localTrans);
            break;
        }
        case BroadphaseNativeTypes::TRIANGLE_MESH_SHAPE_PROXYTYPE:
        {
            btTriangleMeshShape *meshShape = (btTriangleMeshShape *)shape;
            init_mesh((btTriangleMesh *)meshShape->getMeshInterface(), filePath);
            break;
        }
        case BroadphaseNativeTypes::CONVEX_TRIANGLEMESH_SHAPE_PROXYTYPE:
        {
            btConvexTriangleMeshShape *meshShape = (btConvexTriangleMeshShape *)shape;
            init_mesh((btTriangleMesh *)meshShape->getMeshInterface(), filePath);
            break;
        }
        case BroadphaseNativeTypes::CONVEX_HULL_SHAPE_PROXYTYPE:
        {
            btConvexHullShape *convexHullShape = (btConvexHullShape *)shape;
            init_convex(convexHullShape, localTrans);
            break;
        }
        }
    }
    void init_convex(btConvexShape *shape, btTransform localTrans = btTransform::getIdentity())
    {

        btShapeHull *hull = new btShapeHull(shape);
        hull->buildHull(0.0);
        int numVertices = hull->numVertices();
        int numIndices = hull->numIndices();
        for (int i = 0; i < numVertices; i++)
        {
            GLInstanceVertex vtx;
            btVector3 pos = btMatrix3x3(localTrans.getRotation()) * (hull->getVertexPointer()[i]) + localTrans.getOrigin();
            vtx.xyzw[0] = pos.x();
            vtx.xyzw[1] = pos.y();
            vtx.xyzw[2] = pos.z();
            vtx.xyzw[3] = 1.f;
            pos.normalize();
            vtx.normal[0] = pos.x();
            vtx.normal[1] = pos.y();
            vtx.normal[2] = pos.z();
            btScalar u = btAtan2(vtx.normal[0], vtx.normal[2]) / (2 * SIMD_PI) + 0.5;
            btScalar v = vtx.normal[1] * 0.5 + 0.5;
            vtx.uv[0] = u;
            vtx.uv[1] = v;
            vertices.push_back(vtx);
        }
        for (int i = 0; i < numIndices; i++)
        {
            indices.push_back(hull->getIndexPointer()[i]);
        }

        this->bindData();
    }
    void init_mesh(btTriangleMesh *mesh, char *filePath = NULL, btTransform localTrans = btTransform::getIdentity())
    {
        if (!filePath)
        {
            int numVertices = mesh->m_4componentVertices.size();
            int numIndices = mesh->m_32bitIndices.size();
            for (int i = 0; i < numVertices; i++)
            {
                GLInstanceVertex vtx;
                btVector3 pos = mesh->m_4componentVertices.at(i);
                vtx.xyzw[0] = pos.x();
                vtx.xyzw[1] = pos.y();
                vtx.xyzw[2] = pos.z();
                vtx.xyzw[3] = 1.f;
                pos.normalize();
                vtx.normal[0] = pos.x();
                vtx.normal[1] = pos.y();
                vtx.normal[2] = pos.z();
                btScalar u = btAtan2(vtx.normal[0], vtx.normal[2]) / (2 * SIMD_PI) + 0.5;
                btScalar v = vtx.normal[1] * 0.5 + 0.5;
                vtx.uv[0] = u;
                vtx.uv[1] = v;
                vertices.push_back(vtx);
            }
            for (int i = 0; i < numIndices; i++)
            {
                indices.push_back(mesh->m_32bitIndices.at(i));
            }
        }
        else
        {
            b3BulletDefaultFileIO m_fileIO;
            char relativeFileName[1024];
            char pathPrefix[1024];
            pathPrefix[0] = 0;
            if (m_fileIO.findResourcePath(filePath, relativeFileName, 1024))
            {
                b3FileUtils::extractPath(relativeFileName, pathPrefix, 1024);
            }
            GLInstanceGraphicsShape *glmesh = LoadMeshFromObj(filePath, pathPrefix, &m_fileIO);
            for (int i = 0; i < glmesh->m_numvertices; i++)
            {
                vertices.push_back(glmesh->m_vertices->at(i));
            }
            for (int i = 0; i < glmesh->m_numIndices; i++)
            {
                indices.push_back(glmesh->m_indices->at(i));
            }
        }

        this->bindData();
    }
    //void init_box(btBoxShape *shape) {
    //
    //	btVector3 extents = shape->getHalfExtentsWithMargin();
    //	int strideInBytes = 9 * sizeof(float);
    //	int numVertices = sizeof(cube_vertices_textured) / strideInBytes;
    //	int numIndices = sizeof(cube_indices) / sizeof(int);
    //	btScalar halfExtentsX = extents[0];
    //	btScalar halfExtentsY = extents[1] ;
    //	btScalar halfExtentsZ = extents[2] ;
    //	GLInstanceVertex verts;
    //	for (int i = 0; i < numVertices; i++)
    //	{
    //		verts.xyzw[0] = halfExtentsX * cube_vertices_textured[i * 9];
    //		verts.xyzw[1] = halfExtentsY * cube_vertices_textured[i * 9 + 1];
    //		verts.xyzw[2] = halfExtentsZ * cube_vertices_textured[i * 9 + 2];
    //		verts.xyzw[3] = cube_vertices_textured[i * 9 + 3];
    //		verts.normal[0] = cube_vertices_textured[i * 9 + 4];
    //		verts.normal[1] = cube_vertices_textured[i * 9 + 5];
    //		verts.normal[2] = cube_vertices_textured[i * 9 + 6];
    //		verts.uv[0] = cube_vertices_textured[i * 9 + 7];
    //		verts.uv[1] = cube_vertices_textured[i * 9 + 8];
    //		vertices.push_back(verts);
    //	}
    //	for (int i = 0; i < numIndices; i++)
    //	{
    //		indices.push_back(cube_indices[i]);
    //	}
    //	this->bindData();
    //}
    glm::mat4 getModelMatrix()
    {
        btTransform trans = object->getWorldTransform();
        mlVertex3f t(trans.getOrigin().x(), trans.getOrigin().y(), trans.getOrigin().z());

        mlVertex3f r;

        trans.getRotation().getEulerZYX(r.z, r.y, r.x);
        //mlVertex3f scaling(object->getCollisionShape()->getLocalScaling().x(), object->getCollisionShape()->getLocalScaling().y(), object->getCollisionShape()->getLocalScaling().z());
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(t.x, t.y, t.z)); // translate it down so it's at the center of the scene
        //modelMatrix = glm::scale(modelMatrix, glm::vec3(scaling.x, scaling.y, scaling.z));	// it's a bit too big for our scene, so scale it down
        modelMatrix = glm::rotate(modelMatrix, r.x, glm::vec3(1, 0, 0));
        modelMatrix = glm::rotate(modelMatrix, r.y, glm::vec3(0, 1, 0));
        modelMatrix = glm::rotate(modelMatrix, r.z, glm::vec3(0, 0, 1));
        return modelMatrix;
    }
    void bindData()
    {
        glGenVertexArrays(1, &meshVAO);
        glGenBuffers(1, &meshVBO);
        glGenBuffers(1, &meshEBO);
        glBindVertexArray(meshVAO);
        glBindBuffer(GL_ARRAY_BUFFER, meshVBO);
        glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(GLInstanceVertex),
                     &this->vertices[0], GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshEBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(int), &this->indices[0], GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLInstanceVertex),
                              (GLvoid *)0);
        // ���÷���ָ��
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GLInstanceVertex),
                              (GLvoid *)offsetof(GLInstanceVertex, normal));
        // ����uvָ��
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(GLInstanceVertex),
                              (GLvoid *)offsetof(GLInstanceVertex, uv));
        glBindVertexArray(0);
        if (samples.size() <= 0)
        {
            return;
        }
        //samplesDrawing
        glGenVertexArrays(1, &sampleVAO);
        glGenBuffers(1, &sampleVBO);
        glBindVertexArray(sampleVAO);
        glBindBuffer(GL_ARRAY_BUFFER, sampleVBO);
        glBufferData(GL_ARRAY_BUFFER, this->samples.size() * sizeof(float3),
                     &this->samples[0], GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float3), (GLvoid *)0);
        glBindVertexArray(0);
    }
    void draw(glm::mat4 view, glm::mat4 project, glShader &shader)
    {
        glm::mat4 model = getModelMatrix();

        shader.setVec3("objectColor", color.x, color.y, color.z);
        shader.setMat4("projection", project);
        shader.setMat4("view", view);

        shader.setMat4("model", model);
        glBindVertexArray(meshVAO);
        glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
        if (samples.size() <= 0)
        {
            return;
        }
        shader.setVec3("objectColor", 1, 0, 0);
        glBindVertexArray(sampleVAO);
        glDrawArrays(GL_POINTS, 0, samples.size());
        glBindVertexArray(0);
    }
};
class glRender
{
public:
    glCamera camera;
    float3 lightPos, lightColor;
    int width = 1024, height = 768;
    GLFWwindow *window;
    std::vector<model_data> models;
    glShader shader;
    glRender(renderParam param)
    {
        this->camera = glCamera(glm::vec3(param.cameraPos.x, param.cameraPos.y, param.cameraPos.z), glm::vec3(param.cameraUp.x, param.cameraUp.y, param.cameraUp.z));
        this->camera.Front = glm::vec3(param.cameraTarget.x, param.cameraTarget.y, param.cameraTarget.z) - this->camera.Position;
        this->width = param.width;
        this->height = param.height;
        this->lightColor = param.lightColor;
        this->lightPos = param.lightPos;
        initGL();
        models.clear();
    }
    glRender()
    {
        models.clear();
        initGL();
    }
    glm::mat4 getViewMatrix()
    {
        return this->camera.GetViewMatrix();
    }
    glm::mat4 getProjectionMatrix()
    {
        return glm::perspective(glm::radians(this->camera.Zoom), ((float)(width)) / ((float)(height)), 0.1f, 1000.0f);
    }

    inline void initGL()
    {
        /* Initialize the library */
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
        glfwWindowHint(GLFW_SAMPLES, 4);

        /* Create a windowed mode window and its OpenGL context */
        window = glfwCreateWindow(width, height, "Hello World", NULL, NULL);
        if (!window)
        {
            glfwTerminate();
        }
        /* Make the window's context current */
        glfwMakeContextCurrent(window);
        glfwSetWindowUserPointer(window, &camera);

        auto key_callback = [](GLFWwindow *window, int key, int scancode, int action, int mode) {
            static_cast<glCamera *>(glfwGetWindowUserPointer(window))->key_callback(window, key, scancode, action, mode);
        };
        auto mouse_callback = [](GLFWwindow *window, double xpos, double ypos) {
            static_cast<glCamera *>(glfwGetWindowUserPointer(window))->mouse_callback(window, xpos, ypos);
        };
        auto scroll_callback = [](GLFWwindow *window, double xoffset, double yoffset) {
            static_cast<glCamera *>(glfwGetWindowUserPointer(window))->scroll_callback(window, xoffset, yoffset);
        };
        glfwSetKeyCallback(window, key_callback);
        glfwSetCursorPosCallback(window, mouse_callback);
        glfwSetScrollCallback(window, scroll_callback);
        // Options
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

        glewExperimental = GL_TRUE;
        glewInit();
        glEnable(GL_DEPTH_TEST);
        glPointSize(10);
        shader = glShader("simulatorInterface\\render\\phong.vs", "simulatorInterface\\render\\phong.fs");
        shader.use();
        shader.setVec3("viewPos", camera.Position);
        shader.setVec3("lightPos", lightPos.x, lightPos.y, lightPos.z);
        shader.setVec3("lightColor", lightColor.x, lightColor.y, lightColor.z);
        //shader.setBool("blinn", false);
    }
    ~glRender()
    {
        glfwTerminate();
    }
    void render()
    {
        camera.updateDt(glfwGetTime());
        glfwPollEvents();
        camera.Do_Movement();
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        for (int i = 0; i < models.size(); i++)
        {
            models[i].draw(getViewMatrix(), getProjectionMatrix(), shader);
        }
        //FBO_2_PPM_file(frame);
        glfwSwapBuffers(window);
    }
    inline void FBO_2_PPM_file(int num)
    {
        FILE *output_image;
        int output_width, output_height;

        output_width = width;
        output_height = height;
        /// READ THE PIXELS VALUES from FBO AND SAVE TO A .PPM FILE
        unsigned char *pixels = (unsigned char *)malloc(output_width * output_height * 3);
        /// READ THE CONTENT FROM THE FBO
        glReadBuffer(GL_COLOR_ATTACHMENT0);
        //glBindTexture(GL_TEXTURE_2D, _texture);
        //glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA,GL_UNSIGNED_BYTE, pixels);
        glReadPixels(0, 0, output_width, output_height, GL_RGB, GL_UNSIGNED_BYTE, pixels);

        char filename[256];
        sprintf_s(filename, "./data/ppm_ve/static%05d.ppm", num);

        output_image = fopen(filename, "wt");

        std::ofstream ofs(filename, std::ios::out | std::ios::binary);
        ofs << "P6\n"
            << output_width << " " << output_height << "\n255\n";
        for (unsigned i = 0; i < output_width * output_height * 3; ++i)
        {
            ofs << (unsigned char)(pixels[i]);
        }
        ofs.close();
        free(pixels);
    }
};