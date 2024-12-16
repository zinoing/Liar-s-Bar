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
	Card(CardType cardType);
	const CardType getCardType() const;
};