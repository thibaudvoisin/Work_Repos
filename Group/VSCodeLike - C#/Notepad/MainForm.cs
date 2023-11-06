using Notepad.Controls;
using Notepad.Objects;
using System.IO;
using System.Linq;
using System.Windows.Forms;
using System;
using System.Drawing;
using System.Text.RegularExpressions;

namespace Notepad
{
    public partial class MainForm : Form
    {
        public RichTextBox CurrentRtb;
        public TextFile CurrentFile;
        public TabControl MainTabControl;
        public MenuStrip sideMenu;
        public Session Session;
        public string patth;

        public MainForm()
        {
            InitializeComponent();
            var menuStrip = new MainMenuStrip();
            sideMenu = new SideMenu();
            MainTabControl = new MainTabControl();

            Controls.AddRange(new Control[] { MainTabControl, sideMenu, menuStrip });

            InitializeFile();
        }

        private void InitializeFile()
        {
            Session = new Session();

            TabPage tabPage = new TabPage("Welcome");

            var rtb = new CustomRichTextBox();

            rtb.Text = @"Welcome to 3WM Studio, a simple IDE made for young EPITA students!\n" +
                "Here are the different functionalities of the buttons on the top left (from left to right):\n" +
                "- The 'Open Folder' button allows you to open a compressed folder with these files and display them on the left of the IDE. This menu allows you to open the files in tabs by clicking on them\n" +
                "- The 'Export Folder' button allows you to save the project in a location of your choice(the generated folder will be compressed)\n" +
                "- The 'Tests' button opens the test file in read - only mode\n" +
                "- The 'Pdf' button allows you to open the subject with the default pdf reader of your computer\n" +
                "- The 'Run' button allows you to launch the tests of Tests.py and to show in a pop-up message the results obtained\n" +
                "-The 'Console' button displays the console\n" +
                "You can use the shortcut Ctrl +s to quickly save your work.";
            rtb.ReadOnly = true;
            var tabCount = MainTabControl.TabCount;

            MainTabControl.TabPages.Add(tabPage);
            MainTabControl.TabPages[tabCount].Controls.Add(rtb);
        }
    }
}