#include<linux/init.h>
#include<linux/module.h>
#include<linux/platform_device.h>

MODULE_LICENSE("GPL");

static void platform_testdev_release(struct device *dev)
{
	printk("platform_testdev_release success\n");
	return;
}

static struct platform_device pdev = {
	.name = "17022",
	.dev = {
		.release = platform_testdev_release,
	},
};

static int platform_testdev_init(void)
{
	platform_device_register(&pdev);
	printk("platform_test_init success\n");
	return 0;
}
module_init(platform_testdev_init);

static void platform_testdev_exit(void)
{
	platform_device_unregister(&pdev);
	printk("platform_testdev_exit success \n");
	return;
}
module_exit(platform_testdev_exit);
