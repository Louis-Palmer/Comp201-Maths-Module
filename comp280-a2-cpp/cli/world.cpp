#include "world.hpp"

#include <cassert>
#include <ostream>

void showLinks(std::ostream& out, UserInput& input, SessionState& state) {
	if ( !isUser(state.m_level) || state.m_gameState == nullptr) {
		out << "You need to be logged in to do that..." << std::endl;
		return;
	}
	assert( state.m_world != nullptr );
	auto location = state.m_gameState->location;

	// world
	auto* world = state.m_world;
	auto links = world->getLinksFrom( location );

	// show text
	out << "You are in " << location << std::endl;
	for ( std::size_t idx = 0; idx < links.size(); ++idx ) {
		if ( idx != 0 ) {
			out << ", ";
		}
		out << links[idx];
	}

	if ( !links.empty() ) {
		out << std::endl;
	}

}

void go(std::ostream& out, UserInput& input, SessionState& state) {
	if ( !isUser(state.m_level) || state.m_gameState == nullptr ) {
		out << "You need to be logged in to do that..." << std::endl;
		return;
	}
	if (input.args.size() != 1) {
		out << "invalid number of arguments" << std::endl;
		return;
	}

	auto location = state.m_gameState->location;
	auto target = input.args[0];

	// world
	assert( state.m_world != nullptr );
	auto* world = state.m_world;
	if ( !world->hasLink(location, target) ) {
		out << "there is no way to get there from here" << std::endl;
		return;
	}

	// copy the location to the location state variable
	for ( std::size_t i=0; i<16; ++i ) {
		if ( target.size() > i ) {
			state.m_gameState->location[i] = target[i];
		} else {
			state.m_gameState->location[i] = 0;
		}
	}

	// show text
	out << "You are in " << target << std::endl;

    // check for win state
    if ( state.m_gameState->inLocation("dungeon") ) {
        out << "A winner is you!" << std::endl;
        out << "\t Look, I have no idea why going to the dungeon makes you win either." << std::endl;
        out << "\t I'm a programmer, not a designer..." << std::endl;
        state.m_running = false;
    }
}

void admTeleport(std::ostream& out, UserInput& input, SessionState& state) {
	if ( !isUser(state.m_level) || state.m_gameState == nullptr ) {
		out << "You need to be logged in to do that..." << std::endl;
		return;
	}
	if (!is_active(state) || state.m_level != UserLevel::ADMIN) {
		out << "You need to be Admin in to do that..." << std::endl;
		return;
	}

	if (input.args.size() != 1) {
		out << "invalid number of arguments" << std::endl;
		return;
	}
	auto target = input.args[0];

	// copy the location to the location state variable
	for ( std::size_t i=0; i<16; ++i ) {
		if ( target.size() > i ) {
			state.m_gameState->location[i] = target[i];
		} else {
			state.m_gameState->location[i] = 0;
		}
	}

	// show text
	out << "You teleport to " << target << std::endl;
}


void use(std::ostream& out, UserInput& input, SessionState& state) {
	if ( !isUser(state.m_level) || state.m_gameState == nullptr ) {
		out << "You need to be logged in to do that..." << std::endl;
		return;
	}
	if (input.args.size() != 1) {
		out << "invalid number of arguments" << std::endl;
		return;
	}
	Item item = (Item)std::stoi( input.args[0] );

    switch (item) {
		
        case Item::POTION:
			if (!state.m_gameState->hasItem(Item::POTION)) {
				out << "you don't have that item..." << std::endl;
				return;
			}
            state.m_gameState->health += 100;
            state.m_gameState->removeItem( Item::POTION );
            out << "you drink a potion" << std::endl;
            break;
        case Item::KEY:
            if ( !state.m_gameState->hasItem( Item::KEY ) ) {
                out << "you don't have that item..." << std::endl;
                return;
            }

            // check if they are at the gate
            if ( state.m_gameState->inLocation("gate") ) {
                state.m_gameState->removeItem( Item::KEY );
                state.m_world->addLink("gate", "castle");
                out << "you use the key on the gate and it opens..." << std::endl;
            } else {
                out << "you wave the key around for a bit..." << std::endl;
            }
            break;
        default:
            out << "you don't see how that would be useful right now..." << std::endl;
    }

}
