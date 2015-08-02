using System;
using System.Collections.Generic;
using System.Text;
using System.Drawing;
using System.IO;

namespace Controller
{
  public enum WaypointScanMode
  {
    None, Full
  }

  public class PathWaypoint
  {
    public PathWaypoint(PointF Waypoint)
    { this.Waypoint = Waypoint; ScanMode = WaypointScanMode.None; }

    public PointF Waypoint;
    public WaypointScanMode ScanMode;
  }

  public class Path
  {
    private List<PathWaypoint> m_Points;

    public Path()
    {
      m_Points = new List<PathWaypoint>();
    }
    
    public PathWaypoint this[int i]
    { 
      get { return m_Points[i]; }
      set { m_Points[i] = value; }
    }

    public int Count
    { get { return m_Points.Count; } }

    public void Insert(int i, PathWaypoint P)
    { m_Points.Insert(i, P); }

    public void Add(PathWaypoint P)
    { m_Points.Add(P); }

    public void RemoveAt(int i)
    { m_Points.RemoveAt(i); }

    public void Clear()
    { m_Points.Clear(); }

    public static Path Read(string Filename)
    {
      return Read(new BinaryReader(File.Open(Filename, FileMode.Open)));
    }

    public static Path Read(byte[] Buffer)
    {
      return Read(new BinaryReader(new MemoryStream(Buffer)));
    }

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

    public void Write(string Filename)
    {
      Write(new BinaryWriter(File.Open(Filename, FileMode.Create)));
    }

    public void Write(out byte[] Buffer)
    {
      MemoryStream MS = new MemoryStream();
      Write(new BinaryWriter(MS));
      Buffer = new byte[MS.Length];
      Array.Copy(MS.GetBuffer(), Buffer, Buffer.Length);
    }

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
