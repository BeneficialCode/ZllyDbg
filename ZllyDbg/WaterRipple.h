#pragma once

class CWaterRipple {
public:
	//CWaterRipple();
	bool InitWaterRipple(HWND hWnd, int speed, HBITMAP hBitmap);
	void DropStone(int x, int y, int stoneSize, int stoneWeith);
private:
	void WaveSpread(); // ˮ����ɢ
	void WaveRender(); // ���ݲ�����Ⱦˮ��

private:
	int _speed = 0;	// ˮ���ٶ�
	HDC _hdc = nullptr;
	HDC _hRenderDC = nullptr; // �ڴ���Ⱦ���
	HBITMAP _hRenderBitmap = nullptr; // ��Ⱦλͼ���
	BITMAPINFO _info = { 0 };	// λͼ��Ϣ�ṹ
	int _width = 0;		// ˮ������ͼƬ���
	int _height = 0;	// ˮ������ͼƬ�߶�
	int _bytes = 0;	// λͼÿ���ֽ���
	short* m_pWaveBuf1 = nullptr; // ���ܻ�����1
	short* m_pWaveBuf2 = nullptr;	// ���ܻ�����2
	BYTE* m_pBitmapSource = nullptr;	// ˮ��λͼԭʼ����
	BYTE* m_pBitmapRender = nullptr;  // ˮ��λͼ��Ⱦ����
};