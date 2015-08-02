using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Drawing.Drawing2D;
using System.Threading;
using Microsoft.DirectX;

namespace Controller
{
    public partial class Control : Form   //: is equivalent to extended. Control will acquire the characteristics of Form (subclass)
    {
        //variavel q comeca com m eh um atributo de classe (variaveis que valem na classe inteira)

        SphereRecognitionView m_SphereRecognitionView;
        RangeImageViewer m_RangeImageViewer;
        protected Robot m_Robot;
        System.Windows.Forms.Timer movingTimer;
        /**
         * @brief Constant required to convert an angle from degrees to radians
        */
        Double deg2rad = (Math.PI / 180);

        int angleee = 360;

        /**
         * @brief Constructor.
         * @param R The robot to process
         */
        public Control(Robot R)
        {
            m_Robot = R;   //construtor

            InitializeComponent();

            /**
             * @brief Stop the updating of the fields
             */
            updateFieldsTimer.Stop();
            //statusTimer.Stop();

            /** @brief Prepare the delay for when we send the command,
            *but it takes a few seconds for the robot to start moving
            *That's why we dont want to check if its done until it actually starts moving
             */
            movingTimer = new System.Windows.Forms.Timer();
            movingTimer.Interval = 500;
            movingTimer.Tick += new EventHandler(verifyMoving);

            //Fire up the drawing and all other 
            //things related to the Map/Path 
            //Autonomous Navigation
            initAutonomousNav();

            /**
             * @brief Set up all LIDAR related stuff
             */
            initLidarScanning();
        }

        //************************************************************//
        //        Here starts code that is specific for the           //
        //                   NAVIGATION CONTROLS                      //
        //************************************************************//
        #region NAVIGATION RELATED STUFF

        /**
         * @brief Values that represent the actions (move, turn, idle, and external).
        */

        public enum Action
        {
            Move,
            Turn,
            Idle,
            External
        }

        /**
         * @brief The running action
         */
        public Action action;
        /**
         * @brief To internally keep track of the state of the actions
         */
        private bool isActionRunning = false;
        /**
         * @brief Global distance variable
         */
        private float distance;
        /**
         * @brief Global angle variable
         */
        private float angle;

        private float encoder0;
        private float encoder1;
        const int encPerCM = 1373;

        /**
         * @brief To make sure we are running before, we check isMoving
         */
        bool isMoving = false;

        /**
         * @brief Executes the action operation.
         * @param newAction An Action
         * @param value A float
        */
        public void doAction(Action newAction, float value)
        {
            action = newAction;
            switch (action)
            {
                // Set the distance and reset the encoder values to the actual ones
                case Action.Move:
                    distance = value;
                    encoder0 = m_Robot.Telemetry.TrackTicks0;
                    encoder1 = m_Robot.Telemetry.TrackTicks1;

                    // Start updating the fields
                    updateFieldsTimer.Start();

                    isActionRunning = true;

                    movingTimer.Start();

                    //Disable the value field
                    groupBox2.Enabled = false;
                    driveDistance.Enabled = false;

                    // Execute!
                    m_Robot.Drive(distance, 0);
                    break;
                // brief Set the angle and reset the encoder values to the actual ones

                case Action.Turn:
                    angle = value;
                    encoder0 = m_Robot.Telemetry.TrackTicks0;
                    encoder1 = m_Robot.Telemetry.TrackTicks1;

                    // Start updating the fields
                    updateFieldsTimer.Start();

                    isActionRunning = true;

                    movingTimer.Start();

                    // Disable the value field
                    groupBox1.Enabled = false;
                    turnAngle.Enabled = false;

                    // Execute!
                    m_Robot.Turn(angle);
                    break;
            }
        }

        public void updateFields()
        {
            switch (action)
            {
                case Action.Move:
                    //Update with (ticks since we started moving)/(target ticks)
                    percentageMoved.Text = Math.Abs(Math.Round((m_Robot.Telemetry.TrackTicks0 - encoder0) / (distance * encPerCM))).ToString() + " %";
                    distanceMoved.Text = Math.Abs(Math.Round((m_Robot.Telemetry.TrackTicks0 - encoder0) / encPerCM, 1)).ToString() + " cm";
                    break;
                case Action.Turn:
                    //Update with (ticks since we started moving)/(target ticks)                                //MAGIC NUMBER!
                    double actual = Math.Abs(Math.Round((m_Robot.Telemetry.TrackTicks0 - encoder0) / encPerCM * 1.63));
                    percentageTurned.Text = Math.Abs(Math.Round(actual * 100 / angle)).ToString() + " %";
                    degreesTurned.Text = actual.ToString() + "°";
                    break;
            }
        }

        /**
         * @brief Updates the telemetry.
        */
        public void updateTelemetry()
        {
            if (m_Robot == null || !m_Robot.Connected)
            {
                tableLayoutPanel7.BackColor = Color.FromArgb(255, 192, 192);
                Host.Text = "-";
                connectionStatus.Text = "Disconnected";
                ConnectionLatency.Text = "-";
                LastTelemetry.Text = "-";
                return;
            }
            else
            {
                tableLayoutPanel7.BackColor = Color.FromArgb(192, 255, 192);
            }

            Host.Text = m_Robot.Host.ToString();
            connectionStatus.Text = m_Robot.Connected ? "Connected" : "Disconnected";

            TimeSpan Latency = m_Robot.GetRoundTripLatency();
            if (Latency.TotalMilliseconds == 0)
                ConnectionLatency.Text = "< 1 ms";
            else
                ConnectionLatency.Text = Latency.TotalMilliseconds.ToString("0.0") + " ms";

            LastTelemetry.Text = m_Robot.LastTelemetryUpdate.ToString(); //time da ultima telemetria em formato de string
            TimeSpan TelemetryAge = DateTime.Now - m_Robot.LastTelemetryUpdate;  //to know how long from the last telemetry
            if (TelemetryAge.TotalSeconds > 30)
            {
                LastTelemetry.ForeColor = Color.Red;   //Color red means the robot is not receiving telemetry for a long time
            }
            else
            {
                LastTelemetry.ForeColor = Color.Black;
            }
        }

