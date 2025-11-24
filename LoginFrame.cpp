#include "LoginFrame.h"
#include "EduConnectSystem.h"
#include <wx/wx.h>



LoginFrame::LoginFrame(const wxString& title,EduConnectSystem* c_system): wxFrame(nullptr, wxID_ANY, title) {

	this-> connection_system = c_system;
	if(this->connection_system==nullptr){
		wxMessageBox("Error: System pointer is null!", "Crash Warning", wxOK | wxICON_ERROR);
	}
	
	login_switch = new wxSimplebook(this);
	//Student Login Page-------------------------------------------------------------------------
	wxPanel* student_login_panel = new wxPanel(login_switch);
	//,wxID_ANY, wxPoint(0, 0), wxSize(3440, 1440), wxALIGN_CENTER_HORIZONTAL, "Student Portal");
	//student_panel->SetBackgroundColour(wxColour(250, 250, 250));

	//menu
	wxStaticText* student_menu = new wxStaticText(student_login_panel, wxID_ANY, "Student Portal", wxPoint(0, 150), wxSize(800, -1), wxALIGN_CENTER_HORIZONTAL);
		wxFont f1 = student_menu->GetFont();
		f1.MakeBold().Scale(4.0);
		student_menu->SetFont(f1);
		student_menu->SetBackgroundColour(wxColour(250, 250, 250));

	//Buttons
	wxButton* enter_btn_student = new wxButton(student_login_panel, wxID_ANY, "Submit", wxPoint(300, 350), wxSize(100, 35));
	wxButton* forgor_btn_student = new wxButton(student_login_panel, wxID_ANY, "Forgot Password?", wxPoint(400, 350), wxSize(100, 35));
	wxButton* to_student_register_btn1 = new wxButton(student_login_panel, wxID_ANY, "Register as Student", wxPoint(300, 400), wxSize(200, 35));
	wxButton* to_tutor_btn = new wxButton(student_login_panel, wxID_ANY, "Tutor Portal", wxPoint(300, 450), wxSize(200, 35));

	//Text Fields
	wxStaticText* username_text_student = new wxStaticText(student_login_panel, wxID_ANY, "Username/Email:", wxPoint(270, 250), wxSize(125, -1), wxALIGN_CENTER_HORIZONTAL);
		wxFont f2 = username_text_student->GetFont();
		f2.Scale(1.25);
		username_text_student->SetFont(f2);
		username_text_student->SetBackgroundColour(wxColour(250, 250, 250));
	wxStaticText* password_text_student = new wxStaticText(student_login_panel, wxID_ANY, "Password:", wxPoint(270, 300), wxSize(125, -1), wxALIGN_CENTER_HORIZONTAL);
		password_text_student->SetFont(f2);
		password_text_student->SetBackgroundColour(wxColour(250, 250, 250));

	//Typing Fields
	student_username = new wxTextCtrl(student_login_panel, wxID_ANY, "", wxPoint(405, 250), wxSize(125, -1));
	student_password = new wxTextCtrl(student_login_panel, wxID_ANY, "", wxPoint(405, 300), wxSize(125, -1), wxTE_PASSWORD);

	//button functions
	enter_btn_student->Bind(wxEVT_BUTTON, &LoginFrame::enter_student, this);
	forgor_btn_student->Bind(wxEVT_BUTTON, &LoginFrame::forgor_button, this);
	to_tutor_btn->Bind(wxEVT_BUTTON, &LoginFrame::to_tutor, this);
	to_student_register_btn1->Bind(wxEVT_BUTTON, &LoginFrame::to_student_register, this);


	//Tutor Login Page----------------------------------------------------------------------------

	wxPanel* tutor_login_panel = new wxPanel(login_switch);
		//menu
	wxStaticText* tutor_menu = new wxStaticText(tutor_login_panel, wxID_ANY, "Tutor Portal", wxPoint(0, 150), wxSize(800, -1), wxALIGN_CENTER_HORIZONTAL);
	tutor_menu->SetFont(f1);
	tutor_menu->SetBackgroundColour(wxColour(250, 250, 250));

	//Buttons
	wxButton* enter_btn_tutor = new wxButton(tutor_login_panel, wxID_ANY, "Submit", wxPoint(300, 350), wxSize(100, 35));
	wxButton* forgor_btn_tutor = new wxButton(tutor_login_panel, wxID_ANY, "Forgot Password?", wxPoint(400, 350), wxSize(100, 35));
	wxButton* to_tutor_register_btn1 = new wxButton(tutor_login_panel, wxID_ANY, "Register as Tutor", wxPoint(300, 400), wxSize(200, 35));
	wxButton* to_student_button = new wxButton(tutor_login_panel, wxID_ANY, "Student Portal", wxPoint(300, 450), wxSize(200, 35));

	//Text Fields
	wxStaticText* username_text_tutor = new wxStaticText(tutor_login_panel, wxID_ANY, "Username/Email:", wxPoint(270, 250), wxSize(125, -1), wxALIGN_CENTER_HORIZONTAL);
	username_text_tutor->SetFont(f2);
	username_text_tutor->SetBackgroundColour(wxColour(250, 250, 250));
	wxStaticText* password_text_tutor = new wxStaticText(tutor_login_panel, wxID_ANY, "Password:", wxPoint(270, 300), wxSize(125, -1), wxALIGN_CENTER_HORIZONTAL);
	password_text_tutor->SetFont(f2);
	password_text_tutor->SetBackgroundColour(wxColour(250, 250, 250));

	//Typing Fields
	tutor_username = new wxTextCtrl(tutor_login_panel, wxID_ANY, "", wxPoint(405, 250), wxSize(125, -1));
	tutor_password = new wxTextCtrl(tutor_login_panel, wxID_ANY, "", wxPoint(405, 300), wxSize(125, -1), wxTE_PASSWORD);

	//button functions
	enter_btn_tutor->Bind(wxEVT_BUTTON, &LoginFrame::enter_tutor, this);
	forgor_btn_tutor->Bind(wxEVT_BUTTON, &LoginFrame::forgor_button, this);
	to_student_button->Bind(wxEVT_BUTTON, &LoginFrame::to_student, this);
	to_tutor_register_btn1->Bind(wxEVT_BUTTON, &LoginFrame::to_tutor_register, this);


	//Student Register Page----------------------------------------------------------------------------

	wxPanel* student_register_panel = new wxPanel(login_switch);
	wxStaticText* student_register_menu = new wxStaticText(student_register_panel, wxID_ANY, "Student Account Creation", wxPoint(0, 25), wxSize(800, -1), wxALIGN_CENTER_HORIZONTAL);
	student_register_menu->SetFont(f1);
	student_register_menu->SetBackgroundColour(wxColour(250, 250, 250));

	//Text Fields
	wxStaticText* student_enter_name = new wxStaticText(student_register_panel,wxID_ANY,"Enter Name:", wxPoint(270, 150), wxSize(125, -1), wxALIGN_CENTER_HORIZONTAL);
		student_enter_name->SetFont(f2);
		student_enter_name->SetBackgroundColour(wxColour(250, 250, 250));
	wxStaticText* student_enter_email = new wxStaticText(student_register_panel, wxID_ANY, "Enter Email:", wxPoint(270, 200), wxSize(125, -1), wxALIGN_CENTER_HORIZONTAL);
		student_enter_email->SetFont(f2);
		student_enter_email->SetBackgroundColour(wxColour(250, 250, 250));
	wxStaticText* student_enter_password = new wxStaticText(student_register_panel, wxID_ANY, "Enter Password:", wxPoint(270, 250), wxSize(125, -1), wxALIGN_CENTER_HORIZONTAL);
		student_enter_password->SetFont(f2);
		student_enter_password->SetBackgroundColour(wxColour(250, 250, 250));

		//Typing Fields
	new_student_name = new wxTextCtrl(student_register_panel, wxID_ANY, "", wxPoint(405, 150), wxSize(125, -1));
	new_student_email = new wxTextCtrl(student_register_panel, wxID_ANY, "", wxPoint(405, 200), wxSize(125, -1));
	new_student_password = new wxTextCtrl(student_register_panel, wxID_ANY, "", wxPoint(405, 250), wxSize(125, -1), wxTE_PASSWORD);

	wxButton* create_student = new wxButton(student_register_panel, wxID_ANY, "Create Account", wxPoint(300, 300), wxSize(200, 35));
	wxButton* to_tutor = new wxButton(student_register_panel, wxID_ANY, "Register as Tutor", wxPoint(300, 350), wxSize(200, 35));
	wxButton* to_student_login = new wxButton(student_register_panel, wxID_ANY, "Back to Login", wxPoint(300, 400), wxSize(200, 35));

	//button functions
	to_tutor->Bind(wxEVT_BUTTON, &LoginFrame::to_tutor_register, this);
	to_student_login->Bind(wxEVT_BUTTON, &LoginFrame::to_student, this);
	create_student->Bind(wxEVT_BUTTON, &LoginFrame::create_student, this);


	//Tutor Register Page----------------------------------------------------------------------------

	wxPanel* tutor_register_panel = new wxPanel(login_switch);
	wxStaticText* tutor_register_menu = new wxStaticText(tutor_register_panel, wxID_ANY, "Tutor Account Creation", wxPoint(0, 25), wxSize(800, -1), wxALIGN_CENTER_HORIZONTAL);
		tutor_register_menu->SetFont(f1);
		tutor_register_menu->SetBackgroundColour(wxColour(250, 250, 250));

	//Text Fields
	wxStaticText* tutor_enter_name = new wxStaticText(tutor_register_panel, wxID_ANY, "Enter Name:", wxPoint(270, 125), wxSize(125, -1), wxALIGN_CENTER_HORIZONTAL);
		tutor_enter_name->SetFont(f2);
		tutor_enter_name->SetBackgroundColour(wxColour(250, 250, 250));
	wxStaticText* tutor_enter_email = new wxStaticText(tutor_register_panel, wxID_ANY, "Enter Email:", wxPoint(270, 160), wxSize(125, -1), wxALIGN_CENTER_HORIZONTAL);
		tutor_enter_email->SetFont(f2);
		tutor_enter_email->SetBackgroundColour(wxColour(250, 250, 250));
	wxStaticText* tutor_enter_password = new wxStaticText(tutor_register_panel, wxID_ANY, "Enter Password:", wxPoint(270, 195), wxSize(125, -1), wxALIGN_CENTER_HORIZONTAL);
		tutor_enter_password->SetFont(f2);
		tutor_enter_password->SetBackgroundColour(wxColour(250, 250, 250));
	wxStaticText* pick_subjects = new wxStaticText(tutor_register_panel, wxID_ANY, "Choose Subjects", wxPoint(270, 230), wxSize(125, -1), wxALIGN_CENTER_HORIZONTAL);
		pick_subjects->SetFont(f2);
		pick_subjects->SetBackgroundColour(wxColour(250, 250, 250));
	wxStaticText* pick_days = new wxStaticText(tutor_register_panel, wxID_ANY, "Available Days", wxPoint(405, 230), wxSize(125, -1), wxALIGN_CENTER_HORIZONTAL);
		pick_days->SetFont(f2);
		pick_days->SetBackgroundColour(wxColour(250, 250, 250));

	//Typing Fields
	new_tutor_name = new wxTextCtrl(tutor_register_panel, wxID_ANY, "", wxPoint(405, 125), wxSize(125, -1));
	new_tutor_email = new wxTextCtrl(tutor_register_panel, wxID_ANY, "", wxPoint(405, 160), wxSize(125, -1));
	new_tutor_password = new wxTextCtrl(tutor_register_panel, wxID_ANY, "", wxPoint(405, 195), wxSize(125, -1), wxTE_PASSWORD);

	//Selections
	wxArrayString subjects_to_choose;
	subjects_to_choose.Add("Math");
	subjects_to_choose.Add("Physics");
	subjects_to_choose.Add("Chemistry");
	subjects_to_choose.Add("Biology");
	subjects_to_choose.Add("Computer Science");
	subjects_to_choose.Add("History");
	subjects_to_choose.Add("English");
	subjects_to_choose.Add("Writing");
	subjects_to_choose.Add("Law");
	wxArrayString days_to_choose;
	days_to_choose.Add("Sunday");
	days_to_choose.Add("Monday");
	days_to_choose.Add("Tuesday");
	days_to_choose.Add("Wednesday");
	days_to_choose.Add("Thursday");
	days_to_choose.Add("Friday");
	days_to_choose.Add("Saturday");
	choose_subjects = new wxListBox(tutor_register_panel, wxID_ANY, wxPoint(270, 255), wxSize(125, 130), subjects_to_choose, wxLB_MULTIPLE| wxLB_NEEDED_SB| wxLB_SORT);
	choose_days = new wxListBox(tutor_register_panel, wxID_ANY, wxPoint(405,255), wxSize(125,130), days_to_choose,wxLB_MULTIPLE);
	//Buttons
	wxButton* create_tutor = new wxButton(tutor_register_panel, wxID_ANY, "Create Account", wxPoint(300, 400), wxSize(200, 35));
	wxButton* to_student = new wxButton(tutor_register_panel, wxID_ANY, "Register as Student", wxPoint(300, 450), wxSize(200, 35));
	wxButton* to_tutor_login = new wxButton(tutor_register_panel, wxID_ANY, "Back to Login", wxPoint(300, 500), wxSize(200, 35));
	//button functions
		to_student->Bind(wxEVT_BUTTON, &LoginFrame::to_student_register, this);
		to_tutor_login->Bind(wxEVT_BUTTON, &LoginFrame::to_tutor, this);
		create_tutor->Bind(wxEVT_BUTTON, &LoginFrame::create_tutor, this);


	//Student Home Page----------------------------------------------------------------------------

	wxPanel* student_home_page = new wxPanel(login_switch);
		student_home_page->SetBackgroundColour(wxColour(255, 255, 255));
	wxBoxSizer* student_home_sizer = new wxBoxSizer(wxVERTICAL);

	//Display title
		student_home_menu = new wxStaticText(student_home_page, wxID_ANY, 
    	"Student Home Page", // Generic placeholder
   		wxPoint(0, 25), wxSize(800, -1), wxALIGN_CENTER_HORIZONTAL);
		student_home_menu->SetFont(f1);
		student_home_menu->SetBackgroundColour(wxColour(250, 250, 250));
		student_home_sizer->Add(student_home_menu,0,wxALL|wxEXPAND,20);
		
	//Create Tabs
	student_home_pages = new wxNotebook(student_home_page,wxID_ANY);

		//Build the Create Requests Tab
		wxPanel* create_requests = new wxPanel(student_home_pages,wxID_ANY);
		create_requests->SetBackgroundColour(wxColour(250,250,250));
		
		//create book to hold pages of requests tab
		tutor_select = new wxSimplebook(create_requests,wxID_ANY);

		//Criteria Selection Page----------------------------------------
		wxPanel* criteria_input_panel = new wxPanel(tutor_select,wxID_ANY);
				criteria_input_panel->SetBackgroundColour(wxColour(250,250,250));

		//add text 
		wxStaticText* priority_title = new wxStaticText(criteria_input_panel,wxID_ANY,"Priority (Select One)",wxPoint(50, 50), wxSize(200, -1));
					priority_title->SetFont(f2);
		wxStaticText* select_subject_title = new wxStaticText(criteria_input_panel,wxID_ANY,"Select Subject",wxPoint(50, 150), wxSize(200, -1));
				select_subject_title->SetFont(f2);
		wxStaticText* Sessions_day_title = new wxStaticText(criteria_input_panel,wxID_ANY,"Session Day (Select All That Apply)",wxPoint(50, 250), wxSize(200, -1));
				Sessions_day_title->SetFont(f2);
		//Define request priorities
		wxArrayString priority_choices;
		priority_choices.Add("Low");
		priority_choices.Add("Medium");
		priority_choices.Add("High");
		//add request choice, and subject choice
		priority_select = new wxRadioBox(criteria_input_panel,wxID_ANY,"",wxPoint(325,50),wxDefaultSize,priority_choices);
		choose_request_subject = new wxChoice(criteria_input_panel,wxID_ANY, wxPoint(325, 150), wxSize(200, -1),subjects_to_choose);

		//add check boxes
		wxPanel* checkbox_panel = new wxPanel(criteria_input_panel,wxID_ANY,wxPoint(325,250), wxSize(450, 120));
		wxGridSizer* days_sizer = new wxGridSizer(2,4,15,35);
		day_checkboxes.clear();
		for(size_t i = 0; i<days_to_choose.GetCount(); i++){
			wxCheckBox* cb = new wxCheckBox(checkbox_panel, wxID_ANY,days_to_choose[i] );

			days_sizer->Add(cb,0,wxALIGN_CENTER_VERTICAL);

			day_checkboxes.push_back(cb);
		}
		checkbox_panel->SetSizerAndFit(days_sizer);

		//add submit button
		wxButton* submit_request = new wxButton(criteria_input_panel,wxID_ANY,"Submit Criteria",wxPoint(50,300),wxSize(200,35));
		submit_request->Bind(wxEVT_BUTTON,&LoginFrame::on_find_tutors,this);
	//Show Tutors-----------------------------------------------------------
		wxPanel* tutor_selection_panel = new wxPanel(tutor_select, wxID_ANY);

		wxStaticText* results_title = new wxStaticText(tutor_selection_panel, wxID_ANY, "Available Tutors", wxPoint(20, 20));
			
		tutor_select->AddPage(criteria_input_panel, "Criteria Form");
        tutor_select->AddPage(tutor_selection_panel, "Results List");	

	//View Requests tab
		wxBoxSizer* tab1_sizer = new wxBoxSizer(wxVERTICAL);
    	tab1_sizer->Add(tutor_select, 1, wxEXPAND); // The simplebook fills the whole tab
    	create_requests->SetSizer(tab1_sizer);
		wxPanel* view_requests_tab = new wxPanel(student_home_pages, wxID_ANY);
		wxStaticText* history_title = new wxStaticText(view_requests_tab, wxID_ANY, "Your Request History", wxPoint(20, 20));

	
		student_home_pages->AddPage(create_requests, "Create Requests");
    	student_home_pages->AddPage(view_requests_tab, "View Requests");

		student_home_sizer->Add(student_home_pages, 1, wxEXPAND | wxALL, 10);
   		student_home_page->SetSizer(student_home_sizer);
		

	//Tutor Home Page------------------------------------------------------------------------------------------------------------------------------------
		wxPanel* tutor_home_page = new wxPanel(login_switch);
		tutor_home_page->SetBackgroundColour(wxColour(250,250,250));
		wxBoxSizer* tutor_home_sizer = new wxBoxSizer(wxVERTICAL);

		wxStaticText* tutor_home_title = new wxStaticText(tutor_home_page, wxID_ANY, "Tutor Dashboard", wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER_HORIZONTAL);
    		tutor_home_title->SetFont(f1);
			tutor_home_sizer->Add(tutor_home_title,0,wxALL|wxEXPAND,20);

		tutor_home_pages = new wxNotebook(tutor_home_page,wxID_ANY);

		wxPanel* tutor_request_tab = new wxPanel(tutor_home_pages,wxID_ANY);
		wxBoxSizer* request_sizer = new wxBoxSizer(wxVERTICAL);

		tutor_requests= new wxListBox(tutor_request_tab,wxID_ANY,wxDefaultPosition,wxSize(-1,200));
		
		wxButton* accept = new wxButton(tutor_request_tab, wxID_ANY, "Accept Request",wxPoint(100,200),wxSize(200,35));
		wxButton* deny = new wxButton(tutor_request_tab, wxID_ANY, "Deny Request",wxPoint(100,300),wxSize(200,35));

	//Account-----------------------------------------
		wxPanel* tutor_account = new wxPanel(tutor_home_pages, wxID_ANY);
		tutor_account->SetBackgroundColour(wxColour(250, 250, 250));
		wxBoxSizer* account_sizer = new wxBoxSizer(wxVERTICAL);
		
		wxStaticText* tutor_acc_title = new wxStaticText(tutor_account, wxID_ANY, "Update Account Details");
		
		wxStaticText* lbl_name = new wxStaticText(tutor_account, wxID_ANY, "Name:");
   		tutor_update_name = new wxTextCtrl(tutor_account, wxID_ANY);

    	// Email
   		 wxStaticText* lbl_email = new wxStaticText(tutor_account, wxID_ANY, "Email:");
   		tutor_update_email = new wxTextCtrl(tutor_account, wxID_ANY);

    	// Password
    	wxStaticText* lbl_pass = new wxStaticText(tutor_account, wxID_ANY, "New Password:");
    	tutor_update_password = new wxTextCtrl(tutor_account, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxTE_PASSWORD);

		wxButton* save_tutor_account = new wxButton(tutor_account, wxID_ANY, "Save Changes");


	    // Add tabs to notebook
   		tutor_home_pages->AddPage(tutor_home_page, "Request Management");
    	tutor_home_pages->AddPage(tutor_account, "Account Settings");

    // Add the page to the main login switcher



	// Page Bindings-----------------------------------------------------------------------------------
	login_switch->AddPage(student_login_panel, "Student Login");
	login_switch->AddPage(tutor_login_panel, "Tutor Login");
	login_switch->AddPage(student_register_panel, "Tutor Login");
	login_switch->AddPage(tutor_register_panel, "Tutor Login");
	login_switch->AddPage(student_home_page, "Tutor Login");
	login_switch->AddPage(tutor_home_page, "Tutor Home");
}

