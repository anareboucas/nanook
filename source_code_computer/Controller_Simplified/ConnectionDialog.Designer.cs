namespace Controller
{
  partial class ConnectionDialog
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
        System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(ConnectionDialog));
        this.Cancel = new System.Windows.Forms.Button();
        this.HostName = new System.Windows.Forms.TextBox();
        this.HostLabel = new System.Windows.Forms.Label();
        this.ConnectToRobot = new System.Windows.Forms.RadioButton();
        this.NavigationPlanning = new System.Windows.Forms.RadioButton();
        this.ImageViewing = new System.Windows.Forms.RadioButton();
        this.sphereRecognition = new System.Windows.Forms.RadioButton();
        this.helpProvider1 = new System.Windows.Forms.HelpProvider();
        this.OperationModeLabel = new System.Windows.Forms.Label();
        this.Connect = new System.Windows.Forms.Button();
        this.pictureBox1 = new System.Windows.Forms.PictureBox();
        ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).BeginInit();
        this.SuspendLayout();
        // 
        // Cancel
        // 
        this.Cancel.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
        this.Cancel.BackColor = System.Drawing.SystemColors.ButtonFace;
        this.Cancel.DialogResult = System.Windows.Forms.DialogResult.Cancel;
        this.Cancel.Location = new System.Drawing.Point(274, 194);
        this.Cancel.Name = "Cancel";
        this.Cancel.Size = new System.Drawing.Size(75, 23);
        this.Cancel.TabIndex = 1;
        this.Cancel.Text = "Exit";
        this.Cancel.UseVisualStyleBackColor = false;
        this.Cancel.Click += new System.EventHandler(this.Cancel_Click);
        // 
        // HostName
        // 
        this.HostName.Location = new System.Drawing.Point(275, 88);
        this.HostName.Name = "HostName";
        this.HostName.Size = new System.Drawing.Size(124, 20);
        this.HostName.TabIndex = 2;
        this.HostName.Text = "127.0.0.1";
        // 
        // HostLabel
        // 
        this.HostLabel.AutoSize = true;
        this.HostLabel.Location = new System.Drawing.Point(274, 69);
        this.HostLabel.Name = "HostLabel";
        this.HostLabel.Size = new System.Drawing.Size(64, 13);
        this.HostLabel.TabIndex = 6;
        this.HostLabel.Text = "Robot Host:";
        // 
        // ConnectToRobot
        // 
        this.ConnectToRobot.AutoSize = true;
        this.ConnectToRobot.Checked = true;
        this.ConnectToRobot.Location = new System.Drawing.Point(274, 49);
        this.ConnectToRobot.Name = "ConnectToRobot";
        this.ConnectToRobot.Size = new System.Drawing.Size(109, 17);
        this.ConnectToRobot.TabIndex = 7;
        this.ConnectToRobot.TabStop = true;
        this.ConnectToRobot.Text = "Connect to Robot";
        this.ConnectToRobot.UseVisualStyleBackColor = true;
        this.ConnectToRobot.CheckedChanged += new System.EventHandler(this.ConnectToRobot_CheckedChanged);
        // 
        // NavigationPlanning
        // 
        this.NavigationPlanning.AutoSize = true;
        this.NavigationPlanning.Location = new System.Drawing.Point(274, 111);
        this.NavigationPlanning.Name = "NavigationPlanning";
        this.NavigationPlanning.Size = new System.Drawing.Size(153, 17);
        this.NavigationPlanning.TabIndex = 8;
        this.NavigationPlanning.Text = "Offline Navigation Planning";
        this.NavigationPlanning.UseVisualStyleBackColor = true;
        // 
        // ImageViewing
        // 
        this.ImageViewing.AutoSize = true;
        this.ImageViewing.Location = new System.Drawing.Point(274, 134);
        this.ImageViewing.Name = "ImageViewing";
        this.ImageViewing.Size = new System.Drawing.Size(162, 17);
        this.ImageViewing.TabIndex = 9;
        this.ImageViewing.Text = "Offline Range Image Viewing";
        this.ImageViewing.UseVisualStyleBackColor = true;
        // 
        // sphereRecognition
        // 
        this.sphereRecognition.AutoSize = true;
        this.sphereRecognition.Location = new System.Drawing.Point(274, 157);
        this.sphereRecognition.Name = "sphereRecognition";
        this.sphereRecognition.Size = new System.Drawing.Size(152, 17);
        this.sphereRecognition.TabIndex = 10;
        this.sphereRecognition.Text = "Sphere Recognition Dialog";
        this.sphereRecognition.UseVisualStyleBackColor = true;
        // 
        // helpProvider1
        // 
        this.helpProvider1.HelpNamespace = "N:\\University\\Academic Training - NASA\\CapitolTech\\Nanook\\Working on this Backup\\" +
            "controller\\Interface_ConnectionDialog\\Controller_backup6\\bin\\Debug\\html\\index.ht" +
            "ml";
        // 
        // OperationModeLabel
        // 
        this.OperationModeLabel.AutoSize = true;
        this.OperationModeLabel.Location = new System.Drawing.Point(271, 31);
        this.OperationModeLabel.Name = "OperationModeLabel";
        this.OperationModeLabel.Size = new System.Drawing.Size(86, 13);
        this.OperationModeLabel.TabIndex = 13;
        this.OperationModeLabel.Text = "Operation Mode:";
        // 
        // Connect
        // 
        this.Connect.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
        this.Connect.BackColor = System.Drawing.SystemColors.ButtonFace;
        this.Connect.BackgroundImageLayout = System.Windows.Forms.ImageLayout.None;
        this.Connect.DialogResult = System.Windows.Forms.DialogResult.OK;
        this.Connect.Location = new System.Drawing.Point(355, 194);
        this.Connect.Name = "Connect";
        this.Connect.Size = new System.Drawing.Size(75, 23);
        this.Connect.TabIndex = 0;
        this.Connect.Text = "Load";
        this.Connect.UseVisualStyleBackColor = false;
        this.Connect.Click += new System.EventHandler(this.Connect_Click);
        // 
        // pictureBox1
        // 
        this.pictureBox1.ErrorImage = ((System.Drawing.Image)(resources.GetObject("pictureBox1.ErrorImage")));
        this.pictureBox1.Image = global::Controller.Properties.Resources.Nanook_withoutBackground;
        this.pictureBox1.InitialImage = global::Controller.Properties.Resources.Nanook_laboratory;
        this.pictureBox1.Location = new System.Drawing.Point(17, 33);
        this.pictureBox1.Name = "pictureBox1";
        this.pictureBox1.Size = new System.Drawing.Size(230, 184);
        this.pictureBox1.SizeMode = System.Windows.Forms.PictureBoxSizeMode.Zoom;
        this.pictureBox1.TabIndex = 11;
        this.pictureBox1.TabStop = false;
        // 
        // ConnectionDialog
        // 
        this.AcceptButton = this.Connect;
        this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
        this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
        this.BackColor = System.Drawing.SystemColors.InactiveCaption;
        this.CancelButton = this.Cancel;
        this.ClientSize = new System.Drawing.Size(460, 253);
        this.Controls.Add(this.OperationModeLabel);
        this.Controls.Add(this.pictureBox1);
        this.Controls.Add(this.sphereRecognition);
        this.Controls.Add(this.ImageViewing);
        this.Controls.Add(this.NavigationPlanning);
        this.Controls.Add(this.ConnectToRobot);
        this.Controls.Add(this.Connect);
        this.Controls.Add(this.Cancel);
        this.Controls.Add(this.HostName);
        this.Controls.Add(this.HostLabel);
        this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
        this.HelpButton = true;
        this.helpProvider1.SetHelpKeyword(this, "");
        this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
        this.MaximizeBox = false;
        this.MinimizeBox = false;
        this.Name = "ConnectionDialog";
        this.helpProvider1.SetShowHelp(this, true);
        this.Text = "Robot Controller Software";
        ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).EndInit();
        this.ResumeLayout(false);
        this.PerformLayout();

    }

    #endregion

      private System.Windows.Forms.Button Connect;
      private System.Windows.Forms.Button Cancel;
    private System.Windows.Forms.TextBox HostName;
    private System.Windows.Forms.Label HostLabel;
    public System.Windows.Forms.RadioButton ConnectToRobot;
    public System.Windows.Forms.RadioButton NavigationPlanning;
    public System.Windows.Forms.RadioButton ImageViewing;
    public System.Windows.Forms.RadioButton sphereRecognition;
    private System.Windows.Forms.HelpProvider helpProvider1;
    private System.Windows.Forms.PictureBox pictureBox1;
    private System.Windows.Forms.Label OperationModeLabel;
  }
}