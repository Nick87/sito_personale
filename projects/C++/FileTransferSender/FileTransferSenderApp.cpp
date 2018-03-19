/***************************************************************
 * Name:      FileTransferSenderApp.cpp
 * Purpose:   Code for Application Class
 * Author:    Nicola Domingo (nicola.domingo@gmail.com)
 * Created:   2010-08-28
 * Copyright: Nicola Domingo ()
 * License:
 **************************************************************/

#ifdef WX_PRECOMP
#include "wx_pch.h"
#endif

#ifdef __BORLANDC__
#pragma hdrstop
#endif //__BORLANDC__

#include "FileTransferSenderApp.h"
#include "FileTransferSenderMain.h"

IMPLEMENT_APP(FileTransferSenderApp);

bool FileTransferSenderApp::OnInit()
{
    FileTransferSenderFrame* frame = new FileTransferSenderFrame(0L, wxT("FileTransfer Sender"), wxSize(450, 350));
    frame->SetIcon(wxICON(aaaa));
    frame->Show();

    return true;
}
