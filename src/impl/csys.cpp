#include "api_dev_mod.h"

#include "../inc/csys.h"
#include "../inc/msged-exception.h"

using namespace Cet;

// ============================================================================
// Command
// ============================================================================
Command::Command(uint32_t id)
	: m_cmd_id(id)
{
	BEGIN_TRY();
	cmd_mgr().register_cmd(m_cmd_id, this);
	CTA();
}

Command::~Command()
{
	cmd_mgr().unregister_cmd(m_cmd_id);
}

// ============================================================================
// CommandManager
// ============================================================================

CommandManager::CommandManager()
	: m_cmds(new std::unordered_map<uint32_t, Command*>())
{}

CommandManager::~CommandManager()
{
	SAFE_DELETE(m_cmds);
}

CommandManager& CommandManager::get_instance()
{
	static CommandManager inst;
	return inst;
}

void CommandManager::register_cmd(uint32_t id, Command* cmd)
{
	Command* rcmd = (*m_cmds)[id];
	if (rcmd)
		throw msged_exception("cmd-id conflicted.");
	else
		(*m_cmds)[id] = cmd;
}

void CommandManager::unregister_cmd(uint32_t id)
{
	(*m_cmds)[id] = nullptr;
}

void CommandManager::proc(uint32_t cmd_id, void* param)
{
	Command* rcmd = (*m_cmds)[cmd_id];
	if (rcmd)
		rcmd->proc(param);
}