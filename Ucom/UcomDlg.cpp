// UcomDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Ucom.h"
#include "UcomDlg.h"
#include "afxdialogex.h"

#include "DeConsole.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CUcomDlg �Ի���


CUcomDlg::CUcomDlg(CWnd* pParent /*=NULL*/)
: CDialog(CUcomDlg::IDD, pParent)
, DataRx(), DataTx(), mUart()
, MultiSendDlg(this, &uartPortIsOpen,mUart.GetHandleAddr())
{
	//{{AFX_DATA_INIT(CUcomDlg)
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	//���ó���ͼ��
	m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON1);
}

void CUcomDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CUcomDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BtnOpen, OnBtnOpen)
	ON_BN_CLICKED(IDC_BtnSend, OnBtnSend)
	ON_CBN_DROPDOWN(IDC_CbUartPort, OnDropdownCbUartPort)
	ON_CBN_SELENDOK(IDC_CbUartPort, OnSelendokCbUartPort)
	ON_BN_CLICKED(IDC_Launch, OnLaunch)
	ON_BN_CLICKED(IDC_BtnClearRecv, OnBtnClearRecv)
	ON_BN_CLICKED(IDC_BtnClearSend, OnBtnClearSend)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_CkbSendOnTime, OnCkbSendOnTime)
	ON_EN_CHANGE(IDC_EdbSendDelay, OnChangeEdbSendDelay)
	ON_BN_CLICKED(IDC_CkbFrameNewLine, OnCkbFrameNewLine)
	ON_BN_CLICKED(IDC_BtnClearnCnt, OnBtnClearnCnt)
	ON_BN_CLICKED(IDC_CkbDispHex, OnCkbDispHex)
	ON_EN_CHANGE(IDC_EditTxData, OnChangeEditTxData)
	ON_BN_CLICKED(IDC_CkbSendHex, OnCkbSendHex)
	ON_BN_CLICKED(IDC_BtnIsDispRx, OnBnClickedBtnisdisprx)
	ON_MESSAGE(WM_COMM_RX_MSG, &CUcomDlg::OnRxMsgProc)


	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BtnToolBox, &CUcomDlg::OnBnClickedBtntoolbox)
	ON_COMMAND(ID_MeuDevManger, &CUcomDlg::OnMeudevmanger)
	ON_COMMAND(ID_MeuCalc, &CUcomDlg::OnMeucalc)
	ON_COMMAND(ID_MeuNotepad, &CUcomDlg::OnMeunotepad)
	ON_COMMAND(ID_MenuReg, &CUcomDlg::OnMenureg)
	ON_COMMAND(ID_MenuCmd, &CUcomDlg::OnMenucmd)
	ON_COMMAND(ID_MenuServ, &CUcomDlg::OnMenuserv)
	ON_BN_CLICKED(IDC_BtnWinSize, &CUcomDlg::OnBnClickedBtnwinsize)
	ON_BN_CLICKED(IDC_BtnBackColor, &CUcomDlg::OnBnClickedBtnbackcolor)
	ON_BN_CLICKED(IDC_BtnFontColor, &CUcomDlg::OnBnClickedBtnfontcolor)
	//	ON_COMMAND(IDR_MENU1, &CUcomDlg::OnToolsBoxMenu)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BtnFont, &CUcomDlg::OnBnClickedBtnfont)
	ON_BN_CLICKED(IDC_CkbRxFullClc, &CUcomDlg::OnBnClickedCkbrxfullclc)
	ON_EN_CHANGE(IDC_EdbFullBytes, &CUcomDlg::OnChangeEdbfullbytes)
	ON_BN_CLICKED(IDC_BtnSaveRx, &CUcomDlg::OnBnClickedBtnsaverx)
	ON_BN_CLICKED(IDC_BtnClearAll, &CUcomDlg::OnBnClickedBtnclearall)
	ON_BN_CLICKED(IDC_BtnHelp, &CUcomDlg::OnBnClickedBtnhelp)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TABEx, &CUcomDlg::OnSelchangeTabex)
	ON_BN_CLICKED(IDC_BtnEncoder, &CUcomDlg::OnBnClickedBtnencoder)
	ON_BN_CLICKED(IDC_BtnSendFile, &CUcomDlg::OnBnClickedBtnsendfile)
