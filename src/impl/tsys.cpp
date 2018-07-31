#include "api_dev_mod.h"
#include "../inc/tsys.h"

using namespace Cet;

// ============================================================================
// TaskHandler
// ============================================================================

TaskHandler::TaskHandler()
	: m_workthrd(nullptr)
	, m_rsp_idx(0)
	, m_req_idx(0)
	, m_rspcancel_idx(0)
	, m_reqcancel_idx(0)
	, m_wait_end(false)
	, m_keep_proc(true)
	, m_mtx(new std::mutex())
	, m_cv(new std::condition_variable())
{
	memset(m_tasks, 0, MAX_TASK_NUM);
	memset(m_cancel_tasks, 0, sizeof(m_cancel_tasks));
	m_workthrd = new std::thread(std::bind(&TaskHandler::proc_thrd, this));
}

TaskHandler::~TaskHandler()
{
	if (m_keep_proc)
	{
		m_keep_proc = false;
		m_cv->notify_all();
	}
	if (m_workthrd->joinable())
		m_workthrd->join();

	SAFE_DELETE(m_workthrd);
	SAFE_DELETE(m_cv);
	SAFE_DELETE(m_mtx);
}

void TaskHandler::add_task(const Task& task)
{
	bool need_notify = m_req_idx == m_rsp_idx;
	size_t idx = (m_req_idx + 1) % MAX_TASK_NUM;

	if (idx != m_rsp_idx)
	{
		m_tasks[m_req_idx] = task;
		m_req_idx = idx;

		if (need_notify)
		{
			m_cv->notify_all();
		}
	}
	else
	{
		throw std::exception("too much tasks");
	}
}

void TaskHandler::proc_thrd()
{
	while (m_keep_proc)
	{
		if (m_rsp_idx != m_req_idx)
		{ // there are some tasks to handle
			size_t idx = m_rsp_idx;
			m_rsp_idx = (m_rsp_idx + 1) % MAX_TASK_NUM;

			//LOG_D("hdlr = %u, rsp idx = %u, req idx = %u, task-id = %d",
			//	(unsigned int)this, m_rsp_idx, m_req_idx, m_tasks[idx].get_id());
			m_tasks[idx].response();
		}
		else
		{ // no task
			if (!m_wait_end)
			{
				std::unique_lock<std::mutex> lck(*m_mtx);
				m_cv->wait(lck);
			}
			else
			{
				break;
			}
		}
	}
	std::cout << "finished" << std::endl;
}

void TaskHandler::kill()
{
	if (m_keep_proc)
	{
		m_keep_proc = false;
		m_cv->notify_all();
	}
	if (m_workthrd->joinable())
		m_workthrd->join();
}

void TaskHandler::wait_end()
{
	m_wait_end = true;
	m_cv->notify_all();
	if (m_workthrd->joinable())
		m_workthrd->join();
}


// ============================================================================
// TaskManager
// ============================================================================

TaskManager::TaskManager(int handler_num)
	: m_handler_num(handler_num)
	, m_handlers(nullptr)
{
	m_handlers = new TaskHandler[m_handler_num];
}

TaskManager::~TaskManager()
{
	SAFE_DELETE_ARR(m_handlers);
}

TaskManager& TaskManager::get_instance()
{
	static TaskManager inst(4);
	return inst;
}

void TaskManager::add_task(const Task& task)
{
	static int idx = 0;
	m_handlers[idx].add_task(task);
	idx = ++idx % m_handler_num;
}

void TaskManager::wait_end()
{
	for (int i = 0; i < m_handler_num; ++i)
	{
		m_handlers[i].wait_end();
	}
}

