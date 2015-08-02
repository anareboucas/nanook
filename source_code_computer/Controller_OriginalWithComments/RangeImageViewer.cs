using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Drawing.Imaging;
using System.Text;
using System.Windows.Forms;

using Microsoft.DirectX.Direct3D;
using Microsoft.DirectX;

using Microsoft.Win32;
using System.IO;

namespace Controller
{
    public partial class RangeImageViewer : Form
    {
        private Robot m_Robot;
        const float MaxRange = 8192.0f;

        ScanParameters scanParams;


        public RangeImageViewer(Robot Robot)
        {
            m_Robot = Robot;
            m_DrawStyle = DrawStyle.TrianglesColorByDistance;


            scanParams = new ScanParameters();
            // these should probably be read in from a file or something...
            scanParams.HRange = 0;
            scanParams.HRes = 0;
            scanParams.scanBottom = -45;
            scanParams.scanLines = 360;
            scanParams.scanRes = (float).25;
            InitializeComponent();
        }

        public enum DrawStyle
        {
            PointCloud,
            TrianglesColorByDistance,
            TrianglesColorByHeight,
            Flat
        }

        private DrawStyle m_DrawStyle;
        private RangeImage m_RangeImage;

        private Bitmap m_flatImage;

        private PresentParameters m_PresentParam;
        private Device m_Device;
        private VertexBuffer m_VertexBuffer;
        private IndexBuffer m_IndexBuffer;
        private int m_TriangleCount;

        private float m_Theta = 0.0f;
        private float m_Phi = 0.0f;
        //private float m_Distance = 7671.2f;
        private float m_Distance = 50f;

        private float scale = 1.0f;

        private Vector3 m_LookAt = new Vector3(0, 0, 0);


        private String StatusStripText;
        private Color HSVtoRGB(float hue, float saturation, float value)
        {
            if (saturation <= 0.0f)
                return Color.FromArgb((byte)value * 255, (byte)value * 255, (byte)value * 255);

            hue %= 2.0f * (float)Math.PI;
            while (hue < 0.0)
                hue += 2.0f * (float)Math.PI;

            hue /= (float)Math.PI / 3.0f;

            int i = (int)Math.Floor((double)hue);
            float f = hue - i;
            float p = value * (1.0f - saturation);
            float q = value * (1.0f - (saturation * f));
            float t = value * (1.0f - (saturation * (1.0f - f)));

            switch (i)
            {
                case 0:
                    return Color.FromArgb((byte)(value * 255), (byte)(t * 255), (byte)(p * 255));
                case 1:
                    return Color.FromArgb((byte)(q * 255), (byte)(value * 255), (byte)(p * 255));
                case 2:
                    return Color.FromArgb((byte)(p * 255), (byte)(value * 255), (byte)(t * 255));
                case 3:
                    return Color.FromArgb((byte)(p * 255), (byte)(q * 255), (byte)(value * 255));
                case 4:
                    return Color.FromArgb((byte)(t * 255), (byte)(p * 255), (byte)(value * 255));
                default:
                    return Color.FromArgb((byte)(value * 255), (byte)(p * 255), (byte)(q * 255));
            }
        }

        private void LoadCommand_Click(object sender, EventArgs e)
        {
            if (OpenFileDialog.ShowDialog() == DialogResult.OK)
            {
                m_RangeImage = RangeImage.Read(OpenFileDialog.FileName);
                InitRangeImage();
                GenerateFlatImage();
                Canvas.Invalidate();

            }
        }

        private void toolStripButton1_Click(object sender, EventArgs e)
        {
            if (SaveFileDialog.ShowDialog() == DialogResult.OK)
            {

                if (SaveFileDialog.FilterIndex == 2)
                {
                    Bitmap i = new Bitmap(Canvas.Width, Canvas.Height);

                    Graphics g = Graphics.FromImage(i);
                    // Remember Kids: You should never, ever, EVER put magic numbers in your code.
                    // it is a bad programming habit!
                    int x = Location.X + 4;
                    int y = Location.Y + 55;
                    g.CopyFromScreen(x, y, 0, 0, Canvas.Size);

                    i.Save(SaveFileDialog.FileName, ImageFormat.Jpeg);



                }
                else
                {
                    m_RangeImage.Write(SaveFileDialog.FileName);

                }
            }
        }


