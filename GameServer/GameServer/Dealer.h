#pragma once

#include <algorithm>

#define MAX_CARDS_ON_HAND 5

#include "Stdfax.h"
#include "Player.h"
#include "Card.h"

class Dealer
{
	vector<Card> deck;

public:
	Dealer();
	void dealCards(vector<Player*> players);

private:
	void initDeck();
	void shuffleDeck();
};

