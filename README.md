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

	mkdir build && cd build
	cmake ../src/
	make
	sudo make install

libcet will be installed into `/usr/local/lib`


## 2. How to use libcet

###Event Sample

sample source: *__test-cet-ev.cpp__*

	#include "libcet/inc/esys.h"
	
	using namespace Cet;
	
	// firstly, define a event-user which is responsible for specified events.
	class TestEV1 : public EventUser
	{
	public:
		TestEV1()
		{
			// register an event valued 100.
			// in this demo, register handler during construction.
			// but you can register event handler later.
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
	
	
	
	int main(int argc, char** argv)
	{
		TestEV1 tev1;	// initialize TestEV1.
	
		event_mgr().send_event(10);		// send an event valued 10. no one will handle this event.
		event_mgr().send_event(100);	// send an event valued 100. TestEV1 will handle it in on_ev_1().
	
		return 0;
	}

build script:

	g++ -std=c++11 test-cet-ev.cpp -lcet


running result:

	[cet@localhost test]$ ./a.out 
	on event 1.