//Button functions
void LoginFrame::to_student_register(wxCommandEvent& evt) {
	login_switch->ChangeSelection(2);
}
void LoginFrame::to_tutor_register(wxCommandEvent& evt) {
	login_switch->ChangeSelection(3);
}
void LoginFrame::forgor_button(wxCommandEvent& evt) {
	wxMessageBox("Contact help@educonnect.com to reset your password","Forgot Password", wxOK | wxICON_INFORMATION);
}
void LoginFrame::to_tutor(wxCommandEvent& evt) {
	login_switch->ChangeSelection(1);
}
void LoginFrame::to_student(wxCommandEvent& evt) {
	login_switch->ChangeSelection(0);
}
void LoginFrame::create_student(wxCommandEvent& evt) {
	if (new_student_name->IsEmpty()) {
		wxMessageBox("Please Enter a Name ", "Failed Account Creation", wxOK | wxICON_INFORMATION);
	}
	else if (new_student_email->IsEmpty()) {
		wxMessageBox("Please Enter an Email ", "Failed Account Creation", wxOK | wxICON_INFORMATION);
	}
	else if (new_student_password->IsEmpty()) {
		wxMessageBox("Please Enter a Password ", "Failed Account Creation", wxOK | wxICON_INFORMATION);
    }
    else if(!(connection_system->get_tutor(new_student_email->GetValue().ToStdString())==nullptr)){
            wxMessageBox("An Account With This Email Already Exists ", "Failed Account Creation", wxOK | wxICON_INFORMATION);
    }
    else{
        connection_system->register_student(new_student_name->GetValue().ToStdString(),
        new_student_email->GetValue().ToStdString(),
        new_student_password->GetValue().ToStdString());
        wxMessageBox("Account Creation Successful", "Account Created", wxOK | wxICON_INFORMATION);
    }    
}
void LoginFrame::create_tutor(wxCommandEvent& evt) {

    wxArrayInt subject_indices;
    wxArrayInt  day_indices;

    choose_subjects->GetSelections(subject_indices);
    choose_days->GetSelections(day_indices);

	if (new_tutor_name->IsEmpty()) {
		wxMessageBox("Please Enter a Name ", "Failed Account Creation", wxOK | wxICON_INFORMATION);
	}
	else if (new_tutor_email->IsEmpty()) {
		wxMessageBox("Please Enter an Email ", "Failed Account Creation", wxOK | wxICON_INFORMATION);
	}
	else if (new_tutor_password->IsEmpty()) {
		wxMessageBox("Please Enter a Password ", "Failed Account Creation", wxOK | wxICON_INFORMATION);
    }
    else if(subject_indices.IsEmpty()){
        wxMessageBox("Select at Least 1 Subject", "Failed Account Creation", wxOK | wxICON_INFORMATION);
    }
    else if(day_indices.IsEmpty()){
        wxMessageBox("Select at Least 1 Available Day", "Failed Account Creation", wxOK | wxICON_INFORMATION);
    }
    else if(!(connection_system->get_tutor(new_tutor_email->GetValue().ToStdString())==nullptr)){//break in
        wxMessageBox("An Account With This Email Already Exists ", "Failed Account Creation", wxOK | wxICON_INFORMATION);
    }
    else{
        std::vector<std::string> final_subjects;
        std::vector<bool> final_days(7,false);

        for(size_t i =0; i<subject_indices.GetCount();i++){
            int index = subject_indices[i];
            std::string s = choose_subjects->GetString(index).ToStdString();
            final_subjects.push_back(s);
        }
        for(size_t i =0; i<day_indices.GetCount();i++){
            int index = day_indices[i];
            if(index>=0 && index<7)
            final_days[index]=true;
        }
        connection_system->register_tutor(new_tutor_name->GetValue().ToStdString(),
        new_tutor_email->GetValue().ToStdString(),
        new_tutor_password->GetValue().ToStdString(),
        final_days, final_subjects);
        wxMessageBox("Account Creation Successful", "Account Created", wxOK | wxICON_INFORMATION);
    }    
}

