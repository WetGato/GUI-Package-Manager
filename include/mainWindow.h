//mainWindow.h
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <wx/wx.h>

class MainWindow : public wxFrame {
    public: 
        MainWindow();
        void LoadInstalledPackages(wxCommandEvent& event);
        void OnExit(wxCommandEvent& event);
    private:
        //void OnExit(wxCommandEvent& event);
        wxListBox* pkgList;
};



#endif // MAINWINDOW_H
//mainWindow.h