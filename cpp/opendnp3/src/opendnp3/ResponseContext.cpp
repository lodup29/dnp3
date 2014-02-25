/**
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
#include "ResponseContext.h"

#include <openpal/Serialization.h>

#include "WriteConversions.h"

using namespace openpal;

namespace opendnp3
{

ResponseContext::ResponseContext(Database* pDatabase_, const StaticResponseTypes& rspTypes_) : 
	fragmentCount(0),	
	pDatabase(pDatabase_),
	rspTypes(rspTypes_)
{}

bool ResponseContext::IsComplete() const
{
	return staticResponseQueue.IsEmpty();
}

void ResponseContext::Reset()
{
	fragmentCount = 0;	
	staticResponseQueue.Clear();
}

QueueResult ResponseContext::QueueReadAllObjects(GroupVariation gv)
{
	switch(gv)
	{
		case(GroupVariation::Group60Var1):		
			return QueueStaticIntegrity();	
		
		// Group 1
		case(GroupVariation::Group1Var0):
			return QueueReadRange(GetFullRangeWithDefaultLoader<Binary>());		
		case(GroupVariation::Group1Var2):
			return QueueReadRange(GetFullRange<Group1Var2Serializer>());		

		// Group 10
		case(GroupVariation::Group10Var0):
			return QueueReadRange(GetFullRangeWithDefaultLoader<ControlStatus>());
		case(GroupVariation::Group10Var2):
			return QueueReadRange(GetFullRange<Group10Var2Serializer>());
		
		// Group 20
		case(GroupVariation::Group20Var0):
			return QueueReadRange(GetFullRangeWithDefaultLoader<Counter>());
		case(GroupVariation::Group20Var1):
			return QueueReadRange(GetFullRange<Group20Var1Serializer>());
		case(GroupVariation::Group20Var2):
			return QueueReadRange(GetFullRange<Group20Var2Serializer>());
		case(GroupVariation::Group20Var5):
			return QueueReadRange(GetFullRange<Group20Var5Serializer>());
		case(GroupVariation::Group20Var6):
			return QueueReadRange(GetFullRange<Group20Var6Serializer>());

		// Group 21
		case(GroupVariation::Group21Var0):
			return QueueReadRange(GetFullRangeWithDefaultLoader<FrozenCounter>());
		case(GroupVariation::Group21Var1):
			return QueueReadRange(GetFullRange<Group21Var1Serializer>());
		case(GroupVariation::Group21Var2):
			return QueueReadRange(GetFullRange<Group21Var2Serializer>());
		case(GroupVariation::Group21Var5):
			return QueueReadRange(GetFullRange<Group21Var5Serializer>());
		case(GroupVariation::Group21Var6):
			return QueueReadRange(GetFullRange<Group21Var6Serializer>());
		case(GroupVariation::Group21Var9):
			return QueueReadRange(GetFullRange<Group21Var9Serializer>());
		case(GroupVariation::Group21Var10):
			return QueueReadRange(GetFullRange<Group21Var10Serializer>());

		// Group 30
		case(GroupVariation::Group30Var0):
			return QueueReadRange(GetFullRangeWithDefaultLoader<Analog>());
		case(GroupVariation::Group30Var1):
			return QueueReadRange(GetFullRange<Group30Var1Serializer>());
		case(GroupVariation::Group30Var2):
			return QueueReadRange(GetFullRange<Group30Var2Serializer>());
		case(GroupVariation::Group30Var3):
			return QueueReadRange(GetFullRange<Group30Var3Serializer>());
		case(GroupVariation::Group30Var4):
			return QueueReadRange(GetFullRange<Group30Var4Serializer>());
		case(GroupVariation::Group30Var5):
			return QueueReadRange(GetFullRange<Group30Var5Serializer>());
		case(GroupVariation::Group30Var6):
			return QueueReadRange(GetFullRange<Group30Var6Serializer>());

		// Group 40
		case(GroupVariation::Group40Var0):
			return QueueReadRange(GetFullRangeWithDefaultLoader<SetpointStatus>());
		case(GroupVariation::Group40Var1):
			return QueueReadRange(GetFullRange<Group40Var1Serializer>());
		case(GroupVariation::Group40Var2):
			return QueueReadRange(GetFullRange<Group40Var2Serializer>());
		case(GroupVariation::Group40Var3):
			return QueueReadRange(GetFullRange<Group40Var3Serializer>());
		case(GroupVariation::Group40Var4):
			return QueueReadRange(GetFullRange<Group40Var4Serializer>());

		default:
			return QueueResult::OBJECT_UNDEFINED;
	}
}

QueueResult ResponseContext::QueueReadRange(GroupVariation gv, const StaticRange& range)
{
	return QueueResult::OUT_OF_RANGE;
}

QueueResult ResponseContext::QueueStaticIntegrity()
{
	StaticQueue<StaticRangeLoader, uint32_t, 6> values;		
	values.Push(GetFullRangeWithDefaultLoader<Binary>());
	values.Push(GetFullRangeWithDefaultLoader<Counter>());
	values.Push(GetFullRangeWithDefaultLoader<FrozenCounter>());
	values.Push(GetFullRangeWithDefaultLoader<Analog>());
	values.Push(GetFullRangeWithDefaultLoader<ControlStatus>());
	values.Push(GetFullRangeWithDefaultLoader<SetpointStatus>());

	while(values.IsNotEmpty())
	{		
		auto loader = values.Pop();
		if(loader.IsDefined()) 
		{
			auto result = QueueReadRange(loader);
			if(result != QueueResult::SUCCESS) return result;
		}
	}
	
	return QueueResult::SUCCESS;
}

QueueResult ResponseContext::QueueReadRange(const StaticRangeLoader& loader)
{	
	if(loader.IsDefined())
	{
		return staticResponseQueue.Push(loader) ? QueueResult::SUCCESS : QueueResult::FULL;	
	}
	else return QueueResult::OUT_OF_RANGE;
}

StaticLoadResult ResponseContext::Load(APDUResponse& response)
{	
	auto writer = response.GetWriter();
	auto result = LoadStaticData(writer);
	auto control = GetAppControl(fragmentCount, result);
	response.SetControl(control);
	++fragmentCount;
	return result;
}

AppControlField ResponseContext::GetAppControl(uint32_t headerCount, StaticLoadResult result)
{
	bool fir = (headerCount == 0);
	bool fin = (result != StaticLoadResult::FULL);
	return AppControlField(fir, fin, !fin, false, 0);
}

StaticLoadResult ResponseContext::LoadStaticData(ObjectWriter& writer)
{		
	while(!staticResponseQueue.IsEmpty())
	{				
		auto& front = staticResponseQueue.Front();
		auto result = (*front.pLoadFun)(writer, front, *pDatabase);
		if(result == StaticLoadResult::COMPLETED)
		{
			staticResponseQueue.Pop();
			if(staticResponseQueue.IsEmpty()) return StaticLoadResult::COMPLETED;
			else continue;
		}
		else return result;
	}

	return StaticLoadResult::EMPTY;
}


}


