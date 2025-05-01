
#pragma once

#include <functional>
#include <array>

namespace TheExplosion {
	
	enum class EventType {

		WindowClose = 0,
		EventsCount

	};
	
	struct BaseEvent {
		
		virtual ~BaseEvent() = default;
		virtual EventType get_type() const = 0;
		
	};

	class EventDispatcher {

	public:

		template<typename EventType> void add_event_listener(std::function<void(EventType&)> callback) {

			auto baseCallback = [func = std::move(callback)](BaseEvent& e) { func(static_cast<EventType&>(e)); };
			m_eventCallbacks[static_cast<size_t>(EventType::type)] = std::move(baseCallback);

		}

		void dispatch(BaseEvent& event) {
		
			auto& callback = m_eventCallbacks[static_cast<size_t>(event.get_type())];
			callback(event);
		
		}

	private:

		std::array<std::function<void(BaseEvent&)>, static_cast<size_t>(EventType::EventsCount)> m_eventCallbacks;

	};

	struct EventWindowClose : public BaseEvent {

		virtual EventType get_type() const override { return type; }
		static const EventType type = EventType::WindowClose;

	};

}
