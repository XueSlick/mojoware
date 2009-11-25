using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Diagnostics;
using System.Windows.Forms;

namespace MojoInstaller
{
    class Program
    {
        static void Main(string[] args)
        {
            try
            {
                Process.Start("mojo.exe");
            }
            catch (Exception x)
            {
                string msg = "Error launch application:\n\n" + x;
                string cap = "Error Launching Application";
                MessageBox.Show(msg, cap, MessageBoxButtons.OK, MessageBoxIcon.Error);
            }

        }
    }
}
