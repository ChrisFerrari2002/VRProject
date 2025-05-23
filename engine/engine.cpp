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
 * - Chris Ferrari [chris.ferrari@student.supsi.ch]
 * - Veljko Markovic [veljko.markovic@student.supsi.ch]
 * - Marco Bernasconi [marco.bernasconi@student.supsi.ch]
 * - Jonathan Casadei [jonathan.casadei@student.supsi.ch]
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

// LeapMotion:
#include <LeapC.h>

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

Eng::Skybox* Eng::Base::skybox;

glm::mat4 perspective; /**< Perspective projection matrix */
glm::mat4 ortho; /**< Orthographic projection matrix */

Eng::Shader* pointLightShader;
Eng::Shader* directionalLightShader;
Eng::Shader* spotLightShader;

Eng::Leap* leap; 



enum RenderMode
{
   VR = 0,
   NORMAL = 1,
};

RenderMode renderMode = RenderMode::VR; 

Eng::OvVR* ovr = nullptr; /**< OpenVR object */
unsigned int fboSizeX = 0;
unsigned int fboSizeY = 0;

// Enums:
enum Eye
{
   EYE_LEFT = 0,
   EYE_RIGHT = 1,

   // Terminator:
   EYE_LAST,
};

unsigned int fboTexId[EYE_LAST] = { 0, 0 };
Eng::Fbo* fbo[EYE_LAST] = { nullptr, nullptr };

float posxVr = 0.0f;
float posyVr = 0.0f;
float poszVr = 0.0f;

bool whitePosition;
bool freeCamera;

// Window size:
#define APP_WINDOWSIZEX   1024
#define APP_WINDOWSIZEY   512
#define APP_FBOSIZEX      APP_WINDOWSIZEX / 2
#define APP_FBOSIZEY      APP_WINDOWSIZEY / 1


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
   layout(location = 2) in vec2 in_TexCoord;

   // Varying:
   out vec4 fragPosition;
   out vec3 normal;
   out vec2 texCoord;

   void main(void)
   {
      fragPosition = modelview * vec4(in_Position, 1.0f);
      gl_Position = projection * fragPosition;
      normal = normalMatrix * in_Normal;
      texCoord = in_TexCoord;
   }
)";

