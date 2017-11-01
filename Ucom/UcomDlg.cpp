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
, DataRx(), DataTx()
, MultiSendDlg(this, &xAsyncSend, &xIsOpen)
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
	ON_BN_CLICKED(IDC_BtnSend, OnBtnSend)
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
	ON_REGISTERED_MESSAGE(WM_MYONRECVMSG, &CUcomDlg::OnMyReceiveMsg)
//	ON_WM_CHAR()
ON_BN_CLICKED(IDC_CkbCMD, &CUcomDlg::OnBnClickedCkbcmd)
ON_WM_SIZE()
ON_WM_GETMINMAXINFO()
ON_NOTIFY(TCN_SELCHANGE, IDC_TABSrc, &CUcomDlg::OnSelchangeTabsrc)
END_MESSAGE_MAP()


// CUcomDlg ��Ϣ�������

BOOL CUcomDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	CRect rec;

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO:  �ڴ���Ӷ���ĳ�ʼ������
	SetWindowText(_T("Ucom v1.04 �ᴮ�� ��������ѧ����ѧ��"));

	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon


#ifdef USING_CONSLOE
	InitDebugConsole();
#endif
	isNewLineSend = false;
	isDispHex = false;
	isSendHex = false;
	isDispRx = true;
	isCmdMode = false;
	encoderMode = 0;
	rxCnt = 0;
	txCnt = 0;

	InitTabEx();
	InitTabSrc();

	SetDlgItemInt(IDC_EdbSendDelay, 1000);
	SetDlgItemInt(IDC_EdbFullBytes, 5120);

	LoadRegConfig();
	SetRichLineSpace();

	cmdNextPointer = 0;
	cmdDispPointer = 0;


	
	//��ȡ��չ������
	CWnd *pWnd = GetDlgItem(IDC_TABEx);
	pWnd->GetWindowRect(&rectEx);
	ScreenToClient(&rectEx);   //���ؼ���Сת��Ϊ�ڶԻ����е���������
	
	GetDlgItem(IDC_GrpRecv)->GetWindowRect(&rec);
	ScreenToClient(&rec);
	//���ο��20
	//rectEx.left-=20;
	rectEx.left = (rec.right+ rectEx.left) / 2-5;


	LargerMode = 0;
	//�޸�״ֵ̬�˹�����������Ϊ����ģʽ
	isLarge = false;
	OnBnClickedBtnwinsize();

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


//��ʼ��ע�������
bool CUcomDlg::InitRegData(void)
{
		return true;
}

//д��ע�������
void CUcomDlg::WriteRegData(void)
{
	CHARFORMAT cf;
	CRichEditCtrl *pRich = ((CRichEditCtrl *)GetDlgItem(IDC_RichRx));
	pRich->GetSelectionCharFormat(cf);
	AfxGetApp()->WriteProfileInt("Config", "TextColor", cf.crTextColor);
	//û�л�ȡ����ɫ�ķ���ֻ���ñ�����¼
	AfxGetApp()->WriteProfileInt("Config", "GroundColor", backgroudColor);

	TRACE("Color Reg Finish %d\n", backgroudColor);
}

void CUcomDlg::LoadRegConfig()
{
	CRichEditCtrl *pRich = (CRichEditCtrl*)GetDlgItem(IDC_RichRx);
	UINT tmp = AfxGetApp()->GetProfileInt("Config", "TextColor", 0xFF000000);
	CHARFORMAT cf = { 0 };
	cf.cbSize = sizeof(cf);;
	cf.dwMask = CFM_COLOR;
	cf.dwEffects = 0;

	if (tmp == 0xFF000000)
	{
		//��������Ĭ������
		backgroudColor = RGB(242, 241, 215);
		cf.crTextColor = RGB(0, 0, 0);

		pRich->SetDefaultCharFormat(cf);
		pRich->SetBackgroundColor(false, backgroudColor);
		//�����������������Ϣ
		OnBnClickedBtnhelp();
	}
	else
	{
		//��ɫ����
		cf.crTextColor = AfxGetApp()->GetProfileInt("Config", "TextColor", RGB(0, 0, 0));
		pRich->SetDefaultCharFormat(cf);
		backgroudColor = AfxGetApp()->GetProfileInt("Config", "GroundColor", RGB(242, 241, 215));
		pRich->SetBackgroundColor(false, backgroudColor);
	}
}

