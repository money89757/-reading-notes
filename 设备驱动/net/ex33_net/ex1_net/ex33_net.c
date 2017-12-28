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
#include <linux/ip.h>

#include <asm/irq.h>
#include <linux/atomic.h>

static struct net_device* virt_net;


static int emulator_virt_net_dev(struct sk_buff *skb, struct net_device *dev){

	//�ο�LDD3����س����
	unsigned char* type;
	struct iphdr* ih;
	__be32 *saddr, *daddr, tmp;
	unsigned char tmp_dev_addr[ETH_ALEN];
	struct ethhdr* ethhdr;

	struct sk_buff* rx_skb;

	/*��Ӳ������/��������*/
	/*�Ե� macͷ�е� Դ��Ŀ��*/
	ethhdr = (struct ethhdr*)skb->data;
	memcpy(tmp_dev_addr, ethhdr->h_dest, ETH_ALEN);
	memcpy(ethhdr->h_dest,ethhdr->h_source , ETH_ALEN);
	memcpy(ethhdr->h_source, tmp_dev_addr, ETH_ALEN);

	/*�Ե� ipͷ�е�Դ��Ŀ��*/
	ih = (struct iphdr*)(skb->data + sizeof(struct ethhdr));
	saddr = &ih->saddr;
	daddr = &ih->daddr;

	tmp = *saddr;
	*saddr = *daddr;
	*daddr = tmp;

	type = skb->data + sizeof(struct ethhdr) + sizeof(struct iphdr);
	*type = 0; 	//0x8��ʾping�� 0��ʾreply

	ih->check = 0;//����У���룬ip��Ҫ������������....
	ih->check = ip_fast_csum((unsigned char*)ih, ih->ihl);
	 //����һ��sk_buff
	 rx_skb  = dev_alloc_skb(skb->len + 2);
	 skb_reserve(rx_skb,2);//����ip��16B�߽�
	 memcpy(skb_put(rx_skb, skb->len),skb->data, skb->len ); 

	rx_skb->dev = dev;
	rx_skb->protocol = eth_type_trans(rx_skb, dev);
	rx_skb->ip_summed = CHECKSUM_UNNECESSARY;
	
	//�ύsk_buff
	netif_rx(rx_skb);
	//����ͳ����Ϣ
	virt_net->stats.rx_packets++;
	virt_net->stats.rx_bytes += rx_skb->len;
	
	
	return 0;

}


static netdev_tx_t virt_net_send_packet(struct sk_buff *skb, struct net_device *dev){
	static unsigned int cnt = 0;



	//��ͣ���������ݶ��еĲ���
	netif_stop_queue(dev);
	//����skb
	dev_kfree_skb(skb);
	//���Ѷ���
	netif_wake_queue(dev);
	
	//���·�������
	dev->stats.tx_packets ++;
	dev->stats.tx_bytes += skb->len;
	
	//ģ��һ���ذ�����:
	emulator_virt_net_dev(skb,dev);
	
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
	
	//2.1 ���������������ͨ��reply
		virt_net->flags |= IFF_NOARP;
		virt_net->features |= NETIF_F_V6_CSUM;

	//3. ע��net_device������Ϣ�ṹ�嵽�ں���
	register_netdev(virt_net);
	//����mac��ַ
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
