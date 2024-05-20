using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using WPP.Protocol;
using WPP.Protocol.Client;
using WPP.Data.ClientInfo.Tower;
using WPP.Logic.Room;
using WPP.Data;
using MySqlX.XDevAPI;

namespace WPP.Logic.Battle
{
    public enum BattleStatus
    {
        PreBattle,
        Battle,
        Overtime,
        Tiebreaker,
        PostBattle
    }
    class BattleLogic
    {
        public BattleStatus battleStatus;
        public GameRoom gameRoom { get; set; }
        private readonly BattleTimer _timer;
        public ClientObject winner { get; set; }
        public ClientObject loser { get; set; }

        public BattleLogic(GameRoom room) {
            battleStatus = BattleStatus.PreBattle;
            gameRoom = room;
            _timer = new BattleTimer(this);
            winner = null;
            loser = null;
        }

        public void StartBattle()
        {
            battleStatus = BattleStatus.Battle;
            _timer.StartTimer();
        }

        public void DestroyKingTower(int clientID)
        {
            ClientObject client = gameRoom.FindOpponentClient(clientID);
            client.Towers.kingTower.status = TowerStatus.DESTROYED;
            client.Towers.numOfTowersDestroyed++;
            gameRoom.EndGame();
        }

        public void DestroyLeftPrincessTower(int clientID) 
        {
            ClientObject client = gameRoom.FindOpponentClient(clientID);
            client.Towers.numOfTowersDestroyed++;
            client.Towers.leftPrincessTower.status = TowerStatus.DESTROYED;
            if (battleStatus == BattleStatus.Overtime)
            {
                gameRoom.EndGame();
            }
            return;
        }

        public void DestroyRightPrincessTower(int clientID)
        {
            ClientObject client = gameRoom.FindOpponentClient(clientID);
            client.Towers.numOfTowersDestroyed++;
            client.Towers.rightPrincessTower.status = TowerStatus.DESTROYED;
            if (battleStatus == BattleStatus.Overtime)
            {
                gameRoom.EndGame();
            }
            return;
        }

        public void SetOverTimeMode()
        {
            battleStatus = BattleStatus.Overtime;
            gameRoom.SendDataToAll(Server_PacketTagPackages.S_ALERT_OVER_TIME);
        }

        public void EndBattle(object state = null)
        {
            battleStatus = BattleStatus.PostBattle;
            _timer.StopTimer();

            if (gameRoom.Clients.Count == 0)
                return;

            DetermineGameResult();
            if(winner != null && loser != null)
            {
                CalculateScore();
                DatabaseManager.Instance.SetAccount(winner.AccountInfo);
                DatabaseManager.Instance.SetAccount(loser.AccountInfo);
            }
            foreach (ClientObject client in gameRoom.Clients)
            {
                client.Tcp.State = ClientState.CONNECTED;
                client.Towers.kingTower.status = TowerStatus.NOT_DESTROYED;
                client.Towers.leftPrincessTower.status = TowerStatus.NOT_DESTROYED;
                client.Towers.rightPrincessTower.status = TowerStatus.NOT_DESTROYED;
                client.Towers.numOfTowersDestroyed = 0;
            }
        }

        public void DetermineGameResult()
        {
            if (gameRoom.Clients.Count < 2)
            {
                winner = gameRoom.Clients[0];
                loser = null;
                return;
            }

            if (gameRoom.Clients[0].Towers.kingTower.status == TowerStatus.DESTROYED &&
                gameRoom.Clients[1].Towers.kingTower.status == TowerStatus.NOT_DESTROYED)
            {
                loser = gameRoom.Clients[0];
                winner = gameRoom.Clients[1];
                return;
            }
            else if(gameRoom.Clients[1].Towers.kingTower.status == TowerStatus.DESTROYED &&
                    gameRoom.Clients[0].Towers.kingTower.status == TowerStatus.NOT_DESTROYED)
            {
                loser = gameRoom.Clients[1];
                winner = gameRoom.Clients[0];  
                return;
            }

            if (gameRoom.Clients[0].Towers.numOfTowersDestroyed > gameRoom.Clients[1].Towers.numOfTowersDestroyed)
            {
                loser = gameRoom.Clients[0];
                winner = gameRoom.Clients[1];
            }
            else if (gameRoom.Clients[0].Towers.numOfTowersDestroyed < gameRoom.Clients[1].Towers.numOfTowersDestroyed)
            {
                loser = gameRoom.Clients[1];
                winner = gameRoom.Clients[0];
            }
            else // tie
            {
                winner = null;
                loser = null;
            }
            return;
        }

        public void CalculateScore()
        {
            // https://blog.naver.com/wldn154888/221197981397

            int score = (winner.AccountInfo.Trophy - loser.AccountInfo.Trophy) / 12 + 30;

            if (winner != null && loser != null)
            {
                winner.AccountInfo.Trophy += score;
                loser.AccountInfo.Trophy -= score;
                if (loser.AccountInfo.Trophy <= 0)
                    loser.AccountInfo.Trophy = 0;

                winner.AccountInfo.Gold += 20;
            }

            if(winner != null && loser == null)
            {
                winner.AccountInfo.Trophy += score;
                winner.AccountInfo.Gold += 20;
            }

            return;
        }
    }
}
