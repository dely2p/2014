
// DESDlg.cpp : 구현 파일
//

#include "stdafx.h"
#include "DES.h"
#include "DESDlg.h"
#include "afxdialogex.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "afxwin.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#define BLOCK_SIZE 8
#define DES_ROUND 16




BYTE InitialPermutationTable[64]	= 
		{58,50,42,34,26,18,10,2,
		60,52,44,36,28,20,12,4,
		62,54,46,38,30,22,14,6,
		64,56,48,40,32,24,16,8,
		57,49,41,33,25,17,9,1,
		59,51,43,35,27,19,11,3,
		61,53,45,37,29,21,13,5,
		63,55,47,39,31,23,15,7};
BYTE FinalPermutationTable[64]=
		{40,8,48,16,56,24,64,32,
		39,7,47,15,55,23,63,31,
		38,6,46,14,54,22,62,30,
		37,5,45,13,53,21,61,29,
		36,4,44,12,52,20,60,28,
		35,3,43,11,51,19,59,27,
		34,2,42,10,50,18,58,26,
		33,1,41,9,49,17,57,25};
BYTE ParityDropTable[56]=
		{57,49,41,33,25,17,9,
		1,58,50,42,34,26,18,
		10,2,59,51,43,35,27,
		19,11,3,60,52,44,36,
		63,55,47,39,31,23,15,
		7,62,54,46,38,30,22,
		14,6,61,53,45,37,29,
		21,13,5,28,20,12,4};
BYTE CompressionPBoxTable[48]=
		{14,17,11,24,1,5,3,28,
		15,6,21,10,23,19,12,4,
		26,8,16,7,27,20,13,2,
		41,52,31,37,47,55,30,40,
		51,45,33,48,44,49,39,56,
		34,53,46,42,50,36,29,32};
BYTE ExpansionPermutationTable[48]=
		{32,1,2,3,4,5,
		4,5,6,7,8,9,
		8,9,10,11,12,13,
		12,13,14,15,16,17,
		16,17,18,19,20,21,
		20,21,22,23,24,25,
		24,25,26,27,28,29,
		28,29,30,31,32,1};
BYTE StraightPermutationTable[32]=
	{16,7,20,21,29,12,28,17,
	1,15,23,26,5,18,31,10,
	2,8,24,14,32,27,3,9,
	19,13,30,6,22,11,4,25};
BYTE S[8][4][16]=
{
		{14,4,13,1,2,15,11,8,3,10,6,12,5,9,0,7,
		0,15,7,4,14,2,13,1,10,6,12,11,9,5,3,8,
		4,1,14,8,13,6,2,11,15,12,9,7,3,10,5,0,
		15,12,8,2,4,9,1,7,5,11,3,14,10,0,6,13},

		{15,1,8,14,6,11,3,4,9,7,2,13,12,0,5,10,
		3,13,4,7,15,2,8,14,12,0,1,10,6,9,11,5,
		0,14,7,11,10,4,13,1,5,8,12,6,9,3,2,15,
		13,8,10,1,3,15,4,2,11,6,7,12,0,5,14,9},

		{10,0,9,14,6,3,15,5,1,13,12,7,11,4,2,8,
		13,7,0,9,3,4,6,10,2,8,5,14,12,11,15,1,
		13,6,4,9,8,15,3,0,11,1,2,12,5,10,14,7,
		1,10,13,0,6,9,8,7,4,15,14,3,11,5,2,12},

		{7,13,14,3,0,6,9,10,1,2,8,5,11,12,4,15,
		13,8,11,5,6,15,0,3,4,7,2,12,1,10,14,9,
		10,6,9,0,12,11,7,13,15,1,3,14,5,2,8,4,
		3,15,0,6,10,1,13,8,9,4,5,11,12,7,2,14},

		{2,12,4,1,7,10,11,6,8,5,3,15,13,0,14,9,
		14,11,2,12,4,7,13,1,5,0,15,10,3,9,8,6,
		4,2,1,11,10,13,7,8,15,9,12,5,6,3,0,14,
		11,8,12,7,1,14,2,13,6,15,0,9,10,4,5,3},

		{12,1,10,15,9,2,6,8,0,13,3,4,14,7,5,11,
		10,15,4,2,7,12,9,5,6,1,13,14,0,11,3,8,
		9,14,15,5,2,8,12,3,7,0,4,10,1,13,11,6,
		4,3,2,12,9,5,15,10,11,14,1,7,6,0,8,13},

		{4,11,2,14,15,0,8,13,3,12,9,7,5,10,6,1,
		13,0,11,7,4,9,1,10,14,3,5,12,2,15,8,6,
		1,4,11,13,12,3,7,14,10,15,6,8,0,5,9,2,
		6,11,13,8,1,4,10,7,9,5,0,15,14,2,3,12},

		{13,2,8,4,6,15,11,1,10,9,3,14,5,0,12,7,
		1,15,13,8,10,3,7,4,12,5,6,11,0,14,9,2,
		7,11,4,1,9,12,14,2,0,6,10,13,15,3,5,8,
		2,1,14,7,4,10,8,13,15,12,9,0,3,5,6,11}
};


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
public:
//	virtual BOOL OnInitDialog();
	CButton check_b;
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDOK, check_b);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CDESDlg 대화 상자




