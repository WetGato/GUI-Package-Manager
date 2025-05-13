#include <wx/wx.h>

class Popups : public wxFrame {
    public:
        Popups(const wxString& title);
        void About(wxCommandEvent& event);
};


