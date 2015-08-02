using System;
using System.Collections.Generic;
using System.Text;
using System.Drawing;
using System.Net;
using System.Net.Sockets;
using System.IO;
using System.Runtime.InteropServices;
using System.Threading;
using System.Diagnostics;

namespace Controller
{
    /* Flags that can be set in the Status member of Telemetry */
    public enum TelemetryStatus
    {
        Idle = 0,
        Driving = 1,
        Navigating = 2,
        Scanning = 4,

        PathUpdated = 256,
        MapUpdated = 512,
    }

    /* Must be kept in sync with Telemetry.hpp */
    [StructLayout(LayoutKind.Sequential, Pack = 1)]
    public struct Telemetry
    {
        public uint MotionStatus;
        public uint VisionStatus;
        public uint NavigationStatus;
        public uint ClimateStatus;

        public long CurrentTime;
        public long StartTime;

        public PointF Position;
        public SizeF PrimaryAxis;
        public float Heading;

        public ulong TrackTicks0;
        public ulong TrackTicks1;
        public float TrackSpeed0;
        public float TrackSpeed1;

        public float Temperature0;
        public float Temperature1;
        public float Temperature2;
        public float Temperature3;
        public float Temperature4;
        public float Temperature5;
        public float Temperature6;
        public float Temperature7;

        public float TargetTemperature0;
        public float TargetTemperature1;
        public float TargetTemperature2;
        public float TargetTemperature3;
        public float TargetTemperature4;
        public float TargetTemperature5;
        public float TargetTemperature6;
        public float TargetTemperature7;

        public bool HeaterState0;
        public bool HeaterState1;
        public bool HeaterState2;
        public bool HeaterState3;
        public bool HeaterState4;
        public bool HeaterState5;
        public bool HeaterState6;
        public bool HeaterState7;

        public int ImageDataLength;
        public int ScanDataLength;
        public int PathDataLength;
        public int MapDataLength;
    }

    /* Represents the output of a Trace call from the robot */
    public class DebugTrace
    {
        public uint Source;
        public uint Indent;
        public string Message;
    }

    /* Message Headers */
    struct Message
    {
        public uint MessageId;
        public int Length;
    }

    public class Robot
    {
        /* The socket connection to the robot */
        protected Socket m_CommandSocket;

        /* The thread used to collect telemetry */
        protected Thread m_Thread;

        /* The host that we are connected to */
        public EndPoint Host
        {
            get { return m_CommandSocket == null ? null : m_CommandSocket.RemoteEndPoint; }
        }

        /* Determines if we are connected */
        public bool Connected
        {
            get { return m_CommandSocket == null ? false : m_CommandSocket.Connected; }
        }

        protected string m_Host;
        protected int m_Port;

        /* Constructs a new Robot interface class.  Connects to a given host and port */
        public Robot(string Host, int Port)
        {
            m_Host = Host;
            m_Port = Port;
            m_Map = new Map(1024, 1024, 0.05f);

            Connect();
        }


        /* Disconnects from the robot */
        public void Disconnect()
        {
            if (!Connected)
                return;

            SendMessage(0xffffffff, null);
            m_CommandSocket.Close();
            m_CommandSocket = null;

            m_Thread.Abort();
            m_Thread = null;
        }

        /* Disconnects from the robot */
        public void ForceDisconnect()
        {
            m_CommandSocket.Close();
            m_CommandSocket = null;

            m_Thread.Abort();
            m_Thread = null;
        }

        /* Connects to the robot */
        public bool Connect()
        {
            try
            {

                m_CommandSocket = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);

                IPAddress AddressToUse = null;
                AddressToUse = IPAddress.Parse(m_Host);
                /**
                foreach (IPAddress Address in Dns.GetHostEntry(m_Host).AddressList)
                    if (Address.AddressFamily == AddressFamily.InterNetwork)
                        AddressToUse = Address;
                /**/
                m_CommandSocket.ReceiveTimeout = 100000;
                m_CommandSocket.SendTimeout = 100000;
                m_CommandSocket.Connect(new IPEndPoint(AddressToUse, m_Port));

                byte [] response = SendMessage(0x00000001, null);



                m_Thread = new Thread(new ThreadStart(TelemetryMain));
                m_Thread.Start();

                return true;
            }
            catch (SocketException E)
            {
                m_CommandSocket = null;

                return false;
            }
        }

