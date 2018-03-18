## memcpy与strcpy区别
* strcpy  
```
char *strcpy(char *dest,const char *src);
```
* memcpy
```
void *memcpy(void *dest,const void *src,size_t count);
```

以上可以看出：
a. `strcpy只能应用字符类型的复制，而memcpy应用范围更广，任何类型都可以`；

b. `memcpy相比strcpy会更加安全`

c. `strcpy一定会拷贝字符串结尾符'\0'，memcpy在拷贝字符串的时候，根据指定拷贝字节数拷贝字符串，是否拷贝‘\0’结束符，根据count的大小;`

d. `在拷贝相同的字符串，且字节数相同(包括‘]0’)的情况下，strcpy效率比memcpy效率更快。`