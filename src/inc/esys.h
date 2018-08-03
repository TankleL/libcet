/******************************************************************************
	ESys - Event System
******************************************************************************/

#ifndef CET_ESYS_H
#define CET_ESYS_H

#include "prerequisites.h"

namespace Cet
{
	class EventParam
	{
	public:
		virtual std::string param_type() const
		{
			return "eventparam";
		}

		virtual std::shared_ptr<EventParam> clone() const
		{
			return std::make_shared<EventParam>(*this);
		}
	};

	class EmptyEventParam : public EventParam
	{
	public:
		virtual std::string param_type() const override
		{
			std::string res(EventParam::param_type());
			return res.append(":empty");
		}

		virtual std::shared_ptr<EventParam> clone() const override
		{
			return std::make_shared<EmptyEventParam>(*this);
		}
	};

	class EventResult : public EventParam
	{
	public:
		virtual std::string param_type() const override
		{
			std::string res(EventParam::param_type());
			return res.append(":result");
		}

		virtual std::shared_ptr<EventParam> clone() const override
		{
			return std::make_shared<EventResult>(*this);
		}
	};

	class ErrorEventResult : public EventResult
	{
	public:
		ErrorEventResult(const std::string& msg)
			: message(msg)
		{}

	public:
		virtual std::string param_type() const override
		{
			std::string res(EventResult::param_type());
			return res.append(":error");
		}

		virtual std::shared_ptr<EventParam> clone() const override
		{
			return std::make_shared<ErrorEventResult>(*this);
		}

	public:
		std::string	message;
	};

	class EmptyEventResult : public EventResult
	{
	public:
		virtual std::string param_type() const override
		{
			std::string res(EventResult::param_type());
			return res.append(":empty");
		}

		virtual std::shared_ptr<EventParam> clone() const override
		{
			return std::make_shared<EmptyEventResult>(*this);
		}
	};

	class MultiEventResult : public EventResult
	{
		virtual std::string param_type() const override
		{
			std::string res(EventResult::param_type());
			return res.append(":multi");
		}

		virtual std::shared_ptr<EventParam> clone() const override
		{
			return std::make_shared<MultiEventResult>(*this);
		}
	};
	
	typedef unsigned int Event;
	
	class EventManager;
	class CET_API EventUser
	{
		friend class EventManager;
	public:
		typedef std::shared_ptr<EventResult> (EventUser::*EventCallBackFn)(const EventParam& param);

	public:
		virtual ~EventUser();

	protected:
		void register_event(Event event, EventCallBackFn callback);
		void unregister_event(Event event);
		void unregister_event_all();
	};

	class CET_API EventManager
	{
	public:
		~EventManager();
		static EventManager& get_instance();

	public:
		void register_event(Event event, EventUser::EventCallBackFn callback, EventUser* receiver);
		void unregister_event(Event event, EventUser* receiver);
		void unregister_event_user(EventUser* receiver);

		void post_event(Event event, const EventParam& param = EmptyEventParam());
		std::shared_ptr<EventResult> send_event(Event event, const EventParam& param = EmptyEventParam());
		std::vector<std::shared_ptr<EventResult>> send_event_firm(Event event, const EventParam& param = EmptyEventParam());

	public:
		void once();

	protected:
		EventManager(size_t max_call_depth);

	protected:
		struct DeferCall
		{
			DeferCall(const EventParam& param,
				const Event& event)
				: m_param(param.clone())
				, m_event(event)
			{}

			DeferCall(const DeferCall& rhs)
				: m_param(rhs.m_param)
				, m_event(rhs.m_event)
			{}

			DeferCall& operator=(const DeferCall& rhs)
			{
				m_param = rhs.m_param;
				m_event = rhs.m_event;
				return *this;
			}

			std::shared_ptr<EventParam>	m_param;
			Event m_event;
		};

		std::queue<DeferCall>*		m_post_list;

	protected:
		typedef std::map<EventUser*, EventUser::EventCallBackFn>	EventUserMap;
		std::unordered_map<Event, EventUserMap>*	m_events;

		size_t m_max_call_depth;
		size_t m_call_count;
	};

	static inline EventManager& event_mgr() { return EventManager::get_instance(); }
}

#define DECL_EVENT_ACTION(action_name)	Cet::EventResult action_name(const Cet::EventParam& param, int trace_id);

#endif // ! AGAME_BECOM_CORE_ESYSTEM_H