//	ON_WM_CHAR()
ON_BN_CLICKED(IDC_CkbCMD, &CUcomDlg::OnBnClickedCkbcmd)
END_MESSAGE_MAP()


// CUcomDlg ��Ϣ�������

BOOL CUcomDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO:  �ڴ���Ӷ���ĳ�ʼ������
	SetWindowText(_T("Ucom v1.02 �ᴮ�� ��������ѧ����ѧ��"));

	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon


#ifdef USING_CONSLOE
	InitDebugConsole();
#endif

	hRxThread = NULL;
	uartPortIsOpen = false;
	isNewLineSend = false;
	isDispHex = false;
	isSendHex = false;
	isDispRx = true;
	isCmdMode = false;
	encoderMode = 0;
	rxCnt = 0;
	txCnt = 0;

	InitTabEx();

	InitCbBuart();
	//װ��ע�������
	LoadRegConfig();
	SetDlgItemInt(IDC_EdbSendDelay, 1000);
	SetDlgItemInt(IDC_EdbFullBytes, 5120);

	
	DisableCbUart(FALSE);

	SetRichLineSpace();
	OnBnClickedBtnwinsize();

	cmdNextPointer = 0;
	cmdDispPointer = 0;

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}
// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CUcomDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CUcomDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CUcomDlg::ChangeBmpPic(int PicCtrlID, unsigned short nPicID)
{
	CBitmap bitmap;
	HBITMAP hBmp;
	CStatic *pStatic = (CStatic*)GetDlgItem(PicCtrlID);

	bitmap.LoadBitmap(nPicID);				// ��λͼIDB_BITMAP1���ص�bitmap   
	hBmp = (HBITMAP)bitmap.GetSafeHandle();  // ��ȡbitmap����λͼ�ľ��

	pStatic->SetBitmap(hBmp);				// ����ͼƬ�ؼ�
}

//�ӿռ�õ�������Ϣ�ַ���
DCB CUcomDlg::GetUartConfigDCB(void)
{
	DCB configDCB;
	CString tmpStr, UartConfig;
	CComboBox *pCombox;

	//�趨���ڲ���
	pCombox = (CComboBox*)GetDlgItem(IDC_CbUartBaudrate);
	pCombox->GetLBText(pCombox->GetCurSel(), tmpStr);
	configDCB.BaudRate = _ttoi(tmpStr);

	pCombox = (CComboBox*)GetDlgItem(IDC_CbUartECC);
	configDCB.Parity = pCombox->GetCurSel();

	pCombox = (CComboBox*)GetDlgItem(IDC_CbUartDatabit);
	pCombox->GetLBText(pCombox->GetCurSel(), tmpStr);
	configDCB.ByteSize = atoi(tmpStr);

	pCombox = (CComboBox*)GetDlgItem(IDC_CbUartStopbit);
	configDCB.StopBits = pCombox->GetCurSel();

	return configDCB;
}

bool CUcomDlg::IsUartPortAvailable(void)
{
	CString comInfo;
	CComboBox *pCombox = (CComboBox *)GetDlgItem(IDC_CbUartPort);
	pCombox->GetLBText(pCombox->GetCurSel(), comInfo);

	if (comInfo.Left(3) == _T("COM"))
		return TRUE;
	else
		return FALSE;
}


