
#include <iostream>
#include <istream>
#include <vector>
#include <cassert>

#include "commands.hpp"

#include "cli/world.hpp"
#include "cli/shop.hpp"

#include "session.hpp"

int AdminTries = 3;

static void pong(std::ostream& out, UserInput& input, SessionState& state) {
	out << "pong" << std::endl;
}

static void quit(std::ostream& out, UserInput& input, SessionState& state) {
    if ( state.m_gameState != nullptr ) {
	    game_save_state(state.m_gameState);
    }
	state.m_running = false;
}

static void newGame(std::ostream& out, UserInput& input, SessionState& state) {
	if (isUser(state.m_level) || state.m_gameState != nullptr) {
		out << "You are already logged in, cannot do that" << std::endl;
		return;
	}

	state.m_gameState = new GameState();
	state.m_world = create_world();

	state.m_level = UserLevel::USER;
	out << "New Session Created" << std::endl;
}

static void promote(std::ostream& out, UserInput& input, SessionState& state) {
	if ( !isUser(state.m_level) || state.m_gameState == nullptr) {
		out << "You need to be logged in to do that..." << std::endl;
		return;
	}
	if (input.args.size() != 1) {
		out << "invalid number of arguments" << std::endl;
		return;
	}

    auto password = input.args[0];

	

	if (AdminTries <= 0) {
		out << "You have entered too many Inccorect passwords please wait:" << std::endl;
		return;
	}

	std::hash<std::string> Hashing;
	std::string salt = "gjsbijfnasljfkbeaf";

	if (Hashing(password + salt) != 13372464928578503471) {
		AdminTries -= 1;
		//out << Hashing("MagicPassword42"+salt) << std::endl;
		out << "Incorrect password" << std::endl;
		return;
	}
    

    state.m_level = UserLevel::ADMIN;
	out << "You are now admin." << std::endl;
}

static void loadGame(std::ostream& out, UserInput& input, SessionState& state) {
	state.m_level = UserLevel::USER;
	state.m_gameState = game_load_state();
	state.m_world = create_world();
}

static void saveGame(std::ostream& out, UserInput& input, SessionState& state) {
	if ( !isUser(state.m_level) || state.m_gameState == nullptr) {
		out << "You need to be logged in to do that..." << std::endl;
		return;
	}

	game_save_state(state.m_gameState);
}



static void playerInv(std::ostream& out, UserInput& input, SessionState& state) {
	if ( !isUser(state.m_level) || state.m_gameState == nullptr) {
		out << "You need to be logged in to do that..." << std::endl;
		return;
	}

	bool oneOrMore = false;
	auto* player = state.m_gameState;

	// copy the location to the location state variable
	out << "You have: " << std::endl;
	for ( const auto item : ITEMS ) {
		auto qty = player->items.at( (int)item );
		if ( qty != 0 ) {
			out << "\t" << get_item_name(item) << " (x" << (int)qty << ")" << std::endl;
			oneOrMore = true;
		}
	}

	if ( !oneOrMore ) {
		out << "\t no items" << std::endl;
	}

	out << "you have " << player->money << " gold" << std::endl; 
}

void setup_callbacks(Callbacks& callbacks) {
	// basic commands
	callbacks["ping"] = pong;
	callbacks["quit"] = quit;
	callbacks["exit"] = quit;
    callbacks["promote"] = promote;

	// state management commands
	callbacks["new"] = newGame;
	callbacks["load"] = loadGame;
	callbacks["save"] = saveGame;
}

void setup_callbacks_world(Callbacks& callbacks) {
	callbacks["links"] = showLinks;
	callbacks["go"] = go;
	callbacks["use"] = use;

	callbacks["inv"] = playerInv;
    callbacks["admTp"] = admTeleport;
}

void setup_callbacks_shop(Callbacks& callbacks) {
    // shop commands
	callbacks["shop"] = shopList;
	callbacks["buy"] = shopBuy;
	callbacks["sell"] = shopSell;

    // admin commands
	callbacks["admPrice"] = adminShopPrice;
	callbacks["admQty"] = adminShopAmount;
}

int main(int argc, char* argv[]) {
	std::cout << "COMP280 Security Assignment" << std::endl;
    std::cout << "Year 2: Bad Adventure Game" << std::endl;

    // add callbacks for commands
	Callbacks callbacks;
	setup_callbacks(callbacks);
    setup_callbacks_world(callbacks);
    setup_callbacks_shop(callbacks);

    // main game loop
	handle_input(std::cin, std::cout, callbacks);
	return EXIT_SUCCESS;
}
