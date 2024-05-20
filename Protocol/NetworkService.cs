using System;
using System.Collections.Generic;
using System.Linq;
using System.Net.Sockets;
using System.Text;
using System.Threading.Tasks;
using WPP.LogData;
using WPP.Protocol.Client;
using WPP.Protocol.Server;

namespace WPP.Protocol
{
    public class NetworkService
    {
        public void BeginReceive(SocketAsyncEventArgs args)
        {
            ClientTCP client = args.UserToken as ClientTCP;

            if (client == null)
                return;

            bool pending = client.ClntSock.ReceiveAsync(client.ReceiveEventArgs);
            if (!pending)
            {
                ReceiveCompleted(null, client.ReceiveEventArgs);
            }
        }

        public void ReceiveCompleted(object sender, SocketAsyncEventArgs args)
        {
            if (args.LastOperation == SocketAsyncOperation.Receive)
            {
                ProcessReceive(args);
                return;
            }
        }

        void ProcessReceive(SocketAsyncEventArgs args)
        {
            ClientTCP client = args.UserToken as ClientTCP;

            // client's connection has already been disconnected.
            if (client == null || client.ClntSock == null)
                return;

            int bytesReceived = args.BytesTransferred;

            if (args.BytesTransferred > 0 && args.SocketError == SocketError.Success)
            {
                // since args is using a buffer that is already assigned to the SocketAsyncEventArgsPool
                // it requires a process of extracting the buffer by copying it separately using offset
                byte[] receivedBytes = new byte[bytesReceived];
                System.Buffer.BlockCopy(args.Buffer, 0, receivedBytes, 0, receivedBytes.Length);

                // put the buffer with the contents to be processed into the packet queue
                lock (Program.lockObj)
                {
                    PacketHandler.Instance.PacketQueue.Enqueue(Tuple.Create(client.ClientId, receivedBytes));
                }

                try
                {
                    bool pending = client.ClntSock.ReceiveAsync(args);
                    if (!pending)
                    {
                        ReceiveCompleted(null, args);
                    }
                }
                catch (NullReferenceException e)
                {
                    Logger.Instance.WriteLog(LogLevel.ERROR, client.PublicEP.ToString(), e.Message);
                    Console.WriteLine("null reference exception occurred: " + e.Message);
                }
            }
            else
            {
                ServerTCP.Instance.RemoveClient(client.ClientId);
                return;
            }
        }

        public void Send(SocketAsyncEventArgs args, byte[] buffer)
        {
            ClientTCP client = args.UserToken as ClientTCP;

            if (client == null)
                return;

            lock (client.SendQueueLock)
            {
                client.SendQueue.Enqueue(buffer);

                if (client.SendQueue.Count > 1)
                {
                    return;
                }

                StartSend(args);
            }
        }

        void StartSend(SocketAsyncEventArgs args)
        {
            ClientTCP client = args.UserToken as ClientTCP;

            if (client == null)
                return;

            lock (client.SendQueueLock)
            {
                    try
                {
                    if (client.SendQueue.Count == 0)
                        return;

                    byte[] buffer = client.SendQueue.Peek();

                    client.SendEventArgs.SetBuffer(buffer, 0, buffer.Length);
                    bool pending = client.ClntSock.SendAsync(client.SendEventArgs);
                    if (!pending)
                    {
                        OnSendCompleted(null, client.SendEventArgs);
                    }
                }
                catch (NullReferenceException e)
                {
                    Logger.Instance.WriteLog(LogLevel.ERROR, client.ClntSock.RemoteEndPoint.ToString(), e.Message);
                    Console.WriteLine(e.Message);
                }
            }
        }

        public void OnSendCompleted(object sender, SocketAsyncEventArgs args)
        {
            ClientTCP client = args.UserToken as ClientTCP;

            if (client == null)
                return;

            lock (client.SendQueueLock)
            {
                client.SendQueue.Dequeue();

                if (client.SendQueue.Count > 0)
                {
                    StartSend(args);
                    return;
                }

                if (client.State == ClientState.DISCONNECTED)
                {
                    ServerTCP.Instance.RemoveClient(client.ClientId);
                }
            }
        }
    }
}
