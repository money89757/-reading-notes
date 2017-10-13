# I2C子系统 #
```
用户空间         应用程序
————————————————————||——————————————————————————————————
内核空间			\/
				  系统调用
					||
					\/
				  虚拟文件系统
					||
					\/
			      设备驱动层(外设驱动)
				  核心层(承上启下)
				  总线驱动层(I2C控制器驱动)
—————————————————————||—————————————————————————————————
硬件			   I2C控制器 		
					 || I2C总线
					 ||
				   一些I2C设备
```
总结:  
设备驱动层:  
1. 给用户空间提供接口  
2. 知道发什么数据，但是不知道如何发数据  

核心层:  
1. 向上给设备驱动提供接口  
2. 向下给总线驱动提供接口  

总线驱动层  
1. 操作I2C控制器(使能I2C总线)  
2. 知道如何发数据，但是不知道发什么数据  

# 设备驱动我们自己写的，核心层和总线驱动是内核实现的。#
### 总线驱动:vi drivers/i2c/busses/i2c-s3c2410.c ###
```c
287 #define subsys_initcall(fn)     module_init(fn)

1290 subsys_initcall(i2c_adap_s3c_init); 		
	return platform_driver_register(&s3c24xx_i2c_driver); 注册到platform驱动中。

static struct platform_driver s3c24xx_i2c_driver = {
1275     .probe      = s3c24xx_i2c_probe,   如果i2c总线设备信息和驱动匹配执行probe函数
1276     .remove     = s3c24xx_i2c_remove,
1277     .id_table   = s3c24xx_driver_ids,
1278     .driver     = {
1279         .owner  = THIS_MODULE,
1280         .name   = "s3c-i2c",
1281         .pm = S3C24XX_DEV_PM_OPS,
1282         .of_match_table = of_match_ptr(s3c24xx_i2c_match),                 
1283     },
1284 };

1072 static int s3c24xx_i2c_probe(struct platform_device *pdev)
1073 {
1074     struct s3c24xx_i2c *i2c;
				==》struct i2c_adapter  adap;代表了当前操作的i2c适配器
}               		==》const struct i2c_algorithm *algo;
								int (*master_xfer)(struct i2c_adapter *adap, struct i2c_msg *msgs,int num);



		1107     i2c->adap.algo    = &s3c24xx_i2c_algorithm;
									==》 787 static const struct i2c_algorithm s3c24xx_i2c_algorithm = {                
										 788     .master_xfer        = s3c24xx_i2c_xfer,
										 789     .functionality      = s3c24xx_i2c_func,
										 790 };

	}

进入到s3c24xx_i2c_xfer
760 ret = s3c24xx_i2c_doxfer(i2c, msgs, num)我们写的驱动只需要调用到这个函数就可以了
{
	695     ret = s3c24xx_i2c_set_master(i2c); 判断I2C总线是否空闲，如果空间则返回
	708     s3c24xx_i2c_enable_irq(i2c); 使能I2C总线发送和接收数据相关的中断。
	709     s3c24xx_i2c_message_start(i2c, msgs);  
				264     stat |=  S3C2410_IICSTAT_TXRXEN; 使能I2C总线的读/写
				270         stat |= S3C2410_IICSTAT_MASTER_TX; 主机传输模式
				276     s3c24xx_i2c_enable_ack(i2c); 设置应答位
				292     stat |= S3C2410_IICSTAT_START;产生起始信号
	720     timeout = wait_event_timeout(i2c->wait, i2c->msg_num == 0, HZ * 5);	如果总线繁忙当前从机设备需要等待
}

重新回到probe函数中
{
	1172     ret = devm_request_irq(&pdev->dev, i2c->irq, s3c24xx_i2c_irq, 0,dev_name(&pdev->dev), i2c);
														  中断处理函数
}																||
																\/
														598     i2c_s3c_irq_nextbyte(i2c, status);
																		||
																		\/
																411     switch (i2c->state) {
																 412
																 413     case STATE_IDLE:
																 414         dev_err(i2c->dev, "%s: called in STATE_IDLE\n", __func__);
																 415         goto out;
																 416
																 417     case STATE_STOP:
																 418         dev_err(i2c->dev, "%s: called in STATE_STOP\n", __func__);
																 419         s3c24xx_i2c_disable_irq(i2c);
																 420         goto out_ack;
																 421
																 422     case STATE_START:
																		   ...........
																		   如果匹配了STATE_START，后续要去执行传输数据或者接收数据。
																}
总结:I2C总线驱动层完成了，封装了I2C从机地址、读写位、应答位、起始信号和结束信号、如果是起始信号实现了传输数据的方法、接收数据的方法

```

