#include "GUI.h"
#include "LoginFrame.h"
#include <wx/wx.h>
#include "EduConnectSystem.h"

wxIMPLEMENT_APP(GUI);


bool GUI::OnInit() {
    std::cout << "[GUI] Starting OnInit()" << std::endl;

    std::cout << "[GUI] Creating LoginFrame..." << std::endl;
    LoginFrame* login_screen = new LoginFrame("Educonnect: Tutor Connections", &c_system);
    login_screen->SetClientSize(800, 600);
    login_screen->Center();
    login_screen->Show();

    std::cout << "[GUI] OnInit() finished successfully." << std::endl;

    return true;
}
