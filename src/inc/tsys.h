/******************************************************************************
TSystem - Task System
******************************************************************************/

#ifndef CET_TSYSTEM_H
#define CET_TSYSTEM_H

#include "prerequisites.h"

namespace Cet
{
	class CET_API Task
	{
	public:
		typedef uint32_t	IDType;

	public:
		Task()
			: m_id(0)
		{}

		Task(IDType id)
			: m_id(id)
		{}

		IDType get_id() const
		{
			return m_id;
		}

		virtual void response() {}

	protected:
		IDType	m_id;
	};

	class CET_API TaskHandler
	{
	public:
		static const size_t	MAX_TASK_NUM = 3000;
		static const size_t MAX_CANCEL_CHANCE = 128;

	public:
		TaskHandler();
		virtual ~TaskHandler();

		void add_task(const Task& task);
		void kill();
		void wait_end();

	protected:
		void proc_thrd();

	protected:
		Task						m_tasks[MAX_TASK_NUM];
		Task::IDType				m_cancel_tasks[MAX_CANCEL_CHANCE];
		std::mutex*					m_mtx;
		std::condition_variable*	m_cv;
		std::thread*				m_workthrd;

		size_t		m_rsp_idx;
		size_t		m_req_idx;

		size_t		m_rspcancel_idx;
		size_t		m_reqcancel_idx;

		bool					m_wait_end;
		bool					m_keep_proc;
	};

	class CET_API TaskManager
	{
	protected:
		TaskManager(int handler_num);
	public:
		virtual ~TaskManager();
		static TaskManager& get_instance();

		void add_task(const Task& task);
		void wait_end();

	protected:
		int				m_handler_num;
		TaskHandler*	m_handlers;
	};

	static inline TaskManager& task_mgr() { return TaskManager::get_instance(); }
}

#endif