
#pragma once

#include <functional>
#include <array>
#include "keys.hpp"

namespace TheExplosion {
	
	enum class EventType {

		WindowClose = 0,
		KeyPressed,
		KeyReleased,
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

	struct EventKeyPressed : public BaseEvent {

		EventKeyPressed(const KeyCode key_code, const bool repeated) :

			key_code(key_code),
			repeated(repeated)

		{}

		KeyCode key_code;
		bool repeated;
		virtual EventType get_type() const override { return type; }
		static const EventType type = EventType::KeyPressed;

	};

	struct EventKeyReleased : public BaseEvent {

		EventKeyReleased(const KeyCode key_code) : key_code(key_code) {}
		KeyCode key_code;
		virtual EventType get_type() const override { return type; }
		static const EventType type = EventType::KeyReleased;

	};

}
