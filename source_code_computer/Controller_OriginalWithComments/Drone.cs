using System;
using System.Collections.Generic;
using System.Text;
using System.Drawing;
using System.Net.Sockets;
using System.IO;

namespace Controller
{
    /**
     * A drone.
    */
    class Drone
    {
        public PointF absolutePosition;
        public float orientation;
        public PointF destination;
        public bool connected = false;
        public string ip;

        /**
         * @brief Constructor.
         * @param absolutePosition A PointF
         * @param orientation A float
         */
        public Drone(PointF absolutePosition, float orientation)
        {
            this.absolutePosition = absolutePosition;
            this.orientation = orientation;
        }
        /**
         * @brief Sets a destination.
         * @param destination A PointF
         */
        public void setDestination(PointF destination)
        {
            this.destination = destination;
        }

        private TcpClient tcpclnt;

        /**
         * @brief Connects to the given IP.
         * @param ip A String
         */
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

        /**
         * @brief Sends a command
         * @param text A String
         */
        public void sendCommand(String text)
        {
            if (this.connected)
            {
                Console.WriteLine("Response: [" + sendCommandInternal(text) + "]");
            }
        }

        /**
         * @brief Sends a command internal
         * @param text A String
         * @returns response A String
         */
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

        /**
         * @brief Query if this object is running.
         * @returns true if running, false if not.
         */
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
