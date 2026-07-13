#pragma once
#include <string>
#include <SDL3/SDL_stdinc.h>


/**
 * @enum ScrollType
 * @brief Represents the orientation of mouse wheel scrolling.
 */
enum class ScrollType : Uint8 {
    Horizontal,
    Vertical,
};


/**
 * @enum InputType
 * @brief Represents the type of input event.
 */
enum class InputType : Uint8 {
    Keyboard,
    MouseButton,
    MouseWheel,
    MouseMotion,
};


/**
 * @struct InputKey
 * @brief Represents a unique input event key.
 */
struct InputKey {
    InputType type;
    Uint32 code;

    auto operator<=>(const InputKey&) const = default;

    friend std::size_t hash_value(const InputKey &obj) {
        std::size_t seed = 0x087DE6E7;
        seed ^= (seed << 6) + (seed >> 2) + 0x0843AD62 + static_cast<std::size_t>(obj.type);
        seed ^= (seed << 6) + (seed >> 2) + 0x007AD7CC + static_cast<std::size_t>(obj.code);
        return seed;
    }
};

/**
 * @struct InputKeyHasher
 * @brief Hash function for InputKey to be used in unordered_map.
 */
struct InputKeyHasher {
    std::size_t operator()(const InputKey& key) const {
        return hash_value(key);
    }
};

/**
 * @struct Action
 * @brief Represents an input action triggered by keyboard or mouse input, along with its current state.
 *
 * Encapsulates an action with its current state (pressed, scroll, mouse motion, etc.).
 * Used by scenes to process player input mapped to game actions.
 *
 * @see Scene, InputKey, ScrollType, InputType
 */
struct Action {
    /// Action state enumeration
    enum class State {
        Pressed,            ///< Key is currently held down
        Released,           ///< Key was just released
        Horizontal_Scroll,  ///< Mouse wheel scrolled horizontally
        Vertical_Scroll,    ///< Mouse wheel scrolled vertically
        Mouse_Motion,       ///< Mouse motion event
        Not_Assigned        ///< Action not yet assigned
    };

    /// Name/identifier of the action
    std::string name = "NONE";
    /// Current state of the action
    State state = State::Not_Assigned;

    float x = 0.0f;         ///< X value, used for mouse position and horizontal scroll
    float y = 0.0f;         ///< Y value, used for mouse position and vertical scroll
};
