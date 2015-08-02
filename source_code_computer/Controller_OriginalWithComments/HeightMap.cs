/*
 * Author: Pramod K. Verma <pramod@cs.jhu.edu>
 * FILE: height map
 * 
 * 
 * */

using System;
using System.Collections.Generic;
using System.Text;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Drawing.Imaging;
using System.Windows.Forms;
using Microsoft.DirectX;
using Microsoft.DirectX.Direct3D;
using Microsoft.Win32;
using System.IO;

namespace Controller
{
    /**
     * A height map.
    */
    class HeightMap
    {
        private RangeImage m_RangeImage;
        public int[,] occ_grid;
        public bool[,] bool_grid;
        int GRID_WIDTH_Z;
        int GRID_WIDTH_X;
        private int occ_count;

        /**
         * @brief Gets maximum grid z coordinate.
         * @return GRID_WIDTH_Z - An integer (The maximum grid z coordinate)
        */
        public int getMaxGridZ()
        {
            return GRID_WIDTH_Z;
        }

        /**
         * @brief Gets maximum grid x coordinate.
         * @return GRID_WIDTH_X - An integer (The maximum grid x coordinate)
        */
        public int getMaxGridX()
        {
            return GRID_WIDTH_X;
        }

        /**
         * @brief Gets the offset.
           @returns An integer (The offset)
        */

        public int getOffset()
        {
            return (int)Math.Abs(dx_min / 0.2);
        }

        /**
         * @brief Gets free cell count.
         * @returns occ_count An integer (The free cell count)
        */
        public int getFreeCellCount()
        {
            return occ_count;
        }

        public double dx_min;

        /**
         * @brief Constructor.
         * @param RangeImage A RangeImage
         */
        public HeightMap(RangeImage RangeImage)
        {
            m_RangeImage = RangeImage;
            occ_count = -1;
            GRID_WIDTH_Z = 0;
            GRID_WIDTH_Z = 0;
        }

        /**
         * @brief Creates the map.
        */
        public void createMap()
        {

            int x, y, z;

            Vector3[] asdf = m_RangeImage.ConvertToPoints(1000);
            double dz = 100000;
            double dx_max = 0;
            dx_min = 1000000;

            double X, Y, Z;
            double XX, YY, ZZ;
            for (y = 0; y < m_RangeImage.Height; y = y + 1)
            {
                for (x = 0; x < m_RangeImage.Width; x = x + 1)
                {
                    Vector3 tmp = asdf[y * m_RangeImage.Height + x];
                    X = tmp.X;
                    Y = tmp.Y;
                    Z = tmp.Z;


                    double q = ((10 * 3.14) / 180);
                    YY = Y * Math.Cos(q) - Z * Math.Sin(q);
                    ZZ = Y * Math.Sin(q) + Z * Math.Cos(q);
                    XX = X;

                    if (ZZ < 10) // check if depth is less than 10 meter
                    {
                        if (dz > YY) { dz = YY; }
                        if (dx_max < XX) { dx_max = XX; }
                        if (dx_min > XX) { dx_min = XX; }
                    }
                }
            }

            double Range1 = dz + 0.7;
            double Range2 = Range1 + 0.6;
            //MessageBox.Show("Range1:=" + Range1 + ",Range2:=" + Range2);

            int GRID_WIDTH_Z = 1 + (int)(10 / 0.2);
            int GRID_WIDTH_X = 1 + (int)((Math.Abs(dx_min) + Math.Abs(dx_max)) / 0.2);

            occ_grid = new int[GRID_WIDTH_X, GRID_WIDTH_Z];
            bool_grid = new bool[GRID_WIDTH_X, GRID_WIDTH_Z];

            for (z = 0; z < GRID_WIDTH_Z; z++)
            {
                for (x = 0; x < GRID_WIDTH_X; x++)
                {
                    occ_grid[x, z] = 0;
                    bool_grid[x, z] = false;
                }
            }

            // MessageBox.Show("Grid size:(" + GRID_WIDTH_X + "," + GRID_WIDTH_Z + ")");

            occ_count = 0;
            for (y = 0; y < m_RangeImage.Height; y = y + 1)
            {
                for (x = 0; x < m_RangeImage.Width; x = x + 1)
                {
                    Vector3 tmp = asdf[y * m_RangeImage.Height + x];
                    X = tmp.X;
                    Y = tmp.Y;
                    Z = tmp.Z;


                    double q = ((10 * 3.14) / 180);
                    YY = Y * Math.Cos(q) - Z * Math.Sin(q);
                    ZZ = Y * Math.Sin(q) + Z * Math.Cos(q);
                    XX = X;

                    int ox = (int)Math.Floor((XX + Math.Abs(dx_min)) / 0.2);
                    int oz = (int)Math.Floor(ZZ / 0.2);

                    if (ZZ < 10)
                    {
                        if (YY > Range1 && YY < Range2)
                        {
                            // visualize obstacles here :
                            occ_grid[ox, oz] = -1;

                        }
                        else
                        {
                            // visualize other points
                        }

                        if (YY < Range1)
                        {
                            if (occ_grid[ox, oz] != -1)
                                occ_grid[ox, oz] = -100;
                        }
                        //if(dz > rp[1]) { dz = rp[1]; } 
                    }

                    // m_flatImage.SetPixel((int)tmp.X, (int)tmp.Y, Color.White);
                }
            }

            for (z = 0; z < GRID_WIDTH_Z; z++)
            {
                for (x = 0; x < GRID_WIDTH_X; x++)
                {
                    if (occ_grid[x, z] == -1)
                    {
                        bool_grid[x, z] = true;
                        //mkrect(render,lx*0.2,lz*0.2,lx*0.2+0.2,lz*0.2 + 0.2,-1.2,1,0,0);
                        //mkrect(render,-4+lx*0.2,0+lz*0.2,-4+0.2 + lx*0.2,0.2+lz*0.2,-1.2,0.872,  0.339, 0.321);
                    }
                    else if (occ_grid[x, z] == -100)
                    {
                        bool_grid[x, z] = false;
                        occ_count++;
                        //mkrect(render,lx*0.2,lz*0.2,lx*0.2+0.2,lz*0.2 + 0.2,-1.2,1,1,0);
                        //mkrect(render,-4+lx*0.2,0+lz*0.2,-4+0.2 + lx*0.2,0.2+lz*0.2,-1.2,0.872,  0.339, 0.321);
                    }
                    else
                    {
                        bool_grid[x, z] = true;
                        //mkrect(render,lx*0.2,lz*0.2,lx*0.2+0.2,lz*0.2 + 0.2,-1.2,0,1,0);
                    }
                }
            }

            // MessageBox.Show("Occupancy Free Counts:=" + occ_count);
        }
    }
}
