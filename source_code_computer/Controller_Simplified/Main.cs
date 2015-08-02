using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Drawing.Imaging;
using System.Runtime.InteropServices;
using Microsoft.DirectX.DirectInput;

namespace Controller
{
  public partial class MainFrame : Form
  {
    /* The robot client interface */
    protected Robot m_Robot;

    /* The joystick device */
    protected Device m_JoystickDevice;

    private RangeImageViewer m_RangeImageViewer;
    private RangeImage m_CurrentRangeImage;
    private Control m_Control;
    private SphereRecognitionView m_SphereRecognitionView;

    public MainFrame(Robot R)
    {
      m_Robot = R;

      InitializeComponent();

      DeviceList L = Manager.GetDevices(DeviceType.Joystick, EnumDevicesFlags.AllDevices);
      L.MoveNext();
      if (L.Current == null)
      {
        JoystickControl.Enabled = false;
      }
      else
      {
        DeviceInstance DevInstance = (DeviceInstance)L.Current;

        m_JoystickDevice = new Device(DevInstance.InstanceGuid);
        m_JoystickDevice.SetCooperativeLevel(this, CooperativeLevelFlags.NonExclusive | CooperativeLevelFlags.Background);

        foreach (DeviceObjectInstance doi in m_JoystickDevice.Objects)
        {
          if ((doi.ObjectId & (int)DeviceObjectTypeFlags.Axis) != 0)
            m_JoystickDevice.Properties.SetRange(ParameterHow.ById, doi.ObjectId, new InputRange(-2550, 2550));
        }

        m_JoystickDevice.Properties.AxisModeAbsolute = true;
      }

      UpdateFields();

      FastRefreshTimer.Enabled = true;

      //m_ImageViewer = new ImageViewer();
      //m_ImageViewer.Show();

      //m_RangeImageViewer = new RangeImageViewer(m_Robot);
      //m_RangeImageViewer.Show();

      m_Control = new Control(m_Robot);
      m_Control.Show();

      //m_SphereRecognitionView = new SphereRecognitionView(m_Robot);
      //m_SphereRecognitionView.Show();

      //m_Navigation = new Navigation(m_Robot);
      //m_Navigation.Show();
    }

    private void SlowUpdateTimer_Tick(object sender, EventArgs e)
    {
      UpdateSlowFields();

      if (!m_Robot.Connected)
        m_Robot.Connect();
    }

    private void UpdateFields()
    {
      UpdateSlowFields();
      UpdateFastFields();
    }

    private void FastUpdateTimer_Tick(object sender, EventArgs e)
    {
      UpdateFastFields();

      if (JoystickControl.Checked)
        m_Robot.SetSpeedAndBias(m_JoystickPosition.Y / 127.0f, m_JoystickPosition.X / 127.0f);
    }

    private void UpdateSlowFields()
    {
        if (m_Robot != null)
        {
            lock (m_Robot)
            {
                if (m_Robot.Connected)
                {
                    TimeSpan Latency = m_Robot.GetRoundTripLatency();
                    if (Latency.TotalMilliseconds == 0)
                        ConnectionLatency.Text = "< 1 ms";
                    else
                        ConnectionLatency.Text = Latency.TotalMilliseconds.ToString("0.0") + " ms";
                }
                else
                {
                    ConnectionLatency.Text = "";
                }
            }
        } //endif
    }