        public RangeImage RangeImage
        {
            get { return m_RangeImage; }
            set
            {
                m_RangeImage = value;

                if (m_DrawStyle == DrawStyle.Flat)
                {
                    GenerateFlatImage();
                }
                else
                {
                    InitRangeImage();
                    Render();
                }
            }
        }

        private void InitGraphics()
        {
            m_PresentParam = new PresentParameters();
            m_PresentParam.BackBufferCount = 1;
            m_PresentParam.Windowed = true;
            m_PresentParam.SwapEffect = SwapEffect.Discard;
            m_PresentParam.AutoDepthStencilFormat = DepthFormat.D16;
            m_PresentParam.EnableAutoDepthStencil = true;
            m_PresentParam.BackBufferHeight = Canvas.Height;
            m_PresentParam.BackBufferWidth = Canvas.Width;

            m_Device = new Device(0, DeviceType.Hardware, Canvas, CreateFlags.HardwareVertexProcessing, m_PresentParam);


            m_Device.RenderState.ZBufferEnable = true;
            m_Device.VertexFormat = CustomVertex.PositionNormalColored.Format;
            m_Device.RenderState.CullMode = Cull.None;
            m_Device.RenderState.Lighting = true;

            Material M = new Material();
            M.Specular = Color.White;
            M.Diffuse = Color.White;
            M.Ambient = Color.White;
            m_Device.Material = M;

            m_Device.Lights[0].Type = LightType.Directional;
            m_Device.Lights[0].Diffuse = Color.White;
            m_Device.Lights[0].Enabled = true;

            m_Device.RenderState.Ambient = Color.FromArgb(0x202020);
        }

        private int I(int i, int j)
        {
            return i * m_RangeImage.Width + j;
        }

        private Vector3 CalculateNormal(Vector3[] Points, int i, int j, int k)
        {
            Vector3 V0 = Points[k] - Points[i];
            Vector3 V1 = Points[j] - Points[i];

            return Vector3.Cross(V0, V1);
        }