////////////////////////////
const char *pointFragShader = R"(
   #version 440 core

   in vec4 fragPosition;
   in vec3 normal;
   in vec2 texCoord;

   out vec4 fragOutput;

   // Material properties:
   uniform vec3 matAmbient;
   uniform vec3 matDiffuse;
   uniform vec3 matSpecular;
   uniform float matShininess;

   // Light properties:
   uniform vec3 lightPosition;
   uniform vec3 lightAmbient;
   uniform vec3 lightDiffuse;
   uniform vec3 lightSpecular;

   // Texture mapping:
   layout(binding = 0) uniform sampler2D texSampler;

   void main(void)
   {
      // Texture element:
      vec4 texel = texture(texSampler, texCoord);

      // Ambient term:
      vec3 fragColor = matAmbient * lightAmbient;

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
      fragOutput = texel * vec4(fragColor, 1.0f);
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
bool ENG_API Eng::Base::init(int argc, char* argv[], const char* title)
{
    // Already initialized?
    if (reserved->initFlag)
    {
        std::cout << "ERROR: engine already initialized" << std::endl;
        return false;
    }
    // Here you can initialize most of the graphics engine's dependencies and default settings...
    if (!reserved->initFlag) {
        FreeImage_Initialise();
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
        glutInitWindowSize(APP_WINDOWSIZEX, APP_WINDOWSIZEY);

        // Set some optional flags:
        glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);

        // Create window
        windowId = glutCreateWindow(title);

        GLenum error = glewInit();
        if (error != GLEW_OK)
        {
           std::cout << "[ERROR] " << glewGetErrorString(error) << std::endl;
           return false;
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

        leap = new Leap();
        if (!leap->init())
        {
           std::cout << "[ERROR] Unable to init Leap Motion" << std::endl;
           delete leap;
           return false;
        }

        if (renderMode == RenderMode::VR)
        {
           // Init OpenVR:   
           ovr = new OvVR();
           if (ovr->init() == false)
           {
              std::cout << "[ERROR] Unable to init OpenVR" << std::endl;
              delete ovr;
              return false;
           }

           // Report some info:
           std::cout << "   Manufacturer . . :  " << ovr->getManufacturerName() << std::endl;
           std::cout << "   Tracking system  :  " << ovr->getTrackingSysName() << std::endl;
           std::cout << "   Model number . . :  " << ovr->getModelNumber() << std::endl;
        }

        // Set callback functions
        glutDisplayFunc(displayCallback);

        Shader* vs = new Shader();
        vs->loadFromMemory(Shader::TYPE_VERTEX, vertShader);

        Shader* pfs = new Shader(); // PointLight fragment shader
        pfs->loadFromMemory(Shader::TYPE_FRAGMENT, pointFragShader);

        pointLightShader = new Shader();
        pointLightShader->build(vs, pfs);
        Shader::mapShader("lightShader", pointLightShader);
        Shader::getShader("lightShader")->render();
        
        GLint prevViewport[4];
        glGetIntegerv(GL_VIEWPORT, prevViewport);

        if (renderMode == RenderMode::VR)
        {
           fboSizeX = ovr->getHmdIdealHorizRes();
           fboSizeY = ovr->getHmdIdealVertRes();
           std::cout << "   Ideal resolution :  " << fboSizeX << "x" << fboSizeY << std::endl;
        }
        for (int c = 0; c < EYE_LAST; c++)
        {
           glGenTextures(1, &fboTexId[c]);
           glBindTexture(GL_TEXTURE_2D, fboTexId[c]);

           if (renderMode == RenderMode::VR) {
              glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, fboSizeX, fboSizeY, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
              glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
              glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
              glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
              glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
           } else {
              glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, APP_FBOSIZEX, APP_FBOSIZEY, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
              glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
              glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
              glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
              glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
           }
              
           fbo[c] = new Fbo();
           fbo[c]->bindTexture(0, Fbo::BIND_COLORTEXTURE, fboTexId[c]);
           if (renderMode == RenderMode::VR)
              fbo[c]->bindRenderBuffer(1, Fbo::BIND_DEPTHBUFFER, fboSizeX, fboSizeY);
           else
              fbo[c]->bindRenderBuffer(1, Fbo::BIND_DEPTHBUFFER, APP_FBOSIZEX, APP_FBOSIZEY);
           
           if (!fbo[c]->isOk())
              std::cout << "[ERROR] Invalid FBO" << std::endl;
        }
        Fbo::disable();
        glViewport(0, 0, prevViewport[2], prevViewport[3]);
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
   if (renderMode == RenderMode::VR)
   {
      ortho = glm::ortho(0.0f, (float)APP_WINDOWSIZEX, 0.0f, (float)APP_WINDOWSIZEY, -1.0f, 1.0f);

      if (width != APP_WINDOWSIZEX || height != APP_WINDOWSIZEY)
         glutReshapeWindow(APP_WINDOWSIZEX, APP_WINDOWSIZEY);
   }
   else {
      glViewport(0, 0, width, height);

      perspective = glm::perspective(glm::radians(80.0f), (float)APP_FBOSIZEX / (float)APP_FBOSIZEY, 0.01f, 1000.0f);
      if (width != APP_WINDOWSIZEX || height != APP_WINDOWSIZEY)
         glutReshapeWindow(APP_WINDOWSIZEX, APP_WINDOWSIZEY);
   }
}

/**
 * @brief Display callback function.
 */
void ENG_API Eng::Base::displayCallback()
{
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   GLint prevViewport[4];
   glGetIntegerv(GL_VIEWPORT, prevViewport);
   glm::mat4 headPos;
   if (renderMode == RenderMode::VR)
   {
      ovr->update();
      headPos = ovr->getModelviewMatrix();
   }

   leap->update();
   const LEAP_TRACKING_EVENT* l = leap->getCurFrame();


   for (int c = 0; c < EYE_LAST; c++)
   {
      if (renderMode == RenderMode::VR)
      {
         OvVR::OvEye curEye = (OvVR::OvEye)c;
         glm::mat4 projMat = ovr->getProjMatrix(curEye, 0.01f, 100.0f);

         // Applica la traslazione alla matrice eye2Head
         glm::mat4 eye2Head = ovr->getEye2HeadMatrix(curEye);

         // Update camera projection matrix:
         glm::mat4 ovrProjMat = projMat * glm::inverse(eye2Head);
#ifdef APP_VERBOSE   
         std::cout << "Eye " << c << " proj matrix: " << glm::to_string(ovrProjMat) << std::endl;
#endif
         glm::mat4 cameraOffset = glm::translate(glm::mat4(1.0f), glm::vec3(posxVr, posyVr, poszVr));
         if (whitePosition)
            cameraOffset = glm::rotate(cameraOffset, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));

         glm::mat4 ovrModelViewMat = glm::inverse(cameraOffset * headPos); 
#ifdef APP_VERBOSE   
         std::cout << "Eye " << c << " modelview matrix: " << glm::to_string(ovrModelViewMat) << std::endl;
#endif
         fbo[c]->render();
         glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
         glEnable(GL_DEPTH_TEST);
         glDepthFunc(GL_LEQUAL);

         Shader::getShader("skyboxShader")->render();
         Shader::getCurrentShader()->setMatrix("projection", ovrProjMat);
         glm::mat4 skyboxView = glm::mat4(glm::mat3(ovrModelViewMat));
         skybox->render(skyboxView, nullptr);

         Shader::getShader("lightShader")->render();
         Shader::getCurrentShader()->setMatrix("projection", ovrProjMat);

         list.render(ovrModelViewMat, ovrProjMat, nullptr);

         Shader::getShader("leapShader")->render();

         glm::mat4 viewMatrix = glm::inverse(cameraOffset * headPos);

         glm::mat4 leapToWorld = glm::mat4(1.0f);
         if (whitePosition) {
            leapToWorld = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.6f, 0.3f));
            leapToWorld = glm::rotate(leapToWorld, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
         }
         else {
            leapToWorld = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.6f, -0.3f));
         }

         glm::mat4 cameraMovement = glm::translate(glm::mat4(1.0f), glm::vec3(posxVr, posyVr, poszVr));

         glm::mat4 leapModelViewMat = viewMatrix * cameraMovement * leapToWorld;

         leap->renderVRHandBones(l, leapModelViewMat, ovrProjMat, cameraMovement * leapToWorld);
         
         ovr->pass(curEye, fboTexId[c]);
      }
      else
      {
         fbo[c]->render();
         glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
         glEnable(GL_DEPTH_TEST);
         glDepthFunc(GL_LEQUAL);

         cameras.at(activeCamera)->setUserTransform(posxVr, posyVr, poszVr, 0.0f, 0.0f, 0.0f);
         
         Shader::getShader("skyboxShader")->render();
         Shader::getCurrentShader()->setMatrix("projection", perspective);
         glm::mat4 skyboxView = glm::mat4(glm::mat3(cameras.at(activeCamera)->getInverseCameraMat()));
         skybox->render(skyboxView, nullptr);

         Shader::getShader("leapShader")->render();

         leap->renderNormalHandBones(l, cameras.at(activeCamera)->getInverseCameraMat(), perspective);

         Shader::getShader("lightShader")->render();
         Shader::getCurrentShader()->setMatrix("projection", perspective);

         list.render(cameras.at(activeCamera)->getTransform(), perspective, nullptr);
      } 
   }

   if (renderMode == RenderMode::VR)
   {
      ovr->render();
   }

   Fbo::disable();
   glViewport(0, 0, prevViewport[2], prevViewport[3]);

   glBindFramebuffer(GL_READ_FRAMEBUFFER, fbo[0]->getHandle());
   glBlitFramebuffer(0, 0, APP_FBOSIZEX, APP_FBOSIZEY, 0, 0, APP_FBOSIZEX, APP_FBOSIZEY, GL_COLOR_BUFFER_BIT, GL_NEAREST);

   glBindFramebuffer(GL_READ_FRAMEBUFFER, fbo[1]->getHandle());
   glBlitFramebuffer(0, 0, APP_FBOSIZEX, APP_FBOSIZEY, APP_FBOSIZEX, 0, APP_WINDOWSIZEX, APP_FBOSIZEY, GL_COLOR_BUFFER_BIT, GL_NEAREST);
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
    leap->setPickableNodes(list.getPickableObjectsList());
    return list.getObjectList(); 
}

