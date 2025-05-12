#include <wx/wx.h>
#include <string>
#include <cstdlib>
#include <sstream>
#include <memory>
#include <wx/menu.h>


#include "frame.h"
#include <wx/event.h>


MyFrame::MyFrame() : wxFrame(nullptr, wxID_ANY, "Pacman GUI", wxDefaultPosition, wxSize(600, 400)) {
    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
        output = new wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition, wxDefaultSize,
                                 wxTE_MULTILINE | wxTE_READONLY);
    wxButton* listInstalledBtn = new wxButton(this, wxID_ANY, "List Installed Packages");
    sizer->Add(listInstalledBtn, 0, wxALL | wxEXPAND, 5);
    sizer->Add(output, 1, wxALL | wxEXPAND, 5);

    SetSizer(sizer);

    listInstalledBtn->Bind(wxEVT_BUTTON, &MyFrame::OnListInstalled, this);


    menuBar = new wxMenuBar();
    file = new wxMenu();
    file->Append(wxID_EXIT, wxT("&Quit"));
    menuBar->Append(file, wxT("&File"));
    SetMenuBar(menuBar);

    Connect(wxID_EXIT, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MyFrame::OnQuit));
    Centre();
    }

void MyFrame::OnListInstalled(wxCommandEvent&) {
        std::string result = RunCommand("pacman -Q");
        output->SetValue(result);
    }

    void MyFrame::OnQuit(wxCommandEvent& WXUNUSED(event))
{
  Close(true);
}

std::string MyFrame::RunCommand(const std::string& cmd) {
    std::string data;
    FILE* stream = popen(cmd.c_str(), "r");
    if (!stream) return "Failed to run command.";

    char buffer[128];
    while (fgets(buffer, sizeof(buffer), stream)) {
        data.append(buffer);
    }
    pclose(stream);
    return data;
}
