#ifndef __CRIPPLE_H__
#define __CRIPPLE_H__

#include <Windows.h>
class CRipple
{
public:
	BOOL InitRipple(HWND hWnd, UINT uiSpeed, HBITMAP hBmp);//初始化
	void DropStone(int x, int y, int stoneSize, int stoneWeight);//生成一波源
	
	void WaveSpread();//水波扩散
	void WaveRender();//根据波幅渲染水波
	UINT                m_nSpeed;//水波速度
private:

	HDC                 m_hDC;
	HDC					m_hRenderDC;				//内存渲染DC句柄
	HBITMAP				m_hRenderBmp;				//渲染位图句柄
	BITMAPINFO			m_stBitmapInfo;			    //位图信息结构
	int					m_iBmpWidth;				//水波背景图片宽	
	int					m_iBmpHeight;				//水波背景图片高
	int					m_iBytesPerWidth;			//位图每一行的字节数
	short			    *m_pWaveBuf1;				//波能缓冲区1
	short			    *m_pWaveBuf2;				//波能缓冲区2
	BYTE			   *m_pBmpSource;				//水波位图原始数据
	BYTE			   *m_pBmpRender;				//水波位图渲染数据
public:
	CRipple();
	virtual ~CRipple();
};

#endif