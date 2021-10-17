#pragma once

////////////////////////////////////////////////////////////////////////////////
//////////////////////////// CPU-SPECIFIC FUNCTIONS ////////////////////////////

// Setcpu modes.
#define CPU_ASMHIST    0x00001         // Add change to Disassembler history
#define CPU_ASMCENTER  0x00004         // Make address in the middle of window
#define CPU_ASMFOCUS   0x00008         // Move focus to Disassembler
#define CPU_DUMPHIST   0x00010         // Add change to Dump history
#define CPU_DUMPFIRST  0x00020         // Make address the first byte in Dump
#define CPU_DUMPFOCUS  0x00080         // Move focus to Dump
#define CPU_REGAUTO    0x00100         // Automatically switch to FPU/MMX/3DNow!
#define CPU_RUNTRACE   0x00200         // Show run trace data at offset asmaddr
#define CPU_STACKFOCUS 0x00800         // Move focus to Stack
#define CPU_NOCREATE   0x04000         // Don't create CPU window if absent
#define CPU_REDRAW     0x08000         // Redraw CPU window immediately
#define CPU_NOFOCUS    0x10000         // Don't assign focus to main window



struct CpuHelper final abstract {
	static void Setcpu(ulong threadid, ulong asmaddr, ulong dumpaddr, ulong stackaddr, int mode);
	static void InvalidateWindows(bool redraw);
	static ulong Findprocbegin(ulong addr);
	static ulong Findprocend(ulong addr);
	static ulong Findprevproc(ulong addr);
	static ulong Findnextproc(ulong addr);
	static int Getproclimits(ulong addr, ulong* start, ulong* end);
};