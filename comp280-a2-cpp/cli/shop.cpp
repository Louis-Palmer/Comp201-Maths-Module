#include "shop.hpp"

void shopList(std::ostream& out, UserInput& input, SessionState& state) {
	if ( !is_active(state) ) {
		out << "You need to be logged in to do that..." << std::endl;
		return;
	}

    // only works in the tavern
    if ( !state.m_gameState->inLocation("inn") ) {
		out << "The shop only works in the inn..." << std::endl;
        return;
    }

	// show the current shop contents
	out << "The shop has: " << std::endl;
	for ( const auto item : ITEMS ) {
		auto qty = state.m_world->getQty( item );
		auto cost = state.m_world->getCost( item );
		if ( qty != 0 ) {
			out << "\t" << get_item_name(item) << " ( x" << qty << ") @ " << cost << " gold each" << std::endl;
		}
	}
}

void shopBuy(std::ostream& out, UserInput& input, SessionState& state) {
	if ( !is_active(state) ) {
		out << "You need to be logged in to do that..." << std::endl;
		return;
	}

    // only works in the tavern
    if ( !state.m_gameState->inLocation("inn") ) {
		out << "The shop only works in the inn..." << std::endl;
        return;
    }

	if (input.args.size() != 2) {
		out << "invalid number of arguments" << std::endl;
		return;
	}

	

    // figure out what item and amount the user provided
    int itemAsInt = std::stoi( input.args[0] );
	Item itemToBuy = (Item)itemAsInt;
	int amountToBuy = std::stoi( input.args[1] );

	if (amountToBuy <= 0) {
		out << "you need a positive number." << std::endl;
		return;
	}
	// check the shop has that many
	auto shopQty = state.m_world->getQty( itemToBuy );
	if ( amountToBuy > shopQty ) {
		out << "the shop doesn't have that many..." << std::endl;
		return;
	}

	// check we can afford it
	auto cost = state.m_world->getCost( itemToBuy ) * amountToBuy;
	if ( cost > state.m_gameState->money ) {
		out << "you don't have enough gold..." << std::endl;
		return;
	}
	if (state.m_gameState->items.at((int)itemToBuy) + amountToBuy > 255) {
		out << "you don't have enough inventory space" << std::endl;
		return;
	}

	// adjust the amount
	state.m_world->alterItemQty( itemToBuy, -amountToBuy );
	state.m_gameState->money -= cost;
	state.m_gameState->items[itemAsInt] += amountToBuy;
	out << "You purchase " << amountToBuy << " of " << get_item_name(itemToBuy) << std::endl;
}

void shopSell(std::ostream& out, UserInput& input, SessionState& state) {
	if ( !is_active(state) ) {
		out << "You need to be logged in to do that..." << std::endl;
		return;
	}

    // only works in the tavern
    if ( !state.m_gameState->inLocation("inn") ) {
		out << "The shop only works in the inn..." << std::endl;
        return;
    }

	if (input.args.size() != 2) {
		out << "invalid number of arguments" << std::endl;
		return;
	}

	Item itemToSell = (Item)( std::stoi( input.args[0] ) );
	int amountToSell = std::stoi( input.args[1] );

	// check the shop has that many
	auto shopQty = state.m_gameState->items.at( (int)itemToSell );
	if ( amountToSell > shopQty ) {
		out << "you don't have that many..." << std::endl;
		return;
	}
	//Fix to issue 6
	if (amountToSell <= 0) {
		out << "you need to sell atleast one item" << std::endl;
		return;
	}

	// check we can afford it
	auto goldAmount = state.m_world->getCost( itemToSell ) * amountToSell;
	state.m_gameState->money += goldAmount;
	state.m_gameState->items.at( (int)itemToSell ) -= amountToSell;

	//returns item to the shop
	state.m_world->alterItemQty(itemToSell, amountToSell);

	out << "You sell " << amountToSell << " of " << get_item_name(itemToSell) << std::endl;
}

void adminShopAmount(std::ostream& out, UserInput& input, SessionState& state) {
	if ( !is_active(state) || state.m_level != UserLevel::ADMIN) {
		out << "You need to be logged in to do that..." << std::endl;
		return;
	}
	if (input.args.size() != 2) {
		out << "invalid number of arguments" << std::endl;
		return;
	}
	Item item = (Item)std::stoi( input.args[0] );
	int amount = std::stoi( input.args[1] );

    state.m_world->alterItemQty(item, amount);
	out << "Shop qty altered" << std::endl;
}

void adminShopPrice(std::ostream& out, UserInput& input, SessionState& state) {
	if ( !is_active(state) || state.m_level != UserLevel::ADMIN) {
		out << "You need to be logged in to do that..." << std::endl;
		return;
	}
	if (input.args.size() != 2) {
		out << "invalid number of arguments" << std::endl;
		return;
	}
	Item item = (Item)std::stoi( input.args[0] );
	int price = std::stoi( input.args[1] );

    state.m_world->alterItemPrice(item, price);
	out << "Shop price altered" << std::endl;
}

