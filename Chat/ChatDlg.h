
// ChatDlg.h : 头文件
//

#pragma once
#define WM_RECVDATA WM_USER+1
#define WM_SOCK WM_USER+2

// CChatDlg 对话框
class CChatDlg : public CDialogEx
{
// 构造
public:
	CChatDlg(CWnd* pParent = NULL);	// 标准构造函数
	virtual ~CChatDlg();
// 对话框数据
	enum { IDD = IDD_CHAT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
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
	int m_Type;	// 标记通过主机名或IP地址发送信息
	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedRadio2();
};

//struct RECVPARAM
//{
//	HWND hwnd;
//	SOCKET sock;	
//};
