#include "global_header.h"

namespace Lefishe {

	//template<typename... Args>
	//class EventBus {
	//public:
	//	using EventCallback = std::function<void(Args...)>;

	//	virtual ~EventBus() = default;

	//	virtual void publish(Args... args) {
	//		for (auto& callback : m_callbacks) {
	//			callback(args...);
	//		}
	//	}

	//	virtual void subscribe(EventCallback callback) {
	//		m_callbacks.push_back(callback);
	//	}

	//private:
	//	std::vector<EventCallback> m_callbacks;
	//};

}