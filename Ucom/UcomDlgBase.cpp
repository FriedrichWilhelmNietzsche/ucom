// UcomDlgBase.cpp : ����ʵ���ļ�
//

#include "stdafx.h"
#include "Ucom.h"
#include "UcomDlg.h"
#include "afxdialogex.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


void CUcomDlg::OnBnClickedBtnsend2()
{

}


void CUcomDlg::OnBtnSend()
{
	if (uartPortIsOpen == TRUE)
	{
		SendEditBoxData();
	}
}

//�˿��������ȿ���
void CUcomDlg::OnDropdownCbUartPort()
{
	int DropSize;
	CComboBox *pComBox = (CComboBox *)GetDlgItem(IDC_CbUartPort);
	DropSize = pComBox->GetItemHeight(0)*pComBox->GetCount();
	pComBox->SetDroppedWidth(DropSize);
}

//ѡȡ���һ��ˢ�¶���
void CUcomDlg::OnSelendokCbUartPort()
{
	CComboBox *pComBox = (CComboBox *)GetDlgItem(IDC_CbUartPort);
	int sel = pComBox->GetCurSel();
	int cnt = pComBox->GetCount();

	CString str;
	//ĩβˢ�´���
	if (sel == (cnt - 1))
	{
		//clear all
		pComBox->ResetContent();
		mUart.GetComList((CComboBox *)GetDlgItem(IDC_CbUartPort));

		//ѡȡ��һ��
		pComBox->SetCurSel(0);
	}
}

void CUcomDlg::OnBtnClearRecv()
{
	SetDlgItemText(IDC_RichRx, _T(""));
	SetRichLineSpace();
	DataRx.ClearData();
}

void CUcomDlg::OnBtnClearSend()
{
	SetDlgItemText(IDC_EditTxData, _T(""));
	DataTx.ClearData();
}


void CUcomDlg::SetFullBytes(int nbytes)
{
	CRichEditCtrl *pRich = (CRichEditCtrl *)GetDlgItem(IDC_RichRx);

	if (nbytes == -1)
	{
		if (BST_CHECKED == IsDlgButtonChecked(IDC_CkbRxFullClc))
		{
			isRxFullClc = true;
			limitBytes = GetDlgItemInt(IDC_EdbFullBytes);

		}
		else
			isRxFullClc = false;
	}
	else
		//���յ����ֽ�
		limitBytes = nbytes;
}

void CUcomDlg::OnBnClickedCkbrxfullclc()
{
	SetFullBytes();
}
void CUcomDlg::OnChangeEdbfullbytes()
{
	SetFullBytes();
}



void CUcomDlg::OnBnClickedBtnclearall()
{
	OnBtnClearRecv();
	OnBtnClearSend();
	OnBtnClearnCnt();
}


