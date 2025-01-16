
#include "commands.hpp"

#include <cstddef>
#include <iostream>
#include <ostream>
#include <stdexcept>

/**
 * Split a string by spaces and generate a UserInput struct.
 * 
 * Consecutive whitespace will be ingored. The first non-whitespace word is treated as
 * the command, all following words are treated as arguments.
 * 
 * @param inputString a user string to split
 * @return The UserInput struct
 */
UserInput split_string(const std::string& inputString) {
	UserInput input;
	std::string currWord;
	/*if (inputString.length() > 255) {
		std::cout << "Command too many character please make sure you have typed your command correctly";
		return input;
	}*/
	for (std::size_t i = 0; i < inputString.size(); ++i) {
		char nextChar = inputString[i];
		if (nextChar == ' ') {
			if (!currWord.empty()) {
				input.store(currWord);
				currWord = "";
			}
		}
		else {
			currWord.push_back(nextChar);
		}
	}

	if (!currWord.empty()) {
		input.store(currWord);
	}

	return input;
}

static void print_commands(std::ostream& out, const Callbacks& cb, SessionState& state ){
    out << "commands are: ";
    bool isFirst = true;

    for ( auto& cmd : cb ) {

		if (state.m_level == UserLevel::GUEST) {
			if (cmd.first.substr(0, 3) == "new" || cmd.first.substr(0, 4) == "load") {
				if (!isFirst) {
					out << ", ";
				}
				out << cmd.first;
				isFirst = false;
			}
			
		}

		if (state.m_level == UserLevel::ADMIN) {
			if (!isFirst) {
				out << ", ";
			}
			out << cmd.first;
			isFirst = false;
		}
		if(state.m_level == UserLevel::USER)
		{
			if (cmd.first.substr(0, 3) != "adm") {
				if (!isFirst) {
					out << ", ";
				}
				out << cmd.first;
				isFirst = false;
			}
		} 
    }

    out << std::endl;
}

/**
 * Main event loop.
 * 
 * This repeatedly matches user input to commands and runs them. Commands are stored
 * in the callbacks object and can be any suitable function object. Inputs should be
 * read from in, and written to the output streams.
 */
void handle_input(std::istream& in, std::ostream& out, const Callbacks& cb) {
	SessionState state;
	char* line = new char[256];
	
	

	do {

		/*if (in.gcount() > 254) {
			out << "to big"<< std::endl;
		}*/
		
		out << "> ";
		in.getline(line, 256);


		// Code from
		// https://stackoverflow.com/questions/5864540/infinite-loop-with-cin-when-typing-string-while-a-number-is-expected
		if (in.fail()) {
			out << "To large" << std::endl;
			in.ignore(std::numeric_limits <std::streamsize>::max(), '\n');
			in.clear();
		}

		auto args = split_string(line);

		auto itr = cb.find( args.command );
		if ( itr == cb.end() ) {
			out << "unknown command" << std::endl;
            print_commands(out, cb, state);
			continue;
		}

		try {
			auto command = itr->second;
			command( out, args, state );
		} catch ( std::exception& e ) {
			out << "error processing command: " << e.what() << std::endl;
		}

	} while (state.m_running);

	delete[] line;
}
