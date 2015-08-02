using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using Microsoft.DirectX;
using System.IO;
using System.Threading;
//using Microsoft.DirectX.Direct3D;

namespace Controller
{
    public partial class SphereRecognitionView : Form
    {
        /* The robot client interface */
        protected Robot m_Robot;


        public SphereRecognitionView(Robot R)
        {
            m_Robot = R;
            InitializeComponent();
        }

        private void EdgeDetectionView_Load(object sender, EventArgs e)
        {

        }

        Graphics g;
        int height = 0;
        int width = 0;
        RangeImage rangeData;
        float[,] edges;
        float[,] edgesFixed;
        Bitmap bm;

        public void StartRecognition(RangeImage rangeImage)
        {

            rangeData = rangeImage;
            height = rangeImage.Height;
            width = rangeImage.Width;

            bm = new Bitmap(width, height);
            g = Graphics.FromImage(bm);
            image.BackgroundImage = bm;
            image.BackgroundImageLayout = ImageLayout.None;

            image.Size = new Size(width, height);

            g.Clear(Color.White);

            SphereRecognition sr = new SphereRecognition();

            //System.Windows.Forms.MessageBox.Show("W/H = " + width + "/" + height);

            image.Bounds = new Rectangle(0, 0, width, height);

            paintImage();

            edges = sr.detectEdges(rangeData, float.Parse(conf1.Text));

            //doFix();

            /**/
            paintEdges();


            List<Sphere> spheres = sr.FindSpheres(rangeData, edges, int.Parse(maxRadius.Text), int.Parse(minRadius.Text), float.Parse(conf1.Text), float.Parse(conf2.Text));

            Sphere actual;
            for (int i = 0; i < spheres.Count; i++)
            {
                actual = spheres[i];
                //int tmp = actual.center.X;
                actual.center.Y = height - actual.center.Y;
                //actual.center.Y = tmp;
                //actual.center.X += (int)(actual.radius*1.35f);
                g.DrawEllipse(Pens.Blue, (int)actual.center.X - actual.radius, ((int)actual.center.Y - actual.radius), (int)actual.radius * 2, (int)actual.radius * 2);
                g.DrawLine(Pens.Blue, (actual.center.X - 1), (actual.center.Y - 1) - 5, (actual.center.X - 1), (actual.center.Y - 1) + 5);
                g.DrawLine(Pens.Blue, (actual.center.X - 1) - 5, (actual.center.Y - 1), (actual.center.X - 1) + 5, (actual.center.Y - 1));
                image.Refresh();
                //g.DrawEllipse(Pens.Blue, (int)actual.center.Y - actual.radius, ((int)actual.center.X - actual.radius), (int)actual.radius * 2, (int)actual.radius * 2);
                //g.DrawLine(Pens.Blue, (actual.center.Y - 1), (actual.center.X - 1) - 5, (actual.center.Y - 1), (actual.center.X - 1) + 5);
                //g.DrawLine(Pens.Blue, (actual.center.Y - 1) - 5, (actual.center.X - 1), (actual.center.Y - 1) + 5, (actual.center.X - 1));
                float formula2 = ((17f / 9 * (float)actual.radius) - 94f / 9f) / 1000f;
                String algo = "haha " + actual.radius * (Math.Cos(89.9 * (Math.PI / 180)) * actual.avgDistance) + "\n" + (actual.midPoints - actual.avgDistance) + "\n" + Math.Abs(actual.midPoints - actual.avgDistance) + "\n" + formula2 + "\n" + (Math.Abs(actual.midPoints - actual.avgDistance) - formula2);
                //System.Windows.Forms.MessageBox.Show("ABSOLUTE=\nX="+actual.absolutePosition.X+"\nY="+actual.absolutePosition.Y+"\nZ="+actual.absolutePosition.Z+"\n\nx=" + actual.center.X + "\ny=" + actual.center.Y + "\n\nR=" + actual.radius + "\nDiffC=" + actual.diffCircumference + "\nS=" + actual.score + "\nAVGD=" + actual.avgDistance + "\nAVGM=" + actual.midPoints + "\nAVGE=" + actual.avgEdges + "\n\n\n" + algo);
                //paintImage();
                //paintEdges();
            }
            /**/
        }

