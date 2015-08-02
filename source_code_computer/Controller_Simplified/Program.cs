using System;
using System.Collections.Generic;
using System.Windows.Forms;

namespace Controller
{
  static class Program
  {
    /// <summary>
    /// The main entry point for the application.
    /// </summary>
    [STAThread]

    static void Main()
    {

        Application.EnableVisualStyles();
        Application.SetCompatibleTextRenderingDefault(false);
        //Application.Run(new Control());
        //Application.Run(new RangeImageViewer(null));
        //Application.Run(new Navigation(null));
        //Application.Run(new Control(null));
      /**/

        
      
      ConnectionDialog Connect = new ConnectionDialog();
      DialogResult d= DialogResult.Retry;
      while (d == DialogResult.Retry)
      {
          d = Connect.ShowDialog();
          if (d == DialogResult.Cancel)
              return;
      }

      if (Connect.ConnectToRobot.Checked)
      {
        m_Robot = new Robot(Connect.GetHost(), 3000);
        Application.Run(new MainFrame(m_Robot));
        m_Robot.Disconnect();
      }
      else if (Connect.NavigationPlanning.Checked)
      {
          m_Robot = null;
      //  Application.Run(new MainFrame(m_Robot));
        Application.Run(new Control(m_Robot));
      }
      else if (Connect.ImageViewing.Checked)
      {
        Application.Run(new RangeImageViewer(null));
      }
      else if (Connect.sphereRecognition.Checked)
      {
          Application.Run(new SphereRecognitionView(m_Robot));
      }
      /**/
    }

    public static Robot m_Robot;

  }
}