#pragma once

class BreakpointHelper final abstract {
public:
	static ulong Getbreakpointtypecount(ulong addr, ulong* passcount);
};