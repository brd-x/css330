#include <iostream>         // cout, cerr
#include <cstdlib>          // EXIT_FAILURE
#include <GL/glew.h>        // GLEW library
#include <GLFW/glfw3.h>     // GLFW library
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"      // Image loading Utility functions
#include <corecrt_math_defines.h>
#include <cmath>



// GLM Math Header inclusions
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "camera.h" // Camera class


using namespace std; // Standard namespace

/*Shader program Macro*/
#ifndef GLSL
#define GLSL(Version, Source) "#version " #Version " core \n" #Source
#endif

// Unnamed namespace
namespace
{
    const char* const WINDOW_TITLE = "Final Project"; // Macro for window title

    // Variables for window width and height
    const int WINDOW_WIDTH = 800;
    const int WINDOW_HEIGHT = 600;

    // Stores the GL data relative to a given mesh
    struct GLMesh
    {
       
        GLuint vaseVAO;
        GLuint planeVAO;
        GLuint tabletopVAO;
        GLuint tableLeg2VAO;
        GLuint tableLeg3VAO;
        GLuint flowersVAO;
        GLuint flowers2VAO;
        GLuint flowers3VAO;
        GLuint flowers4VAO;
        GLuint flowers5VAO;
        GLuint flowers6VAO;
        GLuint tableLeg4VAO;
        GLuint coasterVAO;
        GLuint bindingVAO;
        GLuint tableLeg1VAO;
        GLuint bowlVAO;
        GLuint plateVAO;


        GLuint vaseVBO;
        GLuint planeVBO;
        GLuint tabletopVBO;
        GLuint tableLeg2VBO;
        GLuint tableLeg3VBO;
        GLuint flowersVBO;
        GLuint flowers2VBO;
        GLuint flowers3VBO;
        GLuint flowers4VBO;
        GLuint flowers5VBO;
        GLuint flowers6VBO;
        GLuint tableLeg4VBO;
        GLuint coasterVBO;
        GLuint bindingVBO;
        GLuint tableLeg1VBO;
        GLuint bowlVBO;
        GLuint plateVBO;


        GLuint nvaseVertices;
        GLuint nPlaneVertices;
        GLuint ntabletopVertices;
        GLuint ntableLeg2Vertices;
        GLuint ntableLeg3Vertices;
        GLuint nflowersVertices;
        GLuint nflowers2Vertices;
        GLuint nflowers3Vertices;
        GLuint nflowers4Vertices;
        GLuint nflowers5Vertices;
        GLuint nflowers6Vertices;
        GLuint ntableLeg4Vertices;
        GLuint ncoasterVertices;
        GLuint nBindingVertices;
        GLuint ntableLeg1Vertices;
        GLuint nbowlVertices;
        GLuint nplateVertices;


        GLuint vao;
        GLuint vbo;
        GLuint nVertices;
    };

    // Main GLFW window
    GLFWwindow* gWindow = nullptr;
    // Triangle mesh data
    GLMesh gMesh;
    //Texture id
    GLuint gTextureIdgrass;
    GLuint gTextureIdgrassColor;
    GLuint gTextureIdtableLeg3;
    GLuint gTextureIdtableLeg3Color;
    GLuint gTextureIdflowers;
    GLuint gTextureIdflowers2;
    GLuint gTextureIdflowers3;
    GLuint gTextureIdflowers4;
    GLuint gTextureIdflowers5;
    GLuint gTextureIdflowers6;
    GLuint gTextureIdtabletop;
    GLuint gTextureIdtableLeg4;
    GLuint gTextureIdcoaster;
    GLuint gTextureIdtableLeg1;
    GLuint gTextureIdbowl;
    GLuint gTextureIdplate;
    GLuint gTextureIdvase;
    // Shader program
    GLuint gProgramId;

    // camera
    Camera gCamera(glm::vec3(0.0f, 15.0f, 3.0f));
    float gLastX = WINDOW_WIDTH / 2.0f;
    float gLastY = WINDOW_HEIGHT / 2.0f;
    bool gFirstMouse = true;

    float angle = 0.0f;
    bool increaseAngle = true;
    bool ortho = false;

    // timing
    float gDeltaTime = 0.0f; // time between current frame and last frame
    float gLastFrame = 0.0f;

}

/* User-defined Function prototypes to:
 * initialize the program, set the window size,
 * redraw graphics on the window when resized,
 * and render graphics on the screen
 */
bool UInitialize(int, char* [], GLFWwindow** window);
void UResizeWindow(GLFWwindow* window, int width, int height);
void UProcessInput(GLFWwindow* window);
void UMousePositionCallback(GLFWwindow* window, double xpos, double ypos);
void UMouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
void UMouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
void UCreateMesh(GLMesh& mesh);
void UCreateTexturedMesh(GLMesh& mesh);
void UDestroyMesh(GLMesh& mesh);
bool UCreateTexture(const char* filename, GLuint& textureId);
void UDestroyTexture(GLuint textureId);
void URender();
bool UCreateShaderProgram(const char* vtxShaderSource, const char* fragShaderSource, GLuint& programId);
void UDestroyShaderProgram(GLuint programId);

/* Vertex Shader Source Code*/
const GLchar* vertexShaderSource = GLSL(440,
    layout(location = 0) in vec3 position; // Vertex data from Vertex Attrib Pointer 0
layout(location = 2) in vec2 textureCoordinate;

out vec2 vertexTextureCoordinate;


//Global variables for the  transform matrices
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(position, 1.0f); // transforms vertices to clip coordinates
    vertexTextureCoordinate = textureCoordinate;
}
);


/* Fragment Shader Source Code*/
const GLchar* fragmentShaderSource = GLSL(440,
    in vec2 vertexTextureCoordinate; // Variable to hold incoming color data from vertex shader

out vec4 fragmentColor;

uniform sampler2D uTexture;

void main()
{
    fragmentColor = texture(uTexture, vertexTextureCoordinate);
}
);

void flipImageVertically(unsigned char* image, int width, int height, int channels)
{
    for (int j = 0; j < height / 2; ++j)
    {
        int index1 = j * width * channels;
        int index2 = (height - 1 - j) * width * channels;

        for (int i = width * channels; i > 0; --i)
        {
            unsigned char tmp = image[index1];
            image[index1] = image[index2];
            image[index2] = tmp;
            ++index1;
            ++index2;
        }
    }
}


int main(int argc, char* argv[])
{
    if (!UInitialize(argc, argv, &gWindow))
        return EXIT_FAILURE;

    // Create the mesh
    UCreateTexturedMesh(gMesh); // Calls the function to create the Vertex Buffer Object

    // Create the shader program
    if (!UCreateShaderProgram(vertexShaderSource, fragmentShaderSource, gProgramId))
        return EXIT_FAILURE;

    // Load textures

    const char* texFilename = "../images/grass.png";
    if (!UCreateTexture(texFilename, gTextureIdgrass))
    {
        cout << "Failed to load texture " << texFilename << endl;
        return EXIT_FAILURE;
    }
    texFilename = "../images/wood.png";
    if (!UCreateTexture(texFilename, gTextureIdtableLeg3))
    {
        cout << "Failed to load texture " << texFilename << endl;
        return EXIT_FAILURE;
    }
    texFilename = "../images/wood.png";
    if (!UCreateTexture(texFilename, gTextureIdtabletop))
    {
        cout << "Failed to load texture " << texFilename << endl;
        return EXIT_FAILURE;
    }
    texFilename = "../images/flower.png";
    if (!UCreateTexture(texFilename, gTextureIdflowers))
    {
        cout << "Failed to load texture " << texFilename << endl;
        return EXIT_FAILURE;
    }
    texFilename = "../images/flower.png";
    if (!UCreateTexture(texFilename, gTextureIdflowers2))
    {
        cout << "Failed to load texture " << texFilename << endl;
        return EXIT_FAILURE;
    }
    texFilename = "../images/flower.png";
    if (!UCreateTexture(texFilename, gTextureIdflowers3))
    {
        cout << "Failed to load texture " << texFilename << endl;
        return EXIT_FAILURE;
    }
    texFilename = "../images/flower.png";
    if (!UCreateTexture(texFilename, gTextureIdflowers4))
    {
        cout << "Failed to load texture " << texFilename << endl;
        return EXIT_FAILURE;
    }
    texFilename = "../images/flower.png";
    if (!UCreateTexture(texFilename, gTextureIdflowers5))
    {
        cout << "Failed to load texture " << texFilename << endl;
        return EXIT_FAILURE;
    }
    texFilename = "../images/flower.png";
    if (!UCreateTexture(texFilename, gTextureIdflowers6))
    {
        cout << "Failed to load texture " << texFilename << endl;
        return EXIT_FAILURE;
    }
    texFilename = "../images/grass.png";
    if (!UCreateTexture(texFilename, gTextureIdgrassColor))
    {
        cout << "Failed to load texture " << texFilename << endl;
        return EXIT_FAILURE;
    }
    texFilename = "../images/wood.png";
    if (!UCreateTexture(texFilename, gTextureIdtableLeg3Color))
    {
        cout << "Failed to load texture " << texFilename << endl;
        return EXIT_FAILURE;
    }
    texFilename = "../images/wood.png";
    if (!UCreateTexture(texFilename, gTextureIdtableLeg4))
    {
        cout << "Failed to load texture " << texFilename << endl;
        return EXIT_FAILURE;
    }
    texFilename = "../images/cork.png";
    if (!UCreateTexture(texFilename, gTextureIdcoaster))
    {
        cout << "Failed to load texture " << texFilename << endl;
        return EXIT_FAILURE;
    }
    texFilename = "../images/wood.png";
    if (!UCreateTexture(texFilename, gTextureIdtableLeg1))
    {
        cout << "Failed to load texture " << texFilename << endl;
        return EXIT_FAILURE;
    }
    texFilename = "../images/blue.png";
    if (!UCreateTexture(texFilename, gTextureIdvase))
    {
        cout << "Failed to load texture " << texFilename << endl;
        return EXIT_FAILURE;
    }
    texFilename = "../images/aluminum.png";
    if (!UCreateTexture(texFilename, gTextureIdbowl))
    {
        cout << "Failed to load texture " << texFilename << endl;
        return EXIT_FAILURE;
    }
    texFilename = "../images/paper.png";
    if (!UCreateTexture(texFilename, gTextureIdplate))
    {
        cout << "Failed to load texture " << texFilename << endl;
        return EXIT_FAILURE;
    }
    // Tell OpenGL for each sampler which texture unit it belongs to (only has to be done once).
    glUseProgram(gProgramId);


    glUniform1i(glGetUniformLocation(gProgramId, "uTexturePlane"), 0);
    glUniform1i(glGetUniformLocation(gProgramId, "uTexturetableLeg3"), 1);
    glUniform1i(glGetUniformLocation(gProgramId, "uTexturetabletop"), 2);
    glUniform1i(glGetUniformLocation(gProgramId, "uTextureflowers"), 3);
    glUniform1i(glGetUniformLocation(gProgramId, "uTextureflowers2"), 4);
    glUniform1i(glGetUniformLocation(gProgramId, "uTextureflowers3"), 5);
    glUniform1i(glGetUniformLocation(gProgramId, "uTextureflowers4"), 6);
    glUniform1i(glGetUniformLocation(gProgramId, "uTextureflowers5"), 7);
    glUniform1i(glGetUniformLocation(gProgramId, "uTextureflowers6"), 8);
    glUniform1i(glGetUniformLocation(gProgramId, "uTexturePlaneColor"), 9);
    glUniform1i(glGetUniformLocation(gProgramId, "uTexturetableLeg3Color"), 10);
    glUniform1i(glGetUniformLocation(gProgramId, "uTexturetableLeg4Color"), 11);
    glUniform1i(glGetUniformLocation(gProgramId, "uTexturecoaster"), 12);
    glUniform1i(glGetUniformLocation(gProgramId, "uTexturetableLeg1"), 13);



    // Sets the background color of the window to black (it will be implicitely used by glClear)
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    // render loop
    // -----------
    while (!glfwWindowShouldClose(gWindow))
    {
        // per-frame timing
        // --------------------
        float currentFrame = glfwGetTime();
        gDeltaTime = currentFrame - gLastFrame;
        gLastFrame = currentFrame;

        // input
        // -----
        UProcessInput(gWindow);

        // Render this frame
        URender();

        glfwPollEvents();
    }

    // Release mesh data
    UDestroyMesh(gMesh);

    // Release texture
    UDestroyTexture(gTextureIdgrass);
    UDestroyTexture(gTextureIdtableLeg3);
    UDestroyTexture(gTextureIdflowers);
    UDestroyTexture(gTextureIdflowers2);
    UDestroyTexture(gTextureIdflowers3);
    UDestroyTexture(gTextureIdflowers4);
    UDestroyTexture(gTextureIdflowers5);
    UDestroyTexture(gTextureIdflowers6);
    UDestroyTexture(gTextureIdtabletop);
    UDestroyTexture(gTextureIdgrassColor);
    UDestroyTexture(gTextureIdtableLeg3Color);
    UDestroyTexture(gTextureIdcoaster);
    UDestroyTexture(gTextureIdtableLeg1);
    UDestroyTexture(gTextureIdbowl);
    UDestroyTexture(gTextureIdplate);
    UDestroyTexture(gTextureIdvase);

    // Release shader program
    UDestroyShaderProgram(gProgramId);

    exit(EXIT_SUCCESS); // Terminates the program successfully
}


