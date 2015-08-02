using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Drawing.Drawing2D;

namespace Controller
{
  public partial class NavigationBAK : Form
  {

    public void Update()
    {
      MapCanvas.Invalidate();
    }

    public NavigationBAK(Robot R)
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
      m_MinorGridPen.DashStyle = DashStyle.Dot;

      m_MajorGridPen = new Pen(Color.Black, 1.0f);
      m_MajorGridPen.DashStyle = DashStyle.Dash;

      m_IndexToMove = -1;
      m_MovingPoint = false;

      UpdateMap();

      if (m_Robot != null)
        ScrollToPosition(m_Robot.Telemetry.Position);

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
      set { m_Path = value; MapCanvas.Invalidate();  }
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

          if (Map[i,j].Flags > 0)
            m_HeightMapImage.SetPixel(i, j, HSVtoRGB(h, 1.0f, 1.0f) );
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
      for (float i = Left; i < Right; i+= 10.0f)
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
      e.Graphics.SmoothingMode = SmoothingMode.HighQuality;
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

        e.Graphics.SmoothingMode = SmoothingMode.Default;

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
          WaypointContextMenu.Show(MapCanvas, e.Location);
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
      OpenFileDialog.DefaultExt = "Map";
      OpenFileDialog.Filter = "Maps|*.Map";

      if (OpenFileDialog.ShowDialog() == DialogResult.OK)
      {
        Map = Map.Read(OpenFileDialog.FileName);
      }

    }

    private void loadPathToolStripMenuItem_Click(object sender, EventArgs e)
    {
      OpenFileDialog.DefaultExt = "Path";
      OpenFileDialog.Filter = "Paths|*.Path";

      if (OpenFileDialog.ShowDialog() == DialogResult.OK)
      {
        Path = Path.Read(OpenFileDialog.FileName);
      }
    }

    private void NoScanToolStripMenuItem_Click(object sender, EventArgs e)
    {
      m_Path[m_SelectedPoint].ScanMode = WaypointScanMode.None;
    }

    private void DirectionalScanToolStripMenuItem_Click(object sender, EventArgs e)
    {
      //m_Path[m_SelectedPoint].ScanMode = WaypointScanMode.Directional;
    }

    private void FullScanToolStripMenuItem_Click(object sender, EventArgs e)
    {
      m_Path[m_SelectedPoint].ScanMode = WaypointScanMode.Full;
    }

    private void toolStrip1_ItemClicked(object sender, ToolStripItemClickedEventArgs e)
    {

    }



  }
}