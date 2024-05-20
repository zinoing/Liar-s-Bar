using System;
using System.Net;
using System.Collections.Generic;
using System.Linq;
using WPP.Logic.Room;
using WPP.Data;
using WPP.Data.ClientInfo.Tower;
using WPP.Data.ClientInfo.Account;
using WPP.Data.ClientInfo.Deck;
using WPP.Data.CardData;
using Newtonsoft.Json;
using WPP.Protocol.Client;
using WPP.Data.Collection;
using WPP.LogData;

namespace WPP.Protocol.Server
{
    class PacketHandler : Singleton<PacketHandler>
    {
        private delegate void HandleFunc(int clientID, ref ByteBuffer buffer);
        private Dictionary<int, HandleFunc> _packetHandler;
        public Queue<Tuple<int, byte[]>> PacketQueue { get; set; }
        private int _packetLength;

        public PacketHandler()
        {
            _packetHandler = new Dictionary<int, HandleFunc>();
            _packetLength = 0;
            PacketQueue = new Queue<Tuple<int, byte[]>>();
        }
        public void InitializePacketHandler()
        {
            _packetHandler.Add((int)Client_PacketTagPackages.C_REQUEST_INITIAL_DATA, LoadInitialData);
            _packetHandler.Add((int)Client_PacketTagPackages.C_REQUEST_REGISTER_ACCOUNT, HandleRegister);
            _packetHandler.Add((int)Client_PacketTagPackages.C_REQUEST_LOGIN, HandleLogin);
            _packetHandler.Add((int)Client_PacketTagPackages.C_REQUEST_ENTER_ROOM, HandleEnterRoom);

            _packetHandler.Add((int)Client_PacketTagPackages.C_DESTROY_OPPONENT_KING_TOWER, DestroyKingTower);
            _packetHandler.Add((int)Client_PacketTagPackages.C_DESTROY_OPPONENT_LEFT_TOWER, DestroyLeftPrincessTower);
            _packetHandler.Add((int)Client_PacketTagPackages.C_DESTROY_OPPONENT_RIGHT_TOWER, DestroyRightPrincessTower);

            _packetHandler.Add((int)Client_PacketTagPackages.C_REQUEST_HOLE_PUNCHING, HandleHolePunching);
            _packetHandler.Add((int)Client_PacketTagPackages.C_REQUEST_RELAY, RelayBuffer);
            _packetHandler.Add((int)Client_PacketTagPackages.C_CLOSE_CONNECTION, CloseConnection);
            _packetHandler.Add((int)Client_PacketTagPackages.C_ANSWER_PING, GetRoundTripTime);
        }
        public void HandlePacket(Tuple<int, byte[]> packetToHandle)
        {
            if (packetToHandle == null)
                return;

            int clientID = packetToHandle.Item1;
            byte[] packet = packetToHandle.Item2;

            if (packet.Length == 0)
                return;

            if (ServerTCP.Instance.Clients[clientID].Tcp.ClntSock == null)
                return;

            if (ServerTCP.Instance.Clients[clientID].Tcp.Buffer == null)
            {
                ServerTCP.Instance.Clients[clientID].Tcp.Buffer = new ByteBuffer();
            }

            // packet 복사
            ServerTCP.Instance.Clients[clientID].Tcp.Buffer.WriteBytes(packet);

            if (ServerTCP.Instance.Clients[clientID].Tcp.IsSegmented == false)
            {
                _packetLength = ServerTCP.Instance.Clients[clientID].Tcp.Buffer.ReadInteger(true);
            }

            // 처음으로 패킷이 분할되어 왔을 경우
            if (_packetLength > ServerTCP.Instance.Clients[clientID].Tcp.Buffer.Count() + 4 && ServerTCP.Instance.Clients[clientID].Tcp.IsSegmented == false)
            {
                ServerTCP.Instance.Clients[clientID].Tcp.IsSegmented = true;
                return;
            }

            if (ServerTCP.Instance.Clients[clientID].Tcp.IsSegmented == true)
            {
                // 패킷을 다 받았다면
                if (_packetLength == ServerTCP.Instance.Clients[clientID].Tcp.Buffer.Count() + 4)
                {
                    ServerTCP.Instance.Clients[clientID].Tcp.IsSegmented = false;
                    HandleData(clientID, ServerTCP.Instance.Clients[clientID].Tcp.Buffer.ToArray());
                    _packetLength = 0;
                    ServerTCP.Instance.Clients[clientID].Tcp.Buffer = null;
                }
            }
            else
            {
                HandleData(clientID, ServerTCP.Instance.Clients[clientID].Tcp.Buffer.ToArray());
                _packetLength = 0;
                ServerTCP.Instance.Clients[clientID].Tcp.Buffer = null;
            }
            return;
        }

