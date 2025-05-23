#include <wx/wx.h>

#include "include/mainWindow.h"
#include "include/menu.h"

MainWindow::MainWindow() : wxFrame(nullptr, wxID_ANY, "ArchPkgUI", wxDefaultPosition, wxSize(600, 400))
{
    SetMenuBar(MenuBuilder::CreateMenuBar(this));
    //Bind(wxEVT_MENU, &MainWindow::OnExit, this, wxID_EXIT);

    // Making the output for the package list function, package search results 
    // function, package info function, etc.


    wxPanel *panel = new wxPanel(this);
    // pkgList = new wxListBox(panel, wxID_ANY, wxPoint(10,10), );

    // Making the size automatic. The static size tends to get cut off when 
    // windows is shrunk and is too small when the window is expanded 

    wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);
    pkgList = new wxListBox(panel, wxID_ANY);
    sizer->Add(pkgList, 1, wxEXPAND | wxALL, 10);
    panel->SetSizer(sizer);

    

    if (MainWindow* win = dynamic_cast<MainWindow*>(this)) {
    win->Bind(wxEVT_MENU, &MainWindow::OnExit, win, wxID_EXIT);
    win->Bind(wxEVT_MENU, &MainWindow::LoadInstalledPackages, win, ID_QueryPackages);
    win->Bind(wxEVT_MENU, &MainWindow::OnAbout, win, ID_ABOUT);
    win->Bind(wxEVT_MENU, &MainWindow::ListUpgradeablePackages, win, ID_ListUpgradeable);
    win->Bind(wxEVT_MENU, &MainWindow::UpdateAllPackages, win, ID_UPDATE);
    }
}

// Should this OnExit function be moved to commands.cpp

void MainWindow::OnExit(wxCommandEvent &event) {
    Close(true);
}