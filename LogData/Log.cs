using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Text;
using System.Threading.Tasks;

namespace WPP.LogData
{
    public enum LogLevel
    {
        TRACE,
        DEBUG,
        INFO,
        ERROR,
        FATAL,
    }

    [Serializable]
    public class Log
    {
        public string logLevel;
        public string timestamp;
        public string ep;
        public string message;

        public Log(LogLevel logLevel, string ep, string message)
        {
            this.logLevel = logLevel.ToString();
            timestamp = DateTime.Now.ToString("yyyy-MM-dd HH:mm:ss.fff");
            this.ep = ep;
            this.message = message;
        }

        public override string ToString()
        {
            string logString = logLevel + " | " + timestamp + " | " + ep + " | " + message;
            return logString;
        }
    }
}
