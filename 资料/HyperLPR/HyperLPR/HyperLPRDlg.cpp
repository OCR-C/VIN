
// HyperLPRDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "HyperLPR.h"
#include "HyperLPRDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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


// CHyperLPRDlg 对话框
std::vector<std::string> chars_code{ "京", "沪", "津", "渝", "冀", "晋", "蒙", "辽", "吉", "黑", "苏", "浙", "皖", "闽", "赣", "鲁", "豫", "鄂", "湘", "粤", "桂", "琼", "川", "贵", "云", "藏", "陕", "甘", "青", "宁", "新", "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "A", "B", "C", "D", "E", "F", "G", "H", "J", "K", "L", "M", "N", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z" };



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


// CHyperLPRDlg 消息处理程序

BOOL CHyperLPRDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标
	GetDlgItem(IDC_EDIT_RESULT)->HideCaret();

	CRect rect;
	GetClientRect(&rect);     //取客户区大小    
	m_old.x = rect.right - rect.left;
	m_old.y = rect.bottom - rect.top;
	// TODO:  在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CHyperLPRDlg::OnPaint()
{
	if (!m_detectImg.empty()){
		ShowImage(m_detectImg, IDC_PICTURE);
	}
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
		CDialog::UpdateWindow(); //UpdateWindow一下  
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
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
	// TODO:  在此添加控件通知处理程序代码
	CFileDialog fileDlg(TRUE,  // TRUE打开Open，FALSE保存Save As文件对话框
		_T(".jpg"),  // 默认的打开文件的类型
		_T(""), // 默认打开的文件名 
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR,  // 单选打开
		_T("jpg文件(*.jpg)|*.jpg|所有文件(*.*) |*.*||")  // 打开的文件类型
		);

	//fileDlg.m_ofn.lpstrInitialDir = strPath;//初始化路径。
	if (fileDlg.DoModal() == IDOK){
		m_path = fileDlg.GetPathName();//返回选择或输入的文件名称，
		char* strFileName = NULL;
#ifdef _UNICODE //宽字符
		int nCharLen = WideCharToMultiByte(CP_ACP, 0, m_path, -1, NULL, 0, NULL, NULL);
		strFileName = new char[nCharLen + 1];
		WideCharToMultiByte(CP_ACP, 0, m_path, -1, strFileName, nCharLen + 1, NULL, NULL);
#else //非宽字符
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
	// TODO:  在此添加控件通知处理程序代码
	if (m_img.empty()){
		AfxMessageBox(_T("请先载入图像"));
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
			name += label +"\r\n置信度:" + std::to_string(res[i].confidence) + "\r\n";
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
	// TODO:  在此添加控件通知处理程序代码
	HWND hwnd = ::GetDlgItem(this->m_hWnd, IDC_EDIT_RESULT); 
	CWnd* pWnd = CWnd::FromHandle(hwnd); 
	pWnd->ModifyStyle(0, pWnd->GetStyle() | WS_DISABLED);
}

void CHyperLPRDlg::Resize()
{
	float fsp[2];
	POINT Newp; //获取现在对话框的大小  
	CRect recta;
	GetClientRect(&recta);     //取客户区大小    
	Newp.x = recta.right - recta.left;
	Newp.y = recta.bottom - recta.top;
	fsp[0] = (float)Newp.x / m_old.x;
	fsp[1] = (float)Newp.y / m_old.y;
	CRect Rect;
	int woc;
	CPoint OldTLPoint, TLPoint; //左上角  
	CPoint OldBRPoint, BRPoint; //右下角  
	HWND  hwndChild = ::GetWindow(m_hWnd, GW_CHILD);  //列出所有控件    
	while (hwndChild)
	{
		woc = ::GetDlgCtrlID(hwndChild);//取得ID  
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

	// TODO:  在此处添加消息处理程序代码
	if (nType == SIZE_RESTORED || nType == SIZE_MAXIMIZED)
	{
		Resize();
	}
}

