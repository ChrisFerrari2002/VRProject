/**
 * @file engine.cpp
 * @brief Graphics engine main file
 *
 * This file contains the implementation of the Base class methods for the graphics engine.
 *
 * @date 2025
 *
 * @details The Base class is the main class of the graphics engine and is implemented as a singleton.
 * @see Eng::Base
 *
 * @authors
 * - Sara Bressan [sara.bressan@student.supsi.ch]
 * - Chris Ferrari [chris.ferrari@student.supsi.ch]
 * - Alessandro Formato [alessandro.formato@student.supsi.ch]
 */

 //////////////
 // #INCLUDE //
 //////////////
using namespace std;
// Main include:
#include "engine.h"

// GLEW
#include <GL/glew.h>

// FreeGLUT:
#include <GL/freeglut.h>

// FreeImage:
#include <FreeImage.h>

// C/C++:
#include <functional>

////////////
// STATIC //
////////////

bool Eng::Base::useZBuffer = true; /**< Z-buffer usage flag */
bool Eng::Base::mainLoopRunning = false; /**< Main loop running flag */
float Eng::Base::bgR = 0.0f; /**< Background red component */
float Eng::Base::bgG = 0.0f; /**< Background green component */
float Eng::Base::bgB = 0.0f; /**< Background blue component */
float Eng::Base::bgA = 0.0f; /**< Background alpha component */
std::vector<Eng::Camera*> Eng::Base::cameras = std::vector<Camera*>(); /**< List of cameras */
int Eng::Base::activeCamera = 0; /**< Active camera index */

Eng::OvoReader Eng::Base::reader = OvoReader(); /**< OvoReader object */
Eng::List Eng::Base::list; /**< List object */

glm::mat4 perspective; /**< Perspective projection matrix */
glm::mat4 ortho; /**< Orthographic projection matrix */

Eng::Shader* shader;
int projLoc = -1; // -1 means 'not assigned', as 0 is a valid location

/////////////////////////
// RESERVED STRUCTURES //
/////////////////////////

/**
 * @brief Base class reserved structure (using PIMPL/Bridge design pattern https://en.wikipedia.org/wiki/Opaque_pointer).
 * 
 * The base class reserved structure is used to hide the internal implementation details of the Base class.
 */
struct Eng::Base::Reserved
{
    // Flags:
	bool initFlag; /**< Initialization flag */

    /**
     * @brief Constructor
     *
     * Initializes the reserved structure.
     */
    Reserved() : initFlag{ false }
    {
    }
};

////////////////////////
// BODY OF CLASS Base //
////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief Constructor
 *
 * Initializes the Base class.
 */
ENG_API Eng::Base::Base() : reserved(std::make_unique<Eng::Base::Reserved>())
{
    // Sistema di Logging
#ifdef _DEBUG   
    std::cout << "[+] " << std::source_location::current().function_name() << " invoked" << std::endl;
#endif
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * @brief Destructor
 *
 * Destroys the Base class.
 */
ENG_API Eng::Base::~Base()
{
#ifdef _DEBUG
    std::cout << "[-] " << std::source_location::current().function_name() << " invoked" << std::endl;
#endif
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief Gets a reference to the (unique) singleton instance.
 * @return Reference to singleton instance.
 */
Eng::Base ENG_API& Eng::Base::getInstance()
{
    static Base instance;
    return instance;
}

#ifndef _WIN32
#define __stdcall // Just defined as an empty macro under Linux
#endif
/**
 * Debug message callback for OpenGL. See https://www.opengl.org/wiki/Debug_Output
 */
void __stdcall DebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, GLvoid* userParam)
{
   std::cout << "OpenGL says: \"" << std::string(message) << "\"" << std::endl;
}

const char* vertShader = R"(
   #version 440 core

   // Uniforms:
   uniform mat4 projection;
   uniform mat4 modelview;
   uniform mat3 normalMatrix;

   // Attributes:
   layout(location = 0) in vec3 in_Position;
   layout(location = 1) in vec3 in_Normal;

   // Varying:
   out vec4 fragPosition;
   out vec3 normal;   

   void main(void)
   {
      fragPosition = modelview * vec4(in_Position, 1.0f);
      gl_Position = projection * fragPosition;      
      normal = normalMatrix * in_Normal;
   }
)";