        private void InitRangeImage()
        {
            if (m_RangeImage == null)
                return;

            m_Device.Transform.Projection = Matrix.PerspectiveFovLH((float)Math.PI / 4, (float)Width / Height, 0.001f, 10.0f * m_RangeImage.MaximumRange + 1.0f);

            m_VertexBuffer = new VertexBuffer(typeof(CustomVertex.PositionNormalColored), m_RangeImage.Width * m_RangeImage.Height, m_Device, Usage.WriteOnly, CustomVertex.PositionNormalColored.Format, Pool.Managed);

            Vector3[] Points = m_RangeImage.ConvertToPoints(float.MaxValue);

            //create a writer and open the file
            TextWriter tw = new StreamWriter("ladarOutput.txt");

            float MinHeight = float.MaxValue;
            float MaxHeight = float.MinValue;
            //TextWriter tw = new StreamWriter("c:\\Users\\tog000\\Desktop\\spheres.txt");
            for (int i = 0; i < Points.Length; i++)
            {
                //CODIGO MIO
                //Points[i].Z = (float)Math.Pow(Points[i].Z,2) / 2;
                //tw.WriteLine(Points[i].X*200);
                //tw.WriteLine(Points[i].Y * 200);
                //tw.WriteLine(Points[i].Z * 200);
                if (Points[i].Y < MinHeight)
                    MinHeight = Points[i].Y;
                if (Points[i].Y > MaxHeight)
                    MaxHeight = Points[i].Y;

                //Write coordinates
                tw.WriteLine(Points[i].X);
                tw.WriteLine(Points[i].Y);
                tw.WriteLine(Points[i].Z);
            }

            //Close stream
            tw.Close();

            CustomVertex.PositionNormalColored[] Vertices = (CustomVertex.PositionNormalColored[])m_VertexBuffer.Lock(0, LockFlags.None);
            for (int i = 0; i < m_RangeImage.Height; i++)
            {
                for (int j = 0; j < m_RangeImage.Width; j++)
                {
                    Vertices[I(i, j)].Position = Points[I(i, j)];

                    Vector3 Normal = new Vector3(0, 0, 0);

                    if (i > 0 && j > 0)
                        Normal += CalculateNormal(Points, I(i, j), I(i, j - 1), I(i - 1, j));
                    if (i > 0 && j < m_RangeImage.Width - 1)
                        Normal += CalculateNormal(Points, I(i, j), I(i - 1, j), I(i, j + 1));
                    if (i < m_RangeImage.Height - 1 && j < m_RangeImage.Width - 1)
                        Normal += CalculateNormal(Points, I(i, j), I(i, j + 1), I(i + 1, j));
                    if (i < m_RangeImage.Height - 1 && j > 0)
                        Normal += CalculateNormal(Points, I(i, j), I(i + 1, j), I(i, j - 1));

                    Normal.Normalize();

                    Vertices[I(i, j)].Normal = Normal;

                    if (m_DrawStyle == DrawStyle.TrianglesColorByDistance)
                    {
                        Vertices[I(i, j)].Color = HSVtoRGB(m_RangeImage[i, j] / m_RangeImage.MaximumRange * (float)Math.PI * 2.0f, 1, 1).ToArgb();
                    }
                    else if (m_DrawStyle == DrawStyle.TrianglesColorByHeight)
                    {
                        float h = Points[I(i, j)].Y;
                        h -= MinHeight;
                        h /= (MaxHeight - MinHeight);
                        h *= (float)Math.PI * 2.0f;
                        Vertices[I(i, j)].Color = HSVtoRGB(h, 1, 1).ToArgb();
                    }

                    /*
                    int Red = (int)((Normal.X + 1.0) * 127.0f);
                    int Green = (int)((Normal.Y + 1.0) * 127.0f);
                    int Blue = (int)((Normal.Z + 1.0) * 127.0f);

                    Vertices[I(i, j)].Color = Color.FromArgb(Red, Green, Blue).ToArgb();
                    */
                }
            }
            m_VertexBuffer.Unlock();

            m_IndexBuffer = new IndexBuffer(typeof(int), (m_RangeImage.Width - 1) * (m_RangeImage.Height - 1) * 6, m_Device, Usage.WriteOnly, Pool.Managed);
            int[] Indices = (int[])m_IndexBuffer.Lock(0, LockFlags.None);
            int k = 0;
            for (int i = 0; i < m_RangeImage.Height - 1; i++)
            {
                for (int j = 0; j < m_RangeImage.Width - 1; j++)
                {
                    /*
                    float MinD;
                    float MaxD;

                    MinD = Math.Min(m_RangeImage[i, j], Math.Min(m_RangeImage[i, j + 1], m_RangeImage[i + 1, j]));
                    MaxD = Math.Max(m_RangeImage[i, j], Math.Max(m_RangeImage[i, j + 1], m_RangeImage[i + 1, j]));

                    if ((MaxD - MinD) < 1000)
                    */
                    Indices[k++] = I(i, j);
                    Indices[k++] = I(i, j + 1);
                    Indices[k++] = I(i + 1, j);
                    /*

                    MinD = Math.Min(m_RangeImage[i, j + 1], Math.Min(m_RangeImage[i + 1, j + 1], m_RangeImage[i + 1, j]));
                    MaxD = Math.Max(m_RangeImage[i, j + 1], Math.Max(m_RangeImage[i + 1, j + 1], m_RangeImage[i + 1, j]));

                    if ((MaxD - MinD) < 1000)
                    */
                    Indices[k++] = I(i, j + 1);
                    Indices[k++] = I(i + 1, j + 1);
                    Indices[k++] = I(i + 1, j);

                }
            }
            m_IndexBuffer.Unlock();
            m_TriangleCount = k / 3;

            StatusStripText = m_RangeImage.AquiredOn.ToString() + " Resolution: (" + m_RangeImage.Width + " x " + m_RangeImage.Height + ")  Scanner Location: (" + m_RangeImage.ScannerLocation.X + "," + m_RangeImage.ScannerLocation.Y + ")  Scanner Orientation: (" + m_RangeImage.ScannerPrimaryAxis.X + "," + m_RangeImage.ScannerPrimaryAxis.Y + ")";

            UpdatedOn.Text = StatusStripText;
        }