    private void UpdateFastFields()
    {
      //if (m_Navigation != null)
        //m_Navigation.Update();

   //   UpdateImage();
      UpdateRangeImage();

      lock (m_Robot)
      {
        if (m_Robot.Host != null)
          Host.Text = m_Robot.Host.ToString();
        else
          Host.Text = "";

        if (m_Robot.Connected)
        {
          ConnectionStatus.Text = "Connected";
          Connect.Text = "Disconnect";
        }
        else
        {
          ConnectionStatus.Text = "Disconnected";
          Connect.Text = "Connect";
        }

        LastTelemetry.Text = m_Robot.LastTelemetryUpdate.ToString();
        TimeSpan TelemetryAge = DateTime.Now - m_Robot.LastTelemetryUpdate;
        if (TelemetryAge.TotalSeconds > 30)
        {
          LastTelemetry.ForeColor = Color.Red;
        }
        else
        {
          LastTelemetry.ForeColor = Color.Black;
        }

        CurrentRobotTime.Text = new DateTime(m_Robot.Telemetry.CurrentTime).ToString();
        RobotUptime.Text = new TimeSpan(m_Robot.Telemetry.CurrentTime - m_Robot.Telemetry.StartTime).ToString();

        /* Environmental Control Telemetry */
        ChassisTemperature.Text = m_Robot.Telemetry.Temperature0.ToString("0.0");
        BatteryTemperature.Text = m_Robot.Telemetry.Temperature1.ToString("0.0");
        MotorControllerTemperature.Text = m_Robot.Telemetry.Temperature2.ToString("0.0");
        Temperature0.Text = m_Robot.Telemetry.Temperature3.ToString("0.0");
        Temperature1.Text = m_Robot.Telemetry.Temperature4.ToString("0.0");
        Temperature2.Text = m_Robot.Telemetry.Temperature5.ToString("0.0");
        Temperature3.Text = m_Robot.Telemetry.Temperature6.ToString("0.0");

        Location.Text = "(" + m_Robot.Telemetry.Position.X.ToString("0.000") + ", " + m_Robot.Telemetry.Position.Y.ToString("0.000") + ")";
        Orientation.Text = "(" + m_Robot.Telemetry.PrimaryAxis.Width.ToString("0.000") + ", " + m_Robot.Telemetry.PrimaryAxis.Height.ToString("0.000") + ")";
        Heading.Text = m_Robot.Telemetry.Heading.ToString("0.0");

        TrackLeftEncoder.Text = m_Robot.Telemetry.TrackTicks0.ToString();
        TrackRightEncoder.Text = m_Robot.Telemetry.TrackTicks1.ToString();

        TrackLeftSpeed.Text = m_Robot.Telemetry.TrackSpeed0.ToString();
        TrackRightSpeed.Text = m_Robot.Telemetry.TrackSpeed1.ToString();

      }

      /* Update Driving status */
      if (m_Robot.Telemetry.MotionStatus > 8)
      {
        DrivingStatus.Text = "Driving";
        
        Drive.Enabled = false;
        DriveDistance.Enabled = false;
        Turn.Enabled = false;
        TurnDistance.Enabled = false;

        m_ExecutedDrivingCommand = true;
        m_PendingDrivingCommand = false;
      }
      else if (m_PendingDrivingCommand)
      {
        DrivingStatus.Text = "Pending";

        Drive.Enabled = false;
        DriveDistance.Enabled = false;
        Turn.Enabled = false;
        TurnDistance.Enabled = false;
      }
      else if (CommandControl.Checked)
      {
        if (m_ExecutedDrivingCommand)
        {
          m_ExecutedDrivingCommand = false;
          m_PendingDrivingCommand = false;
        }

        DrivingStatus.Text = "Idle";
        Drive.Enabled = true;
        DriveDistance.Enabled = true;
        Turn.Enabled = true;
        TurnDistance.Enabled = true;
      }


    }

    private Image m_CurrentImage;
  //  private ImageViewer m_ImageViewer;

 /*   private void UpdateImage()
    {
        if (m_Robot.LatestImage != null && m_Robot.LatestImage != m_CurrentImage)
        {
            if (m_ImageViewer != null)
                m_ImageViewer.Image = m_Robot.LatestImage;
            m_CurrentImage = m_Robot.LatestImage;
        }
    } */

    private void UpdateRangeImage()
    {
      if (m_Robot.LatestRangeImage != null && m_Robot.LatestRangeImage != m_CurrentRangeImage)
      {
        //if (m_RangeImageViewer != null)
        //      m_RangeImageViewer.RangeImage = m_Robot.LatestRangeImage;
        //if (m_Navigation != null)
        //      m_Navigation.Map = m_Robot.Map;
        if (m_Control != null)
        {
            m_Control.Map = m_Robot.Map;
            m_Control.processRangeImage();
        }

        m_CurrentRangeImage = m_Robot.LatestRangeImage;
      }
    }


    private void Connect_Click(object sender, EventArgs e)
    {
      if (m_Robot.Connected)
      {
        m_Robot.Disconnect();
      }
      else
      {
        do
        {
          if (m_Robot.Connect())
            break;
        }
        while (MessageBox.Show("Failed to connect to the robot.  Check network connectivity", "Robot Controller", MessageBoxButtons.RetryCancel) == DialogResult.Retry);
      }

      UpdateFields();
    }

    protected bool m_PendingDrivingCommand;
    protected bool m_ExecutedDrivingCommand;

    private void Drive_Click(object sender, EventArgs e)
    {
      Drive.Enabled = false;
      DriveDistance.Enabled = false;
      Turn.Enabled = false;
      TurnDistance.Enabled = false;

      m_Robot.Drive(float.Parse(DriveDistance.Text), 0);
      m_PendingDrivingCommand = true;
      DriveDistance.Text = "";
    }

    private void Turn_Click(object sender, EventArgs e)
    {
      Drive.Enabled = false;
      DriveDistance.Enabled = false;
      Turn.Enabled = false;
      TurnDistance.Enabled = false;

      m_Robot.Turn(-float.Parse(TurnDistance.Text));
      m_PendingDrivingCommand = true;
      TurnDistance.Text = "";
    }

