//头文件
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
#include <linux/ip.h>

#include <asm/irq.h>
#include <linux/atomic.h>

static struct net_device* virt_net;


static int emulator_virt_net_dev(struct sk_buff *skb, struct net_device *dev){

	//参考LDD3的相关程序段
	unsigned char* type;
	struct iphdr* ih;
	__be32 *saddr, *daddr, tmp;
	unsigned char tmp_dev_addr[ETH_ALEN];
	struct ethhdr* ethhdr;

	struct sk_buff* rx_skb;

	/*从硬件读出/保存数据*/
	/*对调 mac头中的 源和目的*/
	ethhdr = (struct ethhdr*)skb->data;
	memcpy(tmp_dev_addr, ethhdr->h_dest, ETH_ALEN);
	memcpy(ethhdr->h_dest,ethhdr->h_source , ETH_ALEN);
	memcpy(ethhdr->h_source, tmp_dev_addr, ETH_ALEN);

	/*对调 ip头中的源、目的*/
	ih = (struct iphdr*)(skb->data + sizeof(struct ethhdr));
	saddr = &ih->saddr;
	daddr = &ih->daddr;

	tmp = *saddr;
	*saddr = *daddr;
	*daddr = tmp;

	type = skb->data + sizeof(struct ethhdr) + sizeof(struct iphdr);
	*type = 0; 	//0x8表示ping， 0表示reply

	ih->check = 0;//计算校验码，ip需要它。抄过来的....
	ih->check = ip_fast_csum((unsigned char*)ih, ih->ihl);
	 //构造一个sk_buff
	 rx_skb  = dev_alloc_skb(skb->len + 2);
	 skb_reserve(rx_skb,2);//对齐ip的16B边界
	 memcpy(skb_put(rx_skb, skb->len),skb->data, skb->len ); 

	rx_skb->dev = dev;
	rx_skb->protocol = eth_type_trans(rx_skb, dev);
	rx_skb->ip_summed = CHECKSUM_UNNECESSARY;
	
	//提交sk_buff
	netif_rx(rx_skb);
	//更新统计信息
	virt_net->stats.rx_packets++;
	virt_net->stats.rx_bytes += rx_skb->len;
	
	
	return 0;

}


static netdev_tx_t virt_net_send_packet(struct sk_buff *skb, struct net_device *dev){
	static unsigned int cnt = 0;



	//暂停对网络数据队列的操作
	netif_stop_queue(dev);
	//唤醒skb
	dev_kfree_skb(skb);
	//唤醒队列
	netif_wake_queue(dev);
	
	//更新发包数据
	dev->stats.tx_packets ++;
	dev->stats.tx_bytes += skb->len;
	
	//模拟一个回包函数:
	emulator_virt_net_dev(skb,dev);
	
	printk("virt_net_send_packet cnt:%d\n",cnt++);
	return NETDEV_TX_OK;
}

static const struct net_device_ops virt_net_ops = {
	//发送数据的函数
	.ndo_start_xmit		= virt_net_send_packet,
};

static int fs4412_driver_init(void){
	//1. 申请net_device网络信息结构体
	virt_net = alloc_netdev_mqs(0, "vnet%d", ether_setup, 1, 1);
	
	//2. 指定操作方法集:收、发函数
	virt_net->netdev_ops	= &virt_net_ops;
	
	//2.1 设置下面两项才能通过reply
		virt_net->flags |= IFF_NOARP;
		virt_net->features |= NETIF_F_V6_CSUM;

	//3. 注册net_device网络信息结构体到内核中
	register_netdev(virt_net);
	//设置mac地址
	virt_net->dev_addr[0] = 0x08;
	virt_net->dev_addr[1] = 0x08;
	virt_net->dev_addr[2] = 0x08;
	virt_net->dev_addr[3] = 0x08;
	virt_net->dev_addr[4] = 0x08;
	virt_net->dev_addr[5] = 0x08;
	
	
	printk("Hello World ):\n");
	return 0;
}

static void fs4412_driver_exit(void){

	//4.释放资源	
	unregister_netdev(virt_net);
	free_netdev(virt_net);
	
	printk("Good Bye,linux kernel >_<\n");
}

//1. 入口修饰
module_init(fs4412_driver_init);
//2. 出口修饰

module_exit(fs4412_driver_exit);

//3. 许可证
MODULE_LICENSE("GPL");
