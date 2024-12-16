#include "Card.h"

Card::Card(CardType cardType) : type(cardType)
{
}

const CardType Card::getCardType() const
{
	return type;
}

void Card::draw(int x, int y) const
{
    gotoxy(x, y);
    cout << "--------" << endl;

    gotoxy(x, y + 1);
    switch (type) {
    case CardType::CLUB:
        cout << "|     C|" << endl;
        break;
    case CardType::SPADE:
        cout << "|     S|" << endl;
        break;
    case CardType::HEART:
        cout << "|     H|" << endl;
        break;
    case CardType::DIAMOND:
        cout << "|     D|" << endl;
        break;
    case CardType::JOKER:
        cout << "|     J|" << endl;
        break;
    }

    gotoxy(x, y + 2);
    cout << "|      |" << endl;
    gotoxy(x, y + 3);
    cout << "|      |" << endl;

    gotoxy(x, y + 4);
    switch (type) {
    case CardType::CLUB:
        cout << "|C     |" << endl;
        break;
    case CardType::SPADE:
        cout << "|S     |" << endl;
        break;
    case CardType::HEART:
        cout << "|H     |" << endl;
        break;
    case CardType::DIAMOND:
        cout << "|D     |" << endl;
        break;
    case CardType::JOKER:
        cout << "|J     |" << endl;
        break;
    }
    gotoxy(x, y + 5);
    cout << "--------" << endl;
}
