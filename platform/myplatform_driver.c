#include<linux/init.h>
#include<linux/module.h>
#include<linux/platform_device.h>

MODULE_LICENSE("GPL");

#if 0
struct platform_device_id myid_table[] = {
	{"17022"},
	{"hello"},
	{},//最后一个大括号不能省略
};
#endif

static int myplatform_testdrv_probe(struct platform_device *dev)
{
	printk("match ok\n");
	return 0;
}


static int myplatform_testdrv_remove(struct platform_device *dev)
{
	printk("myplatform_testdrv_remove success\n");
	return 0;
}

static struct platform_driver pdrv = {
//	.id_table = myid_table,
	.driver = {
		.name = "17022",
	},
	.probe = myplatform_testdrv_probe,
	.remove = myplatform_testdrv_remove,
};

static int platform_testdrv_init(void)
{
	platform_driver_register(&pdrv);
	printk("platform_driver_register success\n");
	return 0;
}
module_init(platform_testdrv_init);

static void platform_testdrv_exit(void)
{
	platform_driver_unregister(&pdrv);
	printk("platform_driver_unregister success\n");
	return;
}
module_exit(platform_testdrv_exit);
