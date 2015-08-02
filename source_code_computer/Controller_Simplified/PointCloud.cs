using System;
using System.Collections.Generic;
using System.Text;
using System.Drawing;
using System.IO;
using Microsoft.DirectX;

namespace Controller
{
  public class PointCloud
  {
    private List<Vector3> m_Points = new List<Vector3>();

    public Vector3 this[int i]
    {
      get { return m_Points[i]; }
      set { m_Points[i] = value; }
    }

    public int Count
    { get { return m_Points.Count; } }

    public static PointCloud Read(byte[] B)
    {
      return Read(new BinaryReader(new MemoryStream(B)));
    }

    public static PointCloud Read(string Filename)
    {
      return Read(new BinaryReader(File.OpenRead(Filename)));
    }

    public static PointCloud Read(BinaryReader R)
    {
      PointCloud P = new PointCloud();
      int N = R.ReadInt32();

      for (int i = 0; i < N; i++)
      {
        Vector3 V = new Vector3();
        V.X = R.ReadSingle();
        V.Y = R.ReadSingle();
        V.Z = R.ReadSingle();

        P.m_Points.Add(V);
      }

      return P;
    }

    public void Write(string Filename)
    {
      BinaryWriter W = new BinaryWriter(File.Open(Filename, FileMode.Create));

      W.Write(m_Points.Count);
      for (int i = 0; i < Count; i++)
      {

        W.Write(m_Points[i].X);
        W.Write(m_Points[i].Y);
        W.Write(m_Points[i].Z);
      }

    }
  }
}
