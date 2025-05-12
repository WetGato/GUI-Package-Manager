// pacman_gui.cpp
#include <wx/wx.h>
#include <cstdlib>
#include <memory>
#include <string>
#include <sstream>

#include "frame.h"



class MyApp : public wxApp {
public:
    bool OnInit() override {
        (new MyFrame())->Show(); // Parentheses added around the new-expression
        return true;
    }
};

wxIMPLEMENT_APP(MyApp);
