/*
 * fiwix/net/tcp.c
 *
 * Copyright 2026, Benjamin Davis. All rights reserved.
 * Distributed under the terms of the Fiwix License.
 */

#include <fiwix/config.h>
#include <fiwix/errno.h>
#include <fiwix/socket.h>
#include <fiwix/net.h>
#include <fiwix/net/tcp.h>
#include <fiwix/mm.h>
#include <fiwix/string.h>
#include <fiwix/stdio.h>

#ifdef CONFIG_NET

#define TCP_SEND_BUF_SIZE	4096
#define TCP_RECV_BUF_SIZE	4096

int tcp_create(struct socket *s, int domain, int type, int protocol)
{
	struct tcp_sock *tcp;

	if (type != SOCK_STREAM) {
		return -EINVAL;
	}

	tcp = (struct tcp_sock *)kmalloc(sizeof(struct tcp_sock));
	if (!tcp) {
		return -ENOMEM;
	}

	memset_b(tcp, 0, sizeof(struct tcp_sock));
	
	tcp->send_buf = (char *)kmalloc(TCP_SEND_BUF_SIZE);
	if (!tcp->send_buf) {
		kfree((unsigned int)tcp);
		return -ENOMEM;
	}

	tcp->recv_buf = (char *)kmalloc(TCP_RECV_BUF_SIZE);
	if (!tcp->recv_buf) {
		kfree((unsigned int)tcp->send_buf);
		kfree((unsigned int)tcp);
		return -ENOMEM;
	}

	tcp->state = TCP_CLOSED;
	tcp->snd_wnd = TCP_SEND_BUF_SIZE;
	tcp->rcv_wnd = TCP_RECV_BUF_SIZE;
	tcp->send_buf_len = TCP_SEND_BUF_SIZE;
	tcp->recv_buf_len = TCP_RECV_BUF_SIZE;
	tcp->send_buf_head = 0;
	tcp->send_buf_tail = 0;
	tcp->recv_buf_head = 0;
	tcp->recv_buf_tail = 0;

	/* Store pointer to TCP socket structure in the socket union */
	memcpy_b(&s->u.ipv4_info, &tcp, sizeof(struct tcp_sock *));

	return 0;
}

int tcp_bind(struct socket *s, const struct sockaddr *addr, int addrlen)
{
	/* Placeholder for TCP bind implementation */
	return 0;
}

int tcp_listen(struct socket *s, int backlog)
{
	struct tcp_sock *tcp;

	tcp = (struct tcp_sock *)&s->u.ipv4_info;
	tcp->state = TCP_LISTEN;
	tcp->backlog = backlog;
	return 0;
}

int tcp_connect(struct socket *s, const struct sockaddr *addr, int addrlen)
{
	struct tcp_sock *tcp;

	tcp = (struct tcp_sock *)&s->u.ipv4_info;
	tcp->state = TCP_SYN_SENT;

	/* Placeholder for TCP connection establishment */
	return 0;
}

int tcp_accept(struct socket *s, struct sockaddr *addr, unsigned int *addrlen)
{
	/* Placeholder for TCP accept implementation */
	return -EOPNOTSUPP;
}

int tcp_send(struct socket *s, struct fd *f, const char *buffer, __size_t count, int flags)
{
	struct tcp_sock *tcp;
	int available, to_write;

	tcp = (struct tcp_sock *)&s->u.ipv4_info;

	if (tcp->state != TCP_ESTABLISHED) {
		return -ENOTCONN;
	}

	/* Calculate available space in send buffer */
	available = tcp->send_buf_len - ((tcp->send_buf_tail - tcp->send_buf_head + tcp->send_buf_len) % tcp->send_buf_len);
	
	if (available == 0) {
		return -EAGAIN;
	}

	to_write = (count < available) ? count : available;

	/* Copy data to send buffer */
	memcpy_b(tcp->send_buf + tcp->send_buf_tail, buffer, to_write);
	tcp->send_buf_tail = (tcp->send_buf_tail + to_write) % tcp->send_buf_len;

	return to_write;
}

int tcp_recv(struct socket *s, struct fd *f, char *buffer, __size_t count, int flags)
{
	struct tcp_sock *tcp;
	int available, to_read;

	tcp = (struct tcp_sock *)&s->u.ipv4_info;

	if (tcp->state != TCP_ESTABLISHED) {
		return -ENOTCONN;
	}

	/* Calculate available data in receive buffer */
	available = (tcp->recv_buf_tail - tcp->recv_buf_head + tcp->recv_buf_len) % tcp->recv_buf_len;
	
	if (available == 0) {
		return 0;
	}

	to_read = (count < available) ? count : available;

	/* Copy data from receive buffer */
	memcpy_b(buffer, tcp->recv_buf + tcp->recv_buf_head, to_read);
	tcp->recv_buf_head = (tcp->recv_buf_head + to_read) % tcp->recv_buf_len;

	return to_read;
}

int tcp_close(struct socket *s)
{
	struct tcp_sock *tcp;

	tcp = (struct tcp_sock *)&s->u.ipv4_info;

	if (tcp->state == TCP_ESTABLISHED) {
		tcp->state = TCP_FIN_WAIT1;
	}

	if (tcp->send_buf) {
		kfree((unsigned int)tcp->send_buf);
	}
	if (tcp->recv_buf) {
		kfree((unsigned int)tcp->recv_buf);
	}

	return 0;
}

#endif /* CONFIG_NET */
