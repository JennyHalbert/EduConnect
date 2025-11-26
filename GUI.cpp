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


/*include <wx/wx.h>
#include <wx/notebook.h>
#include <wx/listctrl.h>
#include <wx/choice.h>
#include <wx/radiobox.h>
#include <wx/listbox.h>
#include <wx/sizer.h>
#include <vector>

// ---------- Tutor struct ----------

struct Tutor {
    wxString name;
    double rating;
    wxString days;
};

// ---------- Authentication helper ----------

bool authenticateUser(const wxString &role,
                      const wxString &username,
                      const wxString &password)
{
    if (role == "student") {
        return (username == "1" && password == "1");
    } else if (role == "tutor") {
        return (username == "tutor1" && password == "password456");
    }
    return false;
}

wxString JoinDays(const wxArrayString &days)
{
    wxString result;
    for (size_t i = 0; i < days.size(); ++i) {
        if (i > 0) result += ", ";
        result += days[i];
    }
    return result;
}

// Forward declarations
class StudentFrame;
class TutorFrame;

// ---------- Tutor selection dialog (like open_tutor_selection_window) ----------

class TutorSelectionDialog : public wxDialog
{
public:
    TutorSelectionDialog(const wxString &username,
                         const wxString &priority,
                         const wxString &subject,
                         const wxArrayString &selectedDays,
                         wxWindow *parent = nullptr)
        : wxDialog(parent, wxID_ANY, "Select Tutor", wxDefaultPosition, wxSize(550, 300)),
          m_username(username),
          m_priority(priority),
          m_subject(subject),
          m_selectedDays(selectedDays)
    {
        wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);

        wxString info;
        info << "Student: " << username
             << "   |   Priority: " << priority
             << "   |   Subject: " << subject
             << "   |   Days: " << JoinDays(selectedDays);

        wxStaticText *infoText = new wxStaticText(this, wxID_ANY, info);
        infoText->Wrap(520);
        mainSizer->Add(infoText, 0, wxALL | wxEXPAND, 10);

        // Sample tutor data
        std::vector<Tutor> tutors = {
            { "Tutor 1", 4.9, "Tu" },
            { "Tutor 2", 4.3, "Th, Tu" },
            { "Tutor 3", 4.7, "M, W, F" },
        };

        m_list = new wxListCtrl(this, wxID_ANY,
                                wxDefaultPosition, wxDefaultSize,
                                wxLC_REPORT | wxLC_SINGLE_SEL);
        m_list->InsertColumn(0, "Tutor", wxLIST_FORMAT_LEFT, 180);
        m_list->InsertColumn(1, "Rating", wxLIST_FORMAT_CENTER, 80);
        m_list->InsertColumn(2, "Available Days", wxLIST_FORMAT_LEFT, 180);

        long index = 0;
        for (const auto &t : tutors) {
            long row = m_list->InsertItem(index, t.name);
            m_list->SetItem(row, 1, wxString::Format("%.1f", t.rating));
            m_list->SetItem(row, 2, t.days);
            ++index;
        }

        mainSizer->Add(m_list, 1, wxALL | wxEXPAND, 10);

        wxBoxSizer *btnSizer = new wxBoxSizer(wxHORIZONTAL);
        btnSizer->AddStretchSpacer();
        wxButton *submitBtn = new wxButton(this, wxID_OK, "Submit");
        btnSizer->Add(submitBtn, 0, wxALL, 10);
        mainSizer->Add(btnSizer, 0, wxEXPAND);

        submitBtn->Bind(wxEVT_BUTTON, &TutorSelectionDialog::OnSubmit, this);

        SetSizer(mainSizer);
    }

private:
    void OnSubmit(wxCommandEvent &)
    {
        long sel = m_list->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
        if (sel == -1) {
            wxMessageBox("Please select a tutor first.",
                         "No Tutor Selected", wxOK | wxICON_WARNING, this);
            return;
        }

        wxString tutorName   = m_list->GetItemText(sel, 0);
        wxString tutorRating = m_list->GetItemText(sel, 1);
        wxString tutorDays   = m_list->GetItemText(sel, 2);

        wxString msg;
        msg << "Request submitted!\n\n"
            << "Student: " << m_username << "\n"
            << "Tutor: " << tutorName << " (Rating: " << tutorRating << ")\n"
            << "Subject: " << m_subject << "\n"
            << "Priority: " << m_priority << "\n"
            << "Session days: " << JoinDays(m_selectedDays);

        wxMessageBox(msg, "Request Submitted", wxOK | wxICON_INFORMATION, this);
        EndModal(wxID_OK);
    }

    wxString m_username;
    wxString m_priority;
    wxString m_subject;
    wxArrayString m_selectedDays;
    wxListCtrl *m_list{};
};

// ---------- Student window ----------

class StudentFrame : public wxFrame
{
public:
    StudentFrame(const wxString &username)
        : wxFrame(nullptr, wxID_ANY,
                  "EduConnect – Student Portal (" + username + ")",
                  wxDefaultPosition, wxSize(650, 400)),
          m_username(username)
    {
        wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);
        m_notebook = new wxNotebook(this, wxID_ANY);

        BuildCreateRequestTab();
        BuildViewRequestsTab();

        mainSizer->Add(m_notebook, 1, wxALL | wxEXPAND, 10);
        SetSizer(mainSizer);
    }

private:
    void BuildCreateRequestTab()
    {
        wxPanel *panel = new wxPanel(m_notebook, wxID_ANY);
        wxGridSizer *grid = new wxGridSizer(6, 4, 5, 5);
        wxBoxSizer *outerSizer = new wxBoxSizer(wxVERTICAL);
        outerSizer->Add(grid, 1, wxALL | wxEXPAND, 20);

        // Title
        wxStaticText *title = new wxStaticText(panel, wxID_ANY, "Create Request");
        wxFont font = title->GetFont();
        font.SetPointSize(14);
        font.SetWeight(wxFONTWEIGHT_BOLD);
        title->SetFont(font);
        // Put title manually at top
        wxBoxSizer *titleSizer = new wxBoxSizer(wxHORIZONTAL);
        titleSizer->Add(title, 0, wxBOTTOM, 15);
        outerSizer->Prepend(titleSizer, 0, wxLEFT | wxRIGHT | wxTOP, 20);

        // Row 1: Priority
        grid->Add(new wxStaticText(panel, wxID_ANY, "Priority (select one):"),
                  0, wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL);
        wxString priorities[] = { "High", "Medium", "Low" };
        m_priorityBox = new wxRadioBox(panel, wxID_ANY, "",
                                       wxDefaultPosition, wxDefaultSize,
                                       WXSIZEOF(priorities), priorities, 1,
                                       wxRA_SPECIFY_COLS);
        m_priorityBox->SetSelection(1); // default Medium
        grid->Add(m_priorityBox, 0, wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL);

        // Fill row to 4 columns
        grid->AddSpacer(0);
        grid->AddSpacer(0);

        // Row 2: Subject
        grid->Add(new wxStaticText(panel, wxID_ANY, "Select subject:"),
                  0, wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL);
        m_subjectChoice = new wxChoice(panel, wxID_ANY);
        m_subjectChoice->Append("Math");
        m_subjectChoice->Append("Physics");
        m_subjectChoice->Append("Chemistry");
        m_subjectChoice->Append("Biology");
        m_subjectChoice->Append("Computer Science");
        m_subjectChoice->Append("History");
        m_subjectChoice->Append("English");
        m_subjectChoice->Append("Writing");
        m_subjectChoice->Append("Law");     
        m_subjectChoice->SetSelection(0);
        grid->Add(m_subjectChoice, 0, wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL);
        grid->AddSpacer(0);
        grid->AddSpacer(0);

        // Row 3: label for days
        grid->Add(new wxStaticText(panel, wxID_ANY,
                                   "Session Day (select all that apply):"),
                  0, wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL);
        grid->AddSpacer(0);
        grid->AddSpacer(0);
        grid->AddSpacer(0);

        // Row 4: days checkboxes (put in a horizontal box)
        wxBoxSizer *daysSizer = new wxBoxSizer(wxHORIZONTAL);
        wxString days[] = { "Monday", "Tuesday", "Wednesday",
                            "Thursday", "Friday", "Saturday", "Sunday" };
        for (auto &d : days) {
            wxCheckBox *cb = new wxCheckBox(panel, wxID_ANY, d);
            m_dayChecks.push_back(cb);
            daysSizer->Add(cb, 0, wxRIGHT, 5);
        }
        grid->Add(daysSizer, 0, wxALIGN_LEFT);
        grid->AddSpacer(0);
        grid->AddSpacer(0);
        grid->AddSpacer(0);

        // Row 5: empty
        grid->AddSpacer(0);
        grid->AddSpacer(0);
        grid->AddSpacer(0);
        grid->AddSpacer(0);

        // Row 6: Next button
        wxButton *nextBtn = new wxButton(panel, wxID_ANY, "Next");
        grid->AddSpacer(0);
        grid->AddSpacer(0);
        grid->AddSpacer(0);
        grid->Add(nextBtn, 0, wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL);

        nextBtn->Bind(wxEVT_BUTTON, &StudentFrame::OnNext, this);

        panel->SetSizer(outerSizer);
        m_notebook->AddPage(panel, "Create Request");
    }

    void BuildViewRequestsTab()
    {
        wxPanel *panel = new wxPanel(m_notebook, wxID_ANY);
        wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);

        wxString titleText = m_username + "'s Requests";
        wxStaticText *title = new wxStaticText(panel, wxID_ANY, titleText);
        wxFont font = title->GetFont();
        font.SetPointSize(14);
        font.SetWeight(wxFONTWEIGHT_BOLD);
        title->SetFont(font);
        sizer->Add(title, 0, wxALL, 10);

        m_reqTable = new wxListCtrl(panel, wxID_ANY,
                                    wxDefaultPosition, wxDefaultSize,
                                    wxLC_REPORT | wxLC_SINGLE_SEL);
        m_reqTable->InsertColumn(0, "Tutor", wxLIST_FORMAT_LEFT, 100);
        m_reqTable->InsertColumn(1, "Subject", wxLIST_FORMAT_LEFT, 100);
        m_reqTable->InsertColumn(2, "Priority", wxLIST_FORMAT_CENTER, 80);
        m_reqTable->InsertColumn(3, "Day", wxLIST_FORMAT_CENTER, 60);
        m_reqTable->InsertColumn(4, "Status", wxLIST_FORMAT_CENTER, 100);

        // sample rows
        long row = m_reqTable->InsertItem(0, "Tutor 1");
        m_reqTable->SetItem(row, 1, "Math");
        m_reqTable->SetItem(row, 2, "High");
        m_reqTable->SetItem(row, 3, "Tu");
        m_reqTable->SetItem(row, 4, "Pending");

        row = m_reqTable->InsertItem(1, "Tutor 2");
        m_reqTable->SetItem(row, 1, "CS");
        m_reqTable->SetItem(row, 2, "Medium");
        m_reqTable->SetItem(row, 3, "Th");
        m_reqTable->SetItem(row, 4, "Accepted");

        sizer->Add(m_reqTable, 1, wxALL | wxEXPAND, 10);

        wxStaticText *info = new wxStaticText(panel, wxID_ANY,
            "(Demo data only – plug in your backend later.)");
        sizer->Add(info, 0, wxLEFT | wxBOTTOM, 10);

        panel->SetSizer(sizer);
        m_notebook->AddPage(panel, "View Requests");
    }

    void OnNext(wxCommandEvent &)
    {
        wxString priority = m_priorityBox->GetStringSelection();
        wxString subject  = m_subjectChoice->GetStringSelection();

        wxArrayString chosenDays;
        for (auto *cb : m_dayChecks) {
            if (cb->GetValue())
                chosenDays.Add(cb->GetLabel());
        }

        if (chosenDays.IsEmpty()) {
            wxMessageBox("Please select at least one session day.",
                         "Missing Info", wxOK | wxICON_WARNING, this);
            return;
        }

        TutorSelectionDialog dlg(m_username, priority, subject, chosenDays, this);
        dlg.ShowModal();
    }

    wxString m_username;
    wxNotebook *m_notebook{};
    wxRadioBox *m_priorityBox{};
    wxChoice *m_subjectChoice{};
    std::vector<wxCheckBox*> m_dayChecks;
    wxListCtrl *m_reqTable{};
};

// ---------- Tutor window ----------

class TutorFrame : public wxFrame
{
public:
    TutorFrame(const wxString &username)
        : wxFrame(nullptr, wxID_ANY,
                  "EduConnect – Tutor Portal (" + username + ")",
                  wxDefaultPosition, wxSize(650, 400)),
          m_username(username)
    {
        wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);
        m_notebook = new wxNotebook(this, wxID_ANY);

        BuildManageInfoTab();
        BuildViewRequestsTab();

        mainSizer->Add(m_notebook, 1, wxALL | wxEXPAND, 10);
        SetSizer(mainSizer);
    }

private:
    void BuildManageInfoTab()
    {
        wxPanel *panel = new wxPanel(m_notebook, wxID_ANY);
        wxGridSizer *grid = new wxGridSizer(6, 4, 5, 5);
        wxBoxSizer *outerSizer = new wxBoxSizer(wxVERTICAL);
        outerSizer->Add(grid, 1, wxALL | wxEXPAND, 20);

        wxStaticText *title = new wxStaticText(panel, wxID_ANY, "Manage Tutor Info");
        wxFont font = title->GetFont();
        font.SetPointSize(14);
        font.SetWeight(wxFONTWEIGHT_BOLD);
        title->SetFont(font);

        wxBoxSizer *titleSizer = new wxBoxSizer(wxHORIZONTAL);
        titleSizer->Add(title, 0, wxBOTTOM, 10);
        outerSizer->Prepend(titleSizer, 0, wxLEFT | wxRIGHT | wxTOP, 20);

        grid->Add(new wxStaticText(panel, wxID_ANY,
                                   "Schedule (select all that apply):"),
                  0, wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL);
        grid->AddSpacer(0);
        grid->AddSpacer(0);
        grid->AddSpacer(0);

        // Days row
        wxString days[] = {"M", "T", "W", "Th", "F", "Sa", "Su"};
        wxBoxSizer *daysSizer = new wxBoxSizer(wxHORIZONTAL);
        for (auto &d : days) {
            wxCheckBox *cb = new wxCheckBox(panel, wxID_ANY, d);
            m_dayChecks.push_back(cb);
            daysSizer->Add(cb, 0, wxRIGHT, 5);
        }
        grid->Add(daysSizer, 0, wxALIGN_LEFT);
        grid->AddSpacer(0);
        grid->AddSpacer(0);
        grid->AddSpacer(0);

        grid->Add(new wxStaticText(panel, wxID_ANY,
                                   "Select topics you can tutor:"),
                  0, wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL);
        grid->AddSpacer(0);
        grid->AddSpacer(0);
        grid->AddSpacer(0);

        m_topicsList = new wxListBox(panel, wxID_ANY);
        m_topicsList->Append("Math");
        m_topicsList->Append("Physics");
        m_topicsList->Append("Computer Science");
        m_topicsList->Append("Chemistry");
        m_topicsList->Append("Biology");
        m_topicsList->SetSelection(wxNOT_FOUND); // none selected by default
        m_topicsList->SetMinSize(wxSize(200, 100));
        grid->Add(m_topicsList, 0, wxEXPAND);
        grid->AddSpacer(0);
        grid->AddSpacer(0);
        grid->AddSpacer(0);

        wxButton *saveBtn = new wxButton(panel, wxID_ANY, "Save");
        grid->AddSpacer(0);
        grid->AddSpacer(0);
        grid->AddSpacer(0);
        grid->Add(saveBtn, 0, wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL);

        saveBtn->Bind(wxEVT_BUTTON, &TutorFrame::OnSaveInfo, this);

        panel->SetSizer(outerSizer);
        m_notebook->AddPage(panel, "Manage Info");
    }

    void BuildViewRequestsTab()
    {
        wxPanel *panel = new wxPanel(m_notebook, wxID_ANY);
        wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);

        wxString titleText = m_username + "'s Incoming Requests";
        wxStaticText *title = new wxStaticText(panel, wxID_ANY, titleText);
        wxFont font = title->GetFont();
        font.SetPointSize(14);
        font.SetWeight(wxFONTWEIGHT_BOLD);
        title->SetFont(font);
        sizer->Add(title, 0, wxALL, 10);

        m_reqTable = new wxListCtrl(panel, wxID_ANY,
                                    wxDefaultPosition, wxDefaultSize,
                                    wxLC_REPORT | wxLC_SINGLE_SEL);
        m_reqTable->InsertColumn(0, "Student", wxLIST_FORMAT_LEFT, 100);
        m_reqTable->InsertColumn(1, "Subject", wxLIST_FORMAT_LEFT, 100);
        m_reqTable->InsertColumn(2, "Priority", wxLIST_FORMAT_CENTER, 80);
        m_reqTable->InsertColumn(3, "Day", wxLIST_FORMAT_CENTER, 60);
        m_reqTable->InsertColumn(4, "Status", wxLIST_FORMAT_CENTER, 100);

        // Sample data
        long row = m_reqTable->InsertItem(0, "student1");
        m_reqTable->SetItem(row, 1, "Math");
        m_reqTable->SetItem(row, 2, "High");
        m_reqTable->SetItem(row, 3, "Tu");
        m_reqTable->SetItem(row, 4, "Pending");

        row = m_reqTable->InsertItem(1, "student2");
        m_reqTable->SetItem(row, 1, "CS");
        m_reqTable->SetItem(row, 2, "Low");
        m_reqTable->SetItem(row, 3, "F");
        m_reqTable->SetItem(row, 4, "Pending");

        sizer->Add(m_reqTable, 1, wxALL | wxEXPAND, 10);

        wxStaticText *info = new wxStaticText(panel, wxID_ANY,
            "(Demo data only – later you can accept/decline here.)");
        sizer->Add(info, 0, wxLEFT | wxBOTTOM, 10);

        panel->SetSizer(sizer);
        m_notebook->AddPage(panel, "View Requests");
    }

    void OnSaveInfo(wxCommandEvent &)
    {
        wxArrayString selectedDays;
        for (auto *cb : m_dayChecks) {
            if (cb->GetValue())
                selectedDays.Add(cb->GetLabel());
        }

        wxArrayInt selections;
        m_topicsList->GetSelections(selections);
        wxArrayString selectedTopics;
        for (int i = 0; i < (int)selections.GetCount(); ++i) {
            selectedTopics.Add(m_topicsList->GetString(selections[i]));
        }

        wxString msg;
        msg << "Availability updated for " << m_username << ".\n\n"
            << "Days: " << (selectedDays.IsEmpty() ? "None" : JoinDays(selectedDays)) << "\n"
            << "Topics: " << (selectedTopics.IsEmpty() ? "None" : JoinDays(selectedTopics));

        wxMessageBox(msg, "Info Saved", wxOK | wxICON_INFORMATION, this);
    }

    wxString m_username;
    wxNotebook *m_notebook{};
    std::vector<wxCheckBox*> m_dayChecks;
    wxListBox *m_topicsList{};
    wxListCtrl *m_reqTable{};
};

// ---------- Login window (like your main + login tabs) ----------

class LoginFrame : public wxFrame
{
public:
    LoginFrame()
        : wxFrame(nullptr, wxID_ANY, "EduConnect Login",
                  wxDefaultPosition, wxSize(450, 320))
    {
        wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);
        m_notebook = new wxNotebook(this, wxID_ANY);

        BuildLoginTab("student");
        BuildLoginTab("tutor");

        mainSizer->Add(m_notebook, 1, wxALL | wxEXPAND, 20);
        SetSizer(mainSizer);
    }

private:
    void BuildLoginTab(const wxString &role)
    {
        wxPanel *panel = new wxPanel(m_notebook, wxID_ANY);
        wxGridSizer *grid = new wxGridSizer(5, 2, 5, 5);
        wxBoxSizer *outerSizer = new wxBoxSizer(wxVERTICAL);
        outerSizer->Add(grid, 1, wxALL | wxEXPAND, 20);

        wxString roleCap = role;
        roleCap[0] = wxToupper(roleCap[0]);

        wxStaticText *title = new wxStaticText(panel, wxID_ANY,
                                               roleCap + " Portal");
        wxFont font = title->GetFont();
        font.SetPointSize(16);
        font.SetWeight(wxFONTWEIGHT_BOLD);
        title->SetFont(font);

        wxBoxSizer *titleSizer = new wxBoxSizer(wxHORIZONTAL);
        titleSizer->Add(title, 0, wxBOTTOM, 20);
        outerSizer->Prepend(titleSizer, 0, wxLEFT | wxRIGHT | wxTOP, 20);

        grid->Add(new wxStaticText(panel, wxID_ANY, "Username:"),
                  0, wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL);
        wxTextCtrl *usernameEdit = new wxTextCtrl(panel, wxID_ANY);
        grid->Add(usernameEdit, 0, wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL);

        grid->Add(new wxStaticText(panel, wxID_ANY, "Password:"),
                  0, wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL);
        wxTextCtrl *passwordEdit = new wxTextCtrl(panel, wxID_ANY, "",
                                                  wxDefaultPosition, wxDefaultSize,
                                                  wxTE_PASSWORD);
        grid->Add(passwordEdit, 0, wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL);

        wxButton *submitBtn = new wxButton(panel, wxID_ANY, "Submit");
        grid->Add(submitBtn, 0, wxALIGN_CENTER);
        wxButton *forgotBtn = new wxButton(panel, wxID_ANY, "Forgot password?");
        grid->Add(forgotBtn, 0, wxALIGN_CENTER);

        if (role == "student") {
            m_studentUser = usernameEdit;
            m_studentPass = passwordEdit;
            submitBtn->Bind(wxEVT_BUTTON, &LoginFrame::OnStudentLogin, this);
        } else {
            m_tutorUser = usernameEdit;
            m_tutorPass = passwordEdit;
            submitBtn->Bind(wxEVT_BUTTON, &LoginFrame::OnTutorLogin, this);
        }

        forgotBtn->Bind(wxEVT_BUTTON, &LoginFrame::OnForgotPassword, this);

        panel->SetSizer(outerSizer);
        m_notebook->AddPage(panel, roleCap + " Login");
    }

    void OnForgotPassword(wxCommandEvent &)
    {
        wxMessageBox("Contact help@educonnect.com to reset your password.",
                     "Unknown Password", wxOK | wxICON_INFORMATION, this);
    }

    void OnStudentLogin(wxCommandEvent &)
    {
        wxString username = m_studentUser->GetValue().Trim(true).Trim(false);
        wxString password = m_studentPass->GetValue();

        if (username.IsEmpty() || password.IsEmpty()) {
            wxMessageBox("Please enter both username and password.",
                         "Missing Info", wxOK | wxICON_WARNING, this);
            return;
        }

        if (authenticateUser("student", username, password)) {
            m_studentUser->Clear();
            m_studentPass->Clear();
            wxLogMessage("Login successful for %s as student", username);

            StudentFrame *frame = new StudentFrame(username);
            frame->Show();
        } else {
            wxMessageBox("Invalid username or password.",
                         "Login Failed", wxOK | wxICON_ERROR, this);
            m_studentPass->Clear();
        }
    }

    void OnTutorLogin(wxCommandEvent &)
    {
        wxString username = m_tutorUser->GetValue().Trim(true).Trim(false);
        wxString password = m_tutorPass->GetValue();

        if (username.IsEmpty() || password.IsEmpty()) {
            wxMessageBox("Please enter both username and password.",
                         "Missing Info", wxOK | wxICON_WARNING, this);
            return;
        }

        if (authenticateUser("tutor", username, password)) {
            m_tutorUser->Clear();
            m_tutorPass->Clear();
            wxLogMessage("Login successful for %s as tutor", username);

            TutorFrame *frame = new TutorFrame(username);
            frame->Show();
        } else {
            wxMessageBox("Invalid username or password.",
                         "Login Failed", wxOK | wxICON_ERROR, this);
            m_tutorPass->Clear();
        }
    }

    wxNotebook *m_notebook{};
    wxTextCtrl *m_studentUser{};
    wxTextCtrl *m_studentPass{};
    wxTextCtrl *m_tutorUser{};
    wxTextCtrl *m_tutorPass{};
};

// ---------- wxApp ----------

class MyApp : public wxApp
{
public:
    virtual bool OnInit() override
    {
        std::cout << "========== duConnect GUI starting ==========" << std::endl;
        LoginFrame *frame = new LoginFrame();
        frame->Show();
        return true;
    }
};

wxIMPLEMENT_APP(MyApp);
*/