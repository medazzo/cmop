
# CMOP 

[![Build Status](https://travis-ci.org/medazzo/sbr-generator.svg?branch=master)](https://travis-ci.org/medazzo/sbr-generator)

A Http REst Server basend on LibNptune C++

# Linux, Cygwin, etc ...
First,install 'scons' (http://scons.org). </br>
Then  go to cmopo Directory and run
```
> git submodule update --init
> scons target={TARGET} build_config={Debug|Release}
```

To build library and build tests Server :

```
> scons target=x86-unknown-linux  CMOP
> cons target=x86-unknown-linux  Tests/ByServer
```

To Run test, better to cnahge dir to get Neptune logs :
> cd Tests/ByServer
> Tests/ByServer$ ../../Targets/x86-unknown-linux/Debug/ByServerTest 
ThirdParty/Neptune/Source/System/Posix/NptPosixThreads.cpp(399): [neptune.threads.posix] 2020-02-05T12:01:05.984938000+01:00 [NPT_PosixThread] (140344135296832) FINE: NPT_PosixThread::NPT_PosixThread
Source/HTTPServer.cpp(36): [cmop.server] 2020-02-05T12:01:05.985027000+01:00 [HTTPServer] (140344135296832) INFO: >>>>>>>>>>>>>>>>>>>>
Source/CMopServer.cpp(244): [cmop.apiserver] 2020-02-05T12:01:05.985042000+01:00 [getCServer] (140344135296832) INFO: Server Created 
ThirdParty/Neptune/Source/System/Posix/NptPosixThreads.cpp(399): [neptune.threads.posix] 2020-02-05T12:01:05.985071000+01:00 [NPT_PosixThread] (140344135296832) FINE: NPT_PosixThread::NPT_PosixThread
Source/HTTPServer.cpp(36): [cmop.server] 2020-02-05T12:01:05.985085000+01:00 [HTTPServer] (140344135296832) INFO: >>>>>>>>>>>>>>>>>>>>
Source/CMopServer.cpp(244): [cmop.apiserver] 2020-02-05T12:01:05.985097000+01:00 [getCServer] (140344135296832) INFO: Server Created 
...
```


