using System;
using System.Collections.Generic;
using System.Text;
using System.Drawing;
using System.IO;

namespace Controller
{
    /**
     * @brief An image
     */
    public class Image
    {
         /**
         * @brief Reads the given file
         * @param Filename A string (The filename to read)
         * @returns An Image.
         */
        public static Image Read(string Filename)
        {
            return Read(new BinaryReader(File.Open(Filename, FileMode.Open)));
        }

        /**
         * @brief Reads
         * @param Buffer A byte[]
         * @param Offset An integer
         * @param Length An integer
         * @returns An Image
         */
        public static Image Read(byte[] Buffer, int Offset, int Length)
        {
            MemoryStream Stream = new MemoryStream(Buffer, Offset, Length);
            return Read(new BinaryReader(Stream));
        }

        /**
         * @brief Reads the given reader
         * @param Reader A BinaryReader
         * @returns I - An Image
         */
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
                I.m_Pixels[i] = Color.FromArgb(I.m_RawPixels[3 * i], I.m_RawPixels[3 * i + 1], I.m_RawPixels[3 * i + 2]);

            return I;
        }

        /**
         * @brief Indexer to get items within this collection using array index syntax.
         * @param i An integer - Zero-based index of the entry to access.
         * @param j An integer - The int to process.
         * @returns The indexed item.
         */
        public Color this[int i, int j]
        { get { return m_Pixels[i * m_Width + j]; } }

        /**
         * @brief Gets the Date/Time of the aquired on.
         */
        public DateTime AquiredOn
        { get { return m_AquiredOn; } }

        /**
         * @brief Gets the width
         */
        public int Width
        { get { return m_Width; } }

        /**
         * @brief Gets the height.
         */
        public int Height
        { get { return m_Height; } }

        /**
         * @brief Gets the sequence number.
         */
        public int SequenceNumber
        { get { return m_SequenceNumber; } }

        /**
         * @brief Gets the raw pixels.
         */
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