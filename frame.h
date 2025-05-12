#ifndef FRAME_H
#define FRAME_H
#include <wx/wx.h>
#include <wx/menu.h>

class MyFrame : public wxFrame {
public:
    MyFrame();
    void OnQuit(wxCommandEvent& event);
    wxMenuBar *menuBar;
    wxMenu *file;
private:
    wxTextCtrl* output;

    void OnListInstalled(wxCommandEvent&);
    std::string RunCommand(const std::string& cmd);
};

#endif // FRAME_H
// frame.h