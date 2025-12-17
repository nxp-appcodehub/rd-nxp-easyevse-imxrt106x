/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef OCPPASIOCONTEXT_HPP_
#define OCPPASIOCONTEXT_HPP_

//replaces boost::asio::io_context
class OcppAsioContext {
public:
	OcppAsioContext(){}
	~OcppAsioContext(){}

	void run()
	{
		return;
	}

	void stop()
	{
		return;
	}

private:
	uint32_t io_context;
};


#endif /* OCPPASIOCONTEXT_HPP_ */