CDESDlg::CDESDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDESDlg::IDD, pParent)
	, p_t(_T(""))
	, get_k(_T(""))
	, c_t(_T(""))
	, d_c_t(_T(""))
	, d_get_k(_T(""))
	, d_t(_T(""))
{

	
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDESDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, p_t);
	DDX_Text(pDX, IDC_EDIT2, get_k);
	DDX_Text(pDX, IDC_EDIT3, c_t);
	DDX_Text(pDX, IDC_EDIT4, d_c_t);
	DDX_Text(pDX, IDC_EDIT5, d_get_k);
	DDX_Text(pDX, IDC_EDIT6, d_t);
}

BEGIN_MESSAGE_MAP(CDESDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CDESDlg::OnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CDESDlg::OnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CDESDlg::OnClickedButton3)
	ON_BN_CLICKED(IDOK, &CDESDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_CHECK1, &CDESDlg::IsDlgButtonChecked)
END_MESSAGE_MAP()


// CDESDlg 메시지 처리기

BOOL CDESDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
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

	// 이 대화 상자의 아이콘을 설정합니다. 응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CDESDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다. 문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CDESDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CDESDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CDESDlg::OnClickedButton1()
{
	// TODO: Add your control notification handler code here
	msg_len =0;
	block_count = 0;
	int msgend=2,msgend1=2;

	for(i=0;i<128;i++){
		p_text[i] = 0;
		c_text[i] = 0;
		d_text[i] = 0;
	}
	for(i=0;i<9;i++)
		key[i] = 0;


	GetDlgItem(IDC_EDIT1)->GetWindowText(p_t); 
//	*p_text = p_t.GetBuffer;
	WideCharToMultiByte(CP_ACP,0,p_t,-1,(LPSTR)p_text,sizeof(p_text),NULL,NULL);

	GetDlgItem(IDC_EDIT2)->GetWindowText(get_k); // EDIT1 값을 갖어와서 str 로 넘깁니다.
	WideCharToMultiByte(CP_ACP,0,get_k,-1,(LPSTR)key,sizeof(key),NULL,NULL);
	if(p_text[8]!='\0' && msgend1!=0){
		msgend1=AfxMessageBox(_T("평문 입력은 8자 이내로.."));
		if(msgend1==1){
			SetDlgItemText(IDC_EDIT1,L"");
			SetDlgItemText(IDC_EDIT3,L"");
			goto again1;
		}
	}
	again1:
	if(key[8]!='\0' && msgend!=0){
		msgend=AfxMessageBox(_T("암호 key는 8자 이내로.."));
		if(msgend==1){
			SetDlgItemText(IDC_EDIT2,L"");
			SetDlgItemText(IDC_EDIT3,L"");
			goto again;
		}
	}
	else if(msgend1==1)
		goto again;
	else
		;
	msg_len =(int)strlen((char*)p_text);					//평문 size 받아서 
	for(i=0;i<msg_len;i++){									//평문에 space가 있으면 그 자리에 #을 둠
		if(p_text[i]==' ')
			p_text[i]='#';
	}
	
	block_count = (msg_len % BLOCK_SIZE) ? (msg_len /BLOCK_SIZE + 1) : (msg_len /BLOCK_SIZE);
															//8bit의 block의 나머지가 있으면 block갯수 1개 더해줌
	
	for(i=0;i<block_count;i++)
		DES_Encryption(&p_text[i*BLOCK_SIZE], &c_text[i*BLOCK_SIZE],key);

	c_t = c_text;
	GetDlgItem(IDC_EDIT3)->SetWindowText(c_t); 
	again:;
}


