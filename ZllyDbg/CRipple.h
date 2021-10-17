#ifndef __CRIPPLE_H__
#define __CRIPPLE_H__

#include <Windows.h>
class CRipple
{
public:
	BOOL InitRipple(HWND hWnd, UINT uiSpeed, HBITMAP hBmp);//��ʼ��
	void DropStone(int x, int y, int stoneSize, int stoneWeight);//����һ��Դ
	
	void WaveSpread();//ˮ����ɢ
	void WaveRender();//���ݲ�����Ⱦˮ��
	UINT                m_nSpeed;//ˮ���ٶ�
private:

	HDC                 m_hDC;
	HDC					m_hRenderDC;				//�ڴ���ȾDC���
	HBITMAP				m_hRenderBmp;				//��Ⱦλͼ���
	BITMAPINFO			m_stBitmapInfo;			    //λͼ��Ϣ�ṹ
	int					m_iBmpWidth;				//ˮ������ͼƬ��	
	int					m_iBmpHeight;				//ˮ������ͼƬ��
	int					m_iBytesPerWidth;			//λͼÿһ�е��ֽ���
	short			    *m_pWaveBuf1;				//���ܻ�����1
	short			    *m_pWaveBuf2;				//���ܻ�����2
	BYTE			   *m_pBmpSource;				//ˮ��λͼԭʼ����
	BYTE			   *m_pBmpRender;				//ˮ��λͼ��Ⱦ����
public:
	CRipple();
	virtual ~CRipple();
};

#endif