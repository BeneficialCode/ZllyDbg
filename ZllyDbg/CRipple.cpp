
#include "stdafx.h"
#include "CRipple.h"

DWORD WINAPI ThreadFun( LPVOID pParam )
{
	CRipple *pRipple = (CRipple *)pParam;
	while(pRipple->m_nSpeed!=0)
	{
		pRipple->WaveSpread();//ˮ����ɢ����Ⱦ
		pRipple->WaveRender();
		Sleep(pRipple->m_nSpeed);
	}
	return 0;
}


//////////////////////////////////////////////////////////////////////
CRipple::CRipple()
{
	m_hDC        = NULL;
	m_hRenderDC  = NULL;				
	m_hRenderBmp = NULL;				
	m_iBmpWidth  = 0;				
	m_iBmpHeight = 0;				
	m_iBytesPerWidth = 0;			
	m_pWaveBuf1  = NULL;				
	m_pWaveBuf2  = NULL;				
	m_pBmpSource = NULL;				
	m_pBmpRender = NULL;
	m_nSpeed     = 0;			
	memset(&m_stBitmapInfo, 0, sizeof(m_stBitmapInfo));
}

CRipple::~CRipple()//�ͷ���Դ
{
	m_nSpeed     = 0;//�����߳�
	if(m_hDC != NULL)
	{
		DeleteDC(m_hDC);
	}
	if(m_hRenderDC != NULL)
	{
		DeleteDC(m_hRenderDC);
	}
	if(m_hRenderBmp != NULL)
	{
		DeleteObject(m_hRenderBmp);
	}
	if(m_pWaveBuf1 != NULL)
	{
		delete []m_pWaveBuf1;
	}
	if(m_pWaveBuf2 != NULL)
	{
		delete []m_pWaveBuf2;
	}
	if(m_pBmpSource != NULL)
	{
		delete []m_pBmpSource;
	}
	if(m_pBmpRender != NULL)
	{
		delete []m_pBmpRender;
	}
}

/**
 * ���ܣ���ʼ��ˮ������
 * ������
 *		[in]		hWnd		���ھ��
 *		[in]		hBmp		ˮ������ͼƬ���
 *		[in]		uiSpeed		��ʱ�����ʱ�䣨ˢ���ٶȣ�
 * ����ֵ �ɹ�true��ʧ��false
 */
BOOL CRipple::InitRipple(HWND hWnd, UINT uiSpeed, HBITMAP hBmp)
{
	if(!IsWindow(hWnd) )
		return FALSE;
	m_hDC     = GetDC(hWnd);
	m_nSpeed  = uiSpeed;

	BITMAP stBitmap;
	if(GetObject(hBmp, sizeof(stBitmap), &stBitmap) == 0)
	{
		return false;
	}
	//��ȡλͼ���ߡ�һ�е��ֽ���
	m_iBmpWidth      = stBitmap.bmWidth;
	m_iBmpHeight     = stBitmap.bmHeight;
	//24λλͼ��һ������ռ3���ֽڣ�һ�е����ֽ���ҪΪ4�ı���
	m_iBytesPerWidth = (m_iBmpWidth*3 + 3) & ~3;		


	//���䲨�ܻ�����
	m_pWaveBuf1 = new short[m_iBmpWidth*m_iBmpHeight];
	m_pWaveBuf2 = new short[m_iBmpWidth*m_iBmpHeight];

	//�ռ����ʧ��
	if(m_pWaveBuf1 == NULL || m_pWaveBuf2 == NULL)
		return false;

	memset(m_pWaveBuf1, 0, sizeof(short)*m_iBmpWidth*m_iBmpHeight);
	memset(m_pWaveBuf2, 0, sizeof(short)*m_iBmpWidth*m_iBmpHeight);

	//����λͼ�������ݻ�����
	m_pBmpSource = new BYTE[m_iBytesPerWidth*m_iBmpHeight];
	m_pBmpRender = new BYTE[m_iBytesPerWidth*m_iBmpHeight];

	//�ռ����ʧ��
	if(m_pBmpSource == NULL || m_pBmpRender == NULL)
		return false;
	//��ȾDC
	m_hRenderDC  = CreateCompatibleDC(m_hDC);
	m_hRenderBmp = CreateCompatibleBitmap(m_hDC, m_iBmpWidth, m_iBmpHeight);
	SelectObject(m_hRenderDC, m_hRenderBmp);

	//��ʼ��BITMAPINFO�ṹ
	m_stBitmapInfo.bmiHeader.biSize        = sizeof(BITMAPINFOHEADER);
	m_stBitmapInfo.bmiHeader.biWidth       = m_iBmpWidth;
	m_stBitmapInfo.bmiHeader.biHeight      = -m_iBmpHeight;
	m_stBitmapInfo.bmiHeader.biPlanes      = 1;
	m_stBitmapInfo.bmiHeader.biBitCount    = 24;//24λλͼ
	m_stBitmapInfo.bmiHeader.biCompression = BI_RGB;
	m_stBitmapInfo.bmiHeader.biSizeImage   = 0;

	//������ʱ�ڴ�DC����ͼƬ
	HDC hMemDC = CreateCompatibleDC(m_hDC);
	SelectObject(hMemDC, hBmp);
	GetDIBits(hMemDC, hBmp, 0, m_iBmpHeight, m_pBmpSource, &m_stBitmapInfo, DIB_RGB_COLORS);
	GetDIBits(hMemDC, hBmp, 0, m_iBmpHeight, m_pBmpRender, &m_stBitmapInfo, DIB_RGB_COLORS);
	DeleteDC(hMemDC);//��ȡ��λͼ���ݣ��ͷ��ڴ�DC
	::CreateThread(nullptr, 0, ThreadFun, this, 0, nullptr);
	return true;
}
/**
 * ���ܣ���ʯ�ӣ��趨��Դ��
 * ������
 *		[in]		x				ʯ��λ��x
 *		[in]		y				ʯ��λ��y
 *		[in]		stoneSize		ʯ�Ӵ�С���뾶��
 *		[in]		stoneWeight		ʯ������
 */
