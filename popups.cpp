#include "popups.h"
#include "wx/wx.h"

void Popups::About(wxCommandEvent& event) {
    wxMessageBox("This is a simple package manager for Arch Linux.\n"
                 "It allows you to install, remove, and manage packages.\n"
                 "Developed by [Your Name].",
                 "About ArchPkgUI", wxOK | wxICON_INFORMATION);
};
