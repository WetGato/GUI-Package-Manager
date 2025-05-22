//menu.h
#ifndef MENU_H
#define MENU_H

#include <wx/wx.h>

namespace MenuBuilder {
    wxMenuBar* CreateMenuBar(wxFrame* parent);
}

const int ID_QueryPackages = 1;
const int ID_ABOUT = 2;
const int ID_ListUpgradeable = 3;


#endif //MENU_H
//menu.h