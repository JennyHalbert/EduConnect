#include "LoginFrame.h"
#include "EduConnectSystem.h"
#include <wx/wx.h>

//Class to handle Ratings Popup
class RatingDialog : public wxDialog{
public:
	RatingDialog(wxWindow* parent, std::string tutor_name):wxDialog(parent, wxID_ANY, "Rate Tutor", wxDefaultPosition, wxSize(300,250))
{
	wxBoxSizer* vSizer = new wxBoxSizer(wxVERTICAL);
	
	//title
		wxStaticText* label = new wxStaticText(this, wxID_ANY, "Rate your session with " + tutor_name);
		vSizer->Add(label, 0 , wxALIGN_CENTER| wxTOP,20);
		m_slider = new wxSlider(this, wxID_ANY, 5, 1, 5,wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL | wxSL_LABELS);
        vSizer->Add(m_slider, 0, wxEXPAND | wxALL, 20);
		wxSizer* btnSizer = CreateButtonSizer(wxOK | wxCANCEL);
		vSizer->Add(btnSizer, 0, wxALIGN_CENTER | wxBOTTOM, 20);
		SetSizer(vSizer);
}
	int GetRating() const { return m_slider->GetValue(); }

	private:
		wxSlider* m_slider;
};


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
		wxStaticText* select_subject_title = new wxStaticText(criteria_input_panel,wxID_ANY,"Select Subject",wxPoint(50, 110), wxSize(200, -1));
				select_subject_title->SetFont(f2);
		wxStaticText* Sessions_day_title = new wxStaticText(criteria_input_panel,wxID_ANY,"Session Day (Select All That Apply)",wxPoint(50, 250), wxSize(200, -1));
				Sessions_day_title->SetFont(f2);
		wxStaticText* desc_title = new wxStaticText(criteria_input_panel, wxID_ANY, "Description of Request", wxPoint(50, 175), wxSize(200, -1));
				desc_title->SetFont(f2); // Use your standard font
				//take description
		request_description = new wxTextCtrl(criteria_input_panel, wxID_ANY, "", wxPoint(325, 150), wxSize(300, 60), wxTE_MULTILINE);
		//Define request priorities
		wxArrayString priority_choices;
		priority_choices.Add("Low");
		priority_choices.Add("Medium");
		priority_choices.Add("High");
		//add request choice, and subject choice
		priority_select = new wxRadioBox(criteria_input_panel,wxID_ANY,"",wxPoint(325,50),wxDefaultSize,priority_choices);
		choose_request_subject = new wxChoice(criteria_input_panel,wxID_ANY, wxPoint(325, 110), wxSize(200, -1),subjects_to_choose, wxCB_SORT);

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
		wxButton* submit_request = new wxButton(criteria_input_panel,wxID_ANY,"Submit Criteria",wxPoint(50,350),wxSize(200,35));
		submit_request->Bind(wxEVT_BUTTON,&LoginFrame::on_find_tutors,this);
		//add Logout button
		wxButton* student_logout = new wxButton(criteria_input_panel,wxID_ANY,"Logout",wxPoint(525,350),wxSize(200,35));
		student_logout->Bind(wxEVT_BUTTON,&LoginFrame::perform_logout,this);

		//Show Tutors-----------------------------------------------------------
		wxPanel* tutor_selection_panel = new wxPanel(tutor_select, wxID_ANY);

		wxStaticText* results_title = new wxStaticText(tutor_selection_panel, wxID_ANY, "Available Tutors", wxPoint(40, 5));
			results_title->SetBackgroundColour(wxColour(250,250,250));
			results_title->SetFont(f1);
		tutor_results_list = new wxCheckListBox(tutor_selection_panel, wxID_ANY, wxPoint(40, 100), wxSize(600, 250));

		wxStaticText* sort_criteria_text = new wxStaticText(tutor_selection_panel, wxID_ANY, "Choose Sorting Criteria", wxPoint(40,75),wxSize(200,-1));
			sort_criteria_text->SetFont(f2);
			sort_criteria_text->SetBackgroundColour(wxColour(250, 250, 250));

		wxArrayString sorting_criteria;
		sorting_criteria.Add("Rating");
		sorting_criteria.Add("Alphabetical");
		sorting_criteria.Add("Total Completions");
		sorting_criteria.Add("Completion Rate");

		choose_sorting_criteria = new wxChoice(tutor_selection_panel,wxID_ANY, wxPoint(240, 75), wxSize(200, -1),sorting_criteria );
		choose_sorting_criteria->SetSelection(0);

		wxButton* send_request_btn = new wxButton(tutor_selection_panel, wxID_ANY, "Send Requests", wxPoint(40, 380), wxSize(170, 35));
		wxButton* back_btn = new wxButton(tutor_selection_panel, wxID_ANY, "Back to Search", wxPoint(200, 380), wxSize(150, 35));

		choose_sorting_criteria->Bind(wxEVT_CHOICE,&LoginFrame::on_find_tutors,this);
		send_request_btn->Bind(wxEVT_BUTTON, &LoginFrame::on_send_requests, this);
		back_btn->Bind(wxEVT_BUTTON, [this](wxCommandEvent& evt){tutor_select->ChangeSelection(0);});

		wxBoxSizer* create_tab_sizer = new wxBoxSizer(wxVERTICAL);
		create_tab_sizer->Add(tutor_select, 1, wxEXPAND | wxALL, 0);
    
    	// Set the sizer for the panel
   		 create_requests->SetSizer(create_tab_sizer);

		tutor_select->AddPage(criteria_input_panel, "Criteria Form");
        tutor_select->AddPage(tutor_selection_panel, "Results List");	

	//View Requests tab
    	wxPanel* student_view_requests_tab = new wxPanel(student_home_pages, wxID_ANY);
    	wxBoxSizer* history_sizer = new wxBoxSizer(wxVERTICAL);

    	// Active Requests
    	wxStaticText* student_active_title = new wxStaticText(student_view_requests_tab, wxID_ANY, "Active Requests");
    	wxFont titleFont = student_active_title->GetFont();
    	student_active_title->SetFont(f2);

        // Active_List Box
    	student_active_requests_list = new wxListBox(student_view_requests_tab, wxID_ANY, wxDefaultPosition, wxSize(-1, 150));
    
    	// Buttons row
    	wxBoxSizer* action_sizer = new wxBoxSizer(wxHORIZONTAL);
    	student_mark_complete_btn = new wxButton(student_view_requests_tab, wxID_ANY, "Mark Selected as Complete");
    	student_request_refresh_btn = new wxButton(student_view_requests_tab, wxID_ANY, "Refresh Lists");
       		action_sizer->Add(student_mark_complete_btn, 0, wxRIGHT, 10);
    		action_sizer->Add(student_request_refresh_btn, 0);

    	// Request History box
    	wxStaticText* history_title_lbl = new wxStaticText(student_view_requests_tab, wxID_ANY, "History (Last 10)");
    	history_title_lbl->SetFont(f2);

    	student_history_requests_list = new wxListBox(student_view_requests_tab, wxID_ANY, wxDefaultPosition, wxSize(-1, 150));

    	// view request page layout
    	history_sizer->Add(student_active_title, 0, wxALL, 10);
    	history_sizer->Add(student_active_requests_list, 1, wxEXPAND | wxLEFT | wxRIGHT, 10);
    	history_sizer->Add(action_sizer, 0, wxALL, 10);
    	history_sizer->Add(history_title_lbl, 0, wxALL, 10);
    	history_sizer->Add(student_history_requests_list, 1, wxEXPAND | wxALL, 10);

    	student_view_requests_tab->SetSizer(history_sizer);
    	// Button binding
    	student_mark_complete_btn->Bind(wxEVT_BUTTON, &LoginFrame::student_on_mark_complete, this);
    	student_request_refresh_btn->Bind(wxEVT_BUTTON, &LoginFrame::student_on_refresh_click, this);
		student_active_requests_list->Bind(wxEVT_LISTBOX_DCLICK, &LoginFrame::on_request_double_click,this);

		//Student Account Details
		wxPanel* student_account_details_tab = new wxPanel(student_home_pages, wxID_ANY);
    	wxBoxSizer* acc_sizer = new wxBoxSizer(wxVERTICAL);
		//Title
			wxStaticText* acc_title = new wxStaticText(student_account_details_tab, wxID_ANY, "Manage Account Details");
    		acc_title->SetFont(f1);
		//Name
			wxStaticText* student_new_name = new wxStaticText(student_account_details_tab, wxID_ANY, "Full Name:");
    		student_new_name->SetFont(f2);
    		acc_name_input = new wxTextCtrl(student_account_details_tab, wxID_ANY, "");
		//Email
			wxStaticText* student_new_email = new wxStaticText(student_account_details_tab, wxID_ANY, "Email Address:");
    		student_new_email->SetFont(f2);
    		acc_email_input = new wxTextCtrl(student_account_details_tab, wxID_ANY, "");
		//Password
			wxStaticText* student_new_pass = new wxStaticText(student_account_details_tab, wxID_ANY, "Password:");
    		student_new_pass->SetFont(f2);
    		acc_pass_input = new wxTextCtrl(student_account_details_tab, wxID_ANY, "");
		//Button
			wxButton* update_btn = new wxButton(student_account_details_tab, wxID_ANY, "Save Changes", wxDefaultPosition, wxSize(150, 40));
    		update_btn->Bind(wxEVT_BUTTON, &LoginFrame::on_student_update_account, this);
		//Page layout
			acc_sizer->Add(acc_title, 0, wxALL | wxALIGN_CENTER, 20);
    		acc_sizer->Add(student_new_name, 0, wxLEFT | wxTOP, 20);
    		acc_sizer->Add(acc_name_input, 0, wxEXPAND | wxLEFT | wxRIGHT, 20);    
    		acc_sizer->Add(student_new_email, 0, wxLEFT | wxTOP, 20);
    		acc_sizer->Add(acc_email_input, 0, wxEXPAND | wxLEFT | wxRIGHT, 20);
    		acc_sizer->Add(student_new_pass, 0, wxLEFT | wxTOP, 20);
    		acc_sizer->Add(acc_pass_input, 0, wxEXPAND | wxLEFT | wxRIGHT, 20);
    		acc_sizer->Add(update_btn, 0, wxALL | wxALIGN_CENTER, 30);
			student_account_details_tab->SetSizer(acc_sizer);

			student_home_pages->AddPage(create_requests, "Create Requests");
    		student_home_pages->AddPage(student_view_requests_tab, "View Requests");
			student_home_pages->AddPage(student_account_details_tab, "Account Details");

		student_home_sizer->Add(student_home_pages, 1, wxEXPAND | wxALL, 10);
   		student_home_page->SetSizer(student_home_sizer);
		

	//Tutor Home Page ------------------------------------------------------------------------------------------------------------------------------------
		wxPanel* tutor_home_page = new wxPanel(login_switch);
		tutor_home_page->SetBackgroundColour(wxColour(250,250,250));
		wxBoxSizer* tutor_home_sizer = new wxBoxSizer(wxVERTICAL);

		tutor_home_menu = new wxStaticText(tutor_home_page, wxID_ANY, 
			"Tutor Dashboard", wxPoint(0, 25), wxSize(800, -1), wxALIGN_CENTER_HORIZONTAL);
    		tutor_home_menu->SetFont(f1);
			tutor_home_menu->SetBackgroundColour(wxColour(250, 250, 250));
			tutor_home_sizer->Add(tutor_home_menu,0,wxALL|wxEXPAND,20);

		tutor_home_pages = new wxNotebook(tutor_home_page,wxID_ANY);

	//Tutor Request Management Page
		wxPanel* tutor_request_tab = new wxPanel(tutor_home_pages,wxID_ANY);
		wxBoxSizer* request_sizer = new wxBoxSizer(wxVERTICAL);
		wxBoxSizer* request_sizer_horizontal = new wxBoxSizer(wxHORIZONTAL);

		//Inbox
			wxStaticText* inbox_label = new wxStaticText(tutor_request_tab, wxID_ANY, "Inbox (Potential Requests)");
    			inbox_label->SetFont(f2);		
			tutor_inbox_list = new wxListBox(tutor_request_tab, wxID_ANY, wxDefaultPosition, wxSize(-1, 150));
    		wxButton* accept_request_btn = new wxButton(tutor_request_tab, wxID_ANY, "Accept Selected Request");
		//Accepted
			wxStaticText* active_label = new wxStaticText(tutor_request_tab, wxID_ANY, "Active Sessions");
   				active_label->SetFont(f2);
    		tutor_active_list = new wxListBox(tutor_request_tab, wxID_ANY, wxDefaultPosition, wxSize(-1, 150));
    		wxButton* close_request_btn = new wxButton(tutor_request_tab, wxID_ANY, "Mark Selected as Complete");
			wxButton* tutor_logout = new wxButton(tutor_request_tab, wxID_ANY, "Logout");
			tutor_logout->Bind(wxEVT_BUTTON,&LoginFrame::perform_logout,this);


		//Page layout
			request_sizer_horizontal->Add(tutor_logout,0,wxLEFT|wxRIGHT,20);
			request_sizer_horizontal->Add(close_request_btn,0,wxLEFT|wxRIGHT,20);

    		request_sizer->Add(inbox_label, 0, wxLEFT | wxTOP, 10);
    		request_sizer->Add(tutor_inbox_list, 1, wxEXPAND | wxALL, 10);
    		request_sizer->Add(accept_request_btn, 0, wxALIGN_RIGHT | wxRIGHT, 10);    
    		request_sizer->Add(active_label, 0, wxLEFT | wxTOP, 10);
    		request_sizer->Add(tutor_active_list, 1, wxEXPAND | wxALL, 10);
    		request_sizer->Add(request_sizer_horizontal, 0, wxALIGN_CENTER_HORIZONTAL | wxLEFT|wxRIGHT | wxBOTTOM, 20);

			tutor_request_tab->SetSizer(request_sizer);

			accept_request_btn->Bind(wxEVT_BUTTON, &LoginFrame::on_tutor_accept_request, this);
    		close_request_btn->Bind(wxEVT_BUTTON, &LoginFrame::on_tutor_close_request, this);
			tutor_inbox_list->Bind(wxEVT_LISTBOX_DCLICK, &LoginFrame::on_request_double_click,this);
			tutor_active_list->Bind(wxEVT_LISTBOX_DCLICK, &LoginFrame::on_request_double_click,this);

			

	//Tutor Account Page -----------------------------------------------------------------------------------
		wxPanel* tutor_account = new wxPanel(tutor_home_pages, wxID_ANY);
		tutor_account->SetBackgroundColour(wxColour(250, 250, 250));
		wxBoxSizer* account_sizer = new wxBoxSizer(wxHORIZONTAL);
		wxBoxSizer* account_sizer_vertical = new wxBoxSizer(wxVERTICAL);//label, account_sizer
		wxGridSizer* account_grid_left = new wxGridSizer(7,2, wxSize(0,0));
		wxBoxSizer* account_right = new wxBoxSizer(wxVERTICAL);

		
		wxStaticText* tutor_acc_title = new wxStaticText(tutor_account, wxID_ANY, "Update Account Details");
		tutor_acc_title->SetFont(f1);
		tutor_acc_title->SetBackgroundColour(wxColour(250, 250, 250));
		//Name
		wxStaticText* lbl_name = new wxStaticText(tutor_account, wxID_ANY, "Name:");
			lbl_name->SetFont(f2);
			lbl_name->SetBackgroundColour(wxColour(250, 250, 250));
   		tutor_update_name = new wxTextCtrl(tutor_account, wxID_ANY);		
    	// Email
   		wxStaticText* lbl_email = new wxStaticText(tutor_account, wxID_ANY, "Email:");
			lbl_email->SetFont(f2);
			lbl_email->SetBackgroundColour(wxColour(250, 250, 250));
   		tutor_update_email = new wxTextCtrl(tutor_account, wxID_ANY);
    	// Password
    	wxStaticText* lbl_pass = new wxStaticText(tutor_account, wxID_ANY, "New Password:");
			lbl_pass->SetFont(f2);
			lbl_pass->SetBackgroundColour(wxColour(250, 250, 250));
    	tutor_update_password = new wxTextCtrl(tutor_account, wxID_ANY, "", wxDefaultPosition, wxDefaultSize);
		// Tutor Metrics:
			wxStaticText* lbl_avg_rating = new wxStaticText(tutor_account, wxID_ANY, "Average Rating:");
				lbl_avg_rating->SetFont(f2);
				lbl_avg_rating->SetBackgroundColour(wxColour(250, 250, 250));
			tutor_avg_rating = new wxStaticText(tutor_account, wxID_ANY, "Average Rating:");
				tutor_avg_rating->SetFont(f2);
				tutor_avg_rating->SetBackgroundColour(wxColour(250, 250, 250));
			wxStaticText* lbl_comp_rate = new wxStaticText(tutor_account, wxID_ANY, "Completion Rate");
				lbl_comp_rate->SetFont(f2);	
				lbl_comp_rate->SetBackgroundColour(wxColour(250, 250, 250));		
			tutor_completion_rate = new wxStaticText(tutor_account, wxID_ANY, "Completion Rate:");
				tutor_completion_rate->SetFont(f2);
				tutor_completion_rate->SetBackgroundColour(wxColour(250, 250, 250));
			wxStaticText* lbl_completions = new wxStaticText(tutor_account, wxID_ANY, "Completed Sessions:");
				lbl_completions->SetFont(f2);
				lbl_completions->SetBackgroundColour(wxColour(250, 250, 250));
			tutor_completions = new wxStaticText(tutor_account, wxID_ANY, "Completed Sessions:");
				tutor_completions->SetFont(f2);
				tutor_completions->SetBackgroundColour(wxColour(250, 250, 250));
			wxStaticText* lbl_matches = new wxStaticText(tutor_account, wxID_ANY, "Total Matches:");
				lbl_matches->SetFont(f2);
				lbl_matches->SetBackgroundColour(wxColour(250, 250, 250));
			tutor_matched = new wxStaticText(tutor_account, wxID_ANY, "Total Matches:");
				tutor_matched->SetFont(f2);
				tutor_matched->SetBackgroundColour(wxColour(250, 250, 250));

		//Subjects
		wxStaticText* lbl_subs = new wxStaticText(tutor_account, wxID_ANY, "Qualified Subjects (Multi-Select):");
			lbl_subs->SetFont(f2);
    	tutor_update_subjects = new wxListBox(tutor_account, wxID_ANY, wxDefaultPosition, wxSize(-1,100), subjects_to_choose, wxLB_MULTIPLE);
		//Days
    	wxStaticText* lbl_days = new wxStaticText(tutor_account, wxID_ANY, "Available Days (Multi-Select):");
			lbl_days->SetFont(f2);
    	tutor_update_days = new wxListBox(tutor_account, wxID_ANY, wxDefaultPosition, wxSize(-1,75), days_to_choose, wxLB_MULTIPLE);
		
		wxButton* save_tutor_account = new wxButton(tutor_account, wxID_ANY, "Save Changes");
			save_tutor_account->Bind(wxEVT_BUTTON, &LoginFrame::on_save_tutor_account, this);

		// Layout
			//add left side
				//add metrics
			account_grid_left->Add(lbl_avg_rating, 0, wxALL | wxALIGN_RIGHT, 10);
			account_grid_left->Add(tutor_avg_rating, 0, wxALL | wxLEFT, 10);
			account_grid_left->Add(lbl_comp_rate, 0, wxALL | wxALIGN_RIGHT, 10);
			account_grid_left->Add(tutor_completion_rate, 0, wxALL | wxLEFT, 10);
			account_grid_left->Add(lbl_completions, 0, wxALL | wxALIGN_RIGHT, 10);
			account_grid_left->Add(tutor_completions, 0, wxALL | wxLEFT, 10);
			account_grid_left->Add(lbl_matches, 0, wxALL | wxALIGN_RIGHT, 10);
			account_grid_left->Add(tutor_matched, 0, wxALL | wxLEFT, 10);
				//Add account details
			account_grid_left->Add(lbl_name, 0, wxALL | wxALIGN_RIGHT, 10);
			account_grid_left->Add(tutor_update_name, 0, wxALL | wxLEFT, 10);
			account_grid_left->Add(lbl_email, 0, wxALL | wxALIGN_RIGHT, 10);
			account_grid_left->Add(tutor_update_email, 0, wxALL | wxLEFT, 10);
			account_grid_left->Add(lbl_pass, 0, wxALL | wxALIGN_RIGHT, 10);
			account_grid_left->Add(tutor_update_password, 0, wxALL | wxLEFT, 10);
			
			//add right side
			account_right->Add(lbl_subs, 0,wxALIGN_LEFT|wxBOTTOM,5);
    		account_right->Add(tutor_update_subjects, 1, wxBOTTOM|wxEXPAND, 15);
    		account_right->Add(lbl_days, 0, wxBOTTOM|wxALIGN_LEFT, 5);
    		account_right->Add(tutor_update_days, 1, wxBOTTOM| wxEXPAND, 15);
			//add left and right
			account_sizer->Add(account_grid_left,1,wxALIGN_CENTER_VERTICAL|wxRIGHT,20);
			account_sizer->Add(account_right,1,wxEXPAND|wxLEFT,20);
			//add to main
			account_sizer_vertical->Add(tutor_acc_title, 1, wxDOWN| wxUP | wxALIGN_CENTER, 20);
			account_sizer_vertical->Add(account_sizer,4,wxEXPAND);
    		account_sizer_vertical->Add(save_tutor_account, 1, wxALL | wxALIGN_CENTER, 20);
 			tutor_account->SetSizer(account_sizer_vertical);
			account_sizer_vertical->SetSizeHints(this);


	    // Add tabs to notebook
   		tutor_home_pages->AddPage(tutor_request_tab, "Request Management");
    	tutor_home_pages->AddPage(tutor_account, "Account Settings");
		tutor_home_sizer->Add(tutor_home_pages,1,wxEXPAND | wxALL, 10);
		tutor_home_page->SetSizer(tutor_home_sizer);



	// Page Bindings-----------------------------------------------------------------------------------
	login_switch->AddPage(student_login_panel, "Student Login");
	login_switch->AddPage(tutor_login_panel, "Tutor Login");
	login_switch->AddPage(student_register_panel, "Tutor Login");
	login_switch->AddPage(tutor_register_panel, "Tutor Login");
	login_switch->AddPage(student_home_page, "Tutor Login");
	login_switch->AddPage(tutor_home_page, "Tutor Home");
}