    private void JoystickControl_CheckedChanged(object sender, EventArgs e)
    {
      if (JoystickControl.Checked)
      {
        Drive.Enabled = false;
        DriveDistance.Enabled = false;
        Turn.Enabled = false;
        TurnDistance.Enabled = false;

        
        m_JoystickDevice.Acquire();
        JoystickPosition.Invalidate();
        JoystickTimer.Enabled = true;
      }
      else
      {
        JoystickTimer.Enabled = false; 
        m_JoystickDevice.Unacquire();
        JoystickPosition.Invalidate();
      }

    }

    private void CommandControl_CheckedChanged(object sender, EventArgs e)
    {
      if (CommandControl.Checked)
      {
        Drive.Enabled = true;
        DriveDistance.Enabled = true;
        Turn.Enabled = true;
        TurnDistance.Enabled = true;
      }

    }

    private void JoystickPosition_Paint(object sender, PaintEventArgs e)
    {
      if (JoystickControl.Checked)
      {
        Pen P = new Pen(Color.Black);
        P.DashStyle = System.Drawing.Drawing2D.DashStyle.Dash;
        e.Graphics.DrawLine(P, 0, JoystickPosition.Height / 2, JoystickPosition.Width, JoystickPosition.Height / 2);
        e.Graphics.DrawLine(P, JoystickPosition.Width / 2, 0, JoystickPosition.Width / 2, JoystickPosition.Height);

        float X = (m_JoystickPosition.X + 128) / 256.0f * JoystickPosition.Width;
        float Y = (-m_JoystickPosition.Y + 128) / 256.0f * JoystickPosition.Height;
        
        Pen R = new Pen(Color.Red);
        e.Graphics.DrawRectangle(R, X - 2, Y - 2, 4, 4);
      }
    }

    private PointF[] m_JoystickHistory = new PointF[40];
    private PointF m_JoystickPosition;

    private void JoystickTimer_Tick(object sender, EventArgs e)
    {
      Array.Copy(m_JoystickHistory, 0, m_JoystickHistory, 1, m_JoystickHistory.Length - 1);
      m_JoystickHistory[0] = new PointF(m_JoystickDevice.CurrentJoystickState.X, m_JoystickDevice.CurrentJoystickState.Y);

      m_JoystickPosition = new PointF(0.0f, 0.0f);
      for (int i = 0; i < m_JoystickHistory.Length; i++)
      {
        m_JoystickPosition.X += m_JoystickHistory[i].X;
        m_JoystickPosition.Y += m_JoystickHistory[i].Y;
      }
      m_JoystickPosition = new PointF(m_JoystickPosition.X / m_JoystickHistory.Length, m_JoystickPosition.Y / m_JoystickHistory.Length);
      
      float X = Math.Abs(m_JoystickPosition.X / 1270.0f);
      float Y = Math.Abs(m_JoystickPosition.Y / 1270.0f);

      if (Math.Abs(X) < 0.1f)
        X = 0.0f;
      if (Math.Abs(Y) < 0.1f)
        Y = 0.0f;

      X = (float)Math.Pow(X, 1.9);
      Y = (float)Math.Pow(Y, 1.5);

      m_JoystickPosition.X = (int)(X * 127 * Math.Sign(m_JoystickPosition.X));
      m_JoystickPosition.Y = -(int)(Y * 127 * Math.Sign(m_JoystickPosition.Y));

      if (m_JoystickPosition.X < -127)
        m_JoystickPosition.X = -127;
      if (m_JoystickPosition.Y < -127)
        m_JoystickPosition.Y = -127;

      if (m_JoystickPosition.X > 127)
        m_JoystickPosition.X = 127;
      if (m_JoystickPosition.Y > 127)
        m_JoystickPosition.Y = 127;

      JoystickPosition.Invalidate();

    }

    private void Stop_Click(object sender, EventArgs e)
    {
      m_PendingDrivingCommand = false;
      m_Robot.Stop();
      JoystickControl.Checked = false;
    }

    private Debug m_DebugMessages;
    //private Navigation m_Navigation;

    private void MainFrame_Load(object sender, EventArgs e)
    {
        this.SendToBack();
    }

    private void DebugMessages_Click(object sender, EventArgs e)
    {
      if (m_DebugMessages == null)
        m_DebugMessages = new Debug(m_Robot);
      m_DebugMessages.Show();
    }

    /*private void Navigation_Click(object sender, EventArgs e)
    {
     if (m_Navigation == null)
        m_Navigation = new Navigation(m_Robot);
      m_Navigation.Show();
    
    }*/

    private void MainFrame_FormClosing(object sender, FormClosingEventArgs e)
    {
      //m_RangeImageViewer.Close();
      //m_ImageViewer.Close();
      //m_Navigation.Close();
    }

    private void ChassisTemperature_Click(object sender, EventArgs e)
    {

    }



  }
}