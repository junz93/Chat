
// ChatDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Chat.h"
#include "ChatDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CChatDlg �Ի���



CChatDlg::CChatDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CChatDlg::IDD, pParent)
	, m_Type(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CChatDlg::~CChatDlg()
{
	if (m_socket)
		closesocket(m_socket);
}

void CChatDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO1, m_Type);
}

BEGIN_MESSAGE_MAP(CChatDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
//	ON_MESSAGE(WM_RECVDATA, OnRecvData)
	ON_MESSAGE(WM_SOCK, OnSock)
	ON_BN_CLICKED(IDC_SEND, &CChatDlg::OnBnClickedSend)
	ON_BN_CLICKED(IDC_RADIO1, &CChatDlg::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO2, &CChatDlg::OnBnClickedRadio2)
END_MESSAGE_MAP()


// CChatDlg ��Ϣ�������

BOOL CChatDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO:  �ڴ���Ӷ���ĳ�ʼ������
	InitSocket();
	((CIPAddressCtrl*)GetDlgItem(IDC_IPADDRESS1))->SetAddress(127, 0, 0, 1);
			
	//RECVPARAM *pRecvParam = new RECVPARAM;
	//pRecvParam->hwnd = m_hWnd;
	//pRecvParam->sock = m_socket;
	//HANDLE hThread = CreateThread(NULL, 0, RecvProc, (LPVOID)pRecvParam, 0, NULL);
	//CloseHandle(hThread);

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CChatDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CChatDlg::OnPaint()
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
		CDialogEx::OnPaint();
	}
	UpdateData(TRUE);
	if (m_Type == 0)
	{
		GetDlgItem(IDC_EDITHOSTNAME)->EnableWindow(TRUE);
		GetDlgItem(IDC_IPADDRESS1)->EnableWindow(FALSE);
	}
	else if (m_Type == 1)
	{
		GetDlgItem(IDC_EDITHOSTNAME)->EnableWindow(FALSE);
		GetDlgItem(IDC_IPADDRESS1)->EnableWindow(TRUE);
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CChatDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//��ʼ���׽���
BOOL CChatDlg::InitSocket()
{
	m_socket = WSASocket(AF_INET, SOCK_DGRAM, 0, NULL, 0, 0);
	if (m_socket == INVALID_SOCKET)
	{
		MessageBox(_T("�����׽���ʧ�ܣ�"), "����", MB_OK | MB_ICONERROR);
		return FALSE;
	}
	SOCKADDR_IN addrSock;
	addrSock.sin_family = AF_INET;
	addrSock.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	addrSock.sin_port = htons(8888);
	
	if (bind(m_socket, (SOCKADDR*)&addrSock, sizeof(SOCKADDR)) == SOCKET_ERROR)
	{
		closesocket(m_socket);
		MessageBox(_T("���׽���ʧ�ܣ�"), "����", MB_OK | MB_ICONERROR);
		return FALSE;
	}
	if (WSAAsyncSelect(m_socket, m_hWnd, WM_SOCK, FD_READ) == SOCKET_ERROR)
	{
		MessageBox(_T("ע�������ȡ�¼�ʧ��"), "����", MB_OK | MB_ICONERROR);
		return FALSE;
	}

	return TRUE;
}

//�߳���ں���
//DWORD WINAPI CChatDlg::RecvProc(LPVOID lpParameter)
//{
//	HWND hwnd = ((RECVPARAM*)lpParameter)->hwnd;
//	SOCKET sock = ((RECVPARAM*)lpParameter)->sock;
//	delete lpParameter;
//
//	SOCKADDR_IN addrFrom;
//	int len = sizeof(SOCKADDR);
//	char recvBuff[200];
//	char tempBuff[400];
//	int err;
//	while (1)
//	{
//		err = recvfrom(sock, recvBuff, sizeof(recvBuff), 0,
//			(SOCKADDR*)&addrFrom, &len);
//		if (err == SOCKET_ERROR)
//			break;
//		CTime tm = CTime::GetCurrentTime();
//		CString str_time = tm.Format("%X");
//		sprintf_s(tempBuff, "%s  "+str_time+"��\r\n%s", inet_ntoa(addrFrom.sin_addr), recvBuff);
//		::PostMessage(hwnd, WM_RECVDATA, 0, (LPARAM)tempBuff);
//	}
//
//	return 0;
//}

//LRESULT CChatDlg::OnRecvData(WPARAM wParam, LPARAM lParam)
//{
//	CString strTemp((LPCTSTR)lParam);
//	CString str;
//	GetDlgItemText(IDC_EDITRECV, str);
//	if (!str.IsEmpty())
//		str += "\r\n";
//	str += strTemp;
//	SetDlgItemText(IDC_EDITRECV, str);
//	
//	return 0;
//}

LRESULT CChatDlg::OnSock(WPARAM wParam, LPARAM lParam)
{
	switch (LOWORD(lParam))
	{
	case FD_READ:
		WSABUF wsabuf;
		wsabuf.buf = new char[200];
		wsabuf.len = 200;
		DWORD dwBytes;
		DWORD dwFlags = 0;
		SOCKADDR_IN addrFrom;
		int len = sizeof(SOCKADDR);		
		if (WSARecvFrom(m_socket, &wsabuf, 1, &dwBytes, &dwFlags,
			(SOCKADDR*)&addrFrom, &len, NULL, NULL) == SOCKET_ERROR)
		{
			MessageBox(_T("������Ϣʧ�ܣ�"), "����", MB_OK | MB_ICONERROR);
			delete[] wsabuf.buf;
			return -1;
		}
		//HOSTENT *pHost;
		//pHost = gethostbyaddr((char*)&addrFrom.sin_addr.S_un.S_addr, 4, AF_INET);
		CTime tm = CTime::GetCurrentTime();
		CString str_time = tm.Format("%X");
		CString str, strTemp;
		//if (m_Type==0)
		//	strTemp.Format("%s  "+str_time+"��\r\n%s", pHost->h_name, wsabuf.buf);
		//else if (m_Type==1)
			strTemp.Format("[%s]  "+str_time+"��\r\n%s", 
				inet_ntoa(addrFrom.sin_addr),wsabuf.buf);
		GetDlgItemText(IDC_EDITRECV, str);
		if (!str.IsEmpty())
			str += "\r\n";
		str += strTemp;
		SetDlgItemText(IDC_EDITRECV, str);
		CEdit *pEdit = (CEdit*)GetDlgItem(IDC_EDITRECV);
		pEdit->LineScroll(pEdit->GetLineCount());
		delete[] wsabuf.buf;
		break;
	}

	return 0;
}

void CChatDlg::OnBnClickedSend()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	DWORD dwIP;
	CString strHostName;
	HOSTENT *pHost;
	((CIPAddressCtrl*)GetDlgItem(IDC_IPADDRESS1))->GetAddress(dwIP);
	GetDlgItemText(IDC_EDITHOSTNAME, strHostName);
	SOCKADDR_IN addrTo;	
	addrTo.sin_family = AF_INET;
	addrTo.sin_port = htons(8888);
	switch (m_Type)
	{
	case 0:
		pHost = gethostbyname(strHostName);
		if (!pHost)
		{
			MessageBox("�Ҳ���������", "����", MB_OK | MB_ICONERROR);
			return;
		}
		addrTo.sin_addr.S_un.S_addr = *((DWORD*)pHost->h_addr_list[0]);
		break;
	case 1:		
		addrTo.sin_addr.S_un.S_addr = htonl(dwIP);		
		break;
	}

	WSABUF wsabuf;
	DWORD dwBytes;
	int len;
	CString strSend;	
	GetDlgItemText(IDC_EDITSEND, strSend);
	len = strSend.GetLength();
	wsabuf.buf = strSend.GetBuffer(len);
	wsabuf.len = len + 1;

	if (WSASendTo(m_socket, &wsabuf, 1, &dwBytes, 0,
		(SOCKADDR*)&addrTo, sizeof(SOCKADDR), NULL, NULL) == SOCKET_ERROR)
	{
		MessageBox(_T("������Ϣʧ�ܣ�"), "����", MB_OK | MB_ICONERROR);
		return;
	}

	SetDlgItemText(IDC_EDITSEND, _T(""));
	CString str;
	GetDlgItemText(IDC_EDITRECV, str);
	if (!str.IsEmpty())
		str += "\r\n";
	CTime tm = CTime::GetCurrentTime();
	CString str_time = tm.Format("%X");
	str += "[127.0.0.1]  "+str_time+"��\r\n" + strSend;
	SetDlgItemText(IDC_EDITRECV, str);
	CEdit *pEdit = (CEdit*)GetDlgItem(IDC_EDITRECV);
	pEdit->LineScroll(pEdit->GetLineCount());
	GetDlgItem(IDC_EDITSEND)->SetFocus();
}


void CChatDlg::OnBnClickedRadio1()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	Invalidate();	
}


void CChatDlg::OnBnClickedRadio2()
{
	//TODO:  �ڴ���ӿؼ�֪ͨ����������
	Invalidate();
}
