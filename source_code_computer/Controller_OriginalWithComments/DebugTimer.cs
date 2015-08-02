using System;
using System.Collections.Generic;
using System.Text;

namespace Controller
{
    /**
     * @brief A debug timer.
     */
    class DebugTimer
    {
        DateTime start = DateTime.Now, last = DateTime.Now;

        /**
         * @brief Debugs.
         * @param text A String
         */
        public void debug(String text)
        {
            TimeSpan duration = DateTime.Now - start;
            Console.WriteLine("[" + Math.Round(duration.TotalMilliseconds) + "ms]" + text);
        }

        /**
        * @brief Debug enlapsed.
        * @param text A String
        */
        public void debugEnlapsed(String text)
        {
            TimeSpan duration = DateTime.Now - last;
            last = DateTime.Now;
            Console.WriteLine("->[" + Math.Round(duration.TotalMilliseconds) + "ms]" + text);
        }
    }

}
