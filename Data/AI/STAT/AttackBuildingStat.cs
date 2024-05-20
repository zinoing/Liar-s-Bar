using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace WPP.AI.STAT
{
    [Serializable]
    public class AttackBuildingStat : BuildingStat
    {
        public float _damage; // 데미지
        public List<CaptureTag> _targetTag; // 타켓 태그
        public float _hitSpeed; // 공격 속도
        public float _range; // 범위
        public float _captureRange; // 탐지 범위

        public AttackBuildingStat(int id, int Level, string name, float hp, List<CaptureTag> targetTag, float damage, float hitSpeed, float range, float captureRange)
            : base(id, Level, name, hp)
        {
            _damage = damage;
            _targetTag = targetTag;
            _hitSpeed = hitSpeed;
            _range = range;
            _captureRange = captureRange;
        }
    }
}
