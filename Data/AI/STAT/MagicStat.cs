using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace WPP.AI.STAT
{
    [Serializable]
    class MagicStat : BaseStat
    {
        public float _range; // 범위
        public float _durationBeforeDestroy; // Task 적용 후 기간

        public MagicStat(int id, int Level, string name, float range, float durationBeforeDestroy) : base(id, Level, name)
        {
            _range = range;
            _durationBeforeDestroy = durationBeforeDestroy;
        }
    }
}
