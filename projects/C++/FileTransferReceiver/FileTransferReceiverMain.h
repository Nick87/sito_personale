#ifndef FILETRANSFERRECEIVERMAIN_H
#define FILETRANSFERRECEIVERMAIN_H

#include "FileTransferReceiverApp.h"
#include "GUIFrame.h"

class FileTransferReceiverFrame: public GUIFrame
{
    DECLARE_EVENT_TABLE()

    public:
        FileTransferReceiverFrame(wxFrame *, const wxString&, const wxSize&);
        ~FileTransferReceiverFrame();

    private:
        void OnClose(wxCloseEvent&);
        void OnQuit(wxCommandEvent&);
        void OnHelp(wxCommandEvent&);
        void OnInfo(wxCommandEvent&);
        void OnGetIp(wxCommandEvent&);
        void OnAttendiFile(wxCommandEvent&);
        wxString GetErrore(int);
        int  ShowMessage(const wxString&, const wxString& = wxT("Messaggio"), int = wxOK | wxICON_INFORMATION);

        wxPanel    * panel;
        wxBoxSizer * vbox;
        wxBoxSizer * hbox1;
        wxBoxSizer * hbox2;
        wxBoxSizer * hbox3;
        wxBoxSizer * hbox4;
        wxBoxSizer * hbox5;
        wxButton   * btnIP;
        wxButton   * btnAttendiFile;
        wxStaticText * IP;
        wxStaticText * IPString;
        wxStaticText * File;
        wxStaticText * FileName;
        wxGauge      * Gauge;
        wxTextCtrl   * Log;

        wxString     FileToReceive;
        int ACK;
};

#endif // FILETRANSFERRECEIVERMAIN_H