//Logout Function
void LoginFrame::perform_logout(wxCommandEvent& evt){

    if (student_username) student_username->Clear();
    if (student_password) student_password->Clear();
    if (tutor_username) tutor_username->Clear();
    if (tutor_password) tutor_password->Clear();

    if (new_student_name) new_student_name->Clear();
    if (new_student_email) new_student_email->Clear();
    if (new_student_password) new_student_password->Clear();
    
    if (new_tutor_name) new_tutor_name->Clear();
    if (new_tutor_email) new_tutor_email->Clear();
    if (new_tutor_password) new_tutor_password->Clear();

    // Reset Registration Lists
    if (choose_subjects) {
        for (size_t i = 0; i < choose_subjects->GetCount(); i++) 
            choose_subjects->Deselect(i);
    }
    if (choose_days) {
        for (size_t i = 0; i < choose_days->GetCount(); i++) 
            choose_days->Deselect(i);
    }
 
    if (student_home_menu) {
        student_home_menu->SetLabel("Student Home Page");
        student_home_menu->Refresh();
    }

    if (priority_select) priority_select->SetSelection(0);
    if (choose_request_subject) choose_request_subject->SetSelection(wxNOT_FOUND);
    if (request_description) request_description->Clear();
    if (choose_sorting_criteria) choose_sorting_criteria->SetSelection(0);

    // Clear Day Checkboxes
    // (std::vector is safe to loop even if empty, but check the pointers inside)
    for (wxCheckBox* cb : day_checkboxes) {
        if (cb) cb->SetValue(false);
    }

    // Clear Lists
    if (tutor_results_list) tutor_results_list->Clear();
    if (student_active_requests_list) student_active_requests_list->Clear();
    if (student_history_requests_list) student_history_requests_list->Clear();

    // Clear Vectors (Safe to clear even if empty)
    current_search_results.clear();
    student_displayed_active_requests.clear();

    // Clear Account Inputs
    if (acc_name_input) acc_name_input->Clear();
    if (acc_email_input) acc_email_input->Clear();
    if (acc_pass_input) acc_pass_input->Clear();

    // Reset Navigation
    if (tutor_select) tutor_select->ChangeSelection(0); 
    if (student_home_pages) student_home_pages->SetSelection(0);


    if (tutor_home_menu) {
        tutor_home_menu->SetLabel("Tutor Dashboard");
        tutor_home_menu->Refresh();
    }

    // Clear Lists
    if (tutor_inbox_list) tutor_inbox_list->Clear();
    if (tutor_active_list) tutor_active_list->Clear();
    
    // You had 'tutor_requests' in your .h file, but replaced it with inbox_list
    // Check it just in case to prevent crash
    if (tutor_requests) tutor_requests->Clear(); 

    displayed_tutor_inbox.clear();
    displayed_tutor_active.clear();

    // Clear Tutor Account Inputs
    if (tutor_update_name) tutor_update_name->Clear();
    if (tutor_update_email) tutor_update_email->Clear();
    if (tutor_update_password) tutor_update_password->Clear();

    // Clear Redundant Inputs (If you still have them)
    if (tutor_name_input) tutor_name_input->Clear();
    if (tutor_email_input) tutor_email_input->Clear();
    if (tutor_pass_input) tutor_pass_input->Clear();
	if(tutor_completion_rate) tutor_completion_rate->SetLabel(" ");
	if(tutor_completions) tutor_completion_rate->SetLabel(" ");
	if(tutor_avg_rating) tutor_completion_rate->SetLabel(" ");
	if(tutor_matched) tutor_completion_rate->SetLabel(" ");


    // Deselect Settings
    if (tutor_update_subjects) {
        for (size_t i = 0; i < tutor_update_subjects->GetCount(); i++) 
            tutor_update_subjects->Deselect(i);
    }
    if (tutor_update_days) {
        for (size_t i = 0; i < tutor_update_days->GetCount(); i++) 
            tutor_update_days->Deselect(i);
    }

    if (tutor_home_pages) tutor_home_pages->SetSelection(0);



    
    if (login_switch) login_switch->ChangeSelection(0);

    wxMessageBox("You have been logged out successfully.", "Logout", wxOK | wxICON_INFORMATION);
}


