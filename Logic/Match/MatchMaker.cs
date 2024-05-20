using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Threading;
using WPP.Protocol.Server;
using WPP.Protocol.Client;
using WPP.Logic.Room;
using System.Reflection;
using WPP.LogData;

namespace WPP.Logic.Match
{
    class MatchMaker : Singleton<MatchMaker>
    {
        private Thread _matchThread;

        public void InitializeMatchThread()
        {
            _matchThread = new Thread(MatchLoop);
            _matchThread.Name = "matchThread";
            _matchThread.IsBackground = false;
            _matchThread.Start();
        }

       
        private void MatchLoop()
        {
            int clientIndextToMatch = 0;
            while (true)
            {
                lock (ServerTCP.Instance.WaitingClientsLockObj)
                {
                    if (clientIndextToMatch >= ServerTCP.Instance.WaitingClients.Count)
                    {
                        continue;
                    }

                    if (ServerTCP.Instance.WaitingClients.Count > 1)
                    {
                        // reference sites
                        // https://www.reddit.com/r/ClashRoyale/comments/rvghl1/Explaining_the_matchmaking/
                        // https://supercell.com/en/games/clashroyale/blog/news/matchmaking-changes/

                        ClientObject clnt1 = ServerTCP.Instance.WaitingClients[clientIndextToMatch];
                        int opponentIndex = FindOptimalOpponentIndex(clnt1, clientIndextToMatch);
                        if (opponentIndex == -1)
                        {
                            // couldn't find appropriate opponent
                            ++clientIndextToMatch;
                            continue;
                        }

                        ClientObject clnt2 = ServerTCP.Instance.WaitingClients[opponentIndex];
                        Match(clnt1, clnt2);
                        continue;
                    }
                }
            }
        } 

        private void Match(ClientObject clnt1, ClientObject clnt2)
        {
            ServerTCP.Instance.WaitingClients.Remove(clnt1);
            ServerTCP.Instance.WaitingClients.Remove(clnt2);

            GameRoomManager.Instance.EnterGameRoom(clnt1, clnt2);

            Console.WriteLine("clientID: {0}, trophy: {1} and clientID: {2}, trophy: {3} has matched", clnt1.Tcp.ClientId, clnt1.AccountInfo.Trophy, clnt2.Tcp.ClientId, clnt2.AccountInfo.Trophy);
        }

        private int FindOptimalOpponentIndex(ClientObject client, int clientIndextToMatch)
        {
            int trophy = client.AccountInfo.Trophy;
            int kingTowerLevel = client.Towers.kingTower.towerUnit._level;

            int minTrophy = trophy - 100;
            int maxTrophy = trophy + 100;

            int minKingTowerLevel = kingTowerLevel - 1;
            int maxKingTowerLevel = kingTowerLevel + 1;

            int index = -1;

            while(true)
            {
                ++index;

                // couldn't find a client that meets the conditions
                if (index > ServerTCP.Instance.WaitingClients.Count - 1)
                {
                    if (minKingTowerLevel <= kingTowerLevel - 3 &&
                       maxKingTowerLevel >= kingTowerLevel + 3)
                    {
                        Console.WriteLine("Can't find optimal opponent");
                        return -1;
                    }

                    // change condition
                    --minKingTowerLevel;
                    ++maxKingTowerLevel;
                    index = 0;
                }

                ClientObject clientObj = ServerTCP.Instance.WaitingClients[index];

                if (index == clientIndextToMatch)
                    continue;

                if (clientObj.AccountInfo.Trophy < minTrophy)
                    continue;

                if (clientObj.AccountInfo.Trophy > maxTrophy)
                    continue;

                if (clientObj.Towers.kingTower.towerUnit._level < minKingTowerLevel)
                    continue;

                if (clientObj.Towers.kingTower.towerUnit._level > maxKingTowerLevel)
                    continue;

                // all conditions established
                return index;
            }
        }
    }
}
