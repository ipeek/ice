// **********************************************************************
//
// Copyright (c) 2003
// ZeroC, Inc.
// Billerica, MA, USA
//
// All Rights Reserved.
//
// Ice is free software; you can redistribute it and/or modify it under
// the terms of the GNU General Public License version 2 as published by
// the Free Software Foundation.
//
// **********************************************************************

#include <Ice/FactoryTableDef.h>

namespace Ice
{

FactoryTableWrapper factoryTableWrapper;	// Single global instance of the wrapper object that
						// initializes factoryTable.

ICE_API FactoryTableDef* factoryTable;		// Single global instance of the factory table for
						// non-local exceptions and non-abstract classes
}

//
// Add a factory to the exception factory table.
// If the factory is present already, increment its reference count.
//
void
Ice::FactoryTableDef::addExceptionFactory(const std::string& t, const UserExceptionFactoryPtr& f)
{
    IceUtil::StaticMutex::Lock lock(_m);
    EFTable::iterator i = _eft.find(t);
    if(i == _eft.end())
    {
	_eft[t] = EFPair(f, 1);
    }
    else
    {
	i->second.second++;
    }
}

//
// Return the exception factory for a given type ID
//
Ice::UserExceptionFactoryPtr
Ice::FactoryTableDef::getExceptionFactory(const std::string& t) const
{
    IceUtil::StaticMutex::Lock lock(_m);
    EFTable::const_iterator i = _eft.find(t);
    return i != _eft.end() ? i->second.first : UserExceptionFactoryPtr();
}

//
// Remove a factory from the exception factory table. If the factory
// is not present, do nothing; otherwise, decrement the factory's
// reference count if the count drops to zero, remove the factory's
// entry from the table.
//
void
Ice::FactoryTableDef::removeExceptionFactory(const std::string& t)
{
    IceUtil::StaticMutex::Lock lock(_m);
    EFTable::iterator i = _eft.find(t);
    if(i != _eft.end())
    {
	if(--i->second.second == 0)
	{
	    _eft.erase(i);
	}
    }
}

//
// Add a factory to the object factory table.
//
void
Ice::FactoryTableDef::addObjectFactory(const std::string& t, const ObjectFactoryPtr& f)
{
    IceUtil::StaticMutex::Lock lock(_m);
    OFTable::iterator i = _oft.find(t);
    if(i == _oft.end())
    {
	_oft[t] = OFPair(f, 1);
    }
    else
    {
	i->second.second++;
    }
}

//
// Return the object factory for a given type ID
//
Ice::ObjectFactoryPtr
Ice::FactoryTableDef::getObjectFactory(const std::string& t) const
{
    IceUtil::StaticMutex::Lock lock(_m);
    OFTable::const_iterator i = _oft.find(t);
    return i != _oft.end() ? i->second.first : ObjectFactoryPtr();
}

//
// Remove a factory from the object factory table. If the factory
// is not present, do nothing; otherwise, decrement the factory's
// reference count if the count drops to zero, remove the factory's
// entry from the table.
//
void
Ice::FactoryTableDef::removeObjectFactory(const std::string& t)
{
    IceUtil::StaticMutex::Lock lock(_m);
    OFTable::iterator i = _oft.find(t);
    if(i != _oft.end())
    {
	if(--i->second.second == 0)
	{
	    _oft.erase(i);
	}
    }
}

//
// The code generated by slice2cpp contains a file static instance of FactoryTable.
// The constructor of FactoryTable calls initialize(), as does the constructor of
// FactoryTableWrapper. This ensures that the global factoryTable variable is initialized
// before it can be used, regardless of the order of initialization of global objects.
//
Ice::FactoryTableWrapper::FactoryTableWrapper()
{
    initialize();
}

Ice::FactoryTableWrapper::~FactoryTableWrapper()
{
    finalize();
}

//
// Initialize the single global instance of the factory table, counting
// the number of calls made.
//
void
Ice::FactoryTableWrapper::initialize()
{
    IceUtil::StaticMutex::Lock lock(_m);
    if(_initCount == 0)
    {
	factoryTable = new FactoryTableDef;
    }
    ++_initCount;
}

//
// Delete the table if its reference count drops to zero.
//
void
Ice::FactoryTableWrapper::finalize()
{
    IceUtil::StaticMutex::Lock lock(_m);
    if(--_initCount == 0)
    {
	delete factoryTable;
    }
}

int Ice::FactoryTableWrapper::_initCount = 0;	// Initialization count
