#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "Camera.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


void framebufferSizeCallback(GLFWwindow* window, int width, int height);
void processKeyboardInput(GLFWwindow* window, float deltaTime);
void processMouseInput(GLFWwindow* window, double xPosition, double yPosition);
unsigned int loadTexture(const char* imageFilepath);


// settings
const unsigned int SCR_WIDTH = 1200;
const unsigned int SCR_HEIGHT = 675;

double mouseX = SCR_WIDTH / 2.0f;
double mouseY = SCR_HEIGHT / 2.0f;

//The camera has to be a global variable as it's used in the callback for glfwSetCursorPosCallback, which can't
//accept a callback with a camera in its parameter list
Camera camera;


int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
    glfwSetCursorPosCallback(window, processMouseInput);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);


    Shader cubeShader("resources/shaders/vertex_shader.vs", "resources/shaders/fragment_shader.fs");
    Shader lightShader("resources/shaders/light_source.vs", "resources/shaders/light_source.fs");



    float cube[] = {
       //Vertex Data             Normal Data
        -0.5f, -0.5f, -0.5f,     0.0f,  0.0f, -1.0f,
         0.5f, -0.5f, -0.5f,     0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,     0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,     0.0f,  0.0f, -1.0f,
        -0.5f,  0.5f, -0.5f,     0.0f,  0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f,     0.0f,  0.0f, -1.0f,

        -0.5f, -0.5f,  0.5f,     0.0f,  0.0f,  1.0f,
         0.5f, -0.5f,  0.5f,     0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f,     0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f,     0.0f,  0.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,     0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,     0.0f,  0.0f,  1.0f,

        -0.5f,  0.5f,  0.5f,    -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,    -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,    -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,    -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,    -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,    -1.0f,  0.0f,  0.0f,

         0.5f,  0.5f,  0.5f,     1.0f,  0.0f,  0.0f,
         0.5f,  0.5f, -0.5f,     1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,     1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,     1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,     1.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,     1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,     0.0f, -1.0f,  0.0f,
         0.5f, -0.5f, -0.5f,     0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,     0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,     0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,     0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,     0.0f, -1.0f,  0.0f,

        -0.5f,  0.5f, -0.5f,     0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,     0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,     0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,     0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,     0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,     0.0f,  1.0f,  0.0f
    };



    unsigned int VBO, cubeVAO;
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube), cube, GL_STATIC_DRAW);
  
    glBindVertexArray(cubeVAO);

    //Positions
    //Arguments:
    //1: Index of vertex attribute of the VAO we are binding the VBO to
    //2: Number of elements (in this case 3 vertices) in the vertex attribute
    //3: Specifies the type of data being stored
    //4: Specifies whether or not the data should be normalized
    //5: The stride, which is the space between consecutive vertex attributes. In this case, the stride is the length of 
    //   three vertices, three color coordinates, and two texture coordinates (8 floats total)
    //6: The offset of where the data begins in the buffer
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //Specify normal data
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);



    unsigned int lightVAO;
    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);


    unsigned int texture = loadTexture("resources/textures/container.jpg");


    float previousTime = 0.0f;
    float currentTime;
    float deltaTime;


    glEnable(GL_DEPTH_TEST);
    // Render Loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        currentTime = glfwGetTime();
        deltaTime = currentTime - previousTime;
        previousTime = currentTime;

        // input
        // -----
        processKeyboardInput(window, deltaTime);

       
        glClearColor(0.1f, 0.6f, 0.6f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::vec3 lightPosition = glm::vec3(2.0f, 2.0f, 0.0f);

        //-----------
        //Render cube
        cubeShader.use();

        glm::mat4 modelTransform = glm::mat4(1.0f);
        cubeShader.setMat4("modelTransform", modelTransform);


        glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH/(float)SCR_HEIGHT, 0.1f, 100.0f);  
        cubeShader.setMat4("projectionTransform", projection);
          

        glm::mat4 viewTransformation = camera.getViewMatrix();
        cubeShader.setMat4("viewTransform", viewTransformation);


        glm::mat4 normalMatrix = glm::transpose(glm::inverse(modelTransform));
        cubeShader.setMat4("normalMatrix", normalMatrix);


        cubeShader.setVec3("cameraPosition", camera.getPosition());


        cubeShader.setVec3("objectColor", 1.0f, 0.5f, 0.44f);
        
        //Obsidian
      /*  cubeShader.setVec3("material.ambientStrength", glm::vec3(0.05375f, 0.05f, 0.06625f));
        cubeShader.setVec3("material.diffuseStrength", glm::vec3(0.18275f, 0.17f, 0.22525f));
        cubeShader.setVec3("material.specularStrength", glm::vec3(0.332741f, 0.328634f, 0.346435f));
        cubeShader.setFloat("material.shininess", 0.3f);*/

        ///*Pearl*/
        //cubeShader.setVec3("material.ambientStrength", glm::vec3(0.25f, 0.20725f, 0.20725f));
        //cubeShader.setVec3("material.diffuseStrength", glm::vec3(1.0f, 0.829f, 0.829f));
        //cubeShader.setVec3("material.specularStrength", glm::vec3(0.296648f, 0.296648f, 0.296648f));
        //cubeShader.setFloat("material.shininess", 0.088f);
        //

        //Brass
        cubeShader.setVec3("material.ambientStrength", glm::vec3(0.2125f, 0.1275f, 0.054f));
        cubeShader.setVec3("material.diffuseStrength", glm::vec3(0.714f, 0.4284f, 0.18144f));
        cubeShader.setVec3("material.specularStrength", glm::vec3(0.393548f, 0.271906f, 0.166721f));
        cubeShader.setFloat("material.shininess", 25.6);

        cubeShader.setVec3("light.ambientStrength", glm::vec3(1.0f));
        cubeShader.setVec3("light.diffuseStrength", glm::vec3(1.0f));
        cubeShader.setVec3("light.specularStrength", glm::vec3(1.0f));


        lightPosition.z = -1.0f;
        lightPosition.x *= sin(glfwGetTime());
        lightPosition.y *= -cos(glfwGetTime());
        cubeShader.setVec3("lightPosition", lightPosition);


        glBindVertexArray(cubeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        //-------------------
        //Render light source
        lightShader.use();

        lightShader.setMat4("projectionTransform", projection);
        lightShader.setMat4("viewTransform", viewTransformation);

        glm::mat4 lightTransform = glm::mat4(1.0f);
        lightTransform = glm::translate(lightTransform, lightPosition);
        lightTransform = glm::scale(lightTransform, glm::vec3(0.2f));
        lightShader.setMat4("modelTransform", lightTransform);

        glBindVertexArray(lightVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
       
        
      

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteBuffers(1, &VBO);
    //glDeleteBuffers(1, &EBO);
    //glDeleteProgram(shaderProgram);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processKeyboardInput(GLFWwindow* window, float deltaTime)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

   
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.translate(CameraMovement::FORWARD, deltaTime);

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.translate(CameraMovement::BACKWARD, deltaTime);

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.translate(CameraMovement::RIGHT, deltaTime);

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.translate(CameraMovement::LEFT, deltaTime);

    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        camera.translate(CameraMovement::UP, deltaTime);

    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
        camera.translate(CameraMovement::DOWN, deltaTime);

}

void processMouseInput(GLFWwindow* window, double newMouseX, double newMouseY)
{
    double mouseDeltaX = newMouseX - mouseX;
    double mouseDeltaY = mouseY - newMouseY;

    mouseX = newMouseX;
    mouseY = newMouseY;

    camera.rotate(mouseDeltaX, mouseDeltaY);
}


unsigned int loadTexture(const char* imageFilepath)
{
    int width, height, numColorChannels;
    unsigned char* textureData = stbi_load(imageFilepath, &width, &height, &numColorChannels, 0);


    //Generate and bind texture object
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);


    //Set texture wrapping/filtering options for currently bound texture object
    //These two near-identical calls are to set the texture wrapping options along the S and T axes (equivalent to x and y)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    //Texture filtering option for minifying operations (scaling the texture down) and magnifying operations
    //When specifying these options for minifying operations, we can also set the method for mipmap filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


    if (textureData) {
        //Arguments
        //1: -GL_TEXTURE_2D- Specifies the texture target, i.e. the texture currently bound to GL_TEXTURE_2D in the previous line
        //2: -0- Manually specifies the mipmap level of the texture, which we are currently leaving at the base level of 0
        //3: -GL_RGB- Specifies the color format the texture should be stored as
        //4 & 5: -width, height- Sets the width and height of the texture being created
        //6: -0- Always 0 due to legacy code
        //7, 8: -GL_RGB, GL_UNSINGED_BYTE- Specifies the format and datatype of the source image (GL_UNSIGNED_BYTE = unsigned char)
        //9: Image data for the textue
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, textureData);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        std::cout << "Failed to load texture" << std::endl;
    }

    stbi_image_free(textureData);

    return texture;
}



// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}


