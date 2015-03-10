#ifndef GLVIDEO_H_
#define GLVIDEO_H_

#include "../include/glModule.h"

class glVideo : public glModule {
public:
	glVideo(const char *name)
		: glModule(name) { }
	virtual ~glVideo() { }

	virtual void startup() override { glModule::startup(); }
	virtual void cleanup() override { glModule::cleanup(); }
};

DEFINE_MODULE(glVideo);

#endif
