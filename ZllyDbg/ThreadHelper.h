#pragma once

  
class ThreadHelper final abstract{
public:
	static ulong Runsinglethread(ulong threadid);
	static t_thread* Findthread(ulong threadid);
	static t_reg* GetThreadCtx(ulong threadid);
	static void Restroeallthreads();
	static int GetThreadCount();
	static std::vector<t_thread>& GetThreads();
};