#ifndef CET_EXT_NETWORKS_ASYS_H
#define CET_EXT_NETWORKS_ASYS_H

#include "../prerequisites.h"

namespace Cet
{
	namespace Ext_Net
	{
#if defined WIN32
		typedef SOCKET	cet_socket_t;
#endif

		class CET_API Receptionist
		{
		public:
			static Receptionist& get_instance();
			void ignite();

		protected:
			Receptionist();
			static void _do_accept(cet_socket_t listener, short event, void *arg);

		protected:
			static Receptionist* m_self;

			unsigned short	m_port;
			int				m_backlog;
		};

		static inline Receptionist& receptionist() { return Receptionist::get_instance(); }
	}
}

#endif // !CET_EXT_NETWORKS_ASYS_H