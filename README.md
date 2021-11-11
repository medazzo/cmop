
# CMOP 

[![Build Status](https://travis-ci.org/medazzo/cmop.svg?branch=master)](https://travis-ci.org/medazzo/cmop)

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
```
> cd Tests/ByServer
> Tests/ByServer$ ../../Targets/x86-unknown-linux/Debug/ByServerTest 
ThirdParty/Neptune/Source/System/Posix/NptPosixThreads.cpp(399): [neptune.threads.posix] 2 [NPT_PosixThread] (140344135296832) FINE: NPT_PosixThread::NPT_PosixThread
Source/HTTPServer.cpp(36): [cmop.server]  [HTTPServer] (140344135296832) INFO: >>>>>>>>>>>>>>>>>>>>
Source/CMopServer.cpp(244): [cmop.apiserver]  [getCServer] (140344135296832) INFO: Server Created 
ThirdParty/Neptune/Source/System/Posix/NptPosixThreads.cpp(399): [neptune.threads.posix] [NPT_PosixThread] (140344135296832) FINE: NPT_PosixThread::NPT_PosixThread
Source/HTTPServer.cpp(36): [cmop.server]  [HTTPServer] (140344135296832) INFO: >>>>>>>>>>>>>>>>>>>>
Source/CMopServer.cpp(244): [cmop.apiserver]  [getCServer] (140344135296832) INFO: Server Created 
...
```