        public void HandleData(int clientID, byte[] data)
        {
            if (ServerTCP.Instance.Clients[clientID].Tcp.ClntSock == null)
                return;

            ByteBuffer buffer = new ByteBuffer();
            buffer.WriteBytes(data);
            // skip size
            buffer.ReadInteger(true);
            int packetTag = buffer.ReadInteger(true);

            if (_packetHandler.TryGetValue(packetTag, out HandleFunc packet))
            {
                Logger.Instance.WriteLog(LogLevel.INFO, ServerTCP.Instance.Clients[clientID].Tcp.ClntSock.RemoteEndPoint.ToString(), "Handle " + (Client_PacketTagPackages)packetTag);
                Console.WriteLine("Handle " + (Client_PacketTagPackages)packetTag);

                packet.Invoke(clientID, ref buffer);
            }
            else
            {
                Logger.Instance.WriteLog(LogLevel.DEBUG, ServerTCP.Instance.Clients[clientID].Tcp.ClntSock.RemoteEndPoint.ToString(), "Couldn't find tag num");
                Console.WriteLine($"Couldn't find tag number {packetTag}");
                return;
            }
        }

        public void LoadInitialData(int clientID, ref ByteBuffer buffer)
        {
            // Collection 정보 JSON화
            // 모든 카드 정보를 넣기엔 너무 많기 때문에
            // 클라이언트에 별도의 데이터 시트를 먼저 만든 다음
            // 업데이트 된 부분만 별도 적용하는 방법도 생각중

            CardCollection cardCollection = DatabaseManager.Instance.LoadCardCollection();
            var settings = new JsonSerializerSettings
            {
                TypeNameHandling = TypeNameHandling.Auto
            };
            string cardCollectionString = JsonConvert.SerializeObject(cardCollection, settings);
            ByteBuffer initialDataBuffer = new ByteBuffer();
            initialDataBuffer.WriteString(cardCollectionString);
            ServerTCP.Instance.SendDataTo(Server_PacketTagPackages.S_LOAD_CARD_COLLECTION, clientID, initialDataBuffer.ToArray());

        }

        public void HandleRegister(int clientID, ref ByteBuffer buffer)
        {
            string username = buffer.ReadString(true);

            if (DatabaseManager.Instance.CheckUsernameExists(username))
            {
                Logger.Instance.WriteLog(LogLevel.INFO, ServerTCP.Instance.Clients[clientID].Tcp.ClntSock.RemoteEndPoint.ToString(), "There is already player who has same username '" + username + "'");
                Console.WriteLine("There is already player who has same username '" + username + "'");
                ServerTCP.Instance.SendDataTo(Server_PacketTagPackages.S_REJECT_REGISTER_ACCOUNT, clientID);
                return;
            }

            string Password = buffer.ReadString(true);
            ServerTCP.Instance.SendDataTo(Server_PacketTagPackages.S_ACCEPT_REGISTER_ACCOUNT, clientID);
            DatabaseManager.Instance.AddAccount(username, Password);
            DatabaseManager.Instance.AddTowers(username);
            DatabaseManager.Instance.AddCards(username);
            DatabaseManager.Instance.AddDecks(username);
        }

        public void HandleLogin(int clientID, ref ByteBuffer buffer)
        {
            string username = buffer.ReadString(true);
            string password = buffer.ReadString(true);

            if (DatabaseManager.Instance.CheckAccountExists(username, password))
            {
                ClientAccount account = DatabaseManager.Instance.LoadAccount(clientID, username);
                Towers towers = DatabaseManager.Instance.LoadTowers(clientID, username);
                Decks decks = DatabaseManager.Instance.LoadDecks(clientID, username);
                Cards cards = DatabaseManager.Instance.LoadCardInstances(clientID, username);
                ByteBuffer loginBuffer = new ByteBuffer();

                var settings = new JsonSerializerSettings
                {
                    TypeNameHandling = TypeNameHandling.Auto
                };

                // serialize client account information
                string accountString = JsonConvert.SerializeObject(account);
                loginBuffer.WriteString(accountString);

                // serialize client towers information
                string towersString = JsonConvert.SerializeObject(towers, settings);
                loginBuffer.WriteString(towersString);

                // serialize client decks information
                string decksString = JsonConvert.SerializeObject(decks, settings);
                loginBuffer.WriteString(decksString);

                // serialize client card instances information
                string cardInstancesString = JsonConvert.SerializeObject(cards, settings);
                loginBuffer.WriteString(cardInstancesString);

                ServerTCP.Instance.SendDataTo(Server_PacketTagPackages.S_ACCEPT_LOGIN, clientID, loginBuffer.ToArray());

                Logger.Instance.WriteLog(LogLevel.INFO, ServerTCP.Instance.Clients[clientID].Tcp.ClntSock.RemoteEndPoint.ToString(), "[username '" + username + "'] logged in");
                Console.WriteLine("[username '" + username + "'] logged in");
                return;
            }
            ServerTCP.Instance.SendDataTo(Server_PacketTagPackages.S_REJECT_LOGIN, clientID);
            return;
        }
        
