#include "GUI.h"
#include "LoginFrame.h"
#include <wx/wx.h>
#include "EduConnectSystem.h"

wxIMPLEMENT_APP(GUI);

// bool GUI::OnInit() {
//     std::cout << "========== EduConnect GUI starting ==========" << std::endl;
// 	LoginFrame* login_screen = new LoginFrame("Educonnect: Tutor Connections", &c_system);
// 	login_screen->SetClientSize(800, 600);
// 	login_screen->Center();
// 	login_screen->Show();
// 	return true;
// }

bool GUI::OnInit() {
    std::cout << "[GUI] Starting OnInit()" << std::endl;

    std::cout << "[GUI] Creating LoginFrame..." << std::endl;
    LoginFrame* login_screen = new LoginFrame("Educonnect: Tutor Connections", &c_system);

    std::cout << "[GUI] Setting size..." << std::endl;
    login_screen->SetClientSize(800, 600);

    std::cout << "[GUI] Centering window..." << std::endl;
    login_screen->Center();

    std::cout << "[GUI] Showing window..." << std::endl;
    login_screen->Show();

    std::cout << "[GUI] OnInit() finished successfully." << std::endl;

    return true;
}
