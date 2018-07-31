/******************************************************************************
CSys - Command System
******************************************************************************/

#ifndef CET_CSYSTEM_H
#define CET_CSYSTEM_H

#include "prerequisites.h"

namespace Cet
{
	// ============================================================================
	// Command
	// ============================================================================
	class CommandManager;
	class CET_API Command
	{
		friend class CommandManager;
	public:
		Command(uint32_t id);
		virtual ~Command();
		virtual void proc(void* param) {};

	protected:
		uint32_t	m_cmd_id;
	};

	// ============================================================================
	// CommandManager
	// ============================================================================
	class CET_API CommandManager
	{
	public:
		virtual ~CommandManager();
		static CommandManager& get_instance();

	public:
		void register_cmd(uint32_t id, Command* cmd);
		void unregister_cmd(uint32_t id);
		void proc(uint32_t cmd_id, void* param);

	protected:
		CommandManager();

	protected:
		std::unordered_map<uint32_t, Command*>* m_cmds;
	};

	static inline CommandManager& cmd_mgr() { return CommandManager::get_instance(); }
}

#endif // !CET_CSYSTEM_H