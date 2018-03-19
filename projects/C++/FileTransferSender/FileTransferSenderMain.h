#ifndef FILETRANSFERSENDERMAIN_H
#define FILETRANSFERSENDERMAIN_H

#include "FileTransferSenderApp.h"
#include "GUIFrame.h"
#include <winsock.h>

class FileTransferSenderFrame: public GUIFrame
{
    DECLARE_EVENT_TABLE()

    public:
        FileTransferSenderFrame(wxFrame *, const wxString&, const wxSize&);
        ~FileTransferSenderFrame();

    private:
        void OnClose(wxCloseEvent&);
        void OnQuit(wxCommandEvent&);
        void OnHelp(wxCommandEvent&);
        void OnInfo(wxCommandEvent&);
        void OnSfoglia(wxCommandEvent&);
        void OnInvia(wxCommandEvent&);
        void OnAzzera(wxCommandEvent&);
        bool CheckIPHost(const wxString&);
        int  ShowMessage(const wxString&, const wxString& = wxT("Messaggio"), int = wxOK | wxICON_INFORMATION);

        wxPanel    * panel;
        wxBoxSizer * vbox;
        wxBoxSizer * hbox1;
        wxBoxSizer * hbox2;
        wxBoxSizer * hbox3;
        wxBoxSizer * hbox4;
        wxBoxSizer * hbox5;
        wxButton   * btnSfoglia;
        wxButton   * btnInvia;
        wxButton   * btnAzzera;
        wxStaticText * File;
        wxStaticText * FileName;
        wxStaticText * IpHost;
        wxTextCtrl   * IpHostText;
        wxGauge      * Gauge;
        wxTextCtrl   * Log;

        wxString FileToSend;
};

#endif // FILETRANSFERSENDERMAIN_H
