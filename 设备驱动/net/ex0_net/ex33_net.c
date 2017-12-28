//ͷ�ļ�
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/errno.h>
#include <linux/netdevice.h>
#include <linux/etherdevice.h>
#include <linux/platform_device.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/fcntl.h>
#include <linux/interrupt.h>
#include <linux/ioport.h>
#include <linux/in.h>
#include <linux/skbuff.h>
#include <linux/spinlock.h>
#include <linux/string.h>
#include <linux/init.h>
#include <linux/bitops.h>
#include <linux/delay.h>
#include <linux/gfp.h>
#include <linux/io.h>

#include <asm/irq.h>
#include <linux/atomic.h>

static struct net_device* virt_net;



static netdev_tx_t virt_net_send_packet(struct sk_buff *skb, struct net_device *dev){
	static unsigned int cnt = 0;

	printk("virt_net_send_packet cnt:%d\n",cnt++);
	return NETDEV_TX_OK;
}

static const struct net_device_ops virt_net_ops = {
	//�������ݵĺ���
	.ndo_start_xmit		= virt_net_send_packet,
};

static int fs4412_driver_init(void){
	//1. ����net_device������Ϣ�ṹ��
	virt_net = alloc_netdev_mqs(0, "vnet%d", ether_setup, 1, 1);
	
	//2. ָ������������:�ա�������
	virt_net->netdev_ops	= &virt_net_ops;

	//3. ע��net_device������Ϣ�ṹ�嵽�ں���
	register_netdev(virt_net);

	printk("Hello World ):\n");
	return 0;
}

static void fs4412_driver_exit(void){

	//4.�ͷ���Դ	
	unregister_netdev(virt_net);
	free_netdev(virt_net);
	
	printk("Good Bye,linux kernel >_<\n");
}

//1. �������
module_init(fs4412_driver_init);
//2. ��������

module_exit(fs4412_driver_exit);

//3. ���֤
MODULE_LICENSE("GPL");
