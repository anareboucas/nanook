using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Drawing.Imaging;
using System.Runtime.InteropServices;
using Microsoft.Win32;

namespace Controller
{
  public partial class ImageViewer : Form
  {
    public ImageViewer()
    {
      InitializeComponent();
    }

    private Controller.Image m_Image;

    public Controller.Image Image
    {
      get { return m_Image; }
      set { m_Image = value; InitImage(); Canvas.Invalidate(); }
    }

    private void InitImage()
    {
      Bitmap B = new Bitmap((int)m_Image.Width, (int)m_Image.Height);
      BitmapData Data = B.LockBits(new Rectangle(0, 0, B.Width, B.Height), ImageLockMode.WriteOnly, PixelFormat.Format24bppRgb);

      Marshal.Copy(m_Image.RawPixels, 0, Data.Scan0, m_Image.Width * m_Image.Height * 3);
      B.UnlockBits(Data);

      Canvas.Image = B;
      UpdatedOn.Text = m_Image.AquiredOn.ToString() + "(" + m_Image.Width + " x " + m_Image.Height + ")";
    }

    private void ImageViewer_Load(object sender, EventArgs e)
    {
      RegistryKey SettingsKey = Registry.CurrentUser.CreateSubKey("Software\\Nasa\\NasaBot");

      string X = (string)SettingsKey.GetValue("ImageLocationX", "");
      string Y = (string)SettingsKey.GetValue("ImageLocationY", "");

      if (!string.IsNullOrEmpty(X) && !string.IsNullOrEmpty(Y))
        Location = new Point(int.Parse(X), int.Parse(Y));

    }

    private void ImageViewer_FormClosed(object sender, FormClosedEventArgs e)
    {
      RegistryKey SettingsKey = Registry.CurrentUser.CreateSubKey("Software\\Nasa\\NasaBot");
      SettingsKey.SetValue("ImageLocationX", Location.X.ToString());
      SettingsKey.SetValue("ImageLocationY", Location.Y.ToString());
    }

  }
}
