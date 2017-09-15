#pragma once

#include "args.h"
#include "inputs.h"

class oven
{
	public:
		void load_inputs();
		void fetch();
		void process(bool do_wiegley, bool do_fetch);
		period curr_period() const;
		vm_t m_vm;
	private:
		inputs_t user_inputs;
		yahoo_ts fetched_yahoos;
};

