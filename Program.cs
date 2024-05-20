using System;
using System.Threading;
using WPP.Protocol.Server;
using WPP.Logic.Match;
using WPP.Data;
using WPP.Protocol.Client;
using WPP.Protocol;

namespace WPP
{
    internal class Program
    {
        private static Thread _consoleThread;
        public static object lockObj = new object();
        static void Main(string[] args)
        {
            ServerTCP.Instance.InitializeServer();
            DatabaseManager.Instance.InitializeMySQLServer();
            PacketHandler.Instance.InitializePacketHandler();
            MatchMaker.Instance.InitializeMatchThread();

            InitializeConsoleThread();
        }

        private static void InitializeConsoleThread()
        {
            _consoleThread = new Thread(ConsoleLoop);
            _consoleThread.Name = "consoleThread";
            _consoleThread.IsBackground = false;
            _consoleThread.Start();
        }

        private static void ConsoleLoop()
        {
            Console.WriteLine("Server initalization completed");

            while (true)
            {
                lock (lockObj)
                {
                    if (PacketHandler.Instance.PacketQueue.Count > 0)
                    {
                        PacketHandler.Instance.HandlePacket(PacketHandler.Instance.PacketQueue.Dequeue());
                    }
                }

            }
        }
    }
}