void CUcomDlg::OnBnClickedBtnhelp()
{
	CString helpInfo;
	helpInfo = 
"�汾1.03 & ������Ϣ��\n\
0.�������Ȩ���У���������ѧ����ѧ�ᣬ��������QQȺ��560901616\n\
1.�Զ����洮�����������Ϣ���û��趨�ı���ɫ������ɫ���ã�������Ĵ�������\n\
2.�������豸����ʱ��������ںţ��������˵���ѡ��ˢ�´��ڿ���ɨ��˿ڱ䶯\n\
3.��ȫ֧�ֽ�����UTF-8��Unicode�ı�����ʾ\n\
4.�������������Կ��ٵ��ó��õĹ��ߺ�ϵͳ������\n\
5.�ṩ����ͳ�ƺ�ʵʱ��������������ʾ��������յȹ���\n\
6.�ṩHEX������ʾ��HEXģʽ���ͣ����Բ��淶�����������\n\
7.������գ�����ѡ����ʱ�򣬵������ﵽ���ڵ���ֵ�������趨ֵ(Byte)ʱ���Զ����\n\
8.������ͣ�����ѡ����ʱ�򣬵������ﵽ���ڵ���ֵ�������趨ֵ(  ms)ʱ���Զ�����\n\
9.��֡���У�����ѡ����ʱ���Զ�Ϊ���Ϳ������ĩβ��ӻس����з�\n\
10.��ͣ���գ���ͣ���ݵ���ʾ������������Ȼ�ں�̨���գ�ֻ�ǲ���ʾ\n\
11.������գ����ڱ��浱ǰ���տ�����ı���ϢΪTXT�ļ�\n\
12.����ɫ������ɫ��������ʽ�����Զ��ƽ��ܿ����ʽ��������ʽ��Ϣ���ᱣ��\n\
13.͸�����䣬�����谭0x00,0x11���ֽڵķ��ͽ���\n\
14.֧���ļ����ͣ����鲻Ҫ����̫����ļ�\n\
15.֧��CMDģʽ���س���������(���ݰ����س�)�����͵�����ͬʱ�������ʽ��������ʾ����\n\
     �������µ�����ʷ����(����)\n\
16.֧�ֺ��򴰿ڴ�С����\n\n\
a.����ͼ��֧��ָ����ʽ����ֵ�ּ𡢺��Ժ��ͼ�λ���ʾ\n\
b.�����ѯ���ṩ���ֳ��õ��ַ������16���Ʊ����ѯ���ɱ任��ʾ���\n\
c.����ע�ͣ��ܹ����Ӵ�������֡��ʱ�䵽���뼶����������Զ�������������Ŀ��ϸ��ʾ\n\
d.�������֣����ʮ����Ŀ���ͣ���ʱ���ַ����Զ����У�hexģʽ��������ť����\n\
   ��������ѡ����Ӧ����Ŀ�����Ե�����ͽ��·��༭��ĳ��ı�������ָ���༭��\n\
   ѡ�񰴼������������·�������°�ť��ʾ�ַ������ɴ�����Ӧ���ݵķ���";

	SetDlgItemText(IDC_RichRx, helpInfo);
}


void CUcomDlg::SetDelaySend(void)
{
	if (BST_CHECKED == IsDlgButtonChecked(IDC_CkbSendOnTime))
	{
		int time = GetDlgItemInt(IDC_EdbSendDelay);
		//����ѡ��
		if (time <= 0 || uartPortIsOpen == FALSE)
		{
			CButton *pCkb = (CButton *)GetDlgItem(IDC_CkbSendOnTime);
			pCkb->SetCheck(BST_UNCHECKED);
			return;
		}

		SetTimer(AUTO_SEND_TIMER_ID, time, NULL);
	}
	else
	{
		KillTimer(AUTO_SEND_TIMER_ID);
	}
}

//�ı䷢�ͼ������״̬�����趨
void CUcomDlg::OnCkbSendOnTime()
{
	SetDelaySend();
}
void CUcomDlg::OnChangeEdbSendDelay()
{
	SetDelaySend();
}

void CUcomDlg::OnCkbFrameNewLine()
{
	if (BST_CHECKED == IsDlgButtonChecked(IDC_CkbFrameNewLine))
		isNewLineSend = TRUE;
	else
		isNewLineSend = FALSE;
}

void CUcomDlg::OnCkbDispHex()
{
	if (BST_CHECKED == IsDlgButtonChecked(IDC_CkbDispHex))
		isDispHex = TRUE;
	else
		isDispHex = FALSE;
}

//��ͣ������ʾ��Ϣ������
void CUcomDlg::OnBnClickedBtnisdisprx()
{
	isDispRx = !isDispRx;
	if (isDispRx)
		SetDlgItemText(IDC_BtnIsDispRx, "��ͣ����");
	else
		SetDlgItemText(IDC_BtnIsDispRx, "��������");
}


void CUcomDlg::OnBnClickedBtntoolbox()
{
	CMenu menu;
	RECT rect;
	menu.LoadMenu(IDR_MENU_ToolsBox);
	//��ȡ�ؼ�λ��
	::GetWindowRect(GetDlgItem(IDC_BtnToolBox)->GetSafeHwnd(), &rect);
	CMenu *popmenu = menu.GetSubMenu(0);

	popmenu->TrackPopupMenu(TPM_RIGHTBUTTON, rect.right, rect.top, this);
}


