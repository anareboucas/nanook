using System;
using System.Collections.Generic;
using System.Text;
using System.Drawing;
using System.IO;
using Microsoft.DirectX;

namespace Controller
{
    /**
     * @brief A point cloud.
    */
    public class PointCloud
    {
        private List<Vector3> m_Points = new List<Vector3>();

        /**
         * @brief Indexer to get or set items within this collection using array index syntax.
          * @param i An integer - Zero-based index of the entry to access.
         * @return A Vector3 (The indexed item)
         */
        public Vector3 this[int i]
        {
            get { return m_Points[i]; }
            set { m_Points[i] = value; }
        }

        /**
         * @brief Gets the number of Points
         */
        public int Count
        { get { return m_Points.Count; } }

        /**
         * @brief Reads the given byte[]
         * @param B A byte[]
         * @returns A PointCloud
         */
        public static PointCloud Read(byte[] B)
        {
            return Read(new BinaryReader(new MemoryStream(B)));
        }

        /**
         * @brief Reads the given file.
         * @param Filename A string
         * @returns A PointCloud
         */
        public static PointCloud Read(string Filename)
        {
            return Read(new BinaryReader(File.OpenRead(Filename)));
        }

        /**
         * @brief Reads the given BinaryReader
         * @param R A BinaryReader
         * @returns P - A PointCloud
         */
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

        /**
         * @brief Writes the given file.
         * @param Filename A string
         */
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