        /**
        * @brief Event handler. Called by telemetryTimer for tick events.
        * @param sender An object - Source of the event
        * @param e An EventArgs - Event Information
        */
        private void telemetryTimer_Tick(object sender, EventArgs e)
        {
            updateTelemetry();  //conectado ou desconectado
        }

        /**
        * @brief Event handler. Called by updateFieldsTimer for tick events.
        * @param sender An object - Source of the event
        * @param e An EventArgs - Event Information
        */
        private void updateFieldsTimer_Tick(object sender, EventArgs e)
        {
            updateFields();    //move or turn
        }

        /**
         * @brief Updates the Position
        */
        private void updatePosition()
        {
            Location.Text = "(" + m_Robot.Telemetry.Position.X.ToString("0.000") + ", " + m_Robot.Telemetry.Position.Y.ToString("0.000") + ")";
            Orientation.Text = "(" + m_Robot.Telemetry.PrimaryAxis.Width.ToString("0.000") + ", " + m_Robot.Telemetry.PrimaryAxis.Height.ToString("0.000") + ")";  //nao entendi q orientacao eh essa
            Heading.Text = m_Robot.Telemetry.Heading.ToString("0.0");  //descobri o q eh o heading depois

            TrackLeftEncoder.Text = m_Robot.Telemetry.TrackTicks0.ToString();
            TrackRightEncoder.Text = m_Robot.Telemetry.TrackTicks1.ToString();

            TrackLeftSpeed.Text = m_Robot.Telemetry.TrackSpeed0.ToString();
            TrackRightSpeed.Text = m_Robot.Telemetry.TrackSpeed1.ToString();
        }

        /**
         * @brief Event handler. Called by positionTimer for tick events.
         * @param sender An object - Source of the event
         * @param e An EventArgs - Event Information
         */
        private void positionTimer_Tick(object sender, EventArgs e)
        {
            updatePosition();
        }

        //This is very nasty, but the robot takes a few 
        //seconds to start, and I rather do it independent 
        //from the main timers in this class

        /**
         * @brief Verify moving.
         * @param myObject An Object
         * @param myEventArgs An EventArgs
        */
        private void verifyMoving(Object myObject, EventArgs myEventArgs)
        {
            if (m_Robot.Telemetry.TrackSpeed0 != 0.0 && m_Robot.Telemetry.TrackSpeed1 != 0.0)
            {
                isMoving = true;
                movingTimer.Stop();
            }
        }

        /**
         * @brief Robot Status
         */
        public void robotStatus()
        {
            //In case somebody else is moving it
            if (m_Robot != null && m_Robot.Telemetry.MotionStatus > 8 && action.Equals(Action.Idle) && !isActionRunning)
            {
                action = Action.External;
                groupBox1.Enabled = false;
                groupBox2.Enabled = false;
                isMoving = true;
                return;
            }
            else if (isMoving && m_Robot.Telemetry.MotionStatus <= 8 && !action.Equals(Action.Idle))
            {
                //Make multiple IF's to keep the code readeable
                if (m_Robot.Telemetry.TrackSpeed0 == 0 && m_Robot.Telemetry.TrackSpeed1 == 0)
                {
                    action = Action.Idle;
                    isActionRunning = false;
                    updateFieldsTimer.Stop();
                    enableAll();
                    isMoving = false;
                }
            }
        }

         /**
         * @brief Event handler. Called by statusTimer for tick events.
         * @param sender An object - Source of the event
         * @param e An EventArgs - Event Information
         */
        private void statusTimer_Tick(object sender, EventArgs e)
        {
            robotStatus();
        }

        /**
         * @brief Enable all fields (after succesful run or stop command)
        */
        public void enableAll()
        {
            groupBox1.Enabled = true;
            groupBox2.Enabled = true;

            driveDistance.Enabled = true;
            driveDistance.Refresh();

            turnAngle.Enabled = true;
            turnAngle.Refresh();

            //Restore the text
            button1.Text = "Move";
            button3.Text = "Turn";

            distanceMoved.Text = "0 cm";
            percentageMoved.Text = "0 %";
            degreesTurned.Text = "0°";
            percentageTurned.Text = "0 %";
        }

        /**
         * @brief Stops the robot.
        */
        public void stop()
        {
            m_Robot.Stop();
            isActionRunning = false;
            action = Action.Idle;
            movingTimer.Stop();
            isMoving = false;
            enableAll();
        }

        //Region
        #region STOP BUTTONS

        /**
        * @brief Event handler. Called by button2 for click events.
        * @param sender An object - Source of the event
        * @param e An EventArgs - Event Information
        */
        private void button2_Click(object sender, EventArgs e)
        {
            stop();
        }

        /**
        * @brief Event handler. Called by button4 for click events.
        * @param sender An object - Source of the event
        * @param e An EventArgs - Event Information
        */
        private void button4_Click(object sender, EventArgs e)
        {
            stop();
        }

        #endregion

        /**
        * @brief Event handler. Called by button1 for click events.
        * @param sender An object - Source of the event
        * @param e An EventArgs - Event Information
        */
        private void button1_Click(object sender, EventArgs e)
        {
            if (isActionRunning)
            {
                stop();
            }
            else
            {
                button1.Text = "Stop";
                button1.Refresh();
                doAction(Action.Move, float.Parse(driveDistance.Text));
            }
        }

        /**
        * @brief Event handler. Called by button3 for click events.
        * @param sender An object - Source of the event
        * @param e An EventArgs - Event Information
        */
        private void button3_Click(object sender, EventArgs e)
        {
            if (isActionRunning)
            {
                stop();
            }
            else
            {
                float r = -float.Parse(turnAngle.Text);
                button3.Text = "Stop";
                button3.Refresh();
                doAction(Action.Turn, r);
            }
        }