////////////////////////////
const char* fragShader = R"(
   #version 440 core

   in vec4 fragPosition;
   in vec3 normal;   
   
   out vec4 fragOutput;

   // Material properties:
   uniform vec3 matEmission;
   uniform vec3 matAmbient;
   uniform vec3 matDiffuse;
   uniform vec3 matSpecular;
   uniform float matShininess;

   // Light properties:
   uniform vec3 lightPosition; 
   uniform vec3 lightAmbient; 
   uniform vec3 lightDiffuse; 
   uniform vec3 lightSpecular;

   void main(void)
   {      
      // Ambient term:
      vec3 fragColor = matEmission + matAmbient * lightAmbient;

      // Diffuse term:
      vec3 _normal = normalize(normal);
      vec3 lightDirection = normalize(lightPosition - fragPosition.xyz);      
      float nDotL = dot(lightDirection, _normal);   
      if (nDotL > 0.0f)
      {
         fragColor += matDiffuse * nDotL * lightDiffuse;
      
         // Specular term:
         vec3 halfVector = normalize(lightDirection + normalize(-fragPosition.xyz));                     
         float nDotHV = dot(_normal, halfVector);         
         fragColor += matSpecular * pow(nDotHV, matShininess) * lightSpecular;
      } 
      
      // Final color:
      fragOutput = vec4(fragColor, 1.0f);
   }
)";

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief Init internal components of the base class.
 * 
 * @param argc Number of command-line arguments.
 * @param argv Array of command-line arguments.
 * @param title Title of the window.
 * @param width Width of the window.
 * @param height Height of the window.
 * @return True if initialization was successful, false otherwise.
 */
bool ENG_API Eng::Base::init(int argc, char* argv[], const char* title, int width, int height)
{
    // Already initialized?
    if (reserved->initFlag)
    {
        std::cout << "ERROR: engine already initialized" << std::endl;
        return false;
    }
    // Here you can initialize most of the graphics engine's dependencies and default settings...
    if (!reserved->initFlag) {
        //FreeImage_Initialise();
        // FreeGLUT can parse command-line params, in case:
        glutInit(&argc, argv);
        // Init context:
        if (useZBuffer) {
            glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
        }
        else {
            glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
        }
        glutInitContextVersion(4, 4);
        glutInitContextProfile(GLUT_CORE_PROFILE);
        glutInitContextFlags(GLUT_DEBUG);

        glutInitWindowPosition(100, 100);
        glutInitWindowSize(width, height);

        // Set some optional flags:
        glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);

        // Enable Z buffer if it's required
        if (useZBuffer) execZBufferSetup();

        // Create window
        windowId = glutCreateWindow(title);

        GLenum error = glewInit();
        if (error != GLEW_OK)
        {
           std::cout << "[ERROR] " << glewGetErrorString(error) << std::endl;
           return -1;
        }
        else
           if (GLEW_VERSION_4_4)
              std::cout << "Driver supports OpenGL 4.4\n" << std::endl;
           else
           {

           }
        #ifdef _DEBUG
           glDebugMessageCallback((GLDEBUGPROC)DebugCallback, nullptr);
           glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        #endif // _DEBUG

        // Log context properties:
        std::cout << "OpenGL properties:" << std::endl;
        std::cout << "   Vendor . . . :  " << glGetString(GL_VENDOR) << std::endl;
        std::cout << "   Driver . . . :  " << glGetString(GL_RENDERER) << std::endl;


        int oglVersion[2];
        glGetIntegerv(GL_MAJOR_VERSION, &oglVersion[0]);
        glGetIntegerv(GL_MINOR_VERSION, &oglVersion[1]);
        std::cout << "   Version  . . :  " << glGetString(GL_VERSION) << " [" << oglVersion[0] << "." << oglVersion[1] << "]" << std::endl;

        int oglContextProfile;
        glGetIntegerv(GL_CONTEXT_PROFILE_MASK, &oglContextProfile);
        if (oglContextProfile & GL_CONTEXT_CORE_PROFILE_BIT)
           std::cout << "                :  " << "Core profile" << std::endl;
        if (oglContextProfile & GL_CONTEXT_COMPATIBILITY_PROFILE_BIT)
           std::cout << "                :  " << "Compatibility profile" << std::endl;

        int oglContextFlags;
        glGetIntegerv(GL_CONTEXT_FLAGS, &oglContextFlags);
        if (oglContextFlags & GL_CONTEXT_FLAG_FORWARD_COMPATIBLE_BIT)
           std::cout << "                :  " << "Forward compatible" << std::endl;
        if (oglContextFlags & GL_CONTEXT_FLAG_DEBUG_BIT)
           std::cout << "                :  " << "Debug flag" << std::endl;
        if (oglContextFlags & GL_CONTEXT_FLAG_ROBUST_ACCESS_BIT)
           std::cout << "                :  " << "Robust access flag" << std::endl;
        if (oglContextFlags & GL_CONTEXT_FLAG_NO_ERROR_BIT)
           std::cout << "                :  " << "No error flag" << std::endl;

        std::cout << "   GLSL . . . . :  " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
        std::cout << std::endl;

        // Set callback functions
        glutDisplayFunc(displayCallback);
        ////Enable Z-Buffer
        glEnable(GL_DEPTH_TEST);
        ////Enable face culling
        //glEnable(GL_CULL_FACE);
        ////Enable smooth shading
        //glShadeModel(GL_SMOOTH);
        ////enable texture
        //glEnable(GL_TEXTURE_2D);
        //glEnable(GL_NORMALIZE);

        Shader* vs = new Shader();
        vs->loadFromMemory(Shader::TYPE_VERTEX, vertShader);

        Shader* fs = new Shader();
        fs->loadFromMemory(Shader::TYPE_FRAGMENT, fragShader);

        shader = new Shader();
        shader->build(vs, fs);
        shader->render();

    }

    // Done:
    std::cout << "[>] " << LIB_NAME << " initialized" << std::endl;
    reserved->initFlag = true;
    return reserved->initFlag;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief Free internal components.
 * @return True if the resources were successfully freed, false otherwise.
 */
