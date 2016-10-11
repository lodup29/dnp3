/*
 * Licensed to Green Energy Corp (www.greenenergycorp.com) under one or
 * more contributor license agreements. See the NOTICE file distributed
 * with this work for additional information regarding copyright ownership.
 * Green Energy Corp licenses this file to you under the Apache License,
 * Version 2.0 (the "License"); you may not use this file except in
 * compliance with the License.  You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * This project was forked on 01/01/2013 by Automatak, LLC and modifications
 * may have been made to this file. Automatak, LLC licenses these modifications
 * to you under the terms of the License.
 */

#include "asiodnp3/StackBase.h"

namespace asiodnp3
{

StackBase::StackBase(
        const openpal::Logger& logger, 
        const std::shared_ptr<asiopal::StrandExecutor>& executor,
	    const std::shared_ptr<opendnp3::ILinkListener>& listener,
        const std::shared_ptr<IOHandler>& iohandler,
	    uint32_t maxRxFragSize,
	    const opendnp3::LinkConfig& config
) :
	logger(logger),
	statistics(),
	executor(executor),
	iohandler(iohandler),
	tstack(logger, *executor.get(), *listener.get(),  maxRxFragSize, &statistics, config)
{}

bool StackBase::Enable()
{
	auto task = [this]() -> bool
	{
		return this->iohandler->Enable(this->tstack.link);
	};

	return this->executor->ReturnFrom<bool>(task);
}

bool StackBase::Disable()
{
	auto task = [this]() -> bool
	{
		return this->iohandler->Disable(this->tstack.link);
	};

	return this->executor->ReturnFrom<bool>(task);
}

void StackBase::Shutdown()
{
	auto task = [this]() -> bool
	{
		return this->iohandler->Remove(this->tstack.link);
	};

	auto remove = this->executor->ReturnFrom<bool>(task);

	// TODO?
}

opendnp3::StackStatistics StackBase::GetStackStatistics()
{
	auto get = [this] () -> opendnp3::StackStatistics { return this->statistics; };

	return this->executor->ReturnFrom<opendnp3::StackStatistics>(get);
}

}


