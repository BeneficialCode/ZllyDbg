#include "stdafx.h"
#include "InfoHelper.h"


void InfoHelper::Error(const char* format, ...) {
	char text[1024];
	va_list args;
	va_start(args, format);
	sprintf_s(text, 1024, format, args);
	va_end(args);
	MessageBoxA(g_hCpuFrm, text, "Error", MB_OK | MB_ICONHAND | MB_TASKMODAL);
}