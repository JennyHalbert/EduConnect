#include "GUI.h"
#include "LoginFrame.h"
#include <wx/wx.h>
#include "EduConnectSystem.h"

wxIMPLEMENT_APP(GUI);

bool GUI::OnInit() {
	LoginFrame* login_screen = new LoginFrame("Educonnect: Tutor Connections", &c_system);
	login_screen->SetClientSize(800, 600);
	login_screen->Center();
	login_screen->Show();
	return true;
}