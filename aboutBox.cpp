// aboutBox.cpp
//#include <wx/wx.h>

#include "include/aboutBox.h"
#include "version.h.in"

AboutBox::AboutBox(wxWindow* parent) : wxDialog(parent, wxID_ANY, "About", 
    wxDefaultPosition, wxSize(300, 200)) {
    wxBoxSizer* contentSizer = new wxBoxSizer(wxVERTICAL);

    wxStaticText* content = new wxStaticText(this, wxID_ANY,
        "This is a simple application to manage packages on Arch Linux",
        wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER);
    contentSizer->Add(content, 0, wxALIGN_CENTER | wxALL, 10);
    wxString versionText = wxString::Format("%s\nVersion %s", APP_NAME, APP_VERSION);
    wxStaticText* version = new wxStaticText(this, wxID_ANY, versionText, wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER);
    contentSizer->Add(version, 0, wxALIGN_CENTER | wxALL, 10);
    wxButton* okButton = new wxButton(this, wxID_OK, "OK");
    contentSizer->Add(okButton, 0, wxALL | wxALIGN_CENTER, 10);

    SetSizerAndFit(contentSizer);

}