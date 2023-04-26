/* Name: producer.c
 *
 * Description: collects statistical data from netfiller 
 *
 */
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/timer.h>
#include <linux/netfilter.h>
#include <linux/netfilter_ipv4.h>

static unsigned int pkt_count = 0;
static struct timer_list ptimer;
struct nf_hook_ops nfho;

void timer_callback(struct timer_list *ptimer) {
	printk(KERN_INFO "Received %u packets in the last minute\n", pkt_count);
	pkt_count = 0;

	mod_timer(ptimer, jiffies + msecs_to_jiffies(60 * 1000));
}

void setup_timer(unsigned long interval) {
	timer_setup(&ptimer, timer_callback, 0);
	ptimer.expires = jiffies + interval * HZ;
	add_timer(&ptimer);
}


unsigned int hook_func(void *priv, struct sk_buff *skb, const struct nf_hook_state *state) {

	// Increment the packet count
	pkt_count++;

	// Allow the packet to continue on its way
	return NF_ACCEPT;
}

// Initialization function for the module
int init_module(void) {
    	// Register the hook function with netfilter
	nfho.hook = hook_func;
    	nfho.hooknum = NF_INET_PRE_ROUTING;
    	nfho.pf = PF_INET;
    	nfho.priority = NF_IP_PRI_FIRST;
	
	nf_register_net_hook(&init_net, &nfho);

	//Setup the timer
	mod_timer(&ptimer, jiffies + msecs_to_jiffies(60000));

	return 0;
}

// Cleanup function for the module
void cleanup_module(void) 
{
	nf_unregister_net_hook(&init_net, &nfho);
	del_timer(&ptimer);
}

MODULE_LICENSE("GPL");