bool ENG_API Eng::Base::free()
{
    // Not initialized?
    if (!reserved->initFlag)
    {
        std::cout << "ERROR: engine not initialized" << std::endl;
        return false;
    }

    // Here you can properly dispose of any allocated resource (including third-party dependencies)...
    // Close open connections or free allocated memory
    mainLoopRunning = false;

    // Release bitmap and FreeImage:
    FreeImage_DeInitialise();

    // Done:
    std::cout << "[<] " << LIB_NAME << " deinitialized" << std::endl;
    reserved->initFlag = false;
    return true;
}

/**
 * @brief Handle window reshape event.
 * @param width New width of the window.
 * @param height New height of the window.
 */
void ENG_API Eng::Base::reshapeCallback(int width, int height)
{
    glViewport(0, 0, width, height);

    perspective = glm::perspective(glm::radians(80.0f), (float)width / (float)height, 1.0f, 1000.0f);

    shader->setMatrix("projection", perspective);
}

/**
 * @brief Display callback function.
 */
void ENG_API Eng::Base::displayCallback()
{
   // RGBA components
   if (useZBuffer) {
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   }
   else {
      glClear(GL_COLOR_BUFFER_BIT);
   }
    //////
    // 3D:

    //// Set perspective matrix:
    //glMatrixMode(GL_PROJECTION);
    //glLoadMatrixf(glm::value_ptr(perspective));
    //glMatrixMode(GL_MODELVIEW);

    //// Enable Z buffer if it's required
    //if (useZBuffer) execZBufferSetup();

    ////disable texturing
    //glLoadMatrixf(glm::value_ptr(cameras.at(activeCamera)->getInverseCameraMat()));
    list.render(cameras.at(activeCamera)->getInverseCameraMat(), nullptr);
}

/**
 * @brief Add a camera to the engine.
 * @param camera Pointer to the camera to be added.
 */
void ENG_API Eng::Base::addCamera(Camera* camera) {
    cameras.push_back(camera);
}

/**
 * @brief Load a scene from a file.
 * @param pathName Path to the scene file.
 * @return A list of nodes representing the scene.
 */
std::list<Eng::Node*> ENG_API Eng::Base::loadScene(std::string pathName)
{
    Node* root = reader.readFile(pathName.c_str());
    list.addEntry(root);
    return list.getObjectList();
}

/**
 * @brief Set the active camera by index.
 * @param num Index of the camera to be set as active.
 */
void ENG_API Eng::Base::setActiveCamera(int num)
{
    activeCamera = num;
}

/**
 * @brief Add a node to the engine.
 * @param node The node to be added.
 */
void ENG_API Eng::Base::addNode(Node node)
{
    Node* _node = new Node(node);
    list.addEntry(_node);
}

/**
 * @brief Write text on the screen.
 * @param text The text to be written.
 * @param color The color of the text.
 * @param coord The coordinates where the text should be written.
 * @param textType The type of text to be written.
 */
