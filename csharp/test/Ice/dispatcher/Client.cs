// **********************************************************************
//
// Copyright (c) 2003-2018 ZeroC, Inc. All rights reserved.
//
// This copy of Ice is licensed to you under the terms described in the
// ICE_LICENSE file included in this distribution.
//
// **********************************************************************

using System;
using System.Reflection;

[assembly: CLSCompliant(true)]

[assembly: AssemblyTitle("IceTest")]
[assembly: AssemblyDescription("Ice test")]
[assembly: AssemblyCompany("ZeroC, Inc.")]

public class Client : Test.TestHelper
{
    public override void run(string[] args)
    {
        try
        {
            Ice.InitializationData initData = new Ice.InitializationData();
            initData.properties = createTestProperties(ref args);
            initData.properties.setProperty("Ice.Warn.AMICallback", "0");
            //
            // Limit the send buffer size, this test relies on the socket
            // send() blocking after sending a given amount of data.
            //
            initData.properties.setProperty("Ice.TCP.SndSize", "50000");
            initData.dispatcher = new Dispatcher().dispatch;
            using(var communicator = initialize(initData))
            {
                AllTests.allTests(this);
            }
        }
        finally
        {
            Dispatcher.terminate();
        }
    }

    public static int Main(string[] args)
    {
        return Test.TestDriver.runTest<Client>(args);
    }
}