void LoginFrame::enter_student(wxCommandEvent& evt) {
	std::string username = student_username->GetValue().ToStdString();
	std::string password = student_password->GetValue().ToStdString();
	if (connection_system->student_login(username, password))
	{
 		Student* s = connection_system->get_student(student_username->GetValue().ToStdString());        
        // 2. Update the label with their real name
        if (s != nullptr) {
            student_home_menu->SetLabel(s->get_name());
            // Force a refresh so the text re-centers/updates immediately
            student_home_menu->Refresh(); 
        }
		wxMessageBox("Login Successful!", "Success", wxOK | wxICON_INFORMATION);
		login_switch->ChangeSelection(4);
	}
	else
	{
		wxMessageBox("Invalid Username or Password.", "Login Failed", wxOK | wxICON_ERROR);
	}
}
void LoginFrame::enter_tutor(wxCommandEvent& evt) {
	std::string username = tutor_username->GetValue().ToStdString();
	std::string password = tutor_password->GetValue().ToStdString();
	if (connection_system->tutor_login(username, password))
	{
		wxMessageBox("Login Successful!", "Success", wxOK | wxICON_INFORMATION);
		login_switch->ChangeSelection(5);
	}
	else
	{
		wxMessageBox("Invalid Username or Password.", "Login Failed", wxOK | wxICON_ERROR);
	}
}