void CUcomDlg::OpenUart()
{
	CButton *pButton;
	CComboBox *pCombox = (CComboBox *)GetDlgItem(IDC_CbUartPort);
	CString strtmp;

	if (uartPortIsOpen == FALSE)
	{
		//���ռ�þ�ȡ��ռ��
		if (mUart.isConnected())
			mUart.ClosePort();

		if (IsUartPortAvailable())
		{
			pCombox->GetLBText(pCombox->GetCurSel(), strtmp);
			mUart.ConfigUart(strtmp, GetUartConfigDCB());
		}
		else
		{
			AfxMessageBox(_T("������Ч,��ˢ��"));
			return;
		}

		if (!mUart.OpenCom())
		{
			AfxMessageBox(_T("�򿪴���ʧ��"));
			return;
		}

		//���������߳�
		hRxThread = AfxBeginThread(RxThreadFunc, mUart.GetThreadStartPara(), THREAD_PRIORITY_NORMAL);
		if (hRxThread == NULL){
			TRACE("Rx Listenner Thread Created Failed");
			return;
		}

		pButton = (CButton*)GetDlgItem(IDC_BtnOpen);
		SetDlgItemText(IDC_BtnOpen, _T("�رմ���"));
		ChangeBmpPic(IDC_PicUartStatus, IDB_SwOn);

		DisableCbUart(TRUE);
		uartPortIsOpen = TRUE;
		SetDelaySend();
		//�趨���տ��ı�ˢ��ʱ�� ����24Hz
		SetTimer(FLASH_RX_EDITBOX_TIMER_ID, rxFlashPeriod, NULL);
	}
	else if (uartPortIsOpen == TRUE)
	{
		//����λ��־λ����ر�ʱ����մ���
		uartPortIsOpen = FALSE;
		KillTimer(AUTO_SEND_TIMER_ID);//�Զ����͹رն�ʱ��
		KillTimer(FLASH_RX_EDITBOX_TIMER_ID);//�رս�����ʾ��ʱ��

		//���������߳�
		//mUart.CloseWaitThread();
		TerminateThread(hRxThread, 0);

		SetDlgItemText(IDC_BtnOpen, _T("�򿪴���"));
		ChangeBmpPic(IDC_PicUartStatus, IDB_SwOff);
		DisableCbUart(FALSE);
		//�رմ���
		if (mUart.isConnected())
			mUart.ClosePort();
	}
}

void CUcomDlg::OnBtnOpen()
{
	OpenUart();
}

void CUcomDlg::InitCbBuart(void)
{
	CComboBox *pComBox;

	pComBox = (CComboBox *)GetDlgItem(IDC_CbUartPort);
	pComBox->ResetContent();
	//��ȡ�����б�
	mUart.GetComList(pComBox);
	//Ĭ��ѡ���һ��
	pComBox->SetCurSel(0);

	//��Ӳ�����
	pComBox = (CComboBox*)GetDlgItem(IDC_CbUartBaudrate);
	TCHAR BaudrateTable[][7] = { "460800", "230400", "194000", "115200", "57600", "56000", "38400"
		, "19200", "14400", "9600", "4800", "2400", "1200" };
	for (int i = 0; i < (sizeof(BaudrateTable) / sizeof(BaudrateTable[0])); i++)
		pComBox->InsertString(0, BaudrateTable[i]);

	//ѡ��115200ΪĬ��
	pComBox->SetCurSel(pComBox->FindString(-1, "115200"));

	//����λ
	pComBox = (CComboBox*)GetDlgItem(IDC_CbUartDatabit);
	pComBox->InsertString(0, "8");
	pComBox->InsertString(0, "7");
	pComBox->InsertString(0, "6");
	pComBox->InsertString(0, "5");

	pComBox->SetCurSel(3);//ѡ��8λΪĬ��

	//ֹͣλ
	pComBox = (CComboBox*)GetDlgItem(IDC_CbUartStopbit);
	pComBox->InsertString(0, "2");
	pComBox->InsertString(0, "1.5");
	pComBox->InsertString(0, "1");

	pComBox->SetCurSel(0);//ѡ��115200λΪĬ��

	pComBox = (CComboBox*)GetDlgItem(IDC_CbUartECC);
	pComBox->InsertString(0, "even");
	pComBox->InsertString(0, "odd ");
	pComBox->InsertString(0, "none");
	//ѡ����У��ΪĬ��
	pComBox->SetCurSel(0);
	TRACE("Add ok");
}



bool CUcomDlg::InitRegData(void)
{
	CString tmp = AfxGetApp()->GetProfileString("Config", "BDSE", "NULL");
	if (tmp == "NULL")
		return false;
	else
		return true;
}

