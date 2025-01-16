#include "state.hpp"

#include <cstdio>
#include <cassert>

GameState* game_load_state() {
	GameState* state = new GameState();

	FILE* fPtr = fopen("state.bin", "rb");
	fread(state, sizeof(GameState), 1, fPtr);
	fclose(fPtr);

	return state;
}

void game_save_state(const GameState* state) {
	assert(state != nullptr);

	FILE* fPtr = fopen("state.bin", "wb");
	fwrite(state, sizeof(GameState), 1, fPtr);
	fclose(fPtr);
}
