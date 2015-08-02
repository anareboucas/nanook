using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace Controller
{
  //The Classes and Methods below have not been used anymore.
  
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