        private void Render()
        {
            if (m_IsResizing)
                return;

            Vector3 LookAt = new Vector3(m_Distance * (float)Math.Sin(m_Theta) * (float)Math.Cos(m_Phi), m_Distance * (float)Math.Sin(m_Phi), m_Distance * (float)Math.Cos(m_Theta) * (float)Math.Cos(m_Phi));
            Vector3 Left = new Vector3((float)Math.Cos(m_Theta), 0, (float)-Math.Sin(m_Theta));
            Vector3 Up = Vector3.Cross(Left, -LookAt);
            //Matrix scaler = Matrix.Scaling(scale, scale, scale);
            Matrix la = Matrix.LookAtLH(new Vector3(0, 0, 0), LookAt, Up);
            Matrix m = Matrix.Scaling(1, 1, scale);
            //Matrix m = Matrix.Scaling(1, 1, 1);  
            m_Device.Transform.View = Matrix.Multiply(m, la);
            //m_Device.Transform.View = Matrix.LookAtLH(new Vector3(0, 0, 0), LookAt, Up);
            m_Device.Lights[0].Direction = LookAt;

            m_Device.Lights[0].Update();

            m_Device.Clear(ClearFlags.Target | ClearFlags.ZBuffer, Color.Black, 1.0f, 0);
            m_Device.BeginScene();

            if (m_VertexBuffer != null)
            {
                m_Device.SetStreamSource(0, m_VertexBuffer, 0);

                if (m_DrawStyle == DrawStyle.TrianglesColorByDistance || m_DrawStyle == DrawStyle.TrianglesColorByHeight)
                {
                    m_Device.Indices = m_IndexBuffer;
                    m_Device.DrawIndexedPrimitives(PrimitiveType.TriangleList, 0, 0, m_RangeImage.Width * m_RangeImage.Height, 0, m_TriangleCount);
                }
                else
                {
                    m_Device.DrawPrimitives(PrimitiveType.PointList, 0, m_RangeImage.Width * m_RangeImage.Height);
                }
            }

            m_Device.EndScene();
            m_Device.Present();
        }

        private void Canvas_Paint(object sender, PaintEventArgs e)
        {
            if (m_DrawStyle == DrawStyle.Flat)
            {
                Graphics g = Canvas.CreateGraphics();
                g.DrawImage(m_flatImage, new Rectangle(0, 0, Canvas.Width, Canvas.Height));

            }
            else
            {
                Render();
            }
        }

        private void LADARViewer_Load(object sender, EventArgs e)
        {
            RegistryKey SettingsKey = Registry.CurrentUser.CreateSubKey("Software\\Nasa\\NasaBot");

            string X = (string)SettingsKey.GetValue("RangeImageLocationX", "");
            string Y = (string)SettingsKey.GetValue("RangeImageLocationY", "");

            if (!string.IsNullOrEmpty(X) && !string.IsNullOrEmpty(Y))
                Location = new Point(int.Parse(X), int.Parse(Y));

            if (m_Robot == null)
                AquireScan.Enabled = false;

            InitGraphics();
        }

        private Point m_MouseDown;
        private float m_StartTheta;
        private float m_StartPhi;

        private void Canvas_MouseDown(object sender, MouseEventArgs e)
        {
            m_MouseDown = e.Location;
            m_StartPhi = m_Phi;
            m_StartTheta = m_Theta;
        }

        private void Canvas_MouseMove(object sender, MouseEventArgs e)
        {

            //if (m_DrawStyle == DrawStyle.Flat)
            //{
                int x = (int)(e.X / (double)Canvas.Width * m_RangeImage.Width);
                int y = m_RangeImage.Height-(int)(e.Y / (double)Canvas.Height * m_RangeImage.Height);

                if (x >= m_RangeImage.Width)
                    x = m_RangeImage.Width-1;

                if (y >= m_RangeImage.Height)
                    y = m_RangeImage.Height-1;

                if (x < 0)
                    x = 0;
                
                if (y < 0)
                    y = 0;


                float dist = m_RangeImage.Get(y, x);
                //float dist = 3;

                UpdatedOn.Text = StatusStripText+" Distance("+x+","+y+") = "+dist;


            //}
            
            if (e.Button == MouseButtons.Left)
            {
                float Dx = m_MouseDown.X - e.X;
                float Dy = m_MouseDown.Y - e.Y;

                m_Theta = m_StartTheta + Dx * 0.001f;
                m_Phi = m_StartPhi - Dy * 0.001f;

                Render();
            }
            

        }

        private void Canvas_MouseUp(object sender, MouseEventArgs e)
        {

        }

