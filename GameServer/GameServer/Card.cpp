#include "Card.h"

Card::Card(CardType cardType) : type(cardType)
{
}

const CardType Card::getCardType() const
{
	return type;
}
