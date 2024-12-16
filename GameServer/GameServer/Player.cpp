#include "Player.h"

Player::Player(string playerId) : id(playerId), isPlayerTurn(false)
{
}

string Player::getId() const
{
	return id;
}

void Player::discardCards()
{
	cardsOnHand.clear();
	return;
}

void Player::setCardsFromDealer(Card card)
{
	cardsOnHand.push_back(card);
	return;
}

const vector<Card> Player::getCards() const
{
	return cardsOnHand;
}

const bool Player::getTurnStatus()
{
	return isPlayerTurn;
}

void Player::setTurnStatus(bool turn)
{
	isPlayerTurn = turn;
}
