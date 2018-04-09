
// HyperLPRDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "HyperLPR.h"
#include "HyperLPRDlg.h"
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


// CHyperLPRDlg �Ի���
std::vector<std::string> chars_code{ "��", "��", "��", "��", "��", "��", "��", "��", "��", "��", "��", "��", "��", "��", "��", "³", "ԥ", "��", "��", "��", "��", "��", "��", "��", "��", "��", "��", "��", "��", "��", "��", "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "A", "B", "C", "D", "E", "F", "G", "H", "J", "K", "L", "M", "N", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z" };



CHyperLPRDlg::CHyperLPRDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CHyperLPRDlg::IDD, pParent)
	, m_path(_T(""))
	, m_result(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON1);
	psrc = new pr::PipelinePR("../model/cascade.xml",
		"../model/HorizonalFinemapping.prototxt", "../model/HorizonalFinemapping.caffemodel",
		"../model/Segmentation.prototxt", "../model/Segmentation.caffemodel",
		"../model/CharacterRecognization.prototxt", "../model/CharacterRecognization.caffemodel");
}

void CHyperLPRDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_PATH, m_path);
	DDX_Text(pDX, IDC_EDIT_RESULT, m_result);
}

BEGIN_MESSAGE_MAP(CHyperLPRDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_OPEN, &CHyperLPRDlg::OnBnClickedBtnOpen)
	ON_BN_CLICKED(IDC_BTN_DETECT, &CHyperLPRDlg::OnBnClickedBtnDetect)
	ON_EN_SETFOCUS(IDC_EDIT_RESULT, &CHyperLPRDlg::OnEnSetfocusEditResult)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CHyperLPRDlg ��Ϣ�������

BOOL CHyperLPRDlg::OnInitDialog()
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
	GetDlgItem(IDC_EDIT_RESULT)->HideCaret();

	CRect rect;
	GetClientRect(&rect);     //ȡ�ͻ�����С    
	m_old.x = rect.right - rect.left;
	m_old.y = rect.bottom - rect.top;
	// TODO:  �ڴ���Ӷ���ĳ�ʼ������

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CHyperLPRDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CHyperLPRDlg::OnPaint()
{
	if (!m_detectImg.empty()){
		ShowImage(m_detectImg, IDC_PICTURE);
	}
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
		CDialog::UpdateWindow(); //UpdateWindowһ��  
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CHyperLPRDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CHyperLPRDlg::ShowImage(Mat img, UINT ID){
	if (img.empty()){
		return;
	}
	CRect rect;
	GetDlgItem(ID)->GetClientRect(&rect);
	HDC hDC = GetDlgItem(ID)->GetDC()->GetSafeHdc();
	CvvImage cimg;
	IplImage cpy = img;
	cimg.CopyOf(&cpy);
	cimg.DrawToHDC(hDC, &rect);
}

void CHyperLPRDlg::OnBnClickedBtnOpen()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CFileDialog fileDlg(TRUE,  // TRUE��Open��FALSE����Save As�ļ��Ի���
		_T(".jpg"),  // Ĭ�ϵĴ��ļ�������
		_T(""), // Ĭ�ϴ򿪵��ļ��� 
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR,  // ��ѡ��
		_T("jpg�ļ�(*.jpg)|*.jpg|�����ļ�(*.*) |*.*||")  // �򿪵��ļ�����
		);

	//fileDlg.m_ofn.lpstrInitialDir = strPath;//��ʼ��·����
	if (fileDlg.DoModal() == IDOK){
		m_path = fileDlg.GetPathName();//����ѡ���������ļ����ƣ�
		char* strFileName = NULL;
#ifdef _UNICODE //���ַ�
		int nCharLen = WideCharToMultiByte(CP_ACP, 0, m_path, -1, NULL, 0, NULL, NULL);
		strFileName = new char[nCharLen + 1];
		WideCharToMultiByte(CP_ACP, 0, m_path, -1, strFileName, nCharLen + 1, NULL, NULL);
#else //�ǿ��ַ�
		strFileName = m_path.GetBuffer(m_path.GetLength() + 1);
		//m_path.ReleaseBuffer();
#endif
		m_path.Format(_T("%s"), strFileName);
		//SetDlgItemText(IDC_EDIT_PATH, m_path);
		//SetDlgItemText(IDC_EDIT_RESULT, _T(""));
		m_result = _T("");
		UpdateData(FALSE);
		m_img = imread(strFileName);
		ShowImage(m_img, IDC_PICTURE);
		m_detectImg = m_img.clone();

	}
}


