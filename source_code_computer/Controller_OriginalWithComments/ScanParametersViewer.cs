using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace Controller
{
    

    public partial class ScanParametersViewer : Form
    {
        private ScanParameters parameters;
       


        public ScanParametersViewer(ref ScanParameters rp)
        {
            InitializeComponent();
            parameters = rp;
            HRangeBox.SelectedIndex = rp.HRange;
            HResBox.SelectedIndex = rp.HRes;
            BottomBox.Text = "" + rp.scanBottom;
            ScanLineBox.Text = "" + rp.scanLines;
            VResBox.Text = "" + rp.scanRes;

        }

        private void OKButton_Click(object sender, EventArgs e)
        {
            ScanParameters t = new ScanParameters();
            try
            {
                t.HRange = (byte) HRangeBox.SelectedIndex;
                t.HRes = (byte) HResBox.SelectedIndex;
                t.scanBottom = float.Parse(BottomBox.Text);
                t.scanRes = float.Parse(VResBox.Text);
                t.scanLines = Int32.Parse(ScanLineBox.Text);

                lock (parameters)
                {
                    parameters.HRange = t.HRange;
                    parameters.HRes = t.HRes;
                    parameters.scanBottom = t.scanBottom;
                    parameters.scanLines = t.scanLines;
                    parameters.scanRes = t.scanRes;
                }
                Close();

            }
            catch
            {
                // bring up an error box
                MessageBox.Show("Invalid Scan Parameters");


            }

            
            
        }

        
        private void ScanParametersViewer_Load(object sender, EventArgs e)
        {

        }

        private void CancelButton_Click(object sender, EventArgs e)
        {
            Close();
        }




    }
    public class ScanParameters
    {
        public float scanBottom;
        public float scanRes;
        public int scanLines;
        public byte HRes;
        public byte HRange;
    }



}
