cmake_minimum_required(VERSION 3.10)

project(SimpleGUI)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(wxWidgets CONFIG REQUIRED)

add_executable(my_gui_app WIN32 
    "Gui.cpp"
    "LoginFrame.cpp" 
    "LoginFrame.h"
    "EduConnectSystem.cpp"  # <--- MAKE SURE THIS IS HERE!
    "EduConnectSystem.h"    
    "Tutor.cpp"
    "Student.cpp"
    "Request.cpp"
    "Algorithms.h"
    "User.cpp"
    "User.h"
)

target_link_libraries(my_gui_app PRIVATE wx::core wx::base)