void CUcomDlg::OnMeudevmanger()
{
	::ShellExecute(m_hWnd, "open", "devmgmt.msc", NULL, NULL, SW_SHOWNORMAL);
}
void CUcomDlg::OnMeucalc()
{
	::ShellExecute(m_hWnd, "open", "calc", NULL, NULL, SW_SHOWNORMAL);
}
void CUcomDlg::OnMeunotepad()
{
	::ShellExecute(m_hWnd, "open", "notepad", NULL, NULL, SW_SHOWNORMAL);
}

void CUcomDlg::OnMenureg()
{
	::ShellExecute(m_hWnd, "open", "regedit", NULL, NULL, SW_SHOWNORMAL);
}
void CUcomDlg::OnMenucmd()
{
	::ShellExecute(m_hWnd, "open", "cmd", NULL, NULL, SW_SHOWNORMAL);
}
void CUcomDlg::OnMenuserv()
{
	::ShellExecute(m_hWnd, "open", "services.msc", NULL, NULL, SW_SHOWNORMAL);
}



void CUcomDlg::OnBnClickedBtnbackcolor()
{
	CRichEditCtrl *pRich = ((CRichEditCtrl *)GetDlgItem(IDC_RichRx));

	CHARFORMAT cf = { 0 };
	cf.cbSize = sizeof(cf);
	//ȡ���ı���ǰ���ֵĸ�ʽ 
	pRich->GetSelectionCharFormat(cf);
	//������ɫ�Ի���,����ȡ�õ����ָ�ʽ��ʼ���Ի��� 
	CColorDialog dlg(cf.crTextColor, CC_FULLOPEN, this);
	//��ʾ�Ի���,ѡ����ɫ 
	if (dlg.DoModal() == IDOK)
	{
		//ȡ���û���ѡ��ɫ 
		cf.crTextColor = dlg.GetColor();
		//���ı���ʽ��Ϣ���õ��ı���ǰ�ı� 
		cf.dwMask = CFM_COLOR;
		cf.dwEffects = 0;
		backgroudColor = cf.crTextColor;
		pRich->SetBackgroundColor(FALSE, cf.crTextColor);
	}
}


void CUcomDlg::OnBnClickedBtnfontcolor()
{
	CRichEditCtrl *pRich = ((CRichEditCtrl *)GetDlgItem(IDC_RichRx));

	CHARFORMAT cf = { 0 };
	cf.cbSize = sizeof(cf);
	//ȡ���ı���ǰ���ֵĸ�ʽ 
	pRich->GetSelectionCharFormat(cf);
	//������ɫ�Ի���,����ȡ�õ����ָ�ʽ��ʼ���Ի��� 
	CColorDialog dlg(cf.crTextColor, CC_FULLOPEN, this);
	//��ʾ�Ի���,ѡ����ɫ 
	if (dlg.DoModal() == IDOK)
	{
		//ȡ���û���ѡ��ɫ 
		cf.crTextColor = dlg.GetColor();
		//���ı���ʽ��Ϣ���õ��ı���ǰ�ı� 
		cf.dwMask = CFM_COLOR;
		cf.dwEffects = 0;
		pRich->SetDefaultCharFormat(cf);
	}
}


void CUcomDlg::OnBnClickedBtnfont()
{
	CRichEditCtrl *pRich = ((CRichEditCtrl *)GetDlgItem(IDC_RichRx));

	//���岢��ʼ�����ָ�ʽ�ṹ
	CHARFORMAT cf = { 0 };
	cf.cbSize = sizeof(cf);
	//ȡ���ı���ǰ���ֵĸ�ʽ
	pRich->GetSelectionCharFormat(cf);
	//��������Ի���,����ȡ�õ����ָ�ʽ��ʼ���Ի���
	CFontDialog dlg(cf);
	//��ʾ�Ի���,ѡ��������Ϣ
	if (dlg.DoModal() == IDOK)
	{
		//ȡ���û���ѡ������Ϣ
		dlg.GetCharFormat(cf);
		//���ı���ʽ��Ϣ���õ��ı���ǰ�ı�
		cf.dwMask = CFM_BOLD | CFM_COLOR | CFM_FACE | CFM_ITALIC | CFM_SIZE | CFM_UNDERLINE;
		pRich->SetDefaultCharFormat(cf);

		SetRichLineSpace();
	}
}
