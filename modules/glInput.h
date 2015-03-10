#ifndef GLINPUT_H_
#define GLINPUT_H_

#include "../include/glModule.h"

class glInput : public glModule {
public:
	glInput(const char *name)
		: glModule(name) { }
	virtual ~glInput() { }

	virtual void startup() override { glModule::startup(); }
	virtual void cleanup() override { glModule::cleanup(); }
};

DEFINE_MODULE(glInput);

#endif
