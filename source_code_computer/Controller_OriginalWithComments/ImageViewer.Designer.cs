namespace Controller
{
  partial class ImageViewer
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

    /**
     * @brief Required method for Designer support - do not modify the contents of this method with the code editor.
     */
    private void InitializeComponent()
    {
      System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(ImageViewer));
      this.toolStrip1 = new System.Windows.Forms.ToolStrip();
      this.LoadCommand = new System.Windows.Forms.ToolStripButton();
      this.SaveCommand = new System.Windows.Forms.ToolStripButton();
      this.toolStripSeparator1 = new System.Windows.Forms.ToolStripSeparator();
      this.toolStripButton3 = new System.Windows.Forms.ToolStripButton();
      this.statusStrip1 = new System.Windows.Forms.StatusStrip();
      this.UpdatedOn = new System.Windows.Forms.ToolStripStatusLabel();
      this.Canvas = new System.Windows.Forms.PictureBox();
      this.toolStrip1.SuspendLayout();
      this.statusStrip1.SuspendLayout();
      ((System.ComponentModel.ISupportInitialize)(this.Canvas)).BeginInit();
      this.SuspendLayout();
      // 
      // toolStrip1
      // 
      this.toolStrip1.GripStyle = System.Windows.Forms.ToolStripGripStyle.Hidden;
      this.toolStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.LoadCommand,
            this.SaveCommand,
            this.toolStripSeparator1,
            this.toolStripButton3});
      this.toolStrip1.Location = new System.Drawing.Point(0, 0);
      this.toolStrip1.Name = "toolStrip1";
      this.toolStrip1.Size = new System.Drawing.Size(640, 25);
      this.toolStrip1.TabIndex = 0;
      this.toolStrip1.Text = "toolStrip1";
      // 
      // LoadCommand
      // 
      this.LoadCommand.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Text;
      this.LoadCommand.Image = ((System.Drawing.Image)(resources.GetObject("LoadCommand.Image")));
      this.LoadCommand.ImageTransparentColor = System.Drawing.Color.Magenta;
      this.LoadCommand.Name = "LoadCommand";
      this.LoadCommand.Size = new System.Drawing.Size(79, 22);
      this.LoadCommand.Text = "Load Image...";
      // 
      // SaveCommand
      // 
      this.SaveCommand.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Text;
      this.SaveCommand.Image = ((System.Drawing.Image)(resources.GetObject("SaveCommand.Image")));
      this.SaveCommand.ImageTransparentColor = System.Drawing.Color.Magenta;
      this.SaveCommand.Name = "SaveCommand";
      this.SaveCommand.Size = new System.Drawing.Size(80, 22);
      this.SaveCommand.Text = "Save Image...";
      // 
      // toolStripSeparator1
      // 
      this.toolStripSeparator1.Name = "toolStripSeparator1";
      this.toolStripSeparator1.Size = new System.Drawing.Size(6, 25);
      // 
      // toolStripButton3
      // 
      this.toolStripButton3.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Text;
      this.toolStripButton3.Image = ((System.Drawing.Image)(resources.GetObject("toolStripButton3.Image")));
      this.toolStripButton3.ImageTransparentColor = System.Drawing.Color.Magenta;
      this.toolStripButton3.Name = "toolStripButton3";
      this.toolStripButton3.Size = new System.Drawing.Size(49, 22);
      this.toolStripButton3.Text = "Refresh";
      // 
      // statusStrip1
      // 
      this.statusStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.UpdatedOn});
      this.statusStrip1.Location = new System.Drawing.Point(0, 505);
      this.statusStrip1.Name = "statusStrip1";
      this.statusStrip1.Size = new System.Drawing.Size(640, 22);
      this.statusStrip1.TabIndex = 2;
      this.statusStrip1.Text = "statusStrip1";
      // 
      // UpdatedOn
      // 
      this.UpdatedOn.Name = "UpdatedOn";
      this.UpdatedOn.Size = new System.Drawing.Size(53, 17);
      this.UpdatedOn.Text = "No Image";
      // 
      // Canvas
      // 
      this.Canvas.Dock = System.Windows.Forms.DockStyle.Fill;
      this.Canvas.Location = new System.Drawing.Point(0, 25);
      this.Canvas.Name = "Canvas";
      this.Canvas.Size = new System.Drawing.Size(640, 480);
      this.Canvas.TabIndex = 3;
      this.Canvas.TabStop = false;
      // 
      // ImageViewer
      // 
      this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
      this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
      this.ClientSize = new System.Drawing.Size(640, 527);
      this.Controls.Add(this.Canvas);
      this.Controls.Add(this.statusStrip1);
      this.Controls.Add(this.toolStrip1);
      this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
      this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
      this.MaximizeBox = false;
      this.MinimizeBox = false;
      this.Name = "ImageViewer";
      this.Text = "Camera Viewer";
      this.Load += new System.EventHandler(this.ImageViewer_Load);
      this.FormClosed += new System.Windows.Forms.FormClosedEventHandler(this.ImageViewer_FormClosed);
      this.toolStrip1.ResumeLayout(false);
      this.toolStrip1.PerformLayout();
      this.statusStrip1.ResumeLayout(false);
      this.statusStrip1.PerformLayout();
      ((System.ComponentModel.ISupportInitialize)(this.Canvas)).EndInit();
      this.ResumeLayout(false);
      this.PerformLayout();

    }

    #endregion

    private System.Windows.Forms.ToolStrip toolStrip1;
    private System.Windows.Forms.ToolStripButton LoadCommand;
    private System.Windows.Forms.ToolStripButton SaveCommand;
    private System.Windows.Forms.ToolStripSeparator toolStripSeparator1;
    private System.Windows.Forms.ToolStripButton toolStripButton3;
    private System.Windows.Forms.StatusStrip statusStrip1;
    private System.Windows.Forms.ToolStripStatusLabel UpdatedOn;
    private System.Windows.Forms.PictureBox Canvas;
  }
}