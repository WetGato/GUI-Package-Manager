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

void MyFrame::RunAndDisplayCommand(const std::string& cmd) {
    output->Clear(); // Clear the output box first
    FILE* pipe = popen(cmd.c_str(), "r");
    if (!pipe) {
        output->AppendText("Failed to run command.\n");
        return;
    }

    char buffer[256];
    while (fgets(buffer, sizeof(buffer), pipe)) {
        output->AppendText(wxString::FromUTF8(buffer));
        wxYield(); // Allows GUI to update between lines
    }
    pclose(pipe);
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