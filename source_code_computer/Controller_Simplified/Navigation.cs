using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using Microsoft.Win32;
using System.Threading;
using System.IO;
using System.Drawing.Imaging;
using Microsoft.DirectX.Direct3D;
using Microsoft.DirectX;

namespace Controller
{
    public partial class Navigation : Form
    {
        bool m_PendingDrivingCommand;
        int updated = 0;
        ulong encoder0 = 0;
        ulong encoder1 = 0;
        int mode;
        float distance;
        float degrees;
        bool disabled = false;

        //For RangeImage Viewer
        const float MaxRange = 8192.0f;
        ScanParameters scanParams;


        public void Update()
        {
            MapCanvas.Invalidate();
        }

        public Navigation(Robot R)
        {

            InitializeComponent();

            m_Robot = R;

            if (m_Robot == null)
                m_Map = new Map(1024, 1024, 0.1f);
            else
                m_Map = m_Robot.Map;

            m_History = new List<PointF>();
            m_Scale = 0.05f;
            m_PathPen = new Pen(Color.Black, 2.0f);
            m_HistoryBrush = new SolidBrush(Color.Black);

            m_NoScanWaypointBrush = new SolidBrush(Color.Black);
            m_DirectionalScanWaypointBrush = new SolidBrush(Color.Green);
            m_FullScanWaypointBrush = new SolidBrush(Color.Blue);

            m_RobotBrush = new SolidBrush(Color.DarkGray);

            m_BlackPen = new Pen(Color.Black, 2.0f);
            m_Path = new Path();

            m_MinorGridPen = new Pen(Color.Silver, 1.0f);
            m_MinorGridPen.DashStyle = System.Drawing.Drawing2D.DashStyle.Dot;

            m_MajorGridPen = new Pen(Color.Black, 1.0f);
            m_MajorGridPen.DashStyle = System.Drawing.Drawing2D.DashStyle.Dash;

            m_IndexToMove = -1;
            m_MovingPoint = false;

            UpdateMap();

            if (m_Robot != null)
                ScrollToPosition(m_Robot.Telemetry.Position);
            //---------------------
            //For RangeImage Viewer
            //---------------------
                    m_DrawStyle = DrawStyle.TrianglesColorByDistance;
    
                    scanParams = new ScanParameters();
                    // these should probably be read in from a file or something...
                    scanParams.HRange = 0;
                    scanParams.HRes = 0;
                    scanParams.scanBottom = -45;
                    scanParams.scanLines = 360;
                    scanParams.scanRes = (float).25;
                    //InitGraphics();

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

            try
            {
                m_Device = new Device(0, DeviceType.Hardware, Canvas, CreateFlags.HardwareVertexProcessing, m_PresentParam);
            }
            catch
            {
                m_Device = new Device(0, DeviceType.Software, Canvas, CreateFlags.SoftwareVertexProcessing, new PresentParameters());
            }
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

            float MinHeight = float.MaxValue;
            float MaxHeight = float.MinValue;

            for (int i = 0; i < Points.Length; i++)
            {
                if (Points[i].Y < MinHeight)
                    MinHeight = Points[i].Y;
                if (Points[i].Y > MaxHeight)
                    MaxHeight = Points[i].Y;
            }

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

            toolStripStatusLabel1.Text = StatusStripText;
        }

        private bool m_IsResizing = false;
        private void Navigation_ResizeEnd(object sender, EventArgs e)
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

        private void Navigation_ResizeBegin(object sender, EventArgs e)
        {
            m_IsResizing = true;
        }