        /* Sends a message to the robot, and waits for the reply.  Returns the data payload */
        public byte[] SendMessage(uint Message, byte[] Data)
        {
            if (!Connected)
                return null;

            lock (this)
            {
                Message M = new Message();
                if (Data != null)
                    M.Length = Data.Length;
                else
                    M.Length = 0;
                M.MessageId = Message;

                byte[] Buffer = new byte[8 + M.Length];
                GCHandle hBuffer = GCHandle.Alloc(Buffer, GCHandleType.Pinned);
                IntPtr Address = hBuffer.AddrOfPinnedObject();
                Marshal.StructureToPtr(M, Address, false);
                hBuffer.Free();

                if (Data != null)
                    Data.CopyTo(Buffer, 8);

                m_CommandSocket.Send(Buffer);

                Buffer = new byte[8];
                SocketRead(m_CommandSocket, Buffer);
                hBuffer = GCHandle.Alloc(Buffer, GCHandleType.Pinned);
                Address = hBuffer.AddrOfPinnedObject();
                M = (Message)Marshal.PtrToStructure(Address, typeof(Message));
                hBuffer.Free();

                if (M.MessageId != (Message | 0x80000000))
                    throw new InvalidOperationException("The response MessageId was invalid");

                byte[] Response = new byte[M.Length];
                SocketRead(m_CommandSocket, Response);
                return Response;
            }
        }

        /* Estimates the round-trip latency to the robot */
        public TimeSpan GetRoundTripLatency()
        {
            long Ticks = BitConverter.ToInt64(SendMessage(0x00000001, BitConverter.GetBytes(DateTime.Now.Ticks)), 0);
            return new TimeSpan(DateTime.Now.Ticks - Ticks);
        }

        /* Drive the robot a certain distance */
        public void Drive(float Distance, int Bias)
        {
            byte[] B = new byte[8];

            Array.Copy(BitConverter.GetBytes(Distance), 0, B, 0, 4);
            Array.Copy(BitConverter.GetBytes(Bias), 0, B, 4, 4);

            SendMessage(0x00000007, B);
        }

        /* Turn the robot a number of degrees */
        public void Turn(float Angle)
        {
            SendMessage(0x00000008, BitConverter.GetBytes(Angle));
        }

        /* Set the track speed */
        public void SetSpeedAndBias(float Speed, float Bias)
        {
            byte[] B = new byte[8];

            Array.Copy(BitConverter.GetBytes(Speed), 0, B, 0, 4);
            Array.Copy(BitConverter.GetBytes(Bias), 0, B, 4, 4);

            SendMessage(0x00000009, B);
        }

        /* Stop the robot.  Abandon all currrent driving instructions. */
        public void Stop()
        {
            SendMessage(0x0000000a, null);
        }
        // abandon the scan that is currently in progress, and send back whatever range image data has been measured
        public void StopScan()
        {
            SendMessage(0x0000000d, null);
        }

        /* Request the LADAR to scan */
        public void Scan(ScanParameters p)
        {
            byte[] arr = new byte[14];
            lock (p)
            {
                arr[0] = (byte)(p.HRange * 80 + 100);
                arr[1] = (byte)(p.HRes == 0 ? 25 : p.HRes * 50);
                
                Array.Copy(BitConverter.GetBytes(p.scanLines), 0, arr, 2, 4);
                Array.Copy(BitConverter.GetBytes(p.scanRes), 0, arr, 6, 4);
                Array.Copy(BitConverter.GetBytes(p.scanBottom), 0, arr, 10, 4);
            }

            SendMessage(0x0000000b, arr);
        }

        public void SetPath(Path Path)
        {
            byte[] PathData;
            Path.Write(out PathData);

            SendMessage(0x00000003, PathData);
        }

        /* Gets the current active path from the robot */
        public Path GetPath()
        {
            byte[] Response = SendMessage(0x00000002, null);
            return Path.Read(Response);
        }

