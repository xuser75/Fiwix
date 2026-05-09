/*
 * fiwix/net/loopback.c
 *
 * Copyright 2025, Jordi Sanfeliu. All rights reserved.
 * Distributed under the terms of the Fiwix License.
 */

#include <fiwix/config.h>
#include <fiwix/errno.h>
#include <fiwix/net.h>
#include <fiwix/net/loopback.h>
#include <fiwix/mm.h>
#include <fiwix/string.h>
#include <fiwix/stdio.h>

#ifdef CONFIG_NET

struct loopback_dev *loopback_device = NULL;

/*
 * loopback_init() - Initialize the loopback network interface
 *
 * Sets up the local loopback device with IP address 127.0.0.1
 * and netmask 255.255.255.255
 */
int loopback_init(void)
{
	loopback_device = (struct loopback_dev *)kmalloc(sizeof(struct loopback_dev));
	if (!loopback_device) {
		printk("ERROR: loopback_init() could not allocate memory\n");
		return -ENOMEM;
	}

	memset_b(loopback_device, 0, sizeof(struct loopback_dev));

	loopback_device->ipaddr = LOOPBACK_ADDR;
	loopback_device->netmask = LOOPBACK_NETMASK;
	loopback_device->mtu = 65535;
	loopback_device->stats_rx_packets = 0;
	loopback_device->stats_tx_packets = 0;
	loopback_device->stats_rx_bytes = 0;
	loopback_device->stats_tx_bytes = 0;

	printk("loopback: device initialized (127.0.0.1)\n");
	return 0;
}

/*
 * loopback_xmit() - Transmit data on the loopback interface
 *
 * For the loopback interface, transmission directly becomes reception.
 * Data sent on loopback is immediately available for reception.
 */
int loopback_xmit(const char *buf, int len)
{
	if (!loopback_device) {
		return -ENODEV;
	}

	if (len < 0 || len > 65535) {
		return -EINVAL;
	}

	loopback_device->stats_tx_packets++;
	loopback_device->stats_tx_bytes += len;
	
	/* For loopback, transmission immediately becomes reception */
	loopback_device->stats_rx_packets++;
	loopback_device->stats_rx_bytes += len;

	return len;
}

/*
 * loopback_get_ipaddr() - Get the IP address of the loopback device
 */
unsigned int loopback_get_ipaddr(void)
{
	if (!loopback_device) {
		return 0;
	}
	return loopback_device->ipaddr;
}

/*
 * loopback_get_netmask() - Get the netmask of the loopback device
 */
unsigned int loopback_get_netmask(void)
{
	if (!loopback_device) {
		return 0;
	}
	return loopback_device->netmask;
}

/*
 * loopback_get_mtu() - Get the MTU of the loopback device
 */
int loopback_get_mtu(void)
{
	if (!loopback_device) {
		return 0;
	}
	return loopback_device->mtu;
}

#endif /* CONFIG_NET */
