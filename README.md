# vc2019

Para compilar:  
```$ cd vc2019  
$ cd <app>  
$ g++ `pkg-config opencv --cflags` <app.cpp> -o <app> `pkg-config opencv --libs`
```
