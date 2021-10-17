#include "stdafx.h"
#include "WaterRipple.h"

bool CWaterRipple::InitWaterRipple(HWND hWnd, int speed, HBITMAP hBitmap) {
	if (!::IsWindow(hWnd))
		return false;
	_hdc = ::GetDC(hWnd);
	_speed = speed;
	BITMAP bitmap;
	if (::GetObject(hBitmap, sizeof(bitmap), &bitmap) == 0)
		return false;
	// ��ȡλͼ���ߡ�һ�е��ֽ���
	_width = bitmap.bmWidth;
	_height = bitmap.bmHeight;
	// 24bitλͼ��һ������ռ3���ֽڣ�һ�е����ֽ���Ϊ4�ı���
	_bytes = (_width * 3 + 3) & ~3;

	// ���䲨�ܻ�����
	int size = _width * _height;
	m_pWaveBuf1 = new short[size];
	if (m_pWaveBuf1 == nullptr)
		return false;
	m_pWaveBuf2 = new short[size];
	if (m_pWaveBuf2 == nullptr)
		return false;

	memset(m_pWaveBuf1, 0, sizeof(short) * size);
	memset(m_pWaveBuf2, 0, sizeof(short) * size);

	// ����λͼ�������ݻ�����
	size = _bytes * _height;
	m_pBitmapSource = new BYTE[size];
	if (m_pBitmapSource == nullptr) {
		delete[] m_pWaveBuf1;
		delete[] m_pWaveBuf2;
		return false;
	}
	m_pBitmapRender = new BYTE[size];
	if (m_pBitmapRender == nullptr) {
		delete[] m_pWaveBuf1;
		delete[] m_pWaveBuf2;
		delete[] m_pBitmapSource;
		return false;
	}

	// ��ȾDC
	_hRenderDC = CreateCompatibleDC(_hdc);
	_hRenderBitmap = CreateCompatibleBitmap(_hdc, _width, _height);
	//��ʼ��BITMAPINFO�ṹ
	_info.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	_info.bmiHeader.biWidth = _width;
	_info.bmiHeader.biHeight = -_height;
	_info.bmiHeader.biPlanes = 1;
	_info.bmiHeader.biBitCount = 24;//24λλͼ
	_info.bmiHeader.biCompression = BI_RGB;
	_info.bmiHeader.biSizeImage = 0;

	HDC hMemDC = CreateCompatibleDC(_hdc);
	SelectObject(hMemDC, hBitmap);
	GetDIBits(hMemDC, hBitmap, 0, _height, m_pBitmapSource, &_info, DIB_RGB_COLORS);
	GetDIBits(hMemDC, hBitmap, 0, _height, m_pBitmapRender, &_info, DIB_RGB_COLORS);
	DeleteDC(hMemDC); // ��ȡ��λͼ����
	::CreateThread(nullptr, 0, [](void* params) {
		CWaterRipple* pWaterRipple = (CWaterRipple*)params;
		while (pWaterRipple->_speed != 0) {
			pWaterRipple->WaveSpread();
			pWaterRipple->WaveRender();
			Sleep(pWaterRipple->_speed);
		}
		return (DWORD)0;
		}, this, 0, nullptr);
}

/*
�趨��Դ
*/
void CWaterRipple::DropStone(int x, int y, int stoneSize, int stoneWeight) {
	// ͻ�Ʊ߽粻����
	if (x >= _width - stoneSize || y >= _height - stoneSize || x < stoneSize || y < stoneSize)
		return;

	for (int posx = x - stoneSize; posx < x + stoneSize; posx++)
	{
		for (int posy = y - stoneSize; posy < y + stoneSize; posy++)
		{
			if ((posx - x) * (posx - x) + (posy - y) * (posy - y) < stoneSize * stoneSize)
			{
				m_pWaveBuf1[_width * posy + posx] += stoneWeight;//�����stoneSize��Χֵ����stoneWeight
			}
		}
	}
}

void CWaterRipple::WaveSpread()////Ͷ��ʯͷ��ɢ����
{
	for (int i = _width; i < (_height - 1) * _width; i++)
	{
		// ��ʽ��X0'= (X1+X2+X3+X4) / 2 - X0
		m_pWaveBuf2[i] = ((m_pWaveBuf1[i - _width] + m_pWaveBuf1[i + _width] + m_pWaveBuf1[i - 1] + m_pWaveBuf1[i + 1]) >> 1) - m_pWaveBuf2[i];
		m_pWaveBuf2[i] -= m_pWaveBuf2[i] >> 5;// ����˥��
	}
	short* ptmp = m_pWaveBuf1;
	m_pWaveBuf1 = m_pWaveBuf2;
	m_pWaveBuf2 = ptmp;
}

/*
 ���ܣ�����ˮ��������Ⱦˮ��λͼ����
 */
void CWaterRipple::WaveRender()
{
	int iPtrSource = 0;
	int iPtrRender = 0;
	int lineIndex = _width;
	int iPosX = 0;
	int iPosY = 0;

	//ɨ��λͼ
	for (int y = 1; y < _height - 1; y++)
	{
		for (int x = 0; x < _width; x++){
		//���ݲ�������λͼ����ƫ��ֵ����Ⱦ�㣨x��y)��ӦԭʼͼƬ��iPosX��iPosY��
		//��Ч1			
			iPosX = x + (m_pWaveBuf1[lineIndex - 1] - m_pWaveBuf1[lineIndex + 1]);
			iPosY = y + (m_pWaveBuf1[lineIndex - _width] - m_pWaveBuf1[lineIndex + _width]);
		/*��Ч2 /*
						short waveData = (1024 - m_pWaveBuf1[lineIndex]);
						iPosX = (x - m_iBmpWidth/2)*waveData/1024 + m_iBmpWidth/2;
						iPosY = (y - m_iBmpHeight/2)*waveData/1024 + m_iBmpHeight/2;
			///*/

			if (0 <= iPosX && iPosX < _width && 0 <= iPosY && iPosY < _height)
			{
				//�ֱ����ԭʼλͼ��iPosX��iPosY������Ⱦλͼ��x��y)��Ӧ����ʼλͼ����
				iPtrSource = iPosY * _bytes + iPosX * 3;
				iPtrRender = y * _bytes + x * 3;
				for (int c = 0; c < 3; c++)//��Ⱦλͼ�����´������
				{
					m_pBitmapRender[iPtrRender + c] = m_pBitmapSource[iPtrSource + c];
				}
			}
			lineIndex++;
		}
	}
	//������Ⱦ���λͼ
	SetDIBits(_hRenderDC, _hRenderBitmap, 0, _height, m_pBitmapRender, &_info, DIB_RGB_COLORS);
	BitBlt(_hdc, 0, 0, _width, _height, _hRenderDC, 0, 0, SRCCOPY);
}
