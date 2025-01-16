#ifndef COMP280_STATE_H
#define COMP280_STATE_H

#include "game/state.hpp"
#include "game/world.hpp"

enum class UserLevel {
	GUEST, USER, ADMIN
};

struct SessionState {
	bool m_running = true;
	UserLevel m_level = UserLevel::GUEST;
	GameState* m_gameState = nullptr;
	World* m_world = nullptr;
};


inline bool isAdmin(UserLevel level) {
	return level == UserLevel::ADMIN;
}

inline bool isUser(UserLevel level) {
	return level == UserLevel::USER || level == UserLevel::ADMIN;
}

#endif // COMP280_STATE_H
