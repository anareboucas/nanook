using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace Controller
{
    public partial class DroneCommandList : Form
    {
        public DroneCommandList()
        {
            InitializeComponent();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            this.Close();
        }

        private void DroneCommandList_Load(object sender, EventArgs e)
        {

        }

        public void populate(String ip, Stack<String> commands)
        {
            label2.Text = ip;
            foreach (String comm in commands)
            {
                listBox1.Items.Add(comm);
            }
        }

        public void removeElement()
        {
            listBox1.Items.RemoveAt(0);
        }
    }
}
