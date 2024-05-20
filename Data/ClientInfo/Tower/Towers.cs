using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using WPP.Data.ClientInfo;

namespace WPP.Data.ClientInfo.Tower
{
    [Serializable]
    class Towers
    {
        public Tower kingTower;
        public Tower leftPrincessTower;
        public Tower rightPrincessTower;

        public int numOfTowersDestroyed;

        public Towers() {
            kingTower = new Tower();
            leftPrincessTower = new Tower();
            rightPrincessTower = new Tower();

            numOfTowersDestroyed = 0;
        }
    }
}
