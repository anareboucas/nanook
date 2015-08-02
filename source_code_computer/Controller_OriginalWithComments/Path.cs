using System;
using System.Collections.Generic;
using System.Text;
using System.Drawing;
using System.IO;

namespace Controller
{
    /**
     * @brief Values that represent waypoint scan modes.
     */
    public enum WaypointScanMode
    {
        None, Full
    }

    /**
     * @brief A path waypoint.
     */
    public class PathWaypoint
    {
        /**
         * @brief Constructor.
         * @param Waypoint A PointF
         */
        public PathWaypoint(PointF Waypoint)
        { this.Waypoint = Waypoint; ScanMode = WaypointScanMode.None; }

        public PointF Waypoint;
        public WaypointScanMode ScanMode;
    }

    /**
     * @brief A path.
     */
    public class Path
    {
        private List<PathWaypoint> m_Points;

        /**
         * @brief Default constructor.
        */
        public Path()
        {
            m_Points = new List<PathWaypoint>();
        }

        /**
         * @brief Indexer to get or set items within this collection using array index syntax.        
         * @param i An integer - Zero-based index of the entry to access.
         * @returns A PathWaypoint - The indexed item.
            */
        public PathWaypoint this[int i]
        {
            get { return m_Points[i]; }
            set { m_Points[i] = value; }
        }

        /**
         * Gets the number of m_Points
         */
        public int Count
        { get { return m_Points.Count; } }

        /**
         * @param i An integer - Zero-based index of the entry to access.
         * @param P A PathWaypoint.
         */
        public void Insert(int i, PathWaypoint P)
        { m_Points.Insert(i, P); }

        /**
         * @brief Adds a PathWaypoint
         * @param P A PathWaypoint
         */
        public void Add(PathWaypoint P)
        { m_Points.Add(P); }

        /**
         * @brief Removes at described by i.
         * @param i An integer - Zero-based index of the entry to access.
        */
        public void RemoveAt(int i)
        { m_Points.RemoveAt(i); }

        /**
         * @brief Clears this object to its blank/initial state.
        */
        public void Clear()
        { m_Points.Clear(); }

        /**
         * @brief Reads the given file.
         * @param Filename A string
         * @returns A Path
         */
        public static Path Read(string Filename)
        {
            return Read(new BinaryReader(File.Open(Filename, FileMode.Open)));
        }

        /**
         * @brief Reads the given buffer.
         * @param Buffer A byte[]
         * @returns A Path
         */
        public static Path Read(byte[] Buffer)
        {
            return Read(new BinaryReader(new MemoryStream(Buffer)));
        }

        /**
         * @brief Reads the given reader.
         * \exception "InvalidOperationException"
         * @brief Thrown when the requested operation is invalid.
         * @param Reader A BinaryReader
         * @returns P - A Path
         */
        public static Path Read(BinaryReader Reader)
        {
            Path P = new Path();

            UInt32 Version = Reader.ReadUInt32();
            if (Version != 0x101)
                throw new InvalidOperationException("The Path format is too old");

            int Count = Reader.ReadInt32();
            P.m_Points = new List<PathWaypoint>(Count);

            for (int i = 0; i < Count; i++)
                P.m_Points.Add(new PathWaypoint(new PointF(Reader.ReadSingle(), Reader.ReadSingle())));

            return P;
        }

        /**
         * @brief Writes the given file.
         * @param Filename A string
         */
        public void Write(string Filename)
        {
            Write(new BinaryWriter(File.Open(Filename, FileMode.Create)));
        }

        /**
         * @brief Writes the given buffer.
         * @param Buffer out byte[]
         */
        public void Write(out byte[] Buffer)
        {
            MemoryStream MS = new MemoryStream();
            Write(new BinaryWriter(MS));
            Buffer = new byte[MS.Length];
            Array.Copy(MS.GetBuffer(), Buffer, Buffer.Length);
        }

        /**
         * @brief Writes the given writer.
         * @param Writer A BinaryWriter
         */
        public void Write(BinaryWriter Writer)
        {
            UInt32 Version = 0x101;

            Writer.Write(Version);

            Writer.Write(Count);
            for (int i = 0; i < Count; i++)
            {
                Writer.Write(m_Points[i].Waypoint.X);
                Writer.Write(m_Points[i].Waypoint.Y);
                Writer.Write((UInt32)m_Points[i].ScanMode);
            }
        }


    }
}
