#pragma once

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
	Card(CardType card);
	const CardType getCardType();
};