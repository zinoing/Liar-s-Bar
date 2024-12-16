#include "Player.h"

Player::Player(string playerId) : id(playerId), isPlayerTurn(false), numOfCardsOnHand(MAX_CARDS_ON_HAND)
{
}

string Player::getId()
{
	return id;
}

void Player::setCardsFromDealer(vector<Card> cards)
{
	cardsOnHand = cards;
	return;
}

void Player::setNumOfCards(int numOfCards)
{
    numOfCardsOnHand = numOfCards;
    return;
}

const vector<Card> Player::getCards() const
{
	return cardsOnHand;
}

int Player::getNumOfCardsOnHand()
{
	return numOfCardsOnHand;
}

bool Player::getTurnStatus()
{
	return isPlayerTurn;
}

vector<Card> Player::chooseCardsToDiscard(int remainingTime)
{
    char cardInputToDiscard;
    vector<Card> cardsToDiscard;
    bool choosing = true;

    GameManager::uiMutex.lock();
    gotoxy(0, 12);
    cout << "Type the card type you want to discard.\n";
    cout << "You only can discard " << MAX_LIMIT_DISCARD_CARDS << " cards per turn.\n";
    cout << "If you are done, press 'E'.\n";
    GameManager::uiMutex.unlock();

    while (choosing && cardsToDiscard.size() <= MAX_LIMIT_DISCARD_CARDS && remainingTime > 0) {
        cin >> cardInputToDiscard;

        cardInputToDiscard = toupper(cardInputToDiscard);

        switch (cardInputToDiscard) {
        case 'E':
            choosing = false;
            break;
        case 'C':
            cout << "You chose CLUB to discard";
            cardsToDiscard.push_back(Card(CardType::CLUB));
            Sleep(2000);
            cout << "                             ";
            break;
        case 'S':
            cout << "You chose SPADE to discard";
            cardsToDiscard.push_back(Card(CardType::SPADE));
            Sleep(2000);
            cout << "                             ";
            break;
        case 'H':
            cout << "You chose HEART to discard";
            cardsToDiscard.push_back(Card(CardType::HEART));
            Sleep(2000);
            cout << "                             ";
            break;
        case 'D':
            cout << "You chose DIAMOND to discard";
            cardsToDiscard.push_back(Card(CardType::DIAMOND));
            Sleep(2000);
            cout << "                             ";
            break;
        case 'J':
            cout << "You chose JOKER to discard";
            cardsToDiscard.push_back(Card(CardType::JOKER));
            Sleep(2000);
            cout << "                             ";
            break;
        default:
            cout << "Invalid card type. Please enter a valid card type.";
            Sleep(2000);
            cout << "                                                    ";
            break;
        }
    }

    return cardsToDiscard;
}

void Player::discardCards(vector<Card> cardsToDiscard)
{
    if (cardsToDiscard.empty()) {
        GameManager::uiMutex.lock();
        gotoxy(0, 12);
        cout << "There is nothing to discard." << endl;
        cout << "System will randomly choose a card from yours" << endl;
        GameManager::uiMutex.unlock();

        cardsToDiscard.push_back(cardsOnHand[0]); // discard the first card
        --numOfCardsOnHand;
    }

	if (cardsToDiscard.size() > MAX_LIMIT_DISCARD_CARDS) {
		cout << "You can't discard cards more than " << MAX_LIMIT_DISCARD_CARDS << endl;
		return;
	}

    for (auto& cardToDiscard : cardsToDiscard) {
        auto it = find_if(cardsOnHand.begin(), cardsOnHand.end(),
            [&cardToDiscard](const Card& cardOnHand) {
                return cardToDiscard.getCardType() == cardOnHand.getCardType();
            });

        if (it != cardsOnHand.end()) {
            cardsOnHand.erase(it);
            --numOfCardsOnHand;
        }
    }
    return;
}
