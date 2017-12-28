#include<linux/init.h>
#include<linux/module.h>
#include<linux/platform_device.h>

MODULE_LICENSE("GPL");

static void led_release(struct device *dev)
{
	printk("led_release\n");
	return;
}

struct resource led_resource_table[] = {
	{
		.start = 0x11000c40,
		.end = 0x11000c40+0x4,
		.flags = IORESOURCE_MEM,
	},
	{
		.start = 0x11000c20,
		.end = 0x11000c20+0x4,
		.flags = IORESOURCE_MEM,
	},
	{
		.start = 0x114001e0,
		.end = 0x114001e0+0x4,
		.flags = IORESOURCE_MEM,
	},
};

static struct platform_device led_dev = {
	.name = "fs4412-led",
	.dev = {
		.release = led_release,
	},
	.num_resources = ARRAY_SIZE(led_resource_table),
	.resource = led_resource_table,
};

static int led_init(void)
{
	platform_device_register(&led_dev);
	printk("led_init\n");
	return 0;
}
module_init(led_init);

static void led_exit(void)
{
	platform_device_unregister(&led_dev);
	printk("led_exit");

}
module_exit(led_exit);