void CUcomDlg::WriteRegData(void)
{
	CComboBox *pCombox;
	CString strtmp;
	pCombox = (CComboBox *)GetDlgItem(IDC_CbUartPort);
	//���ں�
	pCombox->GetLBText(pCombox->GetCurSel(), strtmp);
	AfxGetApp()->WriteProfileString("Config", "ComName", strtmp);

	strtmp.Empty();
	strtmp.Format("%d-%d-%d-%d",
		((CComboBox*)GetDlgItem(IDC_CbUartBaudrate))->GetCurSel()
		, ((CComboBox*)GetDlgItem(IDC_CbUartDatabit))->GetCurSel()
		, ((CComboBox*)GetDlgItem(IDC_CbUartStopbit))->GetCurSel()
		, ((CComboBox*)GetDlgItem(IDC_CbUartECC))->GetCurSel());
	//��������
	AfxGetApp()->WriteProfileString("Config", "BDSE", strtmp);

	CHARFORMAT cf;
	CRichEditCtrl *pRich = ((CRichEditCtrl *)GetDlgItem(IDC_RichRx));
	pRich->GetSelectionCharFormat(cf);
	AfxGetApp()->WriteProfileInt("Config", "TextColor", cf.crTextColor);
	//û�л�ȡ����ɫ�ķ���ֻ���ñ�����¼
	AfxGetApp()->WriteProfileInt("Config", "GroundColor", backgroudColor);

	TRACE("Write Reg Finish %d\n", backgroudColor);
}

void CUcomDlg::GetRegData(CString &comName, CString &dcbConfig)
{
	comName = AfxGetApp()->GetProfileString("Config", "ComName", "NULL");
	dcbConfig = AfxGetApp()->GetProfileString("Config", "BDSE", "NULL");
	TRACE(comName + dcbConfig);
}

void CUcomDlg::LoadRegConfig()
{
	CString comName, dcbConfig;
	CComboBox *pComBox = (CComboBox*)GetDlgItem(IDC_CbUartPort);
	CRichEditCtrl *pRich = (CRichEditCtrl*)GetDlgItem(IDC_RichRx);
	CHARFORMAT cf = { 0 };
	cf.cbSize = sizeof(cf);;
	cf.dwMask = CFM_COLOR;
	cf.dwEffects = 0;

	if (InitRegData() == false)
	{
		//��������Ĭ������
		backgroudColor = RGB(242, 241, 215);
		cf.crTextColor = RGB(0, 0, 0);

		pRich->SetDefaultCharFormat(cf);
		pRich->SetBackgroundColor(false, backgroudColor);
	}
	else
	{
		GetRegData(comName, dcbConfig);
		int sel = pComBox->FindString(-1, comName);
		//û�иô��ڣ����߲��Ǵ��ں�ʹ��Ĭ�ϣ���ʼ��ֵ��
		if (sel != CB_ERR && comName[0] == 'C')
			pComBox->SetCurSel(sel);

		int baudSel = 0, dataSel = 0, stopSel = 0, eccSel = 0;

		sscanf_s(dcbConfig, "%d-%d-%d-%d", &baudSel, &dataSel, &stopSel, &eccSel);
		((CComboBox*)GetDlgItem(IDC_CbUartBaudrate))->SetCurSel(baudSel);
		((CComboBox*)GetDlgItem(IDC_CbUartDatabit))->SetCurSel(dataSel);
		((CComboBox*)GetDlgItem(IDC_CbUartStopbit))->SetCurSel(stopSel);
		((CComboBox*)GetDlgItem(IDC_CbUartECC))->SetCurSel(eccSel);

		//��ɫ����
		cf.crTextColor = AfxGetApp()->GetProfileInt("Config", "TextColor", RGB(0, 0, 0));
		pRich->SetDefaultCharFormat(cf);
		backgroudColor = AfxGetApp()->GetProfileInt("Config", "GroundColor", RGB(242, 241, 215));
		pRich->SetBackgroundColor(false, backgroudColor);
	}
}

//���������Ƿ�ɲ���
void CUcomDlg::DisableCbUart(bool choose)
{
	choose = !choose;
	GetDlgItem(IDC_CbUartBaudrate)->EnableWindow(choose);
	GetDlgItem(IDC_CbUartDatabit)->EnableWindow(choose);
	GetDlgItem(IDC_CbUartStopbit)->EnableWindow(choose);
	GetDlgItem(IDC_CbUartECC)->EnableWindow(choose);
	GetDlgItem(IDC_CbUartPort)->EnableWindow(choose);
}

