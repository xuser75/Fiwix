/*
 * fiwix/include/fiwix/net/tcp.h
 *
 * Copyright 2026, Benjamin Davis. All rights reserved.
 * Distributed under the terms of the Fiwix License.
 */

#ifdef CONFIG_NET

#ifndef _FIWIX_NET_TCP_H
#define _FIWIX_NET_TCP_H

#include <fiwix/types.h>

/* TCP socket states */
#define TCP_CLOSED		0
#define TCP_LISTEN		1
#define TCP_SYN_SENT	2
#define TCP_SYN_RECV	3
#define TCP_ESTABLISHED	4
#define TCP_FIN_WAIT1	5
#define TCP_FIN_WAIT2	6
#define TCP_CLOSING		7
#define TCP_TIME_WAIT	8
#define TCP_CLOSE_WAIT	9
#define TCP_LAST_ACK	10

/* TCP control structure for each socket */
struct tcp_sock {
	unsigned int snd_una;		/* send unacknowledged sequence */
	unsigned int snd_nxt;		/* send next sequence */
	unsigned int snd_wnd;		/* send window */
	unsigned int rcv_nxt;		/* receive next sequence */
	unsigned int rcv_wnd;		/* receive window */
	int state;					/* TCP connection state */
	int backlog;				/* connection backlog */
	char *send_buf;				/* send buffer */
	char *recv_buf;				/* receive buffer */
	int send_buf_len;
	int recv_buf_len;
	int send_buf_head;
	int send_buf_tail;
	int recv_buf_head;
	int recv_buf_tail;
};

#endif /* _FIWIX_NET_TCP_H */

#endif /* CONFIG_NET */
