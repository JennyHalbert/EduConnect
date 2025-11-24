#pragma once
#include <wx/wx.h>
#include "EduConnectSystem.h"

class GUI : public wxApp
{
public:
	bool OnInit();

private:
	EduConnectSystem c_system;
};