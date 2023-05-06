/* 
 * producer_consumer.c - has a producer which gathers packet count and 
 * 	a consumer that calculates the average packet count in one minute
 */ 
 
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/netfilter.h>
#include <linux/netfilter_ipv4.h>
#include <linux/ip.h>
#include <linux/wait.h>
#include <linux/sched.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Zuhayr Elahi");


static struct nf_hook_ops nfho;
static unsigned int packet_count = 0;
static wait_queue_head_t wait_queue;

unsigned int hook_func(void *priv, struct sk_buff *skb, const struct nf_hook_state *state) {

    struct iphdr *iph;
    
    // Check if the packet is an IPv4 packet
    if (skb->protocol == htons(ETH_P_IP)) {
        iph = ip_hdr(skb);
        printk(KERN_INFO "Packet from %pI4\n", &iph->saddr);
    	packet_count ++;
	wake_up_interruptible(&wait_queue); // wake up the consumer
    }

    return NF_ACCEPT;
}



int init(void) 
{ 
    
    // Construct the Hook Struct
    nfho.hook = hook_func;
    nfho.pf = PF_INET;
    nfho.hooknum = NF_INET_PRE_ROUTING;
    nfho.priority = NF_IP_PRI_FIRST;
    
    nf_register_net_hook(&init_net, &nfho);
    init_waitqueue_head(&wait_queue);

    return 0;
}


unsigned int get_packet_count(void) {
	return packet_count;
}

/* 
 * consumer returns the count and average packets per set time
*/
static int consumer(void *arg) {
	unsigned int count = 0;
	unsigned int avg = 0;
	int i = 0;

	while(!kthread_should_stop()) {
		wait_event_interruptible(wait_queue, packet_count > count);
		count = get_packet_count();
		avg = 0;
		for (i = 0; i < count; i++) {
			avg += i;
		}
		avg /= count;
		printk(KERN_INFO "Average packet count: %u\n", avg);
	}
	do_exit(0);
}

static int __init producer_consumer_init(void) {
	printk(KERN_INFO "Netfilter module loaded\n");
	init();
	kthread_run(consumer, NULL, "Consumer");
	return 0;
}

static void __exit producer_consumer_exit(void) {
	printk(KERN_INFO "Producer-consumer module unloaded\n");
}

module_init(producer_consumer_init);
module_exit(producer_consumer_exit);
MODULE_LICENSE("GPL");
