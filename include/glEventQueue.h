#ifndef GLEVENTQUEUE_H_
#define GLEVENTQUEUE_H_

class glEventQueue {
public:
	glEventQueue(glEventChannel &channel)
		: _channel(channel) { }
private:
	glEventChannel &_channel;
};

#endif
