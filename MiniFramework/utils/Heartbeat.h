#pragma once

#include <string>
#include <stdio.h> // for FILE
#include <WinSock2.h> // for SOCKET

namespace utils {
	struct Monitor {
		enum Type {
			Unknown,
			Handle,
			File,
			Socket
		};

		union Port {
			void* unknown;
			HANDLE handle;
			FILE* file;
			SOCKET socket;
		};

		std::string name;
		Type type;
		Port port;
	};

	void setMonitor(Monitor const& monitor);
	void heartbeat(std::string const& content = "");

	class Heartbeat {
		// monitor handle
		// send heartbeat to monitor freq
		// if self is monitor
		//	  receive heartbeat, if not received, restart the object
	};
}
