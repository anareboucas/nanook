using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using Microsoft.DirectX;
using System.Drawing;

namespace Controller
{

  public class RangeImage
  {
    public static RangeImage Read(string Filename)
    {
        FileStream f = File.Open(Filename, FileMode.Open);
        BinaryReader br = new BinaryReader(f);
        RangeImage ri = Read(br);
        f.Close();
        return ri;
    }

    public static RangeImage Read(byte[] Buffer, int Offset, int Length)
    {
      MemoryStream Stream = new MemoryStream(Buffer, Offset, Length);
      return Read(new BinaryReader(Stream));
    }

    public static RangeImage Read(BinaryReader Reader)
    {
      Reader.BaseStream.Position = 0;
      uint Version = Reader.ReadUInt32();
      RangeImage I = new RangeImage();
      switch (Version)
      {
          case 0x0104:

              //RangeImage I = new RangeImage();

              I.m_Width = Reader.ReadInt32();
              I.m_Height = Reader.ReadInt32();

              I.m_ScannerLocation = new PointF(Reader.ReadSingle(), Reader.ReadSingle());
              I.m_ScannerPrimaryAxis = new PointF(Reader.ReadSingle(), Reader.ReadSingle());

              I.m_HorizontalFieldOfView = Reader.ReadSingle();
              I.m_VerticalFieldOfView = Reader.ReadSingle();
              long l = Reader.ReadInt64();
              I.m_AquiredOn = DateTime.Now;
              I.m_SequenceNumber = Reader.ReadInt32();

              I.m_Range = Reader.ReadSingle();

              break;

          case 0x0200:

              //RangeImage I = new RangeImage();
              Int32 length = Reader.ReadInt32();
              Int32 bytesRead = 8;
              float locX=0.0f, locY=0.0f;
              float oriX=0.0f, oriY=0.0f;
              float temp;

              while (bytesRead < length)
              {
                  Byte key = Reader.ReadByte();
                  bytesRead++;
                  switch (key)
                  {
                      case WIDTH_KEY:
                          I.m_Width = Reader.ReadInt32();
                          bytesRead += 4;
                          break;
                      case HEIGHT_KEY:
                          I.m_Height = Reader.ReadInt32();
                          bytesRead += 4;
                          break;
                      case POSITION_X_KEY:
                          temp = Reader.ReadSingle();
                          bytesRead += 4;
                          I.m_ScannerLocation = new PointF(temp, locY);
                          break;
                      case POSITION_Y_KEY:
                          temp = Reader.ReadSingle();
                          bytesRead += 4;
                          I.m_ScannerLocation = new PointF(locX, temp);
                          break;
                      case ORIENTATION_X_KEY:
                          temp = Reader.ReadSingle();
                          I.m_ScannerPrimaryAxis = new PointF(oriX, temp);
                          bytesRead += 4;
                          break;
                      case ORIENTATION_Y_KEY:
                          temp = Reader.ReadSingle();
                          I.m_ScannerPrimaryAxis = new PointF(temp, oriY);
                          bytesRead += 4;
                          break;
                      case HORIZONTAL_FOV_KEY:
                          I.m_HorizontalFieldOfView = Reader.ReadSingle();
                          bytesRead += 4;
                          break;
                      case VERTICAL_FOV_KEY:
                          I.m_VerticalFieldOfView = Reader.ReadSingle();
                          bytesRead += 4;
                          break;
                      case ACQUISITION_TIME_KEY:
                          // XXX: should probably actually use this data
                          long discarded = Reader.ReadInt64();
                          bytesRead+=8;
                          I.m_AquiredOn = DateTime.Now;
                          break;
                      case SEQUENCE_NUMBER_KEY:
                          I.m_SequenceNumber = Reader.ReadInt32();
                          bytesRead += 4;
                          break;
                      case MAX_RANGE_VALUE_KEY:
                          I.m_Range = Reader.ReadSingle();
                          bytesRead += 4;
                          break;
                      case VERTICAL_BIAS_KEY:
                          I.m_VerticalBias = Reader.ReadSingle();
                          bytesRead += 4;
                          break;
                      case HORIZONTAL_BIAS_KEY:
                          I.m_HorizontalBias = Reader.ReadSingle();
                          bytesRead += 4;
                          break;
                  }
              }
              break;

          default:
              throw new InvalidOperationException("The RangeImage version is not recognized");
      }
      I.m_RangeValues = new float[I.m_Width * I.m_Height];

      for (int i = 0; i < I.m_RangeValues.Length; i++)
          I.m_RangeValues[i] = Reader.ReadSingle();
      return I;
    }