void CUcomDlg::SendEditBoxData(void)
{
	CString tmpStr("\r\n");
	//����
	if (isNewLineSend == TRUE) {
		xAsyncSend(DataTx.GetCStrData() + tmpStr);
		txCnt = txCnt+1+ DataTx.GetLength();
	}
	else {
		xAsyncSend(DataTx.GetCStrData());
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

#include <afxsock.h>
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


	// SOCK_DGRAM->UDP, SOCK_STREAM->TCP
	
	// һ��Ҫ��ʼ��
	AfxSocketInit();
	CAsyncSocket socketClient;
	socketClient.Create();
	int nPort = 8888;
	LPCTSTR sIp = "10.210.83.162";
	CString strText = "123\n";

	if (socketClient.Connect(sIp, nPort))
		AfxMessageBox("����ѧ�Ỷӭ��");
	socketClient.Send(strText, strText.GetLength());
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
		if (isCmdMode && xIsOpen() && (strtmp.Find('\n') != -1)) {
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
			xAsyncSend(strtmp);
			
			txCnt += strtmp.GetLength();

			cf.dwEffects &= ~CFE_AUTOCOLOR;
			cf.crTextColor = RGB(240, 60, 60);
			cf.dwEffects |= CFM_ITALIC | CFM_BOLD;
			//���ı���ʽ��Ϣ���õ��ı���ǰ�ı�
			cf.dwMask = CFM_BOLD | CFM_ITALIC | CFM_COLOR;

			int preLen = pRich->GetWindowTextLengthA();
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

// ȫ����Ϣ
afx_msg LRESULT CUcomDlg::OnMyReceiveMsg(WPARAM wParam, LPARAM lParam)
{
	CString strtmp;
	static CString lastClient;
	cout << hex <<"On Main Msg:" << wParam << ',' << lParam << dec << endl;
	if ((wParam&WL_MASK) == WL_UCOM_RECV  && xIsOpen() == true)
	{
		if ((wParam&WH_MASK) == WH_UCOM_SUBNET) {
			nSocketPara *sParam = (nSocketPara *)lParam;
			((nSocket *)sParam->hSocket)->UnblockRead(strtmp);
			if (sParam->strIP != lastClient) {
				CString strNew = "\n��from: " + sParam->strIP + "��\n";
				DataRx.AppendString(strNew);
			}
			lastClient = sParam->strIP;
		}
		else
		{
			xAsyncRead(strtmp);
		}
		cout << "read:" << strtmp << endl;
		DataWatchDlg.AddItem(strtmp);
		// ������ʾ����ˢ������
		rxCnt += strtmp.GetAllocLength();
		DataRx.AppendString(strtmp, isDispHex);
		GraphDlg.AddDataString(strtmp);
	}
	else if ((wParam&WL_MASK) == WL_UCOM_OPEN)
	{ 
		if (!IsOpenX()) {
			SetDelaySend();
			//�趨���տ��ı�ˢ��ʱ�� ����24Hz
			SetTimer(FLASH_RX_EDITBOX_TIMER_ID, rxFlashPeriod, NULL);
		}
	}
	else if ((wParam&WL_MASK) == WL_UCOM_CLOSE)
	{
		if (IsOpenX()) {
			KillTimer(AUTO_SEND_TIMER_ID);//�Զ����͹رն�ʱ��
			KillTimer(FLASH_RX_EDITBOX_TIMER_ID);//�رս�����ʾ��ʱ��
		}
	}
	//else if (wParam == W_SUBNET_RECV)
	//{

	//	//xAsyncRead(strtmp);
	//	cout << "read:" << strtmp << endl;
	//	rxCnt += strtmp.GetAllocLength();
	//	DataRx.AppendString(strNew, isDispHex);

	//	DataWatchDlg.AddItem(strtmp);
	//	GraphDlg.AddDataString(strtmp);
	//}
	return 0;
}

/* ������Ϣ����
 * ͨ��������Ϣ������
*/
//
//afx_msg LRESULT CUcomDlg::OnRxMsgProc(WPARAM wParam, LPARAM lParam)
//{
//	CString strtmp;
//
//	if (xIsOpen() == true)
//	{
//		xAsyncSend(strtmp);
//		rxCnt += strtmp.GetAllocLength();
//		DataRx.AppendString(strtmp, isDispHex);
//
//		DataWatchDlg.AddItem(strtmp);
//		GraphDlg.AddDataString(strtmp);
//	}
//
//	return 0;
//}


void CUcomDlg::OnClose()
{
	WriteRegData();
	UartDlg.WriteRegData();
	NetDlg.WriteRegData();
	CDialog::OnClose();
}

//��չ��ť�ص�����
void CUcomDlg::OnBnClickedBtnwinsize()
{  
	CRect rectOr;
	GetWindowRect(&rectOr);

	isLarge = !isLarge;
	if (isLarge)
	{
		LargerMode = 2;
		SetWindowPos(NULL, 0, 0, rectOr.Width()- rectEx.Width(), rectOr.Height(), SWP_NOMOVE | SWP_NOZORDER);
		SetDlgItemText(IDC_BtnWinSize, "̽��>>");
	}
	else
	{
		LargerMode = 1;
		SetWindowPos(NULL, 0, 0, rectOr.Width() + rectEx.Width(), rectOr.Height(),SWP_NOMOVE | SWP_NOZORDER);
		SetDlgItemText(IDC_BtnWinSize, "����<<");
	}

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

void CUcomDlg::InitTabSrc(void)
{
	CRect rect;
	CTabCtrl *pTab = (CTabCtrl *)GetDlgItem(IDC_TABSrc);
	pTab->InsertItem(0, "����");
	pTab->InsertItem(1, "����");
	//pTab->GetWindowRect(&rect);
	//ScreenToClient(&rect);
	pTab->GetClientRect(&rect);
	//cout << rect.top << ',' << rect.left << endl;
	rect.top += 28;
	rect.left += 2;
	UartDlg.Create(IDD_UART, pTab);
	UartDlg.MoveWindow(&rect);
	UartDlg.ShowWindow(true);

	pTab->GetClientRect(&rect);
	rect.top += 28;
	rect.left += 2;
	NetDlg.Create(IDD_NET, pTab);
	NetDlg.MoveWindow(&rect);
	NetDlg.ShowWindow(false);

	SwitchCurDataSrc(WH_UCOM_UART);
}

typedef  bool (UcomBase::*IsOpenY)(void);
void CUcomDlg::SwitchCurDataSrc(int dataSrc)
{
	IsOpenY openy = &UcomBase::isOpen;
	(NetDlg.*openy)();
	switch (dataSrc)
	{
	case WH_UCOM_UART:
		curDataSrc = WH_UCOM_UART;
		xIsOpen = UartDlg.isOpen;
		xAsyncSend = UartDlg.AsyncSend;
		xAsyncRead = UartDlg.AsyncRead;
		break;
	case WH_UCOM_NET:
		curDataSrc = WH_UCOM_UART;
		//xIsOpen = NetDlg.isOpen;
		//xAsyncSend = NetDlg.AsyncSend;
		//xAsyncRead = NetDlg.AsyncRead;
		//xAsyncRead = &CNetDlg::VirtualSend;
		
		//xIsOpen = NetDlg.isOpen;
		//xAsyncSend = NetDlg.AsyncSend;
		//xAsyncRead = NetDlg.AsyncRead;
	default:
		break;
	}
}


//�л�ѡ�
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

// ����Դtab�л�
void CUcomDlg::OnSelchangeTabsrc(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CTabCtrl *pTab = (CTabCtrl *)GetDlgItem(IDC_TABSrc);
	int sel = pTab->GetCurSel();
	*pResult = 0;

	switch (sel)
	{
	case 0:
		UartDlg.ShowWindow(true);
		NetDlg.ShowWindow(false);
		SwitchCurDataSrc(WH_UCOM_UART);
		break;
	case 1:
		UartDlg.ShowWindow(false);
		NetDlg.ShowWindow(true);
		SwitchCurDataSrc(WH_UCOM_NET);
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

//�س����˳�����
void CUcomDlg::OnOK()
{
	//CDialog::OnOK();
	return;
}


void CUcomDlg::OnBnClickedBtnsendfile()
{
	if (!xIsOpen())
		return;
	CSendFile SendFileDlg(this, &xAsyncSend);
	SendFileDlg.DoModal();
}


BOOL CUcomDlg::PreTranslateMessage(MSG* pMsg)
{
	CEdit *pEdit= (CEdit*)GetDlgItem(IDC_EditTxData);
	int len;
	//����������ѡ�񰴼��������ҽ�������Ϳ�
	if (pMsg->message == WM_KEYDOWN 
		&& GetDlgItem(IDC_EditTxData) == GetFocus())
	{
		// ATģʽ�ĸ߼�����
		if (isCmdMode) {
			//�����л���ʷ����
			switch (pMsg->wParam)
			{
				case VK_UP:
					SetDlgItemText(IDC_EditTxData, cmdHistory[cmdDispPointer]);
					if (--cmdDispPointer == -1)
						cmdDispPointer = MAX_CMD_HISTORY - 1;
					len = pEdit->GetWindowTextLengthA();
					pEdit->SetSel(len, len);
					pEdit->SetFocus();
					break;
				case VK_DOWN:
					SetDlgItemText(IDC_EditTxData, cmdHistory[cmdDispPointer]);
					if (++cmdDispPointer == MAX_CMD_HISTORY)
						cmdDispPointer = 0;
					len = pEdit->GetWindowTextLengthA();
					pEdit->SetSel(len, len);
					pEdit->SetFocus();
					break;
				case VK_TAB:
					pEdit->SetSel(0, 0);
					pEdit->ReplaceSel("AT+");
					len = pEdit->GetWindowTextLengthA();
					pEdit->SetSel(len, len);
					pEdit->SetFocus();
					return true;
				default:break;
				}
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

//�ؼ�IDΪnID���Ҹı���״��isEnlarge=true����������x(��Ϊ����)���أ����������ұ߽��ƶ�
void CUcomDlg::ChangeItemSize(int nID, int x,int y,bool isEnlarge)
{
	CWnd *pWnd;
	pWnd = GetDlgItem(nID);
	if (pWnd != NULL)
	{
		CRect rec;
		int width,height;
		pWnd->GetWindowRect(&rec);  //��ȡ�ؼ��仯ǰ�Ĵ�С
		ScreenToClient(&rec);   //���ؼ���Сת��Ϊ�ڶԻ����е���������
		width = rec.Width();
		height = rec.Height();
		if (isEnlarge) {
			rec.right = rec.right + x;
			rec.bottom = rec.bottom + y;
		}else{
			rec.left = rec.left + x;
			rec.top = rec.top + y;

			rec.right = rec.left + width;
			rec.bottom = rec.top + height;
		}
		pWnd->MoveWindow(rec);//�����ؼ�
	}
}

void CUcomDlg::OnSize(UINT nType, int cx, int cy)
{
	static int lastCx=0, lastCy=0;

	CDialog::OnSize(nType, cx, cy);
	if (LargerMode) {
		lastCx = cx;
		LargerMode = 0;
		return;
	}
	
	if (nType == SIZE_RESTORED)
	{
		int dX = cx - lastCx;
		//int dY = cy - lastCy;
		int dY = 0;//��ֹ���µ���

		ChangeItemSize(IDC_RichRx, dX, dY, true);
		ChangeItemSize(IDC_EditTxData, dX, dY, true);
		ChangeItemSize(IDC_GrpSend, dX, dY, true);
		ChangeItemSize(IDC_GrpRecv, dX, dY, true);

		ChangeItemSize(IDC_TABEx, dX, dY, false);

		ChangeItemSize(IDC_CkbCMD, dX, dY, false);
		ChangeItemSize(IDC_BtnSendFile, dX, dY, false);
		ChangeItemSize(IDC_BtnToolBox, dX, dY, false);
		ChangeItemSize(IDC_BtnHelp, dX, dY, false);
		ChangeItemSize(IDC_Launch, dX, dY, false);
		ChangeItemSize(IDC_BtnWinSize, dX, dY, false);
		
		//step=1;
	}
	//������С������
	if (cx!=0)
		lastCx = cx;
}


void CUcomDlg::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	//ǿ�ƴ��ڴ�С����ֵ,��Ҫʹ�þ��Դ�С�����м���������
	lpMMI->ptMinTrackSize.y = rectEx.Height() + 55;
	lpMMI->ptMaxTrackSize.y = lpMMI->ptMinTrackSize.y;//��ֹ���µ���

	if (isLarge)
	{
		lpMMI->ptMinTrackSize.x = 500;
	}
	else
	{
		lpMMI->ptMinTrackSize.x = 500+rectEx.Width();
	}
	CDialog::OnGetMinMaxInfo(lpMMI);
}
