using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace Controller
{
  public partial class Debug : Form
  {
    private Robot m_Robot;

    private void UpdateMessages()
    {
      if (!m_Robot.Connected)
        return;

      DebugTrace[] Trace = m_Robot.GetDebugMessages();

      StringBuilder B = new StringBuilder();
      foreach (DebugTrace T in Trace)
        B.AppendLine(T.Message);

      Messages.Text += B.ToString();
      Messages.SelectionStart = Messages.Text.Length-1;
      Messages.SelectionLength = 0;
      Messages.ScrollToCaret();
    }

    public Debug(Robot Robot)
    {
      m_Robot = Robot;

      InitializeComponent();
    }

    private void Timer_Tick(object sender, EventArgs e)
    {
      UpdateMessages();
    }

    private void AutoUpdate_Click(object sender, EventArgs e)
    {
      AutoUpdate.Checked = !AutoUpdate.Checked;
      Timer.Enabled = AutoUpdate.Checked;
    }
  }
}