//Login Functions
void LoginFrame::enter_student(wxCommandEvent& evt) {
	std::string username = student_username->GetValue().ToStdString();
	std::string password = student_password->GetValue().ToStdString();
	if (connection_system->student_login(username, password))
	{
 		Student* s = connection_system->get_student(student_username->GetValue().ToStdString());    
		populate_student_account_details(s);
        if (s != nullptr) {
			student_refresh_request_lists();
            student_home_menu->SetLabel(s->get_name());
            // Force a refresh so the text re-centers/updates immediately
            student_home_menu->Refresh(); 
        }
		wxMessageBox("Login Successful!", "Success", wxOK | wxICON_INFORMATION);
		login_switch->ChangeSelection(4);
	}
	else if(connection_system->get_student(username)==nullptr){
		wxMessageBox("No Account With Those Credentials.", "Login Failed", wxOK | wxICON_ERROR);

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
		refresh_tutor_ui();
		Tutor* t = connection_system->get_tutor(tutor_username->GetValue().ToStdString());
		populate_tutor_account_details(t);
		if(t != nullptr){
			tutor_home_menu->SetLabel(t->get_name());
			tutor_home_menu->Refresh();
		}
		wxMessageBox("Login Successful!", "Success", wxOK | wxICON_INFORMATION);
		login_switch->ChangeSelection(5);
	}
	else if(connection_system->get_tutor(username)==nullptr){
		wxMessageBox("No Account With Those Credentials.", "Login Failed", wxOK | wxICON_ERROR);

	}
	else
	{
		wxMessageBox("Invalid Username or Password.", "Login Failed", wxOK | wxICON_ERROR);
	}
}