void CUcomDlg::InitTabEx(void)
{
	CRect rect;
	CTabCtrl *pTab = (CTabCtrl *)GetDlgItem(IDC_TABEx);
	pTab->InsertItem(0, "����ͼ��");
	pTab->InsertItem(1, "�����ѯ");
	pTab->InsertItem(2, "���ռ���");
	pTab->InsertItem(3, "��������");

	//186*243
	GraphDlg.Create(IDD_GRAPH, pTab);
	pTab->GetClientRect(&rect);
	rect.top += 27;
	GraphDlg.MoveWindow(&rect);
	GraphDlg.ShowWindow(true);

	EncoderDlg.Create(IDD_ENCODER, pTab);
	pTab->GetClientRect(&rect);
	rect.top += 27;
	EncoderDlg.MoveWindow(&rect);
	EncoderDlg.ShowWindow(false);

	DataWatchDlg.Create(IDD_WATCH, pTab);
	pTab->GetClientRect(&rect);
	rect.top += 27;
	DataWatchDlg.MoveWindow(&rect);
	DataWatchDlg.ShowWindow(false);

	MultiSendDlg.Create(IDD_XSEND, pTab);
	pTab->GetClientRect(&rect);
	rect.top += 27;
	MultiSendDlg.MoveWindow(&rect);
	MultiSendDlg.ShowWindow(false);
}


void CUcomDlg::SendEditBoxData(void)
{
	CString tmpStr("\r\n");
	//����
	if (isNewLineSend == TRUE) {
		mUart.UnblockSend(DataTx.GetCStrData() + tmpStr);
		txCnt = txCnt+1+ DataTx.GetLength();
	}
	else {
		mUart.UnblockSend(DataTx.GetCStrData());
		txCnt += DataTx.GetLength();
	}
	
	//ˢ�¼���
	tmpStr.Format(_T("�������ݣ�%d Bytes"), txCnt);
	SetDlgItemText(IDC_GrpSend, tmpStr);
}

//ˢ�½��տ�
void CUcomDlg::ReflashRecvEdit(void)
{
	CString tmpStr;
	CRichEditCtrl *pRich = (CRichEditCtrl *)GetDlgItem(IDC_RichRx);
	
	static int cnt=0,lastRxCnt=0;
	//�����Ͱ�rxFlashPeriod = 50ms����,���г�20

	lastRxCnt += DataRx.GetLength();
	if (++cnt == 10)
	{
		//ˢ�¼���
		tmpStr.Format(_T("�������ݣ�%d Bytes ,�����ٶȣ�%d Bps"), rxCnt, lastRxCnt*2);
		SetDlgItemText(IDC_GrpRecv, tmpStr);
		cnt = 0;
		lastRxCnt = 0;
	}
	


	if (DataRx.GetLength() != 0)
	{
		//��Limt�������У������ֵ���
		if (isRxFullClc && (pRich->GetTextLength() >limitBytes))
		{
			pRich->SetWindowTextA("");
		}
		if (isDispRx)
		{
			//���ѡ�����,���±༭������
			pRich->SetSel(-1, -1);
			if (isDispHex)
			{
				pRich->ReplaceSel(DataRx.GetCStrData());
			}
			else
			{
				switch (encoderMode)
				{
				case 0:
					pRich->ReplaceSel(DataRx.GetCStrData()); break;
				case 1:
					pRich->ReplaceSel(EncoderDlg.Utf8toAscii(DataRx.GetCStrData()));
					break;
				case 2:
					pRich->ReplaceSel(EncoderDlg.Unicode2Ascii(DataRx.GetCStrData()));
					break;
				default:
					break;
				}
			}
		}
		DataRx.ClearData();
	}
}


void CUcomDlg::OnLaunch()
{
	//	AfxMessageBox(_T("Hello World!"));
	//TRACE("%d\n", DataRx.ValueOfString("12"));
	//::SendMessage(::AfxGetMainWnd()->m_hWnd, WM_COMM_RX_MSG, 1, 0);
	// TODO: Add your control notification handler code here
	//CRichEditCtrl *pRich = ((CRichEditCtrl *)GetDlgItem(IDC_RichRx));

	//CRect rect;
	//GetClientRect(&rect);

	//CClientDC dc(this);
	//dc.MoveTo(rect.left, rect.top);
	//dc.LineTo(rect.right, rect.bottom);

	//dc.MoveTo(rect.left, rect.bottom);
	//dc.LineTo(rect.right, rect.top);
	//dc.MoveTo(rect.left, rect.top);

	//dc.Rectangle(0, 0, 100, 100);

	AfxMessageBox("����ѧ�Ỷӭ��");


	//GetFocus()==GetDlgItem(  )
}

