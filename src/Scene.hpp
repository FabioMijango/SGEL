#pragma once
#include <memory>
#include <unordered_map>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_stdinc.h>

#include "Action.hpp"

/**
 * @class Scene
 * @brief Abstract base class for game scenes.
 *
 * Provides the interface for implementing game states/screens.
 * Scenes handle initialization, updates, rendering, input, and exit logic.
 * Supports action mapping from keyboard input.
 *
 * @see Engine, Action
 */
class Scene {
protected:
    typedef std::unordered_map<InputKey, Action, InputKeyHasher> ActionMap;

    /// Map of SDL_Scancode keys to action names
    ActionMap   m_actionMap;
    /// Flag indicating if the scene should exit so the engine could get the next one
    bool        m_hasEnded   = false;

    void registerAction(const SDL_Scancode scancode, const std::string& name) {
        InputKey key{InputType::Keyboard, static_cast<Uint32>(scancode)};
        m_actionMap[key] = Action{name, Action::State::Not_Assigned};
    }

    void registerAction(SDL_MouseButtonFlags mouseButton, const std::string& name) {
        InputKey key{InputType::MouseButton, static_cast<Uint32>(mouseButton)};
        m_actionMap[key] = Action{name, Action::State::Not_Assigned};
    }

    void registerAction(ScrollType scrollOrientation, const std::string& name) {
        InputKey key{InputType::MouseWheel, static_cast<Uint32>(scrollOrientation)};
        m_actionMap[key] = Action{name, Action::State::Not_Assigned, 0};
    }

    void registerMouseMotion(const std::string& name) {
        InputKey key{InputType::MouseMotion, 0};
        m_actionMap[key] = Action{name, Action::State::Not_Assigned, 0.0f, 0.0f};
    }

public:
    Scene() = default;
    virtual ~Scene() = default;

    /**
     * @brief Initializes the scene.
     * @return true if initialization successful, false otherwise.
     */
    virtual bool            init() = 0;

    /**
     * @brief Updates scene logic each frame.
     * @param deltaTime Time in seconds since last frame.
     * @return SDL_APP_CONTINUE to continue, SDL_APP_FAILURE to quit.
     */
    virtual SDL_AppResult   update(float deltaTime) = 0;

    /**
     * @brief Handles SDL events (keyboard, mouse, etc.).
     * @param event Pointer to SDL_Event.
     * @return SDL_APP_CONTINUE on success.
     * @note Optional; default implementation does nothing.
     */
    virtual SDL_AppResult   eventHandler(const SDL_Event* event) { return SDL_APP_CONTINUE; };

    /**
     * @brief Processes an action triggered by input.
     * @param action Action containing name and state (pressed/released).
     */
    virtual void            sDoAction(const Action& action) = 0;

    /**
     * @brief Renders the scene.
     * @param renderer SDL_Renderer instance for drawing.
     */
    virtual void            sRender(SDL_Renderer* renderer) = 0;

    /**
     * @brief Cleans up scene resources before exit.
     */
    virtual void            exit() = 0;

    /**
     * @brief Optionally changes to a different scene. Called when flag m_hasEnded is true.
     * @return Shared pointer to the next scene.
     */
    virtual std::shared_ptr<Scene> changeScene() = 0;

    /**
     * @brief Gets the action map.
     * @return Const reference to the key-to-action mapping.
     */
    const ActionMap& getActionMap() const { return m_actionMap; };

    /**
     * @brief Checks if the scene has ended.
     * @return true if the scene should exit, false otherwise.
     */
    bool hasEnded() const { return m_hasEnded; };
};
