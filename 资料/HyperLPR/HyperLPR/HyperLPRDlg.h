
// HyperLPRDlg.h : 头文件
//

#pragma once
#include "Pipeline.h"
#include "puttext.h"

// CHyperLPRDlg 对话框
class CHyperLPRDlg : public CDialogEx
{
// 构造
public:
	CHyperLPRDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_HYPERLPR_DIALOG };

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
	DECLARE_MESSAGE_MAP()
public:
	pr::PipelinePR* psrc;
	Mat m_img,m_detectImg;
	POINT m_old;
	CString m_path;
	CString m_result;


	void Resize();
	void ShowImage(Mat img,UINT ID);
	afx_msg void OnBnClickedBtnOpen();
	afx_msg void OnBnClickedBtnDetect();
	afx_msg void OnEnSetfocusEditResult();
	afx_msg void OnSize(UINT nType, int cx, int cy);
};
