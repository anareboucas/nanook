namespace Controller
{
  partial class Debug
  {
    /**
     * @brief Required designer variable.
     */
    private System.ComponentModel.IContainer components = null;

    /**
     * @brief Clean up any resources being used.
     * @param disposing A bool (true if managed resources should be disposed; otherwise, false)
     */
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
      this.components = new System.ComponentModel.Container();
      System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(Debug));
      this.toolStrip1 = new System.Windows.Forms.ToolStrip();
      this.ClearMessages = new System.Windows.Forms.ToolStripButton();
      this.SaveMessages = new System.Windows.Forms.ToolStripButton();
      this.Timer = new System.Windows.Forms.Timer(this.components);
      this.Messages = new System.Windows.Forms.TextBox();
      this.AutoUpdate = new System.Windows.Forms.ToolStripButton();
      this.toolStrip1.SuspendLayout();
      this.SuspendLayout();
      // 
      // toolStrip1
      // 
      this.toolStrip1.GripStyle = System.Windows.Forms.ToolStripGripStyle.Hidden;
      this.toolStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.AutoUpdate,
            this.ClearMessages,
            this.SaveMessages});
      this.toolStrip1.Location = new System.Drawing.Point(0, 0);
      this.toolStrip1.Name = "toolStrip1";
      this.toolStrip1.Size = new System.Drawing.Size(881, 25);
      this.toolStrip1.TabIndex = 1;
      this.toolStrip1.Text = "Toolbar";
      // 
      // ClearMessages
      // 
      this.ClearMessages.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Text;
      this.ClearMessages.Image = ((System.Drawing.Image)(resources.GetObject("ClearMessages.Image")));
      this.ClearMessages.ImageTransparentColor = System.Drawing.Color.Magenta;
      this.ClearMessages.Name = "ClearMessages";
      this.ClearMessages.Size = new System.Drawing.Size(38, 22);
      this.ClearMessages.Text = "Clear";
      // 
      // SaveMessages
      // 
      this.SaveMessages.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Text;
      this.SaveMessages.Image = ((System.Drawing.Image)(resources.GetObject("SaveMessages.Image")));
      this.SaveMessages.ImageTransparentColor = System.Drawing.Color.Magenta;
      this.SaveMessages.Name = "SaveMessages";
      this.SaveMessages.Size = new System.Drawing.Size(35, 22);
      this.SaveMessages.Text = "Save";
      // 
      // Timer
      // 
      this.Timer.Enabled = true;
      this.Timer.Interval = 1000;
      this.Timer.Tick += new System.EventHandler(this.Timer_Tick);
      // 
      // Messages
      // 
      this.Messages.Dock = System.Windows.Forms.DockStyle.Fill;
      this.Messages.Font = new System.Drawing.Font("Courier New", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.Messages.Location = new System.Drawing.Point(0, 25);
      this.Messages.Multiline = true;
      this.Messages.Name = "Messages";
      this.Messages.ScrollBars = System.Windows.Forms.ScrollBars.Vertical;
      this.Messages.Size = new System.Drawing.Size(881, 364);
      this.Messages.TabIndex = 2;
      // 
      // AutoUpdate
      // 
      this.AutoUpdate.Checked = true;
      this.AutoUpdate.CheckState = System.Windows.Forms.CheckState.Checked;
      this.AutoUpdate.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Text;
      this.AutoUpdate.Image = ((System.Drawing.Image)(resources.GetObject("AutoUpdate.Image")));
      this.AutoUpdate.ImageTransparentColor = System.Drawing.Color.Magenta;
      this.AutoUpdate.Name = "AutoUpdate";
      this.AutoUpdate.Size = new System.Drawing.Size(49, 22);
      this.AutoUpdate.Text = "Update";
      this.AutoUpdate.Click += new System.EventHandler(this.AutoUpdate_Click);
      // 
      // Debug
      // 
      this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
      this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
      this.ClientSize = new System.Drawing.Size(881, 389);
      this.Controls.Add(this.Messages);
      this.Controls.Add(this.toolStrip1);
      this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
      this.MaximizeBox = false;
      this.MinimizeBox = false;
      this.Name = "Debug";
      this.ShowIcon = false;
      this.ShowInTaskbar = false;
      this.Text = "Robot Debug Messages";
      this.toolStrip1.ResumeLayout(false);
      this.toolStrip1.PerformLayout();
      this.ResumeLayout(false);
      this.PerformLayout();

    }

    #endregion

    private System.Windows.Forms.ToolStrip toolStrip1;
    private System.Windows.Forms.ToolStripButton ClearMessages;
    private System.Windows.Forms.ToolStripButton SaveMessages;
    private System.Windows.Forms.Timer Timer;
    private System.Windows.Forms.TextBox Messages;
    private System.Windows.Forms.ToolStripButton AutoUpdate;
  }
}