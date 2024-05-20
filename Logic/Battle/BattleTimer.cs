using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using WPP.Protocol;
using WPP.Logic;
using WPP.Logic.Room;
using System.Threading;

namespace WPP.Logic.Battle
{
    internal class BattleTimer
    {
        System.Threading.Timer _gameTimer;
        System.Threading.Timer _overTimer;
        System.Threading.Timer _synchronizationTimer;
        private BattleLogic _battle;

        public BattleTimer(BattleLogic battle)
        {
            _battle = battle;
        }

        public void StartTimer()
        {
            Console.WriteLine("start timer");
            _gameTimer = new System.Threading.Timer(SetOverTimeMode, null, 180000, 180000);
            _synchronizationTimer = new System.Threading.Timer(RequestSynchronization, null, 2000, 5000);
        }

        public void RequestSynchronization(object state = null)
        {
            _battle.gameRoom.SendDataToAll(Server_PacketTagPackages.S_REQUEST_SYNCHRONIZATION);
            Console.WriteLine("sync");
        }

        public void SetOverTimeMode(object state = null)
        {
            Console.WriteLine("over time mode on");
            _gameTimer.Dispose();
            _gameTimer = null;
            _overTimer = new System.Threading.Timer(_battle.EndBattle, null, 120000, 120000);

            _battle.SetOverTimeMode();
            return;
        }

        public void StopTimer()
        {
            Console.WriteLine("stop timer");
            if (_gameTimer != null)
            {
                _gameTimer.Dispose();
                _gameTimer = null;
            }

            if (_overTimer != null)
            {
                _overTimer.Dispose();
                _overTimer = null;
            }

            if (_synchronizationTimer != null)
            {
                _synchronizationTimer.Dispose();
                _synchronizationTimer = null;
            }
            return;
        }
    }
}