void LoginFrame::create_request(wxCommandEvent& evt){
}
void LoginFrame::on_find_tutors(wxCommandEvent& evt) {

	bool day = false;
	std::vector<std::string> selected_days_strings;
	for(wxCheckBox* cb : day_checkboxes){
		if(cb->GetValue()){
			day=true;
            selected_days_strings.push_back(cb->GetLabel().ToStdString());
		}
	}
	if(choose_request_subject->GetSelection()==wxNOT_FOUND){
		wxMessageBox("Please select a Subject.", "Validation Error", wxOK | wxICON_WARNING);
		return;
	}
	else if(!day){
		wxMessageBox("Please select at least one Session Day.", "Validation Error", wxOK | wxICON_WARNING);
		return;
	}
	else{

	std::string subject = choose_request_subject->GetStringSelection().ToStdString();
	std::vector<bool> requireddays(7,false);

	for(size_t i = 0;i<day_checkboxes.size();i++){
		if(day_checkboxes[i]->GetValue()){
			requireddays[i]=true;
		}
	}
	current_search_results = connection_system->get_tutors_for_subject(subject,"Rating",requireddays);
	if(current_search_results.empty()){
		wxMessageBox("No tutors found for that subject and schedule.", "No Matches");
		return;
	}
	tutor_results_list->Clear();

	for(Tutor* t : current_search_results){
		std::string label = t->get_name()+" (Rating: " +
		std::to_string(t->avg_rating()).substr(0,3) + ")";
		tutor_results_list->Append(label);
	}
	
	tutor_select->ChangeSelection(1);
	}
}