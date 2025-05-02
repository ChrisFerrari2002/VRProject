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
#include "board.h"

Eng::Base& eng = Eng::Base::getInstance();

// Fps calculation
int fc;
int fps;

#define CHESSBOARD_SIZE 8
#define BLOCK_SIZE 0.0835f
#define FILE_NAME "newScene.ovo"

// Matrixes
std::pair<int, unsigned int> matrix[CHESSBOARD_SIZE][CHESSBOARD_SIZE];
bool positioningMatrix[CHESSBOARD_SIZE][CHESSBOARD_SIZE];

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
Board chessboard;
glm::mat4 initialTransform;
bool moveConfirmed = false;

// Other vars
Eng::Camera* cameras[4];
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
 * @brief Make an object blink (the selected piece will blink)
 *
 * @param obj The object to make blink.
 */
void makeObjectBlink(Eng::Node* obj) {
    if (!(obj->getName().substr(0, 1) == "p")) return;

    step = 0.01f;

    if (blink) {
        blinkStep += step;
        if (blinkStep > range)
            blink = false;
    }
    else {
        blinkStep -= step;
        if (blinkStep < 0.0f)
            blink = true;
    }

    ((Eng::Mesh*)obj)->getMaterial()->setEmission(glm::vec4(blinkStep * 5.0f, 0.0f, 0.0f, 1.0f));
}

/**
 * @brief Update the blinking state of the object
 *
 * @param value Timer value (state)
 */
void updateBlinking(int value) {
    makeObjectBlink(pickedObject);
    eng.startTimer(updateBlinking, 10);
}

/**
 * @brief Rotate the camera
 *
 * @param value Timer value
 */
void rotateCamera(int value) {
    static bool rotationSense = false;
    static float angle = 0.0f;
    glm::mat4 currentTransform = cameras[0]->getTransform();
    float rotation = (rotationSense) ? -0.002f : 0.002f;

    currentTransform = glm::rotate_slow(currentTransform, rotation, glm::vec3(0.0f, 1.0f, 0.0f));
    cameras[0]->setTransform(currentTransform);

    if (angle > 0.15)
        rotationSense = true;
    else if (angle < -0.15f)
        rotationSense = false;
    angle += rotation;

    // Restart timer
    eng.startTimer(rotateCamera, 10);
}

/**
 * @brief Get the picked object
 *
 * @param n The node representing the picked object.
 * @param mousePressed Whether the mouse button is pressed.
 */
void getPickedObject(Eng::Node* n, bool mousePressed) {
    moveConfirmed = false;
    if (n == nullptr || n->getName().substr(0, 1) != "p") return;
    if (mousePressed) {
        if (pickedObject != nullptr) {
            if (!moveConfirmed) {
                chessboard.getSelectedPiece()->resetPosition();
            }
            ((Eng::Mesh*)pickedObject)->getMaterial()->setEmission(lastObjectEmission);
        }
        lastObjectEmission = ((Eng::Mesh*)n)->getMaterial()->getEmission();
        pickedObject = n;
        chessboard.setSelectedPiece(pickedObject);

        // Start blinker timer if it isn't already running
        if (!blinkerTimerStarted) {
            eng.startTimer(updateBlinking, 10);
            blinkerTimerStarted = true;
        }
    }
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
 * @brief Update the FPS counter
 *
 * @param value Timer value (not used).
 */
void updateFPS(int value) {
    fps = fc;
    fc = 0;
    eng.startTimer(updateFPS, 1000);
}

/**
 * @brief Load the scene from a file
 *
 * @param pathName The path to the scene file.
 */
void loadScene(std::string pathName) {
    list = eng.loadScene(pathName);
}

/**
 * @brief Makes the light blinking
 *
 */
void blinkLight() {
    static bool blink = false;
    static Eng::Light* light = (Eng::Light*)(eng.getList()->getObject(0));
    if (lightOn) {
        light->setIntensity(7.0f);
    }
    else {
        light->setIntensity(0.0f);
    }
}

/**
 * @brief Load the cameras
 */
void loadCameras() {
    Eng::Camera* c1 = new Eng::Camera("camera1");
    c1->setUserTransform(6.0f, 2.0f, 0.0f, 0.0f, -90.0f, 0.0f);
    Eng::Camera* c2 = new Eng::Camera("camera2");
    c2->setUserTransform(0.0f, 2.0f, 1.5f, -50.0f, 0.0f, 0.0f);
    Eng::Camera* c3 = new Eng::Camera("camera3");
    c3->setUserTransform(0.0f, 2.0f, -1.5f, -130.0f, 0.0f, 180.0f);
    Eng::Camera* c4 = new Eng::Camera("camera4");
    c4->setUserTransform(-6.0f, 2.0f, 0.0f, 0.0f, -90.0f, 0.0f);
    eng.addCamera(c1);
    eng.addCamera(c2);
    eng.addCamera(c3);
    eng.addCamera(c4);

    cameras[0] = c1;
    cameras[1] = c2;
    cameras[2] = c3;
    cameras[3] = c4;

    eng.setActiveCamera(1);
}

/**
 * @brief Handle special key events
 *
 * @param key The key code.
 * @param x The x-coordinate of the mouse.
 * @param y The y-coordinate of the mouse.
 */
void specialCallback(int key, int x, int y) {
    if (pickedObject == nullptr || chessboard.getSelectedPiece() == nullptr) return;

    switch (key)
    {
    case 100: // Left arrow
        chessboard.getSelectedPiece()->moveLeft();
        break;
    case 102: // Right arrow
        chessboard.getSelectedPiece()->moveRight();
        break;
    case 101: // Up arrow
        chessboard.getSelectedPiece()->moveUp();
        break;
    case 103: // Down arrow
        chessboard.getSelectedPiece()->moveDown();
        break;
    default:
        break;
    }
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
    case '1':
        eng.setActiveCamera(0);
        break;
    case '2':
        eng.setActiveCamera(1);
        break;
    case '3':
        eng.setActiveCamera(2);
        break;
    case '4':
       eng.setActiveCamera(3);
       break;
    case 13:
        moveConfirmed = true;
        chessboard.confirmMovement();
        break;
    case 'r':
        chessboard.resetBoard();
        break;
    case 'u':
        chessboard.undo();
        break;
    case 'i':
        chessboard.redo();
        break;
    case 'l':
        lightOn = !lightOn;
        blinkLight();
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
    eng.loadSkybox("posx.jpg", "negx.jpg", "posy.jpg", "negy.jpg", "posz.jpg", "negz.jpg");
    loadCameras();
    std::cout << std::filesystem::current_path() << std::endl;
    loadScene(".." + getSeparator() + "scene" + getSeparator() + FILE_NAME);
    eng.setWindowResizeHandler(handleWindowResize);
    eng.setKeyboardCallback(keyboardCallback);
    eng.setBackgroundColor(0.01f, 0.01f, 0.3f, 1.0f);
    eng.start();
    eng.startTimer(updateFPS, 1000);
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
    eng.startTimer(rotateCamera, 10);
    while (eng.isRunning()) {
        eng.update();
        eng.refreshAndSwapBuffers();
        fc++;
    }

    // Release engine:
    eng.free();

    // Done:
    std::cout << "\n[application terminated]" << std::endl;
    return 0;
}