//Navigate Menu Functions
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

//Registration Functions
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
    else if(!(connection_system->get_student(new_student_email->GetValue().ToStdString())==nullptr)){
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
    else if(!(connection_system->get_tutor(new_tutor_email->GetValue().ToStdString())==nullptr)){
        wxMessageBox("An Account With This Email Already Exists ", "Failed Account Creation", wxOK | wxICON_INFORMATION);
    }
    else{
        std::vector<std::string> final_subjects;
        std::vector<bool> final_days(7,false);

		std::string email = new_tutor_email->GetValue().ToStdString();
		std::string name = new_tutor_name->GetValue().ToStdString();
    	std::string pass = new_tutor_password->GetValue().ToStdString();

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
        connection_system->register_tutor(name, email, pass, final_days, final_subjects);
		
        wxMessageBox("Account Creation Successful", "Account Created", wxOK | wxICON_INFORMATION);
    }    
}

//Account Detail Updates
void LoginFrame::on_student_update_account(wxCommandEvent& evt){
	std::string current_email = student_username->GetValue().ToStdString();

	std::string new_name = acc_name_input->GetValue().ToStdString();
    std::string new_email = acc_email_input->GetValue().ToStdString();
    std::string new_pass = acc_pass_input->GetValue().ToStdString();

	if(new_name.empty()|| new_email.empty()||new_pass.empty()){
		wxMessageBox("All fields are required.", "Error", wxOK | wxICON_WARNING);
        return;	
	}

	bool success = connection_system-> update_student_details(current_email,new_name,new_email,new_pass);

	if(success){
		wxMessageBox("Account details updated successfully!", "Success");
		
		if(current_email!=new_email){
			student_username->SetValue(new_email);
			student_password->SetValue(new_pass);
		}
		student_home_menu->SetLabel(new_name);
		student_home_menu->Refresh();
	}
	else{
		wxMessageBox("Update failed. Email may already be in use.", "Error", wxOK | wxICON_ERROR);
	}

}
void LoginFrame::on_save_tutor_account(wxCommandEvent& evt){
	std::string current_email = tutor_username->GetValue().ToStdString();

	std::string new_name = tutor_update_name->GetValue().ToStdString();
    std::string new_email = tutor_update_email->GetValue().ToStdString();
    std::string new_pass = tutor_update_password->GetValue().ToStdString();

	std::vector<std::string> new_subjects;
	wxArrayInt subject_selections;
	tutor_update_subjects->GetSelections(subject_selections);
	//Get new Subjects
	for(size_t i = 0; i< subject_selections.GetCount();i++){
		new_subjects.push_back(tutor_update_subjects->GetString(subject_selections[i]).ToStdString());
    }
	//Get day selections
	std::vector<bool> new_days(7,false);
		wxArrayInt day_selections;
		tutor_update_days->GetSelections(day_selections);

	for(size_t i=0; i<day_selections.GetCount();i++){
		int index = day_selections[i];
		if(index>= 0 && index<7){
			new_days[index]=true;
		}
	}

	Tutor* t = connection_system->get_tutor(current_email);
	if(t){
		connection_system->update_tutor_details(current_email,new_name,new_email,new_pass,new_subjects,new_days);
		wxMessageBox("Account Updated!");
	}
}