        /* Retrieves new debug/trace messages */
        public DebugTrace[] GetDebugMessages()
        {
            byte[] DebugInfo = SendMessage(0x00000006, null);
            List<DebugTrace> Messages = new List<DebugTrace>();
            BinaryReader R = new BinaryReader(new MemoryStream(DebugInfo));

            DebugTrace Trace = new DebugTrace();
            while (R.BaseStream.Position < R.BaseStream.Length)
            {

                Trace.Source = R.ReadUInt32();
                Trace.Indent = R.ReadUInt32();
                uint Length = R.ReadUInt32();

                for (uint i = 0; i < Length; i++)
                    Trace.Message += (char)R.ReadByte();

                Messages.Add(Trace);
                Trace = new DebugTrace();
            }

            return Messages.ToArray();
        }


        private Map m_Map;
        public Map Map
        { get { return m_Map; } }

        /* The most recent telemetry data */
        protected Telemetry m_Telemetry;
        public Telemetry Telemetry
        {
            get { return m_Telemetry; }
        }

        /* The telemetry history */
        protected List<Telemetry> m_TelemetryHistory = new List<Telemetry>();
        public List<Telemetry> TelemetryHistory
        {
            get { return m_TelemetryHistory; }
        }

        /* The last time telemetry data was recieved */
        protected DateTime m_LastTelemetryUpdate;
        public DateTime LastTelemetryUpdate
        {
            get { return m_LastTelemetryUpdate; }
        }

        /* The latest camera image */
        protected List<Image> m_Images = new List<Image>();
        public Image LatestImage
        { get { if (m_Images.Count == 0) return null; else return m_Images[m_Images.Count - 1]; } }
        public List<Image> Images
        { get { return m_Images; } }

        /* The latest range image */
        protected List<RangeImage> m_RangeImages = new List<RangeImage>();
        public RangeImage LatestRangeImage
        { get { if (m_RangeImages.Count == 0) return null; else return m_RangeImages[m_RangeImages.Count - 1]; } }
        public List<RangeImage> RangeImages
        { get { return m_RangeImages; } }

        /* The latest point clouds */
        protected List<PointCloud> m_PointClouds = new List<PointCloud>();
        public List<PointCloud> PointClouds
        { get { return m_PointClouds; } }

        /* The collection of telemetry data */
        protected void TelemetryMain()
        {
            while (Connected)
            {
                try
                {
                    byte[] Response = SendMessage(0x0000000c, null);

                    GCHandle hBuffer = GCHandle.Alloc(Response, GCHandleType.Pinned);
                    IntPtr Address = hBuffer.AddrOfPinnedObject();
                    Telemetry T = (Telemetry)Marshal.PtrToStructure(Address, typeof(Telemetry));
                    hBuffer.Free();

                    lock (this)
                    {
                        m_LastTelemetryUpdate = DateTime.Now;
                        m_TelemetryHistory.Add(m_Telemetry);
                        m_Telemetry = T;
                    }

                    int Offset = Marshal.SizeOf(T);

                    if (T.ImageDataLength > 0)
                    {
                        Image I = Image.Read(Response, Offset, T.ImageDataLength);

                        lock (this)
                            m_Images.Add(I);

                        Offset += T.ImageDataLength;
                    }

                    if (T.ScanDataLength > 0)
                    {
                        RangeImage I = RangeImage.Read(Response, Offset, T.ScanDataLength);

                        lock (this)
                        {
                            m_RangeImages.Add(I);
                        }

                        Offset += T.ScanDataLength;
                    }

                    if (T.PathDataLength > 0)
                    {
                        Offset += T.PathDataLength;
                    }

                    if (T.MapDataLength > 0)
                    {
                        Map Map = Map.Read(Response, Offset, T.MapDataLength);

                        lock (this)
                            m_Map = Map;

                        Offset += T.MapDataLength;
                    }

                    Thread.Sleep(1000);
                }
                catch (SocketException E)
                {
                    ForceDisconnect();
                }
            }
        }

        /* Helper function to completely fill a buffer from a socket */
        private void SocketRead(Socket S, byte[] B)
        {
            int TotalRead = 0;
            byte[] Buffer = new byte[B.Length];
            while (TotalRead < B.Length)
            {
                int ThisRead = S.Receive(Buffer, B.Length - TotalRead, SocketFlags.None);
                Array.Copy(Buffer, 0, B, TotalRead, ThisRead);
                TotalRead += ThisRead;
            }
        }

    }
}