void CUcomDlg::OnTimer(UINT nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	switch (nIDEvent)
	{
	case AUTO_SEND_TIMER_ID:
		SendEditBoxData();
		break;
	case FLASH_RX_EDITBOX_TIMER_ID:
		ReflashRecvEdit();
		break;
	default:
		break;
	}
	CDialog::OnTimer(nIDEvent);
}


void CUcomDlg::OnBtnClearnCnt()
{
	CString tmpStr;
	rxCnt = 0;
	txCnt = 0;

	//�������
	tmpStr.Format(_T("�������ݣ�%d Bytes ,�����ٶȣ�0 Bps"), rxCnt);
	SetDlgItemText(IDC_GrpRecv, tmpStr);

	tmpStr.Format(_T("�������ݣ�%d Bytes"), txCnt);
	SetDlgItemText(IDC_GrpSend, tmpStr);
}


void CUcomDlg::OnCkbSendHex()
{
	if (BST_CHECKED == IsDlgButtonChecked(IDC_CkbSendHex))
	{
		if (isCmdMode) {
			CButton *pCkb = (CButton *)GetDlgItem(IDC_CkbSendHex);
			pCkb->SetCheck(BST_UNCHECKED);
		}
		else
		{
			isSendHex = TRUE;
			SetDlgItemText(IDC_EditTxData, "");
		}
	}
	else
		isSendHex = FALSE;
	
	DataTx.ClearData();
}


void CUcomDlg::OnChangeEditTxData()
{
	CString strtmp;
	char *pCh;

	GetDlgItemText(IDC_EditTxData, strtmp);
	CEdit *pEb = (CEdit *)GetDlgItem(IDC_EditTxData);
	if (isSendHex)
	{
		bool isPure = true;
		pCh = strtmp.GetBuffer(0);
		while (*pCh != '\0')
		{
			//��ֹ���������ַ�
			if (!DataTx.isHexChar(*pCh))
			{
				int pos = strtmp.Find(*pCh);
				strtmp = strtmp.Left(pos);
				SetDlgItemText(IDC_EditTxData, strtmp);
				pEb->SetSel(pos, pos, TRUE);
				isPure = FALSE;
				break;
			}
			pCh++;
		}

		if (isPure)
		{
			//16���Ƹ�ʽ����
			DataTx.ReString(strtmp, TRUE);
		}
		else
		{
			HWND hWnd = ::GetLastActivePopup(GetSafeHwnd()); //���Ҽ����MessageBox
			if (hWnd != GetSafeHwnd())
			{
				::EndDialog(hWnd, 0);
			}
			DataTx.ClearData();
			AfxMessageBox(_T("��������Ч��HEX��ʽ!\n��Χ��('0-9','a-f','A-F')\n�ֽڼ�����ÿո����"));
		}
	}
	else
	{
		DataTx.ReString(strtmp);

		//����CMDģʽ�س�����(���ݰ����س�)���������͵��������������ʽ�ӵ����տ��������
		if (isCmdMode && uartPortIsOpen && (strtmp.Find('\n') != -1)) {
			CRichEditCtrl *pRich = (CRichEditCtrl*)GetDlgItem(IDC_RichRx);
			//���ָ�ʽ�ṹ
			CHARFORMAT2 cf;
			//ȡ���ı���ǰ���ֵĸ�ʽ
			pRich->GetSelectionCharFormat(cf);
			//ɾ���س�
			strtmp = DataTx.GetCStrData();
			strtmp.Remove('\r');
			strtmp.Remove('\n');
			//��¼��ʷ����
			cmdHistory[cmdNextPointer].Empty();
			cmdHistory[cmdNextPointer] = strtmp;
			cmdDispPointer = cmdNextPointer;
			if (++cmdNextPointer == MAX_CMD_HISTORY)
				cmdNextPointer = 0;

			strtmp.Append("\r\n");
			mUart.UnblockSend(strtmp);
			txCnt += strtmp.GetLength();

			cf.dwEffects &= ~CFE_AUTOCOLOR;
			cf.crTextColor = RGB(240, 60, 60);
			cf.dwEffects |= CFM_ITALIC | CFM_BOLD;
			//���ı���ʽ��Ϣ���õ��ı���ǰ�ı�
			cf.dwMask = CFM_BOLD | CFM_ITALIC | CFM_COLOR;


			pRich->SetSel(-1, -1);
			pRich->ReplaceSel("\r\n" + strtmp);

			if (pRich->GetLineCount() == 2)
				pRich->SetSel(0, pRich->GetTextLength()-1);//ͷ�������ж�
			else
				pRich->SetSel(pRich->LineIndex(pRich->GetLineCount() - 2) - 1, 
								pRich->LineIndex(pRich->GetLineCount() - 1));
			
			pRich->SetSelectionCharFormat(cf);//������ɫ
			pRich->SetSel(-1, 0);
			
			OnBtnClearSend();
			//ˢ�¼���
			strtmp.Format(_T("�������ݣ�%d Bytes"), txCnt);
			SetDlgItemText(IDC_GrpSend, strtmp);
		}
	}
}

