//mainWindow.h
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <wx/wx.h>


// Define the primary window and the commands the menus will execute
class MainWindow : public wxFrame {
    public: 
        MainWindow();
    private:
        wxListBox* pkgList;
        // Exits the program
        void OnExit(wxCommandEvent& event);
        // Lists all the installed packages
        void LoadInstalledPackages(wxCommandEvent& event);
        // Opens the about menu
        void OnAbout(wxCommandEvent& event);
        // Lists the packages that can be updated
        void ListUpgradeablePackages(wxCommandEvent& event);


};



#endif // MAINWINDOW_H
//mainWindow.h