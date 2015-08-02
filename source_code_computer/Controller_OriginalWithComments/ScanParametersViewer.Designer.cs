namespace Controller
{
    partial class ScanParametersViewer
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
            this.OKButton = new System.Windows.Forms.Button();
            this.BottomBox = new System.Windows.Forms.TextBox();
            this.HResBox = new System.Windows.Forms.ComboBox();
            this.HRangeBox = new System.Windows.Forms.ComboBox();
            this.VResBox = new System.Windows.Forms.TextBox();
            this.ScanLineBox = new System.Windows.Forms.TextBox();
            this.label1 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.label5 = new System.Windows.Forms.Label();
            this.label6 = new System.Windows.Forms.Label();
            this.CancelButton = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // OKButton
            // 
            this.OKButton.Location = new System.Drawing.Point(26, 130);
            this.OKButton.Name = "OKButton";
            this.OKButton.Size = new System.Drawing.Size(75, 23);
            this.OKButton.TabIndex = 0;
            this.OKButton.Text = "OK";
            this.OKButton.UseVisualStyleBackColor = true;
            this.OKButton.Click += new System.EventHandler(this.OKButton_Click);
            // 
            // BottomBox
            // 
            this.BottomBox.Location = new System.Drawing.Point(12, 65);
            this.BottomBox.Name = "BottomBox";
            this.BottomBox.Size = new System.Drawing.Size(100, 20);
            this.BottomBox.TabIndex = 1;
            // 
            // HResBox
            // 
            this.HResBox.AutoCompleteCustomSource.AddRange(new string[] {
            ".25",
            ".5",
            "1"});
            this.HResBox.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.HResBox.FormattingEnabled = true;
            this.HResBox.Items.AddRange(new object[] {
            ".25",
            ".5",
            "1"});
            this.HResBox.Location = new System.Drawing.Point(118, 25);
            this.HResBox.Name = "HResBox";
            this.HResBox.Size = new System.Drawing.Size(100, 21);
            this.HResBox.TabIndex = 2;
            // 
            // HRangeBox
            // 
            this.HRangeBox.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.HRangeBox.FormattingEnabled = true;
            this.HRangeBox.Items.AddRange(new object[] {
            "100",
            "180"});
            this.HRangeBox.Location = new System.Drawing.Point(12, 25);
            this.HRangeBox.Name = "HRangeBox";
            this.HRangeBox.Size = new System.Drawing.Size(100, 21);
            this.HRangeBox.TabIndex = 3;
            // 
            // VResBox
            // 
            this.VResBox.Location = new System.Drawing.Point(12, 104);
            this.VResBox.Name = "VResBox";
            this.VResBox.Size = new System.Drawing.Size(100, 20);
            this.VResBox.TabIndex = 5;
            // 
            // ScanLineBox
            // 
            this.ScanLineBox.Location = new System.Drawing.Point(118, 65);
            this.ScanLineBox.Name = "ScanLineBox";
            this.ScanLineBox.Size = new System.Drawing.Size(100, 20);
            this.ScanLineBox.TabIndex = 6;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(12, 9);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(89, 13);
            this.label1.TabIndex = 7;
            this.label1.Text = "Horizontal Range";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(12, 49);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(90, 13);
            this.label3.TabIndex = 9;
            this.label3.Text = "Starting Elevation";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(9, 88);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(95, 13);
            this.label4.TabIndex = 10;
            this.label4.Text = "Vertical Resolution";
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(115, 49);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(112, 13);
            this.label5.TabIndex = 11;
            this.label5.Text = "Number of Scan Lines";
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(119, 9);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(107, 13);
            this.label6.TabIndex = 12;
            this.label6.Text = "Horizontal Resolution";
            // 
            // CancelButton
            // 
            this.CancelButton.Location = new System.Drawing.Point(122, 130);
            this.CancelButton.Name = "CancelButton";
            this.CancelButton.Size = new System.Drawing.Size(75, 23);
            this.CancelButton.TabIndex = 13;
            this.CancelButton.Text = "Cancel";
            this.CancelButton.UseVisualStyleBackColor = true;
            this.CancelButton.Click += new System.EventHandler(this.CancelButton_Click);
            // 
            // ScanParametersViewer
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(225, 161);
            this.ControlBox = false;
            this.Controls.Add(this.CancelButton);
            this.Controls.Add(this.label6);
            this.Controls.Add(this.label5);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.ScanLineBox);
            this.Controls.Add(this.VResBox);
            this.Controls.Add(this.HRangeBox);
            this.Controls.Add(this.HResBox);
            this.Controls.Add(this.BottomBox);
            this.Controls.Add(this.OKButton);
            this.Name = "ScanParametersViewer";
            this.Text = "Scan Parameters";
            this.Load += new System.EventHandler(this.ScanParametersViewer_Load);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button OKButton;
        private System.Windows.Forms.TextBox BottomBox;
        private System.Windows.Forms.ComboBox HResBox;
        private System.Windows.Forms.ComboBox HRangeBox;
        private System.Windows.Forms.TextBox VResBox;
        private System.Windows.Forms.TextBox ScanLineBox;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.Button CancelButton;
    }
}