void ENG_API Eng::Base::writeOnScreen(std::string text, glm::vec3 color, glm::vec2 coord, int textType)
{
    //// 2D
    //// Set orthographic projection:
    //glMatrixMode(GL_PROJECTION);
    //glLoadMatrixf(glm::value_ptr(ortho));
    //glMatrixMode(GL_MODELVIEW);
    //glLoadMatrixf(glm::value_ptr(glm::mat4(1.0f)));

    //// Write 2D text
    //glDisable(GL_LIGHTING);
    //glColor3f(color.x, color.y, color.z);
    //glRasterPos2f(coord.x, coord.y);

    //switch (textType) {
    //case 1:
    //    glutBitmapString(GLUT_BITMAP_8_BY_13, (unsigned char*)text.c_str());
    //    break;
    //case 2:
    //    glutBitmapString(GLUT_BITMAP_9_BY_15, (unsigned char*)text.c_str());
    //    break;
    //case 3:
    //    glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_10, (unsigned char*)text.c_str());
    //    break;
    //case 4:
    //    glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24, (unsigned char*)text.c_str());
    //    break;
    //case 5:
    //    glutBitmapString(GLUT_BITMAP_HELVETICA_10, (unsigned char*)text.c_str());
    //    break;
    //case 6:
    //    glutBitmapString(GLUT_BITMAP_HELVETICA_12, (unsigned char*)text.c_str());
    //    break;
    //case 7:
    //    glutBitmapString(GLUT_BITMAP_HELVETICA_18, (unsigned char*)text.c_str());
    //    break;
    //}

    //// Re-enable lighting
    //glEnable(GL_LIGHTING);
}

/**
 * @brief Start a timer.
 * @param func Pointer to the function to be called when the timer expires.
 * @param time Duration of the timer in milliseconds.
 */
void ENG_API Eng::Base::startTimer(void(*func)(int), int time)
{
    glutTimerFunc(time, func, 0);
}

/**
 * @brief Get the list of objects in the engine.
 * @return Pointer to the list of objects in the engine.
 */
Eng::List* Eng::Base::getList()
{
    return &list;
}

/**
 * @brief Clear the list of objects in the engine.
 * @return True if the list was successfully cleared, false otherwise.
 */
bool Eng::Base::clearList() {
    list.clear();
    return true;
}

/**
 * @brief Set the keyboard callback.
 * @param func Pointer to the keyboard callback function.
 */
void Eng::Base::setKeyboardCallback(void (*func)(unsigned char key, int x, int y)) {
    glutKeyboardFunc(func);
}

/**
 * @brief Remove the object picked callback.
 */
void Eng::Base::removeObjectPickedCallback() {
    // Disables the generation of mouse callbacks
    glutMouseFunc(NULL);
}

/**
 * @brief Set the special key callback.
 * @param func Pointer to the special key callback function.
 */
void Eng::Base::setSpecialCallback(void (*func)(int key, int x, int y)) {
    glutSpecialFunc(func);
}

/**
 * @brief Set the background color.
 * @param r Red component of the color.
 * @param g Green component of the color.
 * @param b Blue component of the color.
 * @param a Alpha component of the color.
 */
void Eng::Base::setBackgroundColor(float r, float g, float b, float a) {
    bgR = r;
    bgG = g;
    bgB = b;
    bgA = a;
}

/**
 * @brief Set the window resize handler.
 * @param func Pointer to the resize handler function.
 */
void Eng::Base::setWindowResizeHandler(void(*func)(int, int))
{
    glutReshapeFunc(func);
}

/**
 * @brief Check if the engine is running.
 * @return True if the engine is running, false otherwise.
 */
bool Eng::Base::isRunning() {
    return mainLoopRunning;
}

/**
 * @brief Set Z-buffer usage.
 * @param status True to use the Z-buffer, false otherwise.
 */
void Eng::Base::setZBufferUsage(bool status) {
    useZBuffer = status;
}

/**
 * @brief Start the engine.
 */
void Eng::Base::start() {
    mainLoopRunning = true;
}

/**
 * @brief Post a window redisplay event.
 */
void Eng::Base::postWindowRedisplay() {
    // Force rendering refresh
    
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief This function sets up the z-buffer.
 */
void Eng::Base::execZBufferSetup() {
    //glEnable(GL_DEPTH_TEST);
    //glClearDepth(1.0f);
    //glDepthFunc(GL_LESS);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief This function calls the glutMainLoopEvent (instead of using glutMainLoop that blocks the whole program).
 */
void Eng::Base::update() {
    glutMainLoopEvent();
}

/**
 * @brief Refresh and swap the buffers.
 */
void Eng::Base::refreshAndSwapBuffers()
{
    glutPostWindowRedisplay(windowId);
    glutSwapBuffers();
}