### 核心层:drivers/i2c/i2c-core.c ###
```c

1694 postcore_initcall(i2c_init);
i2c_init是核心层驱动的入口
1658     retval = bus_register(&i2c_bus_type);//在/sys/bus/i2c目录下创建两个文件夹:devices和drivers

1668     retval = i2c_add_driver(&dummy_driver);
						||
						\/
				512 #define i2c_add_driver(driver) \                                            
				513     i2c_register_driver(THIS_MODULE, driver)
								||
								\/
				1542     driver->driver.bus = &i2c_bus_type;

161 struct i2c_driver {描述的是设备驱动
170     int (*probe)(struct i2c_client *, const struct i2c_device_id *);
171     int (*remove)(struct i2c_client *);
190     struct device_driver driver;
191     const struct i2c_device_id *id_table;
}

454 struct bus_type i2c_bus_type = {                                                                                          
 455     .name       = "i2c",
 456     .match      = i2c_device_match,
 457     .probe      = i2c_device_probe,
 458     .remove     = i2c_device_remove,
 459     .shutdown   = i2c_device_shutdown,
 460     .pm     = &i2c_device_pm_ops,
 461 };      

1547     res = driver_register(&driver->driver);
					||
					\/
		154     if ((drv->bus->probe && drv->probe) || 第一个probe被i2c核心层初始化，第二个probe没有被初始化

		160     other = driver_find(drv->name, drv->bus);
							||
							\/
						212     struct kobject *k = kset_find_obj(bus->p->drivers_kset, name);
																				   自己在从机驱动中的初始化的字符串
														||
														\/
													 831     list_for_each_entry(k, &kset->list, entry) {
													 832         if (kobject_name(k) && !strcmp(kobject_name(k), name)) {
													 833             ret = kobject_get_unless_zero(k);
													 834             break;
													 835         }
													 836     }
													 验证从机驱动事先是否被注册过。

		167     ret = bus_add_driver(drv);  
						||
						\/
				 690     klist_add_tail(&priv->knode_bus, &bus->p->klist_drivers);
				 691     if (drv->bus->p->drivers_autoprobe) {
				 692         error = driver_attach(drv);  
				}						||
										\/
							479     return bus_for_each_dev(drv->bus, NULL, drv, __driver_attach);
										||
										\/
							 298 int bus_for_each_dev(struct bus_type *bus, struct device *start,
							 299              void *data, int (*fn)(struct device *, void *))
							 300 {   
							 310     while ((dev = next_device(&i)) && !error)
							 311         error = fn(dev, data); 循环调用fn函数指针，fn被__driver_attach赋值，所以我们只需要关心__driver_attach的调用。
							}

}

进入到__driver_attach
{
453     if (!driver_match_device(drv, dev)) 驱动和设备信息的匹配工作
					||
					\/
				117 static inline int driver_match_device(struct device_driver *drv,
				118                       struct device *dev)
				119 {
				120     return drv->bus->match ? drv->bus->match(dev, drv) : 1;                                                                
				121 }										||
															\/
												 454 struct bus_type i2c_bus_type = {                                                                                          
												 455     .name       = "i2c",
												 456     .match      = i2c_device_match,调用了这个函数
												 457     .probe      = i2c_device_probe,
												 458     .remove     = i2c_device_remove,
												 459     .shutdown   = i2c_device_shutdown,
												 460     .pm     = &i2c_device_pm_ops,
												 461 };      
												 进入到i2c_device_match
															||
															\/
												78 static int i2c_device_match(struct device *dev, struct device_driver *drv)
												79 {
														//描述i2c设备信息
												80     struct i2c_client   *client = i2c_verify_client(dev);
												81     struct i2c_driver   *driver;

												97         return i2c_match_id(driver->id_table, client) != NULL;
																		||
																		\/
													70     while (id->name[0]) {
													  71         if (strcmp(client->name, id->name) == 0)
													  72             return id;
													  73         id++;
													  74     }

如果上面的设备和驱动匹配成功后执行:
460   driver_probe_device(drv, dev);
			||
			\/
		382     ret = really_probe(dev, drv);  
						||
						\/
				287     if (dev->bus->probe) {
				288         ret = dev->bus->probe(dev);
				这段代码是驱动和设备匹配后进来的，一旦能匹配说明驱动和设备使用的是同一个总线，
				在操作驱动结构体中bus被初始化为driver->driver.bus = &i2c_bus_type，
				所以dev->bus-probe(dev)调用的就是i2c_bus_type结构体内部的probe成员。
				/*.probe      = i2c_device_probe,*/
									||
									\/
								263     status = driver->probe(client, i2c_match_id(driver->id_table, client));
														设备驱动层的probe，也就是我们自己实现的。
```

