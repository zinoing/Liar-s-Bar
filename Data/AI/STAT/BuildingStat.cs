using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using static Org.BouncyCastle.Asn1.Cmp.Challenge;
using static System.Net.Mime.MediaTypeNames;

namespace WPP.AI.STAT
{
    public struct OffsetRect
    {
        public int _up;
        public int _down;
        public int _left;
        public int _right;

        public OffsetRect NormalFillOffset()
        {
            _up = 1;
            _down = 1;
            _left = 1;
            _right = 1;
            return this;
        }

        public OffsetRect KingTowerFillOffset()
        {
            _up = 1;
            _down = 2;
            _left = 2;
            _right = 1;
            return this;
        }
    }

    [Serializable]

    public class BuildingStat : LifeStat
    {
        public OffsetRect _fillOffset;

        public BuildingStat(int id, int Level, string name, float hp)
            : base(id, Level, name, hp)
        {
            if (String.Equals("king_tower", name))
            {
                _fillOffset = new OffsetRect().KingTowerFillOffset();
            }
            else
            {
                _fillOffset = new OffsetRect().NormalFillOffset();
            };
        }
    }
}