        private void StartRecognition(String filename)
        {

            rangeData = new RangeImage();
            String extension = filename.Substring(filename.LastIndexOf(".")+1).ToLower();

            switch (extension)
            {
                case "rangeimage":
                    rangeData = RangeImage.Read(filename);
                    width = rangeData.Width;
                    height = rangeData.Height;
                    button2.Enabled = true;
                break;

                case "scan":
                    FileStream f = File.Open(filename, FileMode.Open);
                    BinaryReader br = new BinaryReader(f);
                    width = br.ReadInt32();
                    height = br.ReadInt32();
                    rangeData.setWidth(width);
                    rangeData.setHeight(height);

                    if (rangeData.initializeRangeValues(width, height))
                    {

                        for (int i = 0; i < height * width; i++)
                        {
                            try
                            {
                                rangeData.addValue(i, br.ReadSingle());
                            }
                            catch (Exception E) { }
                        }
                        //System.Windows.Forms.MessageBox.Show("W/H = " + width+"/"+height);
                        filename = filename.Replace(".scan", ".rangeimage");
                        rangeData.Write(filename);

                        /*if (value > max)
                                max = value;
                            if (value < min || min == 0)
                                min = value;

                            int val = (int)Math.Min(255, Math.Abs((value / 75f) * 255f));
                            SolidBrush sb = new SolidBrush(Color.FromArgb(255, val, val, val));
                            g.FillRectangle(sb, i/width, i%width, 1, 1);
                             */
                        //System.Windows.Forms.MessageBox.Show("avg" + avg/(height*width));
                        //System.Windows.Forms.MessageBox.Show("min" + min);
                        //System.Windows.Forms.MessageBox.Show("max" + max);
                        f.Close();
                    }
                    button2.Enabled = true;
                break;

                default:
                    System.Windows.Forms.MessageBox.Show("The format of the data is not recognized");
                    return;
                break;
            }

            StartRecognition(rangeData);

            /*
            TextWriter tw = new StreamWriter("c:\\GROUP.txt");

            for(int i=0;i<width*height;i++)
            {
                tw.WriteLine(i/width);
                tw.WriteLine(i % width);
                tw.WriteLine(rangeData.Get(i));
            }
            */
        }

        //Threshold for the distance between "layers" to consider different object
        private String filename = "";

        private void button2_Click(object sender, EventArgs e)
        {
            if (OpenFileDialog.ShowDialog() == DialogResult.OK)
            {
                filename = OpenFileDialog.FileName;

                StartRecognition(filename);
            }
        }

        private void button2_Click_1(object sender, EventArgs e)
        {
            StartRecognition(filename);
        }

        private void button3_Click(object sender, EventArgs e)
        {
            Point algo = new Point(10, 10);
            Point algo2 = new Point(243, 100);
            System.Windows.Forms.MessageBox.Show(pointDistance(algo, algo2).ToString());
        }
        private double pointDistance(Point point1, Point point2)
        {
            int tBase = Math.Abs(point1.X-point2.X);
            int tHeight = Math.Abs(point1.Y - point2.Y);
            return Math.Sqrt(Math.Pow(tBase, 2) + Math.Pow(tHeight, 2));
        }
        private Point pointAverage(Point point1, Point point2)
        {
            int x = point1.X - (point1.X - point2.X);
            int y = point1.Y - (point1.Y - point2.Y);
            return new Point(x,y);
        }

        private void paintEdges()
        {
            int cont = 0;
            for (int i = 0; i < edges.GetLength(0); i++)
            {
                for (int j = 0; j < edges.GetLength(1); j++)
                {
                    if (edges[i, j] == 1)
                    {
                        //Console.WriteLine("Edge at: ("+i+","+j+")");
                        cont++;
                        g.FillRectangle(Brushes.Red, i, height - j, 1, 1);
                        //g.FillRectangle(Brushes.Red, i, height-j, 1, 1);
                        //g.FillRectangle(Brushes.Chocolate, i, j, 1, 1);
                    }
                }
            }
            image.Refresh();
        }

