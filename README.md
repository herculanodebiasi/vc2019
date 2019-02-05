# vc2019

Para compilar:  
```
$ cd vc2019/<app>  
$ g++ `pkg-config opencv --cflags` <app.cpp> -o <app> `pkg-config opencv --libs`
```
