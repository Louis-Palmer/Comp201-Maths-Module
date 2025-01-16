#ifndef COMP280_GAME_WORLD_H
#define COMP280_GAME_WORLD_H

#include <array>
#include <string>
#include <vector>
#include <map>
#include <stdexcept>

enum class Item {
	POTION,
    KEY
};
constexpr const std::array<Item, 2> ITEMS = { Item::POTION, Item::KEY };

std::string get_item_name(Item item);

struct ShopItemState {
	int cost;
	uint32_t amount;
};

class World {
	public:
		using LinkType = std::string;
		bool hasLink( const LinkType& src, const LinkType& dest ) const;
		void addLink( const LinkType& src, const LinkType& dest );

		std::vector<LinkType> getLinksFrom( const LinkType& src ) const;

		inline uint32_t getQty( Item item ) const {
			try {
				return m_shopItems.at(item).amount;
			} catch ( std::out_of_range& e ){
				return 0;
			}
		}

		inline uint32_t getCost( Item item ) const {
			try {
				return m_shopItems.at(item).cost;
			} catch ( std::out_of_range& e ){
				return 0;
			}
		}

		inline void addShopItem( Item item, uint32_t amount, int cost ) {
			m_shopItems[item] = {
				.cost = cost,
				.amount = amount
			};
		}

		inline void alterItemQty( Item item, int amount ) {
			m_shopItems[item].amount += amount;
		}

		inline void alterItemPrice( Item item, int price ) {
			m_shopItems[item].cost += price;
		}

	private:
		std::map< LinkType, std::vector<LinkType> > m_links;
		std::map< Item, ShopItemState > m_shopItems;

};

World* create_world();

#endif
