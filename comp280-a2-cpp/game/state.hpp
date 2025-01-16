#ifndef COMP280_GAME_STATE_H
#define COMP280_GAME_STATE_H

#include "world.hpp"

#include <cstdint>
#include <array>
#include <cstring>

struct GameState {
	char location[16] = "road";
	uint8_t health = 100;
	uint32_t money = 5000;

    //Potential Bug
	std::array<uint8_t, ITEMS.size()> items = {0};
    //std::array<uint16_t, ITEMS.size()> items = { 0 };

    // state checking functions
    inline bool inLocation(const char* location) {
        return std::strcmp( location,  this->location ) == 0;
    }

    inline bool hasItem(Item item){
        return items.at( (int)item ) != 0;
    }

    inline void removeItem(Item item) {
        items.at((int)item)--;
    }
};


GameState* game_load_state();
void game_save_state(const GameState* state);

#endif
