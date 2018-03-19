#ifdef WX_PRECOMP
#include "wx_pch.h"
#endif

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#include "FileTransferSenderMain.h"
#include <wx/tokenzr.h>

enum
{
    BTN_SFOGLIA = 1003,
    BTN_INVIA,
    BTN_AZZERA
};

BEGIN_EVENT_TABLE( FileTransferSenderFrame, GUIFrame )
    EVT_BUTTON( BTN_SFOGLIA, FileTransferSenderFrame::OnSfoglia )
    EVT_BUTTON( BTN_INVIA, FileTransferSenderFrame::OnInvia )
    EVT_BUTTON( BTN_AZZERA, FileTransferSenderFrame::OnAzzera )
END_EVENT_TABLE()

FileTransferSenderFrame::FileTransferSenderFrame(wxFrame *frame, const wxString& title, const wxSize& minsize)
    : GUIFrame(frame, title, minsize)
{
    panel = new wxPanel(this, wxID_ANY);
    vbox  = new wxBoxSizer(wxVERTICAL);
    hbox1 = new wxBoxSizer(wxHORIZONTAL);
    hbox2 = new wxBoxSizer(wxHORIZONTAL);
    hbox3 = new wxBoxSizer(wxHORIZONTAL);
    hbox4 = new wxBoxSizer(wxHORIZONTAL);
    hbox5 = new wxBoxSizer(wxHORIZONTAL);
    btnSfoglia = new wxButton(panel, BTN_SFOGLIA, wxT("&Sfoglia"));
    btnInvia   = new wxButton(panel, BTN_INVIA, wxT("&Invia"));
    btnAzzera  = new wxButton(panel, BTN_AZZERA, wxT("&Azzera"));
    File       = new wxStaticText(panel, wxID_ANY, wxT("File:"));
    FileName   = new wxStaticText(panel, wxID_ANY, wxEmptyString);
    IpHost     = new wxStaticText(panel, wxID_ANY, wxT("I&P (Host):"));
    IpHostText = new wxTextCtrl(panel, wxID_ANY, wxT(""));
    Gauge      = new wxGauge(panel, wxID_ANY, 0, wxDefaultPosition, wxDefaultSize, wxGA_HORIZONTAL | wxGA_SMOOTH);
    Log        = new wxTextCtrl(panel, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE | wxTE_DONTWRAP | wxTE_READONLY);

    hbox1->Add(btnSfoglia, 0, wxALIGN_CENTER | wxALL, 4);
    hbox1->Add(btnInvia, 0, wxALIGN_CENTER | wxALL, 4);
    hbox1->Add(btnAzzera, 0, wxALIGN_CENTER | wxALL, 4);
    vbox->Add(hbox1, 1, wxEXPAND);

    hbox2->Add(File, 0, wxALIGN_CENTER | wxLEFT | wxBOTTOM, 5);
    hbox2->Add(FileName, 1, wxALIGN_CENTER | wxLEFT | wxRIGHT | wxBOTTOM, 5);
    vbox->Add(hbox2, 1, wxEXPAND);

    hbox3->Add(IpHost, 0, wxALIGN_CENTER | wxLEFT | wxBOTTOM, 5);
    hbox3->Add(IpHostText, 1, wxALIGN_CENTER | wxALL, 2);
    vbox->Add(hbox3, 1, wxEXPAND);

    hbox4->Add(Gauge, 1, wxALIGN_CENTER | wxEXPAND | wxALL, 2);
    vbox->Add(hbox4, 0, wxALIGN_CENTER | wxEXPAND);

    hbox5->Add(Log, 1, wxEXPAND | wxALL, 2);
    vbox->Add(hbox5, 4, wxEXPAND);

    panel->SetSizer(vbox);
}

FileTransferSenderFrame::~FileTransferSenderFrame()
{
}

void FileTransferSenderFrame::OnClose(wxCloseEvent &event)
{
    int ret = ShowMessage(wxT("Vuoi realmente uscire?"), wxT("Conferma"), wxYES_NO | wxYES_DEFAULT | wxICON_QUESTION);
    if (ret == wxID_YES)
        Destroy();
    else
        event.Veto();
}

void FileTransferSenderFrame::OnQuit(wxCommandEvent &event)
{
    Destroy();
}

void FileTransferSenderFrame::OnHelp(wxCommandEvent &event)
{
    ShowMessage(wxT("TODO"), wxT("Aiuto"));
}

void FileTransferSenderFrame::OnInfo(wxCommandEvent &event)
{
    ShowMessage(wxT("Realizzato da Nicola Alessandro Domingo."), wxT("Info"));
}

void FileTransferSenderFrame::OnSfoglia(wxCommandEvent &event)
{
    wxFileDialog * FileDialog = new wxFileDialog(panel, wxT("File da inviare"));
    if(FileDialog->ShowModal() == wxID_OK)
    {
        FileToSend = FileDialog->GetPath();
        FileName->SetLabel(FileToSend);
    }
    FileDialog->Destroy();
}

void FileTransferSenderFrame::OnInvia(wxCommandEvent &event)
{
    HANDLE file;
    SOCKET newsock, listener;
    WSADATA wsaData;
	DWORD file_size, bytesWritten;
	struct sockaddr_in serv_addr, cli_addr;
    double tacche;
    int step, i, err, size = sizeof(struct sockaddr_in), n, received = 0, left_to_receive;
    unsigned char * buffer;
    //char base_path[SIZE], dest_path[SIZE], temp[SIZE], public_ip[20], msg[50];

    if(!CheckIPHost(IpHostText->GetValue())){
        ShowMessage(wxT("Indirizzo IP o nome host mancante o non valido."), wxT("Errore"), wxICON_ERROR);
        return;
    }

    if ((err = WSAStartup(MAKEWORD(2,2), &wsaData)) != 0){
        ShowMessage(wxString::Format(wxT("Error at WSAStartup(): %d"), err), wxT("Errore"), wxICON_ERROR);
        return;
    }

    again:
    Log->AppendText(wxT("Test della connessione... "));
    if(gethostbyname("www.google.it") == NULL)
    {
        if(ShowMessage(wxT("Connessione non disponibile. Riprovare?"), wxT("Messaggio"),
                       wxICON_QUESTION | wxYES_NO | wxYES_DEFAULT) == wxID_YES)
        {
            Log->AppendText(wxT("\n"));
            goto again;
        }
            return;
    }
    Log->AppendText(wxT("OK\n"));
}

void FileTransferSenderFrame::OnAzzera(wxCommandEvent &event)
{
    Gauge->SetValue(0);
    IpHostText->Clear();
    FileName->SetLabel(wxEmptyString);
}

bool FileTransferSenderFrame::CheckIPHost(const wxString& IP)
{
    wxString Val;
    long LongVal = 0;

    if(IP == wxEmptyString)
        return false;

    wxStringTokenizer Tkz(IP, wxT("."), wxTOKEN_RET_EMPTY_ALL);
    if(Tkz.CountTokens() != 4)
        return false;

    while ( Tkz.HasMoreTokens() )
    {
        Val = Tkz.GetNextToken();
        if(!Val.IsNumber())
            return false;

        Val.ToLong(&LongVal, 10);
        if(LongVal < 0 || LongVal > 255)
            return false;
    }
    return true;
}

int FileTransferSenderFrame::ShowMessage(const wxString& message, const wxString& caption, int flags)
{
     int ret;
     wxMessageDialog * dial = new wxMessageDialog(NULL, message, caption, flags);
     ret = dial->ShowModal();
     dial->Destroy();
     return ret;
}
