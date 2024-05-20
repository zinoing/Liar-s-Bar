using System;
using System.Collections.Generic;
using System.Linq;
using System.Numerics;
using System.Text;
using System.Threading.Tasks;
using WPP.Data.Collection;
using static System.Net.Mime.MediaTypeNames;

namespace WPP.AI.STAT
{
    [Serializable]
    class LivingOutSpawnBuildingStat : BuildingStat
    {
        public float _lifeTime; // 생존 시간
        public int _spawnUnitId; // 스폰시킬 유닛의 id
        public float _spawnDelay; // 스폰 딜레이
        public int _spawnUnitCount; // 스폰 유닛 개수
        public List<Vector2> _spawnOffset; // 스폰 오프셋

        public LivingOutSpawnBuildingStat(int id, int Level, string name, float hp, float lifeTime, int spawnUnitId, float spawnDelay, int spawnUnitCount, List<Vector2> spawnOffsets)
            : base(id, Level, name, hp)
        {
            _spawnOffset = spawnOffsets;

            _lifeTime = lifeTime;
            _spawnUnitId = spawnUnitId;
            _spawnDelay = spawnDelay;
            _spawnUnitCount = spawnUnitCount;
            _spawnOffset = spawnOffsets;
        }
    }
}
