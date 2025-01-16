#ifndef COMP280_COMMANDS_H
#define COMP280_COMMANDS_H

#include <vector>
#include <string>
#include <map>
#include <functional>

#include "session.hpp"

struct UserInput {
	std::string command;
	std::vector<std::string> args;

	inline void store(const std::string& word) {
		if (command.empty()) {
			command = word;
		} else {
			args.push_back(word);
		}
	}
};

/**
 * Is this user currently in a game session?
 */
inline bool is_active(SessionState& state) {
    return isUser(state.m_level) && state.m_gameState != nullptr;
}


/**
 * A type alias for the callback functions.
 *
 * A callback is a function is one that accepts:
 *  1. the output stream for the user
 *  2. the current user's input
 *  3. the current session state
 *  and returns nothing.
 */
using Callback = std::function<void(std::ostream&, UserInput&, SessionState&)>;

/**
 * Command callback storage type.
 * These actually process the commands.
 */
using Callbacks = std::map<std::string, Callback>;

/**
 * Split a string by spaces and generate a UserInput struct.
 * 
 * Consecutive whitespace will be ingored. The first non-whitespace word is treated as
 * the command, all following words are treated as arguments.
 * 
 * @param inputString a user string to split
 * @return The UserInput struct
 */
UserInput split_string(const std::string& inputString);

/**
 * Main event loop.
 * 
 * This repeatedly matches user input to commands and runs them. Commands are stored
 * in the callbacks object and can be any suitable function object. Inputs should be
 * read from in, and written to the output streams.
 */
void handle_input(std::istream& in, std::ostream& out, const Callbacks& cb);

#endif // COMP280_COMMANDS_H
