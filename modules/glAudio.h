#ifndef GLAUDIO_H_
#define GLAUDIO_H_

#include "../include/glModule.h"

class glAudio : public glModule {
public:
	glAudio(const char *name)
		: glModule(name) { }
	virtual ~glAudio() { }

	virtual void startup() override { glModule::startup(); }
	virtual void cleanup() override { glModule::cleanup(); }
};

DEFINE_MODULE(glAudio);

#endif
