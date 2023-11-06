using System.Windows.Forms;
using System.Linq;

namespace Notepad.Controls
{
    public class SideMenu : MenuStrip
    {
        private const string NAME = "SideMenu";

        public SideMenu()
        {
            Name = NAME;
            
            LayoutStyle = ToolStripLayoutStyle.VerticalStackWithOverflow;

            AutoSize = false;
            this.Width = 200;

            Dock = DockStyle.Left;

            BackColor = System.Drawing.Color.FromArgb(70, 85, 105);
        }
    }
}
