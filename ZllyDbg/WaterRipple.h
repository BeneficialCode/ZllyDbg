#pragma once

class CWaterRipple {
public:
	//CWaterRipple();
	bool InitWaterRipple(HWND hWnd, int speed, HBITMAP hBitmap);
	void DropStone(int x, int y, int stoneSize, int stoneWeith);
private:
	void WaveSpread(); // 水波扩散
	void WaveRender(); // 根据波幅渲染水波

private:
	int _speed = 0;	// 水波速度
	HDC _hdc = nullptr;
	HDC _hRenderDC = nullptr; // 内存渲染句柄
	HBITMAP _hRenderBitmap = nullptr; // 渲染位图句柄
	BITMAPINFO _info = { 0 };	// 位图信息结构
	int _width = 0;		// 水波背景图片宽度
	int _height = 0;	// 水波背景图片高度
	int _bytes = 0;	// 位图每行字节数
	short* m_pWaveBuf1 = nullptr; // 波能缓冲区1
	short* m_pWaveBuf2 = nullptr;	// 波能缓冲区2
	BYTE* m_pBitmapSource = nullptr;	// 水波位图原始数据
	BYTE* m_pBitmapRender = nullptr;  // 水波位图渲染数据
};