void CHyperLPRDlg::OnBnClickedBtnDetect()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (m_img.empty()){
		AfxMessageBox(_T("��������ͼ��"));
		return;
	}
	std::vector<cv::Rect> rects;
	std::vector<pr::PlateInfo> res = psrc->RunPiplineAsImage(m_img, rects);

	std::string name;
	float conf = 0;
	int baseLine = 0;
	for (unsigned i = 0; i < res.size();++i){
		if (res[i].confidence>0.5) {
			//std::cout << st.getPlateName() << " " << st.confidence << std::endl;
			std::string label = res[i].getPlateName();
			name += label +"\r\n���Ŷ�:" + std::to_string(res[i].confidence) + "\r\n";
			//conf += st.confidence;
			cv::rectangle(m_detectImg, rects[i], cv::Scalar(0, 0, 255), 4, 8, 0);
			Size labelSize = cv::getTextSize(label, FONT_HERSHEY_SIMPLEX,0.5, 1, &baseLine);
			int top = max(rects[i].y, labelSize.height);
			putTextZH(m_detectImg, label.c_str(), Point(rects[i].x, top - labelSize.height), cv::Scalar(0, 255, 0), 0.5);
		}
	}
	ShowImage(m_detectImg, IDC_PICTURE);
	m_result.Format(_T("%s"), name.c_str());
	UpdateData(FALSE);
}


void CHyperLPRDlg::OnEnSetfocusEditResult()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	HWND hwnd = ::GetDlgItem(this->m_hWnd, IDC_EDIT_RESULT); 
	CWnd* pWnd = CWnd::FromHandle(hwnd); 
	pWnd->ModifyStyle(0, pWnd->GetStyle() | WS_DISABLED);
}

void CHyperLPRDlg::Resize()
{
	float fsp[2];
	POINT Newp; //��ȡ���ڶԻ���Ĵ�С  
	CRect recta;
	GetClientRect(&recta);     //ȡ�ͻ�����С    
	Newp.x = recta.right - recta.left;
	Newp.y = recta.bottom - recta.top;
	fsp[0] = (float)Newp.x / m_old.x;
	fsp[1] = (float)Newp.y / m_old.y;
	CRect Rect;
	int woc;
	CPoint OldTLPoint, TLPoint; //���Ͻ�  
	CPoint OldBRPoint, BRPoint; //���½�  
	HWND  hwndChild = ::GetWindow(m_hWnd, GW_CHILD);  //�г����пؼ�    
	while (hwndChild)
	{
		woc = ::GetDlgCtrlID(hwndChild);//ȡ��ID  
		GetDlgItem(woc)->GetWindowRect(Rect);
		ScreenToClient(Rect);
		OldTLPoint = Rect.TopLeft();
		TLPoint.x = long(OldTLPoint.x*fsp[0]);
		TLPoint.y = long(OldTLPoint.y*fsp[1]);
		OldBRPoint = Rect.BottomRight();
		BRPoint.x = long(OldBRPoint.x *fsp[0]);
		BRPoint.y = long(OldBRPoint.y *fsp[1]);
		Rect.SetRect(TLPoint, BRPoint);
		GetDlgItem(woc)->MoveWindow(Rect, TRUE);
		hwndChild = ::GetWindow(hwndChild, GW_HWNDNEXT);
	}
	m_old = Newp;
}

void CHyperLPRDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO:  �ڴ˴������Ϣ����������
	if (nType == SIZE_RESTORED || nType == SIZE_MAXIMIZED)
	{
		Resize();
	}
}