        private void doFix()
        {
            /**
            edgesFixed = new float[edges.GetLength(0), edges.GetLength(1)];

            RangeImage rg = new RangeImage();
            rg.setHeight(400);
            rg.setWidth(400);
            rg.initializeRangeValues(400, 400);
            for(int i=0;i<400;i++){
                for(int j=0;j<400;j++){
                    if (i % 30 == 0 || j % 30 == 0)
                    {
                        rg.addValue(i * 400 + j, 10);
                    }
                }
            }
            rg.Write("c:\\Documents and Settings\\TOG000\\Desktop\\Grid.rangeimage");
            /**/
            /**
            double value;
            double y,newX;
            for (int i = 0; i < edges.GetLength(0); i++)
            {
                for (int j = 0; j < edges.GetLength(1); j++)
                {
                    if (edges[i, j] == 1)
                    {
                        value = Math.PI + Math.PI * ((float)j / edges.GetLength(0));
                        //y = Math.Abs(Math.Sin(value)*(Math.Abs((edges.GetLength(1)/2)-j))/5);
                        double bias = Math.Abs((float)i - edges.GetLength(1)/2) / edges.GetLength(1)/2;
                        y = Math.Abs(Math.Sin(value)*(edges.GetLength(0)*bias));
                        double angle = Math.Asin(y / i);
                        //edgesFixed[i, edges.GetLength(1) / 2 + (int)y] = 1;
                        newX = Math.Cos(angle) * i;
                        //newX = Math.Abs(Math.Cos(Math.Asin(y/i)) * i);
                        edgesFixed[(int)newX, j] = 1;
                        //edgesFixed[i+(int)newX, j] = 1;
                    }
                }
            }


            int cont = 0;
            for (int i = 0; i < edgesFixed.GetLength(0); i++)
            {
                for (int j = 0; j < edgesFixed.GetLength(1); j++)
                {
                    if (edgesFixed[i, j] == 1)
                    {
                        //Console.WriteLine("Edge at: ("+i+","+j+")");
                        cont++;
                        g.FillRectangle(Brushes.Red, i, height - j, 1, 1);
                        //g.FillRectangle(Brushes.Red, i, height-j, 1, 1);
                        //g.FillRectangle(Brushes.Chocolate, i, j, 1, 1);
                    }
                }
            }
            image.Refresh();
            /**/

            edgesFixed = new float[edges.GetLength(0), edges.GetLength(1)];
            double value, y, newX;
            for (int i = 0; i < edges.GetLength(0); i++)
            {
                for (int j = 0; j < edges.GetLength(1); j++)
                {
                    if (edges[i, j] == 1)
                    {
                        value = Math.PI + Math.PI * ((float)i / edges.GetLength(0));
                        y = Math.Sin(value) * 40;
                        newX = Math.Sqrt(Math.Pow(i, 2) - Math.Pow(y, 2));
                        edgesFixed[(int)newX, j] = 1;
                    }
                }
            }

            for (int i = 0; i < edgesFixed.GetLength(0); i++)
            {
                for (int j = 0; j < edgesFixed.GetLength(1); j++)
                {
                    if (edgesFixed[i, j] == 1)
                    {
                        g.FillRectangle(Brushes.Red, i, height - j, 1, 1);
                    }
                }
            }
            image.Refresh();

        }

        private void paintImage()
        {
            SolidBrush sb;

            Vector3[] points = rangeData.ConvertToPoints(30);

            //rangeData.
            /*
            for (int i = 0; i < width; i++)
            {
                for (int j = 0; j < height; j++)
                {
                    rangeData.addValue(i * height + j, rangeData.ConvertToPoint(j, i).Z);
                }
            }*/


            for (int i = 0; i < width; i++)
            {
                for (int j = 0; j < height; j++)
                {
                    int val = (int)Math.Min(180, Math.Abs((rangeData.GetCustom(i,j) / 81f) * 180f));
                    double conv = Math.PI / 180f;
                    int red = Convert.ToInt32(Math.Sin(val * conv) * 255);
                    int green = Convert.ToInt32(Math.Tan(val % 3 * conv) * 255);
                    int blue = Convert.ToInt32(Math.Sin(val % 4 * conv) * 255);
                    sb = new SolidBrush(Color.FromArgb(255, red,
                                                            green,
                                                            blue
                                                )
                                        );
                    g.FillRectangle(sb, i, height-j, 1, 1);
                    sb.Dispose();
                }
            }
        }

        private void image_MouseDown(object sender, System.Windows.Forms.MouseEventArgs e) 
        {
            if (rangeData != null)
            {
                statusText.Text = "Distance = " + rangeData.Get(e.X, height - e.Y);
                statusStrip1.Refresh();
            }
        }

        private void button3_Click_1(object sender, EventArgs e)
        {
            this.StartRecognition(m_Robot.RangeImages[m_Robot.RangeImages.Count-1]);
        }

        private void minRadius_TextChanged(object sender, EventArgs e)
        {
        
        }

        private void conf2_TextChanged(object sender, EventArgs e)
        {

        }

        /**
        private void button5_Click(object sender, EventArgs e)
        {
            //rangeData.Write("c:\\Documents and Settings\\tog000\\Desktop\\Image.RangeImage");
            /**
            BinaryWriter W = new BinaryWriter(File.Open("Y:\\image.scan", FileMode.Create));
            W.Write(rangeData.Width);
            W.Write(rangeData.Height);
            for (int i = 0; i < rangeData.Width-1; i++)
            {
                for (int j = 0; j < rangeData.Height-1; j++)
                {
                    W.Write(rangeData.Get(i, j));
                }

            }
            W.Close();
            /**
        }/**/

    }
}