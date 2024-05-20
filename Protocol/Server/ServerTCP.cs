using System;
using System.Collections;
using System.IO;
using System.Net;
using System.Net.Sockets;
using System.Collections.Generic;
using WPP.Protocol.Client;
using WPP.LogData;
using WPP.Logic.Room;
using Org.BouncyCastle.Asn1.X500;
using System.Threading;
using MySqlX.XDevAPI;
using System.Diagnostics;

namespace WPP.Protocol.Server
{
    internal class ServerTCP : Singleton<ServerTCP>
    {
        private NetworkService _networkService;

        private Socket _listener;

        private bool _isStarted;
        private Thread _acceptThread;

        public ClientObject[] Clients { get; set; }

        public List<ClientObject> WaitingClients { get; set; }
        public object WaitingClientsLockObj { get; }

        public ServerTCP()
        {
            _networkService = new NetworkService();

            _isStarted = false;
            _acceptThread = null;

            WaitingClients = new List<ClientObject>();
            WaitingClientsLockObj = new object();
        }

        public void InitializeServer()
        {
            InitializeListener();
            InitializeClients();

            _isStarted = true;
            _acceptThread = new Thread(Run);
            _acceptThread.Start();
        }

        private void InitializeListener()
        {
            _listener = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
            _listener.Bind(new IPEndPoint(IPAddress.Parse(Constants.SERVER_IP), Constants.SERVER_PORT));
            _listener.Listen(100);
        }

        private void InitializeClients()
        {
            Clients = new ClientObject[Constants.MAXIMUM_PLAYERS];
            for(int i=0; i< Constants.MAXIMUM_PLAYERS; i++)
            {
                Clients[i] = new ClientObject(null, i);
            }
            return;
        }

        public void Run()
        {
            while (_isStarted)
            {
                try
                {
                    // "accept" is executed synchronously within a single thread.
                    Socket acceptedClntSock = _listener.Accept();

                    for (int i = 0; i < Constants.MAXIMUM_PLAYERS; i++)
                    {
                        if (Clients[i].Tcp.ClntSock == null)
                        {
                            Logger.Instance.WriteLog(LogLevel.INFO, acceptedClntSock.RemoteEndPoint.ToString(), "connected");
                            Console.WriteLine($"[{Clients[i].Tcp.ClientId}] [{acceptedClntSock.RemoteEndPoint}] connected");

                            Clients[i].Tcp.SetConnection(acceptedClntSock);
                            _networkService.BeginReceive(Clients[i].Tcp.ReceiveEventArgs);
                            break;
                        }
                    }
                }
                catch (SocketException e)
                {
                    Logger.Instance.WriteLog(LogLevel.ERROR, "server", e.ToString());
                    Console.WriteLine(e.ToString());
                }
            }
        }

        public void SendDataTo(Server_PacketTagPackages tag, int clientId, byte[] data = null)
        {
            try
            {
                ByteBuffer buffer = new ByteBuffer();
                // write packet length
                if (data != null)
                    buffer.WriteInteger(data.Length + 8);
                else
                    buffer.WriteInteger(8);

                // write packet tag
                buffer.WriteInteger((int)tag);

                // write data if exists
                if (data != null)
                    buffer.WriteBytes(data);

                if (Clients[clientId].Tcp.State == ClientState.CONNECTED)
                {
                    _networkService.Send(Clients[clientId].Tcp.SendEventArgs, buffer.ToArray());
                }
            }
            catch (Exception e)
            {
                Logger.Instance.WriteLog(LogLevel.ERROR, "server", e.Message);
                Console.WriteLine(e.Message);
            }
        }

        public void SendPing(int clientId)
        {
            ClientTCP clientTCP = Clients[clientId].Tcp;
            clientTCP.PingSentTime = DateTime.Now;
            SendDataTo(Server_PacketTagPackages.S_SEND_PING, clientId);
        }

        public void RemoveClient(int clientId)
        {
            // find the client to remove
            ClientObject client = Clients[clientId];

            if (client == null)
                return;

            // check if the client is currently in a game room
            GameRoom room = GameRoomManager.Instance.FindGameRoom(client.GameRoomId);
            if(room != null)
            {
                // If the client is in a room, remove it from the room
                room.RemoveClient(client);
            }

            client.Tcp.CloseConnection();
        }
    }
}
