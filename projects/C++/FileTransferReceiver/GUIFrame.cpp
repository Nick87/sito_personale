#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include "GUIFrame.h"

BEGIN_EVENT_TABLE( GUIFrame, wxFrame )
    EVT_CLOSE( GUIFrame::_wxFB_OnClose )
    EVT_MENU( idMenuQuit, GUIFrame::_wxFB_OnQuit )
    EVT_MENU( idMenuHelp, GUIFrame::_wxFB_OnHelp )
    EVT_MENU( idMenuInfo, GUIFrame::_wxFB_OnInfo )
END_EVENT_TABLE()

GUIFrame::GUIFrame( wxWindow* parent, const wxString& title, const wxSize& minsize, int id, const wxPoint& pos, int style )
        : wxFrame( parent, id, title, pos, minsize, style )
{
    mbar  = new wxMenuBar( 0 );

    fileMenu = new wxMenu();
    menuFileQuit = new wxMenuItem( fileMenu, idMenuQuit, wxString( wxT("&Esci") ) + wxT("\tAlt+F4"), wxT("Esci dall'applicazione"));
    fileMenu->Append( menuFileQuit );
    mbar->Append( fileMenu, wxT("&File") );
    varieMenu = new wxMenu();
    menuInfo = new wxMenuItem( varieMenu, idMenuInfo, wxString( wxT("&Info") )  + wxT("\tF2"),  wxT("Informazioni sull'autore"));
    menuHelp = new wxMenuItem( varieMenu, idMenuHelp, wxString( wxT("&Aiuto") ) + wxT("\tF1"), wxT("Utilizzo dell'applicazionee"));
    varieMenu->Append( menuInfo );
    separator = varieMenu->AppendSeparator();
    varieMenu->Append( menuHelp );
    mbar->Append( varieMenu, wxT("&?") );
    this->SetMenuBar( mbar );

    this->SetSizeHints( wxDefaultSize, wxDefaultSize );
    SetMinSize(minsize);
    statusBar = this->CreateStatusBar( 2, wxST_SIZEGRIP, wxID_ANY );
    statusBar->SetStatusText(_("Pronto"), 0);
}