void CDESDlg::OnClickedButton2()
{

	int msgend=2,msgend1=2;

	for(i=0;i<128;i++){
		p_text[i] = 0;
//		c_text[i] = 0;
		d_text[i] = 0;
	}
	for(i=0;i<9;i++)
		key[i] = 0;

//	GetDlgItem(IDC_EDIT4)->GetWindowText(d_c_t); // EDIT1 값을 갖어와서 str 로 넘깁니다.
//	WideCharToMultiByte(CP_ACP,0,d_c_t,-1,(LPSTR)c_text,sizeof(c_text),NULL,NULL);

//	MultiByteToWideChar(CP_ACP, NULL, (LPCCH)&c_text,  -1, (LPWSTR)&d_c_t, 0);

	GetDlgItem(IDC_EDIT5)->GetWindowText(d_get_k); // EDIT1 값을 갖어와서 str 로 넘깁니다.
	WideCharToMultiByte(CP_ACP,0,d_get_k,-1,(LPSTR)key,sizeof(key),NULL,NULL);

	if(c_text[8]!='\0' && msgend1!=0){
		msgend1=AfxMessageBox(_T("평문 입력은 8자 이내로.."));
		if(msgend1==1){
			SetDlgItemText(IDC_EDIT4,L"");
			SetDlgItemText(IDC_EDIT6,L"");
			goto again1;
		}
	}
	again1:
	if(key[8]!='\0' && msgend!=0){
		msgend=AfxMessageBox(_T("암호 key는 8자 이내로.."));
		if(msgend==1){
			SetDlgItemText(IDC_EDIT5,L"");
			SetDlgItemText(IDC_EDIT6,L"");
			goto again;
		}
	}
	else if(msgend1==1)
		goto again;
	else
		;

	msg_len =(int)strlen((char*)c_text);					 
	for(i=0;i<msg_len;i++){									
		if(c_text[i]==' ')
			c_text[i]='#';
	}
	
	block_count = (msg_len % BLOCK_SIZE) ? (msg_len /BLOCK_SIZE + 1) : (msg_len /BLOCK_SIZE);
															//8bit의 block의 나머지가 있으면 block갯수 1개 더해줌

	for(i=0;i<block_count;i++)								//1개 block씩 복호화 함
		DES_Decryption(&c_text[i*BLOCK_SIZE], &d_text[i*BLOCK_SIZE],key);

	d_t = d_text;
	GetDlgItem(IDC_EDIT6)->SetWindowText(d_t); 	
	again:;
}


void CDESDlg::OnClickedButton3()
{
	// TODO: Add your control notification handler code here
	CButton *pButton = (CButton*)GetDlgItem(IDC_CHECK1);   //다이얼로그를 버튼의 포인터로 얻어와서
	pButton->SetCheck(FALSE);

	SetDlgItemText(IDC_EDIT1,L"");
	SetDlgItemText(IDC_EDIT2,L"");
	SetDlgItemText(IDC_EDIT3,L"");
	SetDlgItemText(IDC_EDIT4,L"");
	SetDlgItemText(IDC_EDIT5,L"");
	SetDlgItemText(IDC_EDIT6,L"");
}


void CDESDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnOK();
}

void CDESDlg::IsDlgButtonChecked()
{
	GetDlgItem(IDC_EDIT3)->GetWindowText(c_t); 		
	GetDlgItem(IDC_EDIT4)->SetWindowText(c_t); 	

	GetDlgItem(IDC_EDIT2)->GetWindowText(get_k); 		
	GetDlgItem(IDC_EDIT5)->SetWindowText(get_k); 	
	
}

void CDESDlg::DES_Encryption(BYTE *p_text, BYTE *result, BYTE *key){
	int i;
	BYTE data[BLOCK_SIZE]={0,};
	BYTE round_key[16][6]={0,};
	UINT L=0, R=0;

	key_expansion(key,round_key);							//라운드 키 생성
	IP(p_text,data);										//초기치환

	BtoW(data,&L,&R);										//?

	for(i=0;i<DES_ROUND;i++){								//des함수 
		L=L^f(R,round_key[i]);								//XOR연산
		if(i!=DES_ROUND-1)
			swap(&L,&R);									//swapper
	}
	WtoB(L,R,data);
	In_IP(data,result);
}
void CDESDlg::DES_Decryption(BYTE *c_text, BYTE *result, BYTE *key){
	int i;
	BYTE data[BLOCK_SIZE]={0,};
	BYTE round_key[16][6]={0,};
	UINT L=0,R=0;

	key_expansion(key,round_key);
	IP(c_text,data);

	BtoW(data,&L,&R);

	for(i=0;i<DES_ROUND;i++){
		L=L^f(R,round_key[DES_ROUND-i-1]);
		if(i!=DES_ROUND-1)
			swap(&L,&R);
	}
	WtoB(L,R,data);
	In_IP(data,result);
}

