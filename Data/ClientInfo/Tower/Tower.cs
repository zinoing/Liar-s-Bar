using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using WPP.AI.STAT;

namespace WPP.Data.ClientInfo.Tower
{
    public enum TowerStatus
    {
        NOT_DESTROYED,
        DESTROYED
    }
    [Serializable]
    class Tower
    {
        public TowerStatus status;
        public AttackBuildingStat towerUnit;

        public Tower() {
            status = TowerStatus.NOT_DESTROYED;
            towerUnit = null;
        }

        public Tower(AttackBuildingStat tower)
        {
            status = TowerStatus.NOT_DESTROYED;
            towerUnit = tower;
        }
    }
}
