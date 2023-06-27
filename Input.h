#ifndef INPUT_H_
#define INPUT_H_
#include <iostream>
#include <functional>

void fToStop(std::function<void()> interruptFunction) {
	while (std::cin.get() != 'f') {
		;
	}
	interruptFunction();
}

#endif