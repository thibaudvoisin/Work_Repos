using Notepad.Objects;
using System;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Reflection;
using System.Windows.Forms;
using System.Windows.Input;
using System.IO.Compression;
using System.Diagnostics;
using System.Text.RegularExpressions;
using System.Collections.Generic;


namespace Notepad.Controls
{
    public class MainMenuStrip : MenuStrip
    {
        private const string NAME = "MainMenuStrip";

        private MainForm _form;
        private OpenFileDialog _openFileDialog;
        private FolderBrowserDialog _folderBrowserDialog;

        private string _folderName;

        public MainMenuStrip()
        {
            Name = NAME;

            AutoSize = false;
            this.Height = 50;

            Dock = DockStyle.Top;

            _openFileDialog = new OpenFileDialog();
            _folderBrowserDialog = new FolderBrowserDialog();
            _folderName = null;

            DownloadButton();
            UploadButton();
            TestButton();
            OpenSubjectButton();
            RunTestButton();
            CmdButton();


            HandleCreated += (s, e) =>
            {
                _form = FindForm() as MainForm;
            };

            BackColor = System.Drawing.Color.FromArgb(187, 134, 252);
        }

        
        private async void addedItemClickEvent(object sender, EventArgs e)
        {
            foreach(TabPage tab in _form.MainTabControl.TabPages)
            { 
                if(tab.Text == (sender as ToolStripMenuItem).Text)
                { 
                    _form.MainTabControl.SelectedTab = tab;
                    return;
                }
            }
            
            var file = new TextFile(_form.patth+((sender as ToolStripMenuItem).Text));
            using (StreamReader reader = new StreamReader(file.FileName))
            {
                file.Contents = await reader.ReadToEndAsync();
            }
            var rtb = new CustomRichTextBox();



            Regex r = new Regex("\n");  
            String [] lines = r.Split(file.Contents); 
            foreach (string line in lines)  
            {  
                Regex r2 =new Regex("([ \\t{}():;])");  
                String [] tokens = r2.Split(line);   
                foreach (string token in tokens)   
                {

                    // Set the tokens default color and font.  
                    rtb.SelectionColor = Color.White;  
                    rtb.SelectionFont =new Font("Courier New", 10, FontStyle.Regular);   
                    // Check whether the token is a keyword.   
                    String [] keywords = {"public", "void", "using", "static", "class"};  
                    for (int i = 0; i < keywords.Length; i++)  
                    {  
                       // b_ = true;
                        if (keywords[i] == token)  
                        {  
                            // Apply alternative color and font to highlight keyword.  
                            rtb.SelectionColor = Color.Blue;  
                            rtb.SelectionFont =new Font("Courier New", 10,FontStyle.Bold);  
                            break;  
                        }  
                    }  
                    
                    rtb.SelectedText = token;  
                }  
                    rtb.SelectedText = "\n";  
            }


            _form.MainTabControl.TabPages.Add((sender as ToolStripMenuItem).Text);
            _form.MainTabControl.TabPages[_form.MainTabControl.TabCount -1].Controls.Add(rtb);
            _form.MainTabControl.SelectedTab = _form.MainTabControl.TabPages[_form.MainTabControl.TabCount -1 ];
        }

