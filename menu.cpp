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

    fileMenu->Append(wxID_EXIT, "&Exit\tAlt-X", "Quit this program");

    menuBar->Append(fileMenu, "&File");
    menuBar->Append(managePackageMenu, "Manage");

    // The query package menu
    queryManagePackageMenu->Append(ID_QueryPackages, "List installed\tCtrl-L", "List all installed packages");

    // The manage package menu
    managePackageMenu->AppendSubMenu(queryManagePackageMenu, "Query");

    /* Binding the buttons to the functions that handle the functions of the button*/

    if (MainWindow* win = dynamic_cast<MainWindow*>(parent)) {
        win->Bind(wxEVT_MENU, &MainWindow::OnExit, win, wxID_EXIT);
        win->Bind(wxEVT_MENU, &MainWindow::LoadInstalledPackages, win, ID_QueryPackages);
    }


    return menuBar;
}