        /**
        * @brief Event handler. Called by RefreshCanvas for tick events.
        * @param sender An object - Source of the event
        * @param e An EventArgs - Event Information
        */
        private void RefreshCanvas_Tick(object sender, EventArgs e)
        {
            MapCanvas.Invalidate();
        }

        #endregion

        //************************************************************//
        //        Here starts code that is specific for the           //
        //                  AUTONOMOUS NAVIGATION tab                 //
        //************************************************************//
        #region AUTONOMOUS NAVIGATION STUFF

        //**************************************//
        //  INITIALIZE VARIABLES & INIT SCRIPT  //
        //**************************************//
        #region INITIALIZE VARIABLES/INIT SCRIPT
        protected Map m_Map;
        protected Path m_Path;
        protected List<PointF> m_History;
        protected Bitmap m_HeightMapImage;

        /**
         * @brief List of all the detected Spheres in the last scan
         */
        private List<Sphere> detectedSpheres;

        private List<PointF> spherePositions = new List<PointF>();
        private List<float> sphereRadii = new List<float>();

        HeightMap hm;

        private bool[,] heightMap = new bool[300, 400];

        /**
         * @brief List of all the detected Drones in the last scan
         */
        private List<Drone> detectedDrones = new List<Drone>();

        private int selectedDrone = -1;

        /**
         * @brief Meters per pixel
         */
        protected float m_Scale;

        /**
         * @brief Top-Left corner of screen in world coordinates
         */
        protected PointF m_Offset;

        /*
         * @brief Last mouse location in world coordinates
         */
        protected PointF m_LastMousePosition;

        //For painting the map
        protected Pen m_BlackPen;
        protected Pen m_MajorGridPen;
        protected Pen m_MinorGridPen;
        protected Pen m_PathPen;
        protected Brush m_HistoryBrush;
        //More brushes
        protected Brush m_NoScanWaypointBrush;
        protected Brush m_DirectionalScanWaypointBrush;
        protected Brush m_FullScanWaypointBrush;
        //Even more!
        protected Brush m_RobotBrush;

        //For the PATH events
        protected bool m_AddingWaypoint, m_DeletingWaypoint, m_MovingWaypoint, m_MovingPoint;

        //More stuffffffff
        protected int m_IndexToMove;

        /**
        * @brief Initialises the autonomous navigation.
        */
        public void initAutonomousNav()
        {
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
        }
        #endregion
        //**********************************//
        //  PUBLIC ACCESORS FOR MAP & PATH  //
        //**********************************//
        #region public acc Map & Path

        /**
        * @brief Gets or sets the map.
        */
        public Map Map
        {
            get { return m_Map; }
            set { m_Map = value; UpdateMap(); MapCanvas.Invalidate(); }
        }

        /**
         * @brief Gets or sets the full pathname of the file.
        */
        public Path Path
        {
            get { return m_Path; }
            set { m_Path = value; MapCanvas.Invalidate(); }
        }
        #endregion

        //***********************************//
        //  MapCanvas related event handles  //
        //***********************************//
        #region MapCanvas related event handles
        private int m_SelectedPoint = -1;

        /**
        * @brief Event handler. Called by MapCanvas for mouse click events.
        * @param sender An object - Source of the event
        * @param e An EventArgs - Mouse Event Information
        */
        private void MapCanvas_MouseClick(object sender, MouseEventArgs e)
        {
            MapCanvas.Focus();

            if (e.Button == MouseButtons.Right)
            {
                int i = FindPoint(e.Location);
                if (i != -1)
                {
                    //WaypointContextMenu.Show(MapCanvas, e.Location);
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
            else
            {
                bool active = false;
                int i = FindDrone(e.Location);
                if (i != -1)
                {
                    if (selectedDrone == i)
                        selectedDrone = -1;
                    else
                    {
                        active = true;
                        selectedDrone = i;
                    }
                }
                if (!active && selectedDrone != -1)
                {
                    detectedDrones[selectedDrone].setDestination(ScreenToWorld(e.Location));

                    Drone drone = detectedDrones[selectedDrone];

                    selectedDrone = -1;

                    /**/
                    PathFinder pf = new PathFinder(
                        (int)(Math.Abs(drone.absolutePosition.X / 0.20) + hm.getOffset()),
                        (int)((Math.Abs(drone.absolutePosition.Y) / 0.20) - 2),
                        (int)(Math.Abs(drone.destination.X / 0.20) + hm.getOffset()),
                        (int)(Math.Abs(drone.destination.Y) / 0.20),
                        drone.orientation,
                        heightMap);
                    /**/
                    Console.WriteLine("Start position: (" + (int)(Math.Abs(drone.absolutePosition.X / 0.20) + hm.getOffset()) + "," + (int)(Math.Abs(drone.absolutePosition.Y) / 0.20) + ")");
                    Console.WriteLine("End position: (" + (int)(Math.Abs(drone.destination.X / 0.20) + hm.getOffset()) + "," + (int)(Math.Abs(drone.destination.Y) / 0.20) + ")");
                    pf.solve();

                    pf.printGrid();

                    Stack<String> commands = new Stack<string>();
                    foreach (String command in pf.getPath())
                    {
                        commands.Push(command);
                    }
                    doMagic(commands);
                    //pf.printGrid();

                    //System.Windows.Forms.MessageBox.Show("Move drone to position?", "Moving Drone Confirmation", MessageBoxButtons.YesNo);
                }
            }


        }

        /**
        * @brief Event handler. Called by MapCanvas for mouse down events.
        * @param sender An object - Source of the event
        * @param e An EventArgs - Mouse Event Information
        */
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

        /**
        * @brief Event handler. Called by MapCanvas for mouse up events.
        * @param sender An object - Source of the event
        * @param e An EventArgs - Mouse Event Information
        */
        private void MapCanvas_MouseUp(object sender, MouseEventArgs e)
        {
            if (m_MovingWaypoint && m_MovingPoint)
            {
                m_MovingPoint = false;
                MapCanvas.Invalidate();
            }

            MapCanvas.Cursor = Cursors.Default;
        }

        /**
        * @brief Event handler. Called by MapCanvas for mouse move events.
        * @param sender An object - Source of the event
        * @param e An EventArgs - Mouse Event Information
        */
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
            else if (FindDrone(e.Location) > 0)
            {
                MapCanvas.Cursor = Cursors.Hand;
            }
            else
            {
                MapCanvas.Cursor = Cursors.Default;
            }
        }
        #endregion

        //**********************************//
        //     GRAPHIC-RELATED METHODS      //
        //**********************************//
        #region Graphics (or mixed) methods

        /**
        * @brief Event handler. Called by MapCanvas for paint events.
        * @param sender An object - Source of the event
        * @param e An EventArgs - Paint Event Information
        */
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
                    e.Graphics.DrawPie(m_PathPen, ViewRectangle, 220 - Angle0, 100);   //pq 220?
                else
                    e.Graphics.DrawPie(m_PathPen, ViewRectangle, 220 + Angle0, 100);

                e.Graphics.SmoothingMode = SmoothingMode.AntiAlias;

                /* Draw History */
                for (int i = 0; i < m_Robot.TelemetryHistory.Count; i++)
                {
                    PointF P = WorldToScreen(m_Robot.TelemetryHistory[i].Position);
                    e.Graphics.FillRectangle(m_HistoryBrush, P.X - 2, P.Y - 2, 4, 4);
                }
            }

