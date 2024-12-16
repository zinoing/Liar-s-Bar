#include "Dealer.h"

Dealer::Dealer()
{
	initDeck();
}

void Dealer::dealCards(vector<Player*> players)
{
	shuffleDeck();

	int numOfPlayers = players.size();

	for (int i = 0; i < numOfPlayers; ++i) {
		Player* player = players[i];
		player->discardCards();

		for (int j = i * MAX_CARDS_ON_HAND; j < (i + 1) * MAX_CARDS_ON_HAND; ++j) {
			player->setCardsFromDealer(deck[j]);
		}
	}
}

void Dealer::initDeck()
{
	deck.clear();

	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 6; ++j) {
			deck.push_back(Card(CardType(i)));
		}
	}

	deck.push_back(Card(CardType::JOKER));
	deck.push_back(Card(CardType::JOKER));

	return;
}

void Dealer::shuffleDeck()
{
	random_shuffle(deck.begin(), deck.end());
	return;
}
