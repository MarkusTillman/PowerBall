using System;
using System.Collections.Generic;
using System.Linq;
using System.Windows.Forms;

namespace PowerBallAI
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
            //Application.Run(new form1());

            Form1 form = new Form1();
            form.Show();
            form.GameLoop();
        }
    }
}
