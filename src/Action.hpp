#pragma once
#include <string>

struct Action {
    enum class State { Pressed, Released, Not_Assigned };

    std::string name = "NONE";
    State state = State::Not_Assigned;
};
