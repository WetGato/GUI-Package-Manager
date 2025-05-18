//main.cpp
#include <wx/wx.h>

#include "include/mainWindow.h"


class ArchPkgUI : public wxApp {
public:
    bool OnInit() override {
        (new MainWindow())->Show();
        return true;
    }
};

wxIMPLEMENT_APP(ArchPkgUI);