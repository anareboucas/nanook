using System;
using System.Collections.Generic;
using System.Text;
using System.Drawing;
using System.IO;

namespace Controller
{
  public class Image
  {
    public static Image Read(string Filename)
    {
      return Read(new BinaryReader(File.Open(Filename, FileMode.Open)));
    }

    public static Image Read(byte[] Buffer, int Offset, int Length)
    {
      MemoryStream Stream = new MemoryStream(Buffer, Offset, Length); 
      return Read(new BinaryReader(Stream));
    }

    public static Image Read(BinaryReader Reader)
    {
      Image I = new Image();

      I.m_Width = Reader.ReadInt32();
      I.m_Height = Reader.ReadInt32();

      Reader.ReadSingle();
      Reader.ReadSingle();

      Reader.ReadSingle();
      Reader.ReadSingle();

      I.m_HorizontalFieldOfView = Reader.ReadSingle();
      I.m_VerticalFieldOfView = Reader.ReadSingle();
      long l = Reader.ReadInt64();
      I.m_AquiredOn = DateTime.Now; //= new DateTime(l);
      I.m_SequenceNumber = Reader.ReadInt32();

      I.m_RawPixels = Reader.ReadBytes(I.m_Width * I.m_Height * 3);
      
      I.m_Pixels = new Color[I.m_Width * I.m_Height];
      for (int i = 0; i < I.m_Pixels.Length; i++)
        I.m_Pixels[i] = Color.FromArgb(I.m_RawPixels[3 * i], I.m_RawPixels[3 * i+1], I.m_RawPixels[3 * i + 2]);
      
      return I;
    }
    public Color this[int i, int j]
    { get { return m_Pixels[i * m_Width + j]; } }

    public DateTime AquiredOn
    { get { return m_AquiredOn; } }
    public int Width
    { get { return m_Width; } }
    public int Height
    { get { return m_Height; } }
    public int SequenceNumber
    { get { return m_SequenceNumber; } }
    public byte[] RawPixels
    { get { return m_RawPixels; } }

    private DateTime m_AquiredOn;
    private int m_SequenceNumber;
    private int m_Width;
    private int m_Height;
    private Color[] m_Pixels;
    private byte[] m_RawPixels;
    private float m_HorizontalFieldOfView;
    private float m_VerticalFieldOfView;
  }
}
