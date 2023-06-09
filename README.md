## Network Driver Test

## Installation Requirements

Since I had a M1 laptop, I was having trouble getting Vagrant and different VMs to work well.  To get around this I spun up a VM in GCP and tested the scripts over there

### Loading a Linux Module

1. I have a `test.c` which I use to learn how to load a kernal module into a VM
2. Run the Makefile command: `make all`
3. Load the module: `sudo insmod test.ko`
4. View the cmd in the kernal using: `dmesg`
5. Remove the module: `sudo rmmod test.ko`
5. Run `dmesg` and Verify we see a `Goodbye` message


###  Network module
The network module looks at packets that go between  `NIC1` and `NIC2`. To find these via linux, I did:

1. ip addr
2. From there I retrieved the MAC address which on the VM I spun up was set to 00:00:00:00:00
3. `network.c` is the file I used to measure packet arrival and destination
4. `make load-module` loads the kernel module

### Debugging tips
Check `dmesg` -> This has all the networking messages from the module once it loads

### References
https://netfilter.org/documentation/HOWTO/netfilter-hacking-HOWTO.txt
http://wiki.dreamrunner.org/public_html/Linux/Networks/netfilter.html
