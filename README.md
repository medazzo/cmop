## Linux, Cygwin, etc ...
Open a shell, go to the Platinum root directory and type 'scons' (http://scons.org).
```
> brew update || brew install scons
> git submodule update --init
> scons target={TARGET} build_config={Debug|Release}
```
