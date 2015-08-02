using System;
using System.Collections.Generic;
using System.Text;
using System.Drawing;
using System.Net.Sockets;
using System.IO;

namespace Controller
{
    class Drone
    {
        public PointF absolutePosition;
        public float orientation;
        public PointF destination;
        public bool connected = false;
        public string ip;

        public Drone(PointF absolutePosition, float orientation)
        {
            this.absolutePosition = absolutePosition;
            this.orientation = orientation;
        }

        public void setDestination(PointF destination)
        {
            this.destination = destination;
        }

        private TcpClient tcpclnt;

        public void connect(String ip)
        {
            this.ip = ip;
            tcpclnt = new TcpClient();
            
            Console.WriteLine("Connecting...");

            try
            {
                tcpclnt.Connect(ip, 1024);
                // use the ipaddress as in the server program
            }
            catch (Exception e)
            {
                Console.WriteLine(e);
            }

            this.connected = true;
            
        }

        Stream stm;

        public void sendCommand(String text)
        {
            if (this.connected)
            {
               Console.WriteLine("Response: ["+sendCommandInternal(text)+"]");
            }
        }

        private String sendCommandInternal(String text)
        {
            Stream stm = tcpclnt.GetStream();
            ASCIIEncoding asen = new ASCIIEncoding();

            byte[] ba = asen.GetBytes(text);

            Console.Write("Sending...");
            stm.Write(ba, 0, ba.Length);

            byte[] bb = new byte[100];
            int k = stm.Read(bb, 0, 100);

            String response = "";
            for (int i = 0; i < k; i++)
                response += Convert.ToChar(bb[i]);

            return response;
        }

        public bool isRunning()
        {
            if (this.connected)
            {
                String response = sendCommandInternal("status");
                if (response == "stopped")
                {
                    Console.WriteLine();
                    return false;
                }
                else
                {
                    Console.WriteLine(response);
                }
                Console.WriteLine();
                return true;
            }
            return true;
        }
    }
}
