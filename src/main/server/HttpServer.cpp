

#include "HttpServer.hpp"

void HTTPServer::process() {
	int poll_res;
//	std::cout << GREEN << "SERVER IS WORKING NOW!" << SHALLOW << std::endl;
//	std::cout << GREEN << "_________________________________________________________________________________________________" << SHALLOW << std::endl;
	while (true) {
		poll_res = poll(fds.data(), nfds, timeout);
		_error_handler(poll_res, "poll() failed");
		int current_size = nfds;

		for (int clientIndex = 0; clientIndex < current_size; ++clientIndex) {
			if (isNothingEvents(clientIndex))
				continue;
			else if ((fds[clientIndex].revents & POLLHUP)) {
				_clients[fds[clientIndex].fd]->setNeedToRemove(true);
			}
				// isReadable
			else if (fds[clientIndex].revents & POLLIN) {
				if (isListeningSocket(fds[clientIndex].fd))
					accepter(clientIndex);
				else
					client_socket_handler(clientIndex);
			}
			// isWriteable
			if (fds[clientIndex].revents & POLLOUT)
				sender(clientIndex);
		}

		close_connection(); // if some connection was closed
		if (end_server)
			break;
	}

}

bool HTTPServer::isNothingEvents(int clientIndex) {
	return fds[clientIndex].revents == 0;
}
