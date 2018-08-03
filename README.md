# libcet
A Simple Implementation of Command-Event-Task structure

## 1. How to build libcet
Use cmake to construct solution-files or make-files.

Windows

- a) Create a new directory named "build" under the libcet root path.    
`mkdir build && cd build`

- b) CMake visual stuido solution. (using visual studio 2017 for example)   
`cmake -G "Visual Studio 15 2017" ../src/`   
Tips: If you want to choose other -G flag params, you can type `cmake --help` and see the supported params.

- c) Buid libcet via visual studio.

Linux(CentOS 7)


## 2. How to use libcet
###Event Sample

sample source: *__test-cet-ev.cpp__*

	#include "libcet/inc/esys.h"
	
	using namespace Cet;
	
	class TestEV1 : public EventUser
	{
	public:
		TestEV1()
		{
			register_event(100, (EventCallBackFn)&TestEV1::on_ev_1);
		}
	
	protected:
		std::shared_ptr<EventResult> on_ev_1(const Cet::EventParam& param, int trace_id)
		{
			std::shared_ptr<EventResult> res = std::make_shared<EventResult>();
			std::cout << "on event 1." << std::endl;
			return res;
		}
	};
	
	
	
	int main()
	{
		TestEV1 tev1;
	
		event_mgr().send_event(10);
		event_mgr().send_event(100);
		
	
	    return 0;
	}

build script:

	g++ -std=c++11 test-cet-ev.cpp -lcet


running result:

	[cet@localhost test]$ ./a.out 
	on event 1.