            Brush[] inactive = { Brushes.PaleVioletRed, Brushes.Navy, Brushes.Olive, Brushes.Orange };
            Brush[] active = { Brushes.Red, Brushes.Blue, Brushes.Green, Brushes.Orange };
            Brush[] colors = inactive;

            e.Graphics.SmoothingMode = SmoothingMode.AntiAlias;

            if (detectedDrones.Count > 0)
            {       //we have to work on it!!!!!
                Drone actual;
                //TODO FIX THIS CRAP      
                PointF tmp = new PointF();

                for (int i = 0; i < detectedDrones.Count; i++)
                {
                    actual = detectedDrones[i];
                    //TODO FIX THIS CRAP
                    tmp.X = actual.absolutePosition.X - 0.3f;
                    tmp.Y = actual.absolutePosition.Y - 0.3f;

                    if (selectedDrone != -1)
                        colors = active;
                    else
                        colors = inactive;

                    e.Graphics.FillEllipse(colors[0], WorldToScreen(tmp).X, WorldToScreen(tmp).Y, WorldToScreenSize(new SizeF(0.6f, 0.6f)).Width, WorldToScreenSize(new SizeF(0.6f, 0.6f)).Width);

                    tmp.X = actual.absolutePosition.X + ((float)Math.Cos((90 * deg2rad) + actual.orientation) * 0.33f) - 0.05f;
                    tmp.Y = actual.absolutePosition.Y + ((float)Math.Sin((90 * deg2rad) + actual.orientation) * 0.33f) - 0.05f;

                    e.Graphics.FillEllipse(colors[2], WorldToScreen(tmp).X, WorldToScreen(tmp).Y, WorldToScreenSize(new SizeF(0.1f, 0.1f)).Width, WorldToScreenSize(new SizeF(0.1f, 0.1f)).Width);

                    if (actual.destination.X != 0)
                    {
                        PointF tmp2 = new PointF();
                        tmp2.X = actual.destination.X - 0.25f;
                        tmp2.Y = actual.destination.Y - 0.25f;
                        e.Graphics.FillEllipse(Brushes.Green, WorldToScreen(tmp2).X, WorldToScreen(tmp2).Y, WorldToScreenSize(new SizeF(0.5f, 0.5f)).Width, WorldToScreenSize(new SizeF(0.5f, 0.5f)).Width);
                    }
                }
            }

            if (spherePositions.Count > 0)
            {
                float radius;
                for (int i = 0; i < spherePositions.Count; i++)
                {
                    //Size in pixels (for zoom in/out)
                    radius = WorldToScreenSize(new SizeF(sphereRadii[i], sphereRadii[i])).Width;
                    e.Graphics.FillEllipse(colors[3], WorldToScreen(spherePositions[i]).X, WorldToScreen(spherePositions[i]).Y, radius, radius);
                }
            }
        }

    
        private void prepareSpheres()
        {
            Sphere actual;
            for (int i = 0; i < detectedSpheres.Count; i++)
            {
                actual = detectedSpheres[i];
                PointF center = prepareSphere(actual, true);
            }
        }
        //PAREI AQUIIIIIIIIII

        /**
         * @brief Prepare the spheres to be drawn on the MapCanvas
         * @param actual A Sphere
         * @param add A bool
         * @returns spherePosition A PointF
        */
        private PointF prepareSphere(Sphere actual, bool add)
        {

            PointF spherePosition;
            double globalAngle, realAngle, radius, hypo;


            spherePosition = new PointF();

            hypo = Math.Sqrt(Math.Pow(actual.absolutePosition.X * 100, 2) + Math.Pow(actual.absolutePosition.Z * 100, 2)) / 100;

            //Angle of the spheres relative to the MotherBot
            realAngle = Math.Asin(actual.absolutePosition.X / actual.absolutePosition.Z);

            //Angle of the robot relative to map
            if (m_Robot != null)
                globalAngle = (m_Robot.Telemetry.Heading - 90) * deg2rad;
            else
                globalAngle = (angleee - 90) * deg2rad;

            spherePosition.X = (float)(Math.Cos(globalAngle + realAngle) * hypo) - actual.realRadius / 2;
            spherePosition.Y = (float)(Math.Sin(globalAngle + realAngle) * hypo) - actual.realRadius / 2;

            if (add)
            {
                spherePositions.Add(spherePosition);
                sphereRadii.Add(actual.realRadius);
            }
            return spherePosition;
        }

