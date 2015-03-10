#ifndef GLMODULE_H_
#define GLMODULE_H_

#include "glEventChannel.h"

#include <iostream>
#include <string>

#define EXPORT_MODULE(module) \
	extern "C" glModule *export_##module() { return new module(#module); }
#define OBTAIN_MODULE(module) ("export_" + module)

#ifdef _WIN32 // windows likes declspec syntax, linux is ok with extern
#define DEFINE_MODULE(module) \
	extern "C" __declspec(dllexport) glModule * __cdecl export_##module();
#elif __unix
#define DEFINE_MODULE(module) \
	extern "C" extern glModule *export_##module();
#endif

class glModule {
public:
	glModule(const char *name)
		: _module_name(name) { }

	virtual ~glModule() { }
	virtual void startup() { std::cout << _module_name << " entering" << std::endl; }
	virtual void cleanup() { std::cout << _module_name << " exiting"  << std::endl; }

	static glEventChannel channel;
public:
	std::string _module_name;
};

#endif
