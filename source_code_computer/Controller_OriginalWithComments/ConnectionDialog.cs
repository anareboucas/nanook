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
    /**
     * @brief Dialog for setting the connection options.
    */
    public partial class ConnectionDialog : Form
    {
        /**
         * @brief Gets the host.
         * @returns HostName.Text - A String
         */
        public string GetHost()
        { return HostName.Text; }

        /**
         * @brief Default constructor
         */
        public ConnectionDialog()
        {
            InitializeComponent();

            RegistryKey SettingsKey = Registry.CurrentUser.CreateSubKey("Software\\Nasa\\NasaBot");
            HostName.Text = (string)SettingsKey.GetValue("Host", "127.0.0.1");
        }

        /**
        * @brief Event handler. Called by Connect for click events.
        * @param sender An object - Source of the event
        * @param e An EventArgs - Event Information
        */
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

            }

            Close();
        }

        /**
        * @brief Event handler. Called by Cancel for click events.
        * @param sender An object - Source of the event
        * @param e An EventArgs - Event Information
        */
        private void Cancel_Click(object sender, EventArgs e)
        {
            Close();
        }

        /**
        * @brief Event handler. Called by ConnectToRobot for checked changed events.
        * @param sender An object - Source of the event
        * @param e An EventArgs - Event Information
        */
        private void ConnectToRobot_CheckedChanged(object sender, EventArgs e)
        {
            HostName.Enabled = ConnectToRobot.Checked;
        }

        /**
        * @brief Event handler. Called by ConnectionDialog for load events.
        * @param sender An object - Source of the event
        * @param e An EventArgs - Event Information
        */
        private void ConnectionDialog_Load(object sender, EventArgs e)
        {

        }

        /**
        * @brief Event handler. Called by NavigationPlanning for checked changed events.
        * @param sender An object - Source of the event
        * @param e An EventArgs - Event Information
        */
        private void NavigationPlanning_CheckedChanged(object sender, EventArgs e)
        {

        }

    }
}