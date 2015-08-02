using System;
using System.Collections.Generic;
using System.Text;

namespace Controller
{
    class DebugTimer
    {
        DateTime start = DateTime.Now, last = DateTime.Now;

        public void debug(String text)
        {    
            TimeSpan duration = DateTime.Now - start;
            Console.WriteLine("[" + Math.Round(duration.TotalMilliseconds) + "ms]" + text);
        }
        public void debugEnlapsed(String text)
        {
            TimeSpan duration = DateTime.Now - last;
            last = DateTime.Now;
            Console.WriteLine("->[" + Math.Round(duration.TotalMilliseconds) + "ms]" + text);
        }
    }
 
}