    public void Write(string Filename)
    {
      Write(new BinaryWriter(File.Open(Filename, FileMode.Create)));
    }

    public void Write(byte[] Buffer)
    {
      Write(new BinaryWriter(new MemoryStream(Buffer)));
    }

    public void Write(BinaryWriter Writer, uint Version)
    {
        switch (Version)
        {
            case 0x0104:
                Writer.Write(Version);


                Writer.Write(m_Width);
                Writer.Write(m_Height);

                Writer.Write(m_ScannerLocation.X);
                Writer.Write(m_ScannerLocation.Y);

                Writer.Write(m_ScannerPrimaryAxis.X);
                Writer.Write(m_ScannerPrimaryAxis.Y);

                Writer.Write(m_HorizontalFieldOfView);
                Writer.Write(m_VerticalFieldOfView);

                Writer.Write(m_AquiredOn.Ticks);
                Writer.Write(m_SequenceNumber);

                Writer.Write(m_Range);
                break;

            case 0x0200:
                Writer.Write(Version);
                Writer.Write((Int32)77);
                Writer.Write(WIDTH_KEY);
                Writer.Write(m_Width);
                Writer.Write(HEIGHT_KEY);
                Writer.Write(m_Height);

                Writer.Write(POSITION_X_KEY);
                Writer.Write(m_ScannerLocation.X);
                Writer.Write(POSITION_Y_KEY);
                Writer.Write(m_ScannerLocation.Y);

                Writer.Write(ORIENTATION_X_KEY);
                Writer.Write(m_ScannerPrimaryAxis.X);
                Writer.Write(ORIENTATION_Y_KEY);
                Writer.Write(m_ScannerPrimaryAxis.Y);

                Writer.Write(HORIZONTAL_FOV_KEY);
                Writer.Write(m_HorizontalFieldOfView);
                Writer.Write(VERTICAL_FOV_KEY);
                Writer.Write(m_VerticalFieldOfView);

                Writer.Write(ACQUISITION_TIME_KEY);
                Writer.Write(m_AquiredOn.Ticks);
                Writer.Write(SEQUENCE_NUMBER_KEY);
                Writer.Write(m_SequenceNumber);

                Writer.Write(MAX_RANGE_VALUE_KEY);
                Writer.Write(m_Range);

                Writer.Write(VERTICAL_BIAS_KEY);
                Writer.Write(m_VerticalBias);
                Writer.Write(HORIZONTAL_BIAS_KEY);
                Writer.Write(m_HorizontalBias);
                break;

            default:
                throw new InvalidOperationException("The RangeImage version is not recognized"); 
        }
      
      for (int i = 0; i < m_RangeValues.Length; i++)
        Writer.Write(m_RangeValues[i]);
    }

    public void Write(BinaryWriter Writer)
    {
        Write(Writer, 0x0200);
    }

    public float this[int i, int j]
    { get { return m_RangeValues[i * m_Width + m_Width - j - 1]; } }

    public float GetCustom(int i, int j)
    {
        /**/
        if (m_Height < m_Width)
        {
            j = (int)(j / 1.1f);
        }
        else
        {
            i = (int)(i / 1.07f);
        }

        if (i == 0)
            return m_RangeValues[j];
        /**/
        return m_RangeValues[j * m_Width + m_Width - i - 1];
    }
    public float Get(int i, int j)
    {
        return m_RangeValues[i * m_Width + m_Width - j - 1];
    }
    public float Get(int i)
    { return m_RangeValues[i]; }

