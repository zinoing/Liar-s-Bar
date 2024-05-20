using System;
using System.Collections.Generic;
using System.Linq;
using System.Net.Sockets;
using WPP.Protocol.Server;
using WPP.LogData;
using WPP;
using System.Net;
using MySqlX.XDevAPI;
using Org.BouncyCastle.Bcpg;
using System.ComponentModel;
using System.Diagnostics;

namespace WPP.Protocol.Client
{
    public enum ClientState
    {
        CONNECTED,
        DISCONNECTED
    }
    class ClientTCP
    {
        private NetworkService _networkService;
        public ClientState State { get; set; }
        public Socket ClntSock { get; set; }
        public ByteBuffer Buffer { get; set; }
        public int ClientId { get; set; }
        public IPEndPoint PublicEP { get; set; }
        public IPEndPoint PrivateEP { get; set; }
        public SocketAsyncEventArgs ReceiveEventArgs { get; private set; }
        byte[] _receiveBuffer;

        public SocketAsyncEventArgs SendEventArgs { get; private set; }
        byte[] _sendBuffer;

        public bool IsSegmented { get; set; }

        public Queue<byte[]> SendQueue = new Queue<byte[]>();
        public object SendQueueLock { get; set; }

        // variables about ping
        public DateTime PingSentTime { get; set; }
        public DateTime PingAnsweredTime { get; set; }
        public TimeSpan Rtt { get; set; }


        public ClientTCP(Socket socket, int id)
        {
            if (socket != null)
            {
                // set sock option
                ClntSock = socket;
                ClntSock.NoDelay = true;
            }

            _networkService = new NetworkService();

            State = ClientState.DISCONNECTED;

            Buffer = null;
            ClientId = id;

            PublicEP = null;
            PrivateEP = null;

            InitializeSocketAsyncEventArgs();

            IsSegmented = false;
            SendQueueLock = new object();
        }
        
        public void SetConnection(Socket acceptedClntSock)
        {
            // set connectable socket
            ClntSock = acceptedClntSock;
            ClntSock.NoDelay = true;

            State = ClientState.CONNECTED;
        }

        public void InitializeSocketAsyncEventArgs()
        {
            ReceiveEventArgs = new SocketAsyncEventArgs();
            ReceiveEventArgs.UserToken = this;
            ReceiveEventArgs.Completed += new EventHandler<SocketAsyncEventArgs>(_networkService.ReceiveCompleted);
            _receiveBuffer = new byte[Constants.MAXIMUM_BUFFER_SIZE];
            ReceiveEventArgs.SetBuffer(_receiveBuffer, 0, Constants.MAXIMUM_BUFFER_SIZE);

            SendEventArgs = new SocketAsyncEventArgs();
            SendEventArgs.UserToken = this;
            SendEventArgs.Completed += new EventHandler<SocketAsyncEventArgs>(_networkService.OnSendCompleted);
            //_sendBuffer = new byte[Constants.MAXIMUM_BUFFER_SIZE];
            //SendEventArgs.SetBuffer(_sendBuffer, 0, Constants.MAXIMUM_BUFFER_SIZE);
        }

        public void CloseConnection()
        {
            if (ClntSock == null)
                return;

            try
            {
                Logger.Instance.WriteLog(LogLevel.INFO, ClntSock.RemoteEndPoint.ToString(), "closed connection");
                Console.WriteLine($"[{ClntSock.RemoteEndPoint}] closed connection");
                ClntSock.Close();
                ClntSock = null;
                return;
            }
            catch(ObjectDisposedException e)
            {
                Logger.Instance.WriteLog(LogLevel.ERROR, "server", e.Message);
                Console.WriteLine("Object disposed exception occurred: " + e.Message);
            }
        }
    }
}
