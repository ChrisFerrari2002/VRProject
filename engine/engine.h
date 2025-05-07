/**
 * @file engine.h
 * @brief Graphics engine main include file
 *
 * This file contains the main include file for the graphics engine, including the definition of the Base class.
 *
 * @date 2025
 *
 * @details The Base class is the main class of the graphics engine and is implemented as a singleton.
 * @see Eng::Node, Eng::Camera, Eng::Light, Eng::Mesh, Eng::Texture, Eng::Material, Eng::List, Eng::OvoReader
 *
 * @authors
 * - Sara Bressan [sara.bressan@student.supsi.ch]
 * - Chris Ferrari [chris.ferrari@student.supsi.ch]
 * - Alessandro Formato [alessandro.formato@student.supsi.ch]
 */
#pragma once

 //////////////
 // #INCLUDE //
 //////////////

    // C/C++:    
#include <LeapC.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/packing.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>
#include <iostream>
#include <source_location>
#include <memory>
#include <vector>
#include <string>
#include <list>
#include <map>

/////////////
// VERSION //
/////////////

   // Generic info:
#ifdef _DEBUG
#define LIB_NAME      "My Graphics Engine v0.1a (debug)"   ///< Library credits
#else
#define LIB_NAME      "My Graphics Engine v0.1a"   ///< Library credits
#endif
#define LIB_VERSION   10                           ///< Library version (divide by 10)

// Export API:
#ifdef _WINDOWS
   // Specifies i/o linkage (VC++ spec):
#ifdef ENGINE_EXPORTS
#define ENG_API __declspec(dllexport)
#else
#define ENG_API __declspec(dllimport)
#endif      

// Get rid of annoying warnings:
#pragma warning(disable : 4251) 
#else // Under linux
#define ENG_API
#endif

///////////////
// NAMESPACE //
///////////////

namespace Eng {

    //////////////
    // #INCLUDE //
    //////////////   

       // You can subinclude here other headers of your engine...
#include "object.h"
#include "node.h"
#include "leap.h"
#include "skybox.h"
#include "fbo.h"
#include "ovVr.h"
#include "texture.h"
#include "material.h"
#include "camera.h"
#include "vertex.h"
#include "shader.h"
#include "mesh.h"
#include "light.h"
#include "directionalLight.h"
#include "pointLight.h"
#include "spotLight.h"
#include "frustum.h"
#include "list.h"
#include "LODData.h"
#include "ovoReader.h"

///////////////////////
// MAIN ENGINE CLASS //
///////////////////////

/**
 * @brief Base engine main class. This class is a singleton.
 *
 * The Base class is the main class of the graphics engine and is implemented as a singleton.
 * The responsibilities of the Base class include initializing the engine, managing the scene, and handling user input.
 */
    class ENG_API Base final {
    public:	      
        /**
         * @brief Constructor
         *
         * Initializes the Base class.
         */
        Base(Base const&) = delete;

        /**
         * @brief Destructor
         *
         * Destroys the Base class.
         */
        ~Base();

        /**
         * @brief Assignment operator
         *
         * Prevents assignment of the Base class.
         */
        void operator=(Base const&) = delete;

        /**
         * @brief Get the singleton instance
         *
         * @return Reference to the singleton instance.
         */
        static Base& getInstance();

        /**
         * @brief Initialize the engine
         *
         * Initializes the engine with the given parameters.
         *
         * @param argc Number of command-line arguments.
         * @param argv Array of command-line arguments.
         * @param windowTitle Title of the window.
         * @param windowWidth Width of the window.
         * @param windowHeight Height of the window.
         * @return True if initialization was successful, false otherwise.
         */
        bool init(int argc, char** argv, const char* windowTitle);

        /**
         * @brief Free the engine resources
         *
         * Frees the resources used by the engine.
         *
         * @return True if the resources were successfully freed, false otherwise.
         */
        bool free();

        /**
         * @brief Set Z-buffer usage
         *
         * Sets whether the Z-buffer should be used.
         *
         * @param useZBuffer True to use the Z-buffer, false otherwise.
         */
        void setZBufferUsage(bool useZBuffer);

        /**
         * @brief Set the background color of the window
         *
         * Sets the background color of the window based on the given rgba params.
         *
         * @param r Red component of the color.
         * @param g Green component of the color.
         * @param b Blue component of the color.
         * @param a Alpha component of the color.
         */
        void setBackgroundColor(float r, float g, float b, float a);

