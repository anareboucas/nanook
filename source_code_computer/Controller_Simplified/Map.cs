using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using System.Runtime.InteropServices;
using System.Drawing;

namespace Controller
{
  public struct MapCell
  {
    public float Height;
    public uint Flags;

    public MapCell(float Height, uint Flags)
    {
      this.Height = Height;
      this.Flags = Flags;
    }
  }

  public class Map
  {
    protected int m_Width;
    protected int m_Height;
    protected float m_MinHeight;
    protected float m_MaxHeight;
    protected float m_Resolution;
    protected MapCell[] m_Cells;

    public Map()
    {
      m_MaxHeight = float.MinValue;
      m_MinHeight = float.MaxValue;
    }

    public Map(int Width, int Height, float Resolution)
    {
      m_Width = Width;
      m_Height = Height;
      m_Resolution = Resolution;
      m_MaxHeight = float.MinValue;
      m_MinHeight = float.MaxValue;

      m_Cells = new MapCell[m_Width * m_Height];
    }

    public int Width
    { get { return m_Width; } }
    public int Height
    { get { return m_Height; } }
    public Size Size
    { get { return new Size(Width, Height); } }
    public float Resolution
    { get { return m_Resolution; } }
    public float MaximumHeightValue
    { get { return m_MaxHeight; } set { m_MaxHeight = value; } }
    public float MinimumHeightValue
    { get { return m_MinHeight; } set { m_MinHeight = value; } }
    public float WorldWidth
    { get { return m_Width * m_Resolution; } }
    public float WorldHeight
    { get { return m_Height * m_Resolution; } }
    public SizeF WorldSize
    { get { return new SizeF(WorldWidth, WorldHeight); } }

    public bool ComputeMapCell(PointF World, out Point Map)
    {
      Map = new Point();
      if (World.X < (-WorldWidth / 2.0f) || World.X > (WorldWidth / 2.0f))
        return false;
      if (World.Y < (-WorldHeight / 2.0f) || World.Y > (WorldHeight / 2.0f))
        return false;

      Map = new Point((int)((World.X + (WorldWidth / 2.0f)) / m_Resolution), (int)((World.Y + (WorldWidth / 2.0f)) / m_Resolution));
      return true;
    }

    public MapCell this[int x, int y]
    {
      get { return m_Cells[y * m_Width + x]; }
      set { m_Cells[y * m_Width + x] = value; }
    }

    public MapCell this[Point P]
    {
      get { return m_Cells[P.Y * m_Width + P.X]; }
      set { m_Cells[P.Y * m_Width + P.X] = value; }
    }

    public void Save(string Filename)
    {
      byte[] B = new byte[m_Cells.Length * Marshal.SizeOf(typeof(MapCell))];
      GCHandle hData = GCHandle.Alloc(m_Cells, GCHandleType.Pinned);
      IntPtr pData = hData.AddrOfPinnedObject();
      Marshal.Copy(pData, B, 0, B.Length);
      hData.Free();

      BinaryWriter W = new BinaryWriter(File.Open(Filename, FileMode.Create));
      W.Write(m_Width);
      W.Write(m_Height);
      W.Write(m_Resolution);
      W.Write(m_MinHeight);
      W.Write(m_MaxHeight);
      W.Write(B, 0, B.Length);

      W.Close();
    }

    public static Map Read(string Filename)
    {
      return Read(new BinaryReader(File.Open(Filename, FileMode.Open)));
    }

    public static Map Read(byte[] Buffer, int Offset, int Length)
    {
      MemoryStream Stream = new MemoryStream(Buffer, Offset, Length);
      return Read(new BinaryReader(Stream));
    }

    public static Map Read(BinaryReader Reader)
    {
      Map M = new Map();

      UInt32 Version = Reader.ReadUInt32();
      if (Version != 0x101)
        throw new InvalidOperationException("The Map format is too old");
        
      M.m_Width = Reader.ReadInt32();
      M.m_Height = Reader.ReadInt32();
      M.m_Resolution = Reader.ReadSingle();
      M.m_MinHeight = Reader.ReadSingle();
      M.m_MaxHeight = Reader.ReadSingle();

      M.m_Cells = new MapCell[M.m_Width * M.m_Height];

      byte[] B = Reader.ReadBytes(M.m_Width * M.m_Height * Marshal.SizeOf(typeof(MapCell)));
      GCHandle hData = GCHandle.Alloc(M.m_Cells, GCHandleType.Pinned);
      IntPtr pData = hData.AddrOfPinnedObject();
      Marshal.Copy(B, 0, pData, B.Length);
      hData.Free();

      return M;
    }

    public void Write(string Filename)
    {
      Write(new BinaryWriter(File.Open(Filename, FileMode.Create)));
    }

    public void Write(byte[] Buffer)
    {
      Write(new BinaryWriter(new MemoryStream(Buffer)));
    }

    public void Write(BinaryWriter Writer)
    {
      UInt32 Version = 0x101;
      Writer.Write(Version);

      Writer.Write(m_Width);
      Writer.Write(m_Height);
      Writer.Write(m_Resolution);
      Writer.Write(m_MinHeight);
      Writer.Write(m_MaxHeight);
      
      byte[] B = new byte[m_Width * m_Height * Marshal.SizeOf(typeof(MapCell))];
      GCHandle hData = GCHandle.Alloc(m_Cells, GCHandleType.Pinned);
      IntPtr pData = hData.AddrOfPinnedObject();
      Marshal.Copy(pData, B, 0, B.Length);
      hData.Free();

      Writer.Write(B);
    }

  }
}
