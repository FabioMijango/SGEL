#pragma once
#include <cstdlib>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_render.h>

#include "Assets.hpp"
#include "FpsManager.hpp"
#include "Scene.hpp"

/**
 * @class Engine
 * @brief Main engine class managing SDL systems, scenes, and the main game loop.
 *
 * Singleton pattern. Handles window creation, rendering, frame rate management,
 * and scene updates. Coordinates all engine subsystems including assets and input.
 *
 * @see Scene, Assets
 */
class Engine {
    /// SDL window handle
    SDL_Window*     m_window        = nullptr;
    /// SDL renderer handle
    SDL_Renderer*   m_renderer      = nullptr;
    /// Frame rate manager instance
    FPSManager*     m_fpsManager    = nullptr;
    /// Time delta in seconds since last frame
    float           m_deltaTime     = 0;
    /// Currently active scene
    std::shared_ptr<Scene> m_scene;

    Engine() = default;

public:
    friend class Assets;

    /**
     * @brief Gets the singleton instance of the Engine.
     * @return Reference to the Engine singleton.
     */
    static Engine&  Instance();

    /**
     * @brief Initializes the engine with window and scene.
     * @param windowSize Window dimensions in pixels.
     * @param fpsLimit Frame rate limit in FPS.
     * @param initialScene Initial scene to load.
     * @param windowTitle Title for the application window.
     * @param windowFlags Flags for configuring the application window.
     * @return SDL_APP_CONTINUE on success, SDL_APP_FAILURE on error.
     */
    SDL_AppResult   init(const SDL_Point &windowSize, Uint32 fpsLimit, const std::shared_ptr<Scene>& initialScene, const std::string& windowTitle= "SGEL App", SDL_WindowFlags windowFlags = SDL_WINDOW_RESIZABLE);

    /**
     * @brief Sets the window icon from a file path.
     * @param iconPath Path to the icon file.
     * @return SDL_APP_CONTINUE on success, SDL_APP_FAILURE on error.
     */
    SDL_AppResult   setWindowIcon(const std::string& iconPath);

    /**
     * @brief Updates the engine, scene, and frame timing.
     * @return SDL_APP_CONTINUE to continue, SDL_APP_FAILURE to quit.
     */
    SDL_AppResult   update();

    /**
     * @brief Renders the current scene.
     */
    void            sRender() const;

    /**
     * @brief Handles user input events.
     * @param event Pointer to SDL_Event.
     * @return SDL_APP_CONTINUE on success.
     */
    SDL_AppResult   sUserInput(const SDL_Event *event) const;

    /**
     * @brief Cleans up and quits the engine.
     */
    void            sQuit() const;
};
