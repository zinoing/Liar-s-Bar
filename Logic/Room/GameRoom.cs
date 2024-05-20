using MySql.Data.MySqlClient.Authentication;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using WPP.Protocol.Server;
using WPP.Protocol.Client;
using WPP.Protocol;
using WPP.Logic.Battle;
using WPP.Data.ClientInfo.Tower;
using System.Net;
using System.Diagnostics;

namespace WPP.Logic.Room
{
    public enum GameRoomStatus
    {
        ACTIVE,
        INACTIVE
    }

    class GameRoom
    {
        public int Id { get; set; }

        public List<ClientObject> Clients { get; set; }
        public GameRoomStatus Status { get; set; }
        public BattleLogic Battle { get; set; }
        public GameRoom(int id) {
            Id = id;
            Clients = new List<ClientObject>();
            Status = GameRoomStatus.ACTIVE;
            Battle = new BattleLogic(this);
        }

        public bool CheckFull()
        {
            return Clients.Count == 2;
        }

        public void AddClient(ClientObject client)
        {
            client.GameRoomId = Id;
            if (Clients.Count == 1)
                Status = GameRoomStatus.ACTIVE;
            Clients.Add(client);
            Console.WriteLine("[ Player '" + client.AccountInfo.Username + "' ] entered room");
        }

        public void RemoveClient(ClientObject client)
        {
            if (!Clients.Contains(client)) return;

            Clients.Remove(client);
            Console.WriteLine("[ Player '" + client.AccountInfo.Username + "' ] exited room");

            if(Clients.Count == 0)
            {
                EndGame();
            }
        }

        public ClientObject FindClient(int clientId)
        {
            if (Clients.Count < 2)
                return null;

            if (Clients[0].Tcp.ClientId == clientId)
                return Clients[0];
            return Clients[1];
        }

        public ClientObject FindOpponentClient(int clientId)
        {
            if (Clients.Count < 2)
                return null;

            if (Clients[0].Tcp.ClientId != clientId)
                return Clients[0];

            return Clients[1];
        }

        public void SendDataToAll(Server_PacketTagPackages tag, byte[] data = null)
        {
            foreach(ClientObject client in Clients)
            {
                ServerTCP.Instance.SendDataTo(tag, client.Tcp.ClientId, data);
            }
            return;
        }

        public void StartGame()
        {
            // send roomID, opponent's ip address and playerID
            ByteBuffer buffer1 = new ByteBuffer();
            ByteBuffer buffer2 = new ByteBuffer();

            IPEndPoint opponentPrivateEP = Clients[1].Tcp.PrivateEP;
            IPEndPoint opponentPublicEP = Clients[1].Tcp.PublicEP;
            // add room id
            buffer1.WriteInteger(Id);
            // add private ip
            if (opponentPrivateEP != null)
                buffer1.WriteEndPoint(opponentPrivateEP);
            // add public ip
            if (opponentPublicEP != null)
                buffer1.WriteEndPoint(opponentPublicEP);
            // add player id
            buffer1.WriteInteger(1);
            // add start time
            TimeSpan rttHalf = TimeSpan.FromTicks(Clients[1].Tcp.Rtt.Ticks / 2);
            buffer1.WriteDateTime(DateTime.Now + rttHalf + TimeSpan.FromSeconds(1));

            ServerTCP.Instance.SendDataTo(Server_PacketTagPackages.S_REQUEST_PLAY_GAME, Clients[0].Tcp.ClientId, buffer1.ToArray());

            opponentPrivateEP = Clients[0].Tcp.PrivateEP;
            opponentPublicEP = Clients[0].Tcp.PublicEP;
            buffer2.WriteInteger(Id);
            if (opponentPrivateEP != null)
                buffer2.WriteEndPoint(opponentPrivateEP);
            if (opponentPublicEP != null)
                buffer2.WriteEndPoint(opponentPublicEP);
            buffer2.WriteInteger(0);
            rttHalf = TimeSpan.FromTicks(Clients[0].Tcp.Rtt.Ticks / 2);
            buffer2.WriteDateTime(DateTime.Now + rttHalf + TimeSpan.FromSeconds(1));
            ServerTCP.Instance.SendDataTo(Server_PacketTagPackages.S_REQUEST_PLAY_GAME, Clients[1].Tcp.ClientId, buffer2.ToArray());

            Battle.StartBattle();
        }

        public void EndGame()
        {
            Battle.EndBattle();
            Status = GameRoomStatus.INACTIVE;
            SendDataToAll(Server_PacketTagPackages.S_REQUEST_END_GAME);
            Console.WriteLine("[ Room '" + Id + "' ] closed");
        }

    }
}
