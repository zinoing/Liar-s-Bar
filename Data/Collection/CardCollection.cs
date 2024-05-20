using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using WPP.Data.CardData;

namespace WPP.Data.Collection
{
    public struct CARD_ID
    {
        // card id
        public const int BABARIANS_ID = 1;
        public const int BATS_ID = BABARIANS_ID + 10;
        public const int DRAGON_ID = BATS_ID + 10;
        public const int GIANT_ID = DRAGON_ID + 10;
        public const int KNIGHT_ID = GIANT_ID + 10;
        public const int MEGA_MINION_ID = KNIGHT_ID + 10;
        public const int SHOOTER_ID = MEGA_MINION_ID + 10;
        public const int WIZARD_ID = SHOOTER_ID + 10;

        public const int CANNON_ID = WIZARD_ID + 10;
        public const int BABARIAN_HUT_ID = CANNON_ID + 10;

        public const int ARROWS_ID = BABARIAN_HUT_ID + 10;

        public static int[] card_ids = { BABARIANS_ID, BATS_ID, DRAGON_ID, GIANT_ID, KNIGHT_ID, MEGA_MINION_ID, SHOOTER_ID, WIZARD_ID, CANNON_ID, BABARIAN_HUT_ID, ARROWS_ID };
    }
    [Serializable]
    class CardCollection
    {

        public List<Card> cardCollection;

        public CardCollection()
        {
            cardCollection = new List<Card>();
        }

        public void AddCard(Card card)
        {
            cardCollection.Add(card);
        }
        public Card FindCard(int cardID)
        {
            foreach (Card card in cardCollection)
            {
                if (card.id == cardID)
                    return card;
            }
            return null;
        }
    }
}
