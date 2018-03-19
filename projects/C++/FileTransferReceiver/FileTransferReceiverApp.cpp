/***************************************************************
 * Name:      FileTransferReceiverApp.cpp
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
#endif

#include "FileTransferReceiverApp.h"
#include "FileTransferReceiverMain.h"

IMPLEMENT_APP(FileTransferReceiverApp);

bool FileTransferReceiverApp::OnInit()
{
    FileTransferReceiverFrame* frame = new FileTransferReceiverFrame(0L, wxT("FileTransfer Receiver"), wxSize(450, 350));
    frame->SetIcon(wxICON(aaaa));
    frame->Show();

    return true;
}
