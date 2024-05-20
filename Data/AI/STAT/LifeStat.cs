using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace WPP.AI.STAT
{
    [Serializable]
    public class LifeStat : BaseStat
    {
        public float _hp; // 체력

        public LifeStat(int id, int level, string name, float hp)
            : base(id, level, name)
        {
            _hp = hp;
        }
    }
}
