#ifdef WX_PRECOMP
#include "wx_pch.h"
#endif

#ifdef __BORLANDC__
#pragma hdrstop
#endif

//#if defined(WIN32) || defined(__MINGW32__)

#include "FileTransferReceiverMain.h"
#ifdef __WXMSW__
    #include <winsock.h>
#else
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include <sys/wait.h>
    #include <netdb.h>
#endif
#include <windows.h>

enum
{
    BTN_IP = 1003,
    BTN_ATTENDI
};

BEGIN_EVENT_TABLE( FileTransferReceiverFrame, GUIFrame )
    EVT_BUTTON( BTN_IP, FileTransferReceiverFrame::OnGetIp )
    EVT_BUTTON( BTN_ATTENDI, FileTransferReceiverFrame::OnAttendiFile )
END_EVENT_TABLE()

FileTransferReceiverFrame::FileTransferReceiverFrame(wxFrame *frame, const wxString& title, const wxSize& minsize)
    : GUIFrame(frame, title, minsize)
{
    panel      = new wxPanel(this, wxID_ANY);
    vbox       = new wxBoxSizer(wxVERTICAL);
    hbox1      = new wxBoxSizer(wxHORIZONTAL);
    hbox2      = new wxBoxSizer(wxHORIZONTAL);
    hbox3      = new wxBoxSizer(wxHORIZONTAL);
    hbox4      = new wxBoxSizer(wxHORIZONTAL);
    hbox5      = new wxBoxSizer(wxHORIZONTAL);
    btnIP      = new wxButton(panel, BTN_IP, wxT("Trova &IP"));
    btnAttendiFile = new wxButton(panel, BTN_ATTENDI, wxT("&Attendi File"));
    IP         = new wxStaticText(panel, wxID_ANY, wxT("IP Pubblico:"));
    IPString   = new wxStaticText(panel, wxID_ANY, wxEmptyString);
    File       = new wxStaticText(panel, wxID_ANY, wxT("Nome file:"));
    FileName   = new wxStaticText(panel, wxID_ANY, wxEmptyString);
    Gauge      = new wxGauge(panel, wxID_ANY, 0, wxDefaultPosition, wxDefaultSize, wxGA_HORIZONTAL | wxGA_SMOOTH);
    Log        = new wxTextCtrl(panel, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE | wxTE_DONTWRAP | wxTE_READONLY);
    ACK        = 1;

    hbox1->Add(btnIP, 0, wxALIGN_CENTER | wxALL, 4);
    hbox1->Add(btnAttendiFile, 0, wxALIGN_CENTER | wxALL, 4);
    vbox->Add(hbox1, 1, wxEXPAND);

    hbox2->Add(IP, 0, wxALIGN_CENTER | wxLEFT | wxBOTTOM, 5);
    hbox2->Add(IPString, 1, wxALIGN_CENTER | wxLEFT | wxRIGHT | wxBOTTOM, 5);
    vbox->Add(hbox2, 1, wxEXPAND);

    hbox3->Add(File, 0, wxALIGN_CENTER | wxLEFT | wxBOTTOM, 5);
    hbox3->Add(FileName, 1, wxALIGN_CENTER | wxLEFT | wxRIGHT | wxBOTTOM, 5);
    vbox->Add(hbox3, 1, wxEXPAND);

    hbox4->Add(Gauge, 1, wxALIGN_CENTER | wxEXPAND | wxALL, 2);
    vbox->Add(hbox4, 0, wxALIGN_CENTER | wxEXPAND);

    hbox5->Add(Log, 1, wxEXPAND | wxALL, 2);
    vbox->Add(hbox5, 4, wxEXPAND);

    panel->SetSizer(vbox);
}

FileTransferReceiverFrame::~FileTransferReceiverFrame() {}

void FileTransferReceiverFrame::OnClose(wxCloseEvent &event)
{
    int ret = ShowMessage(wxT("Vuoi realmente uscire?"), wxT("Conferma"), wxYES_NO | wxYES_DEFAULT | wxICON_QUESTION);
    if (ret == wxID_YES)
        Destroy();
    else
        event.Veto();
}

void FileTransferReceiverFrame::OnQuit(wxCommandEvent &event)
{
    Destroy();
}