//Retrieve Account Details
void LoginFrame::populate_student_account_details(Student* s) {
	if(!s) return;
	acc_name_input->SetValue(s->get_name());
	acc_email_input->SetValue(s->get_email());
    acc_pass_input->SetValue(s->get_password());	
}
void LoginFrame::populate_tutor_account_details(Tutor* t){
	if(!t) return;
	tutor_update_name->SetValue(t->get_name());
	tutor_update_email->SetValue(t->get_email());
    tutor_update_password->SetValue(t->get_password());
	tutor_completion_rate->SetLabel(std::to_string(t->avg_completion()));
	tutor_completions->SetLabel(std::to_string(t->get_completed()));
	tutor_avg_rating->SetLabel(wxString::Format("%.2f", t->avg_rating()));
	tutor_matched->SetLabel(std::to_string(t->get_matched()));

	tutor_update_subjects->DeselectAll();
	tutor_update_days->DeselectAll();
	
	
	std::vector<std::string> subjects = t->get_subjects();
	for(std::string s:subjects){
		tutor_update_subjects->SetStringSelection(s);
	}
	std::vector<bool> days = t->get_days();
	for(size_t i = 0; i < days.size(); ++i){
		if(days[i]){
			tutor_update_days->SetSelection(i);
		}
	}

}

