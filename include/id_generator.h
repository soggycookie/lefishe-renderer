#pragma once
#include <atomic>

namespace Lefishe{

	class IDGenerator{
	public:
		static std::atomic<long> counter;
		
		static long Generate();
	};

}
