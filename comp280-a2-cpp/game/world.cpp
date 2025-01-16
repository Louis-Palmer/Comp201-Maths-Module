#include "world.hpp"
#include <algorithm>
#include <cstring>

bool World::hasLink( const World::LinkType& src, const World::LinkType& dest ) const {
	auto& v = m_links.at(src);
	return (std::find(v.begin(), v.end(), dest) != v.end());
}

std::vector<World::LinkType> World::getLinksFrom( const World::LinkType& src ) const {
	return m_links.at( src );
}

void World::addLink( const World::LinkType& src, const World::LinkType& dest ) {
	auto& nodeLinks = m_links[src];
	nodeLinks.push_back( dest );

	auto& destLinks = m_links[dest];
}

std::string get_item_name(Item item) {
    std::string name;

    if ( item == Item::POTION ) {
        name = "Potion";
    } else if ( item == Item::KEY ) {
        name = "Key";
    } else {
        name = "Unknown";
    }

    return std::to_string( (int)item ) + ": " + name;
}

World* create_world() {
	World* world = new World();

    // inn <-> road
	world->addLink("road", "inn");
	world->addLink("inn", "road");

    // road <-> gate
	world->addLink("road", "gate");
	world->addLink("gate", "road");

    // castle -> places
	world->addLink("castle", "gate" );
	world->addLink("castle", "dungeon" );

	world->addShopItem( Item::POTION, 100, 50 );
	world->addShopItem( Item::KEY, 1, 100 );

	return world;
}

