# /dev/null 的含义和用途

`/dev/null代表空设备文件，它等价于一个只写文件，所有写入它的内容都会永远丢失，  
而尝试从它那儿读取内容则什么也读不到`

* 0：stdin  
* 1: stdout(系统默认是1)  
* 2：stderr  

* 1 > /dev/null 等同于 >/dev/null  
* 2 > /dev/null 2 > /dev/null 等同于 >/dev/null 2 > &1  
```
&1:&表示等同于的意思，2>&1,表示2的输出重定向等于1
```

### 举例
```
cat $filename >/dev/null 2 >/dev/null 等同于 cat $filename >/dev/null 2 > &1

# 如果 filename不存在，将不会有任何错误信息提示
# 如果 filename存在，文件的内容不会打印到标准输出
# 因此,上边代码根本不会输出任何信息
```
