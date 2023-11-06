public MainForm()
{
	InitializeComponent();
	var menuStrip = new MainMenuStrip();
	sideMenu = new SideMenu();
	MainTabControl = new MainTabControl();
	Controls.AddRange(new Control[] { MainTabControl, sideMenu, menuStrip });

	InitializeFile();
}

public