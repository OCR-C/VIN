
// HyperLPRDlg.h : ͷ�ļ�
//

#pragma once
#include "Pipeline.h"
#include "puttext.h"

// CHyperLPRDlg �Ի���
class CHyperLPRDlg : public CDialogEx
{
// ����
public:
	CHyperLPRDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_HYPERLPR_DIALOG };

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
