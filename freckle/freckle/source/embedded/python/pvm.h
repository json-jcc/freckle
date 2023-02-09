#ifndef PVM_H
#define PVM_H

#include <iostream>

namespace python_api
{
	namespace basic
	{

	}

	class python_vm
	{
	public:

		bool initialize()  noexcept;

		void run() noexcept;

	public:

		void notify_to_exit() noexcept;

	private:

		std::atomic_bool _should_exit = false;

	};

}


#endif