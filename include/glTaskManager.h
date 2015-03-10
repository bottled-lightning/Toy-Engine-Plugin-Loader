#ifndef GLTASKMANAGER_H_
#define GLTASKMANAGER_H_

class glEventChannel;

class glTaskManager {
public:
	glTaskManager(glEventChannel &channel) :
		_channel(channel), _running(false) { }

	void sendStartSignal() { _running = true; }
	bool shouldClose() { return false; }
	void pollEvents() { }

private:
	glEventChannel &_channel;
	bool _running;
};

#endif
