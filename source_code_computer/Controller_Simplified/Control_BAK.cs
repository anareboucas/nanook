using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace Controller
{
    public partial class Control : Form
    {
        protected Robot m_Robot;

        public Control(Robot R)
        {
            m_Robot = R;
            
            //Stop the updating of the fields
            updateFieldsTimer.Stop();

            InitializeComponent();
        }

        public enum Action
        {
            Move,
            Turn,
            Idle,
            External
        }

        //The running action
        public Action action;
        //To internally keep track of the state of the actions
        private bool isActionRunning = false;
        //Global distance and angle variables
        private float distance;
        private float angle;

        private float encoder0;
        private float encoder1;
        const int encPerCM = 13736;

        public void doAction(Action newAction, float value)
        {
            action = newAction;
            switch(action)
            {
                case Action.Move:
                    //Set the distance and reset the encoder values to the actual ones
                    distance = value;
                    encoder0 = m_Robot.Telemetry.TrackTicks0;
                    encoder1 = m_Robot.Telemetry.TrackTicks1;
                    //Start updating the fields
                    updateFieldsTimer.Start();
                    isActionRunning = true;

                    //Disable the value field
                    groupBox2.Enabled = false;
                    driveDistance.Enabled = false;

                    //Execute!
                    m_Robot.Drive(distance, 0);
                break;
                case Action.Turn:
                    //Set the angle and reset the encoder values to the actual ones
                    angle = value;
                    encoder0 = m_Robot.Telemetry.TrackTicks0;
                    encoder1 = m_Robot.Telemetry.TrackTicks1;
                    //Start updating the fields
                    updateFieldsTimer.Start();
                    isActionRunning = true;

                    //Disable the value field
                    groupBox1.Enabled = false;
                    turnAngle.Enabled = false;

                    //Execute!
                    m_Robot.Turn(angle);
                break;
            }
        }

        public void updateFields()
        {
            switch (action)
            {
                case Action.Move:
                    //Update with (ticks since we started moving)/(target ticks)
                    percentageMoved.Text = ((m_Robot.Telemetry.TrackTicks0-encoder0)/(distance*encPerCM)).ToString() + "%";
                break;
            }
        }

        private void updateFieldsTimer_Tick(object sender, EventArgs e)
        {
            updateFields();
        }

        public void robotStatus()
        {
            //In case somebody else is moving it
            if (m_Robot.Telemetry.MotionStatus > 8 && action.Equals(Action.Idle))
            {
                action = Action.External;
                groupBox1.Enabled = false;
                groupBox2.Enabled = false;
            }
            //Make multiple IF's to keep the code readeable
            if (m_Robot.Telemetry.MotionStatus <= 8 && !action.Equals(Action.Idle))
            {
                if (m_Robot.Telemetry.TrackSpeed0 == 0 && m_Robot.Telemetry.TrackSpeed0 == 0)
                {
                    action = Action.Idle;
                    isActionRunning = false;
                    updateFieldsTimer.Stop();
                    enableAll();
                }
            }
        }

        private void statusTimer_Tick(object sender, EventArgs e)
        {
            robotStatus();
        }

        //Enable all fields (after succesful run or stop command)
        public void enableAll()
        {
            groupBox1.Enabled = true;
            groupBox2.Enabled = true;
            
            driveDistance.Enabled = true;
            driveDistance.Refresh();

            turnAngle.Enabled = true;
            turnAngle.Refresh();

            //Restore the text
            button1.Text = "Move";
            button3.Text = "Turn";
        }

        public void stop()
        {
            m_Robot.Stop();
            enableAll();
        }

        //Region
        #region STOP BUTTONS

        private void button2_Click(object sender, EventArgs e)
        {
            stop();
        }

        private void button4_Click(object sender, EventArgs e)
        {
            stop();
        }

        #endregion

        private void button1_Click(object sender, EventArgs e)
        {
            if (isActionRunning)
            {
                stop();
            }
            else
            {
                this.Text = "Stop";
                doAction(Action.Move, float.Parse(driveDistance.Text));
            }
        }

        private void button3_Click(object sender, EventArgs e)
        {
            if (isActionRunning)
            {
                stop();
            }
            else
            {
                this.Text = "Stop";
                doAction(Action.Turn, float.Parse(turnAngle.Text));
            }
        }
    }
}
