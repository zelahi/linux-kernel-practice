#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/netfilter.h>
#include <linux/netfilter_ipv4.h>
#include <linux/ktime.h>

static struct nf_hook_ops nfho_incoming, nfho_outgoing;

static unsigned int hook_incoming(void *priv, struct sk_buff *skb, const struct nf_hook_state *state) {
    ktime_t arrival_time = ktime_get();
    printk(KERN_INFO "Packet arrived at %lld", arrival_time);
    return NF_ACCEPT; // or NF_DROP, etc.
}

static unsigned int hook_outgoing(void *priv, struct sk_buff *skb, const struct nf_hook_state *state) {
    ktime_t departure_time = ktime_get();
    printk(KERN_INFO "Packet departed at %lld", departure_time);
    return NF_ACCEPT; // or NF_DROP, etc.
}

int init_module() {
    nfho_incoming.hook = hook_incoming;
    nfho_incoming.pf = PF_INET;  // IPv4 - Protocol for packet filtering
    nfho_incoming.hooknum = NF_INET_LOCAL_IN; // all incoming packets addressed to the local computer pass this hook in the function 
    nfho_incoming.priority = NF_IP_PRI_FIRST;
    nf_register_net_hook(&init_net, &nfho_incoming);

    nfho_outgoing.hook = hook_outgoing;
    nfho_outgoing.pf = PF_INET;
    nfho_outgoing.hooknum = NF_INET_LOCAL_OUT; // all outgoing packets address -> used to measure departure time
    nfho_outgoing.priority = NF_IP_PRI_FIRST;
    nf_register_hook(&init_net, &nfho_outgoing);

    return 0;
}

void cleanup_module() {
    nf_unregister_net_hook(&init_net, &nfho_outgoing);
    nf_unregister_net_hook(&init_net, &nfho_incoming);
}

MODULE_LICENSE("GPL");