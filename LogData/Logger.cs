using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.Threading.Tasks;
using Newtonsoft.Json;
using System.Net;

namespace WPP.LogData
{
    /*
     Log에서 필요시 되는 내용
    - register
    - login
    - enter game
    - finish game
     */

    public class Logger : Singleton<Logger>
    {
        private bool _initialized = false;
        private string _logFilePath = Directory.GetParent(Environment.CurrentDirectory).Parent.FullName + "\\LogData\\log.txt";
        //private static string _logFilePath = "C:\\Users\\zinol\\OneDrive\\바탕 화면\\DuelOfKingdom\\DuelOfKingdom_Server\\DuelOfKingdom_Server\\LogData\\log.txt";
        private object _logLockObj = new object();
        public void WriteLog(LogLevel logLevel, string ep, string message)
        {
            try
            {
                lock (_logLockObj)
                {
                    Log log = new Log(logLevel, ep, message);
                    string logString = log.ToString() + Environment.NewLine;

                    if (!_initialized && File.Exists(_logFilePath))
                    {
                        File.Delete(_logFilePath);
                        _initialized = true;
                    }

                    using (StreamWriter sw = new StreamWriter(_logFilePath, true))
                    {
                        sw.WriteLine(logString);
                        sw.Close();
                    }
                    return;
                }
            }
            catch(Exception e)
            {
                Console.WriteLine(e.ToString());
            }
        }
    }
}
