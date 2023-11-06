using System;
using System.Drawing;
using System.Text.RegularExpressions;
using System.Windows.Forms;

namespace Notepad.Controls
{
    public class RichTextBoxContextMenuStrip : ContextMenuStrip
    {
        private const string NAME = "RtbContextMenuStrip";

        private RichTextBox _richtextBox;
           
        public RichTextBoxContextMenuStrip(RichTextBox richTextBox)
        {
            richTextBox.BackColor = Color.FromArgb(50,50,50);
            richTextBox.ForeColor = Color.White;
            richTextBox.Multiline = true;
            richTextBox.WordWrap = false;
            richTextBox.AcceptsTab = true;
            richTextBox.ScrollBars = RichTextBoxScrollBars.ForcedBoth;
            richTextBox.Dock = DockStyle.Fill;
            richTextBox.SelectionColor = Color.White;
            richTextBox.SelectionFont = new Font("Microsoft Sans Serif", 10, FontStyle.Regular);
            

            
            _richtextBox = richTextBox;


            var cut = new ToolStripMenuItem("Couper");
            var copy = new ToolStripMenuItem("Copier");
            var paste = new ToolStripMenuItem("Coller");
            var selectAll = new ToolStripMenuItem("Sélectionner tout");

            cut.Click += (s, e) => _richtextBox.Cut();
            copy.Click += (s, e) => _richtextBox.Copy();
            paste.Click += (s, e) => _richtextBox.Paste();
            selectAll.Click += (s, e) => _richtextBox.SelectAll();

            Items.AddRange(new ToolStripItem[] { cut, copy, paste, selectAll });
        }
    }
}