        private void Render()
        {
            if (m_IsResizing || m_Device == null)
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

        private void PrepareLIDAR(object sender, EventArgs e)
        {
            RegistryKey SettingsKey = Registry.CurrentUser.CreateSubKey("Software\\Nasa\\NasaBot");

            string X = (string)SettingsKey.GetValue("RangeImageLocationX", "");
            string Y = (string)SettingsKey.GetValue("RangeImageLocationY", "");

            if (!string.IsNullOrEmpty(X) && !string.IsNullOrEmpty(Y))
                Location = new Point(int.Parse(X), int.Parse(Y));

            if (m_Robot == null)
                AquireScan.Enabled = false;

            //InitGraphics();
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

            if (m_DrawStyle == DrawStyle.Flat)
            {
                int x = (int)(e.X / (double)Canvas.Width * m_RangeImage.Width);
                int y = m_RangeImage.Height - (int)(e.Y / (double)Canvas.Height * m_RangeImage.Height);

                if (x >= m_RangeImage.Width)
                    x = m_RangeImage.Width - 1;

                if (y >= m_RangeImage.Height)
                    y = m_RangeImage.Height - 1;

                if (x < 0)
                    x = 0;

                if (y < 0)
                    y = 0;


                float dist = m_RangeImage.Get(y, x);
                //float dist = 3;

                toolStripStatusLabel1.Text = StatusStripText + " Distance(" + x + "," + y + ") = " + dist;


            }

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

        private void Navigation_Activated(object sender, EventArgs e)
        {
            Render();
        }

        private void showParams_Click(object sender, EventArgs e)
        {
            m_Robot.Scan(scanParams);
        }

        private void Navigation_FormClosed(object sender, FormClosedEventArgs e)
        {
            RegistryKey SettingsKey = Registry.CurrentUser.CreateSubKey("Software\\Nasa\\NasaBot");
            SettingsKey.SetValue("RangeImageLocationX", Location.X.ToString());
            SettingsKey.SetValue("RangeImageLocationY", Location.Y.ToString());
        }

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


        protected override void OnClosing(CancelEventArgs e)
        {
            if (m_Robot != null)
            {
                Hide();
                e.Cancel = true;
            }
        }

        protected Robot m_Robot;
        protected Map m_Map;

        protected Bitmap m_HeightMapImage;
        protected Pen m_BlackPen;
        protected Pen m_MajorGridPen;
        protected Pen m_MinorGridPen;
        protected Pen m_PathPen;
        protected Brush m_HistoryBrush;

        //protected Brush m_WaypointBrush;
        protected Brush m_NoScanWaypointBrush;
        protected Brush m_DirectionalScanWaypointBrush;
        protected Brush m_FullScanWaypointBrush;

        protected Brush m_RobotBrush;

        protected Path m_Path;
        protected List<PointF> m_History;

        /* Meters per pixel */
        protected float m_Scale;

        /* Top-Left corner of screen in world coordinates */
        protected PointF m_Offset;
        /* Last mouse location in world coordinates */
        protected PointF m_LastMousePosition;

        protected bool m_AddingWaypoint, m_DeletingWaypoint, m_MovingWaypoint, m_MovingPoint;
        protected int m_IndexToMove;

        public Map Map
        {
            get { return m_Map; }
            set { m_Map = value; UpdateMap(); MapCanvas.Invalidate(); }
        }

        public Path Path
        {
            get { return m_Path; }
            set { m_Path = value; MapCanvas.Invalidate(); }
        }

        public void UpdateMap()
        {
            if (Map == null)
                return;

            m_HeightMapImage = new Bitmap(Map.Height, Map.Width);

            for (int i = 0; i < Map.Height; i++)
            {
                for (int j = 0; j < Map.Width; j++)
                {

                    float h = Map[i, j].Height;

                    h -= Map.MinimumHeightValue;
                    h /= (Map.MaximumHeightValue - Map.MinimumHeightValue) + 1;
                    h *= 0.66f;
                    h = 0.66f - h;
                    h *= (float)Math.PI * 2.0f;

                    if (Map[i, j].Flags > 0)
                        m_HeightMapImage.SetPixel(i, j, HSVtoRGB(h, 1.0f, 1.0f));
                    else
                        m_HeightMapImage.SetPixel(i, j, Color.White);

                }
            }
        }

        private PointF WorldToScreen(PointF P)
        {
            return new PointF((P.X - m_Offset.X + (Map.WorldWidth / 2.0f)) / m_Scale, (P.Y - m_Offset.Y + (Map.WorldHeight / 2.0f)) / m_Scale);
        }

        private SizeF WorldToScreenSize(SizeF S)
        {
            return new SizeF(S.Width / m_Scale, S.Height / m_Scale);
        }

        private PointF ScreenToWorld(PointF P)
        {
            return new PointF((P.X * m_Scale) + m_Offset.X - (Map.WorldWidth / 2.0f), (P.Y * m_Scale) + m_Offset.Y - (Map.WorldHeight / 2.0f));
        }

        private PointF ScreenToWorldSize(PointF P)
        {
            return new PointF(P.X * m_Scale, P.Y * m_Scale);
        }

        public void ScrollToPosition(PointF World)
        {
            PointF ViewportSize = ScreenToWorldSize(new PointF(MapCanvas.Width, MapCanvas.Height));

            m_Offset = new PointF(World.X + Map.WorldWidth / 2.0f - ViewportSize.X / 2.0f, World.Y + Map.WorldHeight / 2.0f - ViewportSize.Y / 2.0f);

            MapCanvas.Invalidate();
        }

        private void Zoom(int Direction)
        {
            const float ZoomSpeed = 1.1f;
            float Delta = 0.0f;
            if (Direction < 0)
                Delta = ZoomSpeed;
            if (Direction > 0)
                Delta = 1.0f / ZoomSpeed;

            PointF World = ScreenToWorld(new PointF(MapCanvas.Width / 2.0f, MapCanvas.Height / 2.0f));
            m_Scale *= Delta;
            ScrollToPosition(World);

            MapCanvas.Invalidate();
        }

        private int FindPoint(PointF CursorLocation)
        {
            for (int i = 0; i < m_Path.Count; i++)
            {
                PointF Point = WorldToScreen(m_Path[i].Waypoint);
                if (Math.Abs(Point.X - CursorLocation.X) < 10 && Math.Abs(Point.Y - CursorLocation.Y) < 10)
                    return i;
            }
            return -1;
        }


        private void Navigation_Load(object sender, EventArgs e)
        {
            if (m_Robot == null)
            {
                StopDrivingCommand.Enabled = false;
                DriveCommand.Enabled = false;
                ViewRobotCommand.Enabled = false;
            }
            else
            {
                Path = m_Robot.GetPath();
            }

            UpdateMap();
        }

        private void MapCanvas_Paint(object sender, PaintEventArgs e)
        {
            float Left = -Map.WorldWidth / 2.0f;
            float Right = Map.WorldWidth / 2.0f;

            float Top = -Map.WorldHeight / 2.0f;
            float Bottom = Map.WorldHeight / 2.0f;

            /* Draw height map */
            RectangleF FullMapRectangle = new RectangleF(WorldToScreen(new PointF(Left, Top)), WorldToScreenSize(Map.WorldSize));
            e.Graphics.DrawImage(m_HeightMapImage, FullMapRectangle);

            /* Draw minor gridlines */
            for (float i = Left; i < Right; i++)
            {
                e.Graphics.DrawLine(m_MinorGridPen, WorldToScreen(new PointF(Left, i)), WorldToScreen(new PointF(Right, i)));
                e.Graphics.DrawLine(m_MinorGridPen, WorldToScreen(new PointF(i, Top)), WorldToScreen(new PointF(i, Bottom)));
            }

            /* Draw major gridlines */
            for (float i = Left; i < Right; i += 10.0f)
            {
                e.Graphics.DrawLine(m_MajorGridPen, WorldToScreen(new PointF(Left, i)), WorldToScreen(new PointF(Right, i)));
                e.Graphics.DrawLine(m_MajorGridPen, WorldToScreen(new PointF(i, Top)), WorldToScreen(new PointF(i, Bottom)));
            }

            /* Draw outline */
            e.Graphics.DrawLine(m_PathPen, WorldToScreen(new PointF(Left, Top)), WorldToScreen(new PointF(Right, Top)));
            e.Graphics.DrawLine(m_PathPen, WorldToScreen(new PointF(Left, Top)), WorldToScreen(new PointF(Left, Bottom)));
            e.Graphics.DrawLine(m_PathPen, WorldToScreen(new PointF(Right, Bottom)), WorldToScreen(new PointF(Right, Top)));
            e.Graphics.DrawLine(m_PathPen, WorldToScreen(new PointF(Right, Bottom)), WorldToScreen(new PointF(Left, Bottom)));


            /* Draw path */
            e.Graphics.SmoothingMode = System.Drawing.Drawing2D.SmoothingMode.HighQuality;
            for (int i = 1; i < m_Path.Count; i++)
            {
                e.Graphics.DrawLine(m_PathPen, WorldToScreen(m_Path[i - 1].Waypoint), WorldToScreen(m_Path[i].Waypoint));
            }

            for (int i = 0; i < m_Path.Count; i++)
            {
                /* Draw point marker */
                RectangleF Marker = new RectangleF(WorldToScreen(m_Path[i].Waypoint), new SizeF(8.0f, 8.0f));
                Marker.Offset(-4.0f, -4.0f);

                if (m_Path[i].ScanMode == WaypointScanMode.None)
                    e.Graphics.FillRectangle(m_NoScanWaypointBrush, Marker);
                //if (m_Path[i].ScanMode == WaypointScanMode.Directional)
                //  e.Graphics.FillRectangle(m_DirectionalScanWaypointBrush, Marker);
                if (m_Path[i].ScanMode == WaypointScanMode.Full)
                    e.Graphics.FillRectangle(m_FullScanWaypointBrush, Marker);
            }

            if (m_Robot != null)
            {
                /* Draw Robot Marker, and viewport */
                PointF Robot = WorldToScreen(m_Robot.Telemetry.Position);
                e.Graphics.FillRectangle(m_RobotBrush, Robot.X - 8, Robot.Y - 8, 16, 16);

                RectangleF ViewRectangle = new RectangleF();
                ViewRectangle.Location = WorldToScreen(new PointF(m_Robot.Telemetry.Position.X - 8.0f, m_Robot.Telemetry.Position.Y - 8.0f));
                ViewRectangle.Size = new SizeF(WorldToScreenSize(new SizeF(16.0f, 16.0f)));

                float Angle0 = (float)Math.Acos(-m_Robot.Telemetry.PrimaryAxis.Height) * 180.0f / (float)Math.PI;
                float Angle1 = (float)Math.Asin(m_Robot.Telemetry.PrimaryAxis.Width) * 180.0f / (float)Math.PI;

                if (Angle1 < 0)
                    e.Graphics.DrawPie(m_PathPen, ViewRectangle, 220 - Angle0, 100);
                else
                    e.Graphics.DrawPie(m_PathPen, ViewRectangle, 220 + Angle0, 100);

                e.Graphics.SmoothingMode = System.Drawing.Drawing2D.SmoothingMode.Default;

                /* Draw History */
                for (int i = 0; i < m_Robot.TelemetryHistory.Count; i++)
                {
                    PointF P = WorldToScreen(m_Robot.TelemetryHistory[i].Position);
                    e.Graphics.FillRectangle(m_HistoryBrush, P.X - 2, P.Y - 2, 4, 4);
                }
            }

        }

        private int m_SelectedPoint = -1;
        private void MapCanvas_MouseClick(object sender, MouseEventArgs e)
        {
            MapCanvas.Focus();

            if (e.Button == MouseButtons.Right)
            {
                int i = FindPoint(e.Location);
                if (i != -1)
                {
//                    WaypointContextMenu.Show(MapCanvas, e.Location);
                    m_SelectedPoint = i;
                }

            }
            else if (m_AddingWaypoint)
            {
                if (m_Path != null)
                    m_Path.Add(new PathWaypoint(ScreenToWorld(e.Location)));

                MapCanvas.Invalidate();
            }
            else if (m_DeletingWaypoint)
            {
                int i = FindPoint(e.Location);

                if (i != -1)
                    m_Path.RemoveAt(i);

                MapCanvas.Invalidate();
            }
        }

        private void MapCanvas_MouseDown(object sender, MouseEventArgs e)
        {
            m_LastMousePosition = ScreenToWorldSize(e.Location);

            if (m_MovingWaypoint)
            {
                if (e.Button == MouseButtons.Left)
                {
                    m_IndexToMove = FindPoint(e.Location);
                    if (m_IndexToMove != -1)
                        m_MovingPoint = true;

                }
            }
        }

        private void MapCanvas_MouseUp(object sender, MouseEventArgs e)
        {
            if (m_MovingWaypoint && m_MovingPoint)
            {
                m_MovingPoint = false;
                MapCanvas.Invalidate();
            }

            MapCanvas.Cursor = Cursors.Default;
        }

        private void MapCanvas_MouseMove(object sender, MouseEventArgs e)
        {
            PointF CurrentMousePosition = ScreenToWorldSize(e.Location);

            PointF MouseDelta = new PointF(m_LastMousePosition.X - CurrentMousePosition.X, m_LastMousePosition.Y - CurrentMousePosition.Y);

            PointF CurrentWorldPosition = ScreenToWorld(e.Location);
            WorldPositionLabel.Text = "X: " + CurrentWorldPosition.X + "  Y: " + CurrentWorldPosition.Y;

            Point CurrentMapPosition;
            if (Map.ComputeMapCell(CurrentWorldPosition, out CurrentMapPosition))
                MapPositionLabel.Text = "X: " + CurrentMapPosition.X + "  Y: " + CurrentMapPosition.Y;

            if (e.Button == MouseButtons.Left && !m_MovingPoint)
            {
                MapCanvas.Cursor = Cursors.SizeAll;

                m_Offset.X += MouseDelta.X;
                m_Offset.Y += MouseDelta.Y;

                m_LastMousePosition = CurrentMousePosition;

                MapCanvas.Invalidate();
            }

            if (m_AddingWaypoint)
            {
                MapCanvas.Cursor = Cursors.Cross;
            }
            else if (m_DeletingWaypoint)
            {
                MapCanvas.Cursor = Cursors.Cross;
            }
            else if (m_MovingWaypoint)
            {
                MapCanvas.Cursor = Cursors.Cross;

                if (m_MovingPoint && (e.Button == MouseButtons.Left))
                {
                    if (m_IndexToMove >= 0)
                        m_Path[m_IndexToMove].Waypoint = ScreenToWorld(e.Location);

                    MapCanvas.Invalidate();
                }
            }
            else
            {
                MapCanvas.Cursor = Cursors.Default;
            }
        }

        private void AddWaypointCommand_Click(object sender, EventArgs e)
        {
            m_AddingWaypoint = !m_AddingWaypoint;
            AddWaypointCommand.Checked = !AddWaypointCommand.Checked;

            m_DeletingWaypoint = false;
            DeleteWaypointCommand.Checked = false;
            m_MovingWaypoint = false;
            MoveWaypointCommand.Checked = false;
        }

        private void DeleteWaypointCommand_Click(object sender, EventArgs e)
        {
            m_DeletingWaypoint = !m_DeletingWaypoint;
            DeleteWaypointCommand.Checked = !DeleteWaypointCommand.Checked;

            m_AddingWaypoint = false;
            AddWaypointCommand.Checked = false;
            m_MovingWaypoint = false;
            MoveWaypointCommand.Checked = false;
        }

        private void MoveWaypointCommand_Click(object sender, EventArgs e)
        {
            m_MovingWaypoint = !m_MovingWaypoint;
            MoveWaypointCommand.Checked = !MoveWaypointCommand.Checked;

            m_AddingWaypoint = false;
            AddWaypointCommand.Checked = false;
            m_DeletingWaypoint = false;
            DeleteWaypointCommand.Checked = false;
        }

        private void ZoomInCommand_Click(object sender, EventArgs e)
        {
            Zoom(+1);
        }

        private void ZoomOutCommand_Click(object sender, EventArgs e)
        {
            Zoom(-1);
        }

        private void ClearPathCommand_Click(object sender, EventArgs e)
        {
            m_Path.Clear();
            MapCanvas.Invalidate();
        }

        private void StopDrivingCommand_Click(object sender, EventArgs e)
        {
            m_Robot.Stop();
        }

        private void DriveCommand_Click(object sender, EventArgs e)
        {
            m_Robot.SetPath(m_Path);
        }

        private void ViewRobotCommand_Click(object sender, EventArgs e)
        {
            ScrollToPosition(m_Robot.Telemetry.Position);
        }

        private void FullScanToolStripMenuItem_Click(object sender, EventArgs e)
        {
            m_Path[m_SelectedPoint].ScanMode = WaypointScanMode.Full;
        }

        private void saveMapToolStripMenuItem_Click(object sender, EventArgs e)
        {
            SaveFileDialog.DefaultExt = "Map";
            SaveFileDialog.Filter = "Maps|*.Map";
            if (SaveFileDialog.ShowDialog() == DialogResult.OK)
            {
                Map.Write(SaveFileDialog.FileName);
            }

        }

        private void savePathToolStripMenuItem_Click(object sender, EventArgs e)
        {
            SaveFileDialog.DefaultExt = "Path";
            SaveFileDialog.Filter = "Paths|*.Path";

            if (SaveFileDialog.ShowDialog() == DialogResult.OK)
            {
                Path.Write(SaveFileDialog.FileName);
            }
        }

        private void loadMapToolStripMenuItem_Click(object sender, EventArgs e)
        {

            openFileDialog.DefaultExt = "Map";
            openFileDialog.Filter = "Maps|*.Map";

            if (openFileDialog.ShowDialog() == DialogResult.OK)
            {
                Map = Map.Read(openFileDialog.FileName);
            }

        }

        private void loadPathToolStripMenuItem_Click(object sender, EventArgs e)
        {
            openFileDialog.DefaultExt = "Path";
            openFileDialog.Filter = "Paths|*.Path";

            if (openFileDialog.ShowDialog() == DialogResult.OK)
            {
                Path = Path.Read(openFileDialog.FileName);
            }
        }

        private void NoScanToolStripMenuItem_Click(object sender, EventArgs e)
        {

        }

        private void DirectionalScanToolStripMenuItem_Click(object sender, EventArgs e)
        {
            //m_Path[m_SelectedPoint].ScanMode = WaypointScanMode.Directional;
        }

        private void stop()
        {
            m_PendingDrivingCommand = false;
            m_Robot.Stop();
            enableAll();
        }
        
        private void disableAll()
        {
            disabled = true;

            driveDistance.Enabled = false;
            turnDegrees.Enabled = false;
            speed.Enabled = false;
            groupBox1.Enabled = false;
            groupBox2.Enabled = false;
            groupBox3.Enabled = false;
            turnDegrees.Refresh();
            speed.Refresh();
            driveDistance.Refresh();
        }

        private void enableAll()
        {
            disabled = false;

            distanceMoved.Text = "0 cm";
            degreesTurned.Text = "0°";
            percentageMoved.Text = "100%";
            percentageTurned.Text = "100%";

            //driveDistance.Text = "";
            driveDistance.Enabled = true;
            //turnDistance.Text = "";
            turnDegrees.Enabled = true;
            speed.Enabled = true;
            groupBox1.Enabled = true;
            groupBox2.Enabled = true;
            groupBox3.Enabled = true;
            turnDegrees.Refresh();
            speed.Refresh();
            driveDistance.Refresh();
            button1.Text = "Move";
            button3.Text = "Rotate";
        }

        private void startMovement(int motionMode,float value)
        {
            mode = motionMode;

            encoder0 = m_Robot.Telemetry.TrackTicks0;
            encoder1 = m_Robot.Telemetry.TrackTicks1;

            disableAll();

            m_PendingDrivingCommand = true;
            if (mode == 1)
            {
                distance = value;
                m_Robot.Drive(distance, 0);
            }
            else
            {
                degrees = value;
                m_Robot.Turn(degrees);
            }
        }

        private void button1_Click(object sender, EventArgs e)
        {
            if (m_PendingDrivingCommand)
            {
                stop();
                button1.Text = "Move";
            }
            else
            {
                button1.Text = "Stop";
                startMovement(1, float.Parse(driveDistance.Text));
                groupBox1.Enabled = true;
                driveDistance.Enabled = false;
                driveDistance.Refresh();
            }
        }

        private void button3_Click(object sender, EventArgs e)
        {
            if (m_PendingDrivingCommand)
            {
                stop();
                button1.Text = "Rotate";
            }
            else
            {
                    Console.WriteLine(this.Controls.Find("turnDegrees",true)[0].Text);
                button3.Text = "Stop";
                //startMovement(2, float.Parse(turnDegrees.Text));
                groupBox2.Enabled = true;
                turnDegrees.Enabled = false;
                turnDegrees.Refresh();
              
            }
        }

        private void button2_Click(object sender, EventArgs e)
        {
            stop();
        }

        private void button4_Click(object sender, EventArgs e)
        {
            stop();
        }

        private void button5_Click(object sender, EventArgs e)
        {

        }

        private void robotStatus_Tick(object sender, EventArgs e)
        {
            
            if (m_PendingDrivingCommand
                && m_Robot.Telemetry.MotionStatus < 8
                && m_Robot.Telemetry.TrackSpeed0 == 0.0
                && m_Robot.Telemetry.TrackSpeed1 == 0.0)
            {
                updated++;
                if (updated > 4)
                {
                    m_PendingDrivingCommand = false;

                    if (disabled)
                    {
                        enableAll();
                    }

                    updated = 0;
                    mode = -1;
                }
            }
        }

        private void updatePosition_Tick(object sender, EventArgs e)
        {
            updateFields();
        }

        private void updateFields()
        {
            MapCanvas.Invalidate();
            switch(mode)
            {
                case 1:
                    distanceMoved.Text = Math.Round(((float)(m_Robot.Telemetry.TrackTicks0 - encoder0) / (13680))*10, 2).ToString()+" cm";
                    percentageMoved.Text = Math.Min(100, Math.Round((((m_Robot.Telemetry.TrackTicks0 - encoder0) / (distance * 137368)) * 100), 1)).ToString() + "%";
                break;
                case 2:
                    double actual = Math.Round(((float)(m_Robot.Telemetry.TrackTicks0 - encoder0) / (13680))*15, 2);
                    degreesTurned.Text = actual.ToString() + "°";
                    percentageTurned.Text = Math.Min(100,Math.Round((actual / degrees)*100,1)).ToString() + "%";
                break;
            }
        }

        private void driveDistance_KeyPress(object sender, MaskInputRejectedEventArgs e)
        {
            System.Windows.Forms.MessageBox.Show("asdfasdasdf");        
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

        private void LoadCommand_Click(object sender, EventArgs e)
        {
            if (openFileDialog1.ShowDialog() == DialogResult.OK)
            {
                m_RangeImage = RangeImage.Read(openFileDialog1.FileName);
                InitRangeImage();
                GenerateFlatImage();
                Canvas.Invalidate();

            }
        }

        private void AquireScan_Click(object sender, EventArgs e)
        {
            m_Robot.Scan(scanParams);
        }

        private void toolStripButton5_Click(object sender, EventArgs e)
        {
            m_Robot.StopScan();
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

        private void pointCloudToolStripMenuItem_Click(object sender, EventArgs e)
        {
            m_DrawStyle = DrawStyle.PointCloud;
            InitRangeImage();
            Render();
        }

        private void coloredByDistanceToolStripMenuItem_Click(object sender, EventArgs e)
        {
            m_DrawStyle = DrawStyle.TrianglesColorByHeight;
            InitRangeImage();
            Render();
        }

        private void flatImageToolStripMenuItem_Click(object sender, EventArgs e)
        {
            m_DrawStyle = DrawStyle.Flat;
            GenerateFlatImage();
        }

        private void toolStrip2_ItemClicked(object sender, ToolStripItemClickedEventArgs e)
        {

        }

        private void tableLayoutPanel5_Paint(object sender, PaintEventArgs e)
        {

        }

        private void toolStripButton2_Click(object sender, EventArgs e)
        {

        }

        private void toolStripButton12_Click(object sender, EventArgs e)
        {
            if (openFileDialog1.ShowDialog() == DialogResult.OK)
            {
                m_RangeImage = RangeImage.Read(openFileDialog1.FileName);
                InitRangeImage();
                GenerateFlatImage();
                Canvas.Invalidate();

            }
        }

        private void toolStripButton13_Click(object sender, EventArgs e)
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

        private void toolStripButton14_Click(object sender, EventArgs e)
        {
            m_Robot.Scan(scanParams);
        }

        private void toolStripButton15_Click(object sender, EventArgs e)
        {
            m_Robot.StopScan();
        }

        private void toolStripButton16_Click(object sender, EventArgs e)
        {
            ScanParametersViewer v = new ScanParametersViewer(ref scanParams);
            v.ShowDialog();
        }

        private void toolStripButton17_Click(object sender, EventArgs e)
        {
            scale *= 2;
            InitGraphics();
            InitRangeImage();
            Render();
        }

        private void toolStripButton18_Click(object sender, EventArgs e)
        {
            scale /= 2;
            InitGraphics();
            InitRangeImage();
            Render();
        }

        private void toolStripMenuItem5_Click(object sender, EventArgs e)
        {
            m_DrawStyle = DrawStyle.PointCloud;
            InitRangeImage();
            Render();
        }

        private void toolStripMenuItem6_Click(object sender, EventArgs e)
        {
            m_DrawStyle = DrawStyle.TrianglesColorByDistance;
            InitRangeImage();
            Render();
        }

        private void toolStripMenuItem7_Click(object sender, EventArgs e)
        {
            m_DrawStyle = DrawStyle.TrianglesColorByHeight;
            InitRangeImage();
            Render();
        }

        private void toolStripMenuItem8_Click(object sender, EventArgs e)
        {
            m_DrawStyle = DrawStyle.Flat;
            GenerateFlatImage();
        }



    }
}