namespace Controller
{
    partial class Control
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(Control));
            this.statusStrip1 = new System.Windows.Forms.StatusStrip();
            this.WorldPositionLabel = new System.Windows.Forms.ToolStripStatusLabel();
            this.MapPositionLabel = new System.Windows.Forms.ToolStripStatusLabel();
            this.tableLayoutPanel1 = new System.Windows.Forms.TableLayoutPanel();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.tableLayoutPanel2 = new System.Windows.Forms.TableLayoutPanel();
            this.driveDistance = new System.Windows.Forms.TextBox();
            this.distanceMoved = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.percentageMoved = new System.Windows.Forms.Label();
            this.button1 = new System.Windows.Forms.Button();
            this.button2 = new System.Windows.Forms.Button();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.tableLayoutPanel3 = new System.Windows.Forms.TableLayoutPanel();
            this.degreesTurned = new System.Windows.Forms.Label();
            this.label6 = new System.Windows.Forms.Label();
            this.label7 = new System.Windows.Forms.Label();
            this.label8 = new System.Windows.Forms.Label();
            this.percentageTurned = new System.Windows.Forms.Label();
            this.button3 = new System.Windows.Forms.Button();
            this.button4 = new System.Windows.Forms.Button();
            this.turnAngle = new System.Windows.Forms.TextBox();
            this.tabControl1 = new System.Windows.Forms.TabControl();
            this.tabPage1 = new System.Windows.Forms.TabPage();
            this.tableLayoutPanel4 = new System.Windows.Forms.TableLayoutPanel();
            this.MapCanvas = new System.Windows.Forms.PictureBox();
            this.toolStrip1 = new System.Windows.Forms.ToolStrip();
            this.ZoomInCommand = new System.Windows.Forms.ToolStripButton();
            this.ZoomOutCommand = new System.Windows.Forms.ToolStripButton();
            this.toolStripSeparator1 = new System.Windows.Forms.ToolStripSeparator();
            this.ClearPathCommand = new System.Windows.Forms.ToolStripButton();
            this.AddWaypointCommand = new System.Windows.Forms.ToolStripButton();
            this.DeleteWaypointCommand = new System.Windows.Forms.ToolStripButton();
            this.MoveWaypointCommand = new System.Windows.Forms.ToolStripButton();
            this.toolStripSeparator2 = new System.Windows.Forms.ToolStripSeparator();
            this.toolStripDropDownButton1 = new System.Windows.Forms.ToolStripDropDownButton();
            this.loadMapToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.loadPathToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripSplitButton1 = new System.Windows.Forms.ToolStripDropDownButton();
            this.saveMapToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.savePathToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripSeparator3 = new System.Windows.Forms.ToolStripSeparator();
            this.StopDrivingCommand = new System.Windows.Forms.ToolStripButton();
            this.DriveCommand = new System.Windows.Forms.ToolStripButton();
            this.ViewRobotCommand = new System.Windows.Forms.ToolStripButton();
            this.tabPage2 = new System.Windows.Forms.TabPage();
            this.tableLayoutPanel5 = new System.Windows.Forms.TableLayoutPanel();
            this.updateFieldsTimer = new System.Windows.Forms.Timer(this.components);
            this.statusTimer = new System.Windows.Forms.Timer(this.components);
            this.openFileDialog1 = new System.Windows.Forms.OpenFileDialog();
            this.saveFileDialog1 = new System.Windows.Forms.SaveFileDialog();
            this.button5 = new System.Windows.Forms.Button();
            this.button6 = new System.Windows.Forms.Button();
            this.statusStrip1.SuspendLayout();
            this.tableLayoutPanel1.SuspendLayout();
            this.groupBox1.SuspendLayout();
            this.tableLayoutPanel2.SuspendLayout();
            this.groupBox2.SuspendLayout();
            this.tableLayoutPanel3.SuspendLayout();
            this.tabControl1.SuspendLayout();
            this.tabPage1.SuspendLayout();
            this.tableLayoutPanel4.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.MapCanvas)).BeginInit();
            this.toolStrip1.SuspendLayout();
            this.tabPage2.SuspendLayout();
            this.tableLayoutPanel5.SuspendLayout();
            this.SuspendLayout();
            // 
            // statusStrip1
            // 
            this.statusStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.WorldPositionLabel,
            this.MapPositionLabel});
            this.statusStrip1.Location = new System.Drawing.Point(0, 399);
            this.statusStrip1.Name = "statusStrip1";
            this.statusStrip1.Size = new System.Drawing.Size(1006, 22);
            this.statusStrip1.TabIndex = 0;
            this.statusStrip1.Text = "statusStrip1";
            // 
            // WorldPositionLabel
            // 
            this.WorldPositionLabel.Name = "WorldPositionLabel";
            this.WorldPositionLabel.Size = new System.Drawing.Size(17, 17);
            this.WorldPositionLabel.Text = "W";
            // 
            // MapPositionLabel
            // 
            this.MapPositionLabel.Name = "MapPositionLabel";
            this.MapPositionLabel.Size = new System.Drawing.Size(12, 17);
            this.MapPositionLabel.Text = "L";
            // 
            // tableLayoutPanel1
            // 
            this.tableLayoutPanel1.AutoSize = true;
            this.tableLayoutPanel1.ColumnCount = 2;
            this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 23.16103F));
            this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 76.83897F));
            this.tableLayoutPanel1.Controls.Add(this.groupBox1, 0, 0);
            this.tableLayoutPanel1.Controls.Add(this.groupBox2, 0, 1);
            this.tableLayoutPanel1.Controls.Add(this.tabControl1, 1, 0);
            this.tableLayoutPanel1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tableLayoutPanel1.Location = new System.Drawing.Point(0, 0);
            this.tableLayoutPanel1.Name = "tableLayoutPanel1";
            this.tableLayoutPanel1.Padding = new System.Windows.Forms.Padding(0, 5, 0, 0);
            this.tableLayoutPanel1.RowCount = 3;
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 180F));
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 180F));
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 20F));
            this.tableLayoutPanel1.Size = new System.Drawing.Size(1006, 399);
            this.tableLayoutPanel1.TabIndex = 1;
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.tableLayoutPanel2);
            this.groupBox1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.groupBox1.Font = new System.Drawing.Font("Tahoma", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.groupBox1.Location = new System.Drawing.Point(3, 8);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(226, 174);
            this.groupBox1.TabIndex = 2;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Linear Movement";
            // 
            // tableLayoutPanel2
            // 
            this.tableLayoutPanel2.ColumnCount = 3;
            this.tableLayoutPanel2.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 40F));
            this.tableLayoutPanel2.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 30F));
            this.tableLayoutPanel2.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 30F));
            this.tableLayoutPanel2.Controls.Add(this.driveDistance, 0, 0);
            this.tableLayoutPanel2.Controls.Add(this.distanceMoved, 1, 1);
            this.tableLayoutPanel2.Controls.Add(this.label2, 0, 0);
            this.tableLayoutPanel2.Controls.Add(this.label3, 0, 1);
            this.tableLayoutPanel2.Controls.Add(this.label4, 0, 2);
            this.tableLayoutPanel2.Controls.Add(this.percentageMoved, 1, 2);
            this.tableLayoutPanel2.Controls.Add(this.button1, 2, 0);
            this.tableLayoutPanel2.Controls.Add(this.button2, 0, 3);
            this.tableLayoutPanel2.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tableLayoutPanel2.Location = new System.Drawing.Point(3, 18);
            this.tableLayoutPanel2.Name = "tableLayoutPanel2";
            this.tableLayoutPanel2.RowCount = 3;
            this.tableLayoutPanel2.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 23.52941F));
            this.tableLayoutPanel2.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 23.52941F));
            this.tableLayoutPanel2.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 23.52941F));
            this.tableLayoutPanel2.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 29.41176F));
            this.tableLayoutPanel2.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 20F));
            this.tableLayoutPanel2.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 20F));
            this.tableLayoutPanel2.Size = new System.Drawing.Size(220, 153);
            this.tableLayoutPanel2.TabIndex = 0;
            // 
            // driveDistance
            // 
            this.driveDistance.Dock = System.Windows.Forms.DockStyle.Fill;
            this.driveDistance.Location = new System.Drawing.Point(88, 7);
            this.driveDistance.Margin = new System.Windows.Forms.Padding(0, 7, 0, 0);
            this.driveDistance.MaxLength = 5;
            this.driveDistance.Name = "driveDistance";
            this.driveDistance.Size = new System.Drawing.Size(66, 22);
            this.driveDistance.TabIndex = 11;
            this.driveDistance.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
            // 
            // distanceMoved
            // 
            this.distanceMoved.AutoSize = true;
            this.tableLayoutPanel2.SetColumnSpan(this.distanceMoved, 2);
            this.distanceMoved.Dock = System.Windows.Forms.DockStyle.Fill;
            this.distanceMoved.Font = new System.Drawing.Font("Tahoma", 14.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.distanceMoved.Location = new System.Drawing.Point(91, 36);
            this.distanceMoved.Name = "distanceMoved";
            this.distanceMoved.Size = new System.Drawing.Size(126, 36);
            this.distanceMoved.TabIndex = 9;
            this.distanceMoved.Text = "0.0 cm";
            this.distanceMoved.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Dock = System.Windows.Forms.DockStyle.Fill;
            this.label2.Font = new System.Drawing.Font("Tahoma", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label2.Location = new System.Drawing.Point(3, 0);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(82, 36);
            this.label2.TabIndex = 0;
            this.label2.Text = "Distance (m)";
            this.label2.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Dock = System.Windows.Forms.DockStyle.Fill;
            this.label3.Font = new System.Drawing.Font("Tahoma", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label3.Location = new System.Drawing.Point(3, 36);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(82, 36);
            this.label3.TabIndex = 1;
            this.label3.Text = "Distance Moved";
            this.label3.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Dock = System.Windows.Forms.DockStyle.Fill;
            this.label4.Font = new System.Drawing.Font("Tahoma", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label4.Location = new System.Drawing.Point(3, 72);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(82, 36);
            this.label4.TabIndex = 2;
            this.label4.Text = "Percentage Complete";
            this.label4.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // percentageMoved
            // 
            this.percentageMoved.AutoSize = true;
            this.tableLayoutPanel2.SetColumnSpan(this.percentageMoved, 2);
            this.percentageMoved.Dock = System.Windows.Forms.DockStyle.Fill;
            this.percentageMoved.Font = new System.Drawing.Font("Tahoma", 14.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.percentageMoved.Location = new System.Drawing.Point(91, 72);
            this.percentageMoved.Name = "percentageMoved";
            this.percentageMoved.Size = new System.Drawing.Size(126, 36);
            this.percentageMoved.TabIndex = 4;
            this.percentageMoved.Text = "100 %";
            this.percentageMoved.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // button1
            // 
            this.button1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.button1.Location = new System.Drawing.Point(157, 3);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(60, 30);
            this.button1.TabIndex = 5;
            this.button1.Text = "Move";
            this.button1.UseVisualStyleBackColor = true;
            this.button1.Click += new System.EventHandler(this.button1_Click);
            // 
            // button2
            // 
            this.tableLayoutPanel2.SetColumnSpan(this.button2, 3);
            this.button2.Dock = System.Windows.Forms.DockStyle.Fill;
            this.button2.Font = new System.Drawing.Font("Tahoma", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.button2.Location = new System.Drawing.Point(3, 111);
            this.button2.Name = "button2";
            this.button2.Size = new System.Drawing.Size(214, 39);
            this.button2.TabIndex = 7;
            this.button2.Text = "STOP";
            this.button2.UseVisualStyleBackColor = true;
            this.button2.Click += new System.EventHandler(this.button2_Click);
            // 
            // groupBox2
            // 
            this.groupBox2.Controls.Add(this.tableLayoutPanel3);
            this.groupBox2.Dock = System.Windows.Forms.DockStyle.Fill;
            this.groupBox2.Font = new System.Drawing.Font("Tahoma", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.groupBox2.Location = new System.Drawing.Point(3, 188);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(226, 174);
            this.groupBox2.TabIndex = 3;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "Rotational Movement";
            // 
            // tableLayoutPanel3
            // 
            this.tableLayoutPanel3.ColumnCount = 3;
            this.tableLayoutPanel3.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 40F));
            this.tableLayoutPanel3.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 30F));
            this.tableLayoutPanel3.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 30F));
            this.tableLayoutPanel3.Controls.Add(this.degreesTurned, 1, 1);
            this.tableLayoutPanel3.Controls.Add(this.label6, 0, 0);
            this.tableLayoutPanel3.Controls.Add(this.label7, 0, 1);
            this.tableLayoutPanel3.Controls.Add(this.label8, 0, 2);
            this.tableLayoutPanel3.Controls.Add(this.percentageTurned, 1, 2);
            this.tableLayoutPanel3.Controls.Add(this.button3, 2, 0);
            this.tableLayoutPanel3.Controls.Add(this.button4, 0, 3);
            this.tableLayoutPanel3.Controls.Add(this.turnAngle, 1, 0);
            this.tableLayoutPanel3.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tableLayoutPanel3.Location = new System.Drawing.Point(3, 18);
            this.tableLayoutPanel3.Name = "tableLayoutPanel3";
            this.tableLayoutPanel3.RowCount = 4;
            this.tableLayoutPanel3.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 23.52941F));
            this.tableLayoutPanel3.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 23.52941F));
            this.tableLayoutPanel3.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 23.52941F));
            this.tableLayoutPanel3.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 29.41176F));
            this.tableLayoutPanel3.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 20F));
            this.tableLayoutPanel3.Size = new System.Drawing.Size(220, 153);
            this.tableLayoutPanel3.TabIndex = 0;
            // 
            // degreesTurned
            // 
            this.degreesTurned.AutoSize = true;
            this.tableLayoutPanel3.SetColumnSpan(this.degreesTurned, 2);
            this.degreesTurned.Dock = System.Windows.Forms.DockStyle.Fill;
            this.degreesTurned.Font = new System.Drawing.Font("Tahoma", 14.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.degreesTurned.Location = new System.Drawing.Point(91, 36);
            this.degreesTurned.Name = "degreesTurned";
            this.degreesTurned.Size = new System.Drawing.Size(126, 36);
            this.degreesTurned.TabIndex = 9;
            this.degreesTurned.Text = "0.0°";
            this.degreesTurned.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Dock = System.Windows.Forms.DockStyle.Fill;
            this.label6.Font = new System.Drawing.Font("Tahoma", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label6.Location = new System.Drawing.Point(3, 0);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(82, 36);
            this.label6.TabIndex = 0;
            this.label6.Text = "Degees";
            this.label6.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Dock = System.Windows.Forms.DockStyle.Fill;
            this.label7.Font = new System.Drawing.Font("Tahoma", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label7.Location = new System.Drawing.Point(3, 36);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(82, 36);
            this.label7.TabIndex = 1;
            this.label7.Text = "Degrees Turned";
            this.label7.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // label8
            // 
            this.label8.AutoSize = true;
            this.label8.Dock = System.Windows.Forms.DockStyle.Fill;
            this.label8.Font = new System.Drawing.Font("Tahoma", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label8.Location = new System.Drawing.Point(3, 72);
            this.label8.Name = "label8";
            this.label8.Size = new System.Drawing.Size(82, 36);
            this.label8.TabIndex = 2;
            this.label8.Text = "Percentage Complete";
            this.label8.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // percentageTurned
            // 
            this.percentageTurned.AutoSize = true;
            this.tableLayoutPanel3.SetColumnSpan(this.percentageTurned, 2);
            this.percentageTurned.Dock = System.Windows.Forms.DockStyle.Fill;
            this.percentageTurned.Font = new System.Drawing.Font("Tahoma", 14.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.percentageTurned.Location = new System.Drawing.Point(91, 72);
            this.percentageTurned.Name = "percentageTurned";
            this.percentageTurned.Size = new System.Drawing.Size(126, 36);
            this.percentageTurned.TabIndex = 4;
            this.percentageTurned.Text = "100 %";
            this.percentageTurned.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // button3
            // 
            this.button3.Dock = System.Windows.Forms.DockStyle.Fill;
            this.button3.Location = new System.Drawing.Point(157, 3);
            this.button3.Name = "button3";
            this.button3.Size = new System.Drawing.Size(60, 30);
            this.button3.TabIndex = 5;
            this.button3.Text = "Rotate";
            this.button3.UseVisualStyleBackColor = true;
            this.button3.Click += new System.EventHandler(this.button3_Click);
            // 
            // button4
            // 
            this.tableLayoutPanel3.SetColumnSpan(this.button4, 3);
            this.button4.Dock = System.Windows.Forms.DockStyle.Fill;
            this.button4.Font = new System.Drawing.Font("Tahoma", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.button4.Location = new System.Drawing.Point(3, 111);
            this.button4.Name = "button4";
            this.button4.Size = new System.Drawing.Size(214, 39);
            this.button4.TabIndex = 7;
            this.button4.Text = "STOP";
            this.button4.UseVisualStyleBackColor = true;
            this.button4.Click += new System.EventHandler(this.button4_Click);
            // 
            // turnAngle
            // 
            this.turnAngle.Dock = System.Windows.Forms.DockStyle.Fill;
            this.turnAngle.Location = new System.Drawing.Point(88, 7);
            this.turnAngle.Margin = new System.Windows.Forms.Padding(0, 7, 0, 0);
            this.turnAngle.MaxLength = 5;
            this.turnAngle.Name = "turnAngle";
            this.turnAngle.Size = new System.Drawing.Size(66, 22);
            this.turnAngle.TabIndex = 10;
            this.turnAngle.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
            // 
            // tabControl1
            // 
            this.tabControl1.Controls.Add(this.tabPage1);
            this.tabControl1.Controls.Add(this.tabPage2);
            this.tabControl1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tabControl1.Font = new System.Drawing.Font("Tahoma", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.tabControl1.Location = new System.Drawing.Point(235, 8);
            this.tabControl1.Name = "tabControl1";
            this.tableLayoutPanel1.SetRowSpan(this.tabControl1, 2);
            this.tabControl1.SelectedIndex = 0;
            this.tabControl1.Size = new System.Drawing.Size(768, 354);
            this.tabControl1.TabIndex = 4;
            // 
            // tabPage1
            // 
            this.tabPage1.Controls.Add(this.tableLayoutPanel4);
            this.tabPage1.Location = new System.Drawing.Point(4, 25);
            this.tabPage1.Name = "tabPage1";
            this.tabPage1.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage1.Size = new System.Drawing.Size(760, 325);
            this.tabPage1.TabIndex = 0;
            this.tabPage1.Text = "Autonomous Movement";
            this.tabPage1.UseVisualStyleBackColor = true;
            // 
            // tableLayoutPanel4
            // 
            this.tableLayoutPanel4.ColumnCount = 1;
            this.tableLayoutPanel4.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel4.Controls.Add(this.MapCanvas, 0, 1);
            this.tableLayoutPanel4.Controls.Add(this.toolStrip1, 0, 0);
            this.tableLayoutPanel4.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tableLayoutPanel4.Location = new System.Drawing.Point(3, 3);
            this.tableLayoutPanel4.Name = "tableLayoutPanel4";
            this.tableLayoutPanel4.RowCount = 2;
            this.tableLayoutPanel4.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 25F));
            this.tableLayoutPanel4.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel4.Size = new System.Drawing.Size(754, 319);
            this.tableLayoutPanel4.TabIndex = 0;
            // 
            // MapCanvas
            // 
            this.MapCanvas.Dock = System.Windows.Forms.DockStyle.Fill;
            this.MapCanvas.Location = new System.Drawing.Point(3, 28);
            this.MapCanvas.Name = "MapCanvas";
            this.MapCanvas.Size = new System.Drawing.Size(748, 288);
            this.MapCanvas.TabIndex = 4;
            this.MapCanvas.TabStop = false;
            this.MapCanvas.MouseMove += new System.Windows.Forms.MouseEventHandler(this.MapCanvas_MouseMove);
            this.MapCanvas.MouseClick += new System.Windows.Forms.MouseEventHandler(this.MapCanvas_MouseClick);
            this.MapCanvas.MouseDown += new System.Windows.Forms.MouseEventHandler(this.MapCanvas_MouseDown);
            this.MapCanvas.Paint += new System.Windows.Forms.PaintEventHandler(this.MapCanvas_Paint);
            this.MapCanvas.MouseUp += new System.Windows.Forms.MouseEventHandler(this.MapCanvas_MouseUp);
            // 
            // toolStrip1
            // 
            this.toolStrip1.GripStyle = System.Windows.Forms.ToolStripGripStyle.Hidden;
            this.toolStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.ZoomInCommand,
            this.ZoomOutCommand,
            this.toolStripSeparator1,
            this.ClearPathCommand,
            this.AddWaypointCommand,
            this.DeleteWaypointCommand,
            this.MoveWaypointCommand,
            this.toolStripSeparator2,
            this.toolStripDropDownButton1,
            this.toolStripSplitButton1,
            this.toolStripSeparator3,
            this.StopDrivingCommand,
            this.DriveCommand,
            this.ViewRobotCommand});
            this.toolStrip1.Location = new System.Drawing.Point(0, 0);
            this.toolStrip1.Name = "toolStrip1";
            this.toolStrip1.Size = new System.Drawing.Size(754, 25);
            this.toolStrip1.TabIndex = 3;
            this.toolStrip1.Text = "toolStrip1";
            // 
            // ZoomInCommand
            // 
            this.ZoomInCommand.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Text;
            this.ZoomInCommand.Image = ((System.Drawing.Image)(resources.GetObject("ZoomInCommand.Image")));
            this.ZoomInCommand.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.ZoomInCommand.Name = "ZoomInCommand";
            this.ZoomInCommand.Size = new System.Drawing.Size(50, 22);
            this.ZoomInCommand.Text = "Zoom In";
            this.ZoomInCommand.Click += new System.EventHandler(this.ZoomInCommand_Click);
            // 
            // ZoomOutCommand
            // 
            this.ZoomOutCommand.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Text;
            this.ZoomOutCommand.Image = ((System.Drawing.Image)(resources.GetObject("ZoomOutCommand.Image")));
            this.ZoomOutCommand.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.ZoomOutCommand.Name = "ZoomOutCommand";
            this.ZoomOutCommand.Size = new System.Drawing.Size(58, 22);
            this.ZoomOutCommand.Text = "Zoom Out";
            this.ZoomOutCommand.Click += new System.EventHandler(this.ZoomOutCommand_Click);
            // 
            // toolStripSeparator1
            // 
            this.toolStripSeparator1.Name = "toolStripSeparator1";
            this.toolStripSeparator1.Size = new System.Drawing.Size(6, 25);
            // 
            // ClearPathCommand
            // 
            this.ClearPathCommand.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Text;
            this.ClearPathCommand.Image = ((System.Drawing.Image)(resources.GetObject("ClearPathCommand.Image")));
            this.ClearPathCommand.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.ClearPathCommand.Name = "ClearPathCommand";
            this.ClearPathCommand.Size = new System.Drawing.Size(61, 22);
            this.ClearPathCommand.Text = "Clear Path";
            this.ClearPathCommand.Click += new System.EventHandler(this.ClearPathCommand_Click);
            // 
            // AddWaypointCommand
            // 
            this.AddWaypointCommand.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Text;
            this.AddWaypointCommand.Image = ((System.Drawing.Image)(resources.GetObject("AddWaypointCommand.Image")));
            this.AddWaypointCommand.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.AddWaypointCommand.Name = "AddWaypointCommand";
            this.AddWaypointCommand.Size = new System.Drawing.Size(79, 22);
            this.AddWaypointCommand.Text = "Add Waypoint";
            this.AddWaypointCommand.Click += new System.EventHandler(this.AddWaypointCommand_Click);
            // 
            // DeleteWaypointCommand
            // 
            this.DeleteWaypointCommand.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Text;
            this.DeleteWaypointCommand.Image = ((System.Drawing.Image)(resources.GetObject("DeleteWaypointCommand.Image")));
            this.DeleteWaypointCommand.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.DeleteWaypointCommand.Name = "DeleteWaypointCommand";
            this.DeleteWaypointCommand.Size = new System.Drawing.Size(91, 22);
            this.DeleteWaypointCommand.Text = "Delete Waypoint";
            this.DeleteWaypointCommand.Click += new System.EventHandler(this.DeleteWaypointCommand_Click);
            // 
            // MoveWaypointCommand
            // 
            this.MoveWaypointCommand.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Text;
            this.MoveWaypointCommand.Image = ((System.Drawing.Image)(resources.GetObject("MoveWaypointCommand.Image")));
            this.MoveWaypointCommand.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.MoveWaypointCommand.Name = "MoveWaypointCommand";
            this.MoveWaypointCommand.Size = new System.Drawing.Size(86, 22);
            this.MoveWaypointCommand.Text = "Move Waypoint";
            this.MoveWaypointCommand.Click += new System.EventHandler(this.MoveWaypointCommand_Click);
            // 
            // toolStripSeparator2
            // 
            this.toolStripSeparator2.Name = "toolStripSeparator2";
            this.toolStripSeparator2.Size = new System.Drawing.Size(6, 25);
            // 
            // toolStripDropDownButton1
            // 
            this.toolStripDropDownButton1.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Text;
            this.toolStripDropDownButton1.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.loadMapToolStripMenuItem,
            this.loadPathToolStripMenuItem});
            this.toolStripDropDownButton1.Image = ((System.Drawing.Image)(resources.GetObject("toolStripDropDownButton1.Image")));
            this.toolStripDropDownButton1.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolStripDropDownButton1.Name = "toolStripDropDownButton1";
            this.toolStripDropDownButton1.Size = new System.Drawing.Size(43, 22);
            this.toolStripDropDownButton1.Text = "Load";
            // 
            // loadMapToolStripMenuItem
            // 
            this.loadMapToolStripMenuItem.Name = "loadMapToolStripMenuItem";
            this.loadMapToolStripMenuItem.Size = new System.Drawing.Size(145, 22);
            this.loadMapToolStripMenuItem.Text = "Load Map...";
            this.loadMapToolStripMenuItem.Click += new System.EventHandler(this.loadMapToolStripMenuItem_Click);
            // 
            // loadPathToolStripMenuItem
            // 
            this.loadPathToolStripMenuItem.Name = "loadPathToolStripMenuItem";
            this.loadPathToolStripMenuItem.Size = new System.Drawing.Size(145, 22);
            this.loadPathToolStripMenuItem.Text = "Load Path...";
            this.loadPathToolStripMenuItem.Click += new System.EventHandler(this.loadPathToolStripMenuItem_Click);
            // 
            // toolStripSplitButton1
            // 
            this.toolStripSplitButton1.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Text;
            this.toolStripSplitButton1.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.saveMapToolStripMenuItem,
            this.savePathToolStripMenuItem});
            this.toolStripSplitButton1.Image = ((System.Drawing.Image)(resources.GetObject("toolStripSplitButton1.Image")));
            this.toolStripSplitButton1.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolStripSplitButton1.Name = "toolStripSplitButton1";
            this.toolStripSplitButton1.Size = new System.Drawing.Size(44, 22);
            this.toolStripSplitButton1.Text = "Save";
            // 
            // saveMapToolStripMenuItem
            // 
            this.saveMapToolStripMenuItem.Name = "saveMapToolStripMenuItem";
            this.saveMapToolStripMenuItem.Size = new System.Drawing.Size(146, 22);
            this.saveMapToolStripMenuItem.Text = "Save Map...";
            this.saveMapToolStripMenuItem.Click += new System.EventHandler(this.saveMapToolStripMenuItem_Click);
            // 
            // savePathToolStripMenuItem
            // 
            this.savePathToolStripMenuItem.Name = "savePathToolStripMenuItem";
            this.savePathToolStripMenuItem.Size = new System.Drawing.Size(146, 22);
            this.savePathToolStripMenuItem.Text = "Save Path...";
            this.savePathToolStripMenuItem.Click += new System.EventHandler(this.savePathToolStripMenuItem_Click);
            // 
            // toolStripSeparator3
            // 
            this.toolStripSeparator3.Name = "toolStripSeparator3";
            this.toolStripSeparator3.Size = new System.Drawing.Size(6, 25);
            // 
            // StopDrivingCommand
            // 
            this.StopDrivingCommand.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Text;
            this.StopDrivingCommand.Image = ((System.Drawing.Image)(resources.GetObject("StopDrivingCommand.Image")));
            this.StopDrivingCommand.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.StopDrivingCommand.Name = "StopDrivingCommand";
            this.StopDrivingCommand.Size = new System.Drawing.Size(33, 22);
            this.StopDrivingCommand.Text = "Stop";
            this.StopDrivingCommand.Click += new System.EventHandler(this.StopDrivingCommand_Click);
            // 
            // DriveCommand
            // 
            this.DriveCommand.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Text;
            this.DriveCommand.Image = ((System.Drawing.Image)(resources.GetObject("DriveCommand.Image")));
            this.DriveCommand.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.DriveCommand.Name = "DriveCommand";
            this.DriveCommand.Size = new System.Drawing.Size(36, 22);
            this.DriveCommand.Text = "Drive";
            this.DriveCommand.Click += new System.EventHandler(this.DriveCommand_Click);
            // 
            // ViewRobotCommand
            // 
            this.ViewRobotCommand.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Text;
            this.ViewRobotCommand.Image = ((System.Drawing.Image)(resources.GetObject("ViewRobotCommand.Image")));
            this.ViewRobotCommand.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.ViewRobotCommand.Name = "ViewRobotCommand";
            this.ViewRobotCommand.Size = new System.Drawing.Size(65, 22);
            this.ViewRobotCommand.Text = "View Robot";
            this.ViewRobotCommand.Click += new System.EventHandler(this.ViewRobotCommand_Click);
            // 
            // tabPage2
            // 
            this.tabPage2.Controls.Add(this.tableLayoutPanel5);
            this.tabPage2.Location = new System.Drawing.Point(4, 25);
            this.tabPage2.Name = "tabPage2";
            this.tabPage2.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage2.Size = new System.Drawing.Size(760, 325);
            this.tabPage2.TabIndex = 1;
            this.tabPage2.Text = "LIDAR Viewer";
            this.tabPage2.UseVisualStyleBackColor = true;
            // 
            // tableLayoutPanel5
            // 
            this.tableLayoutPanel5.ColumnCount = 1;
            this.tableLayoutPanel5.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel5.Controls.Add(this.button5, 0, 1);
            this.tableLayoutPanel5.Controls.Add(this.button6, 0, 0);
            this.tableLayoutPanel5.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tableLayoutPanel5.Location = new System.Drawing.Point(3, 3);
            this.tableLayoutPanel5.Name = "tableLayoutPanel5";
            this.tableLayoutPanel5.RowCount = 2;
            this.tableLayoutPanel5.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 25F));
            this.tableLayoutPanel5.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel5.Size = new System.Drawing.Size(754, 319);
            this.tableLayoutPanel5.TabIndex = 1;
            // 
            // updateFieldsTimer
            // 
            this.updateFieldsTimer.Enabled = true;
            this.updateFieldsTimer.Tick += new System.EventHandler(this.updateFieldsTimer_Tick);
            // 
            // statusTimer
            // 
            this.statusTimer.Enabled = true;
            this.statusTimer.Interval = 1000;
            this.statusTimer.Tick += new System.EventHandler(this.statusTimer_Tick);
            // 
            // openFileDialog1
            // 
            this.openFileDialog1.FileName = "openFileDialog1";
            // 
            // button5
            // 
            this.button5.Location = new System.Drawing.Point(3, 28);
            this.button5.Name = "button5";
            this.button5.Size = new System.Drawing.Size(109, 44);
            this.button5.TabIndex = 0;
            this.button5.Text = "Scan";
            this.button5.UseVisualStyleBackColor = true;
            this.button5.Click += new System.EventHandler(this.button5_Click);
            // 
            // button6
            // 
            this.button6.Location = new System.Drawing.Point(3, 3);
            this.button6.Name = "button6";
            this.button6.Size = new System.Drawing.Size(75, 19);
            this.button6.TabIndex = 1;
            this.button6.Text = "Settings";
            this.button6.UseVisualStyleBackColor = true;
            this.button6.Click += new System.EventHandler(this.button6_Click);
            // 
            // Control
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1006, 421);
            this.Controls.Add(this.tableLayoutPanel1);
            this.Controls.Add(this.statusStrip1);
            this.Name = "Control";
            this.Text = "Control";
            this.statusStrip1.ResumeLayout(false);
            this.statusStrip1.PerformLayout();
            this.tableLayoutPanel1.ResumeLayout(false);
            this.groupBox1.ResumeLayout(false);
            this.tableLayoutPanel2.ResumeLayout(false);
            this.tableLayoutPanel2.PerformLayout();
            this.groupBox2.ResumeLayout(false);
            this.tableLayoutPanel3.ResumeLayout(false);
            this.tableLayoutPanel3.PerformLayout();
            this.tabControl1.ResumeLayout(false);
            this.tabPage1.ResumeLayout(false);
            this.tableLayoutPanel4.ResumeLayout(false);
            this.tableLayoutPanel4.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.MapCanvas)).EndInit();
            this.toolStrip1.ResumeLayout(false);
            this.toolStrip1.PerformLayout();
            this.tabPage2.ResumeLayout(false);
            this.tableLayoutPanel5.ResumeLayout(false);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.StatusStrip statusStrip1;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel1;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel2;
        private System.Windows.Forms.TextBox driveDistance;
        private System.Windows.Forms.Label distanceMoved;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Label percentageMoved;
        private System.Windows.Forms.Button button1;
        private System.Windows.Forms.Button button2;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel3;
        private System.Windows.Forms.Label degreesTurned;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.Label label8;
        private System.Windows.Forms.Label percentageTurned;
        private System.Windows.Forms.Button button3;
        private System.Windows.Forms.Button button4;
        private System.Windows.Forms.TextBox turnAngle;
        private System.Windows.Forms.TabControl tabControl1;
        private System.Windows.Forms.TabPage tabPage1;
        private System.Windows.Forms.TabPage tabPage2;
        private System.Windows.Forms.Timer updateFieldsTimer;
        private System.Windows.Forms.Timer statusTimer;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel4;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel5;
        private System.Windows.Forms.ToolStrip toolStrip1;
        private System.Windows.Forms.ToolStripButton ZoomInCommand;
        private System.Windows.Forms.ToolStripButton ZoomOutCommand;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator1;
        private System.Windows.Forms.ToolStripButton ClearPathCommand;
        private System.Windows.Forms.ToolStripButton AddWaypointCommand;
        private System.Windows.Forms.ToolStripButton DeleteWaypointCommand;
        private System.Windows.Forms.ToolStripButton MoveWaypointCommand;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator2;
        private System.Windows.Forms.ToolStripDropDownButton toolStripDropDownButton1;
        private System.Windows.Forms.ToolStripMenuItem loadMapToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem loadPathToolStripMenuItem;
        private System.Windows.Forms.ToolStripDropDownButton toolStripSplitButton1;
        private System.Windows.Forms.ToolStripMenuItem saveMapToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem savePathToolStripMenuItem;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator3;
        private System.Windows.Forms.ToolStripButton StopDrivingCommand;
        private System.Windows.Forms.ToolStripButton DriveCommand;
        private System.Windows.Forms.ToolStripButton ViewRobotCommand;
        private System.Windows.Forms.OpenFileDialog openFileDialog1;
        private System.Windows.Forms.SaveFileDialog saveFileDialog1;
        private System.Windows.Forms.PictureBox MapCanvas;
        private System.Windows.Forms.ToolStripStatusLabel MapPositionLabel;
        private System.Windows.Forms.ToolStripStatusLabel WorldPositionLabel;
        private System.Windows.Forms.Button button5;
        private System.Windows.Forms.Button button6;

    }
}