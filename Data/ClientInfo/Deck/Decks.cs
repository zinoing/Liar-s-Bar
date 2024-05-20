using System;
using System.Collections.Generic;

namespace WPP.Data.ClientInfo.Deck
{
    [Serializable]
    class Decks
    {
        public List<Deck> decks;
        public Decks()
        {
            decks = new List<Deck>();
        }

        public void AddDeck(Deck deck)
        {
            if (decks.Count > Constants.MAXIMUM_DECKS)
                return;

            decks.Add(deck);
        }
    }
}
