#ifndef FRAME_H
#define FRAME_H
#include <wx/wx.h>

class MyFrame : public wxFrame {
public:
    MyFrame();
private:
    wxTextCtrl* output;

    void OnListInstalled(wxCommandEvent&);
    std::string RunCommand(const std::string& cmd);
};

#endif // FRAME_H
// frame.h