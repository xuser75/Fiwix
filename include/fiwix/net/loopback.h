/*
 * fiwix/include/fiwix/net/loopback.h
 *
 * Copyright 2025, Jordi Sanfeliu. All rights reserved.
 * Distributed under the terms of the Fiwix License.
 */

#ifdef CONFIG_NET

#ifndef _FIWIX_NET_LOOPBACK_H
#define _FIWIX_NET_LOOPBACK_H

#include <fiwix/types.h>

/* Loopback interface configuration */
#define LOOPBACK_ADDR		0x7f000001	/* 127.0.0.1 */
#define LOOPBACK_NETMASK	0xffffffff	/* 255.255.255.255 */

/* Loopback device structure */
struct loopback_dev {
	unsigned int ipaddr;
	unsigned int netmask;
	int mtu;
	int stats_rx_packets;
	int stats_tx_packets;
	int stats_rx_bytes;
	int stats_tx_bytes;
};

extern struct loopback_dev *loopback_device;

int loopback_init(void);
int loopback_xmit(const char *buf, int len);

#endif /* _FIWIX_NET_LOOPBACK_H */

#endif /* CONFIG_NET */
