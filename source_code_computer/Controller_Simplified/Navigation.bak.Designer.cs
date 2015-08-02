namespace Controller
{
  partial class NavigationBAK
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
        System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(NavigationBAK));
        this.loadMapToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
        this.MapCanvas = new System.Windows.Forms.PictureBox();
        this.loadPathToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
        this.toolStripDropDownButton1 = new System.Windows.Forms.ToolStripDropDownButton();
        this.toolStripSplitButton1 = new System.Windows.Forms.ToolStripDropDownButton();
        this.saveMapToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
        this.savePathToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
        this.OpenFileDialog = new System.Windows.Forms.OpenFileDialog();
        this.toolStripSeparator2 = new System.Windows.Forms.ToolStripSeparator();
        this.ZoomInCommand = new System.Windows.Forms.ToolStripButton();
        this.MoveWaypointCommand = new System.Windows.Forms.ToolStripButton();
        this.ZoomOutCommand = new System.Windows.Forms.ToolStripButton();
        this.toolStrip1 = new System.Windows.Forms.ToolStrip();
        this.toolStripSeparator1 = new System.Windows.Forms.ToolStripSeparator();
        this.ClearPathCommand = new System.Windows.Forms.ToolStripButton();
        this.AddWaypointCommand = new System.Windows.Forms.ToolStripButton();
        this.DeleteWaypointCommand = new System.Windows.Forms.ToolStripButton();
        this.toolStripSeparator3 = new System.Windows.Forms.ToolStripSeparator();
        this.StopDrivingCommand = new System.Windows.Forms.ToolStripButton();
        this.DriveCommand = new System.Windows.Forms.ToolStripButton();
        this.ViewRobotCommand = new System.Windows.Forms.ToolStripButton();
        this.SaveFileDialog = new System.Windows.Forms.SaveFileDialog();
        this.statusStrip1 = new System.Windows.Forms.StatusStrip();
        this.WorldPositionLabel = new System.Windows.Forms.ToolStripStatusLabel();
        this.MapPositionLabel = new System.Windows.Forms.ToolStripStatusLabel();
        this.WaypointContextMenu = new System.Windows.Forms.ContextMenuStrip(this.components);
        this.NoScanToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
        this.FullScanToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
        ((System.ComponentModel.ISupportInitialize)(this.MapCanvas)).BeginInit();
        this.toolStrip1.SuspendLayout();
        this.statusStrip1.SuspendLayout();
        this.WaypointContextMenu.SuspendLayout();
        this.SuspendLayout();
        // 
        // loadMapToolStripMenuItem
        // 
        this.loadMapToolStripMenuItem.Name = "loadMapToolStripMenuItem";
        this.loadMapToolStripMenuItem.Size = new System.Drawing.Size(145, 22);
        this.loadMapToolStripMenuItem.Text = "Load Map...";
        this.loadMapToolStripMenuItem.Click += new System.EventHandler(this.loadMapToolStripMenuItem_Click);
        // 
        // MapCanvas
        // 
        this.MapCanvas.Dock = System.Windows.Forms.DockStyle.Fill;
        this.MapCanvas.Location = new System.Drawing.Point(0, 25);
        this.MapCanvas.Name = "MapCanvas";
        this.MapCanvas.Size = new System.Drawing.Size(1015, 654);
        this.MapCanvas.TabIndex = 3;
        this.MapCanvas.TabStop = false;
        this.MapCanvas.MouseMove += new System.Windows.Forms.MouseEventHandler(this.MapCanvas_MouseMove);
        this.MapCanvas.MouseClick += new System.Windows.Forms.MouseEventHandler(this.MapCanvas_MouseClick);
        this.MapCanvas.MouseDown += new System.Windows.Forms.MouseEventHandler(this.MapCanvas_MouseDown);
        this.MapCanvas.Paint += new System.Windows.Forms.PaintEventHandler(this.MapCanvas_Paint);
        this.MapCanvas.MouseUp += new System.Windows.Forms.MouseEventHandler(this.MapCanvas_MouseUp);
        // 
        // loadPathToolStripMenuItem
        // 
        this.loadPathToolStripMenuItem.Name = "loadPathToolStripMenuItem";
        this.loadPathToolStripMenuItem.Size = new System.Drawing.Size(145, 22);
        this.loadPathToolStripMenuItem.Text = "Load Path...";
        this.loadPathToolStripMenuItem.Click += new System.EventHandler(this.loadPathToolStripMenuItem_Click);
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
        // OpenFileDialog
        // 
        this.OpenFileDialog.FileName = "OpenFileDialog";
        // 
        // toolStripSeparator2
        // 
        this.toolStripSeparator2.Name = "toolStripSeparator2";
        this.toolStripSeparator2.Size = new System.Drawing.Size(6, 25);
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
        this.toolStrip1.Size = new System.Drawing.Size(1015, 25);
        this.toolStrip1.TabIndex = 2;
        this.toolStrip1.Text = "toolStrip1";
        this.toolStrip1.ItemClicked += new System.Windows.Forms.ToolStripItemClickedEventHandler(this.toolStrip1_ItemClicked);
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
        // statusStrip1
        // 
        this.statusStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.WorldPositionLabel,
            this.MapPositionLabel});
        this.statusStrip1.Location = new System.Drawing.Point(0, 657);
        this.statusStrip1.Name = "statusStrip1";
        this.statusStrip1.Size = new System.Drawing.Size(1015, 22);
        this.statusStrip1.TabIndex = 4;
        this.statusStrip1.Text = "statusStrip1";
        // 
        // WorldPositionLabel
        // 
        this.WorldPositionLabel.Name = "WorldPositionLabel";
        this.WorldPositionLabel.Size = new System.Drawing.Size(109, 17);
        this.WorldPositionLabel.Text = "toolStripStatusLabel1";
        // 
        // MapPositionLabel
        // 
        this.MapPositionLabel.Name = "MapPositionLabel";
        this.MapPositionLabel.Size = new System.Drawing.Size(109, 17);
        this.MapPositionLabel.Text = "toolStripStatusLabel1";
        // 
        // WaypointContextMenu
        // 
        this.WaypointContextMenu.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.NoScanToolStripMenuItem,
            this.FullScanToolStripMenuItem});
        this.WaypointContextMenu.Name = "WaypointContextMenu";
        this.WaypointContextMenu.Size = new System.Drawing.Size(128, 48);
        // 
        // NoScanToolStripMenuItem
        // 
        this.NoScanToolStripMenuItem.Name = "NoScanToolStripMenuItem";
        this.NoScanToolStripMenuItem.Size = new System.Drawing.Size(127, 22);
        this.NoScanToolStripMenuItem.Text = "No Scan";
        this.NoScanToolStripMenuItem.Click += new System.EventHandler(this.NoScanToolStripMenuItem_Click);
        // 
        // FullScanToolStripMenuItem
        // 
        this.FullScanToolStripMenuItem.Name = "FullScanToolStripMenuItem";
        this.FullScanToolStripMenuItem.Size = new System.Drawing.Size(127, 22);
        this.FullScanToolStripMenuItem.Text = "Full Scan";
        this.FullScanToolStripMenuItem.Click += new System.EventHandler(this.FullScanToolStripMenuItem_Click);
        // 
        // Navigation
        // 
        this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
        this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
        this.ClientSize = new System.Drawing.Size(1015, 679);
        this.Controls.Add(this.statusStrip1);
        this.Controls.Add(this.MapCanvas);
        this.Controls.Add(this.toolStrip1);
        this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
        this.Name = "Navigation";
        this.Text = "Navigation";
        this.Load += new System.EventHandler(this.Navigation_Load);
        ((System.ComponentModel.ISupportInitialize)(this.MapCanvas)).EndInit();
        this.toolStrip1.ResumeLayout(false);
        this.toolStrip1.PerformLayout();
        this.statusStrip1.ResumeLayout(false);
        this.statusStrip1.PerformLayout();
        this.WaypointContextMenu.ResumeLayout(false);
        this.ResumeLayout(false);
        this.PerformLayout();

    }

    #endregion

    private System.Windows.Forms.ToolStripMenuItem loadMapToolStripMenuItem;
    private System.Windows.Forms.PictureBox MapCanvas;
    private System.Windows.Forms.ToolStripMenuItem loadPathToolStripMenuItem;
    private System.Windows.Forms.ToolStripDropDownButton toolStripDropDownButton1;
    private System.Windows.Forms.ToolStripDropDownButton toolStripSplitButton1;
    private System.Windows.Forms.ToolStripMenuItem saveMapToolStripMenuItem;
    private System.Windows.Forms.ToolStripMenuItem savePathToolStripMenuItem;
    private System.Windows.Forms.OpenFileDialog OpenFileDialog;
    private System.Windows.Forms.ToolStripSeparator toolStripSeparator2;
    private System.Windows.Forms.ToolStripButton ZoomInCommand;
    private System.Windows.Forms.ToolStripButton MoveWaypointCommand;
    private System.Windows.Forms.ToolStripButton ZoomOutCommand;
    private System.Windows.Forms.ToolStrip toolStrip1;
    private System.Windows.Forms.ToolStripSeparator toolStripSeparator1;
    private System.Windows.Forms.ToolStripButton ClearPathCommand;
    private System.Windows.Forms.ToolStripButton AddWaypointCommand;
    private System.Windows.Forms.ToolStripButton DeleteWaypointCommand;
    private System.Windows.Forms.SaveFileDialog SaveFileDialog;
    private System.Windows.Forms.StatusStrip statusStrip1;
    private System.Windows.Forms.ToolStripStatusLabel MapPositionLabel;
    private System.Windows.Forms.ToolStripSeparator toolStripSeparator3;
    private System.Windows.Forms.ToolStripButton StopDrivingCommand;
    private System.Windows.Forms.ToolStripButton DriveCommand;
    private System.Windows.Forms.ToolStripStatusLabel WorldPositionLabel;
    private System.Windows.Forms.ToolStripButton ViewRobotCommand;
    private System.Windows.Forms.ContextMenuStrip WaypointContextMenu;
    private System.Windows.Forms.ToolStripMenuItem NoScanToolStripMenuItem;
    private System.Windows.Forms.ToolStripMenuItem FullScanToolStripMenuItem;

  }
}