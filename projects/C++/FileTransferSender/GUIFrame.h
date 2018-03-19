#ifndef __GUIFrame__
#define __GUIFrame__

// Define WX_GCH in order to support precompiled headers with GCC compiler.
// You have to create the header "wx_pch.h" and include all files needed
// for compile your gui inside it.
// Then, compile it and place the file "wx_pch.h.gch" into the same
// directory that "wx_pch.h".
#ifdef WX_GCH
#include <wx_pch.h>
#else
#include <wx/wx.h>
#endif

#include <wx/menu.h>

enum
{
    idMenuQuit = 1000,
    idMenuHelp,
    idMenuInfo
};

class GUIFrame : public wxFrame
{
    DECLARE_EVENT_TABLE()

    public:
        GUIFrame( wxWindow* parent,
                  const wxString& title,
                  const wxSize& minsize,
                  int id = wxID_ANY,
                  const wxPoint& pos = wxDefaultPosition,
                  int style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );
    private:
        void _wxFB_OnClose( wxCloseEvent& event ){ OnClose( event ); }
        void _wxFB_OnQuit( wxCommandEvent& event ){ OnQuit( event ); }
        void _wxFB_OnHelp( wxCommandEvent& event ){ OnHelp( event ); }
        void _wxFB_OnInfo( wxCommandEvent& event ){ OnInfo( event ); }

    protected:
        wxMenuBar* mbar;
        wxStatusBar* statusBar;
        wxMenu* fileMenu;
        wxMenu* varieMenu;
        wxMenuItem* menuFileQuit;
        wxMenuItem* menuInfo;
        wxMenuItem* menuHelp;
        wxMenuItem* separator;

        // Virtual event handlers, overide them in your derived class
        virtual void OnClose( wxCloseEvent& event ){ event.Skip(); }
        virtual void OnQuit( wxCommandEvent& event ){ event.Skip(); }
        virtual void OnHelp( wxCommandEvent& event ){ event.Skip(); }
        virtual void OnInfo( wxCommandEvent& event ){ event.Skip(); }
};

#endif //__GUIFrame__