//������Ϣ����
afx_msg LRESULT CUcomDlg::OnRxMsgProc(WPARAM wParam, LPARAM lParam)
{
	CString strtmp;
	if (uartPortIsOpen == true)
	{
		mUart.UnblockRead(strtmp);
		rxCnt += strtmp.GetAllocLength();
		DataRx.AppendString(strtmp, isDispHex);

		DataWatchDlg.AddItem(strtmp);
		GraphDlg.AddDataString(strtmp);
	}

	return 0;
}


void CUcomDlg::OnClose()
{
	WriteRegData();
	CDialog::OnClose();
}


void CUcomDlg::OnBnClickedBtnwinsize()
{
	static bool isLarge = true;
	static CRect rectlarge;      //����û���и�ģ���ȫ���ģ�����         
	static CRect rectsmall;      //����ʣ���С����

	if (rectlarge.IsRectEmpty())  //�����û�и�����εĴ�С��ֵ�Ļ����ڣ����и�ֵ
	{
		GetWindowRect(&rectlarge);
		CRect rectseperator;
		GetDlgItem(IDC_BtnWinSize)->GetWindowRect(&rectseperator);
		rectsmall.left = rectlarge.left;
		rectsmall.top = rectlarge.top;
		rectsmall.right = rectseperator.right + 20;
		rectsmall.bottom = rectlarge.bottom;
	}

	if (isLarge)
	{
		SetWindowPos(NULL, 0, 0, rectsmall.Width(), rectsmall.Height(), SWP_NOMOVE | SWP_NOZORDER);
		SetDlgItemText(IDC_BtnWinSize, "̽��>>");
	}
	else
	{
		SetWindowPos(NULL, 0, 0, rectlarge.Width(), rectlarge.Height(), SWP_NOMOVE | SWP_NOZORDER);
		SetDlgItemText(IDC_BtnWinSize, "����<<");
	}
	isLarge = !isLarge;
}


void CUcomDlg::SetRichLineSpace(void)
{
	CRichEditCtrl *pRich = ((CRichEditCtrl *)GetDlgItem(IDC_RichRx));

	PARAFORMAT2 pf;
	CHARFORMAT cf = { 0 };
	cf.cbSize = sizeof(cf);

	memset(&pf, 0, sizeof(PARAFORMAT2));
	pf.cbSize = sizeof(PARAFORMAT2);
	pf.dwMask = PFM_LINESPACING;
	pf.bLineSpacingRule = 4;   //0��5���ֱ��Ӧword�еĵ����оࡢ1.5���оࡢ2���оࡢ����1���趨����Сֵ���౶�о�
	
	pRich->GetDefaultCharFormat(cf);
	pf.dyLineSpacing = cf.yHeight * 14 / 10;	//����word�а���20������bLineSpacingRule����3��4��5ʱ��Ч

	pRich->SetSel(0, -1);
	pRich->SetParaFormat(pf);
	pRich->SetSel(-1, 0);
}


