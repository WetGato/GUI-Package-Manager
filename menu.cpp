#include <wx/wx.h>
#include <string>
#include <cstdlib>
#include <sstream>
#include <memory>
#include <wx/menu.h>


#include "menu.h"
#include "popups.h"
#include <wx/event.h>


MyFrame::MyFrame() : wxFrame(nullptr, wxID_ANY, "ArchPkgUI", wxDefaultPosition, wxSize(600, 400)) {
    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
    output = new wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE | wxTE_READONLY);
    sizer->Add(output, 1, wxALL | wxEXPAND, 5);
    SetSizer(sizer);


    // Create the MenuBar
    menuBar = new wxMenuBar();

    // File Menu
    file = new wxMenu();
    file->Append(wxID_EXIT, wxT("&Quit"));
    menuBar->Append(file, wxT("&File"));

    // Manage Menu
    manage = new wxMenu();

    // Install/Remove Submenu
    wxMenu* installMenu = new wxMenu();

    /*  future work
    installMenu->Append(wxID_ANY, wxT("&Install"), "Install a package");
    installMenu->Append(wxID_ANY, wxT("&Remove"), "Remove a package");
    installMenu->Append(wxID_ANY, wxT("&Update"), "Update a package");*/
    installMenu->Append(ID_UpdateAll, wxT("&Upgrade"), "Upgrade all packages");
    Connect(ID_UpdateAll, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MyFrame::OnUpgradeAll));
    manage->AppendSubMenu(installMenu, wxT("&Install/Remove"));
    
    // Query Submenu
    wxMenu* queryMenu = new wxMenu();
    /* 
    queryMenu->Append(wxID_ANY, wxT("&Search"), "Search for a package");
    queryMenu->Append(wxID_ANY, wxT("&Info"), "Get information about a package"); */
    queryMenu->Append(wxID_ANY, wxT("&List"), "List all packages");
    Connect(ID_List, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MyFrame::OnListInstalled));
    manage->AppendSubMenu(queryMenu, wxT("&Query"));

    // Maintenance Submenu
    wxMenu* maintenanceMenu = new wxMenu();
/*     maintenanceMenu->Append(wxID_ANY, wxT("&Check"), "Check for package updates");
    maintenanceMenu->Append(wxID_ANY, wxT("&Sync"), "Synchronize package databases");
    maintenanceMenu->Append(wxID_ANY, wxT("&Clean"), "Clean up package cache");
    maintenanceMenu->Append(wxID_ANY, wxT("&Verify"), "Verify package integrity");
    manage->AppendSubMenu(maintenanceMenu, wxT("&Maintenance")); */

    menuBar->Append(manage, wxT("&Manage"));

    // About Menu
    help = new wxMenu();
    help->Append(ID_About, wxT("&About"), "About this application");
    Connect(ID_About, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(Popups::About));
    menuBar->Append(help, wxT("&Help"));


    // Set the MenuBar
    SetMenuBar(menuBar);

    Connect(wxID_EXIT, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MyFrame::OnQuit));
    Centre();
    }
    
    void MyFrame::OnUpgradeAll(wxCommandEvent& event) {
        RunAndDisplayCommand("sudo pacman -Sy --noconfirm archlinux-keyring && sudo pacman -Su --noconfirm");
    }



