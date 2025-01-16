#ifndef COMP280_CLI_SHOP_H
#define COMP280_CLI_SHOP_H

#include "../commands.hpp"

#include <ostream>
#include <string>

void shopList(std::ostream& out, UserInput& input, SessionState& state);
void shopBuy(std::ostream& out, UserInput& input, SessionState& state);
void shopSell(std::ostream& out, UserInput& input, SessionState& state);

//
// ADMIN ONLY COMMANDS
//
void adminShopAmount(std::ostream& out, UserInput& input, SessionState& state);
void adminShopPrice(std::ostream& out, UserInput& input, SessionState& state);

#endif
