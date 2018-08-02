#ifndef CET_EXT_NETWORKS_MSG_H
#define CET_EXT_NETWORKS_MSG_H

#include "../prerequisites.h"

namespace Cet
{
	namespace Ext_Net
	{
		class CET_API Message
		{
		public:
			enum DataType : unsigned short
			{
				Unknown,
				Int32,
				Int64,
				String,
			};

		public:
			Message();
			Message(const Message& rhs);
			Message(Message&& rhs);
			Message(int32_t msg);
			Message(int64_t msg);
			Message(const std::string& msg);
			Message(const unsigned char* data, unsigned short len);
			~Message();

			Message& operator=(const Message& rhs);
			Message& operator=(Message&& rhs);

			std::string to_string() const;
			int32_t to_int32() const;
			int64_t to_int64() const;

			unsigned short size() const;
			unsigned char* buffer(unsigned short offset = 0) const;

		protected:
			DataType		m_type;
			unsigned short	m_size;		// one message must be no longer than 65536 bytes
			unsigned char*	m_data;
		};
	}
}
#endif // !CET_EXT_NETWORKS_MSG_H