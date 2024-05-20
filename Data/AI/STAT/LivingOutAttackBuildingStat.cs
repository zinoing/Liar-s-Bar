using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using WPP.AI.STAT;

namespace WPP.AI.STAT
{
    [Serializable]
    class LivingOutAttackBuildingStat : AttackBuildingStat
    {
        public float _lifeTime; // 생존 시간

        public LivingOutAttackBuildingStat(int id, int Level, string name, float hitpoints, List<CaptureTag> targetTag, float damage, float hitSpeed, float range, float captureRange, float lifeTime) 
            : base(id, Level, name, hitpoints, targetTag, damage, hitSpeed, range, captureRange)
        {
            _lifeTime = lifeTime;
        }
    }
}
