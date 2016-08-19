
// DESDlg.h : ��� ����
//

#pragma once
#include "afxwin.h"
#include<stdio.h>
#include<math.h>
#include<malloc.h>
#include<stdlib.h>


// CDESDlg ��ȭ ����
class CDESDlg : public CDialogEx
{
// �����Դϴ�.
public:
	CDESDlg(CWnd* pParent = NULL);	// ǥ�� �������Դϴ�.

	// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DES_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �����Դϴ�.


// �����Դϴ�.
protected:
	HICON m_hIcon;

	// ������ �޽��� �� �Լ�
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnClickedButton1();
	afx_msg void OnClickedButton2();
	afx_msg void OnClickedButton3();
	afx_msg void OnBnClickedOk();
	afx_msg void IsDlgButtonChecked();

	typedef unsigned char BYTE;
	typedef unsigned int UINT;
	CString p_t;
	CString get_k;
	CString c_t;
	CString d_c_t;
	CString d_get_k;
	CString d_t;
	
	int i;
	int msg_len, block_count;
	BYTE p_text[128];
	BYTE c_text[128];
	BYTE d_text[128];
	BYTE key[9];

	void DES_Encryption(BYTE *p_text, BYTE *result, BYTE *key);
	void DES_Decryption(BYTE *c_text, BYTE *result, BYTE *key);
	void IP(BYTE *in, BYTE *out);
	void In_IP(BYTE *in, BYTE *out);
	void EP(UINT r, BYTE *out);
	UINT Permutation(UINT in);
	void PC1(BYTE *in, BYTE *out);
	void PC2(UINT c, UINT d, BYTE *out);
	UINT S_box_Transfer(BYTE* in);
	UINT f(UINT in, BYTE * rkey);
	void key_expansion(BYTE *key, BYTE round_key[16][6]);
	void swap(UINT *x, UINT *y);
	void makeBit28(UINT *c, UINT *d, BYTE *data);
	UINT cir_shift(UINT n, int r);
	void BtoW(BYTE *in, UINT * x, UINT *y);
	void WtoB(UINT l, UINT r, BYTE *out);

};