        /**
         * @brief Set the window resize handler
         *
         * Sets the function to be called when the window is resized.
         *
         * @param func Pointer to the resize handler function.
         */
        void setWindowResizeHandler(void (*func)(int, int));

        /**
         * @brief Check if the engine is running
         *
         * @return True if the engine is running, false otherwise.
         */
        bool isRunning();

        /**
         * @brief Start the engine
         *
         * Starts the engine.
         */
        void start();

        /**
         * @brief Handle window reshape event
         *
         * Handles the window reshape event.
         *
         * @param width New width of the window.
         * @param height New height of the window.
         */
        void reshapeCallback(int width, int height);

        /**
         * @brief Post a window redisplay event
         *
         * Posts a window redisplay event.
         */
        void postWindowRedisplay();

        /**
         * @brief Update the engine
         *
         * Updates the engine.
         * This function calls the glutMainLoopEvent (instead of using glutMainLoop that blocks the whole program).
         */
        void update();

        /**
         * @brief Refresh and swap the buffers
         *
         * Refreshes the window and swaps the buffers.
         */
        void refreshAndSwapBuffers();

        /**
         * @brief Set the keyboard callback
         *
         * Sets the function to be called when a key is pressed.
         *
         * @param func Pointer to the keyboard callback function.
         */
        void setKeyboardCallback(void (*func)(unsigned char, int, int));

        /**
         * @brief Set the special key callback
         *
         * Sets the function to be called when a special key is pressed.
         *
         * @param func Pointer to the special key callback function.
         */
        void setSpecialCallback(void (*func)(int, int, int));

        /**
         * @brief Remove the object picked callback
         *
         * Removes the function to be called when an object is picked.
         */
        void removeObjectPickedCallback();

        /**
         * @brief Load a scene from a file
         *
         * Loads a scene from the specified file.
         *
         * @param sceneFile Path to the scene file.
         * @return A list of nodes representing the scene.
         */
        std::list<Node*> loadScene(std::string sceneFile);

        /**
         * @brief Set the active camera
         *
         * Sets the active camera selected by index.
         *
         * @param index Index of the camera to be set as active.
         */
        void setActiveCamera(int index);

        /**
         * @brief Add a camera to the engine
         *
         * Adds a camera to the engine.
         *
         * @param camera Pointer to the camera to be added.
         */
        void addCamera(Eng::Camera* camera);

        /**
         * @brief Add a skybox to the engine
         *
         * Add a skybox to the engine
         *
         * @param face1 Path of face1.
         * @param face2 Path of face2.
         * @param face3 Path of face3.
         * @param face4 Path of face4.
         * @param face5 Path of face5.
         * @param face6 Path of face6.
         * 
         */
        void loadSkybox(const std::string& face1, const std::string& face2, const std::string& face3,
           const std::string& face4, const std::string& face5, const std::string& face6);

        /**
         * @brief Update the camera position
         *
         * Update the camera position
         *
         * @param posx X translation of the camera.
         * @param posy Y translation of the camera.
         * @param posz Z translation of the camera.
         */
        void updateCameraPosition(float posx, float posy, float posz);

        /**
         * @brief Change camera visualization from black to white position
         *
         * Change camera visualization from black to white position
         */
        void switchPosition();

    private: 

        // Reserved:
		struct Reserved; /**< Reserved structure */
		std::unique_ptr<Reserved> reserved; /**< Reserved pointer */ 

        /**
         * @brief Constructor
         *
         * Initializes the Base class.
         */
        Base();

        // Context params:
		int windowId; /**< Window ID */

        /**
         * @brief Display callback
         *
         * Callback function for display events.
         */
        static void displayCallback();

        // Internal vars:
		static bool initFlag;  /**< Initialization flag */
		static bool useZBuffer; /**< Z-buffer usage flag */
		static bool mainLoopRunning; /**< Main loop running flag */
		static float bgR; /**< Background red component */
		static float bgG; /**< Background green component */
		static float bgB; /**< Background blue component */
		static float bgA; /**< Background alpha component */

		static Eng::OvoReader reader; /**< OvoReader object */
		static Eng::List list; /**< List object */
      static Eng::Skybox* skybox; /**< Skybox object */

        // Cameras
		static int activeCamera; /**< Active camera index */
		static std::vector<Eng::Camera*> cameras; /**< List of cameras */
    };

}; // end of namespace Eng::