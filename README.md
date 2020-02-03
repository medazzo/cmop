## Linux, Cygwin, etc ...
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

