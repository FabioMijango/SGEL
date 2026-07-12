#pragma once
#include <string>

enum class ScrollType : Uint8 {
    Horizontal,
    Vertical,
};

enum class InputType : Uint8 {
    Keyboard,
    MouseButton,
    MouseWheel,
    MouseMotion,
};

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

struct InputKeyHasher {
    std::size_t operator()(const InputKey& key) const {
        return hash_value(key);
    }
};

/**
 * @struct Action
 * @brief Represents an input action triggered by keyboard events.
 *
 * Encapsulates an action name with its current state (pressed, released, or unassigned).
 * Used by scenes to process player input mapped to game actions.
 *
 * @see Scene
 */
struct Action {
    /// Action state enumeration
    enum class State {
        Pressed,       ///< Key is currently held down
        Released,      ///< Key was just released
        Horizontal_Scroll,
        Vertical_Scroll,
        Mouse_Motion,
        Not_Assigned   ///< Action not yet assigned
    };

    /// Name/identifier of the action
    std::string name = "NONE";
    /// Current state of the action
    State state = State::Not_Assigned;

    float x = 0.0f;
    float y = 0.0f;
};
