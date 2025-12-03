#pragma once
#include <wx/wx.h>
#include <wx/simplebook.h>
#include <string>
#include <vector>
#include "Tutor.h"

enum
{
	to_student_button_id = 2,
	to_tutor_button_id = 3
};
class EduConnectSystem;

class LoginFrame : public wxFrame
{
public:
	LoginFrame(const wxString& title,EduConnectSystem* system);

private:
    EduConnectSystem* connection_system;

	void perform_logout(wxCommandEvent& evt);
	void enter_student(wxCommandEvent& evt);
	void enter_tutor(wxCommandEvent& evt);
	void forgor_button(wxCommandEvent& evt);
	void to_student_register(wxCommandEvent& evt);
	void to_tutor_register(wxCommandEvent& evt);

	bool authenticate_student(std::string user, std::string pass);
	bool authenticate_tutor(std::string user, std::string pass);


	void to_tutor(wxCommandEvent& evt);
	void to_student(wxCommandEvent& evt);

	void create_student(wxCommandEvent& evt);
	void create_tutor(wxCommandEvent& evt);
	void create_request(wxCommandEvent& evt);
	
    void on_cancel_request(wxCommandEvent& evt);
    // Update your existing search handler to switch pages instead of popping up
    void on_find_tutors(wxCommandEvent& evt);
	void on_sort_change(wxCommandEvent& evt);
	void on_send_requests(wxCommandEvent& evt);

	//Student: View requests page
	void student_refresh_request_lists(); // Refills the listboxes
    void student_on_mark_complete(wxCommandEvent& evt);
    void student_on_refresh_click(wxCommandEvent& evt);
	void on_request_double_click(wxCommandEvent& evt);
    
    //Student update account
    void on_student_update_account(wxCommandEvent& evt);
	void populate_student_account_details(Student* s);


	//Tutor Related Functions
	
		//Helper Functions
			void refresh_tutor_ui();
    		void populate_tutor_account_details(Tutor* t);

		//Request Management
			void on_tutor_accept_request(wxCommandEvent& evt);
    		void on_tutor_close_request(wxCommandEvent& evt);
		
		//Account Management
			void on_save_tutor_account(wxCommandEvent& evt);


	//GUI Navigation elements
	wxSimplebook *login_switch = nullptr;
	wxSimplebook *tutor_select = nullptr;
		//Notebook variable to tutor and student display pages
		wxNotebook* student_home_pages = nullptr;
		wxNotebook* tutor_home_pages = nullptr;
		//Variable to display individual student name
		wxStaticText* student_home_menu = nullptr;
		wxStaticText* tutor_home_menu = nullptr;

	//usernames and passwords
		wxTextCtrl* student_password = nullptr;
		wxTextCtrl* student_username = nullptr;
		wxTextCtrl* tutor_password = nullptr;
		wxTextCtrl* tutor_username = nullptr;

	//Account creation variables
		wxTextCtrl* new_student_name = nullptr;
		wxTextCtrl* new_student_email = nullptr;
		wxTextCtrl* new_student_password = nullptr;
		wxTextCtrl* new_tutor_name = nullptr;
		wxTextCtrl* new_tutor_email = nullptr;
		wxTextCtrl* new_tutor_password = nullptr;
   		wxListBox* choose_subjects = nullptr;
    	wxListBox* choose_days = nullptr;
	

	//Student menu
	//Create Request Parameters
		wxRadioBox* priority_select = nullptr;
		wxChoice* choose_request_subject = nullptr;
		wxTextCtrl* request_description = nullptr;
    	std::vector<wxCheckBox*> day_checkboxes;
		wxChoice* choose_sorting_criteria = nullptr;
		//List of matched tutors
		std::vector<Tutor*> current_search_results;

		//UI elements for view requests tab
			wxCheckListBox* tutor_results_list = nullptr;
			wxListBox* student_active_requests_list = nullptr;
   	 		wxListBox* student_history_requests_list = nullptr;
    		wxButton* student_mark_complete_btn = nullptr;
    		wxButton* student_request_refresh_btn = nullptr;
			std::vector<Request*> student_displayed_active_requests;

		//UI elements for student account tab
			wxTextCtrl* acc_name_input = nullptr;
    		wxTextCtrl* acc_email_input = nullptr;
			wxTextCtrl* acc_pass_input = nullptr;
		

	//Tutor Menu
		//Tutor Request Management
   			wxListBox* tutor_inbox_list = nullptr;   // Potential requests
    		wxListBox* tutor_active_list = nullptr;  // Accepted requests        
    		std::vector<Request*> displayed_tutor_inbox;
    		std::vector<Request*> displayed_tutor_active;

		//Tutor Account Details Section
			wxTextCtrl* tutor_update_name = nullptr;
			wxTextCtrl* tutor_update_email = nullptr;
			wxTextCtrl* tutor_update_password = nullptr;
			wxListBox* tutor_update_subjects = nullptr;
    		wxListBox* tutor_update_days = nullptr;
		
		//UI elements for Tutor account tab
			wxTextCtrl* tutor_name_input = nullptr;
    		wxTextCtrl* tutor_email_input = nullptr;
			wxTextCtrl* tutor_pass_input = nullptr;
			wxStaticText* tutor_completion_rate = nullptr;
			wxStaticText* tutor_completions = nullptr;
			wxStaticText* tutor_avg_rating = nullptr;
			wxStaticText* tutor_matched = nullptr;

	//tutor list variable
	wxListBox* tutor_requests = nullptr;


};