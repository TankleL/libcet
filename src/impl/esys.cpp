#include "api_dev_mod.h"

#include "../inc/esys.h"
#include "../inc/msged-exception.h"

#define MAX_CALL_STACK_DEPTH	64

using namespace Cet;

EventManager::EventManager(size_t max_call_depth)
	: m_post_list(new std::queue<DeferCall>())
	, m_events(new std::unordered_map<Event, EventUserMap>())
	, m_max_call_depth(max_call_depth)
	, m_call_count(0)
{}

EventManager::~EventManager()
{
	SAFE_DELETE(m_post_list);
	SAFE_DELETE(m_events);
}

EventManager& EventManager::get_instance()
{
	static EventManager instance(MAX_CALL_STACK_DEPTH);
	return instance;
}

void EventManager::register_event(Event event, EventUser::EventCallBackFn callback, EventUser* receiver)
{
	(*m_events)[event][receiver] = callback;
}

void EventManager::unregister_event(Event event, EventUser* receiver)
{
	(*m_events)[event][receiver] = nullptr;
}

void EventManager::unregister_event_user(EventUser* receiver)
{
	for (auto& event_users : *m_events)
	{
		for (auto& user : event_users.second)
		{
			if (user.first == receiver)
				user.second = nullptr;
		}
	}
}

std::shared_ptr<EventResult> EventManager::send_event(Event event, const EventParam& param)
{
	std::shared_ptr<EventResult> res(std::make_shared<EmptyEventResult>());
	++m_call_count;

	if (m_call_count >= m_max_call_depth)
	{
		assert(false);
		res = std::make_shared<ErrorEventResult>("send_event: cross max call stack depth");
	}
	else
	{
		size_t receiver_count = (*m_events)[event].size();
		switch (receiver_count)
		{
		case 0:
			break;

		case 1:
		{
			EventUser*	user = (*m_events)[event].begin()->first;
			EventUser::EventCallBackFn callback = (*m_events)[event].begin()->second;
			if (callback && user)
			{
				res = ((*(user)).*callback)(param);
			}
		}
			break;
		default:
			res = std::make_shared<MultiEventResult>();
			for (auto& user : (*m_events)[event])
			{
				if (user.second && user.first)
				{
					((*(user.first)).*(user.second))(param);
				}
			}
		}
	}

	--m_call_count;
	return res;
}

std::vector<std::shared_ptr<EventResult>> EventManager::send_event_firm(Event event, const EventParam& param)
{
	std::vector<std::shared_ptr<EventResult>> res;
	++m_call_count;

	if (m_call_count >= m_max_call_depth)
	{
		assert(false);
		res.push_back(std::make_shared<ErrorEventResult>("send_event: cross max call stack depth"));
	}
	else
	{
		for (auto& user : (*m_events)[event])
		{
			if (user.second && user.first)
			{
				res.push_back(((*(user.first)).*(user.second))(param));
			}
		}
	}

	--m_call_count;
	return res;
}

void EventManager::post_event(Event event, const EventParam& param)
{
	++m_call_count;
	if (m_call_count >= m_max_call_depth)
	{
		assert(false);
	}
	else
	{
		m_post_list->push(DeferCall(param, event));
		if (m_post_list->size() == 1)
		{
			while (m_post_list->size() > 0)
			{
				DeferCall& cur = m_post_list->front();
				for (auto& user : (*m_events)[cur.m_event])
				{
					if (user.second && user.first)
					{
						((*(user.first)).*(user.second))(*cur.m_param);
					}
				}
				m_post_list->pop();
				--m_call_count;
			}
		}
	}
}

EventUser::~EventUser()
{
	unregister_event_all();
}

void EventUser::register_event(Event event, EventCallBackFn callback)
{
	event_mgr().register_event(event, callback, this);
}

void EventUser::unregister_event(Event event)
{
	event_mgr().unregister_event(event, this);
}

void EventUser::unregister_event_all()
{
	event_mgr().unregister_event_user(this);
}