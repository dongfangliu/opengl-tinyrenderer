#include "tinyRenderer.h"
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}
tinyRenderer::tinyRenderer(int width, int height, float zNear, float zFar)
{
    this->width = width;
    this->height = height;
    this->projection = glm::perspective(glm::radians(this->cam.Zoom), (float)width / (float)height, zNear, zFar);
};
tinyRenderer::~tinyRenderer()
{
    this->unloadGL();
};
void tinyRenderer::initDefaultShader()
{
    this->defaultShader = std::make_shared<Shader>(FileSystem::getPath("shaders/3.1.materials.vs").c_str(), FileSystem::getPath("shaders/3.1.materials.fs").c_str());
   this->defaultShader->use();
    this->defaultShader->setVec3("light.ambient", this->light.ambient.x(), this->light.ambient.y(), this->light.ambient.z());
    this->defaultShader->setVec3("light.diffuse", this->light.diffuse.x(), this->light.diffuse.y(), this->light.diffuse.z());
    this->defaultShader->setVec3("light.specular", this->light.specular.x(), this->light.specular.y(), this->light.specular.z());
    // material properties
    this->defaultShader->setVec3("material.ambient", 1.0f, 1.0f, 1.0f);
    this->defaultShader->setVec3("material.diffuse", 1.0f, 1.0f, 1.0f);
    this->defaultShader->setVec3("material.specular", 0.5f, 0.5f, 0.5f); // specular lighting doesn't have full effect on this object's material
    this->defaultShader->setFloat("material.shininess", 32.0f);
};

void tinyRenderer::updateShaders()
{
    this->defaultShader->use();
    this->defaultShader->setVec3("light.position", this->light.position.x(), this->light.position.y(), this->light.position.z());
    this->defaultShader->setVec3("viewPos", this->cam.Position);
    this->defaultShader->setMat4("projection", this->projection);
    this->defaultShader->setMat4("view", this->cam.GetViewMatrix());
}
int tinyRenderer::initGL()
{

    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    this->window = glfwCreateWindow(this->width, this->height, "renderer", NULL, NULL);
    if (this->window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    // glfwSetCursorPosCallback(window, mouse_callback);
    // glfwSetScrollCallback(window, scroll_callback);

    // // tell GLFW to capture our mouse
    // glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);
this->initDefaultShader();
    for (int i = 0; i < this->objects.size(); i++)
    {
        this->objects[i]->loadGLData();
    }
    return 0;
};
void tinyRenderer::renderSingleFrame()
{
    if (!glInited)
    {
        this->initGL();
        glInited = true;
    }
    // this->cam.updateDt(glfwGetTime());
    glfwPollEvents();
    // this->cam.Do_Movement();
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //update
    for (int i = 0; i < this->objects.size(); i++)
    {
        this->objects[i]->updateGLData();
    }
    this->updateShaders();
    //draw
    for (int i = 0; i < this->objects.size(); i++)
    {
       this->objects[i]->draw(*this->defaultShader);
    }
    glfwSwapBuffers(this->window);
};
void tinyRenderer::unloadGL()
{
    for (int i = 0; i < this->objects.size(); i++)
    {
        this->objects[i]->removeGLData();
    }
    glfwTerminate();
};
void tinyRenderer::savePPM(std::string filename)
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

    output_image = fopen(filename.c_str(), "wt");

    std::ofstream ofs(filename.c_str(), std::ios::out | std::ios::binary);
    ofs << "P6\n"
        << output_width << " " << output_height << "\n255\n";
    for (unsigned i = 0; i < output_width * output_height * 3; ++i)
    {
        ofs << (unsigned char)(pixels[i]);
    }
    ofs.close();
    free(pixels);
};
void tinyRenderer::setCamera(Eigen::Vector3f position, Eigen::Vector3f target, Eigen::Vector3f up)
{
    this->cam.Position = glm::vec3(position.x(), position.y(), position.z());
    this->cam.WorldUp = glm::vec3(up.x(), up.y(), up.z());
    this->cam.SetTarget(glm::vec3(target.x(), target.y(), target.z()));
};