void FileTransferReceiverFrame::OnHelp(wxCommandEvent &event)
{
    ShowMessage(wxT("TODO"), wxT("Aiuto"));
}

void FileTransferReceiverFrame::OnInfo(wxCommandEvent &event)
{
    ShowMessage(wxT("Realizzato da Nicola Alessandro Domingo."), wxT("Info"));
}

void FileTransferReceiverFrame::OnAttendiFile(wxCommandEvent &event)
{
    HANDLE file;
    SOCKET newsock, listener;
    WSADATA wsaData;
    const int SIZE = 512;
    int Step, err, size = sizeof(struct sockaddr_in);
    unsigned int Received = 0, LeftToReceive, FileSize;
    struct sockaddr_in serv_addr, cli_addr;
    char temp[SIZE];//, msg[50];
    wxString DestPath, BasePath, Temp;
    char * buffer = NULL;

    if ((err = WSAStartup(MAKEWORD(2, 2), &wsaData)) != 0){
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

    if((listener = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET){
        ShowMessage(wxT("Error at creating socket(): ") + GetErrore(WSAGetLastError()), wxT("Errore"), wxICON_ERROR);
        WSACleanup();
        return;
    }

    ZeroMemory(&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(atoi("12345"));

	if(bind(listener, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) == SOCKET_ERROR){
        ShowMessage(wxT("Error at bind(): ") + GetErrore(WSAGetLastError()), wxT("Errore"), wxICON_ERROR);
        closesocket(listener);
        WSACleanup();
        return;
    }

	if (listen(listener, SOMAXCONN) == SOCKET_ERROR){
        ShowMessage(wxT("Error at listen(): ") + GetErrore(WSAGetLastError()), wxT("Errore"), wxICON_ERROR);
        closesocket(listener);
        WSACleanup();
        return;
    }
    Log->AppendText(wxT("Attesa file...\n"));

    if((newsock = accept(listener, (struct sockaddr *)&cli_addr, &size)) == INVALID_SOCKET){
        ShowMessage(wxT("Error at accept(): ") + GetErrore(WSAGetLastError()), wxT("Errore"), wxICON_ERROR);
        closesocket(listener);
        WSACleanup();
        return;
    }
    closesocket(listener);
    Log->AppendText(wxT("Attesa file...\n"));

    wxGetEnv(wxString(wxT("HOMEDRIVE")), &DestPath);
    wxGetEnv(wxString(wxT("HOMEPATH")), &Temp);
    BasePath = DestPath + Temp + wxString(wxT("\\Desktop\\"));
    Temp.Empty();
    BasePath.Empty();

    //Riceviamo il nome del file
    if(recv(newsock, temp, SIZE, 0) == SOCKET_ERROR){
        ShowMessage(wxT("Error at receiving file name: ") + GetErrore(WSAGetLastError()), wxT("Errore"), wxICON_ERROR);
        closesocket(newsock);
        WSACleanup();
        return;
    }

    //Per evitare il backslash iniziale del nome del file
    int i = 0;
	if(temp[i] == '\\') i++;
    for(; temp[i] != '\0'; i++)
        Temp.Append(temp[i]);
    FileToReceive = BasePath + Temp;

    Temp.Printf(wxT("Ricezione del file \"%s\" da %s\n"), (const char *)FileToReceive.c_str(), inet_ntoa(cli_addr.sin_addr));
    Log->AppendText(Temp);
    ZeroMemory(temp, SIZE);

    //Invio ack
    if (ACK && send(newsock, "1", 2, 0) == SOCKET_ERROR){
        ShowMessage(wxT("Error at sending ack: ") + GetErrore(WSAGetLastError()), wxT("Errore"), wxICON_ERROR);
        closesocket(newsock);
        WSACleanup();
        return;
    }

    if((file = CreateFile((WCHAR *)FileToReceive.c_str(), GENERIC_WRITE,
                          0, NULL, CREATE_NEW,
                          FILE_ATTRIBUTE_NORMAL, NULL)) == INVALID_HANDLE_VALUE)
    {
        FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, GetLastError(), 0, (WCHAR *)temp, SIZE, NULL);
        ShowMessage(wxString::Format(wxT("%s"), temp), wxT("Errore"), wxICON_ERROR);
        closesocket(newsock);
        WSACleanup();
        return;
    }

    //Riceviamo la dimensione del file
    if(recv(newsock, temp, SIZE, 0) == SOCKET_ERROR){
        ShowMessage(wxT("Error at receiving file size: ") + GetErrore(WSAGetLastError()), wxT("Errore"), wxICON_ERROR);
        closesocket(newsock);
        CloseHandle(file);
        DeleteFile((WCHAR *)FileToReceive.c_str());
        WSACleanup();
        return;
    }

    FileSize = LeftToReceive = atoi(temp);
    Gauge->SetRange(FileSize);
    ZeroMemory(temp, SIZE);
    Log->AppendText(wxString::Format(wxT("Dimensione del file: %.*f MB\n"), 2, FileSize/1024.0/1024.0));

    //Invio ack
    if (ACK && send(newsock, "1", 2, 0) == SOCKET_ERROR){
        ShowMessage(wxT("Error at sending ack: ") + GetErrore(WSAGetLastError()), wxT("Errore"), wxICON_ERROR);
        closesocket(newsock);
        WSACleanup();
        CloseHandle(file);
        DeleteFile((WCHAR *)FileToReceive.c_str());
        return;
    }

    //Riceviamo il numero di step
    if(recv(newsock, temp, SIZE, 0) == SOCKET_ERROR){
        ShowMessage(wxT("Error at receiving steps: ") + GetErrore(WSAGetLastError()), wxT("Errore"), wxICON_ERROR);
        closesocket(newsock);
        CloseHandle(file);
        DeleteFile((WCHAR *)FileToReceive.c_str());
        WSACleanup();
        return;
    }
    Step = atoi(temp);
    ZeroMemory(temp, SIZE);

    //Invio ack
    if (ACK && send(newsock, "1", 2, 0) == SOCKET_ERROR){
        ShowMessage(wxT("Error at sending ack: ") + GetErrore(WSAGetLastError()), wxT("Errore"), wxICON_ERROR);
        closesocket(newsock);
        CloseHandle(file);
        DeleteFile((WCHAR *)FileToReceive.c_str());
        WSACleanup();
        return;
    }

	while(Received < FileSize)
    {
        //Alloco memoria per il buffer di ricezione
        if((buffer = (char *) malloc(Step)) == NULL){
            ShowMessage(wxT("Impossibile allocare memoria per il file"), wxT("Errore"), wxICON_ERROR);
            closesocket(newsock);
            WSACleanup();
            CloseHandle(file);
            DeleteFile((WCHAR *)FileToReceive.c_str());
            return;
        }

        if((err = recv(newsock, buffer, Step, 0)) == SOCKET_ERROR){
            ShowMessage(wxT("Error at receiving file: ") + GetErrore(WSAGetLastError()), wxT("Errore"), wxICON_ERROR);
            closesocket(newsock);
            free(buffer);
            CloseHandle(file);
            DeleteFile((WCHAR *)FileToReceive.c_str());
            WSACleanup();
            return;
        }

        if(WriteFile(file, buffer, Step, NULL, NULL) == 0){
            FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, GetLastError(), 0, (WCHAR *)temp, SIZE, NULL);
            ShowMessage(wxString::Format(wxT("Impossibile scrivere il file; %s"), temp), wxT("Errore"), wxICON_ERROR);
            CloseHandle(file);
            DeleteFile((WCHAR *)FileToReceive.c_str());
            free(buffer);
            return;
        }
        FlushFileBuffers(file);
        free(buffer);

        Received += Step;
        LeftToReceive -= Step;
        if(LeftToReceive < (unsigned int)Step)
           Step = LeftToReceive;

        Gauge->SetValue(Received);

        //Invio ack
        if (ACK && send(newsock, "1", 2, 0) == SOCKET_ERROR){
            ShowMessage(wxT("Error at receiving file: ") + GetErrore(WSAGetLastError()), wxT("Errore"), wxICON_ERROR);
            closesocket(newsock);
            CloseHandle(file);
            DeleteFile((WCHAR *)FileToReceive.c_str());
            WSACleanup();
            return;
        }
    }

    CloseHandle(file);
    free(buffer);
}

void FileTransferReceiverFrame::OnGetIp(wxCommandEvent &event)
{
    int n;
    const int Size = 300;
    WSADATA wsaData;
    SOCKET mysock;
    wxString IP, errore;
    struct sockaddr_in serv;
	struct hostent * myip_server = NULL;
	char * ptr;
	char str[Size];
    char message[Size] = "GET /ip.php?.txt HTTP/1.1\r\n"
                         "User-Agent: filetransfer\r\n"
                         "Accept: */*\r\n"
                         "Host: www.indirizzo-ip.com\r\n"
                         "Connection: close\r\n"
                         "\r\n";

    if ((n = WSAStartup(MAKEWORD(2, 2), &wsaData)) != 0){
        ShowMessage(wxString::Format(wxT("Error at WSAStartup(): %d"), n), wxT("Errore"), wxICON_ERROR);
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

    if((mysock = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET){
        ShowMessage(wxT("Error at creating socket: ") + GetErrore(WSAGetLastError()), wxT("Errore"), wxICON_ERROR);
        WSACleanup();
        return;
    }

    if((myip_server = gethostbyname("www.indirizzo-ip.com")) == NULL){
        ShowMessage(wxT("Error at gethostbyname(): ") + GetErrore(WSAGetLastError()), wxT("Errore"), wxICON_ERROR);
        closesocket(mysock);
        WSACleanup();
        return;
    }

    ZeroMemory(&serv, sizeof(serv));
    CopyMemory((char *)&serv.sin_addr.s_addr, (char *)myip_server->h_addr, myip_server->h_length);
	serv.sin_family = AF_INET;
	serv.sin_port = htons(80);

	if (connect(mysock,(struct sockaddr *)&serv, sizeof(serv)) == SOCKET_ERROR){
        ShowMessage(wxT("Error at connect(): ") + GetErrore(WSAGetLastError()), wxT("Errore"), wxICON_ERROR);
        closesocket(mysock);
        WSACleanup();
        return;
    }

    if (send(mysock, message, Size, 0) == SOCKET_ERROR){
        ShowMessage(wxT("Error at send(): ") + GetErrore(WSAGetLastError()), wxT("Errore"), wxICON_ERROR);
        closesocket(mysock);
        WSACleanup();
        return;
    }

    if(recv(mysock, str, Size, 0) == SOCKET_ERROR){
        ShowMessage(wxT("Error at recv(): ") + GetErrore(WSAGetLastError()), wxT("Errore"), wxICON_ERROR);
        closesocket(mysock);
        WSACleanup();
        return;
    }
    closesocket(mysock);

    ptr = strstr(str, "Content-Type: text/plain");
    ptr += 28;
    for(; ((*ptr)-'0' >= 0 && (*ptr)-'0' <= 9) || (*ptr) == '.'; ++ptr)
        IP.Append(*ptr);
    IPString->SetLabel(IP);
}

int FileTransferReceiverFrame::ShowMessage(const wxString& message, const wxString& caption, int flags)
{
     int ret;
     wxMessageDialog * dial = new wxMessageDialog(NULL, message, caption, flags);
     ret = dial->ShowModal();
     dial->Destroy();
     return ret;
}

wxString FileTransferReceiverFrame::GetErrore(int err)
{
    wxString str;
    switch(err)
    {
        case WSAEACCES:
            str = wxT("Permission denied.");
            break;
        case WSAEFAULT:
            str = wxT("Bad address.");
            break;
        case WSAEADDRINUSE:
            str = wxT("Address already in use.");
            break;
        case WSAECONNABORTED:
            str = wxT("Software caused connection abort.");
            break;
        case WSAECONNRESET:
            str = wxT("Connection reset by peer.");
            break;
        case WSAETIMEDOUT:
            str = wxT("Connection timed out.");
            break;
        case WSAECONNREFUSED:
            str = wxT("Connection refused.");
            break;
        case WSAELOOP:
            str = wxT("Cannot translate name.");
            break;
        case WSAEHOSTDOWN:
            str = wxT("Host is down.");
            break;
        case WSAHOST_NOT_FOUND:
            str = wxT("Host not found.");
            break;
        default:
            str.Printf(wxT("%d"), err);
    }
    return str;
}
