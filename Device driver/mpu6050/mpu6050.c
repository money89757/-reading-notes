#include<linux/init.h>
#include<linux/module.h>
#include<linux/i2c.h>

int mpu6050_probe(struct i2c_client *client,const struct i2c_device_id *id)
{
	printk("match ok\n");
	return 0;
}

int mpu6050_remove(struct i2c_client *client)
{
	printk("mpu6050_remove\n");
	return 0;
}

struct of_device_id mpu6050_of_table[] = {
	{
		.compatible = "fs4412,mpu6050",
	},
	{},
};

struct i2c_device_id mpu6050_id_table[] = {
	{
		.name = "mpu6050",
	},
	{}
};

struct i2c_driver mpu6050_drv = {
	.driver = {
		.name = "mpu6050",
		.of_match_table = mpu6050_of_table,
	},
	.probe = mpu6050_probe,
	.remove = mpu6050_remove,
	.id_table = mpu6050_id_table,
};

int mpu6050_init(void)
{
	i2c_add_driver(&mpu6050_drv);
	printk("mpu6050_init\n");
	return 0;
}
module_init(mpu6050_init);

void mpu6050_exit(void)
{
	i2c_del_driver(&mpu6050_drv);
	printk("mpu6050_exit\n");
	return;
}
module_exit(mpu6050_exit);
MODULE_LICENSE("GPL");