        private void pointCloudToolStripMenuItem_Click(object sender, EventArgs e)
        {
            m_DrawStyle = DrawStyle.PointCloud;
            InitRangeImage();
            Render();
        }

        private void depthMapToolStripMenuItem_Click(object sender, EventArgs e)
        {
            m_DrawStyle = DrawStyle.TrianglesColorByDistance;
            InitRangeImage();
            Render();
        }

        private void RangeImageViewer_Activated(object sender, EventArgs e)
        {
            Render();
        }

        private void toolStripButton2_Click(object sender, EventArgs e)
        {
            m_Robot.Scan(scanParams);
        }

        private void RangeImageViewer_FormClosed(object sender, FormClosedEventArgs e)
        {
            RegistryKey SettingsKey = Registry.CurrentUser.CreateSubKey("Software\\Nasa\\NasaBot");
            SettingsKey.SetValue("RangeImageLocationX", Location.X.ToString());
            SettingsKey.SetValue("RangeImageLocationY", Location.Y.ToString());
        }

        private void coloredByDistanceToolStripMenuItem_Click(object sender, EventArgs e)
        {
            m_DrawStyle = DrawStyle.TrianglesColorByHeight;
            InitRangeImage();
            Render();
        }

        private bool m_IsResizing = false;
        private void RangeImageViewer_ResizeEnd(object sender, EventArgs e)
        {
            if (m_DrawStyle != DrawStyle.Flat)
            {
                m_IsResizing = false;
                InitGraphics();
                InitRangeImage();
                Render();
            }
            else
            {
                GenerateFlatImage();

            }
        }

        private void RangeImageViewer_ResizeBegin(object sender, EventArgs e)
        {
            m_IsResizing = true;
        }

        private void toolStripButton2_Click_1(object sender, EventArgs e)
        {

            ScanParametersViewer v = new ScanParametersViewer(ref scanParams);
            v.ShowDialog();


        }

        private void toolStripButton3_Click(object sender, EventArgs e)
        {
            scale *= 2;
            InitGraphics();
            InitRangeImage();
            Render();
        }

        private void toolStripButton4_Click(object sender, EventArgs e)
        {
            scale /= 2;
            InitGraphics();
            InitRangeImage();
            Render();
        }

        private void toolStripButton5_Click(object sender, EventArgs e)
        {
            m_Robot.StopScan();
        }

        private void flatImageToolStripMenuItem_Click(object sender, EventArgs e)
        {
            m_DrawStyle = DrawStyle.Flat;
            GenerateFlatImage();
        }
        private void GenerateFlatImage()
        {

            if (m_RangeImage == null)
            {
                m_flatImage = new Bitmap(Canvas.Width, Canvas.Height);
                Graphics.FromImage(m_flatImage).FillRectangle(Brushes.Black, new Rectangle(0, 0, Canvas.Width, Canvas.Height));
                return;
            }
            m_flatImage = new Bitmap(m_RangeImage.Width, m_RangeImage.Height);
            // first loop over the range image, and compute the min and max ranges

            float minRange = m_RangeImage.MaximumRange;
            float maxRange = 0;

            for (int i = 0; i < m_RangeImage.Width; i++)
            {
                for (int j = 0; j < m_RangeImage.Height; j++)
                {
                    if (m_RangeImage[j, i] < minRange)
                        minRange = m_RangeImage[j, i];
                    if ((m_RangeImage[j, i] > maxRange) && (m_RangeImage[j, i] < m_RangeImage.MaximumRange))
                        maxRange = m_RangeImage[j, i];
                }
            }

            float range = maxRange - minRange;


            for (int i = 0; i < m_RangeImage.Width; i++)
            {
                for (int j = 0; j < m_RangeImage.Height; j++)
                {
                    if (m_RangeImage[j, i] < m_RangeImage.MaximumRange)
                    {
                        m_flatImage.SetPixel(i, m_RangeImage.Height - j - 1, HSVtoRGB((float)((m_RangeImage[j, i] - minRange) / range * 5.5), 1f, .5f));
                    }
                    else
                    {
                        m_flatImage.SetPixel(i, m_RangeImage.Height - j - 1, Color.Black);
                    }
                }
            }



        }

        private void toolStripDropDownButton1_Click(object sender, EventArgs e)
        {

        }



    }
}