        /**
         * @brief Searches for the first drones.
        */
        public void findDrones()
        {
            Sphere actual, actual2;

            double distance, ratio;

            //TODO create a list of the different distances/ratio
            double expectedDistance = 0.28; //HARDCODED
            double sphereDistanceThreshold = 0.4; //HARDCODED

            double expectedRatio = 0.80; //HARDCODED
            double expectedRatioThreshold = 0.4; //HARDCODED

            double diffX, diffY, droneAngle;
            PointF dronePosition = new PointF();

            double globalAngle, realAngle, radius, hypo;

            List<Sphere> associatedSpheres = new List<Sphere>();

            for (int i = 0; i < detectedSpheres.Count; i++)
            {
                actual = detectedSpheres[i];

                for (int m = 0; m < detectedSpheres.Count; m++)
                {

                    actual2 = detectedSpheres[m];
                    //If its us or if the sphere is used already, break
                    if (actual.Equals(actual2) || associatedSpheres.Contains(actual2))
                        break;

                    diffX = actual.absolutePosition.X - actual2.absolutePosition.X;
                    diffY = actual.absolutePosition.Z - actual2.absolutePosition.Z;
                    distance = Math.Sqrt(Math.Pow(diffX, 2) + Math.Pow(diffY, 2));
                    ratio = Math.Min(actual.realRadius, actual2.realRadius) / Math.Max(actual.realRadius, actual2.realRadius);

                    //IF we find a pair of spheres that look right...
                    if (Math.Abs(distance - expectedDistance) < sphereDistanceThreshold && Math.Abs(ratio - expectedRatio) < expectedRatioThreshold)
                    {
                        associatedSpheres.Add(actual);
                        associatedSpheres.Add(actual2);

                        //newDiffX = diffX / 2;
                        //newDiffY = diffY / 2;

                        //dronePosition.X = (float)(Math.Min(actual.absolutePosition.X, actual2.absolutePosition.X) + newDiffX);
                        //dronePosition.Y = (float)(Math.Min(actual.absolutePosition.Z, actual2.absolutePosition.Z) + newDiffY);

                        dronePosition.X = (float)((actual.absolutePosition.X + actual2.absolutePosition.X) / 2.0);
                        dronePosition.Y = (float)((actual.absolutePosition.Z + actual2.absolutePosition.Z) / 2.0);


                        //droneAngle = 0;
                        //Yeah, im repeating code, but otherwise nobody would understand it
                        /*if (actual.realRadius > actual2.realRadius && actual.absolutePosition.X > actual2.absolutePosition.X)
                            droneAngle += 180;

                        if (actual2.realRadius > actual.realRadius && actual2.absolutePosition.Z > actual.absolutePosition.Z)
                            droneAngle -= 180;
                         * */
                        /**
                            if(actual2.realRadius > actual.realRadius)
                                droneAngle = (droneAngle*deg2rad) + Math.Atan(diffX / diffY);
                        else
                            droneAngle = (droneAngle * deg2rad) + Math.Atan(diffY / diffX);

                        //dronePosition.X = (float)(actual.absolutePosition.X + newDiffX);
                        //dronePosition.Y = (float)(actual.absolutePosition.Z + newDiffY);

                        //Angle of the Drome relative to the MotherBot
                        realAngle = Math.Asin(dronePosition.X / dronePosition.Y);

                        hypo = Math.Sqrt(Math.Pow(dronePosition.X, 2) + Math.Pow(dronePosition.Y, 2));



                        dronePosition.X = (float)(Math.Cos(globalAngle + realAngle) * hypo);
                        dronePosition.Y = (float)(Math.Sin(globalAngle + realAngle) * hypo);
                        /**/

                        //Angle of the robot relative to map
                        if (m_Robot != null)
                            globalAngle = (m_Robot.Telemetry.Heading - 90) * deg2rad;
                        else
                            globalAngle = (angleee - 90) * deg2rad;

                        double angle = 0;
                        if (actual.realRadius > actual2.realRadius)
                            angle = Math.Atan((prepareSphere(actual, false).X - prepareSphere(actual2, false).X) / (prepareSphere(actual, false).Y - prepareSphere(actual2, false).Y));
                        else
                            angle = Math.Atan((prepareSphere(actual2, false).X - prepareSphere(actual, false).X) / (prepareSphere(actual2, false).Y - prepareSphere(actual, false).Y));

                        //Angle of the Drone relative to the MotherBot
                        realAngle = Math.Asin(dronePosition.X / dronePosition.Y);
                        hypo = Math.Sqrt(Math.Pow(dronePosition.X, 2) + Math.Pow(dronePosition.Y, 2));
                        dronePosition.X = (float)(Math.Cos(globalAngle + realAngle) * hypo);
                        dronePosition.Y = (float)(Math.Sin(globalAngle + realAngle) * hypo);

                        if (dronePosition.X < 0 && dronePosition.Y < 0)
                            angle -= 90 * deg2rad;
                        else if (dronePosition.X > 0 && dronePosition.Y > 0)
                            angle -= 90 * deg2rad;

                        //
                        Console.WriteLine(((float)globalAngle * 57.3));
                        //Console.WriteLine(((float)globalAngle + (float)droneAngle) * 57.3);
                        detectedDrones.Add(new Drone(dronePosition, (float)globalAngle - (float)angle));
                        //System.Windows.Forms.MessageBox.Show("Found DRONE!\n\nX=" + dronePosition.X + "\nY=" + dronePosition.Y + "\nTHETA=" + droneAngle / deg2rad);
                    }
                }
            }
        }

        /**
         * @brief Updates the map.
        */

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
        #endregion

        #region ZOOM RELATED BUTTONS