void CDESDlg::IP(BYTE *in, BYTE *out){
	int i;
	BYTE index, bit, mask=0x80;

	for(i=0;i<64;i++){
		index=(InitialPermutationTable[i]-1)/8;
		bit =(InitialPermutationTable[i]-1) %8;

		if(in[index]& (mask>>bit))
			out[i/8] |= mask>>(i%8);
	}
}
void CDESDlg::In_IP(BYTE *in, BYTE *out){
	int i;
	BYTE index, bit, mask=0x80;

	for(i=0;i<64;i++){
		index=(FinalPermutationTable[i]-1)/8;
		bit =(FinalPermutationTable[i]-1) %8;

		if(in[index]& (mask>>bit))
			out[i/8] |= mask>>(i%8);
	}
}
void CDESDlg::EP(UINT r, BYTE *out){
	int i;
	UINT mask =0x80000000;

	for(i=0;i<48;i++){
		if(r & (mask >> (ExpansionPermutationTable[i]-1))){
			out[i/8] |= (BYTE) (0x80 >> (i%8));
		}
	}
}
UINT CDESDlg::Permutation(UINT in){
	int i;
	UINT out=0, mask =0x80000000;
	for(i=0;i<32;i++){
		if(in & (mask>> (StraightPermutationTable[i]-1)))
			out |= (mask>> i);
	}
	return out;
}
void CDESDlg::PC1(BYTE *in, BYTE *out){
	int i,index,bit;
	UINT mask = 0x00000080;

	for(i=0;i<56;i++){
		index =(ParityDropTable[i]-1) /8;
		bit = (ParityDropTable[i]-1) % 8;

		if(in[index]& (BYTE) (mask >> bit))
			out[i/8] |= (BYTE) (mask >> (i%8));
	}
}
void CDESDlg::PC2(UINT c, UINT d, BYTE *out){
	int i;
	UINT mask = 0x08000000;

	for(i=0;i<48;i++){
		if(CompressionPBoxTable[i]<28){
			out[i/8] |= 0x80 >> (i%8);
		}
		else{
			if(d & (mask >> (CompressionPBoxTable[i]-1-28)))
				out[i/8] |= 0x80 >>(i%8);
		}
	}
}
UINT CDESDlg::S_box_Transfer(BYTE* in){
	int i,row, column,shift =28;
	UINT temp =0,result =0, mask =0x00000080;

	for(i=0;i<48;i++){
		if(in[i/8] & (BYTE)(mask >>(i*8)))
			temp |= 0x20 >>(i%6);
		if((i+1)%6 ==0){
			row = ((temp & 0x20) >> 4) + (temp & 0x01);
			column = (temp & 0x1E) >>1;
			result += ((UINT) S[i/6][row][column]<<shift);
			shift-=4;
			temp =0;
		}
	}
	return result;
}
UINT CDESDlg::f(UINT in, BYTE * rkey){
 int i;
 BYTE data[6] = {0,};
 UINT out;
 EP(in, data); // 확장 치환

 // 라운드 키와 XOR
 for(i=0;i<6;i++)
  data[i] = data[i] ^ rkey[i];

 out = Permutation(S_box_Transfer(data)); // S-box 변환 결과를 치환

 return out;
}

void CDESDlg::key_expansion(BYTE *key, BYTE round_key[16][6]){
	int i;
	BYTE pc1_result[7] ={0,};
	UINT c=0,d=0;

	PC1(key, pc1_result);
	makeBit28(&c,&d,pc1_result);

	for(i=0;i<16;i++){
		c=cir_shift(c,i);
		d=cir_shift(d,i);

		PC2(c,d,round_key[i]);
	}
}
void CDESDlg::swap(UINT *x, UINT *y){
	UINT temp;
	temp= *x;
	*x=*y;
	*y=temp;
}
void CDESDlg::makeBit28(UINT *c, UINT *d, BYTE *data){
	int i;
	BYTE mask = 0x80;

	for(i=0;i<56;i++){
		if(i<28){
			if(data[i/8] & (mask>> (i%8)))
				*c |= 0x8000000>>i;
		}
		else{
			if(data[i/8] & (mask>> (i%8)))
				*d |= 0x8000000>>(i-28);
		}
	}
}
UINT CDESDlg::cir_shift(UINT n, int r){
	int n_shift[16]={1,1,2,2,2,2,2,2,1,2,2,2,2,2,2,1};

	if(n_shift[r] ==1){
		n= (n<<1) + (n>>28);
	}
	else{
		n =(n<<2)+(n>>29);
	}
	return n;
}
void CDESDlg::BtoW(BYTE *in, UINT * x, UINT *y){
	int i;
	for(i=0;i<8;i++){
		if(i<4)
			*x |= (UINT)in[i]<<(24-(i*8));
		else
			*y |= (UINT)in[i]<<(56-(i*8));
	}
}
void CDESDlg::WtoB(UINT l, UINT r, BYTE *out){
	int i;
	UINT mask = 0xff000000;

	for(i=0;i<8;i++){
		if(i<4)
			out[i] = (l & (mask>>i*8)) >> (24-(i*8));
		else
			out[i] = (r & (mask>>(i-4)*8)) >> (56-(i*8));
	}
}

