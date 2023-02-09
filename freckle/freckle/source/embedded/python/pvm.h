#ifndef PVM_H
#define PVM_H

#include <iostream>
#include <vector>
#include <string>

class python_vm
{
public:

	python_vm();

	~python_vm();

	bool initialize()  noexcept;

	void tick() noexcept;

	std::vector<std::string> preload;

private:

	std::atomic_bool _should_exit = false;

};


#endif