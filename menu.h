#ifndef MENU_H
#define MENU_H
#include <wx/wx.h>
#include <wx/menu.h>

class MyFrame : public wxFrame {
public:
    MyFrame();
    void OnQuit(wxCommandEvent& event);
    wxMenuBar *menuBar;
    wxMenu *file;
    wxMenu *manage;
    wxMenu *help;
private:
    wxTextCtrl* output;

    void OnListInstalled(wxCommandEvent&);
    void UpdateAll(wxCommandEvent&);
    std::string RunCommand(const std::string& cmd);
};


const int ID_About = 1;
const int ID_List = 2;
const int ID_UpdateAll = 3;

#endif // MENU_H
// menu.h