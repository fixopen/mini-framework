#include "Heartbeat.h"

namespace utils {
	namespace {
		Monitor monitor_;
	}

	void setMonitor(Monitor const& monitor) {
		monitor_ = monitor;
	}

	void heartbeat(std::string const& content) {
		switch (monitor_.type) {
		case Monitor::Unknown:
			// do nothing
			break;
		case Monitor::Handle:
			// send message to monitor_.port.handle
			break;
		case Monitor::File:
			// write content to monitor_.port.file
			break;
		case Monitor::Socket:
			// send content to monitor_.port.socket
			break;
		}
	}
}
