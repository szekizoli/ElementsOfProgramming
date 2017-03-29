#include "list.h"

namespace eop {

    int& slist_node_count()
	{
		static int count = 0;
		return count;
	}

} // namespace eop