### 核心层总结: ###
1. 使用i2c_add_driver()函数最终调用了设备驱动层的probe函数。这个probe成员属于i2c_driver结构体，
所有又说明，设备驱动层中需要我们自己去实现i2c_driver的内容。  

2. 核心层提供了一个常用的函数i2c_transfer
```c

1745 int i2c_transfer(struct i2c_adapter *adap, struct i2c_msg *msgs, int num)
1746 {
1766     if (adap->algo->master_xfer) {  //就是调用了I2C总线驱动层中的s3c24xx_i2c_xfer
		}
}

```
### 自己实现挂载到I2C总线上从设备驱动。###

```c
添加I2C从设备的相关硬件信息:
1、添加平台代码
2、添加设备树
	187     i2c@138B0000{    138B0000通过查看原理图知道MPU6050挂接到I2C5适配器                                                                                 
	188         #address-cells = <1>; 默认代表了当前节点的子节点reg属性的地址个数
	189         #size-cells = <0>;   默认代表了当前节点的子节点reg属性的偏移量个数
	190         samsung,i2c-sda-delay = <100>; I2C数据线电平跳变的时间延时为100ns
	191         samsung,i2c-max-bus-freq = <100000>; I2C标准模式下最大总线通信频率
	192         pinctrl-0 = <&i2c5_bus>; i2c5_bus是arch/arm/boot/dts/exynos4x12-pinctl.txt
	193         pinctrl-names = "default";
	194         status = "okay"; 代表了当前I2C适配器状态——可以使用的
	195         
	196         mpu6050@68{
	197             .compatible = "fs4412,mpu6050";      
	198             ret = <0x68>;
	199         };  
	200     };

Documentation/devicetree/bindings/i2c/i2c-s3c2410.txt
Documentation/devicetree/bindings/i2c/i2c-mux-pinctrl.txt 	
Documentation/devicetree/bindings/pinctrl/samsung-pinctrl.txt

进入i2c5_bus是arch/arm/boot/dts/exynos4x12-pinctl.txt
219         i2c5_bus: i2c5-bus {                                                
220             samsung,pins = "gpb-2", "gpb-3";使用的哪个gpio控制中的具体引脚
221             samsung,pin-function = <3>; 代表了gpb-2和gpb-3仅存器中的值。
222             samsung,pin-pud = <3>;  操作的是GPBPUD寄存器，使能为上拉
223             samsung,pin-drv = <0>; 驱动强度，操作的是GPBDRV寄存器
224         };

添加完设备树，可以写驱动了。但是还要确认i2c-s3c2410.c是否默认被编译进了内核。
1、进入到drivers/i2c/busses/Makefile中寻找obj-$(CONFIG_I2C_S3C2410) += i2c-s3c2410.o
2、进入同级目标的Kconfig文件中寻找:config I2C_S3C2410
	668 config I2C_S3C2410                                                         
	669     tristate "S3C2410 I2C Driver"
	670     depends on HAVE_S3C2410_I2C
    说明我们需要的驱动对应选项名称为 "S3C2410 I2C Driver"
3、确认"S3C2410 I2C Driver"位置
	1）先确认依赖的东西是否被选中
	2）依赖选中后，同个Kconfig中找到menu "I2C Hardware Bus support"
		返回到上一级的Kconfig文件中
		tristate "I2C support"
		继续返回上一级Kconfig: menu "Device Drivers"

I2C从机驱动实现框架：
模块声明

模块加载函数
{
	i2c_add_driver();
}

模块卸载函数
{
	i2c_del_driver();
}
```
