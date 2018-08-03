#ifndef CET_MSGED_EXCEPTION_H
#define CET_MSGED_EXCEPTION_H

#include <exception>

namespace Cet
{
	class msged_exception : public std::exception
	{
	public:
		msged_exception(const std::string& msg)
			: m_msg(msg)
		{}

		virtual const char* what() const throw() override
		{
			return m_msg.c_str();
		}
	protected:
		std::string m_msg;
	};
}

#endif // !CET_MSGED_EXCEPTION_H
