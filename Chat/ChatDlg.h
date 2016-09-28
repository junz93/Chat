
// ChatDlg.h : ͷ�ļ�
//

#pragma once
#define WM_RECVDATA WM_USER+1
#define WM_SOCK WM_USER+2

// CChatDlg �Ի���
class CChatDlg : public CDialogEx
{
// ����
public:
	CChatDlg(CWnd* pParent = NULL);	// ��׼���캯��
	virtual ~CChatDlg();
// �Ի�������
	enum { IDD = IDD_CHAT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
//	afx_msg LRESULT OnRecvData(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSock(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
private:
	SOCKET m_socket;
public:
	BOOL InitSocket();
//	static DWORD WINAPI RecvProc(LPVOID lpParameter);
	afx_msg void OnBnClickedSend();
	int m_Type;	// ���ͨ����������IP��ַ������Ϣ
	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedRadio2();
};

//struct RECVPARAM
//{
//	HWND hwnd;
//	SOCKET sock;	
//};
