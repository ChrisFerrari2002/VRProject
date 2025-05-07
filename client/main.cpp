/**
 * @file main.cpp
 * @brief Client application (that uses the graphics engine)
 *
 * This file contains the main function and related functions for the client application that uses the graphics engine.
 * The client application initializes the graphics engine, loads the scene, and handles user input to interact with the chessboard.
 * The app is in fact a chessboard where the user can move the pieces by selecting them and then moving them around the board.
 *
 * @details The client application initializes the graphics engine, loads the scene, and handles user input to interact with the chessboard.
 *
 * @date 2025
 *
 * @see Eng::Base, Board
 *
 * @authors
 * - Sara Bressan [sara.bressan@student.supsi.ch]
 * - Chris Ferrari [chris.ferrari@student.supsi.ch]
 * - Alessandro Formato [alessandro.formato@student.supsi.ch]
 */

 //////////////
 // #INCLUDE //
 //////////////

#include <filesystem>
// Library header:
#include "engine.h"

Eng::Base& eng = Eng::Base::getInstance();

#define FILE_NAME "newScene.ovo"

// Object blinking vars
Eng::Node* pickedObject = nullptr;
glm::vec3 lastObjectEmission;
static float range = .5f;
static float step = .01f;
static float blinkStep = 0.0f;
bool blink = false;
bool blinkerTimerStarted = false;

// List with all elements
std::list<Eng::Node*> list;

// Other vars
Eng::Camera* cameras[1];
bool lightOn = true;

/**
 * @brief Get the right separator depending on the OS this program runs on
 *
 * @return The file path separator for the current OS.
 */
std::string getSeparator() {
#ifdef _WIN32
    return "\\";
#else
    return "/";
#endif
}

/**
 * @brief Handle window resize event
 *
 * @param w The new width of the window.
 * @param h The new height of the window.
 */
void handleWindowResize(int w, int h) {
    eng.reshapeCallback(w, h);
}

/**
 * @brief Load the scene from a file
 *
 * @param pathName The path to the scene file.
 */
void loadScene(std::string pathName) {
   std::cout << "Loading scene: " << pathName << std::endl;
    list = eng.loadScene(pathName);
}

/**
 * @brief Load the cameras
 */
void loadCameras() {
    Eng::Camera* camera = new Eng::Camera("camera");
    camera->setUserTransform(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
    cameras[0] = camera;
    eng.addCamera(camera);
    eng.setActiveCamera(0);
}

/**
 * @brief Handle special key events
 *
 * @param key The key code.
 * @param x The x-coordinate of the mouse.
 * @param y The y-coordinate of the mouse.
 */
void specialCallback(int key, int x, int y) {
    //float moveStep = 1.0f; // NORMAL
    float moveStep = 0.01f; // VR
    float moveStepX = 0.0f;
    float moveStepY = 0.0f;
    float moveStepZ = 0.0f;
    switch (key)
    {
    case 100: // Left arrow
       moveStepX -= moveStep; // Aggiorna la posizione X
       break;
    case 102: // Right arrow
       moveStepX += moveStep; // Aggiorna la posizione X
       break;
    case 101: // Up arrow (Avanti)
       moveStepZ += moveStep; // Aggiorna la posizione Z (Avanti)
       break;
    case 103: // Down arrow (Indietro)
       moveStepZ -= moveStep; // Aggiorna la posizione Z (Indietro)
       break;
    case 104: // Page up (Alzarsi)
       moveStepY += moveStep++; // Aggiorna la posizione Y (Su)
       break;
    case 105: // Page down (Abbassarsi)
       moveStepY -= moveStep; // Aggiorna la posizione Y (Giù)
       break;
    default:
       break;
    }

    // Passa la posizione aggiornata alla funzione di rendering
    eng.updateCameraPosition(moveStepX, moveStepY, moveStepZ);
    eng.postWindowRedisplay();
}

/**
 * @brief Handle keyboard events
 *
 * @param key The key code.
 * @param x The x-coordinate of the mouse.
 * @param y The y-coordinate of the mouse.
 */
void keyboardCallback(unsigned char key, int x, int y) {
   switch (key) {
   case 'c':
      eng.switchPosition();
      break;
   }
    eng.postWindowRedisplay();
}

/**
 * @brief Initialize the application
 *
 * @param argc Number of command-line arguments.
 * @param argv Array of command-line arguments.
 */
void init(int argc, char* argv[])
{
    eng.setZBufferUsage(true);
    eng.loadSkybox("../skybox/posx.jpg", "../skybox/negx.jpg", "../skybox/posy.jpg", "../skybox/negy.jpg", "../skybox/posz.jpg", "../skybox/negz.jpg");
    loadCameras();
    std::cout << std::filesystem::current_path() << std::endl;
    loadScene(".." + getSeparator() + "scene" + getSeparator() + FILE_NAME);
    eng.setWindowResizeHandler(handleWindowResize);
    eng.setKeyboardCallback(keyboardCallback);
    eng.setSpecialCallback(specialCallback);
    eng.setBackgroundColor(0.01f, 0.01f, 0.3f, 1.0f);
    eng.start();
}

//////////
// MAIN //
//////////

/**
 * @brief Application entry point.
 * 
 * The main function initializes the engine, loads the scene, and starts the main loop.
 *
 * @param argc Number of command-line arguments passed.
 * @param argv Array containing up to argc passed arguments.
 * @return Error code (0 on success, error code otherwise).
 */
int main(int argc, char* argv[])
{
    // Credits:
    std::cout << "Chessboard Project - Group 12" << std::endl;
    std::cout << std::endl;

    // Initialize the engine
    if (!eng.init(argc, argv, "Chessboard project"))
    {
        std::cerr << "Failed to initialize the engine" << std::endl;
        return -1;
    }
    init(argc, argv);
    while (eng.isRunning()) {
        eng.update();
        eng.refreshAndSwapBuffers();
    }

    // Release engine:
    eng.free();

    // Done:
    std::cout << "\n[application terminated]" << std::endl;
    return 0;
}