// Initialize GLFW, GLEW, and create a window
bool UInitialize(int argc, char* argv[], GLFWwindow** window)
{
    // GLFW: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // GLFW: window creation
    // ---------------------
    * window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, NULL, NULL);
    if (*window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return false;
    }
    glfwMakeContextCurrent(*window);
    glfwSetFramebufferSizeCallback(*window, UResizeWindow);
    glfwSetCursorPosCallback(*window, UMousePositionCallback);
    glfwSetScrollCallback(*window, UMouseScrollCallback);
    glfwSetMouseButtonCallback(*window, UMouseButtonCallback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(*window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // GLEW: initialize
    // ----------------
    // Note: if using GLEW version 1.13 or earlier
    glewExperimental = GL_TRUE;
    GLenum GlewInitResult = glewInit();

    if (GLEW_OK != GlewInitResult)
    {
        std::cerr << glewGetErrorString(GlewInitResult) << std::endl;
        return false;
    }

    // Displays GPU OpenGL version
    cout << "INFO: OpenGL Version: " << glGetString(GL_VERSION) << endl;

    return true;
}


// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
void UProcessInput(GLFWwindow* window)
{
    //static float cameraSpeed = 2.5f;
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }

    //moves camera forward and backward
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        gCamera.ProcessKeyboard(FORWARD, gDeltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        gCamera.ProcessKeyboard(BACKWARD, gDeltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        gCamera.ProcessKeyboard(FORWARD, gDeltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        gCamera.ProcessKeyboard(BACKWARD, gDeltaTime);
    }

    //moves camera left and right
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        gCamera.ProcessKeyboard(LEFT, gDeltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        gCamera.ProcessKeyboard(RIGHT, gDeltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
        gCamera.ProcessKeyboard(LEFT, gDeltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        gCamera.ProcessKeyboard(RIGHT, gDeltaTime);
    }

}


// glfw: whenever the window size changed (by OS or user resize) this callback function executes
void UResizeWindow(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}



// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void UMousePositionCallback(GLFWwindow* window, double xpos, double ypos)
{
    if (gFirstMouse)
    {
        gLastX = xpos;
        gLastY = ypos;
        gFirstMouse = false;
    }

    float xoffset = xpos - gLastX;
    float yoffset = gLastY - ypos; // reversed since y-coordinates go from bottom to top

    gLastX = xpos;
    gLastY = ypos;

    gCamera.ProcessMouseMovement(xoffset, yoffset);
}


// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void UMouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
    gCamera.ProcessMouseScroll(yoffset);
}

// glfw: handle mouse button events
// --------------------------------
void UMouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
    switch (button)
    {
    case GLFW_MOUSE_BUTTON_LEFT:
    {
        if (action == GLFW_PRESS)
            cout << "Left mouse button pressed" << endl;
        else
            cout << "Left mouse button released" << endl;
    }
    break;

    case GLFW_MOUSE_BUTTON_MIDDLE:
    {
        if (action == GLFW_PRESS)
            cout << "Middle mouse button pressed" << endl;
        else
            cout << "Middle mouse button released" << endl;
    }
    break;

    case GLFW_MOUSE_BUTTON_RIGHT:
    {
        if (action == GLFW_PRESS)
            cout << "Right mouse button pressed" << endl;
        else
            cout << "Right mouse button released" << endl;
    }
    break;

    default:
        cout << "Unhandled mouse button event" << endl;
        break;
    }
}


// Function called to render a frame
void URender()
{
    // Enable z-depth
    glEnable(GL_DEPTH_TEST);
    // Clear the frame and z buffers
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    glm::mat4 scale = glm::scale(glm::vec3(1.0f, 1.0f, 1.0f));

    glm::mat4 rotation = glm::rotate(0.0f, glm::vec3(1.0, 1.0f, 1.0f));

    glm::mat4 translation = glm::translate(glm::vec3(0.0f, -2.0f, 0.0f));

    glm::mat4 model = translation * rotation * scale;

    // camera/view transformation
    glm::mat4 view = gCamera.GetViewMatrix();

    // Creates a perspective projection
    glm::mat4 projection = glm::ortho(-10.0f, 10.0f, -10.0f, 100.0f, -100.0f, 100.0f);
    if (!ortho) {
        projection = glm::perspective(glm::radians(gCamera.Zoom), (GLfloat)WINDOW_WIDTH / (GLfloat)WINDOW_HEIGHT, 0.1f, 100.0f);
    }
    if (ortho) {
        projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, -10.0f, 10.0f);
    };
    // Set the shader to be used
    glUseProgram(gProgramId);

    // Retrieves and passes transform matrices to the Shader program
    GLint modelLoc = glGetUniformLocation(gProgramId, "model");
    GLint viewLoc = glGetUniformLocation(gProgramId, "view");
    GLint projLoc = glGetUniformLocation(gProgramId, "projection");

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

    //render boxes

    //plane
    glBindVertexArray(gMesh.planeVAO);
    model = glm::mat4(1.0f);

    //bind textures on corresponding texture units

    glBindTexture(GL_TEXTURE_2D, gTextureIdgrass);

    //Draws the triangles
    glDrawArrays(GL_TRIANGLES, 0, gMesh.nPlaneVertices);



    /////////////////////////////////////draw vase
    glm::mat4 vaseScale = glm::scale(glm::vec3(0.30f, 0.04f, 0.3f));
    glm::mat4 vaseRotation = glm::rotate(3.0f, glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 vaseTranslation = glm::translate(glm::vec3(8.0f, 11.7f, -5.0f));
    glm::mat4 vaseModel = vaseTranslation * vaseRotation * vaseScale;

    // Retrieves and passes transform matrices to the Shader program
    GLint vasemodelLoc = glGetUniformLocation(gProgramId, "model");
    GLint vaseviewLoc = glGetUniformLocation(gProgramId, "view");
    GLint vaseprojLoc = glGetUniformLocation(gProgramId, "projection");

    glUniformMatrix4fv(vasemodelLoc, 1, GL_FALSE, glm::value_ptr(vaseModel));
    glUniformMatrix4fv(vaseviewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(vaseprojLoc, 1, GL_FALSE, glm::value_ptr(projection));

    // Activate the VBOs contained within the mesh's VAO
    glBindVertexArray(gMesh.vaseVAO);
    model = glm::mat4(1.0f);

    //bind textures on corresponding texture units
    glActiveTexture(GL_TEXTURE0);
    //glBindTexture(GL_TEXTURE_2D, gTextureIdDice1); // vasebasecolor
    glBindTexture(GL_TEXTURE_2D, gTextureIdvase);

    // Draws the triangles
    glDrawArrays(GL_TRIANGLES, 0, gMesh.nvaseVertices);

    //////////////////////////////////////////////tabletop
    glm::mat4 tabletopScale = glm::scale(glm::vec3(1.0f, 1.0f, 1.0f));
    glm::mat4 tabletopRotation = glm::rotate(0.0f, glm::vec3(1.0f, 1.0f, 0.0f));
    glm::mat4 tabletopTranslation = glm::translate(glm::vec3(0.0f, 0.0f, 0.0f));
    glm::mat4 tabletopModel = tabletopTranslation * tabletopRotation * tabletopScale;

    // Retrieves and passes transform matrices to the Shader program
    GLint tabletopModelLoc = glGetUniformLocation(gProgramId, "model");
    GLint tabletopViewLoc = glGetUniformLocation(gProgramId, "view");
    GLint tabletopProjLoc = glGetUniformLocation(gProgramId, "projection");

    glUniformMatrix4fv(tabletopModelLoc, 1, GL_FALSE, glm::value_ptr(tabletopModel));
    glUniformMatrix4fv(tabletopViewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(tabletopProjLoc, 1, GL_FALSE, glm::value_ptr(projection));

    // Activate the VBOs contained within the mesh's VAO
    glBindVertexArray(gMesh.tabletopVAO);
    model = glm::mat4(1.0f);
    //bind textures on corresponding texture units
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, gTextureIdtabletop);

    // Draws the triangles
    glDrawArrays(GL_TRIANGLES, 0, gMesh.ntabletopVertices);

    //////////////////////////////////////////////tableLeg1
    glm::mat4 tableLeg1Scale = glm::scale(glm::vec3(1.0f, 1.0f, 1.0f));
    glm::mat4 tableLeg1Rotation = glm::rotate(0.0f, glm::vec3(1.0f, 0.0f, 0.0f));
    glm::mat4 tableLeg1Translation = glm::translate(glm::vec3(0.0f, 0.0f, 0.0f));
    glm::mat4 tableLeg1Model = tableLeg1Translation * tableLeg1Rotation * tableLeg1Scale;

    // Retrieves and passes transform matrices to the Shader program
    GLint tableLeg1ModelLoc = glGetUniformLocation(gProgramId, "model");
    GLint tableLeg1ViewLoc = glGetUniformLocation(gProgramId, "view");
    GLint tableLeg1ProjLoc = glGetUniformLocation(gProgramId, "projection");

    glUniformMatrix4fv(tableLeg1ModelLoc, 1, GL_FALSE, glm::value_ptr(tableLeg1Model));
    glUniformMatrix4fv(tableLeg1ViewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(tableLeg1ProjLoc, 1, GL_FALSE, glm::value_ptr(projection));

    // Activate the VBOs contained within the mesh's VAO
    glBindVertexArray(gMesh.tableLeg1VAO);
    model = glm::mat4(1.0f);
    //bind textures on corresponding texture units
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, gTextureIdtableLeg1);

    // Draws the triangles
    glDrawArrays(GL_TRIANGLES, 0, gMesh.ntableLeg1Vertices);

    ////////////////////////////////////////tabletop handle
    glm::mat4 tableLeg2Scale = glm::scale(glm::vec3(1.0f, 1.0f, 1.0f));
    glm::mat4 tableLeg2Rotation = glm::rotate(0.0f, glm::vec3(1.0f, 0.0f, 0.0f));
    glm::mat4 tableLeg2Translation = glm::translate(glm::vec3(19.0f, 0.0f, 0.0f));
    glm::mat4 tableLeg2Model = tableLeg2Translation * tableLeg2Rotation * tableLeg2Scale;

    // Retrieves and passes transform matrices to the Shader program
    GLint tableLeg2ModelLoc = glGetUniformLocation(gProgramId, "model");
    GLint tableLeg2ViewLoc = glGetUniformLocation(gProgramId, "view");
    GLint tableLeg2ProjLoc = glGetUniformLocation(gProgramId, "projection");

    glUniformMatrix4fv(tableLeg2ModelLoc, 1, GL_FALSE, glm::value_ptr(tableLeg2Model));
    glUniformMatrix4fv(tableLeg2ViewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(tableLeg2ProjLoc, 1, GL_FALSE, glm::value_ptr(projection));

    // Activate the VBOs contained within the mesh's VAO
    glBindVertexArray(gMesh.tableLeg2VAO);
    model = glm::mat4(1.0f);

    //bind textures on corresponding texture units
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, gTextureIdtabletop);

    // Draws the triangles
    glDrawArrays(GL_TRIANGLES, 0, gMesh.ntableLeg2Vertices);

    ////////////////////////////////////////tableLeg3
    glm::mat4 tableLeg3Scale = glm::scale(glm::vec3(1.0f, 1.0f, 1.0f));
    glm::mat4 tableLeg3Rotation = glm::rotate(0.0f, glm::vec3(1.0f, 0.0f, 0.0f));
    glm::mat4 tableLeg3Translation = glm::translate(glm::vec3(0.0f, 0.0f, -10.0f));
    glm::mat4 tableLeg3Model = tableLeg3Translation * tableLeg3Rotation * tableLeg3Scale;

    // Retrieves and passes transform matrices to the Shader program
    GLint tableLeg3ModelLoc = glGetUniformLocation(gProgramId, "model");
    GLint tableLeg3ViewLoc = glGetUniformLocation(gProgramId, "view");
    GLint tableLeg3ProjLoc = glGetUniformLocation(gProgramId, "projection");

    glUniformMatrix4fv(tableLeg3ModelLoc, 1, GL_FALSE, glm::value_ptr(tableLeg3Model));
    glUniformMatrix4fv(tableLeg3ViewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(tableLeg3ProjLoc, 1, GL_FALSE, glm::value_ptr(projection));

    // Activate the VBOs contained within the mesh's VAO
    glBindVertexArray(gMesh.tableLeg3VAO);
    model = glm::mat4(1.0f);

    //bind textures on corresponding texture units
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, gTextureIdtableLeg3Color);

    // Draws the triangles
    glDrawArrays(GL_TRIANGLES, 0, gMesh.ntableLeg3Vertices);


    ////////////////////////////////////////tableLeg4
    glm::mat4 tableLeg4Scale = glm::scale(glm::vec3(1.0f, 1.0f, 1.0f));
    glm::mat4 tableLeg4Rotation = glm::rotate(0.0f, glm::vec3(1.0f, 0.0f, 0.0f));
    glm::mat4 tableLeg4Translation = glm::translate(glm::vec3(19.0f, 0.0f, -10.0f));
    glm::mat4 tableLeg4Model = tableLeg4Translation * tableLeg4Rotation * tableLeg4Scale;

    // Retrieves and passes transform matrices to the Shader program
    GLint tableLeg4ModelLoc = glGetUniformLocation(gProgramId, "model");
    GLint tableLeg4ViewLoc = glGetUniformLocation(gProgramId, "view");
    GLint tableLeg4ProjLoc = glGetUniformLocation(gProgramId, "projection");

    glUniformMatrix4fv(tableLeg4ModelLoc, 1, GL_FALSE, glm::value_ptr(tableLeg4Model));
    glUniformMatrix4fv(tableLeg4ViewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(tableLeg4ProjLoc, 1, GL_FALSE, glm::value_ptr(projection));

    //draw tableLeg4
    // Activate the VBOs contained within the mesh's VAO
    glBindVertexArray(gMesh.tableLeg4VAO);
    model = glm::mat4(1.0f);

    //bind textures on corresponding texture units
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, gTextureIdtableLeg4);

    // Draws the triangles
    glDrawArrays(GL_TRIANGLES, 0, gMesh.ntableLeg4Vertices);

    ////////////////////////////////////////coaster
    glm::mat4 coasterScale = glm::scale(glm::vec3(0.40f, 0.0080f, 0.4f));
    glm::mat4 coasterRotation = glm::rotate(3.0f, glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 coasterTranslation = glm::translate(glm::vec3(4.0f, 11.1f, -5.0f));
    glm::mat4 coasterModel = coasterTranslation * coasterRotation * coasterScale;

    // Retrieves and passes transform matrices to the Shader program
    GLint coasterModelLoc = glGetUniformLocation(gProgramId, "model");
    GLint coasterViewLoc = glGetUniformLocation(gProgramId, "view");
    GLint coasterProjLoc = glGetUniformLocation(gProgramId, "projection");

    glUniformMatrix4fv(coasterModelLoc, 1, GL_FALSE, glm::value_ptr(coasterModel));
    glUniformMatrix4fv(coasterViewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(coasterProjLoc, 1, GL_FALSE, glm::value_ptr(projection));

    // Activate the VBOs contained within the mesh's VAO
    glBindVertexArray(gMesh.coasterVAO);
    model = glm::mat4(1.0f);

    //bind textures on corresponding texture units
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, gTextureIdcoaster);

    // Draws the triangles
    glDrawArrays(GL_TRIANGLES, 0, gMesh.ncoasterVertices);
    /////////////////////////////////////////////flowers
    glm::mat4 flowersScale = glm::scale(glm::vec3(1.0f, 1.0f, 1.0));
    glm::mat4 flowersRotation = glm::rotate(45.0f, glm::vec3(1.34f, 1.0f, 0.53f));
    glm::mat4 flowersTranslation = glm::translate(glm::vec3(7.3f, 14.0f, -5.0f));
    glm::mat4 flowersModel = flowersTranslation * flowersRotation * flowersScale;

    // Retrieves and passes transform matrices to the Shader program
    GLint flowersmodelLoc = glGetUniformLocation(gProgramId, "model");
    GLint flowersviewLoc = glGetUniformLocation(gProgramId, "view");
    GLint flowersprojLoc = glGetUniformLocation(gProgramId, "projection");

    glUniformMatrix4fv(flowersmodelLoc, 1, GL_FALSE, glm::value_ptr(flowersModel));
    glUniformMatrix4fv(flowersviewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(flowersprojLoc, 1, GL_FALSE, glm::value_ptr(projection));

    //draw flowers
    // Activate the VBOs contained within the mesh's VAO
    glBindVertexArray(gMesh.flowersVAO);
    model = glm::mat4(1.0f);
    //bind textures on corresponding texture units
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, gTextureIdflowers);

    // Draws the triangles
    glDrawArrays(GL_TRIANGLES, 0, gMesh.nflowersVertices);

    ////////////////////////////////

    /////////////////////////////////////////////flowers22
    glm::mat4 flowers2Scale = glm::scale(glm::vec3(1.3f, 1.3f, 1.3));
    glm::mat4 flowers2Rotation = glm::rotate(45.0f, glm::vec3(1.64f, 0.23f, 0.83f));
    glm::mat4 flowers2Translation = glm::translate(glm::vec3(8.1f, 14.0f, -4.5f));
    glm::mat4 flowers2Model = flowers2Translation * flowers2Rotation * flowers2Scale;

    // Retrieves and passes transform matrices to the Shader program
    GLint flowers2modelLoc = glGetUniformLocation(gProgramId, "model");
    GLint flowers2viewLoc = glGetUniformLocation(gProgramId, "view");
    GLint flowers2projLoc = glGetUniformLocation(gProgramId, "projection");

    glUniformMatrix4fv(flowers2modelLoc, 1, GL_FALSE, glm::value_ptr(flowers2Model));
    glUniformMatrix4fv(flowers2viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(flowers2projLoc, 1, GL_FALSE, glm::value_ptr(projection));

    //draw flowers2
    // Activate the VBOs contained within the mesh's VAO
    glBindVertexArray(gMesh.flowers2VAO);
    model = glm::mat4(1.0f);
    //bind textures on corresponding texture units
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, gTextureIdflowers2);

    // Draws the triangles
    glDrawArrays(GL_TRIANGLES, 0, gMesh.nflowers2Vertices);

    ////////////////////////////////

    /////////////////////////////////////////////flowers3
    glm::mat4 flowers3Scale = glm::scale(glm::vec3(1.1f, 1.1f, 1.1));
    glm::mat4 flowers3Rotation = glm::rotate(45.0f, glm::vec3(1.34f, 1.0f, 0.53f));
    glm::mat4 flowers3Translation = glm::translate(glm::vec3(7.8f, 14.0f, -6.0f));
    glm::mat4 flowers3Model = flowers3Translation * flowers3Rotation * flowers3Scale;

    // Retrieves and passes transform matrices to the Shader program
    GLint flowers3modelLoc = glGetUniformLocation(gProgramId, "model");
    GLint flowers3viewLoc = glGetUniformLocation(gProgramId, "view");
    GLint flowers3projLoc = glGetUniformLocation(gProgramId, "projection");

    glUniformMatrix4fv(flowers3modelLoc, 1, GL_FALSE, glm::value_ptr(flowers3Model));
    glUniformMatrix4fv(flowers3viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(flowers3projLoc, 1, GL_FALSE, glm::value_ptr(projection));

    //draw flowers3
    // Activate the VBOs contained within the mesh's VAO
    glBindVertexArray(gMesh.flowers3VAO);
    model = glm::mat4(1.0f);
    //bind textures on corresponding texture units
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, gTextureIdflowers3);

    // Draws the triangles
    glDrawArrays(GL_TRIANGLES, 0, gMesh.nflowers3Vertices);

    ////////////////////////////////

    /////////////////////////////////////////////flowers44
    glm::mat4 flowers4Scale = glm::scale(glm::vec3(1.0f, 1.0f, 1.0));
    glm::mat4 flowers4Rotation = glm::rotate(45.0f, glm::vec3(1.0f, 1.0f, 1.0f));
    glm::mat4 flowers4Translation = glm::translate(glm::vec3(8.7f, 14.0f, -5.0f));
    glm::mat4 flowers4Model = flowers4Translation * flowers4Rotation * flowers4Scale;

    // Retrieves and passes transform matrices to the Shader program
    GLint flowers4modelLoc = glGetUniformLocation(gProgramId, "model");
    GLint flowers4viewLoc = glGetUniformLocation(gProgramId, "view");
    GLint flowers4projLoc = glGetUniformLocation(gProgramId, "projection");

    glUniformMatrix4fv(flowers4modelLoc, 1, GL_FALSE, glm::value_ptr(flowers4Model));
    glUniformMatrix4fv(flowers4viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(flowers4projLoc, 1, GL_FALSE, glm::value_ptr(projection));

    //draw flowers4
    // Activate the VBOs contained within the mesh's VAO
    glBindVertexArray(gMesh.flowers4VAO);
    model = glm::mat4(1.0f);
    //bind textures on corresponding texture units
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, gTextureIdflowers4);

    // Draws the triangles
    glDrawArrays(GL_TRIANGLES, 0, gMesh.nflowers4Vertices);

    ////////////////////////////////

    /////////////////////////////////////////////flowers55
    glm::mat4 flowers5Scale = glm::scale(glm::vec3(1.5f, 1.5f, 1.5));
    glm::mat4 flowers5Rotation = glm::rotate(45.0f, glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 flowers5Translation = glm::translate(glm::vec3(8.0f, 14.7f, -5.0f));
    glm::mat4 flowers5Model = flowers5Translation * flowers5Rotation * flowers5Scale;

    // Retrieves and passes transform matrices to the Shader program
    GLint flowers5modelLoc = glGetUniformLocation(gProgramId, "model");
    GLint flowers5viewLoc = glGetUniformLocation(gProgramId, "view");
    GLint flowers5projLoc = glGetUniformLocation(gProgramId, "projection");

    glUniformMatrix4fv(flowers5modelLoc, 1, GL_FALSE, glm::value_ptr(flowers5Model));
    glUniformMatrix4fv(flowers5viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(flowers5projLoc, 1, GL_FALSE, glm::value_ptr(projection));

    //draw flowers5
    // Activate the VBOs contained within the mesh's VAO
    glBindVertexArray(gMesh.flowers5VAO);
    model = glm::mat4(1.0f);
    //bind textures on corresponding texture units
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, gTextureIdflowers5);

    // Draws the triangles
    glDrawArrays(GL_TRIANGLES, 0, gMesh.nflowers5Vertices);

    ////////////////////////////////

    /////////////////////////////////////////////flowers6
    glm::mat4 flowers6Scale = glm::scale(glm::vec3(1.0f, 1.0f, 1.0));
    glm::mat4 flowers6Rotation = glm::rotate(45.0f, glm::vec3(1.34f, 1.0f, 0.53f));
    glm::mat4 flowers6Translation = glm::translate(glm::vec3(7.3f, 14.0f, -5.0f));
    glm::mat4 flowers6Model = flowers6Translation * flowers6Rotation * flowers6Scale;

    // Retrieves and passes transform matrices to the Shader program
    GLint flowers6modelLoc = glGetUniformLocation(gProgramId, "model");
    GLint flowers6viewLoc = glGetUniformLocation(gProgramId, "view");
    GLint flowers6projLoc = glGetUniformLocation(gProgramId, "projection");

    glUniformMatrix4fv(flowers6modelLoc, 1, GL_FALSE, glm::value_ptr(flowers6Model));
    glUniformMatrix4fv(flowers6viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(flowers6projLoc, 1, GL_FALSE, glm::value_ptr(projection));

    //draw flowers6
    // Activate the VBOs contained within the mesh's VAO
    glBindVertexArray(gMesh.flowers6VAO);
    model = glm::mat4(1.0f);
    //bind textures on corresponding texture units
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, gTextureIdflowers6);

    // Draws the triangles
    glDrawArrays(GL_TRIANGLES, 0, gMesh.nflowers6Vertices);

    ////////////////////////////////

    /////////////////////////////////////////////bowl
    glm::mat4 bowlScale = glm::scale(glm::vec3(0.5f, 0.03f, 0.5f));
    glm::mat4 bowlRotation = glm::rotate(15.0f, glm::vec3(0.0f, 2.0f, 0.0f));
    glm::mat4 bowlTranslation = glm::translate(glm::vec3(14.0f, 11.7f, -7.5f));
    glm::mat4 bowlModel = bowlTranslation * bowlRotation * bowlScale;

    // Retrieves and passes transform matrices to the Shader program
    GLint bowlmodelLoc = glGetUniformLocation(gProgramId, "model");
    GLint bowlviewLoc = glGetUniformLocation(gProgramId, "view");
    GLint bowlprojLoc = glGetUniformLocation(gProgramId, "projection");

    glUniformMatrix4fv(bowlmodelLoc, 1, GL_FALSE, glm::value_ptr(bowlModel));
    glUniformMatrix4fv(bowlviewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(bowlprojLoc, 1, GL_FALSE, glm::value_ptr(projection));

    // Activate the VBOs contained within the mesh's VAO
    glBindVertexArray(gMesh.bowlVAO);
    model = glm::mat4(1.0f);
    //bind textures on corresponding texture units
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, gTextureIdbowl);

    // Draws the triangles
    glDrawArrays(GL_TRIANGLES, 0, gMesh.nbowlVertices);

    /////////////////////////////////////////////plate
    glm::mat4 plateScale = glm::scale(glm::vec3(0.4f, 0.005f, 0.4f));
    glm::mat4 plateRotation = glm::rotate(15.0f, glm::vec3(0.0f, 2.0f, 0.0f));
    glm::mat4 plateTranslation = glm::translate(glm::vec3(14.0f, 11.2f, -2.0f));
    glm::mat4 plateModel = plateTranslation * plateRotation * plateScale;

    // Retrieves and passes transform matrices to the Shader program
    GLint platemodelLoc = glGetUniformLocation(gProgramId, "model");
    GLint plateviewLoc = glGetUniformLocation(gProgramId, "view");
    GLint plateprojLoc = glGetUniformLocation(gProgramId, "projection");

    glUniformMatrix4fv(platemodelLoc, 1, GL_FALSE, glm::value_ptr(plateModel));
    glUniformMatrix4fv(plateviewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(plateprojLoc, 1, GL_FALSE, glm::value_ptr(projection));

    // Activate the VBOs contained within the mesh's VAO
    glBindVertexArray(gMesh.plateVAO);
    model = glm::mat4(1.0f);
    //bind textures on corresponding texture units
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, gTextureIdplate);

    // Draws the triangles
    glDrawArrays(GL_TRIANGLES, 0, gMesh.nplateVertices);

    /////////////////////////////////////////////

    // Deactivate the Vertex Array Object
    glBindVertexArray(0);

    // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
    glfwSwapBuffers(gWindow);    // Flips the the back buffer with the front buffer every frame.

}



void UCreateTexturedMesh(GLMesh& mesh)
{
    // Vertex data
    GLfloat planeVerts[] = {
        -10.0f,  2.4f, -20.0f, 0.0f, 0.0f,      //top right
        -10.0f,  2.4f,  20.0f, 1.0f, 0.0f,      //bottom right
         30.0f,  2.4f,  20.0f, 1.0f, 1.0f,      //bottom left
         30.0f,  2.4f,  20.0f, 1.0f, 1.0f,      //bottom left
         30.0f,  2.4f, -20.0f, 0.0f, 1.0f,      //top left
        -10.0f,  2.4f, -20.0f, 0.0f, 0.0f,      //top right
    };

    
    GLfloat vaseVerts[] = {
         -3.0f,  60.0f,  3.0f, 0.0f, 0.0f,
        -3.0f, -20.0f,  3.0f, 1.0f, 0.0f,
         0.0f, -20.0f,  4.0f, 1.0f, 1.0f,
         0.0f, -20.0f,  4.0f, 1.0f, 1.0f,
         0.0f,  60.0f,  4.0f, 0.0f, 1.0f,
        -3.0f,  60.0f,  3.0f, 0.0f, 0.0f,

         0.0f,  60.0f,  4.0f, 0.0f, 0.0f,
         0.0f, -20.0f,  4.0f, 1.0f, 0.0f,
         3.0f, -20.0f,  3.0f, 1.0f, 1.0f,
         3.0f, -20.0f,  3.0f, 1.0f, 1.0f,
         3.0f,  60.0f,  3.0f, 0.0f, 1.0f,
         0.0f,  60.0f,  4.0f, 0.0f, 0.0f,

         3.0f,  60.0f,  3.0f, 0.0f, 0.0f,
         3.0f, -20.0f,  3.0f, 1.0f, 0.0f,
         4.0f, -20.0f,  0.0f, 1.0f, 1.0f,
         4.0f, -20.0f,  0.0f, 1.0f, 1.0f,
         4.0f,  60.0f,  0.0f, 0.0f, 1.0f,
         3.0f,  60.0f,  3.0f, 0.0f, 0.0f,

         4.0f,  60.0f,  0.0f, 0.0f, 0.0f,
         4.0f, -20.0f,  0.0f, 1.0f, 0.0f,
         3.0f, -20.0f, -3.0f, 1.0f, 1.0f,
         3.0f, -20.0f, -3.0f, 1.0f, 1.0f,
         3.0f,  60.0f, -3.0f, 0.0f, 1.0f,
         4.0f,  60.0f,  0.0f, 0.0f, 0.0f,

         3.0f,  60.0f, -3.0f, 0.0f, 0.0f,
         3.0f, -20.0f, -3.0f, 1.0f, 0.0f,
         0.0f, -20.0f, -4.0f, 1.0f, 1.0f,
         0.0f, -20.0f, -4.0f, 1.0f, 1.0f,
         0.0f,  60.0f, -4.0f, 0.0f, 1.0f,
         3.0f,  60.0f, -3.0f, 0.0f, 0.0f,

         0.0f,  60.0f, -4.0f, 0.0f, 0.0f,
         0.0f, -20.0f, -4.0f, 1.0f, 0.0f,
        -3.0f, -20.0f, -3.0f, 1.0f, 1.0f,
        -3.0f, -20.0f, -3.0f, 1.0f, 1.0f,
        -3.0f,  60.0f, -3.0f, 0.0f, 1.0f,
         0.0f,  60.0f, -4.0f, 0.0f, 0.0f,

        -3.0f,  60.0f, -3.0f, 0.0f, 0.0f,
        -3.0f, -20.0f, -3.0f, 1.0f, 0.0f,
        -4.0f, -20.0f,  0.0f, 1.0f, 1.0f,
        -4.0f, -20.0f,  0.0f, 1.0f, 1.0f,
        -4.0f,  60.0f,  0.0f, 0.0f, 1.0f,
        -3.0f,  60.0f, -3.0f, 0.0f, 0.0f,

        -4.0f,  60.0f,  0.0f, 0.0f, 0.0f,
        -4.0f, -20.0f,  0.0f, 1.0f, 0.0f,
        -3.0f, -20.0f,  3.0f, 1.0f, 1.0f,
        -3.0f, -20.0f,  3.0f, 1.0f, 1.0f,
        -3.0f,  60.0f,  3.0f, 0.0f, 1.0f,
        -4.0f,  60.0f,  0.0f, 0.0f, 0.0f,

    };

    

    GLfloat tableLeg2Verts[] = {
        0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        1.0f, 10.0f, 0.0f, 1.0f, 1.0f,
        1.0f, 10.0f, 0.0f, 1.0f, 1.0f,
        0.0f, 10.0f, 0.0f, 0.0f, 1.0f,
         0.0f, 0.0f, 0.0f, 0.0f, 0.0f,

         //Back
          0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
         1.0f, 0.0f, -1.0f, 1.0f, 0.0f,
         1.0f, 10.0f, -1.0f, 1.0f, 1.0f,
         1.0f, 10.0f, -1.0f, 1.0f, 1.0f,
         0.0f, 10.0f, -1.0f, 0.0f, 1.0f,
          0.0f, 0.0f, -1.0f, 0.0f, 0.0f,

          //Left
          0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
          0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
          0.0f, 10.0f, 0.0f, 1.0f, 1.0f,
          0.0f, 10.0f, 0.0f, 1.0f, 1.0f,
          0.0f, 10.0f, -1.0f, 0.0f, 1.0f,
          0.0f, 0.0f, -1.0f, 0.0f, 0.0f,

          //R,ight
          1.0f, 0.0f, -1.0f, 0.0f, 0.0f,
          1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
          1.0f, 10.0f, 0.0f, 1.0f, 1.0f,
          1.0f, 10.0f, 0.0f, 1.0f, 1.0f,
          1.0f, 10.0f, -1.0f, 0.0f, 1.0f,
          1.0f, 0.0f, -1.0f, 0.0f, 0.0f,
    };

    GLfloat tabletopVerts[] = { //tabletop
        -2.0f, 10.0f, 2.0f, 0.0f, 0.0f,
        23.0f, 10.0f, 2.0f, 1.0f, 0.0f,
        23.0f, 11.0f, 2.0f, 1.0f, 1.0f,
        23.0f, 11.0f, 2.0f, 1.0f, 1.0f,
        -2.0f, 11.0f, 2.0f, 0.0f, 1.0f,
        -2.0f, 10.0f, 2.0f, 0.0f, 0.0f,

        //Back
        -2.0f, 10.0f, -13.0f, 0.0f, 0.0f,
        23.0f, 10.0f, -13.0f, 1.0f, 0.0f,
        23.0f, 11.0f, -13.0f, 1.0f, 1.0f,
        23.0f, 11.0f, -13.0f, 1.0f, 1.0f,
        -2.0f, 11.0f, -13.0f, 0.0f, 1.0f,
        -2.0f, 10.0f, -13.0f, 0.0f, 0.0f,

        //Left
        -2.0f, 10.0f, -13.0f, 0.0f, 0.0f,
        -2.0f, 10.0f, 2.0f, 1.0f, 0.0f,
        -2.0f, 11.0f, 2.0f, 1.0f, 1.0f,
        -2.0f, 11.0f, 2.0f, 1.0f, 1.0f,
        -2.0f, 11.0f, -13.0f, 0.0f, 1.0f,
        -2.0f, 10.0f, -13.0f, 0.0f, 0.0f,

        //Right
        23.0f, 10.0f, -13.0f, 0.0f, 0.0f,
        23.0f, 10.0f, 2.0f, 1.0f, 0.0f,
        23.0f, 11.0f, 2.0f, 1.0f, 1.0f,
        23.0f, 11.0f, 2.0f, 1.0f, 1.0f,
        23.0f, 11.0f, -13.0f, 0.0f, 1.0f,
        23.0f, 10.0f, -13.0f, 0.0f, 0.0f,

        //Bottom
        -2.0f, 10.0f, 2.0f, 0.0f, 0.0f,
        23.0f, 10.0f, 2.0f, 1.0f, 0.0f,
        23.0f, 10.0f, -13.0f, 1.0f, 1.0f,
        23.0f, 10.0f, -13.0f, 1.0f, 1.0f,
        -2.0f, 10.0f, -13.0f, 0.0f, 1.0f,
        -2.0f, 10.0f, 2.0f, 0.0f, 0.0f,

        //Top
        -2.0f, 11.0f, 2.0f, 0.0f, 0.0f,
        23.0f, 11.0f, 2.0f, 1.0f, 0.0f,
        23.0f, 11.0f, -13.0f, 1.0f, 1.0f,
        23.0f, 11.0f, -13.0f, 1.0f, 1.0f,
        -2.0f, 11.0f, -13.0f, 0.0f, 1.0f,
        -2.0f, 11.0f, 2.0f, 0.0f, 0.0f,
    };

    GLfloat tableLeg3Verts[] = {
        0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        1.0f, 10.0f, 0.0f, 1.0f, 1.0f,
        1.0f, 10.0f, 0.0f, 1.0f, 1.0f,
        0.0f, 10.0f, 0.0f, 0.0f, 1.0f,
         0.0f, 0.0f, 0.0f, 0.0f, 0.0f,

         //Back
          0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
         1.0f, 0.0f, -1.0f, 1.0f, 0.0f,
         1.0f, 10.0f, -1.0f, 1.0f, 1.0f,
         1.0f, 10.0f, -1.0f, 1.0f, 1.0f,
         0.0f, 10.0f, -1.0f, 0.0f, 1.0f,
          0.0f, 0.0f, -1.0f, 0.0f, 0.0f,

          //Left
          0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
          0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
          0.0f, 10.0f, 0.0f, 1.0f, 1.0f,
          0.0f, 10.0f, 0.0f, 1.0f, 1.0f,
          0.0f, 10.0f, -1.0f, 0.0f, 1.0f,
          0.0f, 0.0f, -1.0f, 0.0f, 0.0f,

          //R,ight
          1.0f, 0.0f, -1.0f, 0.0f, 0.0f,
          1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
          1.0f, 10.0f, 0.0f, 1.0f, 1.0f,
          1.0f, 10.0f, 0.0f, 1.0f, 1.0f,
          1.0f, 10.0f, -1.0f, 0.0f, 1.0f,
          1.0f, 0.0f, -1.0f, 0.0f, 0.0f,

    };

    GLfloat tableLeg4Verts[] = {
         0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        1.0f, 10.0f, 0.0f, 1.0f, 1.0f,
        1.0f, 10.0f, 0.0f, 1.0f, 1.0f,
        0.0f, 10.0f, 0.0f, 0.0f, 1.0f,
         0.0f, 0.0f, 0.0f, 0.0f, 0.0f,

        //Back
         0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, -1.0f, 1.0f, 0.0f,
        1.0f, 10.0f, -1.0f, 1.0f, 1.0f,
        1.0f, 10.0f, -1.0f, 1.0f, 1.0f,
        0.0f, 10.0f, -1.0f, 0.0f, 1.0f,
         0.0f, 0.0f, -1.0f, 0.0f, 0.0f,

        //Left
        0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 10.0f, 0.0f, 1.0f, 1.0f,
        0.0f, 10.0f, 0.0f, 1.0f, 1.0f,
        0.0f, 10.0f, -1.0f, 0.0f, 1.0f,
        0.0f, 0.0f, -1.0f, 0.0f, 0.0f,

        //R,ight
        1.0f, 0.0f, -1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        1.0f, 10.0f, 0.0f, 1.0f, 1.0f,
        1.0f, 10.0f, 0.0f, 1.0f, 1.0f,
        1.0f, 10.0f, -1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, -1.0f, 0.0f, 0.0f,
    };

    GLfloat coasterVerts[] = {
        -3.0f,  60.0f,  3.0f, 0.0f, 0.0f,
        -3.0f, -20.0f,  3.0f, 1.0f, 0.0f,
         0.0f, -20.0f,  4.0f, 1.0f, 1.0f,
         0.0f, -20.0f,  4.0f, 1.0f, 1.0f,
         0.0f,  60.0f,  4.0f, 0.0f, 1.0f,
        -3.0f,  60.0f,  3.0f, 0.0f, 0.0f,

         0.0f,  60.0f,  4.0f, 0.0f, 0.0f,
         0.0f, -20.0f,  4.0f, 1.0f, 0.0f,
         3.0f, -20.0f,  3.0f, 1.0f, 1.0f,
         3.0f, -20.0f,  3.0f, 1.0f, 1.0f,
         3.0f,  60.0f,  3.0f, 0.0f, 1.0f,
         0.0f,  60.0f,  4.0f, 0.0f, 0.0f,

         3.0f,  60.0f,  3.0f, 0.0f, 0.0f,
         3.0f, -20.0f,  3.0f, 1.0f, 0.0f,
         4.0f, -20.0f,  0.0f, 1.0f, 1.0f,
         4.0f, -20.0f,  0.0f, 1.0f, 1.0f,
         4.0f,  60.0f,  0.0f, 0.0f, 1.0f,
         3.0f,  60.0f,  3.0f, 0.0f, 0.0f,

         4.0f,  60.0f,  0.0f, 0.0f, 0.0f,
         4.0f, -20.0f,  0.0f, 1.0f, 0.0f,
         3.0f, -20.0f, -3.0f, 1.0f, 1.0f,
         3.0f, -20.0f, -3.0f, 1.0f, 1.0f,
         3.0f,  60.0f, -3.0f, 0.0f, 1.0f,
         4.0f,  60.0f,  0.0f, 0.0f, 0.0f,

         3.0f,  60.0f, -3.0f, 0.0f, 0.0f,
         3.0f, -20.0f, -3.0f, 1.0f, 0.0f,
         0.0f, -20.0f, -4.0f, 1.0f, 1.0f,
         0.0f, -20.0f, -4.0f, 1.0f, 1.0f,
         0.0f,  60.0f, -4.0f, 0.0f, 1.0f,
         3.0f,  60.0f, -3.0f, 0.0f, 0.0f,

         0.0f,  60.0f, -4.0f, 0.0f, 0.0f,
         0.0f, -20.0f, -4.0f, 1.0f, 0.0f,
        -3.0f, -20.0f, -3.0f, 1.0f, 1.0f,
        -3.0f, -20.0f, -3.0f, 1.0f, 1.0f,
        -3.0f,  60.0f, -3.0f, 0.0f, 1.0f,
         0.0f,  60.0f, -4.0f, 0.0f, 0.0f,

        -3.0f,  60.0f, -3.0f, 0.0f, 0.0f,
        -3.0f, -20.0f, -3.0f, 1.0f, 0.0f,
        -4.0f, -20.0f,  0.0f, 1.0f, 1.0f,
        -4.0f, -20.0f,  0.0f, 1.0f, 1.0f,
        -4.0f,  60.0f,  0.0f, 0.0f, 1.0f,
        -3.0f,  60.0f, -3.0f, 0.0f, 0.0f,

        -4.0f,  60.0f,  0.0f, 0.0f, 0.0f,
        -4.0f, -20.0f,  0.0f, 1.0f, 0.0f,
        -3.0f, -20.0f,  3.0f, 1.0f, 1.0f,
        -3.0f, -20.0f,  3.0f, 1.0f, 1.0f,
        -3.0f,  60.0f,  3.0f, 0.0f, 1.0f,
        -4.0f,  60.0f,  0.0f, 0.0f, 0.0f,



         0.0f, 60.0f,  4.0f, 0.0f, 0.0f,
         3.0f, 60.0f,  3.0f, 1.0f, 0.0f,
         4.0f, 60.0f,  0.0f, 1.0f, 1.0f,


         0.0f, 60.0f,  4.0f, 0.0f, 0.0f,
         4.0f, 60.0f,  0.0f, 1.0f, 0.0f,
         3.0f, 60.0f, -3.0f, 1.0f, 1.0f,


         0.0f, 60.0f,  4.0f, 0.0f, 0.0f,
         3.0f, 60.0f, -3.0f, 1.0f, 0.0f,
         0.0f, 60.0f, -4.0f, 1.0f, 1.0f,


         0.0f, 60.0f,  4.0f, 0.0f, 0.0f,
         0.0f, 60.0f, -4.0f, 1.0f, 0.0f,
        -3.0f, 60.0f, -3.0f, 1.0f, 1.0f,


         0.0f, 60.0f,  4.0f, 0.0f, 0.0f,
        -3.0f, 60.0f, -3.0f, 1.0f, 0.0f,
        -4.0f, 60.0f,  0.0f, 1.0f, 1.0f,


         0.0f, 60.0f,  4.0f, 0.0f, 0.0f,
        -4.0f, 60.0f,  0.0f, 1.0f, 0.0f,
        -3.0f, 60.0f,  3.0f, 1.0f, 1.0f,


         0.0f, 60.0f,  4.0f, 0.0f, 0.0f,
        -3.0f, 60.0f,  3.0f, 1.0f, 0.0f,
        -3.0f, 60.0f,  3.0f, 1.0f, 1.0f,



    };

    GLfloat flowersVerts[] = {

        //back
       -0.5f, -0.25f, -0.5f,  0.0f, 0.0f,
        0.5f, -0.25f, -0.5f,  1.0f, 0.0f,
        0.5f,  0.25f, -0.5f,  1.0f, 1.0f,
        0.5f,  0.25f, -0.5f,  1.0f, 1.0f,
       -0.5f,  0.25f, -0.5f,  0.0f, 1.0f,
       -0.5f, -0.25f, -0.5f,  0.0f, 0.0f,

       //front
       -0.5f, -0.25f,  0.5f,  0.0f, 0.0f,
        0.5f, -0.25f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.25f,  0.5f,  1.0f, 1.0f,
        0.5f,  0.25f,  0.5f,  1.0f, 1.0f,
       -0.5f,  0.25f,  0.5f,  0.0f, 1.0f,
       -0.5f, -0.25f,  0.5f,  0.0f, 0.0f,

       //left
       -0.5f,  0.25f,  0.5f,  1.0f, 0.0f,
       -0.5f,  0.25f, -0.5f,  1.0f, 1.0f,
       -0.5f, -0.25f, -0.5f,  0.0f, 1.0f,
       -0.5f, -0.25f, -0.5f,  0.0f, 1.0f,
       -0.5f, -0.25f,  0.5f,  0.0f, 0.0f,
       -0.5f,  0.25f,  0.5f,  1.0f, 0.0f,

       //right
        0.5f,  0.25f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.25f, -0.5f,  1.0f, 1.0f,
        0.5f, -0.25f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.25f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.25f,  0.5f,  0.0f, 0.0f,
        0.5f,  0.25f,  0.5f,  1.0f, 0.0f,






        //front to top
       -0.5f,  0.25f,  0.5f,  0.0f, 0.0f,
        0.5f,  0.25f,  0.5f,  0.0f, 0.0f,
        0.25f,  0.75f,  0.25f,  0.0f, 0.0f,
        0.25f,  0.75f,  0.25f,  0.0f, 0.0f,
       -0.25f,  0.75f, 0.25f,  0.0f, 0.0f,
       -0.5f,  0.25f,  0.5f,  0.0f, 0.0f,

       //back to top
       -0.5f,  0.25f, -0.5f,  0.0f, 0.0f,
        0.5f,  0.25f, -0.5f,  0.0f, 0.0f,
        0.25f,  0.75f,  -0.25f,  0.0f, 0.0f,
        0.25f,  0.75f,  -0.25f,  0.0f, 0.0f,
       -0.25f,  0.75f, -0.25f,  0.0f, 0.0f,
       -0.5f,  0.25f, -0.5f,  0.0f, 0.0f,

       //right to top
        0.5f,  0.25f, -0.5f,  0.0f, 0.0f,
       0.25f,  0.75f, -0.25f,  0.0f, 0.0f,
       0.25f,  0.75f, 0.25f,  0.0f, 0.0f,
       0.25f,  0.75f, 0.25f,  0.0f, 0.0f,
        0.5f,  0.25f, 0.5f,  0.0f, 0.0f,
        0.5f,  0.25f, -0.5f,  0.0f, 0.0f,

        //left to top
        -0.5f,  0.25f, -0.5f,  0.0f, 0.0f,
       -0.25f,  0.75f, -0.25f,  0.0f, 0.0f,
       -0.25f,  0.75f, 0.25f,  0.0f, 0.0f,
       -0.25f,  0.75f, 0.25f,  0.0f, 0.0f,
        -0.5f,  0.25f, 0.5f,  0.0f, 0.0f,
        -0.5f,  0.25f, -0.5f,  0.0f, 0.0f,





        //front to bottom
       -0.5f,  -0.25f,  0.5f,  0.0f, 0.0f,
        0.5f,  -0.25f,  0.5f,  0.0f, 0.0f,
        0.25f,  -0.75f,  0.25f,  0.0f, 0.0f,
        0.25f,  -0.75f,  0.25f,  0.0f, 0.0f,
       -0.25f,  -0.75f, 0.25f,  0.0f, 0.0f,
       -0.5f,  -0.25f,  0.5f,  0.0f, 0.0f,

       //back to bottom
       -0.5f,  -0.25f, -0.5f,  0.0f, 0.0f,
        0.5f,  -0.25f, -0.5f,  0.0f, 0.0f,
        0.25f,  -0.75f,  -0.25f,  0.0f, 0.0f,
        0.25f,  -0.75f,  -0.25f,  0.0f, 0.0f,
       -0.25f,  -0.75f, -0.25f,  0.0f, 0.0f,
       -0.5f,  -0.25f, -0.5f,  0.0f, 0.0f,

       //right to bottom
        0.5f,  -0.25f, -0.5f,  0.0f, 0.0f,
       0.25f,  -0.75f, -0.25f,  0.0f, 0.0f,
       0.25f,  -0.75f, 0.25f,  0.0f, 0.0f,
       0.25f,  -0.75f, 0.25f,  0.0f, 0.0f,
        0.5f,  -0.25f, 0.5f,  0.0f, 0.0f,
        0.5f,  -0.25f, -0.5f,  0.0f, 0.0f,

        //left to bottom
        -0.5f,  -0.25f, -0.5f,  0.0f, 0.0f,
       -0.25f,  -0.75f, -0.25f,  0.0f, 0.0f,
       -0.25f,  -0.75f, 0.25f,  0.0f, 0.0f,
       -0.25f,  -0.75f, 0.25f,  0.0f, 0.0f,
        -0.5f,  -0.25f, 0.5f,  0.0f, 0.0f,
        -0.5f,  -0.25f, -0.5f,  0.0f, 0.0f,




        //bottom
       -0.25f, -0.75f, -0.25f,  0.0f, 1.0f,
        0.25f, -0.75f, -0.25f,  1.0f, 1.0f,
        0.25f, -0.75f,  0.25f,  1.0f, 0.0f,
        0.25f, -0.75f,  0.25f,  1.0f, 0.0f,
       -0.25f, -0.75f,  0.25f,  0.0f, 0.0f,
       -0.25f, -0.75f, -0.25f,  0.0f, 1.0f,

       //top
       -0.25f,  0.75f, -0.25f,  0.0f, 1.0f,
        0.25f,  0.75f, -0.25f,  1.0f, 1.0f,
        0.25f,  0.75f,  0.25f,  1.0f, 0.0f,
        0.25f,  0.75f,  0.25f,  1.0f, 0.0f,
       -0.25f,  0.75f,  0.25f,  0.0f, 0.0f,
       -0.25f,  0.75f, -0.25f,  0.0f, 1.0f
    };

    GLfloat tableLeg1Verts[] = { //front left leg
        0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        1.0f, 10.0f, 0.0f, 1.0f, 1.0f,
        1.0f, 10.0f, 0.0f, 1.0f, 1.0f,
        0.0f, 10.0f, 0.0f, 0.0f, 1.0f,
         0.0f, 0.0f, 0.0f, 0.0f, 0.0f,

         //Back
          0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
         1.0f, 0.0f, -1.0f, 1.0f, 0.0f,
         1.0f, 10.0f, -1.0f, 1.0f, 1.0f,
         1.0f, 10.0f, -1.0f, 1.0f, 1.0f,
         0.0f, 10.0f, -1.0f, 0.0f, 1.0f,
          0.0f, 0.0f, -1.0f, 0.0f, 0.0f,

          //Left
          0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
          0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
          0.0f, 10.0f, 0.0f, 1.0f, 1.0f,
          0.0f, 10.0f, 0.0f, 1.0f, 1.0f,
          0.0f, 10.0f, -1.0f, 0.0f, 1.0f,
          0.0f, 0.0f, -1.0f, 0.0f, 0.0f,

          //R,ight
          1.0f, 0.0f, -1.0f, 0.0f, 0.0f,
          1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
          1.0f, 10.0f, 0.0f, 1.0f, 1.0f,
          1.0f, 10.0f, 0.0f, 1.0f, 1.0f,
          1.0f, 10.0f, -1.0f, 0.0f, 1.0f,
          1.0f, 0.0f, -1.0f, 0.0f, 0.0f,

    };

    GLfloat bowlVerts[] = {
        -4.0f,  60.0f,  4.0f, 0.0f, 0.0f,
        -3.0f, -20.0f,  3.0f, 1.0f, 0.0f,
         0.0f, -20.0f,  4.0f, 1.0f, 1.0f,
         0.0f, -20.0f,  4.0f, 1.0f, 1.0f,
         1.0f,  60.0f,  5.0f, 0.0f, 1.0f,
        -4.0f,  60.0f,  4.0f, 0.0f, 0.0f,

         1.0f,  60.0f,  5.0f, 0.0f, 0.0f,
         0.0f, -20.0f,  4.0f, 1.0f, 0.0f,
         3.0f, -20.0f,  3.0f, 1.0f, 1.0f,
         3.0f, -20.0f,  3.0f, 1.0f, 1.0f,
         4.0f,  60.0f,  4.0f, 0.0f, 1.0f,
         1.0f,  60.0f,  5.0f, 0.0f, 0.0f,



         4.0f,  60.0f,  4.0f, 0.0f, 0.0f,
         3.0f, -20.0f,  3.0f, 1.0f, 0.0f,
         4.0f, -20.0f,  0.0f, 1.0f, 1.0f,
         4.0f, -20.0f,  0.0f, 1.0f, 1.0f,
         5.0f,  60.0f,  2.0f, 0.0f, 1.0f,
         4.0f,  60.0f,  4.0f, 0.0f, 0.0f,

         5.0f,  60.0f,  2.0f, 0.0f, 0.0f,
         4.0f, -20.0f,  0.0f, 1.0f, 0.0f,
         3.0f, -20.0f, -3.0f, 1.0f, 1.0f,
         3.0f, -20.0f, -3.0f, 1.0f, 1.0f,
         4.0f,  60.0f, -4.0f, 0.0f, 1.0f,
         5.0f,  60.0f,  2.0f, 0.0f, 0.0f,




         4.0f,  60.0f, -4.0f, 0.0f, 0.0f,
         3.0f, -20.0f, -3.0f, 1.0f, 0.0f,
         0.0f, -20.0f, -4.0f, 1.0f, 1.0f,
         0.0f, -20.0f, -4.0f, 1.0f, 1.0f,
         1.0f,  60.0f, -5.0f, 0.0f, 1.0f,
         4.0f,  60.0f, -4.0f, 0.0f, 0.0f,

         1.0f,  60.0f, -5.0f, 0.0f, 0.0f,
         0.0f, -20.0f, -4.0f, 1.0f, 0.0f,
        -3.0f, -20.0f, -3.0f, 1.0f, 1.0f,
        -3.0f, -20.0f, -3.0f, 1.0f, 1.0f,
        -4.0f,  60.0f, -4.0f, 0.0f, 1.0f,
         1.0f,  60.0f, -5.0f, 0.0f, 0.0f,

        -4.0f,  60.0f, -4.0f, 0.0f, 0.0f,
        -3.0f, -20.0f, -3.0f, 1.0f, 0.0f,
        -4.0f, -20.0f,  0.0f, 1.0f, 1.0f,
        -4.0f, -20.0f,  0.0f, 1.0f, 1.0f,
        -5.0f,  60.0f,  1.0f, 0.0f, 1.0f,
        -4.0f,  60.0f, -4.0f, 0.0f, 0.0f,

        -5.0f,  60.0f,  1.0f, 0.0f, 0.0f,
        -4.0f, -20.0f,  0.0f, 1.0f, 0.0f,
        -3.0f, -20.0f,  3.0f, 1.0f, 1.0f,
        -3.0f, -20.0f,  3.0f, 1.0f, 1.0f,
        -4.0f,  60.0f,  4.0f, 0.0f, 1.0f,
        -5.0f,  60.0f,  1.0f, 0.0f, 0.0f,




        -3.0f, -20.0f,  3.0f, 0.0f, 0.0f,
        -3.0f, -20.0f,  3.0f, 1.0f, 0.0f,
         0.0f, -20.0f,  4.0f, 1.0f, 1.0f,



        -3.0f, -20.0f,  3.0f, 0.0f, 0.0f,
         0.0f, -20.0f,  4.0f, 1.0f, 0.0f,
         3.0f, -20.0f,  3.0f, 1.0f, 1.0f,



        -3.0f, -20.0f,  3.0f, 0.0f, 0.0f,
         3.0f, -20.0f,  3.0f, 1.0f, 0.0f,
         4.0f, -20.0f,  0.0f, 1.0f, 1.0f,



        -3.0f, -20.0f,  3.0f, 0.0f, 0.0f,
         4.0f, -20.0f,  0.0f, 1.0f, 0.0f,
         4.0f, -20.0f,  0.0f, 1.0f, 1.0f,



        -3.0f, -20.0f,  3.0f, 0.0f, 0.0f,
         4.0f, -20.0f,  0.0f, 1.0f, 0.0f,
         3.0f, -20.0f, -3.0f, 1.0f, 1.0f,



        -3.0f, -20.0f,  3.0f, 0.0f, 0.0f,
         3.0f, -20.0f, -3.0f, 1.0f, 0.0f,
        -3.0f, -20.0f, -3.0f, 1.0f, 1.0f,



        -3.0f, -20.0f,  3.0f, 0.0f, 0.0f,
        -3.0f, -20.0f, -3.0f, 1.0f, 0.0f,
        -4.0f, -20.0f,  0.0f, 1.0f, 1.0f,


    };

    GLfloat plateVerts[] = {
        -4.0f,  60.0f,  4.0f, 0.0f, 0.0f,
        -3.0f, -20.0f,  3.0f, 1.0f, 0.0f,
         0.0f, -20.0f,  4.0f, 1.0f, 1.0f,
         0.0f, -20.0f,  4.0f, 1.0f, 1.0f,
         1.0f,  60.0f,  5.0f, 0.0f, 1.0f,
        -4.0f,  60.0f,  4.0f, 0.0f, 0.0f,

         1.0f,  60.0f,  5.0f, 0.0f, 0.0f,
         0.0f, -20.0f,  4.0f, 1.0f, 0.0f,
         3.0f, -20.0f,  3.0f, 1.0f, 1.0f,
         3.0f, -20.0f,  3.0f, 1.0f, 1.0f,
         4.0f,  60.0f,  4.0f, 0.0f, 1.0f,
         1.0f,  60.0f,  5.0f, 0.0f, 0.0f,



         4.0f,  60.0f,  4.0f, 0.0f, 0.0f,
         3.0f, -20.0f,  3.0f, 1.0f, 0.0f,
         4.0f, -20.0f,  0.0f, 1.0f, 1.0f,
         4.0f, -20.0f,  0.0f, 1.0f, 1.0f,
         5.0f,  60.0f,  2.0f, 0.0f, 1.0f,
         4.0f,  60.0f,  4.0f, 0.0f, 0.0f,

         5.0f,  60.0f,  2.0f, 0.0f, 0.0f,
         4.0f, -20.0f,  0.0f, 1.0f, 0.0f,
         3.0f, -20.0f, -3.0f, 1.0f, 1.0f,
         3.0f, -20.0f, -3.0f, 1.0f, 1.0f,
         4.0f,  60.0f, -4.0f, 0.0f, 1.0f,
         5.0f,  60.0f,  2.0f, 0.0f, 0.0f,




         4.0f,  60.0f, -4.0f, 0.0f, 0.0f,
         3.0f, -20.0f, -3.0f, 1.0f, 0.0f,
         0.0f, -20.0f, -4.0f, 1.0f, 1.0f,
         0.0f, -20.0f, -4.0f, 1.0f, 1.0f,
         1.0f,  60.0f, -5.0f, 0.0f, 1.0f,
         4.0f,  60.0f, -4.0f, 0.0f, 0.0f,

         1.0f,  60.0f, -5.0f, 0.0f, 0.0f,
         0.0f, -20.0f, -4.0f, 1.0f, 0.0f,
        -3.0f, -20.0f, -3.0f, 1.0f, 1.0f,
        -3.0f, -20.0f, -3.0f, 1.0f, 1.0f,
        -4.0f,  60.0f, -4.0f, 0.0f, 1.0f,
         1.0f,  60.0f, -5.0f, 0.0f, 0.0f,

        -4.0f,  60.0f, -4.0f, 0.0f, 0.0f,
        -3.0f, -20.0f, -3.0f, 1.0f, 0.0f,
        -4.0f, -20.0f,  0.0f, 1.0f, 1.0f,
        -4.0f, -20.0f,  0.0f, 1.0f, 1.0f,
        -5.0f,  60.0f,  1.0f, 0.0f, 1.0f,
        -4.0f,  60.0f, -4.0f, 0.0f, 0.0f,

        -5.0f,  60.0f,  1.0f, 0.0f, 0.0f,
        -4.0f, -20.0f,  0.0f, 1.0f, 0.0f,
        -3.0f, -20.0f,  3.0f, 1.0f, 1.0f,
        -3.0f, -20.0f,  3.0f, 1.0f, 1.0f,
        -4.0f,  60.0f,  4.0f, 0.0f, 1.0f,
        -5.0f,  60.0f,  1.0f, 0.0f, 0.0f,




        -3.0f, -20.0f,  3.0f, 0.0f, 0.0f,
        -3.0f, -20.0f,  3.0f, 1.0f, 0.0f,
         0.0f, -20.0f,  4.0f, 1.0f, 1.0f,



        -3.0f, -20.0f,  3.0f, 0.0f, 0.0f,
         0.0f, -20.0f,  4.0f, 1.0f, 0.0f,
         3.0f, -20.0f,  3.0f, 1.0f, 1.0f,



        -3.0f, -20.0f,  3.0f, 0.0f, 0.0f,
         3.0f, -20.0f,  3.0f, 1.0f, 0.0f,
         4.0f, -20.0f,  0.0f, 1.0f, 1.0f,



        -3.0f, -20.0f,  3.0f, 0.0f, 0.0f,
         4.0f, -20.0f,  0.0f, 1.0f, 0.0f,
         4.0f, -20.0f,  0.0f, 1.0f, 1.0f,



        -3.0f, -20.0f,  3.0f, 0.0f, 0.0f,
         4.0f, -20.0f,  0.0f, 1.0f, 0.0f,
         3.0f, -20.0f, -3.0f, 1.0f, 1.0f,



        -3.0f, -20.0f,  3.0f, 0.0f, 0.0f,
         3.0f, -20.0f, -3.0f, 1.0f, 0.0f,
        -3.0f, -20.0f, -3.0f, 1.0f, 1.0f,



        -3.0f, -20.0f,  3.0f, 0.0f, 0.0f,
        -3.0f, -20.0f, -3.0f, 1.0f, 0.0f,
        -4.0f, -20.0f,  0.0f, 1.0f, 1.0f,


    };


    const GLuint floatsPerVertex = 3;
    const GLuint floatsPerUV = 2;

    mesh.nvaseVertices = sizeof(vaseVerts) / (sizeof(vaseVerts[0]) * (floatsPerVertex + floatsPerUV));

    glGenVertexArrays(1, &mesh.vaseVAO); // we can also generate multiple VAOs or buffers at the same time
    glBindVertexArray(mesh.vaseVAO);


    // Create VBO
    glGenBuffers(1, &mesh.vaseVBO);
    glBindBuffer(GL_ARRAY_BUFFER, mesh.vaseVBO); // Activates the buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(vaseVerts), vaseVerts, GL_STATIC_DRAW); // Sends vertex or coordinate data to the GPU

    // Strides between vertex coordinates
    GLint stride = sizeof(float) * (floatsPerVertex + floatsPerUV);

    // Create Vertex Attribute Pointers
    glVertexAttribPointer(0, floatsPerVertex, GL_FLOAT, GL_FALSE, stride, 0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(2, floatsPerUV, GL_FLOAT, GL_FALSE, stride, (char*)(sizeof(float) * floatsPerVertex));
    glEnableVertexAttribArray(2);
    //////////////////////////////////////////////
    mesh.nPlaneVertices = sizeof(planeVerts) / (sizeof(planeVerts[0]) * (floatsPerVertex + floatsPerUV));

    glGenVertexArrays(1, &mesh.planeVAO); // we can also generate multiple VAOs or buffers at the same time
    glBindVertexArray(mesh.planeVAO);


    // Create VBO
    glGenBuffers(1, &mesh.planeVBO);
    glBindBuffer(GL_ARRAY_BUFFER, mesh.planeVBO); // Activates the buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(planeVerts), planeVerts, GL_STATIC_DRAW); // Sends vertex or coordinate data to the GPU

    // Create Vertex Attribute Pointers
    glVertexAttribPointer(0, floatsPerVertex, GL_FLOAT, GL_FALSE, stride, 0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(2, floatsPerUV, GL_FLOAT, GL_FALSE, stride, (char*)(sizeof(float) * floatsPerVertex));
    glEnableVertexAttribArray(2);
    
    /////////////////////////////////////////////
    mesh.ntabletopVertices = sizeof(tabletopVerts) / (sizeof(tabletopVerts[0]) * (floatsPerVertex + floatsPerUV));

    glGenVertexArrays(1, &mesh.tabletopVAO); // we can also generate multiple VAOs or buffers at the same time
    glBindVertexArray(mesh.tabletopVAO);


    // Create VBO
    glGenBuffers(1, &mesh.tabletopVBO);
    glBindBuffer(GL_ARRAY_BUFFER, mesh.tabletopVBO); // Activates the buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(tabletopVerts), tabletopVerts, GL_STATIC_DRAW); // Sends vertex or coordinate data to the GPU

    // Create Vertex Attribute Pointers

    glVertexAttribPointer(0, floatsPerVertex, GL_FLOAT, GL_FALSE, stride, 0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(2, floatsPerUV, GL_FLOAT, GL_FALSE, stride, (char*)(sizeof(float) * floatsPerVertex));
    glEnableVertexAttribArray(2);
    ///////////////////////////////////////////
    mesh.ntableLeg1Vertices = sizeof(tableLeg1Verts) / (sizeof(tableLeg1Verts[0]) * (floatsPerVertex + floatsPerUV));

    glGenVertexArrays(1, &mesh.tableLeg1VAO); // we can also generate multiple VAOs or buffers at the same time
    glBindVertexArray(mesh.tableLeg1VAO);


    // Create VBO
    glGenBuffers(1, &mesh.tableLeg1VBO);
    glBindBuffer(GL_ARRAY_BUFFER, mesh.tableLeg1VBO); // Activates the buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(tableLeg1Verts), tableLeg1Verts, GL_STATIC_DRAW); // Sends vertex or coordinate data to the GPU

    // Create Vertex Attribute Pointers
    glVertexAttribPointer(0, floatsPerVertex, GL_FLOAT, GL_FALSE, stride, 0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(2, floatsPerUV, GL_FLOAT, GL_FALSE, stride, (char*)(sizeof(float) * floatsPerVertex));
    glEnableVertexAttribArray(2);
    ///////////////////////////////////////////
    mesh.ntableLeg2Vertices = sizeof(tableLeg2Verts) / (sizeof(tableLeg2Verts[0]) * (floatsPerVertex + floatsPerUV));

    glGenVertexArrays(1, &mesh.tableLeg2VAO); // we can also generate multiple VAOs or buffers at the same time
    glBindVertexArray(mesh.tableLeg2VAO);


    // Create VBO
    glGenBuffers(1, &mesh.tableLeg2VBO);
    glBindBuffer(GL_ARRAY_BUFFER, mesh.tableLeg2VBO); // Activates the buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(tableLeg2Verts), tableLeg2Verts, GL_STATIC_DRAW); // Sends vertex or coordinate data to the GPU

    // Create Vertex Attribute Pointers
    glVertexAttribPointer(0, floatsPerVertex, GL_FLOAT, GL_FALSE, stride, 0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(2, floatsPerUV, GL_FLOAT, GL_FALSE, stride, (char*)(sizeof(float) * floatsPerVertex));
    glEnableVertexAttribArray(2);
    //////////////////////tableLeg3
    mesh.ntableLeg3Vertices = sizeof(tableLeg3Verts) / (sizeof(tableLeg3Verts[0]) * (floatsPerVertex + floatsPerUV));

    glGenVertexArrays(1, &mesh.tableLeg3VAO); // we can also generate multiple VAOs or buffers at the same time
    glBindVertexArray(mesh.tableLeg3VAO);


    // Create VBO
    glGenBuffers(1, &mesh.tableLeg3VBO);
    glBindBuffer(GL_ARRAY_BUFFER, mesh.tableLeg3VBO); // Activates the buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(tableLeg3Verts), tableLeg3Verts, GL_STATIC_DRAW); // Sends vertex or coordinate data to the GPU

    // Create Vertex Attribute Pointers
    glVertexAttribPointer(0, floatsPerVertex, GL_FLOAT, GL_FALSE, stride, 0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(2, floatsPerUV, GL_FLOAT, GL_FALSE, stride, (char*)(sizeof(float) * floatsPerVertex));
    glEnableVertexAttribArray(2);

    //////////////////////tableLeg4
    mesh.ntableLeg4Vertices = sizeof(tableLeg4Verts) / (sizeof(tableLeg4Verts[0]) * (floatsPerVertex + floatsPerUV));

    glGenVertexArrays(1, &mesh.tableLeg4VAO); // we can also generate multiple VAOs or buffers at the same time
    glBindVertexArray(mesh.tableLeg4VAO);


    // Create VBO
    glGenBuffers(1, &mesh.tableLeg4VBO);
    glBindBuffer(GL_ARRAY_BUFFER, mesh.tableLeg4VBO); // Activates the buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(tableLeg4Verts), tableLeg4Verts, GL_STATIC_DRAW); // Sends vertex or coordinate data to the GPU

    // Create Vertex Attribute Pointers
    glVertexAttribPointer(0, floatsPerVertex, GL_FLOAT, GL_FALSE, stride, 0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(2, floatsPerUV, GL_FLOAT, GL_FALSE, stride, (char*)(sizeof(float) * floatsPerVertex));
    glEnableVertexAttribArray(2);
    //////////////////////coaster
    mesh.ncoasterVertices = sizeof(coasterVerts) / (sizeof(coasterVerts[0]) * (floatsPerVertex + floatsPerUV));

    glGenVertexArrays(1, &mesh.coasterVAO); // we can also generate multiple VAOs or buffers at the same time
    glBindVertexArray(mesh.coasterVAO);


    // Create VBO
    glGenBuffers(1, &mesh.coasterVBO);
    glBindBuffer(GL_ARRAY_BUFFER, mesh.coasterVBO); // Activates the buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(coasterVerts), coasterVerts, GL_STATIC_DRAW); // Sends vertex or coordinate data to the GPU

    // Create Vertex Attribute Pointers
    glVertexAttribPointer(0, floatsPerVertex, GL_FLOAT, GL_FALSE, stride, 0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(2, floatsPerUV, GL_FLOAT, GL_FALSE, stride, (char*)(sizeof(float) * floatsPerVertex));
    glEnableVertexAttribArray(2);
    //////////////////////binding
    mesh.nBindingVertices = sizeof(coasterVerts) / (sizeof(coasterVerts[0]) * (floatsPerVertex + floatsPerUV));

    glGenVertexArrays(1, &mesh.bindingVAO); // we can also generate multiple VAOs or buffers at the same time
    glBindVertexArray(mesh.bindingVAO);


    // Create VBO
    glGenBuffers(1, &mesh.bindingVBO);
    glBindBuffer(GL_ARRAY_BUFFER, mesh.bindingVBO); // Activates the buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(coasterVerts), coasterVerts, GL_STATIC_DRAW); // Sends vertex or coordinate data to the GPU

    // Create Vertex Attribute Pointers
    glVertexAttribPointer(0, floatsPerVertex, GL_FLOAT, GL_FALSE, stride, 0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(2, floatsPerUV, GL_FLOAT, GL_FALSE, stride, (char*)(sizeof(float) * floatsPerVertex));
    glEnableVertexAttribArray(2);
    //////////////////////flowers
    mesh.nflowersVertices = sizeof(flowersVerts) / (sizeof(flowersVerts[0]) * (floatsPerVertex + floatsPerUV));

    glGenVertexArrays(1, &mesh.flowersVAO); // we can also generate multiple VAOs or buffers at the same time
    glBindVertexArray(mesh.flowersVAO);


    // Create VBO
    glGenBuffers(1, &mesh.flowersVBO);
    glBindBuffer(GL_ARRAY_BUFFER, mesh.flowersVBO); // Activates the buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(flowersVerts), flowersVerts, GL_STATIC_DRAW); // Sends vertex or coordinate data to the GPU

    // Create Vertex Attribute Pointers
    glVertexAttribPointer(0, floatsPerVertex, GL_FLOAT, GL_FALSE, stride, 0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(2, floatsPerUV, GL_FLOAT, GL_FALSE, stride, (char*)(sizeof(float) * floatsPerVertex));
    glEnableVertexAttribArray(2);

    //////////////////////////////////////////

    //////////////////////flowers2
    mesh.nflowers2Vertices = sizeof(flowersVerts) / (sizeof(flowersVerts[0]) * (floatsPerVertex + floatsPerUV));

    glGenVertexArrays(1, &mesh.flowers2VAO); // we can also generate multiple VAOs or buffers at the same time
    glBindVertexArray(mesh.flowers2VAO);


    // Create VBO
    glGenBuffers(1, &mesh.flowers2VBO);
    glBindBuffer(GL_ARRAY_BUFFER, mesh.flowers2VBO); // Activates the buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(flowersVerts), flowersVerts, GL_STATIC_DRAW); // Sends vertex or coordinate data to the GPU

    // Create Vertex Attribute Pointers
    glVertexAttribPointer(0, floatsPerVertex, GL_FLOAT, GL_FALSE, stride, 0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(2, floatsPerUV, GL_FLOAT, GL_FALSE, stride, (char*)(sizeof(float)* floatsPerVertex));
    glEnableVertexAttribArray(2);

    //////////////////////////////////////////

    //////////////////////flowers3
    mesh.nflowers3Vertices = sizeof(flowersVerts) / (sizeof(flowersVerts[0]) * (floatsPerVertex + floatsPerUV));

    glGenVertexArrays(1, &mesh.flowers3VAO); // we can also generate multiple VAOs or buffers at the same time
    glBindVertexArray(mesh.flowers3VAO);


    // Create VBO
    glGenBuffers(1, &mesh.flowers3VBO);
    glBindBuffer(GL_ARRAY_BUFFER, mesh.flowers3VBO); // Activates the buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(flowersVerts), flowersVerts, GL_STATIC_DRAW); // Sends vertex or coordinate data to the GPU

    // Create Vertex Attribute Pointers
    glVertexAttribPointer(0, floatsPerVertex, GL_FLOAT, GL_FALSE, stride, 0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(2, floatsPerUV, GL_FLOAT, GL_FALSE, stride, (char*)(sizeof(float)* floatsPerVertex));
    glEnableVertexAttribArray(2);

    //////////////////////////////////////////

    //////////////////////flowers4
    mesh.nflowers4Vertices = sizeof(flowersVerts) / (sizeof(flowersVerts[0]) * (floatsPerVertex + floatsPerUV));

    glGenVertexArrays(1, &mesh.flowers4VAO); // we can also generate multiple VAOs or buffers at the same time
    glBindVertexArray(mesh.flowers4VAO);


    // Create VBO
    glGenBuffers(1, &mesh.flowers4VBO);
    glBindBuffer(GL_ARRAY_BUFFER, mesh.flowers4VBO); // Activates the buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(flowersVerts), flowersVerts, GL_STATIC_DRAW); // Sends vertex or coordinate data to the GPU

    // Create Vertex Attribute Pointers
    glVertexAttribPointer(0, floatsPerVertex, GL_FLOAT, GL_FALSE, stride, 0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(2, floatsPerUV, GL_FLOAT, GL_FALSE, stride, (char*)(sizeof(float)* floatsPerVertex));
    glEnableVertexAttribArray(2);

    //////////////////////////////////////////

    //////////////////////flowers5
    mesh.nflowers5Vertices = sizeof(flowersVerts) / (sizeof(flowersVerts[0]) * (floatsPerVertex + floatsPerUV));

    glGenVertexArrays(1, &mesh.flowers5VAO); // we can also generate multiple VAOs or buffers at the same time
    glBindVertexArray(mesh.flowers5VAO);


    // Create VBO
    glGenBuffers(1, &mesh.flowers5VBO);
    glBindBuffer(GL_ARRAY_BUFFER, mesh.flowers5VBO); // Activates the buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(flowersVerts), flowersVerts, GL_STATIC_DRAW); // Sends vertex or coordinate data to the GPU

    // Create Vertex Attribute Pointers
    glVertexAttribPointer(0, floatsPerVertex, GL_FLOAT, GL_FALSE, stride, 0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(2, floatsPerUV, GL_FLOAT, GL_FALSE, stride, (char*)(sizeof(float)* floatsPerVertex));
    glEnableVertexAttribArray(2);

    //////////////////////////////////////////

    //////////////////////flowers6
    mesh.nflowers6Vertices = sizeof(flowersVerts) / (sizeof(flowersVerts[0]) * (floatsPerVertex + floatsPerUV));

    glGenVertexArrays(1, &mesh.flowers6VAO); // we can also generate multiple VAOs or buffers at the same time
    glBindVertexArray(mesh.flowers6VAO);


    // Create VBO
    glGenBuffers(1, &mesh.flowers6VBO);
    glBindBuffer(GL_ARRAY_BUFFER, mesh.flowers6VBO); // Activates the buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(flowersVerts), flowersVerts, GL_STATIC_DRAW); // Sends vertex or coordinate data to the GPU

    // Create Vertex Attribute Pointers
    glVertexAttribPointer(0, floatsPerVertex, GL_FLOAT, GL_FALSE, stride, 0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(2, floatsPerUV, GL_FLOAT, GL_FALSE, stride, (char*)(sizeof(float)* floatsPerVertex));
    glEnableVertexAttribArray(2);

    //////////////////////////////////////////

    mesh.nbowlVertices = sizeof(bowlVerts) / (sizeof(bowlVerts[0]) * (floatsPerVertex + floatsPerUV));

    glGenVertexArrays(1, &mesh.bowlVAO); // we can also generate multiple VAOs or buffers at the same time
    glBindVertexArray(mesh.bowlVAO);


    // Create VBO
    glGenBuffers(1, &mesh.bowlVBO);
    glBindBuffer(GL_ARRAY_BUFFER, mesh.bowlVBO); // Activates the buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(bowlVerts), bowlVerts, GL_STATIC_DRAW); // Sends vertex or coordinate data to the GPU

    // Create Vertex Attribute Pointers
    glVertexAttribPointer(0, floatsPerVertex, GL_FLOAT, GL_FALSE, stride, 0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(2, floatsPerUV, GL_FLOAT, GL_FALSE, stride, (char*)(sizeof(float) * floatsPerVertex));
    glEnableVertexAttribArray(2);
    /////////////////////////////////////////////

    mesh.nplateVertices = sizeof(plateVerts) / (sizeof(plateVerts[0]) * (floatsPerVertex + floatsPerUV));

    glGenVertexArrays(1, &mesh.plateVAO); // we can also generate multiple VAOs or buffers at the same time
    glBindVertexArray(mesh.plateVAO);


    // Create VBO
    glGenBuffers(1, &mesh.plateVBO);
    glBindBuffer(GL_ARRAY_BUFFER, mesh.plateVBO); // Activates the buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(plateVerts), plateVerts, GL_STATIC_DRAW); // Sends vertex or coordinate data to the GPU

    // Create Vertex Attribute Pointers
    glVertexAttribPointer(0, floatsPerVertex, GL_FLOAT, GL_FALSE, stride, 0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(2, floatsPerUV, GL_FLOAT, GL_FALSE, stride, (char*)(sizeof(float) * floatsPerVertex));
    glEnableVertexAttribArray(2);
   
}

void UDestroyMesh(GLMesh& mesh)
{
    glDeleteVertexArrays(1, &mesh.planeVAO);
    glDeleteBuffers(1, &mesh.planeVBO);

    glDeleteVertexArrays(1, &mesh.vaseVAO);
    glDeleteBuffers(1, &mesh.vaseVBO);

    glDeleteVertexArrays(1, &mesh.tabletopVAO);
    glDeleteBuffers(1, &mesh.tabletopVBO);

    glDeleteVertexArrays(1, &mesh.tableLeg3VAO);
    glDeleteBuffers(1, &mesh.tableLeg3VBO);

    glDeleteVertexArrays(1, &mesh.flowersVAO);
    glDeleteBuffers(1, &mesh.flowersVBO);

    glDeleteVertexArrays(1, &mesh.flowers2VAO);
    glDeleteBuffers(1, &mesh.flowers2VBO);

    glDeleteVertexArrays(1, &mesh.flowers3VAO);
    glDeleteBuffers(1, &mesh.flowers3VBO);

    glDeleteVertexArrays(1, &mesh.flowers4VAO);
    glDeleteBuffers(1, &mesh.flowers4VBO);

    glDeleteVertexArrays(1, &mesh.flowers5VAO);
    glDeleteBuffers(1, &mesh.flowers5VBO);

    glDeleteVertexArrays(1, &mesh.flowers6VAO);
    glDeleteBuffers(1, &mesh.flowers6VBO);

    glDeleteVertexArrays(1, &mesh.tableLeg2VAO);
    glDeleteBuffers(1, &mesh.tableLeg2VBO);

    glDeleteVertexArrays(1, &mesh.bindingVAO);
    glDeleteBuffers(1, &mesh.bindingVBO);

    glDeleteVertexArrays(1, &mesh.coasterVAO);
    glDeleteBuffers(1, &mesh.coasterVBO);

    glDeleteVertexArrays(1, &mesh.tableLeg4VAO);
    glDeleteBuffers(1, &mesh.tableLeg4VBO);

    glDeleteVertexArrays(1, &mesh.tableLeg1VAO);
    glDeleteBuffers(1, &mesh.tableLeg1VBO);
}

/*Generate and load the texture*/
bool UCreateTexture(const char* filename, GLuint& textureId)
{
    int width, height, channels;
    unsigned char* image = stbi_load(filename, &width, &height, &channels, 0);
    if (image)
    {
        flipImageVertically(image, width, height, channels);

        glGenTextures(1, &textureId);
        glBindTexture(GL_TEXTURE_2D, textureId);

        // set the texture wrapping parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        // set texture filtetableLeg2 parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


        if (channels == 3)
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
        else if (channels == 4)
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
        else
        {
            cout << "Not implemented to handle image with " << channels << " channels" << endl;
            return false;
        }

        glGenerateMipmap(GL_TEXTURE_2D);

        stbi_image_free(image);
        glBindTexture(GL_TEXTURE_2D, 0); // Unbind the texture

        return true;
    }

    // Error loading the image
    return false;
}


void UDestroyTexture(GLuint textureId)
{
    glGenTextures(1, &textureId);
}


// Implements the UCreateShaders function
bool UCreateShaderProgram(const char* vtxShaderSource, const char* fragShaderSource, GLuint& programId)
{
    // Compilation and linkage error reporting
    int success = 0;
    char infoLog[512];

    // Create a Shader program object.
    programId = glCreateProgram();

    // Create the vertex and fragment shader objects
    GLuint vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);

    // Retrive the shader source
    glShaderSource(vertexShaderId, 1, &vtxShaderSource, NULL);
    glShaderSource(fragmentShaderId, 1, &fragShaderSource, NULL);

    // Compile the vertex shader, and print compilation errors (if any)
    glCompileShader(vertexShaderId); // compile the vertex shader
    // check for shader compile errors
    glGetShaderiv(vertexShaderId, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShaderId, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;

        return false;
    }

    glCompileShader(fragmentShaderId); // compile the fragment shader
    // check for shader compile errors
    glGetShaderiv(fragmentShaderId, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShaderId, sizeof(infoLog), NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;

        return false;
    }

    // Attached compiled shaders to the shader program
    glAttachShader(programId, vertexShaderId);
    glAttachShader(programId, fragmentShaderId);

    glLinkProgram(programId);   // links the shader program
    // check for linking errors
    glGetProgramiv(programId, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(programId, sizeof(infoLog), NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;

        return false;
    }

    glUseProgram(programId);    // Uses the shader program

    return true;
}


void UDestroyShaderProgram(GLuint programId)
{
    glDeleteProgram(programId);
}
