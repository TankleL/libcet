#ifndef CET_LOGGER_H
#define CET_LOGGER_H

#include "prerequisites.h"

#pragma warning(disable:4996)

namespace Cet
{
	class Logger
	{
	public:
		enum LogLevel
		{
			Debug,
			Info,
			Key,
			Error,
			// <---------- insert new enums here !
			LevelNum,	// this must be the last one enum.
		};

	public:
		static Logger* get_instance()
		{
			static Logger inst;
			return &inst;
		}

		virtual void log(LogLevel level, const std::thread::id& thrd_id, const char* file, const char* module, const int line, const char* fmt, ...)
		{
			std::ofstream* outstr(nullptr);
			std::map<std::thread::id, std::ofstream*>::const_iterator citer_outstr = m_outstrs.find(thrd_id);
			if (m_outstrs.end() == citer_outstr)
			{
				std::unique_lock<std::mutex> lock(m_mtx_outstr);
				
				outstr = new std::ofstream(_get_new_logname(), std::ios::app);
				m_outstrs[thrd_id] = outstr;

				_touch_start_tag(*outstr);
			}
			else
			{
				outstr = citer_outstr->second;
			}

			char* temp_buf(nullptr);
			std::map<std::thread::id, char*>::const_iterator citer_temp_buf = m_temp_bufs.find(thrd_id);
			if (m_temp_bufs.end() == citer_temp_buf)
			{
				std::unique_lock<std::mutex> lock(m_mtx_tempbuf);
				temp_buf = new char[MAX_LOG_STR_LEN];
				m_temp_bufs[thrd_id] = temp_buf;
			}
			else
			{
				temp_buf = citer_temp_buf->second;
			}

			va_list args;
			va_start(args, fmt);

			switch (level)
			{
			case Debug:
				log_debug(*outstr, temp_buf, file, module, line, fmt, args);
				break;

			case Info:
				log_info(*outstr, temp_buf, file, module, line, fmt, args);
				break;

			case Key:
				log_key(*outstr, temp_buf, file, module, line, fmt, args);
				break;

			case Error:
				log_error(*outstr, temp_buf, file, module, line, fmt, args);
				break;

			default:
				assert(false);	// miss log level
			}
			va_end(args);
		}

	protected:
		void log_debug(
			std::ofstream& outstr,
			char* temp_buf,
			const char* file, 
			const char* module,
			const int line,
			const char* fmt,
			va_list args)
		{
			vsprintf(temp_buf, fmt, args);
			std::ostringstream oss;
			oss << get_time() << "|Debug|"
				<< file << "@" << line << ": "
				<< temp_buf << std::endl;
			outstr << oss.str();
#ifdef WIN32
			OutputDebugStringA(oss.str().c_str());
#endif
		}

		void log_info(
			std::ofstream& outstr,
			char* temp_buf,
			const char* file,
			const char* module,
			const int line,
			const char* fmt,
			va_list args)
		{
			vsprintf(temp_buf, fmt, args);
			std::ostringstream oss;
			oss << get_time() << "|Info|"
				<< file << "@" << line << ": "
				<< temp_buf << std::endl;
			outstr << oss.str();
#ifdef WIN32
			OutputDebugStringA(oss.str().c_str());
#endif
		}

		void log_key(
			std::ofstream& outstr,
			char* temp_buf,
			const char* file,
			const char* module,
			const int line,
			const char* fmt,
			va_list args)
		{
			vsprintf(temp_buf, fmt, args);
			std::ostringstream oss;
			oss << get_time() << "|Key|"
				<< file << "@" << line << ": "
				<< temp_buf << std::endl;
			outstr << oss.str();
#ifdef WIN32
			OutputDebugStringA(oss.str().c_str());
#endif
		}

		void log_error(
			std::ofstream& outstr,
			char* temp_buf,
			const char* file,
			const char* module,
			const int line,
			const char* fmt,
			va_list args)
		{
			vsprintf(temp_buf, fmt, args);
			std::ostringstream oss;
			oss << get_time() << "|Error|"
				<< file << "@" << line << ": "
				<< temp_buf << std::endl;
			outstr << oss.str();
#ifdef WIN32
			OutputDebugStringA(oss.str().c_str());
#endif
		}

		std::string get_time()
		{
			timeval tv;
			memset(&tv, 0, sizeof(tv));
//			evutil_gettimeofday(&tv, nullptr);
			time_t ti = time(nullptr);
			tm* view_time = localtime(&ti);

			char buf[64] = { 0 };
			sprintf(buf, "%02d:%02d:%02d.%03d", view_time->tm_hour,
				view_time->tm_min, view_time->tm_sec, tv.tv_usec / 1000);
			return buf;
		}

		std::string _get_new_logname()
		{
			static int count = 0;
			std::ostringstream oss;
			oss << m_name_prefix << "channel-" << count << ".log";
			++count;
			return oss.str();
		}

		void _touch_start_tag(std::ofstream& outstr)
		{
			outstr
				<< "========================================" << std::endl
				<< "#             START LOGGING            #" << std::endl
				<< "#          " << "time[" << get_time() << "]" << "          #" << std::endl
				<< "----------------------------------------" << std::endl;
		}

		void _touch_stop_tag(std::ofstream& outstr)
		{
			outstr
				<< "----------------------------------------" << std::endl
				<< "#             STOP  LOGGING            #" << std::endl
				<< "#          " << "time[" << get_time() << "]" << "          #" << std::endl
				<< "========================================" << std::endl;
		}

	protected:
		Logger()
			: m_name_prefix("logs/dev-")
		{}

	public:
		virtual ~Logger()
		{
			for (auto& outstr : m_outstrs)
			{
				_touch_stop_tag(*outstr.second);
				outstr.second->close();
				delete outstr.second;
			}

			for (auto& buf : m_temp_bufs)
			{
				delete buf.second;
			}
		}

	protected:
		static const size_t							MAX_LOG_STR_LEN = 4096;
		std::map<std::thread::id, char*>			m_temp_bufs;
		std::map<std::thread::id, std::ofstream*>	m_outstrs;
		std::mutex									m_mtx_outstr;
		std::mutex									m_mtx_tempbuf;
		std::string									m_name_prefix;
	};

}


#ifdef _DEBUG
#	define LOG_D(fmt, ...)	CET::Logger::get_instance()->log(\
		Cet::Logger::Debug, std::this_thread::get_id(), __FILENAME__, __FUNCTION__, __LINE__, fmt, __VA_ARGS__)
#else
#	define LOG_D(fmt, ...)
#endif

#define LOG_I(fmt, ...)	Cet::Logger::get_instance()->log(\
	Cet::Logger::Info, std::this_thread::get_id(), __FILENAME__, __FUNCTION__, __LINE__, fmt, __VA_ARGS__)
#define LOG_K(fmt, ...)	Cet::Logger::get_instance()->log(\
	Cet::Logger::Key, std::this_thread::get_id(), __FILENAME__, __FUNCTION__, __LINE__, fmt, __VA_ARGS__)
#define LOG_E(fmt, ...)	Cet::Logger::get_instance()->log(\
	Cet::Logger::Error, std::this_thread::get_id(), __FILENAME__, __FUNCTION__, __LINE__, fmt, __VA_ARGS__)

#endif // !CET_LOGGER_H