        public void DownloadButton()
        {
            Image dlIco = Image.FromFile(Path.Combine(Environment.CurrentDirectory, @"..\..\Icons\", "dl.ico"));

            var downLoadButton = new ToolStripMenuItem(dlIco);

            downLoadButton.AutoSize = false;

            downLoadButton.Width = 40;
            downLoadButton.Height = 40;

            var save = new ToolStripMenuItem("Enregistrer", null, null, Keys.Control | Keys.S);
            var quit = new ToolStripMenuItem("Quitter", null, null, Keys.Alt | Keys.F4);

            save.Click += async (s, e) =>
            {

                var currentFile = new TextFile(_form.patth + _form.MainTabControl.SelectedTab.Text);
                var currentRtbText = _form.CurrentRtb.Text;

                if (currentFile.Contents != currentRtbText)
                {
                    if (File.Exists(currentFile.FileName))
                    {
                        using (StreamWriter writer = File.CreateText(currentFile.FileName))
                        {
                            await writer.WriteAsync(currentRtbText);
                        }

                        currentFile.Contents = currentRtbText;
                        _form.Text = currentFile.FileName;
                        _form.MainTabControl.SelectedTab.Text = currentFile.SafeFileName;
                    }
                    else
                    {
                        MessageBox.Show("No file find.", "ERROR", MessageBoxButtons.OK, MessageBoxIcon.Error);
                    }
                }
            };

            quit.Click += (s, e) =>
            {
                Application.Exit();
            };

            downLoadButton.Click += (s, e) =>
            {
                _form.sideMenu.Items.Clear();

                string[] files = null;

                if (_openFileDialog.ShowDialog() == DialogResult.OK)
                {
                    var filePath = _openFileDialog.FileName;
                    //files = Directory.GetFiles(_folderBrowserDialog.SelectedPath);
                    FileInfo fi = new FileInfo(filePath);

                    if(fi.Extension != ".zip")
                    { 
                        MessageBox.Show("Please select a zip file");
                        return;
                    }

                    if (Directory.Exists(@".\data"))
                    {
                         if (Directory.Exists(@".\data\" + fi.Name))
                        {
                            var dir = new DirectoryInfo(@".\data\" + fi.Name);
                            dir.Delete(true);
                        }
                    }
                    else
                    { 
                    DirectoryInfo di = Directory.CreateDirectory(@".\data");
                    }
                    var folderName = fi.Name.Remove(fi.Name.Length - 4);

                    if (!Directory.Exists(@".\data\" + folderName))
                    { 
                        ZipFile.ExtractToDirectory(filePath, @".\data\" + folderName);
                    }


                    if (!Directory.Exists(@".\data\" + folderName) || !Directory.Exists(@".\data\" + folderName + "\\Files"))
                    {
                        MessageBox.Show("Invalid zip file: " + folderName);
                        return;
                    }

                    files = Directory.GetFiles(@".\data\" + folderName + "\\Files");
                    _folderName = folderName;

                }

                if (files != null)
                {
                    foreach (var currentFile in files)
                    {
                        var file = new TextFile(currentFile);

                        int index = currentFile.IndexOf(file.SafeFileName);
                        _form.patth = (index < 0 || index >= currentFile.Length) ? currentFile : currentFile.Remove(index, file.SafeFileName.Length);


                        var fileButton = new ToolStripMenuItem(file.SafeFileName);
                        fileButton.TextAlign = ContentAlignment.TopLeft;
                        fileButton.Click +=new EventHandler(addedItemClickEvent);
                        _form.sideMenu.Items.Add(fileButton);
                    }
                }
                for(var i = _form.MainTabControl.TabCount -1 ; i > 0 ; --i)
                {
                    _form.MainTabControl.TabPages.RemoveAt(i);
                }
            };

            save.Visible = false;
            quit.Visible = false;

            downLoadButton.DropDownItems.AddRange(new ToolStripItem[] { save, quit });

            Items.Add(downLoadButton);
        }

        public void UploadButton()
        {
            Image upIco = Image.FromFile(Path.Combine(Environment.CurrentDirectory, @"..\..\Icons\", "ul.ico"));

            var upLoadButton = new ToolStripMenuItem(upIco);

            upLoadButton.AutoSize = false;

            upLoadButton.Width = 40;
            upLoadButton.Height = 40;

            upLoadButton.Click += (o, e) =>
            {
                if (_folderName != null)
                {
                    // where the zip will be created
                    string folderPathToZip = null;

                    if (_folderBrowserDialog.ShowDialog() == DialogResult.OK)
                    {
                        folderPathToZip = _folderBrowserDialog.SelectedPath;
                    }
                    try
                    {
                        if (File.Exists(folderPathToZip + "\\" + _folderName + ".zip"))
                        {
                            File.Delete(folderPathToZip + "\\" + _folderName + ".zip");
                        }
                        ZipFile.CreateFromDirectory(@".\data\" + _folderName, folderPathToZip + "\\" + _folderName + ".zip");
                    }
                    catch (Exception ex)
                    {
                        MessageBox.Show(ex.Message);
                    }
                }
            };

            Items.Add(upLoadButton);
        }

        public void TestButton()
        {
            Image testIco = Image.FromFile(Path.Combine(Environment.CurrentDirectory, @"..\..\Icons\", "test.ico"));

            var testButton = new ToolStripMenuItem(testIco);

            testButton.AutoSize = false;

            testButton.Width = 40;
            testButton.Height = 40;

            testButton.Click += async (s, e) =>
            {
                if (_folderName != null)
                {
                    var file = new TextFile(@".\data\" + _folderName + "\\Tests.py");

                    using (StreamReader reader = new StreamReader(file.FileName))
                    {
                        file.Contents = await reader.ReadToEndAsync();

                        var rtb = new CustomRichTextBox();

                        rtb.ReadOnly = true;

                        rtb.Text = file.Contents;

                        _form.MainTabControl.TabPages.Add(file.SafeFileName);
                        _form.MainTabControl.TabPages[_form.MainTabControl.TabCount - 1].Controls.Add(rtb);
                        _form.MainTabControl.SelectedTab = _form.MainTabControl.TabPages[_form.MainTabControl.TabCount - 1];
                    }
                }
            };

            Items.Add(testButton);
        }

        public void OpenSubjectButton()
        {
            Image pdfIco = Image.FromFile(Path.Combine(Environment.CurrentDirectory, @"..\..\Icons\", "pdf.ico"));

            var pdfButton = new ToolStripMenuItem(pdfIco);

            pdfButton.AutoSize = false;

            pdfButton.Width = 40;
            pdfButton.Height = 40;

            pdfButton.Click += (o, e) =>
            {
                if (_folderName != null)
                {
                    ProcessStartInfo pdf = new ProcessStartInfo(@".\data\" + _folderName + "\\Subject.pdf");
                    Process.Start(pdf);
                }
            };

            Items.Add(pdfButton);
        }

        public void RunTestButton()
        {
            Image runIco = Image.FromFile(Path.Combine(Environment.CurrentDirectory, @"..\..\Icons\", "run.ico"));

            var runButton = new ToolStripMenuItem(runIco);

            runButton.AutoSize = false;

            runButton.Width = 40;
            runButton.Height = 40;

            runButton.Click += (s, e) =>
            {
                if (_folderName != null)
                {
                    var startInfo = new ProcessStartInfo
                    {
                        FileName = "cmd.exe",
                        RedirectStandardInput = true,
                        RedirectStandardOutput = true,
                        UseShellExecute = false,
                        CreateNoWindow = true
                    };

                    var process = new Process { StartInfo = startInfo };

                    process.Start();
                    process.StandardInput.WriteLine("python " + @".\data\" + _folderName + "\\Tests.py");
                    process.StandardInput.WriteLine("exit");

                    var output = "";

                    while (!process.StandardOutput.EndOfStream)
                    {
                        var line = process.StandardOutput.ReadLine();
                        output += line + "\n";
                    }

                    process.WaitForExit();
                    MessageBox.Show(output);
                }
            };

            Items.Add(runButton);
        }

        public void CmdButton()
        {
            Image cmdIco = Image.FromFile(Path.Combine(Environment.CurrentDirectory, @"..\..\Icons\", "cmd.ico"));

            var cmdButton = new ToolStripMenuItem(cmdIco);

            cmdButton.AutoSize = false;

            cmdButton.Width = 40;
            cmdButton.Height = 40;

            cmdButton.Click += (o, e) =>
            {
                System.Diagnostics.Process.Start("CMD.exe");
            };

            Items.Add(cmdButton);
        }
    }
}