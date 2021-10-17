#include "stdafx.h"
#include "BreakpointHelper.h"
#include "BreakpointTable.h"
#include "resource.h"
#include "BreakpointFrm.h"

ulong BreakpointHelper::Getbreakpointtypecount(ulong addr, ulong* passcount) {
	t_bpoint* bp = nullptr;
	bp = CBreakpointFrame::m_BreakpointTable->Findsorteddata(addr);
	if (bp != nullptr) {
		if(passcount)
			*passcount = bp->passcount;
		return bp->type;
	}
	return TY_INVALID;
}