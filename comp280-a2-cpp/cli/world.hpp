#ifndef COMP280_CLI_WORLD_H
#define COMP280_CLI_WORLD_H

#include "../game/world.hpp"
#include "../commands.hpp"

void showLinks(std::ostream& out, UserInput& input, SessionState& state);

void go(std::ostream& out, UserInput& input, SessionState& state);

void use(std::ostream& out, UserInput& input, SessionState& state);

void admTeleport(std::ostream& out, UserInput& input, SessionState& state);

#endif
