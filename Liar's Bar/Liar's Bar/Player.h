#pragma once

#define MAX_CARDS_ON_HAND 5

#include "Stdfax.h"
#include "GameManager.h"
#include "Card.h"

class Player
{
	string id;

	vector<Card> cardsOnHand; // This stores cards for this player only (not for others)
	int numOfCardsOnHand;

	bool isPlayerTurn;

public:
	Player(string playerId);
	string getId();

	void setCardsFromDealer(vector<Card> cards);
	void setNumOfCards(int numOfCards);
	const vector<Card> getCards() const;

	int getNumOfCardsOnHand();

	bool getTurnStatus();

	vector<Card> chooseCardsToDiscard(int remainingTime);
	void discardCards(vector<Card> cardsToDiscard);
private:
};