void CUcomDlg::OnBnClickedBtnsaverx()
{
	CFileDialog fDlg(
		false, // FALSEΪ�洢�ļ�
		NULL, // Ĭ����չ��
		"UcomRecvData.txt", // Ĭ���ļ���
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, // ����ֻ����ѡ�Ϳ����ɸ�д����
		_T("�ı��ļ�(*.txt)|*.txt||"), // ��׺����
		NULL);
	CString strtmp;
	GetDlgItemText(IDC_RichRx, strtmp);
	if (IDOK == fDlg.DoModal())
	{
		CStdioFile stdFile;
		stdFile.Open(fDlg.GetPathName(), CFile::modeCreate | CFile::modeWrite | CFile::typeBinary);
		stdFile.WriteString(strtmp);
		stdFile.Close();
	}
}


void CUcomDlg::OnSelchangeTabex(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CTabCtrl *pTab = (CTabCtrl *)GetDlgItem(IDC_TABEx);
	int sel = pTab->GetCurSel();
	*pResult = 0;

	switch (sel)
	{
	case 0:
		GraphDlg.ShowWindow(true);
		EncoderDlg.ShowWindow(false);
		DataWatchDlg.ShowWindow(false);
		MultiSendDlg.ShowWindow(false);
		break;
	case 1:
		GraphDlg.ShowWindow(false);
		EncoderDlg.ShowWindow(true);
		DataWatchDlg.ShowWindow(false);
		MultiSendDlg.ShowWindow(false);
		break; 
	case 2:
		GraphDlg.ShowWindow(false);
		EncoderDlg.ShowWindow(false);
		DataWatchDlg.ShowWindow(true);
		MultiSendDlg.ShowWindow(false);
		break;
	case 3:
		GraphDlg.ShowWindow(false);
		EncoderDlg.ShowWindow(false);
		DataWatchDlg.ShowWindow(false);
		MultiSendDlg.ShowWindow(true);
		break;
	default:
		break;
	}
}


void CUcomDlg::OnBnClickedBtnencoder()
{
	if (++encoderMode == 3)
		encoderMode = 0;
	switch (encoderMode)
	{
	case 0:
		SetDlgItemText(IDC_BtnEncoder, "  ���ձ��룺ASCII");
		break;
	case 1:
		SetDlgItemText(IDC_BtnEncoder, "  ���ձ��룺UTF-8");
		break;
	case 2:
		SetDlgItemText(IDC_BtnEncoder, "  ���ձ��룺Unicode");
		break;
	default:
		break;
	}

}

void CUcomDlg::OnOK()
{
	//�س����˳�����
	//CDialog::OnOK();
	return;
}


void CUcomDlg::OnBnClickedBtnsendfile()
{
	if (!uartPortIsOpen)
		return;
	CSendFile SendFileDlg(this, mUart.GetHandleAddr());
	SendFileDlg.DoModal();
}


BOOL CUcomDlg::PreTranslateMessage(MSG* pMsg)
{
	CEdit *pEdit= (CEdit*)GetDlgItem(IDC_EditTxData);
	//����������ѡ�񰴼��������ҽ�������Ϳ�
	if (pMsg->message == WM_KEYDOWN 
		&& GetDlgItem(IDC_EditTxData) == GetFocus())
	{
		//�����л���ʷ����
		switch (pMsg->wParam)
		{
		case VK_UP:
			SetDlgItemText(IDC_EditTxData, cmdHistory[cmdDispPointer]);
			if (--cmdDispPointer == -1)
				cmdDispPointer = MAX_CMD_HISTORY - 1;
			pEdit->SetSel(-1, -1, FALSE);
			pEdit->SetFocus();
			break;
		case VK_DOWN:
			SetDlgItemText(IDC_EditTxData, cmdHistory[cmdDispPointer]);
			if (++cmdDispPointer == MAX_CMD_HISTORY)
				cmdDispPointer = 0;
			pEdit->SetSel(-1, -1, FALSE);
			pEdit->SetFocus();
			break;
		default:break;
		}
	}

	return CDialog::PreTranslateMessage(pMsg);
}

void CUcomDlg::OnBnClickedCkbcmd()
{
	if (BST_CHECKED == IsDlgButtonChecked(IDC_CkbCMD))
	{
		if (isSendHex != true) {
			isCmdMode = TRUE;
		}
		else
		{
			//��֧��HEXģʽ��cmd�Ի�
			CButton *pCkb = (CButton *)GetDlgItem(IDC_CkbCMD);
			pCkb->SetCheck(BST_UNCHECKED);
		}
	}
	else
		isCmdMode = FALSE;
}
