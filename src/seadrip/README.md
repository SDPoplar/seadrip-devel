# seadrip-devel
some self-use common classes for linux

## Dependence
Most of classes includes boost lib, make sure you've installed boost-devel first
>these may works:
```
yum install boost
yum install boost-devel
yum install boost-regex
```
### Compile with gcc-c++
You may need
```
g++ -std=c++11 -lboost_regex ...
```
