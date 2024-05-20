using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Numerics;
using System.Text;
using System.Threading.Tasks;
using static Google.Protobuf.Reflection.SourceCodeInfo.Types;

namespace WPP.Protocol
{
    class ByteBuffer
    {
        private byte[] _tempBuffer;

        public List<byte> buffer;
        public int ReadIndex { get; set; }

        public ByteBuffer()
        {
            buffer = new List<byte>();
            ReadIndex = 0;
        }

        public int Count()
        {
            return buffer.Count - ReadIndex;
        }

        public void Clear()
        {
            buffer.Clear();
            return;
        }

        public byte[] ToArray()
        {
            return buffer.ToArray();
        }

        public void WriteBytes(byte[] input)
        {
            buffer.AddRange(input);
            return;
        }
        public void WriteShort(short input)
        {
            buffer.AddRange(BitConverter.GetBytes(input));
            return;
        }

        public void WriteInteger(int input)
        {
            buffer.AddRange(BitConverter.GetBytes(input));
            return;
        }

        public void WriteLong(long input)
        {
            buffer.AddRange(BitConverter.GetBytes(input));
        }

        public void WriteFloat(float input)
        {
            buffer.AddRange(BitConverter.GetBytes(input));
            return;
        }

        public void WriteString(string input)
        {
            buffer.AddRange(BitConverter.GetBytes(input.Length));
            buffer.AddRange(Encoding.ASCII.GetBytes(input));
            return;
        }

        public void WriteEndPoint(IPEndPoint ep)
        {
            string endPointString = $"{ep.Address}:{ep.Port}";
            buffer.AddRange(BitConverter.GetBytes(endPointString.Length));
            buffer.AddRange(Encoding.ASCII.GetBytes(endPointString));
            return;
        }

        public void WriteVector3(Vector3 position)
        {
            buffer.AddRange(BitConverter.GetBytes(position.X));
            buffer.AddRange(BitConverter.GetBytes(position.Y));
            buffer.AddRange(BitConverter.GetBytes(position.Z));
            return;
        }

        public void WriteQuaternion(Quaternion rotation)
        {
            buffer.AddRange(BitConverter.GetBytes(rotation.X));
            buffer.AddRange(BitConverter.GetBytes(rotation.Y));
            buffer.AddRange(BitConverter.GetBytes(rotation.Z));
            buffer.AddRange(BitConverter.GetBytes(rotation.W));
            return;
        }

        public void WriteDateTime(DateTime dateTime)
        {
            buffer.AddRange(BitConverter.GetBytes(dateTime.Ticks));
            return;
        }

        public byte[] ReadBytes(int numOfBytesToRead, bool moveHead)
        {
            _tempBuffer = buffer.ToArray();
            byte[] returnBytes = new byte[numOfBytesToRead];
            Buffer.BlockCopy(_tempBuffer, ReadIndex, returnBytes, 0, numOfBytesToRead);
            if (moveHead)
                ReadIndex += numOfBytesToRead;
            return returnBytes;
        }

        public short ReadShort(bool moveHead)
        {
            _tempBuffer = buffer.ToArray();
            short ret = BitConverter.ToInt16(_tempBuffer, ReadIndex);
            if (moveHead)
                ReadIndex += sizeof(short);
            return ret;
        }
        public int ReadInteger(bool moveHead)
        {
            _tempBuffer = buffer.ToArray();
            int ret = BitConverter.ToInt32(_tempBuffer, ReadIndex);
            if (moveHead)
                ReadIndex += sizeof(int);
            return ret;
        }

        public long ReadLong(bool moveHead)
        {
            _tempBuffer = buffer.ToArray();
            long ret = BitConverter.ToInt64(_tempBuffer, ReadIndex);
            if (moveHead)
                ReadIndex += sizeof(long);
            return ret;
        }

        public float ReadFloat(bool moveHead)
        {
            _tempBuffer = buffer.ToArray();
            float ret = BitConverter.ToSingle(_tempBuffer, ReadIndex);
            if (moveHead)
                ReadIndex += sizeof(float);
            return ret;
        }

        public string ReadString(bool moveHead)
        {
            _tempBuffer = buffer.ToArray();
            int length = ReadInteger(true);
            string ret = Encoding.ASCII.GetString(_tempBuffer, ReadIndex, length);
            if (moveHead)
                ReadIndex += length;
            return ret;
        }

        public IPEndPoint ReadEndPoint(bool moveHead)
        {
            _tempBuffer = buffer.ToArray();
            int length = ReadInteger(true);

            string ret = Encoding.ASCII.GetString(_tempBuffer, ReadIndex, length);

            if (moveHead)
                ReadIndex += length;

            string[] parts = ret.Split(':');
            if (parts.Length == 2 && IPAddress.TryParse(parts[0], out IPAddress ipAddress) && int.TryParse(parts[1], out int port))
            {
                IPEndPoint endPoint = new IPEndPoint(ipAddress, port);
                return endPoint;
            }
            return null;
        }

        public Vector3 ReadVector3(bool moveHead)
        {
            _tempBuffer = buffer.ToArray();
            float x = BitConverter.ToSingle(_tempBuffer, ReadIndex);
            if (moveHead)
                ReadIndex += sizeof(float);
            float y = BitConverter.ToSingle(_tempBuffer, ReadIndex);
            if (moveHead)
                ReadIndex += sizeof(float);
            float z = BitConverter.ToSingle(_tempBuffer, ReadIndex);
            if (moveHead)
                ReadIndex += sizeof(float);
            Vector3 ret = new Vector3(x, y, z);
            return ret;
        }

        public Quaternion ReadQuaternion(bool moveHead)
        {
            _tempBuffer = buffer.ToArray();
            float x = BitConverter.ToSingle(_tempBuffer, ReadIndex);
            if (moveHead)
                ReadIndex += sizeof(float);
            float y = BitConverter.ToSingle(_tempBuffer, ReadIndex);
            if (moveHead)
                ReadIndex += sizeof(float);
            float z = BitConverter.ToSingle(_tempBuffer, ReadIndex);
            if (moveHead)
                ReadIndex += sizeof(float);
            float w = BitConverter.ToSingle(_tempBuffer, ReadIndex);
            if (moveHead)
                ReadIndex += sizeof(float);
            Quaternion ret = new Quaternion(x, y, z, w);
            return ret;
        }

        public DateTime ReadDateTime(bool moveHead)
        {
            _tempBuffer = buffer.ToArray();
            long ticks = BitConverter.ToInt64(_tempBuffer.ToArray(), ReadIndex);
            if (moveHead)
                ReadIndex += sizeof(long);
            return new DateTime(ticks);
        }
    }
}
