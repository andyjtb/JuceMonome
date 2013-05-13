
		#include <stdlib.h>
		#include <poll.h>

		int main(int argc, char **argv) {
		    struct pollfd fds[1];

		    fds[0].fd = open("/dev/null", 1);
		    fds[0].events = POLLIN;

		    if( poll(fds, 1, 0) < 0 || fds[0].revents & POLLNVAL )
		        exit(1);
		    exit(0);
		}