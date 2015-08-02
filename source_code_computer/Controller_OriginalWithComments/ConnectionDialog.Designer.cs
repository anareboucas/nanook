namespace Controller
{
  /**
   * @brief Dialog for setting the connection.
   */

  partial class ConnectionDialog
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
        this.Connect = new System.Windows.Forms.Button();
        this.Cancel = new System.Windows.Forms.Button();
        this.HostName = new System.Windows.Forms.TextBox();
        this.label1 = new System.Windows.Forms.Label();
        this.ConnectToRobot = new System.Windows.Forms.RadioButton();
        this.NavigationPlanning = new System.Windows.Forms.RadioButton();
        this.ImageViewing = new System.Windows.Forms.RadioButton();
        this.sphereRecognition = new System.Windows.Forms.RadioButton();
        this.SuspendLayout();
        // 
        // Connect
        // 
        this.Connect.DialogResult = System.Windows.Forms.DialogResult.OK;
        this.Connect.Location = new System.Drawing.Point(211, 136);
        this.Connect.Name = "Connect";
        this.Connect.Size = new System.Drawing.Size(75, 23);
        this.Connect.TabIndex = 0;
        this.Connect.Text = "Load";
        this.Connect.UseVisualStyleBackColor = true;
        this.Connect.Click += new System.EventHandler(this.Connect_Click);
        // 
        // Cancel
        // 
        this.Cancel.DialogResult = System.Windows.Forms.DialogResult.Cancel;
        this.Cancel.Location = new System.Drawing.Point(130, 136);
        this.Cancel.Name = "Cancel";
        this.Cancel.Size = new System.Drawing.Size(75, 23);
        this.Cancel.TabIndex = 1;
        this.Cancel.Text = "Exit";
        this.Cancel.UseVisualStyleBackColor = true;
        this.Cancel.Click += new System.EventHandler(this.Cancel_Click);
        // 
        // HostName
        // 
        this.HostName.Location = new System.Drawing.Point(107, 34);
        this.HostName.Name = "HostName";
        this.HostName.Size = new System.Drawing.Size(179, 20);
        this.HostName.TabIndex = 2;
        this.HostName.Text = "127.0.0.1";
        // 
        // label1
        // 
        this.label1.AutoSize = true;
        this.label1.Location = new System.Drawing.Point(36, 37);
        this.label1.Name = "label1";
        this.label1.Size = new System.Drawing.Size(64, 13);
        this.label1.TabIndex = 6;
        this.label1.Text = "Robot Host:";
        // 
        // ConnectToRobot
        // 
        this.ConnectToRobot.AutoSize = true;
        this.ConnectToRobot.Checked = true;
        this.ConnectToRobot.Location = new System.Drawing.Point(13, 13);
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
        this.NavigationPlanning.Location = new System.Drawing.Point(13, 60);
        this.NavigationPlanning.Name = "NavigationPlanning";
        this.NavigationPlanning.Size = new System.Drawing.Size(153, 17);
        this.NavigationPlanning.TabIndex = 8;
        this.NavigationPlanning.Text = "Offline Navigation Planning";
        this.NavigationPlanning.UseVisualStyleBackColor = true;
        this.NavigationPlanning.CheckedChanged += new System.EventHandler(this.NavigationPlanning_CheckedChanged);
        // 
        // ImageViewing
        // 
        this.ImageViewing.AutoSize = true;
        this.ImageViewing.Location = new System.Drawing.Point(13, 83);
        this.ImageViewing.Name = "ImageViewing";
        this.ImageViewing.Size = new System.Drawing.Size(162, 17);
        this.ImageViewing.TabIndex = 9;
        this.ImageViewing.Text = "Offline Range Image Viewing";
        this.ImageViewing.UseVisualStyleBackColor = true;
        // 
        // sphereRecognition
        // 
        this.sphereRecognition.AutoSize = true;
        this.sphereRecognition.Location = new System.Drawing.Point(13, 106);
        this.sphereRecognition.Name = "sphereRecognition";
        this.sphereRecognition.Size = new System.Drawing.Size(152, 17);
        this.sphereRecognition.TabIndex = 10;
        this.sphereRecognition.Text = "Sphere Recognition Dialog";
        this.sphereRecognition.UseVisualStyleBackColor = true;
        // 
        // ConnectionDialog
        // 
        this.AcceptButton = this.Connect;
        this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
        this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
        this.CancelButton = this.Cancel;
        this.ClientSize = new System.Drawing.Size(298, 183);
        this.Controls.Add(this.sphereRecognition);
        this.Controls.Add(this.ImageViewing);
        this.Controls.Add(this.NavigationPlanning);
        this.Controls.Add(this.ConnectToRobot);
        this.Controls.Add(this.Connect);
        this.Controls.Add(this.Cancel);
        this.Controls.Add(this.HostName);
        this.Controls.Add(this.label1);
        this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
        this.Name = "ConnectionDialog";
        this.Text = "Robot Controller Software";
        this.Load += new System.EventHandler(this.ConnectionDialog_Load);
        this.ResumeLayout(false);
        this.PerformLayout();

    }

    #endregion

      private System.Windows.Forms.Button Connect;
      private System.Windows.Forms.Button Cancel;
    private System.Windows.Forms.TextBox HostName;
    private System.Windows.Forms.Label label1;
    public System.Windows.Forms.RadioButton ConnectToRobot;
    public System.Windows.Forms.RadioButton NavigationPlanning;
    public System.Windows.Forms.RadioButton ImageViewing;
    public System.Windows.Forms.RadioButton sphereRecognition;
  }
}