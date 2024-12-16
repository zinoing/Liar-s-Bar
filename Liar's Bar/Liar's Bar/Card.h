#pragma once

#include "Stdfax.h"
#include "Utility.h"

enum class CardType {
	CLUB,
	SPADE,
	HEART,
	DIAMOND,
	JOKER
};

class Card {
	CardType type;

public:
	Card(CardType cardType);
	const CardType getCardType() const;
	void draw(int x, int y) const;
};