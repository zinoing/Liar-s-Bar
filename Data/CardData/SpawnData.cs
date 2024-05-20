using System;
using System.Collections.Generic;
using System.Linq;
using System.Numerics;
using System.Text;
using System.Threading.Tasks;

namespace WPP.Data.CardData
{
    public enum SpawnType { 
        single_spawn,
        triple_spawn
    }

    [Serializable]
    public struct SerializableVector2
    {
        public float _x, _y;

        public SerializableVector2(float x, float y)
        {
            _x = x;
            _y = y;
        }

        public Vector2 ConvertToV2() { return new Vector2(_x, _y); }
    }

    [Serializable]
    public class SpawnData
    {
        public SpawnType type;
        public int spawnUnitCount;
        public SerializableVector2[] spawnOffset;

        public SpawnData(int spawnUnitCount)
        {
            type = 0;
            this.spawnUnitCount = spawnUnitCount;
            spawnOffset = new SerializableVector2[spawnUnitCount];
        }
    }
}
