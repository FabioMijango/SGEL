#pragma once
#include <string>

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
        Not_Assigned   ///< Action not yet assigned
    };

    /// Name/identifier of the action
    std::string name = "NONE";
    /// Current state of the action
    State state = State::Not_Assigned;
};
