using System;
using System.Collections.Generic;

namespace WPP.Data.CardData
{
    [Serializable]
    public class Cards
    {
        public List<Card> cards;
        public Cards()
        {
            cards = new List<Card>();
        }

        public void AddCard(Card card)
        {
            if (cards.Count > Constants.MAXIMUM_CARDS)
                return;

            cards.Add(card);
        }
    }
}