#pragma once

#include "Stdfax.h"
#include "Card.h"

class Player
{
	string id;
	vector<Card> cardsOnHand;
	bool isPlayerTurn;

public:
	Player(string playerId);
	string getId() const;
	void discardCards();
	void setCardsFromDealer(Card card);
	const vector<Card> getCards() const;

	const bool getTurnStatus();
	void setTurnStatus(bool turn);
private:
};

