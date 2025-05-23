// menu.cpp
#include <wx/wx.h>

#include "include/menu.h"
#include "include/mainWindow.h"

wxMenuBar *MenuBuilder::CreateMenuBar(wxFrame *parent)
{
    wxMenuBar *menuBar = new wxMenuBar();

    // defining the menus
    wxMenu *fileMenu = new wxMenu();
    wxMenu *managePackageMenu = new wxMenu();
    wxMenu *queryManagePackageMenu = new wxMenu();
    wxMenu *updatePackageMenu = new wxMenu();
    wxMenu *helpMenu = new wxMenu();

    menuBar->Append(fileMenu, "&File");
        fileMenu->Append(wxID_EXIT, "&Exit\tAlt-X", "Quit this program");

    menuBar->Append(managePackageMenu, "Manage");
        managePackageMenu->AppendSubMenu(queryManagePackageMenu, "Query");
            queryManagePackageMenu->Append(ID_QueryPackages, "List installed\tCtrl-L", "List all installed packages");
        managePackageMenu->AppendSubMenu(updatePackageMenu, "Update");
            updatePackageMenu->Append(ID_ListUpgradeable, "List Upgradeable", "List the upgradeable packages");
            updatePackageMenu->Append(ID_UPDATE, "Update all packages", "Update all packages with available updates");

    menuBar->Append(helpMenu, "&Help");
        helpMenu->Append(ID_ABOUT, "About", "About this program");

    return menuBar;
}