        /**
         * @brief Zooms
         * @param Direction
         **/
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

        /**
        * @brief Event handler. Called by ZoomInCommand for click events.
        * @param sender An object - Source of the event
        * @param e An EventArgs - Event Information
        */
        private void ZoomInCommand_Click(object sender, EventArgs e)
        {
            Zoom(+1);
        }

        /**
        * @brief Event handler. Called by ZoomOutCommand for click events.
        * @param sender An object - Source of the event
        * @param e An EventArgs - Event Information
        */
        private void ZoomOutCommand_Click(object sender, EventArgs e)
        {
            Zoom(-1);
        }
        #endregion

        #region PATH PLANNING RELATED BUTTONS

        /**
        * @brief Event handler. Called by ClearPathCommand for click events.
        * @param sender An object - Source of the event
        * @param e An EventArgs - Event Information
        */
        private void ClearPathCommand_Click(object sender, EventArgs e)
        {
            m_Path.Clear();
            MapCanvas.Invalidate();
        }

        /**
        * @brief Event handler. Called by AddWaypointCommand for click events.
        * @param sender An object - Source of the event
        * @param e An EventArgs - Event Information
        */
        private void AddWaypointCommand_Click(object sender, EventArgs e)
        {
            m_AddingWaypoint = !m_AddingWaypoint;
            AddWaypointCommand.Checked = !AddWaypointCommand.Checked;

            m_DeletingWaypoint = false;
            DeleteWaypointCommand.Checked = false;
            m_MovingWaypoint = false;
            MoveWaypointCommand.Checked = false;
        }

        /**
        * @brief Event handler. Called by DeleteWaypointCommand for click events.
        * @param sender An object - Source of the event
        * @param e An EventArgs - Event Information
        */
        private void DeleteWaypointCommand_Click(object sender, EventArgs e)
        {
            m_DeletingWaypoint = !m_DeletingWaypoint;
            DeleteWaypointCommand.Checked = !DeleteWaypointCommand.Checked;

            m_AddingWaypoint = false;
            AddWaypointCommand.Checked = false;
            m_MovingWaypoint = false;
            MoveWaypointCommand.Checked = false;
        }

        /**
        * @brief Event handler. Called by MoveWaypointCommand for click events.
        * @param sender An object - Source of the event
        * @param e An EventArgs - Event Information
        */
        private void MoveWaypointCommand_Click(object sender, EventArgs e)
        {
            m_MovingWaypoint = !m_MovingWaypoint;
            MoveWaypointCommand.Checked = !MoveWaypointCommand.Checked;

            m_AddingWaypoint = false;
            AddWaypointCommand.Checked = false;
            m_DeletingWaypoint = false;
            DeleteWaypointCommand.Checked = false;
        }

        /**
        * @brief Event handler. Called by loadMapToolStripMenuItem for click events.
        * @param sender An object - Source of the event
        * @param e An EventArgs - Event Information
        */
        private void loadMapToolStripMenuItem_Click(object sender, EventArgs e)
        {
            openFileDialog1.DefaultExt = "Map";
            openFileDialog1.Filter = "Maps|*.Map";

            if (openFileDialog1.ShowDialog() == DialogResult.OK)
            {
                Map = Map.Read(openFileDialog1.FileName);
            }
        }

        /**
        * @brief Event handler. Called by loadPathToolStripMenuItem for click events.
        * @param sender An object - Source of the event
        * @param e An EventArgs - Event Information
        */
        private void loadPathToolStripMenuItem_Click(object sender, EventArgs e)
        {
            openFileDialog1.DefaultExt = "Path";
            openFileDialog1.Filter = "Paths|*.Path";

            if (openFileDialog1.ShowDialog() == DialogResult.OK)
            {
                Path = Path.Read(openFileDialog1.FileName);
            }
        }

        /**
        * @brief Event handler. Called by saveMapToolStripMenuItem for click events.
        * @param sender An object - Source of the event
        * @param e An EventArgs - Event Information
        */
        private void saveMapToolStripMenuItem_Click(object sender, EventArgs e)
        {
            saveFileDialog1.DefaultExt = "Map";
            saveFileDialog1.Filter = "Maps|*.Map";
            if (saveFileDialog1.ShowDialog() == DialogResult.OK)
            {
                Map.Write(saveFileDialog1.FileName);
            }
        }

        /**
        * @brief Event handler. Called by savePathToolStripMenuItem for click events.
        * @param sender An object - Source of the event
        * @param e An EventArgs - Event Information
        */
        private void savePathToolStripMenuItem_Click(object sender, EventArgs e)
        {
            saveFileDialog1.DefaultExt = "Path";
            saveFileDialog1.Filter = "Paths|*.Path";

            if (saveFileDialog1.ShowDialog() == DialogResult.OK)
            {
                Path.Write(saveFileDialog1.FileName);
            }
        }

        /**
        * @brief Event handler. Called by StopDrivingCommand for click events.
        * @param sender An object - Source of the event
        * @param e An EventArgs - Event Information
        */
        private void StopDrivingCommand_Click(object sender, EventArgs e)
        {
            stop();
        }

        /**
        * @brief Event handler. Called by DriveCommand for click events.
        * @param sender An object - Source of the event
        * @param e An EventArgs - Event Information
        */
        private void DriveCommand_Click(object sender, EventArgs e)
        {
            m_Robot.SetPath(m_Path);
        }

        /**
        * @brief Event handler. Called by ViewRobotCommand for click events.
        * @param sender An object - Source of the event
        * @param e An EventArgs - Event Information
        */
        private void ViewRobotCommand_Click(object sender, EventArgs e)
        {
            if (m_Robot != null)
                ScrollToPosition(m_Robot.Telemetry.Position);
        }

        #endregion

        //***********************************//
        // GENERAL (DUBIOUS UTILITY) METHODS //
        //***********************************//
        #region GENERAL METHODS

