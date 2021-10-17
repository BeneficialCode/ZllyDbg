
#include "stdafx.h"
#include "CRipple.h"

DWORD WINAPI ThreadFun( LPVOID pParam )
{
	CRipple *pRipple = (CRipple *)pParam;
	while(pRipple->m_nSpeed!=0)
	{
		pRipple->WaveSpread();//水波扩散、渲染
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

CRipple::~CRipple()//释放资源
{
	m_nSpeed     = 0;//结束线程
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
 * 功能：初始化水波对象
 * 参数：
 *		[in]		hWnd		窗口句柄
 *		[in]		hBmp		水波背景图片句柄
 *		[in]		uiSpeed		定时器间隔时间（刷新速度）
 * 返回值 成功true、失败false
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
	//获取位图宽、高、一行的字节数
	m_iBmpWidth      = stBitmap.bmWidth;
	m_iBmpHeight     = stBitmap.bmHeight;
	//24位位图，一个像素占3个字节，一行的总字节数要为4的倍数
	m_iBytesPerWidth = (m_iBmpWidth*3 + 3) & ~3;		


	//分配波能缓冲区
	m_pWaveBuf1 = new short[m_iBmpWidth*m_iBmpHeight];
	m_pWaveBuf2 = new short[m_iBmpWidth*m_iBmpHeight];

	//空间分配失败
	if(m_pWaveBuf1 == NULL || m_pWaveBuf2 == NULL)
		return false;

	memset(m_pWaveBuf1, 0, sizeof(short)*m_iBmpWidth*m_iBmpHeight);
	memset(m_pWaveBuf2, 0, sizeof(short)*m_iBmpWidth*m_iBmpHeight);

	//分配位图像素数据缓冲区
	m_pBmpSource = new BYTE[m_iBytesPerWidth*m_iBmpHeight];
	m_pBmpRender = new BYTE[m_iBytesPerWidth*m_iBmpHeight];

	//空间分配失败
	if(m_pBmpSource == NULL || m_pBmpRender == NULL)
		return false;
	//渲染DC
	m_hRenderDC  = CreateCompatibleDC(m_hDC);
	m_hRenderBmp = CreateCompatibleBitmap(m_hDC, m_iBmpWidth, m_iBmpHeight);
	SelectObject(m_hRenderDC, m_hRenderBmp);

	//初始化BITMAPINFO结构
	m_stBitmapInfo.bmiHeader.biSize        = sizeof(BITMAPINFOHEADER);
	m_stBitmapInfo.bmiHeader.biWidth       = m_iBmpWidth;
	m_stBitmapInfo.bmiHeader.biHeight      = -m_iBmpHeight;
	m_stBitmapInfo.bmiHeader.biPlanes      = 1;
	m_stBitmapInfo.bmiHeader.biBitCount    = 24;//24位位图
	m_stBitmapInfo.bmiHeader.biCompression = BI_RGB;
	m_stBitmapInfo.bmiHeader.biSizeImage   = 0;

	//创建临时内存DC保存图片
	HDC hMemDC = CreateCompatibleDC(m_hDC);
	SelectObject(hMemDC, hBmp);
	GetDIBits(hMemDC, hBmp, 0, m_iBmpHeight, m_pBmpSource, &m_stBitmapInfo, DIB_RGB_COLORS);
	GetDIBits(hMemDC, hBmp, 0, m_iBmpHeight, m_pBmpRender, &m_stBitmapInfo, DIB_RGB_COLORS);
	DeleteDC(hMemDC);//获取完位图数据，释放内存DC
	::CreateThread(nullptr, 0, ThreadFun, this, 0, nullptr);
	return true;
}
/**
 * 功能：扔石子（设定波源）
 * 参数：
 *		[in]		x				石子位置x
 *		[in]		y				石子位置y
 *		[in]		stoneSize		石子大小（半径）
 *		[in]		stoneWeight		石子重量
 */
void CRipple::DropStone(int x, int y, int stoneSize, int stoneWeight)
{
	// 突破边界不处理
	if ( x >= m_iBmpWidth - stoneSize  ||  y >= m_iBmpHeight - stoneSize|| x < stoneSize || y < stoneSize )
		return;

	for (int posx=x-stoneSize; posx<x+stoneSize; posx++)
	{
		for (int posy=y-stoneSize; posy<y+stoneSize; posy++)
		{
			if ((posx-x)*(posx-x) + (posy-y)*(posy-y) < stoneSize*stoneSize)
			{
				m_pWaveBuf1[m_iBmpWidth*posy+posx] += stoneWeight;//点矩形stoneSize范围值都加stoneWeight
			}
		}
	}
}

void CRipple::WaveSpread()////投入石头扩散波纹
{
	for(int i = m_iBmpWidth; i < (m_iBmpHeight-1)*m_iBmpWidth; i++)
	{
		// 公式：X0'= (X1+X2+X3+X4) / 2 - X0
		m_pWaveBuf2[i] = ((m_pWaveBuf1[i-m_iBmpWidth] + m_pWaveBuf1[i+m_iBmpWidth] + m_pWaveBuf1[i-1] + m_pWaveBuf1[i+1]) >> 1) - m_pWaveBuf2[i];
		m_pWaveBuf2[i] -= m_pWaveBuf2[i] >> 5;// 波能衰减
	}
	short *ptmp = m_pWaveBuf1;
	m_pWaveBuf1  = m_pWaveBuf2;
	m_pWaveBuf2 = ptmp;
}

/*
 功能：根据水波波幅渲染水波位图数据
 */
void CRipple::WaveRender()
{
	int iPtrSource = 0;
	int iPtrRender = 0;
	int lineIndex = m_iBmpWidth;
	int iPosX = 0;
	int iPosY = 0;

	//扫描位图
	for(int y = 1; y < m_iBmpHeight - 1; y++)
	{
		for(int x = 0; x < m_iBmpWidth; x++)
		{//根据波幅计算位图数据偏移值，渲染点（x，y)对应原始图片（iPosX，iPosY）
//特效1			
			iPosX = x + (m_pWaveBuf1[lineIndex - 1] - m_pWaveBuf1[lineIndex + 1]);
			iPosY = y + (m_pWaveBuf1[lineIndex - m_iBmpWidth] - m_pWaveBuf1[lineIndex + m_iBmpWidth]);
/*特效2 /*
			short waveData = (1024 - m_pWaveBuf1[lineIndex]);
			iPosX = (x - m_iBmpWidth/2)*waveData/1024 + m_iBmpWidth/2;
			iPosY = (y - m_iBmpHeight/2)*waveData/1024 + m_iBmpHeight/2;
///*/
		
			if(0 <= iPosX && iPosX < m_iBmpWidth && 0 <= iPosY && iPosY < m_iBmpHeight)
			{
				//分别计算原始位图（iPosX，iPosY）和渲染位图（x，y)对应的起始位图数据
				iPtrSource = iPosY*m_iBytesPerWidth + iPosX*3;
				iPtrRender = y*m_iBytesPerWidth + x*3;
				for(int c = 0; c < 3; c++)//渲染位图，重新打点数据
				{
					m_pBmpRender[iPtrRender + c] = m_pBmpSource[iPtrSource + c];
				}
			}
			lineIndex++;
		}
	}
//设置渲染后的位图
	::SetDIBits(m_hRenderDC, m_hRenderBmp, 0, m_iBmpHeight, m_pBmpRender, &m_stBitmapInfo, DIB_RGB_COLORS);
	::BitBlt(m_hDC, 0, 0, m_iBmpWidth, m_iBmpHeight, m_hRenderDC, 0, 0, SRCCOPY);
}

