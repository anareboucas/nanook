using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using Microsoft.Win32;
using System.Net;
using System.Net.Sockets;




namespace Controller
{
    public partial class ConnectionDialog : Form
    {
//        Control m_ControlWindow;
//        protected Robot m_Robot;

        public string GetHost()
        { return HostName.Text; }

        public ConnectionDialog()
        {
            InitializeComponent();

            RegistryKey SettingsKey = Registry.CurrentUser.CreateSubKey("Software\\Nasa\\NasaBot");
            HostName.Text = (string)SettingsKey.GetValue("Host", "127.0.0.1");
        }

        private void Connect_Click(object sender, EventArgs e)
        {
            if (ConnectToRobot.Checked)
            {

                Socket m_CommandSocket = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
                try
                {
                    IPAddress AddressToUse = null;
                    if (!IPAddress.TryParse(HostName.Text,out AddressToUse))
                    {

                        foreach (IPAddress Address in Dns.GetHostEntry(HostName.Text).AddressList)
                            if (Address.AddressFamily == AddressFamily.InterNetwork)
                                AddressToUse = Address;
                    }

                    m_CommandSocket.ReceiveTimeout = 1000;
                    m_CommandSocket.SendTimeout = 1000;


                    m_CommandSocket.Connect(new IPEndPoint(AddressToUse, 3000));

                    byte[] buf = new byte[] { 1, 0, 0, 0, 0, 0, 0, 0 };

                    m_CommandSocket.Send(buf);

                    m_CommandSocket.Receive(buf);

                    


                    m_CommandSocket.Disconnect(false);


                }
                catch
                {
                    MessageBox.Show("Cannot connect to specified host");
                    DialogResult = DialogResult.Retry;
                    return;
                }
                
                RegistryKey SettingsKey = Registry.CurrentUser.CreateSubKey("Software\\Nasa\\NasaBot");
                SettingsKey.SetValue("Host", HostName.Text);

            }//ConnectToRobot end

//            else if (NavigationPlanning.Checked)
//            {
//                m_ControlWindow = new Control(m_Robot);
//               m_ControlWindow.Show();
//            }
            Close();
        }

        private void Cancel_Click(object sender, EventArgs e)
        {
            Close();
        }

        private void ConnectToRobot_CheckedChanged(object sender, EventArgs e)
        {
            HostName.Enabled = ConnectToRobot.Checked;
        }

        private void pictureBox2_Click(object sender, EventArgs e)
        {
            if (ConnectToRobot.Checked)
            {

                Socket m_CommandSocket = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
                try
                {
                    IPAddress AddressToUse = null;
                    if (!IPAddress.TryParse(HostName.Text, out AddressToUse))
                    {

                        foreach (IPAddress Address in Dns.GetHostEntry(HostName.Text).AddressList)
                            if (Address.AddressFamily == AddressFamily.InterNetwork)
                                AddressToUse = Address;
                    }

                    m_CommandSocket.ReceiveTimeout = 1000;
                    m_CommandSocket.SendTimeout = 1000;


                    m_CommandSocket.Connect(new IPEndPoint(AddressToUse, 3000));

                    byte[] buf = new byte[] { 1, 0, 0, 0, 0, 0, 0, 0 };

                    m_CommandSocket.Send(buf);

                    m_CommandSocket.Receive(buf);




                    m_CommandSocket.Disconnect(false);


                }
                catch
                {
                    MessageBox.Show("Cannot connect to specified host");
                    DialogResult = DialogResult.Retry;
                    return;
                }

                RegistryKey SettingsKey = Registry.CurrentUser.CreateSubKey("Software\\Nasa\\NasaBot");
                SettingsKey.SetValue("Host", HostName.Text);

            }//ConnectToRobot end

            //            else if (NavigationPlanning.Checked)
            //            {
            //                m_ControlWindow = new Control(m_Robot);
            //               m_ControlWindow.Show();
            //            }
            Close();
        }

        private void pictureBox3_Click(object sender, EventArgs e)
        {
            Close();
        }





    }
}