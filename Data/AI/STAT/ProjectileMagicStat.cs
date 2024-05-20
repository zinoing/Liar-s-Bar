using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace WPP.AI.STAT
{
    [Serializable]
    class ProjectileMagicStat : MagicStat
    {
        // 기본 능력치
        public float _damage; // 데미지
        public float _speed; // Task 적용 전 딜레이

        public ProjectileMagicStat(int id, int Level, string name, float range, float durationBeforeDestroy, float damage, float speed)
            : base(id, Level, name, range, durationBeforeDestroy)
        {
            _damage = damage;
            _speed = speed;
        }
    }
}