        /**
        * @param hue A float
        * @param saturation A float
        * @param value A float
        * @returns A Color
        */
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

        /**
         * @param P A PointF
         * @returns A PointF
        */
        private PointF WorldToScreen(PointF P)
        {
            return new PointF((P.X - m_Offset.X + (Map.WorldWidth / 2.0f)) / m_Scale, (P.Y - m_Offset.Y + (Map.WorldHeight / 2.0f)) / m_Scale);
        }

         /**
         * @param S A SizeF
         * @return A SizeF
         */
        private SizeF WorldToScreenSize(SizeF S)
        {
            return new SizeF(S.Width / m_Scale, S.Height / m_Scale);
        }

        /**
         * @param P A PointF
         * @returns A PointF
        */
        private PointF ScreenToWorld(PointF P)
        {
            return new PointF((P.X * m_Scale) + m_Offset.X - (Map.WorldWidth / 2.0f), (P.Y * m_Scale) + m_Offset.Y - (Map.WorldHeight / 2.0f));
        }

        /**
         * @param P A PointF
         * @returns A PointF
        */
        private PointF ScreenToWorldSize(PointF P)
        {
            return new PointF(P.X * m_Scale, P.Y * m_Scale);
        }

        /**
         * @param World A PointF
         */
        public void ScrollToPosition(PointF World)
        {
            PointF ViewportSize = ScreenToWorldSize(new PointF(MapCanvas.Width, MapCanvas.Height));

            m_Offset = new PointF(World.X + Map.WorldWidth / 2.0f - ViewportSize.X / 2.0f, World.Y + Map.WorldHeight / 2.0f - ViewportSize.Y / 2.0f);

            MapCanvas.Invalidate();
        }
        
        /**
         * @brief Searches for the first point
         * @param CursorLocation A PointF
         * @returns An Integer
        */
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

        /*
         * @brief Searches for the first drone
         * @param CursorLocation A PointF
         * @returns The Found Drone - An Integer
         */
        private int FindDrone(PointF CursorLocation)
        {
            Drone actual;
            for (int i = 0; i < detectedDrones.Count; i++)
            {
                actual = detectedDrones[i];
                PointF Point = WorldToScreen(actual.absolutePosition);
                if (Math.Abs(Point.X - CursorLocation.X) < WorldToScreenSize(new SizeF(0.6f, 0.6f)).Width && Math.Abs(Point.Y - CursorLocation.Y) < WorldToScreenSize(new SizeF(0.6f, 0.6f)).Width)
                    return i;
            }
            return -1;
        }

        #endregion

        #endregion

        //************************************************************//
        //        Here starts code that is specific for the           //
        //                      LIDAR SCANNING tab~                   //
        //************************************************************//
        #region LIDAR SCANNING STUFF

        ScanParameters scanParams;

        /**
         * @brief Initializes the lidar scanning
         */
        public void initLidarScanning()
        {
            scanParams = new ScanParameters();
            // these should probably be read in from a file or something...
            scanParams.HRange = 0;
            scanParams.HRes = 0;
            scanParams.scanBottom = -45;
            scanParams.scanLines = 360;
            scanParams.scanRes = (float).25;
        }

      /**
       * @brief Event handler. Called by button6 for click events.
       * @param sender An object - Source of the event
       * @param e An EventArgs - Event Information
       */
        private void button6_Click(object sender, EventArgs e)
        {
            ScanParametersViewer v = new ScanParametersViewer(ref scanParams);
            v.ShowDialog();
        }
      /**
       * @brief Event handler. Called by button5 for click events.
       * @param sender An object - Source of the event
       * @param e An EventArgs - Event Information
       */
        private void button5_Click(object sender, EventArgs e)
        {
            m_Robot.Scan(scanParams);
        }

        #endregion

       /**
       * @brief Process the range image.
       */
        public void processRangeImage()
        {
            //Are these two line crashing the program because we are trying to do the detection after each scan?
            //SphereRecognition sr = new SphereRecognition();
            //detectedSpheres = sr.FindSpheres(m_Robot.LatestRangeImage, null, 35, 7, 0.2f, 65f);

            SphereRecognition2 sr = new SphereRecognition2();
            detectedSpheres = sr.FindSpheres(m_Robot.LatestRangeImage);


            hm = new HeightMap(m_Robot.LatestRangeImage);
            hm.createMap();
            heightMap = hm.bool_grid;

            UpdateMap();
            MapCanvas.Invalidate();

            if (m_RangeImageViewer != null)
            {
                if (m_Robot.LatestRangeImage.Height > 2)
                    m_RangeImageViewer.RangeImage = m_Robot.LatestRangeImage;
            }

            spherePositions.Clear();
            sphereRadii.Clear();
            detectedDrones.Clear();

            prepareSpheres();
            findDrones();
        }

        /**
       * @brief Event handler. Called by Control for load events.
       * @param sender An object - Source of the event
       * @param e An EventArgs - Event Information
       */
        private void Control_Load(object sender, EventArgs e)
        {

        }

        /**
         * @brief Event handler. Called by button7 for click events.
         * @param sender An object - Source of the event
         * @param e An EventArgs - Event Information
       */
        private void button7_Click(object sender, EventArgs e)
        {
            SphereRecognition sr = new SphereRecognition();
            if (openFileDialog1.ShowDialog() == DialogResult.OK)
            {
                sr = new SphereRecognition();

                hm = new HeightMap(RangeImage.Read(openFileDialog1.FileName));
                hm.createMap();
                heightMap = hm.bool_grid;

                detectedSpheres = sr.FindSpheres(RangeImage.Read(openFileDialog1.FileName), null, 35, 7, 0.2f, 70f);
                spherePositions.Clear();
                sphereRadii.Clear();
                detectedDrones.Clear();

                prepareSpheres();
                findDrones();
            }
        }

        bool conn = false;
        Drone dr;
        DroneCommandList dcl;

