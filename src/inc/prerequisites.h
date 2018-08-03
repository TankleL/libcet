#ifndef CET_PREREQUISITES_H
#define CET_PREREQUISITES_H

#include <iostream>
#include <iomanip>
#include <memory>
#include <vector>
#include <list>
#include <string>
#include <sstream>
#include <unordered_map>
#include <unordered_set>
#include <map>
#include <set>
#include <queue>
#include <typeinfo>
#include <fstream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>

#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#ifdef WIN32
#	include <WinSock2.h>
#	include <WS2tcpip.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <assert.h>
#include <time.h>

#ifdef WIN32
#	include <objbase.h>
#endif // WIN32

#define SAFE_DELETE(X)		if(X){delete X; X = nullptr;}
#define SAFE_DELETE_ARR(X)	if(X){delete[] X; X = nullptr;}

#define BEGIN_TRY()			try{
#define	CTA()				} catch(...){throw;}

#ifdef WIN32
#	define __FILENAME__ (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)
#else
#	define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
#endif

#ifdef WIN32
#	ifdef API_DEV_MOD
#		define	CET_API	__declspec(dllexport)
#	else
#		define	CET_API	__declspec(dllimport)
#	endif
#else
#	define CET_API
#endif
#endif // !CET_PREREQUISITES_H