//Student Request Help Functions
void LoginFrame::on_find_tutors(wxCommandEvent& evt) {

	// dispay a list of possible tutors, submit call on send request 
	//look at list of select tutor, and creates a request and send to each avaible tutor 
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

	std::string sort_mode = choose_sorting_criteria->GetStringSelection().ToStdString();
	std::string subject = choose_request_subject->GetStringSelection().ToStdString();
	std::vector<bool> requireddays(7,false);

	for(size_t i = 0;i<day_checkboxes.size();i++){
		if(day_checkboxes[i]->GetValue()){
			requireddays[i]=true;
		}
	}
	current_search_results = connection_system->get_tutors_for_subject(subject,sort_mode,requireddays);
	if(current_search_results.empty()){
		wxMessageBox("No tutors found for that subject and schedule.", "No Matches");
		return;
	}
	if (tutor_results_list == nullptr) {
    wxMessageBox("Crash Avoided: tutor_results_list is NULL. Check Constructor!");
    return;
	}
	else{	
	tutor_results_list->Clear();
	
	for(Tutor* t : current_search_results){
		std::string label = "Name: " + t->get_name() + " | Rating: " +
		std::to_string(t->avg_rating()).substr(0,3) + " | Total Completed Sessions: " +
		std::to_string(t->get_completed()) + " | Completion Rate: " +
		std::to_string(t->avg_completion()).substr(0,3);
		tutor_results_list->Append(label);
	}
	
	tutor_select->ChangeSelection(1);
	}
	}
}
void LoginFrame::on_send_requests(wxCommandEvent& evt){
		std::vector<Tutor*> selected_tutors;

		for(size_t i = 0; i< tutor_results_list->GetCount();i++){
			if(tutor_results_list->IsChecked(i)){
				if(i<current_search_results.size()){
					selected_tutors.push_back(current_search_results[i]);
				}
			}
		}
		if(selected_tutors.empty()){
		wxMessageBox("Please select at least one tutor to send a request to.", "No Selection", wxOK | wxICON_WARNING);
        return;			
		}

		std::string subject = choose_request_subject->GetStringSelection().ToStdString();
		int priorityIndex = priority_select->GetSelection();
		Request::UrgencyLevel urgency;
		if(priority_select->GetSelection() == 0 ) urgency = Request::LOW;
		if(priority_select->GetSelection() == 1 ) urgency = Request::MEDIUM;
		if(priority_select->GetSelection() == 2 ) urgency = Request::HIGH;

		std::vector<bool> required_days(7,false);
		for (size_t i = 0; i < day_checkboxes.size(); i++) {
        if (day_checkboxes[i]->GetValue()) {
            required_days[i] = true;
        }
    	}
		std::string description = request_description->GetValue().ToStdString();
		std::string email = student_username->GetValue().ToStdString();
		Student* current_student = connection_system->get_student(email);

		if(current_student==nullptr){
		wxMessageBox("Error: Could not identify current student session.", "System Error", wxOK | wxICON_ERROR);
        return;
		}

		connection_system->send_requests(current_student,selected_tutors,urgency,description,subject,required_days);

		wxMessageBox("Request sent successfully to " + std::to_string(selected_tutors.size()) + " tutor(s)!", "Success", wxOK | wxICON_INFORMATION);

		tutor_results_list->Clear();
		current_search_results.clear();
		tutor_select->ChangeSelection(0);


}