       /**
       * @brief Event handler. Called by button8_Click_1 for click events.
       * @param sender An object - Source of the event
       * @param e An EventArgs - Event Information
       */
        private void button8_Click_1(object sender, EventArgs e)
        {
            if (dr == null)
                dr = new Drone(new PointF(), 0);

            if (!dr.connected)
            {
                dr.connect("192.168.1.100");
                button8.Text = "Send";
                button8.Refresh();
            }
            else
            {
                //dr.sendCommand(command.Text);
                Stack<String> stack = new Stack<string>();
                //stack.Push("forward 40 100");
                //stack.Push("left 40 45");
                //stack.Push("reverse 40 200");
                //stack.Push("right 40 100");
                //stack.Push(command.Text);
                doMagic(stack);
            }
        }

        /**
         * @brief Executes the magic operation.
         * @param commands A Stack<String>
         */
        private void doMagic(Stack<String> commands)
        {

            //dcl = new DroneCommandList();
            //dcl.populate(dr.ip, commands);
            //dcl.Show();

            while (commands.Count > 0)
            {
                dr.sendCommand(commands.Pop());
                while (dr.isRunning())
                {
                    Thread.Sleep(300);
                }
                Thread.Sleep(1000);
                //dcl.removeElement();
            }

        }

       /**
       * @brief Event handler. Called by button9_Click_1 for click events.
       * @param sender An object - Source of the event
       * @param e An EventArgs - Event Information
       */
        private void button9_Click_1(object sender, EventArgs e)
        {
            angleee += 90;
            if (angleee >= 360)
            {
                angleee = 0;
            }
            findDrones();
            prepareSpheres();
        }

       /**
       * @brief Event handler. Called by tableLayoutPanel8 for paint events.
       * @param sender An object - Source of the event
       * @param e An EventArgs - Paint Event Information
       */
        private void tableLayoutPanel8_Paint(object sender, PaintEventArgs e)
        {

        }

       /**
       * @brief Event handler. Called by label14 for click events.
       * @param sender An object - Source of the event
       * @param e An EventArgs - Event Information
       */
        private void label14_Click(object sender, EventArgs e)
        {

        }

       /**
       * @brief Event handler. Called by label19 for click events.
       * @param sender An object - Source of the event
       * @param e An EventArgs - Event Information
       */
        private void label19_Click(object sender, EventArgs e)
        {

        }

       /**
       * @brief Event handler. Called by label12 for click events.
       * @param sender An object - Source of the event
       * @param e An EventArgs - Event Information
       */
        private void label12_Click(object sender, EventArgs e)
        {

        }

       /**
       * @brief Event handler. Called by label11 for click events.
       * @param sender An object - Source of the event
       * @param e An EventArgs - Event Information
       */
        private void label11_Click(object sender, EventArgs e)
        {

        }

       /**
       * @brief Event handler. Called by label13 for click events.
       * @param sender An object - Source of the event
       * @param e An EventArgs - Event Information
       */
        private void label13_Click(object sender, EventArgs e)
        {

        }

       /**
       * @brief Event handler. Called by button8 for click events.
       * @param sender An object - Source of the event
       * @param e An EventArgs - Event Information
       */
        private void button8_Click(object sender, EventArgs e)
        {
            m_RangeImageViewer = new RangeImageViewer(m_Robot);
            m_RangeImageViewer.Show();
            if (m_Robot.RangeImages.Count > 0)
                m_RangeImageViewer.RangeImage = m_Robot.RangeImages[m_Robot.RangeImages.Count - 1];
        }

       /**
       * @brief Event handler. Called by button9 for click events.
       * @param sender An object - Source of the event
       * @param e An EventArgs - Event Information
       */
        private void button9_Click(object sender, EventArgs e)
        {
            m_SphereRecognitionView = new SphereRecognitionView(m_Robot);
            m_SphereRecognitionView.Show();
        }

       /**
       * @brief Event handler. Called by button10 for click events.
       * @param sender An object - Source of the event
       * @param e An EventArgs - Event Information
       */
        private void button10_Click(object sender, EventArgs e)
        {

            ScanParameters sp = new ScanParameters();
            sp.HRange = 0;
            sp.HRes = 0;
            sp.scanBottom = -45;
            sp.scanLines = 1;
            sp.scanRes = (float).25;

            m_Robot.Scan(sp);

            sp.scanBottom = -45;


        }

        /**
       * @brief Event handler. Called by tableLayoutPanel8_Paint for Paint events.
       * @param sender An object - Source of the event
       * @param e An EventArgs - Paint event Information
       */
        private void tableLayoutPanel8_Paint_1(object sender, PaintEventArgs e)
        {

        }

       /**
       * @brief Event handler. Called by button11 for click events.
       * @param sender An object - Source of the event
       * @param e An EventArgs - Event Information
       */
        private void button11_Click(object sender, EventArgs e)
        {

        }

       /**
       * @brief Event handler. Called by button11_Click_1 for click events.
       * @param sender An object - Source of the event
       * @param e An EventArgs - Event Information
       */
        private void button11_Click_1(object sender, EventArgs e)
        {
            System.Diagnostics.Process.Start(@"Robot_GUI.exe");
        }

       /**
       * @brief Event handler. Called by distanceMoved for click events.
       * @param sender An object - Source of the event
       * @param e An EventArgs - Event Information
       */
        private void distanceMoved_Click(object sender, EventArgs e)
        {

        }

       /**
       * @brief Event handler. Called by label11_Click_1 for click events.
       * @param sender An object - Source of the event
       * @param e An EventArgs - Event Information
       */
        private void label11_Click_1(object sender, EventArgs e)
        {

        }

       /**
       * @brief Event handler. Called by label12_Click_1 for click events.
       * @param sender An object - Source of the event
       * @param e An EventArgs - Event Information
       */
        private void label12_Click_1(object sender, EventArgs e)
        {

        }

    }
}