        public void HandleHolePunching(int clientID, ref ByteBuffer buffer)
        {
            ByteBuffer byteBuffer = new ByteBuffer();
            
            byteBuffer.WriteEndPoint(ServerTCP.Instance.Clients[clientID].Tcp.ClntSock.RemoteEndPoint as IPEndPoint);
            ServerTCP.Instance.SendDataTo(Server_PacketTagPackages.S_REQUEST_HOLE_PUNCHING, clientID, byteBuffer.ToArray());
        }

        public void RelayBuffer(int clientID, ref ByteBuffer buffer)
        {
            int roomID = buffer.ReadInteger(true);
            // skip size
            buffer.ReadInteger(true);
            int packetTag = buffer.ReadInteger(true);
            int remainingDataLength = buffer.Count();
            byte[] dataToRelay = buffer.ReadBytes(remainingDataLength, true);

            ClientObject opponentClient = GameRoomManager.Instance.FindGameRoom(roomID).FindOpponentClient(clientID);
            if (opponentClient == null)
                return;
            int opponentID = opponentClient.Tcp.ClientId;

            ServerTCP.Instance.SendDataTo((Server_PacketTagPackages)packetTag, opponentID, dataToRelay);
        }

        public void HandleEnterRoom(int clientID, ref ByteBuffer buffer)
        {
            ServerTCP.Instance.Clients[clientID].Tcp.PrivateEP = buffer.ReadEndPoint(true);
            ServerTCP.Instance.Clients[clientID].Tcp.PublicEP = buffer.ReadEndPoint(true);

            Console.WriteLine($"{ServerTCP.Instance.Clients[clientID].AccountInfo.Username}'s private ep: {ServerTCP.Instance.Clients[clientID].Tcp.PrivateEP}");
            Console.WriteLine($"{ServerTCP.Instance.Clients[clientID].AccountInfo.Username}'s public ep: {ServerTCP.Instance.Clients[clientID].Tcp.PublicEP}");

            lock (ServerTCP.Instance.WaitingClientsLockObj)
            {
                // WaitingClients에 추가하면 후에 MatchingThread에서 처리한다.
                ServerTCP.Instance.WaitingClients.Add(ServerTCP.Instance.Clients[clientID]);
                ServerTCP.Instance.WaitingClients.OrderByDescending(p => p.AccountInfo.Trophy).ThenBy(p => p.Towers.kingTower.towerUnit._id);
            }

            ServerTCP.Instance.SendPing(clientID);
        }

        public void DestroyKingTower(int clientID, ref ByteBuffer buffer)
        {
            int roomID = buffer.ReadInteger(true);
            GameRoom room = GameRoomManager.Instance.FindGameRoom(roomID);
            room.Battle.DestroyKingTower(clientID);
            return;
        }

        public void DestroyLeftPrincessTower(int clientID, ref ByteBuffer buffer)
        {
            int roomID = buffer.ReadInteger(true);
            GameRoom room = GameRoomManager.Instance.FindGameRoom(roomID);
            room.Battle.DestroyLeftPrincessTower(clientID);
            return;
        }

        public void DestroyRightPrincessTower(int clientID, ref ByteBuffer buffer)
        {
            int roomID = buffer.ReadInteger(true);
            GameRoom room = GameRoomManager.Instance.FindGameRoom(roomID);
            room.Battle.DestroyRightPrincessTower(clientID);
            return;
        }

        public void CloseConnection(int clientID, ref ByteBuffer buffer)
        {
            string username = ServerTCP.Instance.Clients[clientID].AccountInfo.Username;
            string decksString = buffer.ReadString(true);
            Decks decks = JsonConvert.DeserializeObject<Decks>(decksString);
            DatabaseManager.Instance.SetDecks(username, decks);
            ServerTCP.Instance.RemoveClient(clientID);
        }

        public void GetRoundTripTime(int clientID, ref ByteBuffer buffer)
        {
            ClientTCP clientTCP = ServerTCP.Instance.Clients[clientID].Tcp;
            clientTCP.PingAnsweredTime = DateTime.Now;
            clientTCP.Rtt = clientTCP.PingAnsweredTime.Subtract(clientTCP.PingSentTime);
            double rttMilliseconds = clientTCP.Rtt.TotalMilliseconds;

            Console.WriteLine("Round-Trip Time: " + rttMilliseconds + " milliseconds");
        }
    }
}
