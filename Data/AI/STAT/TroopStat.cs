using System;
using System.Collections.Generic;
using System.Linq;
using System.Security.Cryptography.X509Certificates;
using System.Text;
using System.Threading.Tasks;

namespace WPP.Units
{
    [Serializable]
    class TroopStat : LifeStat
    {
        public float damage;
        public float hit_speed;
        public float move_speed;
        public float rotation_speed;
        public float range;
        public float capture_range;

        public List<CaptureTag> target_tag;

        public TroopStat(int id, string name, int level, float hp)
            : base(id, level, name, hp)
        {
            damage = 0.0f;
            hit_speed = 0.0f;
            move_speed = 0.0f;
            rotation_speed = 0.0f;
            range = 0.0f;
            capture_range = 0.0f;
            target_tag = new List<CaptureTag>();
        }
    }
}