void ENG_API Eng::Base::loadSkybox(const std::string& face1, const std::string& face2, const std::string& face3,
   const std::string& face4, const std::string& face5, const std::string& face6) {
      if (skybox != nullptr) {
         std::cout << "C'� gi� una skybox attiva" << std::endl;
         return;
      }
      Skybox* skybox = new Skybox("Skybox");
      Shader::getShader("skyboxShader")->render();
      skybox->setupSkybox(face1, face2, face3, face4, face5, face6);
      skybox->buildCubemap();
      this->skybox = skybox;
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
    glutPostWindowRedisplay(windowId);
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

/**
 * @brief Update the camera position
 * @param posx X translation of the camera.
 * @param posy Y translation of the camera.
 * @param posz Z translation of the camera.
 */
void Eng::Base::updateCameraPosition(float posx, float posy, float posz) {
   posxVr += posx;
   posyVr += posy;
   poszVr += posz;
}

/**
 * @brief Change camera position for chessboard visualization.

 */
void Eng::Base::switchPosition() {
   whitePosition = !whitePosition;
   if (whitePosition) {
      posxVr = 0.04f;
      posyVr = -0.36f;
      poszVr = -0.31f;
   }
   else {
      posxVr = 0.04f;
      posyVr = -0.36f;
      poszVr = 0.31f;
   }
}