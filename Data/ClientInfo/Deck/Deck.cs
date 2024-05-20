using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using WPP.Data.CardData;

namespace WPP.Data.ClientInfo.Deck
{
    [Serializable]
    class Deck
    {
        public int id;
        public List<Card> cards;

        public Deck(int id)
        {
            this.id = id;
            cards = new List<Card>();
        }

        public void AddCard(Card card)
        {
            if (cards.Count > Constants.MAXIMUM_CARDS_IN_DECK)
                return;

            cards.Add(card);
        }
    }
}
