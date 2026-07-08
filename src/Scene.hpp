#pragma once
#include <memory>
#include <unordered_map>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_stdinc.h>

#include "Action.hpp"

class Scene {
protected:
    typedef  std::unordered_map<SDL_Scancode, std::string> ActionMap;
    // EntityManager m_entityManager;
    ActionMap m_actionMap;
    bool m_hasEnded   = false;

    void registerAction(const SDL_Scancode key, const std::string& action) {
        m_actionMap[key] = action;
    }

public:
    Scene() = default;
    virtual ~Scene() = default;

    virtual bool            init() = 0;
    virtual SDL_AppResult   update(float deltaTime) = 0;
    virtual SDL_AppResult   eventHandler(const SDL_Event* event) { return SDL_APP_CONTINUE; };
    virtual void            sDoAction(const Action& action) = 0;
    virtual void            sRender(SDL_Renderer* renderer) = 0;
    virtual void            exit() = 0;

    virtual std::shared_ptr<Scene> changeScene() = 0;

    const ActionMap& getActionMap() const { return m_actionMap; };

    bool hasEnded() const { return m_hasEnded; };
};
