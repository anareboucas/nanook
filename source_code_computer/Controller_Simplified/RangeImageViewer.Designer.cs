namespace Controller
{
  partial class RangeImageViewer
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
        System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(RangeImageViewer));
        this.toolStrip1 = new System.Windows.Forms.ToolStrip();
        this.LoadCommand = new System.Windows.Forms.ToolStripButton();
        this.toolStripButton1 = new System.Windows.Forms.ToolStripButton();
        this.toolStripSeparator1 = new System.Windows.Forms.ToolStripSeparator();
        this.AquireScan = new System.Windows.Forms.ToolStripButton();
        this.StopScanButton = new System.Windows.Forms.ToolStripButton();
        this.toolStripButton2 = new System.Windows.Forms.ToolStripButton();
        this.toolStripSeparator2 = new System.Windows.Forms.ToolStripSeparator();
        this.toolStripButton3 = new System.Windows.Forms.ToolStripButton();
        this.toolStripButton4 = new System.Windows.Forms.ToolStripButton();
        this.toolStripDropDownButton1 = new System.Windows.Forms.ToolStripDropDownButton();
        this.pointCloudToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
        this.depthMapToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
        this.coloredByDistanceToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
        this.flatImageToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
        this.OpenFileDialog = new System.Windows.Forms.OpenFileDialog();
        this.statusStrip1 = new System.Windows.Forms.StatusStrip();
        this.UpdatedOn = new System.Windows.Forms.ToolStripStatusLabel();
        this.Canvas = new System.Windows.Forms.Panel();
        this.SaveFileDialog = new System.Windows.Forms.SaveFileDialog();
        this.toolTip1 = new System.Windows.Forms.ToolTip(this.components);
        this.toolStrip1.SuspendLayout();
        this.statusStrip1.SuspendLayout();
        this.SuspendLayout();
        // 
        // toolStrip1
        // 
        this.toolStrip1.GripStyle = System.Windows.Forms.ToolStripGripStyle.Hidden;
        this.toolStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.LoadCommand,
            this.toolStripButton1,
            this.toolStripSeparator1,
            this.AquireScan,
            this.StopScanButton,
            this.toolStripButton2,
            this.toolStripSeparator2,
            this.toolStripButton3,
            this.toolStripButton4,
            this.toolStripDropDownButton1});
        this.toolStrip1.Location = new System.Drawing.Point(0, 0);
        this.toolStrip1.Name = "toolStrip1";
        this.toolStrip1.Size = new System.Drawing.Size(865, 25);
        this.toolStrip1.TabIndex = 0;
        this.toolStrip1.Text = "toolStrip1";
        // 
        // LoadCommand
        // 
        this.LoadCommand.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Text;
        this.LoadCommand.Image = ((System.Drawing.Image)(resources.GetObject("LoadCommand.Image")));
        this.LoadCommand.ImageTransparentColor = System.Drawing.Color.Magenta;
        this.LoadCommand.Name = "LoadCommand";
        this.LoadCommand.Size = new System.Drawing.Size(118, 22);
        this.LoadCommand.Text = "Load Range Image...";
        this.LoadCommand.ToolTipText = "Load Range Image file from your computer";
        this.LoadCommand.MouseUp += new System.Windows.Forms.MouseEventHandler(this.LoadCommand_MouseUp);
        this.LoadCommand.Click += new System.EventHandler(this.LoadCommand_Click);
        // 
        // toolStripButton1
        // 
        this.toolStripButton1.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Text;
        this.toolStripButton1.Image = ((System.Drawing.Image)(resources.GetObject("toolStripButton1.Image")));
        this.toolStripButton1.ImageTransparentColor = System.Drawing.Color.Magenta;
        this.toolStripButton1.Name = "toolStripButton1";
        this.toolStripButton1.Size = new System.Drawing.Size(116, 22);
        this.toolStripButton1.Text = "Save Range Image...";
        this.toolStripButton1.Click += new System.EventHandler(this.toolStripButton1_Click);
        // 
        // toolStripSeparator1
        // 
        this.toolStripSeparator1.Name = "toolStripSeparator1";
        this.toolStripSeparator1.Size = new System.Drawing.Size(6, 25);
        // 
        // AquireScan
        // 
        this.AquireScan.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Text;
        this.AquireScan.Image = ((System.Drawing.Image)(resources.GetObject("AquireScan.Image")));
        this.AquireScan.ImageTransparentColor = System.Drawing.Color.Magenta;
        this.AquireScan.Name = "AquireScan";
        this.AquireScan.Size = new System.Drawing.Size(50, 22);
        this.AquireScan.Text = "Refresh";
        this.AquireScan.ToolTipText = "Starts another LIDAR Scanning Process.";
        this.AquireScan.Click += new System.EventHandler(this.toolStripButton2_Click);
        // 
        // StopScanButton
        // 
        this.StopScanButton.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Text;
        this.StopScanButton.Image = ((System.Drawing.Image)(resources.GetObject("StopScanButton.Image")));
        this.StopScanButton.ImageTransparentColor = System.Drawing.Color.Magenta;
        this.StopScanButton.Name = "StopScanButton";
        this.StopScanButton.Size = new System.Drawing.Size(61, 22);
        this.StopScanButton.Text = "Halt Scan";
        this.StopScanButton.ToolTipText = "Stops immediately the Scanning Process.";
        this.StopScanButton.MouseUp += new System.Windows.Forms.MouseEventHandler(this.Canvas_MouseMove);
        this.StopScanButton.Click += new System.EventHandler(this.toolStripButton5_Click);
        // 
        // toolStripButton2
        // 
        this.toolStripButton2.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Text;
        this.toolStripButton2.Image = ((System.Drawing.Image)(resources.GetObject("toolStripButton2.Image")));
        this.toolStripButton2.ImageTransparentColor = System.Drawing.Color.Magenta;
        this.toolStripButton2.Name = "toolStripButton2";
        this.toolStripButton2.Size = new System.Drawing.Size(134, 22);
        this.toolStripButton2.Text = "Set Scanner Parameters";
        this.toolStripButton2.ToolTipText = "Open a new window for setting the Scanner Parameters";
        this.toolStripButton2.Click += new System.EventHandler(this.toolStripButton2_Click_1);
        // 
        // toolStripSeparator2
        // 
        this.toolStripSeparator2.Name = "toolStripSeparator2";
        this.toolStripSeparator2.Size = new System.Drawing.Size(6, 25);
        // 
        // toolStripButton3
        // 
        this.toolStripButton3.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Text;
        this.toolStripButton3.Image = ((System.Drawing.Image)(resources.GetObject("toolStripButton3.Image")));
        this.toolStripButton3.ImageTransparentColor = System.Drawing.Color.Magenta;
        this.toolStripButton3.Name = "toolStripButton3";
        this.toolStripButton3.Size = new System.Drawing.Size(66, 22);
        this.toolStripButton3.Text = "Zoom Out";
        this.toolStripButton3.ToolTipText = "Gets away from the image.";
        this.toolStripButton3.Click += new System.EventHandler(this.toolStripButton3_Click);
        // 
        // toolStripButton4
        // 
        this.toolStripButton4.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Text;
        this.toolStripButton4.Image = ((System.Drawing.Image)(resources.GetObject("toolStripButton4.Image")));
        this.toolStripButton4.ImageTransparentColor = System.Drawing.Color.Magenta;
        this.toolStripButton4.Name = "toolStripButton4";
        this.toolStripButton4.Size = new System.Drawing.Size(56, 22);
        this.toolStripButton4.Text = "Zoom In";
        this.toolStripButton4.ToolTipText = "Approximates to the image.";
        this.toolStripButton4.Click += new System.EventHandler(this.toolStripButton4_Click);
        // 
        // toolStripDropDownButton1
        // 
        this.toolStripDropDownButton1.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Text;
        this.toolStripDropDownButton1.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.pointCloudToolStripMenuItem,
            this.depthMapToolStripMenuItem,
            this.coloredByDistanceToolStripMenuItem,
            this.flatImageToolStripMenuItem});
        this.toolStripDropDownButton1.Image = ((System.Drawing.Image)(resources.GetObject("toolStripDropDownButton1.Image")));
        this.toolStripDropDownButton1.ImageTransparentColor = System.Drawing.Color.Magenta;
        this.toolStripDropDownButton1.Name = "toolStripDropDownButton1";
        this.toolStripDropDownButton1.Size = new System.Drawing.Size(114, 22);
        this.toolStripDropDownButton1.Text = "Visualisation Style";
        this.toolStripDropDownButton1.ToolTipText = "Chooses the style of visualization";
        this.toolStripDropDownButton1.Click += new System.EventHandler(this.toolStripDropDownButton1_Click);
        // 
        // pointCloudToolStripMenuItem
        // 
        this.pointCloudToolStripMenuItem.Name = "pointCloudToolStripMenuItem";
        this.pointCloudToolStripMenuItem.Size = new System.Drawing.Size(180, 22);
        this.pointCloudToolStripMenuItem.Text = "Point Cloud";
        this.pointCloudToolStripMenuItem.Click += new System.EventHandler(this.pointCloudToolStripMenuItem_Click);
        // 
        // depthMapToolStripMenuItem
        // 
        this.depthMapToolStripMenuItem.Name = "depthMapToolStripMenuItem";
        this.depthMapToolStripMenuItem.Size = new System.Drawing.Size(180, 22);
        this.depthMapToolStripMenuItem.Text = "Colored By Distance";
        this.depthMapToolStripMenuItem.Click += new System.EventHandler(this.depthMapToolStripMenuItem_Click);
        // 
        // coloredByDistanceToolStripMenuItem
        // 
        this.coloredByDistanceToolStripMenuItem.Name = "coloredByDistanceToolStripMenuItem";
        this.coloredByDistanceToolStripMenuItem.Size = new System.Drawing.Size(180, 22);
        this.coloredByDistanceToolStripMenuItem.Text = "Colored By Height";
        this.coloredByDistanceToolStripMenuItem.Click += new System.EventHandler(this.coloredByDistanceToolStripMenuItem_Click);
        // 
        // flatImageToolStripMenuItem
        // 
        this.flatImageToolStripMenuItem.Name = "flatImageToolStripMenuItem";
        this.flatImageToolStripMenuItem.Size = new System.Drawing.Size(180, 22);
        this.flatImageToolStripMenuItem.Text = "Flat Image";
        this.flatImageToolStripMenuItem.Click += new System.EventHandler(this.flatImageToolStripMenuItem_Click);
        // 
        // OpenFileDialog
        // 
        this.OpenFileDialog.DefaultExt = "RangeImage";
        this.OpenFileDialog.Filter = "Range Images|*.RangeImage";
        // 
        // statusStrip1
        // 
        this.statusStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.UpdatedOn});
        this.statusStrip1.LayoutStyle = System.Windows.Forms.ToolStripLayoutStyle.Flow;
        this.statusStrip1.Location = new System.Drawing.Point(0, 620);
        this.statusStrip1.Name = "statusStrip1";
        this.statusStrip1.Size = new System.Drawing.Size(865, 20);
        this.statusStrip1.TabIndex = 1;
        this.statusStrip1.Text = "statusStrip1";
        // 
        // UpdatedOn
        // 
        this.UpdatedOn.Name = "UpdatedOn";
        this.UpdatedOn.Overflow = System.Windows.Forms.ToolStripItemOverflow.Always;
        this.UpdatedOn.Size = new System.Drawing.Size(95, 15);
        this.UpdatedOn.Text = "No Range Image";
        // 
        // Canvas
        // 
        this.Canvas.Dock = System.Windows.Forms.DockStyle.Fill;
        this.Canvas.Location = new System.Drawing.Point(0, 25);
        this.Canvas.Name = "Canvas";
        this.Canvas.Size = new System.Drawing.Size(865, 595);
        this.Canvas.TabIndex = 2;
        this.Canvas.Paint += new System.Windows.Forms.PaintEventHandler(this.Canvas_Paint);
        this.Canvas.MouseMove += new System.Windows.Forms.MouseEventHandler(this.Canvas_MouseMove);
        this.Canvas.MouseDown += new System.Windows.Forms.MouseEventHandler(this.Canvas_MouseDown);
        // 
        // SaveFileDialog
        // 
        this.SaveFileDialog.DefaultExt = "RangeImage";
        this.SaveFileDialog.Filter = "Range Images|*.RangeImage|JPEG Files|*.jpg";
        // 
        // RangeImageViewer
        // 
        this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
        this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
        this.ClientSize = new System.Drawing.Size(865, 640);
        this.Controls.Add(this.Canvas);
        this.Controls.Add(this.statusStrip1);
        this.Controls.Add(this.toolStrip1);
        this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
        this.MaximizeBox = false;
        this.MinimizeBox = false;
        this.Name = "RangeImageViewer";
        this.Text = "LADAR Viewer";
        this.Load += new System.EventHandler(this.LADARViewer_Load);
        this.ResizeBegin += new System.EventHandler(this.RangeImageViewer_ResizeBegin);
        this.Activated += new System.EventHandler(this.RangeImageViewer_Activated);
        this.FormClosed += new System.Windows.Forms.FormClosedEventHandler(this.RangeImageViewer_FormClosed);
        this.ResizeEnd += new System.EventHandler(this.RangeImageViewer_ResizeEnd);
        this.toolStrip1.ResumeLayout(false);
        this.toolStrip1.PerformLayout();
        this.statusStrip1.ResumeLayout(false);
        this.statusStrip1.PerformLayout();
        this.ResumeLayout(false);
        this.PerformLayout();

    }

    #endregion

    private System.Windows.Forms.ToolStrip toolStrip1;
    private System.Windows.Forms.ToolStripButton LoadCommand;
    private System.Windows.Forms.OpenFileDialog OpenFileDialog;
    private System.Windows.Forms.ToolStripDropDownButton toolStripDropDownButton1;
    private System.Windows.Forms.ToolStripMenuItem pointCloudToolStripMenuItem;
    private System.Windows.Forms.ToolStripMenuItem depthMapToolStripMenuItem;
    private System.Windows.Forms.ToolStripButton toolStripButton1;
    private System.Windows.Forms.ToolStripSeparator toolStripSeparator1;
    private System.Windows.Forms.ToolStripButton AquireScan;
    private System.Windows.Forms.StatusStrip statusStrip1;
    private System.Windows.Forms.ToolStripStatusLabel UpdatedOn;
    private System.Windows.Forms.Panel Canvas;
    private System.Windows.Forms.ToolStripMenuItem coloredByDistanceToolStripMenuItem;
    private System.Windows.Forms.SaveFileDialog SaveFileDialog;
    private System.Windows.Forms.ToolStripButton toolStripButton2;
    private System.Windows.Forms.ToolStripButton toolStripButton3;
    private System.Windows.Forms.ToolStripButton toolStripButton4;
    private System.Windows.Forms.ToolStripSeparator toolStripSeparator2;
    private System.Windows.Forms.ToolStripButton StopScanButton;
    private System.Windows.Forms.ToolStripMenuItem flatImageToolStripMenuItem;
    private System.Windows.Forms.ToolTip toolTip1;
  }
}