//Student Request Management
void LoginFrame::student_refresh_request_lists(){
	student_active_requests_list->Clear();
	student_history_requests_list->Clear();
	student_displayed_active_requests.clear();

	std::string email = student_username->GetValue().ToStdString();
	Student* s = connection_system->get_student(email);

	if(!s) return;

	std::vector<Request*> unrated_requests;
	const std::vector<Request*>& active_req = s->get_active_requests();

	for(Request* r: active_req){
		std::string label = "Subject: " + r->get_subject()+" | Status: "+ (r->get_status() == Request::MATCHED ? "Matched" : "Posted") + " | ";

		if(r->get_status()==Request::MATCHED && r->get_tutor() != nullptr){
			label += " Tutor: " + r->get_tutor()->get_name();
		}

		student_active_requests_list->Append(label);
		student_displayed_active_requests.push_back(r);

		if(r->get_status()==Request::COMPLETED){
			unrated_requests.push_back(r);
		}
	}

	//get historical requests
	const std::vector<Request*>& history = s->get_previous_requests();

	int count = history.size();
	int start = (count>10) ? count-10:0;

	for(int i = start; i<count;i++){
		Request* r = history[i];
		std::string label = "Subject: " + r->get_subject()+" | Status: Completed | Tutor: " + r->get_tutor()->get_name();
		student_history_requests_list->Append(label);
	}
	
	if(!unrated_requests.empty()){
		wxMessageBox("You have " + std::to_string(unrated_requests.size()) + 
                     " completed session(s) to rate.", "Feedback Required");

		for(Request* req: unrated_requests){
			std::string tutor_name = req->get_tutor()->get_name();
			RatingDialog dlg(this, tutor_name);
			if(dlg.ShowModal() == wxID_OK){
				int rating = dlg.GetRating();

				req->get_tutor()->update_ratings(rating);
			}
		}
	}

}
void LoginFrame::student_on_mark_complete(wxCommandEvent& evt){
		int sel = student_active_requests_list->GetSelection();
		if(sel == wxNOT_FOUND){
			wxMessageBox("Please select an active request first.");
        	return;
		}

		Request* req = student_displayed_active_requests[sel];
		
		if(req->get_status() != Request::MATCHED){
			wxMessageBox("You can only complete requests that have been accepted by a tutor.", "Error");
        	return;			
		}

		RatingDialog dlg(this, req->get_tutor()->get_name());

	if(dlg.ShowModal()== wxID_OK){
		int rating = dlg.GetRating();
		std::string email = student_username->GetValue().ToStdString();
		Student* s =connection_system->get_student(email);
		s->close_request(req,rating);
		connection_system->complete_request(req);

		wxMessageBox("Session completed and rating submitted!");
        	student_refresh_request_lists(); // Update UI
	}

}
void LoginFrame::student_on_refresh_click(wxCommandEvent& evt){
	student_refresh_request_lists();
}

