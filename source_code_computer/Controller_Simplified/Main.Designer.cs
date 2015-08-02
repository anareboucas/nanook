namespace Controller
{
  partial class MainFrame
  {
    /// <summary>
    /// Required designer variable.
    /// </summary>
    private System.ComponentModel.IContainer components = null;

    /// <summary>
    /// Clean up any resources being used.
    /// </summary>
    /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
    protected override void Dispose(bool disposing)
    {
      if (disposing && (components != null))
      {
        components.Dispose();
      }
      base.Dispose(disposing);
    }

    #region Windows Form Designer generated code

    /// <summary>
    /// Required method for Designer support - do not modify
    /// the contents of this method with the code editor.
    /// </summary>
    private void InitializeComponent()
    {
        this.components = new System.ComponentModel.Container();
        System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(MainFrame));
        this.FastRefreshTimer = new System.Windows.Forms.Timer(this.components);
        this.groupBox1 = new System.Windows.Forms.GroupBox();
        this.label23 = new System.Windows.Forms.Label();
        this.label21 = new System.Windows.Forms.Label();
        this.label22 = new System.Windows.Forms.Label();
        this.label20 = new System.Windows.Forms.Label();
        this.label18 = new System.Windows.Forms.Label();
        this.ChassisTemperature = new System.Windows.Forms.Label();
        this.Temperature3 = new System.Windows.Forms.Label();
        this.Temperature2 = new System.Windows.Forms.Label();
        this.Temperature1 = new System.Windows.Forms.Label();
        this.Temperature0 = new System.Windows.Forms.Label();
        this.MotorControllerTemperature = new System.Windows.Forms.Label();
        this.BatteryTemperature = new System.Windows.Forms.Label();
        this.label7 = new System.Windows.Forms.Label();
        this.label6 = new System.Windows.Forms.Label();
        this.groupBox2 = new System.Windows.Forms.GroupBox();
        this.Heading = new System.Windows.Forms.Label();
        this.label19 = new System.Windows.Forms.Label();
        this.label3 = new System.Windows.Forms.Label();
        this.Location = new System.Windows.Forms.Label();
        this.TrackRightSpeed = new System.Windows.Forms.Label();
        this.TrackLeftSpeed = new System.Windows.Forms.Label();
        this.TrackRightEncoder = new System.Windows.Forms.Label();
        this.TrackLeftEncoder = new System.Windows.Forms.Label();
        this.Orientation = new System.Windows.Forms.Label();
        this.label5 = new System.Windows.Forms.Label();
        this.label12 = new System.Windows.Forms.Label();
        this.label2 = new System.Windows.Forms.Label();
        this.groupBox4 = new System.Windows.Forms.GroupBox();
        this.RobotUptime = new System.Windows.Forms.Label();
        this.label30 = new System.Windows.Forms.Label();
        this.CurrentRobotTime = new System.Windows.Forms.Label();
        this.label17 = new System.Windows.Forms.Label();
        this.LastTelemetry = new System.Windows.Forms.Label();
        this.ConnectionLatency = new System.Windows.Forms.Label();
        this.ConnectionStatus = new System.Windows.Forms.Label();
        this.Host = new System.Windows.Forms.Label();
        this.Connect = new System.Windows.Forms.Button();
        this.label9 = new System.Windows.Forms.Label();
        this.label8 = new System.Windows.Forms.Label();
        this.label4 = new System.Windows.Forms.Label();
        this.label1 = new System.Windows.Forms.Label();
        this.groupBox5 = new System.Windows.Forms.GroupBox();
        this.PathControl = new System.Windows.Forms.RadioButton();
        this.JoystickPosition = new System.Windows.Forms.PictureBox();
        this.JoystickControl = new System.Windows.Forms.RadioButton();
        this.CommandControl = new System.Windows.Forms.RadioButton();
        this.DrivingStatus = new System.Windows.Forms.Label();
        this.label16 = new System.Windows.Forms.Label();
        this.Turn = new System.Windows.Forms.Button();
        this.TurnDistance = new System.Windows.Forms.TextBox();
        this.DriveDistance = new System.Windows.Forms.TextBox();
        this.label15 = new System.Windows.Forms.Label();
        this.label14 = new System.Windows.Forms.Label();
        this.label13 = new System.Windows.Forms.Label();
        this.label11 = new System.Windows.Forms.Label();
        this.Drive = new System.Windows.Forms.Button();
        this.JoystickTimer = new System.Windows.Forms.Timer(this.components);
        this.SlowUpdateTimer = new System.Windows.Forms.Timer(this.components);
        this.groupBox7 = new System.Windows.Forms.GroupBox();
        this.label27 = new System.Windows.Forms.Label();
        this.label26 = new System.Windows.Forms.Label();
        this.label25 = new System.Windows.Forms.Label();
        this.label24 = new System.Windows.Forms.Label();
        this.button2 = new System.Windows.Forms.Button();
        this.button1 = new System.Windows.Forms.Button();
        this.button4 = new System.Windows.Forms.Button();
        this.button3 = new System.Windows.Forms.Button();
        this.Navigation = new System.Windows.Forms.Button();
        this.DebugMessages = new System.Windows.Forms.Button();
        this.Stop = new System.Windows.Forms.Button();
        this.groupBox1.SuspendLayout();
        this.groupBox2.SuspendLayout();
        this.groupBox4.SuspendLayout();
        this.groupBox5.SuspendLayout();
        ((System.ComponentModel.ISupportInitialize)(this.JoystickPosition)).BeginInit();
        this.groupBox7.SuspendLayout();
        this.SuspendLayout();
        // 
        // FastRefreshTimer
        // 
        this.FastRefreshTimer.Interval = 250;
        this.FastRefreshTimer.Tick += new System.EventHandler(this.FastUpdateTimer_Tick);
        // 
        // groupBox1
        // 
        this.groupBox1.Controls.Add(this.label23);
        this.groupBox1.Controls.Add(this.label21);
        this.groupBox1.Controls.Add(this.label22);
        this.groupBox1.Controls.Add(this.label20);
        this.groupBox1.Controls.Add(this.label18);
        this.groupBox1.Controls.Add(this.ChassisTemperature);
        this.groupBox1.Controls.Add(this.Temperature3);
        this.groupBox1.Controls.Add(this.Temperature2);
        this.groupBox1.Controls.Add(this.Temperature1);
        this.groupBox1.Controls.Add(this.Temperature0);
        this.groupBox1.Controls.Add(this.MotorControllerTemperature);
        this.groupBox1.Controls.Add(this.BatteryTemperature);
        this.groupBox1.Controls.Add(this.label7);
        this.groupBox1.Controls.Add(this.label6);
        this.groupBox1.Location = new System.Drawing.Point(8, 457);
        this.groupBox1.Name = "groupBox1";
        this.groupBox1.Size = new System.Drawing.Size(281, 171);
        this.groupBox1.TabIndex = 29;
        this.groupBox1.TabStop = false;
        this.groupBox1.Text = "Environmental Control";
        // 
        // label23
        // 
        this.label23.AutoSize = true;
        this.label23.Location = new System.Drawing.Point(9, 146);
        this.label23.Name = "label23";
        this.label23.Size = new System.Drawing.Size(79, 13);
        this.label23.TabIndex = 6;
        this.label23.Text = "Temperature 3:";
        // 
        // label21
        // 
        this.label21.AutoSize = true;
        this.label21.Location = new System.Drawing.Point(9, 125);
        this.label21.Name = "label21";
        this.label21.Size = new System.Drawing.Size(79, 13);
        this.label21.TabIndex = 6;
        this.label21.Text = "Temperature 2:";
        // 
        // label22
        // 
        this.label22.AutoSize = true;
        this.label22.Location = new System.Drawing.Point(9, 104);
        this.label22.Name = "label22";
        this.label22.Size = new System.Drawing.Size(79, 13);
        this.label22.TabIndex = 6;
        this.label22.Text = "Temperature 1:";
        // 
        // label20
        // 
        this.label20.AutoSize = true;
        this.label20.Location = new System.Drawing.Point(9, 83);
        this.label20.Name = "label20";
        this.label20.Size = new System.Drawing.Size(79, 13);
        this.label20.TabIndex = 6;
        this.label20.Text = "Temperature 0:";
        // 
        // label18
        // 
        this.label18.AutoSize = true;
        this.label18.Location = new System.Drawing.Point(9, 63);
        this.label18.Name = "label18";
        this.label18.Size = new System.Drawing.Size(147, 13);
        this.label18.TabIndex = 6;
        this.label18.Text = "Motor Controller Temperature:";
        // 
        // ChassisTemperature
        // 
        this.ChassisTemperature.Location = new System.Drawing.Point(169, 25);
        this.ChassisTemperature.Name = "ChassisTemperature";
        this.ChassisTemperature.Size = new System.Drawing.Size(91, 16);
        this.ChassisTemperature.TabIndex = 2;
        this.ChassisTemperature.Text = "label8";
        this.ChassisTemperature.Click += new System.EventHandler(this.ChassisTemperature_Click);
        // 
        // Temperature3
        // 
        this.Temperature3.Location = new System.Drawing.Point(169, 146);
        this.Temperature3.Name = "Temperature3";
        this.Temperature3.Size = new System.Drawing.Size(91, 16);
        this.Temperature3.TabIndex = 2;
        this.Temperature3.Text = "label8";
        // 
        // Temperature2
        // 
        this.Temperature2.Location = new System.Drawing.Point(169, 125);
        this.Temperature2.Name = "Temperature2";
        this.Temperature2.Size = new System.Drawing.Size(91, 16);
        this.Temperature2.TabIndex = 2;
        this.Temperature2.Text = "label8";
        // 
        // Temperature1
        // 
        this.Temperature1.Location = new System.Drawing.Point(169, 104);
        this.Temperature1.Name = "Temperature1";
        this.Temperature1.Size = new System.Drawing.Size(91, 16);
        this.Temperature1.TabIndex = 2;
        this.Temperature1.Text = "label8";
        // 
        // Temperature0
        // 
        this.Temperature0.Location = new System.Drawing.Point(169, 83);
        this.Temperature0.Name = "Temperature0";
        this.Temperature0.Size = new System.Drawing.Size(91, 16);
        this.Temperature0.TabIndex = 2;
        this.Temperature0.Text = "label8";
        // 
        // MotorControllerTemperature
        // 
        this.MotorControllerTemperature.Location = new System.Drawing.Point(169, 63);
        this.MotorControllerTemperature.Name = "MotorControllerTemperature";
        this.MotorControllerTemperature.Size = new System.Drawing.Size(91, 16);
        this.MotorControllerTemperature.TabIndex = 2;
        this.MotorControllerTemperature.Text = "label8";
        // 
        // BatteryTemperature
        // 
        this.BatteryTemperature.Location = new System.Drawing.Point(169, 44);
        this.BatteryTemperature.Name = "BatteryTemperature";
        this.BatteryTemperature.Size = new System.Drawing.Size(91, 16);
        this.BatteryTemperature.TabIndex = 2;
        this.BatteryTemperature.Text = "label8";
        // 
        // label7
        // 
        this.label7.AutoSize = true;
        this.label7.Location = new System.Drawing.Point(9, 44);
        this.label7.Name = "label7";
        this.label7.Size = new System.Drawing.Size(106, 13);
        this.label7.TabIndex = 1;
        this.label7.Text = "Battery Temperature:";
        // 
        // label6
        // 
        this.label6.AutoSize = true;
        this.label6.Location = new System.Drawing.Point(9, 25);
        this.label6.Name = "label6";
        this.label6.Size = new System.Drawing.Size(109, 13);
        this.label6.TabIndex = 0;
        this.label6.Text = "Chassis Temperature:";
        // 
        // groupBox2
        // 
        this.groupBox2.Controls.Add(this.Heading);
        this.groupBox2.Controls.Add(this.label19);
        this.groupBox2.Controls.Add(this.label3);
        this.groupBox2.Controls.Add(this.Location);
        this.groupBox2.Controls.Add(this.TrackRightSpeed);
        this.groupBox2.Controls.Add(this.TrackLeftSpeed);
        this.groupBox2.Controls.Add(this.TrackRightEncoder);
        this.groupBox2.Controls.Add(this.TrackLeftEncoder);
        this.groupBox2.Controls.Add(this.Orientation);
        this.groupBox2.Controls.Add(this.label5);
        this.groupBox2.Controls.Add(this.label12);
        this.groupBox2.Controls.Add(this.label2);
        this.groupBox2.Location = new System.Drawing.Point(301, 341);
        this.groupBox2.Name = "groupBox2";
        this.groupBox2.Size = new System.Drawing.Size(281, 220);
        this.groupBox2.TabIndex = 30;
        this.groupBox2.TabStop = false;
        this.groupBox2.Text = "Navigation";
        // 
        // Heading
        // 
        this.Heading.Location = new System.Drawing.Point(140, 54);
        this.Heading.Name = "Heading";
        this.Heading.Size = new System.Drawing.Size(97, 16);
        this.Heading.TabIndex = 5;
        this.Heading.Text = "label13";
        // 
        // label19
        // 
        this.label19.AutoSize = true;
        this.label19.Location = new System.Drawing.Point(12, 115);
        this.label19.Name = "label19";
        this.label19.Size = new System.Drawing.Size(72, 13);
        this.label19.TabIndex = 4;
        this.label19.Text = "Track Speed:";
        // 
        // label3
        // 
        this.label3.AutoSize = true;
        this.label3.Location = new System.Drawing.Point(12, 74);
        this.label3.Name = "label3";
        this.label3.Size = new System.Drawing.Size(85, 13);
        this.label3.TabIndex = 3;
        this.label3.Text = "Motor Encoders:";
        // 
        // Location
        // 
        this.Location.Location = new System.Drawing.Point(140, 19);
        this.Location.Name = "Location";
        this.Location.Size = new System.Drawing.Size(97, 16);
        this.Location.TabIndex = 2;
        this.Location.Text = "label13";
        // 
        // TrackRightSpeed
        // 
        this.TrackRightSpeed.Location = new System.Drawing.Point(140, 132);
        this.TrackRightSpeed.Name = "TrackRightSpeed";
        this.TrackRightSpeed.Size = new System.Drawing.Size(97, 16);
        this.TrackRightSpeed.TabIndex = 2;
        this.TrackRightSpeed.Text = "label13";
        // 
        // TrackLeftSpeed
        // 
        this.TrackLeftSpeed.Location = new System.Drawing.Point(140, 112);
        this.TrackLeftSpeed.Name = "TrackLeftSpeed";
        this.TrackLeftSpeed.Size = new System.Drawing.Size(97, 16);
        this.TrackLeftSpeed.TabIndex = 2;
        this.TrackLeftSpeed.Text = "label13";
        // 
        // TrackRightEncoder
        // 
        this.TrackRightEncoder.Location = new System.Drawing.Point(140, 90);
        this.TrackRightEncoder.Name = "TrackRightEncoder";
        this.TrackRightEncoder.Size = new System.Drawing.Size(97, 16);
        this.TrackRightEncoder.TabIndex = 2;
        this.TrackRightEncoder.Text = "label13";
        // 
        // TrackLeftEncoder
        // 
        this.TrackLeftEncoder.Location = new System.Drawing.Point(140, 71);
        this.TrackLeftEncoder.Name = "TrackLeftEncoder";
        this.TrackLeftEncoder.Size = new System.Drawing.Size(97, 16);
        this.TrackLeftEncoder.TabIndex = 2;
        this.TrackLeftEncoder.Text = "label13";
        // 
        // Orientation
        // 
        this.Orientation.Location = new System.Drawing.Point(140, 37);
        this.Orientation.Name = "Orientation";
        this.Orientation.Size = new System.Drawing.Size(97, 16);
        this.Orientation.TabIndex = 2;
        this.Orientation.Text = "label13";
        // 
        // label5
        // 
        this.label5.AutoSize = true;
        this.label5.Location = new System.Drawing.Point(12, 57);
        this.label5.Name = "label5";
        this.label5.Size = new System.Drawing.Size(50, 13);
        this.label5.TabIndex = 1;
        this.label5.Text = "Heading:";
        // 
        // label12
        // 
        this.label12.AutoSize = true;
        this.label12.Location = new System.Drawing.Point(12, 40);
        this.label12.Name = "label12";
        this.label12.Size = new System.Drawing.Size(61, 13);
        this.label12.TabIndex = 1;
        this.label12.Text = "Orientation:";
        // 
        // label2
        // 
        this.label2.AutoSize = true;
        this.label2.Location = new System.Drawing.Point(12, 22);
        this.label2.Name = "label2";
        this.label2.Size = new System.Drawing.Size(51, 13);
        this.label2.TabIndex = 0;
        this.label2.Text = "Location:";
        // 
        // groupBox4
        // 
        this.groupBox4.Controls.Add(this.RobotUptime);
        this.groupBox4.Controls.Add(this.label30);
        this.groupBox4.Controls.Add(this.CurrentRobotTime);
        this.groupBox4.Controls.Add(this.label17);
        this.groupBox4.Controls.Add(this.LastTelemetry);
        this.groupBox4.Controls.Add(this.ConnectionLatency);
        this.groupBox4.Controls.Add(this.ConnectionStatus);
        this.groupBox4.Controls.Add(this.Host);
        this.groupBox4.Controls.Add(this.Connect);
        this.groupBox4.Controls.Add(this.label9);
        this.groupBox4.Controls.Add(this.label8);
        this.groupBox4.Controls.Add(this.label4);
        this.groupBox4.Controls.Add(this.label1);
        this.groupBox4.Location = new System.Drawing.Point(8, 8);
        this.groupBox4.Name = "groupBox4";
        this.groupBox4.Size = new System.Drawing.Size(281, 187);
        this.groupBox4.TabIndex = 32;
        this.groupBox4.TabStop = false;
        this.groupBox4.Text = "Connection";
        // 
        // RobotUptime
        // 
        this.RobotUptime.Location = new System.Drawing.Point(140, 152);
        this.RobotUptime.Name = "RobotUptime";
        this.RobotUptime.Size = new System.Drawing.Size(129, 16);
        this.RobotUptime.TabIndex = 9;
        this.RobotUptime.Text = "label11";
        // 
        // label30
        // 
        this.label30.AutoSize = true;
        this.label30.Location = new System.Drawing.Point(11, 155);
        this.label30.Name = "label30";
        this.label30.Size = new System.Drawing.Size(75, 13);
        this.label30.TabIndex = 8;
        this.label30.Text = "Robot Uptime:";
        // 
        // CurrentRobotTime
        // 
        this.CurrentRobotTime.Location = new System.Drawing.Point(140, 129);
        this.CurrentRobotTime.Name = "CurrentRobotTime";
        this.CurrentRobotTime.Size = new System.Drawing.Size(129, 16);
        this.CurrentRobotTime.TabIndex = 7;
        this.CurrentRobotTime.Text = "label11";
        // 
        // label17
        // 
        this.label17.AutoSize = true;
        this.label17.Location = new System.Drawing.Point(11, 132);
        this.label17.Name = "label17";
        this.label17.Size = new System.Drawing.Size(102, 13);
        this.label17.TabIndex = 6;
        this.label17.Text = "Current Robot Time:";
        // 
        // LastTelemetry
        // 
        this.LastTelemetry.Location = new System.Drawing.Point(140, 108);
        this.LastTelemetry.Name = "LastTelemetry";
        this.LastTelemetry.Size = new System.Drawing.Size(129, 16);
        this.LastTelemetry.TabIndex = 5;
        this.LastTelemetry.Text = "label11";
        // 
        // ConnectionLatency
        // 
        this.ConnectionLatency.Location = new System.Drawing.Point(140, 87);
        this.ConnectionLatency.Name = "ConnectionLatency";
        this.ConnectionLatency.Size = new System.Drawing.Size(112, 21);
        this.ConnectionLatency.TabIndex = 5;
        // 
        // ConnectionStatus
        // 
        this.ConnectionStatus.Location = new System.Drawing.Point(140, 36);
        this.ConnectionStatus.Name = "ConnectionStatus";
        this.ConnectionStatus.Size = new System.Drawing.Size(97, 16);
        this.ConnectionStatus.TabIndex = 5;
        this.ConnectionStatus.Text = "label11";
        // 
        // Host
        // 
        this.Host.Location = new System.Drawing.Point(140, 16);
        this.Host.Name = "Host";
        this.Host.Size = new System.Drawing.Size(129, 17);
        this.Host.TabIndex = 5;
        this.Host.Text = "label11";
        // 
        // Connect
        // 
        this.Connect.Location = new System.Drawing.Point(28, 57);
        this.Connect.Name = "Connect";
        this.Connect.Size = new System.Drawing.Size(75, 23);
        this.Connect.TabIndex = 4;
        this.Connect.Text = "Disconnect";
        this.Connect.UseVisualStyleBackColor = true;
        this.Connect.Click += new System.EventHandler(this.Connect_Click);
        // 
        // label9
        // 
        this.label9.AutoSize = true;
        this.label9.Location = new System.Drawing.Point(10, 90);
        this.label9.Name = "label9";
        this.label9.Size = new System.Drawing.Size(105, 13);
        this.label9.TabIndex = 3;
        this.label9.Text = "Connection Latency:";
        // 
        // label8
        // 
        this.label8.AutoSize = true;
        this.label8.Location = new System.Drawing.Point(11, 111);
        this.label8.Name = "label8";
        this.label8.Size = new System.Drawing.Size(79, 13);
        this.label8.TabIndex = 2;
        this.label8.Text = "Last Telemetry:";
        // 
        // label4
        // 
        this.label4.AutoSize = true;
        this.label4.Location = new System.Drawing.Point(10, 39);
        this.label4.Name = "label4";
        this.label4.Size = new System.Drawing.Size(97, 13);
        this.label4.TabIndex = 1;
        this.label4.Text = "Connection Status:";
        // 
        // label1
        // 
        this.label1.AutoSize = true;
        this.label1.Location = new System.Drawing.Point(10, 20);
        this.label1.Name = "label1";
        this.label1.Size = new System.Drawing.Size(32, 13);
        this.label1.TabIndex = 0;
        this.label1.Text = "Host:";
        // 
        // groupBox5
        // 
        this.groupBox5.Controls.Add(this.PathControl);
        this.groupBox5.Controls.Add(this.JoystickPosition);
        this.groupBox5.Controls.Add(this.JoystickControl);
        this.groupBox5.Controls.Add(this.CommandControl);
        this.groupBox5.Controls.Add(this.DrivingStatus);
        this.groupBox5.Controls.Add(this.label16);
        this.groupBox5.Controls.Add(this.Turn);
        this.groupBox5.Controls.Add(this.TurnDistance);
        this.groupBox5.Controls.Add(this.DriveDistance);
        this.groupBox5.Controls.Add(this.label15);
        this.groupBox5.Controls.Add(this.label14);
        this.groupBox5.Controls.Add(this.label13);
        this.groupBox5.Controls.Add(this.label11);
        this.groupBox5.Controls.Add(this.Drive);
        this.groupBox5.Location = new System.Drawing.Point(301, 9);
        this.groupBox5.Name = "groupBox5";
        this.groupBox5.Size = new System.Drawing.Size(281, 326);
        this.groupBox5.TabIndex = 33;
        this.groupBox5.TabStop = false;
        this.groupBox5.Text = "Driving";
        // 
        // PathControl
        // 
        this.PathControl.AutoSize = true;
        this.PathControl.Enabled = false;
        this.PathControl.Location = new System.Drawing.Point(17, 46);
        this.PathControl.Name = "PathControl";
        this.PathControl.Size = new System.Drawing.Size(112, 17);
        this.PathControl.TabIndex = 15;
        this.PathControl.TabStop = true;
        this.PathControl.Text = "Autonomous Path ";
        this.PathControl.UseVisualStyleBackColor = true;
        // 
        // JoystickPosition
        // 
        this.JoystickPosition.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
        this.JoystickPosition.Location = new System.Drawing.Point(35, 205);
        this.JoystickPosition.Name = "JoystickPosition";
        this.JoystickPosition.Size = new System.Drawing.Size(115, 109);
        this.JoystickPosition.TabIndex = 14;
        this.JoystickPosition.TabStop = false;
        this.JoystickPosition.Paint += new System.Windows.Forms.PaintEventHandler(this.JoystickPosition_Paint);
        // 
        // JoystickControl
        // 
        this.JoystickControl.AutoSize = true;
        this.JoystickControl.Location = new System.Drawing.Point(17, 182);
        this.JoystickControl.Name = "JoystickControl";
        this.JoystickControl.Size = new System.Drawing.Size(99, 17);
        this.JoystickControl.TabIndex = 13;
        this.JoystickControl.Text = "Joystick Control";
        this.JoystickControl.UseVisualStyleBackColor = true;
        this.JoystickControl.CheckedChanged += new System.EventHandler(this.JoystickControl_CheckedChanged);
        // 
        // CommandControl
        // 
        this.CommandControl.AutoSize = true;
        this.CommandControl.Checked = true;
        this.CommandControl.Location = new System.Drawing.Point(17, 66);
        this.CommandControl.Name = "CommandControl";
        this.CommandControl.Size = new System.Drawing.Size(108, 17);
        this.CommandControl.TabIndex = 12;
        this.CommandControl.TabStop = true;
        this.CommandControl.Text = "Command Control";
        this.CommandControl.UseVisualStyleBackColor = true;
        this.CommandControl.CheckedChanged += new System.EventHandler(this.CommandControl_CheckedChanged);
        // 
        // DrivingStatus
        // 
        this.DrivingStatus.AutoSize = true;
        this.DrivingStatus.Location = new System.Drawing.Point(78, 23);
        this.DrivingStatus.Name = "DrivingStatus";
        this.DrivingStatus.Size = new System.Drawing.Size(41, 13);
        this.DrivingStatus.TabIndex = 9;
        this.DrivingStatus.Text = "label17";
        // 
        // label16
        // 
        this.label16.AutoSize = true;
        this.label16.Location = new System.Drawing.Point(15, 23);
        this.label16.Name = "label16";
        this.label16.Size = new System.Drawing.Size(40, 13);
        this.label16.TabIndex = 8;
        this.label16.Text = "Status:";
        // 
        // Turn
        // 
        this.Turn.Location = new System.Drawing.Point(157, 146);
        this.Turn.Name = "Turn";
        this.Turn.Size = new System.Drawing.Size(39, 23);
        this.Turn.TabIndex = 7;
        this.Turn.Text = "Go";
        this.Turn.UseVisualStyleBackColor = true;
        this.Turn.Click += new System.EventHandler(this.Turn_Click);
        // 
        // TurnDistance
        // 
        this.TurnDistance.Location = new System.Drawing.Point(102, 154);
        this.TurnDistance.Name = "TurnDistance";
        this.TurnDistance.Size = new System.Drawing.Size(49, 20);
        this.TurnDistance.TabIndex = 6;
        // 
        // DriveDistance
        // 
        this.DriveDistance.Location = new System.Drawing.Point(102, 107);
        this.DriveDistance.Name = "DriveDistance";
        this.DriveDistance.Size = new System.Drawing.Size(49, 20);
        this.DriveDistance.TabIndex = 5;
        // 
        // label15
        // 
        this.label15.AutoSize = true;
        this.label15.Location = new System.Drawing.Point(50, 151);
        this.label15.Name = "label15";
        this.label15.Size = new System.Drawing.Size(50, 13);
        this.label15.TabIndex = 4;
        this.label15.Text = "Degrees:";
        // 
        // label14
        // 
        this.label14.AutoSize = true;
        this.label14.Location = new System.Drawing.Point(54, 110);
        this.label14.Name = "label14";
        this.label14.Size = new System.Drawing.Size(34, 13);
        this.label14.TabIndex = 3;
        this.label14.Text = "Units:";
        // 
        // label13
        // 
        this.label13.AutoSize = true;
        this.label13.Location = new System.Drawing.Point(33, 130);
        this.label13.Name = "label13";
        this.label13.Size = new System.Drawing.Size(32, 13);
        this.label13.TabIndex = 2;
        this.label13.Text = "Turn:";
        // 
        // label11
        // 
        this.label11.AutoSize = true;
        this.label11.Location = new System.Drawing.Point(33, 86);
        this.label11.Name = "label11";
        this.label11.Size = new System.Drawing.Size(99, 13);
        this.label11.TabIndex = 1;
        this.label11.Text = "Forward / Reverse:";
        // 
        // Drive
        // 
        this.Drive.Location = new System.Drawing.Point(157, 105);
        this.Drive.Name = "Drive";
        this.Drive.Size = new System.Drawing.Size(39, 23);
        this.Drive.TabIndex = 0;
        this.Drive.Text = "Go";
        this.Drive.UseVisualStyleBackColor = true;
        this.Drive.Click += new System.EventHandler(this.Drive_Click);
        // 
        // JoystickTimer
        // 
        this.JoystickTimer.Interval = 25;
        this.JoystickTimer.Tick += new System.EventHandler(this.JoystickTimer_Tick);
        // 
        // SlowUpdateTimer
        // 
        this.SlowUpdateTimer.Interval = 5000;
        this.SlowUpdateTimer.Tick += new System.EventHandler(this.SlowUpdateTimer_Tick);
        // 
        // groupBox7
        // 
        this.groupBox7.Controls.Add(this.label27);
        this.groupBox7.Controls.Add(this.label26);
        this.groupBox7.Controls.Add(this.label25);
        this.groupBox7.Controls.Add(this.label24);
        this.groupBox7.Controls.Add(this.button2);
        this.groupBox7.Controls.Add(this.button1);
        this.groupBox7.Controls.Add(this.button4);
        this.groupBox7.Controls.Add(this.button3);
        this.groupBox7.Location = new System.Drawing.Point(8, 201);
        this.groupBox7.Name = "groupBox7";
        this.groupBox7.Size = new System.Drawing.Size(281, 250);
        this.groupBox7.TabIndex = 37;
        this.groupBox7.TabStop = false;
        this.groupBox7.Text = "Subsystem Control";
        // 
        // label27
        // 
        this.label27.AutoSize = true;
        this.label27.Location = new System.Drawing.Point(15, 189);
        this.label27.Name = "label27";
        this.label27.Size = new System.Drawing.Size(126, 13);
        this.label27.TabIndex = 2;
        this.label27.Text = "Robot Navigation Status:";
        // 
        // label26
        // 
        this.label26.AutoSize = true;
        this.label26.Location = new System.Drawing.Point(15, 130);
        this.label26.Name = "label26";
        this.label26.Size = new System.Drawing.Size(120, 13);
        this.label26.TabIndex = 2;
        this.label26.Text = "Robot Scanning Status:";
        // 
        // label25
        // 
        this.label25.AutoSize = true;
        this.label25.Location = new System.Drawing.Point(15, 76);
        this.label25.Name = "label25";
        this.label25.Size = new System.Drawing.Size(103, 13);
        this.label25.TabIndex = 2;
        this.label25.Text = "Robot Vision Status:";
        // 
        // label24
        // 
        this.label24.AutoSize = true;
        this.label24.Location = new System.Drawing.Point(15, 22);
        this.label24.Name = "label24";
        this.label24.Size = new System.Drawing.Size(107, 13);
        this.label24.TabIndex = 2;
        this.label24.Text = "Robot Motion Status:";
        // 
        // button2
        // 
        this.button2.Location = new System.Drawing.Point(33, 206);
        this.button2.Name = "button2";
        this.button2.Size = new System.Drawing.Size(70, 23);
        this.button2.TabIndex = 1;
        this.button2.Text = "Reset";
        this.button2.UseVisualStyleBackColor = true;
        // 
        // button1
        // 
        this.button1.Location = new System.Drawing.Point(33, 147);
        this.button1.Name = "button1";
        this.button1.Size = new System.Drawing.Size(70, 23);
        this.button1.TabIndex = 1;
        this.button1.Text = "Reset";
        this.button1.UseVisualStyleBackColor = true;
        // 
        // button4
        // 
        this.button4.Location = new System.Drawing.Point(33, 93);
        this.button4.Name = "button4";
        this.button4.Size = new System.Drawing.Size(70, 23);
        this.button4.TabIndex = 1;
        this.button4.Text = "Reset";
        this.button4.UseVisualStyleBackColor = true;
        // 
        // button3
        // 
        this.button3.Location = new System.Drawing.Point(33, 41);
        this.button3.Name = "button3";
        this.button3.Size = new System.Drawing.Size(70, 23);
        this.button3.TabIndex = 1;
        this.button3.Text = "Reset";
        this.button3.UseVisualStyleBackColor = true;
        // 
        // Navigation
        // 
        this.Navigation.Location = new System.Drawing.Point(8, 677);
        this.Navigation.Name = "Navigation";
        this.Navigation.Size = new System.Drawing.Size(122, 23);
        this.Navigation.TabIndex = 39;
        this.Navigation.Text = "Navigation...";
        this.Navigation.UseVisualStyleBackColor = true;
        // 
        // DebugMessages
        // 
        this.DebugMessages.Location = new System.Drawing.Point(8, 648);
        this.DebugMessages.Name = "DebugMessages";
        this.DebugMessages.Size = new System.Drawing.Size(122, 23);
        this.DebugMessages.TabIndex = 40;
        this.DebugMessages.Text = "Debug Messages...";
        this.DebugMessages.UseVisualStyleBackColor = true;
        this.DebugMessages.Click += new System.EventHandler(this.DebugMessages_Click);
        // 
        // Stop
        // 
        this.Stop.Font = new System.Drawing.Font("Microsoft Sans Serif", 15.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
        this.Stop.Location = new System.Drawing.Point(8, 706);
        this.Stop.Name = "Stop";
        this.Stop.Size = new System.Drawing.Size(122, 89);
        this.Stop.TabIndex = 41;
        this.Stop.Text = "Stop";
        this.Stop.UseVisualStyleBackColor = true;
        this.Stop.Click += new System.EventHandler(this.Stop_Click);
        // 
        // MainFrame
        // 
        this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
        this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
        this.BackColor = System.Drawing.SystemColors.Control;
        this.ClientSize = new System.Drawing.Size(589, 673);
        this.Controls.Add(this.Stop);
        this.Controls.Add(this.DebugMessages);
        this.Controls.Add(this.Navigation);
        this.Controls.Add(this.groupBox7);
        this.Controls.Add(this.groupBox5);
        this.Controls.Add(this.groupBox4);
        this.Controls.Add(this.groupBox2);
        this.Controls.Add(this.groupBox1);
        this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
        this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
        this.MaximizeBox = false;
        this.Name = "MainFrame";
        this.Text = "Robot Information and Command";
        this.WindowState = System.Windows.Forms.FormWindowState.Minimized;
        this.Load += new System.EventHandler(this.MainFrame_Load);
        this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.MainFrame_FormClosing);
        this.groupBox1.ResumeLayout(false);
        this.groupBox1.PerformLayout();
        this.groupBox2.ResumeLayout(false);
        this.groupBox2.PerformLayout();
        this.groupBox4.ResumeLayout(false);
        this.groupBox4.PerformLayout();
        this.groupBox5.ResumeLayout(false);
        this.groupBox5.PerformLayout();
        ((System.ComponentModel.ISupportInitialize)(this.JoystickPosition)).EndInit();
        this.groupBox7.ResumeLayout(false);
        this.groupBox7.PerformLayout();
        this.ResumeLayout(false);

    }

      private object super() {
          throw new System.Exception("The method or operation is not implemented.");
      }

    #endregion

    private System.Windows.Forms.GroupBox groupBox1;
    private System.Windows.Forms.Label label7;
    private System.Windows.Forms.Label label6;
    private System.Windows.Forms.Label ChassisTemperature;
    private System.Windows.Forms.Label BatteryTemperature;
    private System.Windows.Forms.GroupBox groupBox2;
    private System.Windows.Forms.Label label3;
    private System.Windows.Forms.Label Location;
    private System.Windows.Forms.Label TrackLeftEncoder;
    private System.Windows.Forms.Label Orientation;
    private System.Windows.Forms.Label label12;
    private System.Windows.Forms.Label label2;
    private System.Windows.Forms.Label TrackRightEncoder;
    private System.Windows.Forms.GroupBox groupBox4;
    private System.Windows.Forms.Label LastTelemetry;
    private System.Windows.Forms.Label ConnectionLatency;
    private System.Windows.Forms.Label ConnectionStatus;
    private System.Windows.Forms.Label Host;
    private System.Windows.Forms.Button Connect;
    private System.Windows.Forms.Label label9;
    private System.Windows.Forms.Label label8;
    private System.Windows.Forms.Label label4;
    private System.Windows.Forms.Label label1;
    private System.Windows.Forms.Timer FastRefreshTimer;
    private System.Windows.Forms.GroupBox groupBox5;
    private System.Windows.Forms.Button Drive;
    private System.Windows.Forms.Label DrivingStatus;
    private System.Windows.Forms.Label label16;
    private System.Windows.Forms.Button Turn;
    private System.Windows.Forms.TextBox TurnDistance;
    private System.Windows.Forms.TextBox DriveDistance;
    private System.Windows.Forms.Label label15;
    private System.Windows.Forms.Label label14;
    private System.Windows.Forms.Label label13;
    private System.Windows.Forms.Label label11;
    private System.Windows.Forms.Label label18;
    private System.Windows.Forms.Label MotorControllerTemperature;
    private System.Windows.Forms.RadioButton JoystickControl;
    private System.Windows.Forms.RadioButton CommandControl;
    private System.Windows.Forms.Label label22;
    private System.Windows.Forms.Label label20;
    private System.Windows.Forms.Label Temperature1;
    private System.Windows.Forms.Label Temperature0;
    private System.Windows.Forms.PictureBox JoystickPosition;
    private System.Windows.Forms.Timer JoystickTimer;
    private System.Windows.Forms.Label label19;
    private System.Windows.Forms.Label TrackRightSpeed;
    private System.Windows.Forms.Label TrackLeftSpeed;
    private System.Windows.Forms.Label label23;
    private System.Windows.Forms.Label label21;
    private System.Windows.Forms.Label Temperature3;
    private System.Windows.Forms.Label Temperature2;
    private System.Windows.Forms.Timer SlowUpdateTimer;
    private System.Windows.Forms.RadioButton PathControl;
    private System.Windows.Forms.GroupBox groupBox7;
    private System.Windows.Forms.Button button3;
    private System.Windows.Forms.Label label27;
    private System.Windows.Forms.Label label26;
    private System.Windows.Forms.Label label25;
    private System.Windows.Forms.Label label24;
    private System.Windows.Forms.Button button2;
    private System.Windows.Forms.Button button1;
    private System.Windows.Forms.Button button4;
    private System.Windows.Forms.Button Navigation;
    private System.Windows.Forms.Button DebugMessages;
    private System.Windows.Forms.Label RobotUptime;
    private System.Windows.Forms.Label label30;
    private System.Windows.Forms.Label CurrentRobotTime;
    private System.Windows.Forms.Label label17;
    private System.Windows.Forms.Button Stop;
    private System.Windows.Forms.Label Heading;
    private System.Windows.Forms.Label label5;
  }
}