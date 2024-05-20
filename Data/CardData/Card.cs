using WPP.AI.STAT;
using System;
using System.Text.Json.Serialization;

namespace WPP.Data.CardData
{
    public enum CardType
    {
        troop, building, spell
    }
    public enum CardRarity
    {
        common, rare, epic, legendary
    }


    public class Card
    {
        public int id { get; set; }
        public int unitID { get; set; }

        public BaseStat unit { get; set; }

        public CardType type { get; set; }

        public CardRarity rarity { get; set; }

        public int needElixir { get; set; }

        public float duration;

        public float radius;

        public GridSize gridSize { get; set; }
        public SpawnData spawnData { get; set; }

        public Card() 
        {
            id = 0;
            unitID = 0;
            unit = null;
            type = 0;
            rarity = 0;
            needElixir = 0;
            duration = 1.5f;
            radius = 0;
            gridSize = null;
            spawnData = null;
        }

        public Card(int id, int unitID, CardType type, CardRarity rarity, int needElixir, float duration, float radius, GridSize gridSize, SpawnData spawnData)
        {
            this.id = id;
            this.unitID = unitID;
            this.type = type;
            unit = DatabaseManager.Instance.FindUnit(type, unitID);
            this.rarity = rarity;
            this.needElixir = needElixir;
            this.duration = duration;
            this.radius = radius;
            this.gridSize = gridSize;
            this.spawnData = spawnData;
        }
    }
}