//Tutor Request Management
void LoginFrame::refresh_tutor_ui(){
	tutor_inbox_list->Clear();
	tutor_active_list->Clear();
	displayed_tutor_inbox.clear();
	displayed_tutor_active.clear();

	std::string email = tutor_username->GetValue().ToStdString();
	Tutor* t = connection_system->get_tutor(email);
	if(!t) return;

	//Get request inbox to display
	std::vector<Request*> inbox = t->get_valid_inbox();
	for(Request* r : inbox){
		std::string urgency_str;
		std::vector<bool> req_days = r->get_days();
		std::string days = "";
		int commas = ((req_days.size())-1);
		int index = 0;


        switch (r->get_urgency()) {
            case Request::HIGH:   urgency_str = "High";   break;
            case Request::MEDIUM: urgency_str = "Medium"; break;
            case Request::LOW:    urgency_str = "Low";    break;
            default:              urgency_str = "Unknown"; break;
        }
		for(bool b : req_days){
			if(b){
				switch(index){
				case 0: days.append("Sun"); break;
				case 1: days.append("Mon"); break;
				case 2: days.append("Tue"); break;
				case 3: days.append("Wed"); break;
				case 4: days.append("Thu"); break;
				case 5: days.append("Fri"); break;
				case 6: days.append("Sat"); break;			
			}
				if(commas>0){
					days.append(", ");
					commas--;
				}
			}
			++index;
		}
		tutor_inbox_list->Append("Student Name: " + r->get_student()->get_name() + " | Subject:  " +
			r->get_subject() + " | Urgency: " + urgency_str + " | Status: Posted" + " | Days: " +  days);
		displayed_tutor_inbox.push_back(r);
	}
	//Get active requests to display
	const std::vector<Request*>& active = t->get_active_requests();

	for(Request* r: active){
		std::string urgency_str;
		std::vector<bool> req_days = r->get_days();
		std::string days = "";
		int commas = req_days.size()-1;
		int index = 0;

        switch (r->get_urgency()) {
            case Request::HIGH:   urgency_str = "High";   break;
            case Request::MEDIUM: urgency_str = "Medium"; break;
            case Request::LOW:    urgency_str = "Low";    break;
            default:              urgency_str = "Unknown"; break;
        }
		for(bool b : req_days){
			if(b){
				switch(index){
				case 0: days.append("Sun"); break;
				case 1: days.append("Mon"); break;
				case 2: days.append("Tue"); break;
				case 3: days.append("Wed"); break;
				case 4: days.append("Thu"); break;
				case 5: days.append("Fri"); break;
				case 6: days.append("Sat"); break;						
			}
				if(commas>0){
					days.append(", ");
					commas--;
				}
			}
			++index;
		}
		tutor_active_list->Append("Student Name: " + r->get_student()->get_name() + " | Subject:  " +
			r->get_subject() + " | Urgency: " + urgency_str + " | Status: Matched" + " | Days: " +  days);
		displayed_tutor_active.push_back(r);

	}
}
void LoginFrame::on_tutor_accept_request(wxCommandEvent& evt){
	int selection = tutor_inbox_list->GetSelection();
	if(selection == wxNOT_FOUND)return;

	Request* req = displayed_tutor_inbox[selection];

	std::string email = tutor_username->GetValue().ToStdString();
	Tutor*t = connection_system->get_tutor(email);

	if(connection_system->accept_request(t, req)){
		wxMessageBox("Request Accepted!");
        refresh_tutor_ui(); // Move from Inbox to Active UI
	}
	else{
		wxMessageBox("This request is no longer available.", "Error");
        refresh_tutor_ui();
	}
}
void LoginFrame::on_tutor_close_request(wxCommandEvent& evt){
	int selection = tutor_active_list->GetSelection();
	if(selection == wxNOT_FOUND) return;
	
Request* req = displayed_tutor_active[selection];
	req-> update_status(Request::COMPLETED);
	connection_system->complete_request(req);
	//---------------------------------------------------------------------------------------------------------------------add history vector logic
	wxMessageBox("Session Marked as Complete.");
    refresh_tutor_ui();
}
void LoginFrame::on_request_double_click(wxCommandEvent& evt){
	wxListBox* source = dynamic_cast<wxListBox*>(evt.GetEventObject());
	if(!source) return;

	int selection = source->GetSelection();
	if(selection == wxNOT_FOUND) return;

	Request* req = nullptr;

	if (source == tutor_inbox_list) {
        if (selection < displayed_tutor_inbox.size()) 
            req = displayed_tutor_inbox[selection];
    }
    else if (source == tutor_active_list) {
        if (selection < displayed_tutor_active.size()) 
            req = displayed_tutor_active[selection];
    }
    else if (source == student_active_requests_list) {
        if (selection < student_displayed_active_requests.size()) 
            req = student_displayed_active_requests[selection];
    }

	if(req){
		std::string title = "Request Details: " + req->get_subject();
		std::string content = "";
		content+= "Student: " + req->get_student()->get_name();
		content+="\nUrgency: " + std::string(req->get_urgency() == Request::HIGH ? "High" : 
                                            (req->get_urgency() == Request::MEDIUM ? "Medium" : "Low")) + "\n";
		content += "-----------------------------------\n";
        content += "Description:\n";
        content += req->get_description(); // Assuming you added this getter!
        // Display the message box
        wxMessageBox(content, title, wxOK | wxICON_INFORMATION);
	}
}