void CRipple::DropStone(int x, int y, int stoneSize, int stoneWeight)
{
	// ͻ�Ʊ߽粻����
	if ( x >= m_iBmpWidth - stoneSize  ||  y >= m_iBmpHeight - stoneSize|| x < stoneSize || y < stoneSize )
		return;

	for (int posx=x-stoneSize; posx<x+stoneSize; posx++)
	{
		for (int posy=y-stoneSize; posy<y+stoneSize; posy++)
		{
			if ((posx-x)*(posx-x) + (posy-y)*(posy-y) < stoneSize*stoneSize)
			{
				m_pWaveBuf1[m_iBmpWidth*posy+posx] += stoneWeight;//�����stoneSize��Χֵ����stoneWeight
			}
		}
	}
}

void CRipple::WaveSpread()////Ͷ��ʯͷ��ɢ����
{
	for(int i = m_iBmpWidth; i < (m_iBmpHeight-1)*m_iBmpWidth; i++)
	{
		// ��ʽ��X0'= (X1+X2+X3+X4) / 2 - X0
		m_pWaveBuf2[i] = ((m_pWaveBuf1[i-m_iBmpWidth] + m_pWaveBuf1[i+m_iBmpWidth] + m_pWaveBuf1[i-1] + m_pWaveBuf1[i+1]) >> 1) - m_pWaveBuf2[i];
		m_pWaveBuf2[i] -= m_pWaveBuf2[i] >> 5;// ����˥��
	}
	short *ptmp = m_pWaveBuf1;
	m_pWaveBuf1  = m_pWaveBuf2;
	m_pWaveBuf2 = ptmp;
}

/*
 ���ܣ�����ˮ��������Ⱦˮ��λͼ����
 */
void CRipple::WaveRender()
{
	int iPtrSource = 0;
	int iPtrRender = 0;
	int lineIndex = m_iBmpWidth;
	int iPosX = 0;
	int iPosY = 0;

	//ɨ��λͼ
	for(int y = 1; y < m_iBmpHeight - 1; y++)
	{
		for(int x = 0; x < m_iBmpWidth; x++)
		{//���ݲ�������λͼ����ƫ��ֵ����Ⱦ�㣨x��y)��ӦԭʼͼƬ��iPosX��iPosY��
//��Ч1			
			iPosX = x + (m_pWaveBuf1[lineIndex - 1] - m_pWaveBuf1[lineIndex + 1]);
			iPosY = y + (m_pWaveBuf1[lineIndex - m_iBmpWidth] - m_pWaveBuf1[lineIndex + m_iBmpWidth]);
/*��Ч2 /*
			short waveData = (1024 - m_pWaveBuf1[lineIndex]);
			iPosX = (x - m_iBmpWidth/2)*waveData/1024 + m_iBmpWidth/2;
			iPosY = (y - m_iBmpHeight/2)*waveData/1024 + m_iBmpHeight/2;
///*/
		
			if(0 <= iPosX && iPosX < m_iBmpWidth && 0 <= iPosY && iPosY < m_iBmpHeight)
			{
				//�ֱ����ԭʼλͼ��iPosX��iPosY������Ⱦλͼ��x��y)��Ӧ����ʼλͼ����
				iPtrSource = iPosY*m_iBytesPerWidth + iPosX*3;
				iPtrRender = y*m_iBytesPerWidth + x*3;
				for(int c = 0; c < 3; c++)//��Ⱦλͼ�����´������
				{
					m_pBmpRender[iPtrRender + c] = m_pBmpSource[iPtrSource + c];
				}
			}
			lineIndex++;
		}
	}
//������Ⱦ���λͼ
	::SetDIBits(m_hRenderDC, m_hRenderBmp, 0, m_iBmpHeight, m_pBmpRender, &m_stBitmapInfo, DIB_RGB_COLORS);
	::BitBlt(m_hDC, 0, 0, m_iBmpWidth, m_iBmpHeight, m_hRenderDC, 0, 0, SRCCOPY);
}

