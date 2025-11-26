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

	//GUI Navigation elements
	wxSimplebook *login_switch;
	wxSimplebook *tutor_select;
		//Notebook variable to tutor and student display pages
		wxNotebook* student_home_pages;
		wxNotebook* tutor_home_pages;
		//Variable to display individual student name
		wxStaticText* student_home_menu;
		wxStaticText* tutor_home_menu;

	//usernames and passwords
		wxTextCtrl* student_password;
		wxTextCtrl* student_username;
		wxTextCtrl* tutor_password;
		wxTextCtrl* tutor_username;

	//Account creation variables
		wxTextCtrl* new_student_name;
		wxTextCtrl* new_student_email;
		wxTextCtrl* new_student_password;
		wxTextCtrl* new_tutor_name;
		wxTextCtrl* new_tutor_email;
		wxTextCtrl* new_tutor_password;
   		wxListBox* choose_subjects;
    	wxListBox* choose_days;
	

	//Student menu
	//Create Request Parameters
		wxRadioBox* priority_select;
		wxChoice* choose_request_subject;
		wxTextCtrl* request_description;
    	std::vector<wxCheckBox*> day_checkboxes;
		wxChoice* choose_sorting_criteria;
		//List of matched tutors
		std::vector<Tutor*> current_search_results;

		//UI elements for view requests tab
			wxCheckListBox* tutor_results_list;
			wxListBox* student_active_requests_list;
   	 		wxListBox* student_history_requests_list;
    		wxButton* student_mark_complete_btn;
    		wxButton* student_request_refresh_btn;
			std::vector<Request*> student_displayed_active_requests;
	//Tutor Selection Variable

	wxTextCtrl* tutor_update_name;
	wxTextCtrl* tutor_update_email;
	wxTextCtrl* tutor_update_password;
	//tutor list variable
	wxListBox* tutor_requests;


};