    public int Width
    { get { return m_Width; }  }
    public int Height
    { get { return m_Height; } }
    public DateTime AquiredOn
    { get { return m_AquiredOn; } }
    public int SequenceNumber
    { get { return m_SequenceNumber; } }
    public float MaximumRange
    { get { return m_Range; } }
    public PointF ScannerLocation
    { get { return m_ScannerLocation; } }
    public PointF ScannerPrimaryAxis
    { get { return m_ScannerPrimaryAxis; } }

    public Vector3 ConvertToPoint(int i, int j)
    {
      float Theta = (i - (m_Height * 0.5f)) / m_Height * 2.0f * m_VerticalFieldOfView * 0.5f * 0.01745329f;
      float Phi = (j - (m_Width * 0.5f)) / m_Width * 2.0f * m_HorizontalFieldOfView * 0.5f * 0.01745329f;
      float r = Get(i, j);
    
      Vector3 Point;

      Point.X = (float)Math.Sin(Phi) * r;
      Point.Y = (float)Math.Sin(Theta) * (float)Math.Cos(Phi) * r;
      Point.Z = (float)Math.Cos(Theta) * (float)Math.Cos(Phi) * r;

      return Point;
    }

    public float[,] ConvertToHeightMap()
    {
        float[,] grid = new float[m_Width,m_Height];
        /*
        for (int i = 0; i < 20; i++)
        {
            //grid[i,
        }*/


        return grid;
    }

    public Vector3[] ConvertToPoints(float MaxDistance)
    {
      Vector3[] Points = new Vector3[m_Width * m_Height];

      int k = 0;
      for (int i = 0; i < m_Height; i++)
      {
        float Theta = (i - (m_Height * 0.5f)) / m_Height * 2.0f * m_VerticalFieldOfView * 0.5f * 0.01745329f;
        for (int j = 0; j < m_Width; j++)
        {
          float Phi = (j - (m_Width * 0.5f)) / m_Width * 2.0f * m_HorizontalFieldOfView * 0.5f * 0.01745329f;
          float r = Get(i, j);

          if (r < MaxDistance)
          {
            //Turn to polar coordinates
            Points[k].X = (float)Math.Sin(Phi) * r;
            Points[k].Y = (float)Math.Sin(Theta) * (float)Math.Cos(Phi) * r;
            Points[k].Z = (float)Math.Cos(Theta) * (float)Math.Cos(Phi) * r;

            k++;
          }
        }
      }

      return Points;
    }

      public void setWidth(int width){
          this.m_Width = width;
      }
      public void setHeight(int height)
      {
          this.m_Height = height;
      }
      public bool initializeRangeValues(int width, int height)
      {
          try
          {
              m_RangeValues = new float[width*height];
              return true;
          }
          catch
          {
              return false;
          }
      }
      public void addValue(int index,float value)
      {
          m_RangeValues[index] = value;
      }
    private int m_Width;
    private int m_Height;
    private DateTime m_AquiredOn;
    private int m_SequenceNumber;
    private float[] m_RangeValues;
    private float m_HorizontalFieldOfView;
    private float m_VerticalFieldOfView;
    private float m_Range;
    private PointF m_ScannerLocation;
    private PointF m_ScannerPrimaryAxis;
    private float m_HorizontalBias;
    private float m_VerticalBias;

    const Byte WIDTH_KEY = 0x0;
    const Byte HEIGHT_KEY = 0x1;
    const Byte POSITION_X_KEY = 0x2;
    const Byte POSITION_Y_KEY = 0x3;
    const Byte ORIENTATION_X_KEY = 0x4;
    const Byte ORIENTATION_Y_KEY = 0x5;
    const Byte HORIZONTAL_FOV_KEY = 0x6;
    const Byte VERTICAL_FOV_KEY = 0x7;
    const Byte ACQUISITION_TIME_KEY = 0x8;
    const Byte SEQUENCE_NUMBER_KEY = 0x9;
    const Byte MAX_RANGE_VALUE_KEY = 0xa;
    const Byte VERTICAL_BIAS_KEY = 0xb;
    const Byte HORIZONTAL_BIAS_KEY = 0xc;

  }
}
