// commands.cpp
#include <wx/wx.h>

#include "menu.h"


void MyFrame::OnListInstalled(wxCommandEvent&) {
        std::string result = RunCommand("pacman -Q");
        output->SetValue(result);
}

void MyFrame::UpdateAll(wxCommandEvent&) {
    std::string result = RunCommand("sudo pacman -Sy --noconfirm archlinux-keyring && sudo pacman -Su --noconfirm");
    output->SetValue(result);
}

void MyFrame::OnQuit(wxCommandEvent& WXUNUSED(event)) {
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