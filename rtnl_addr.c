/*
 * Copyright (c) 2016 Fabien Siron <fabien.siron@epita.fr>
 * Copyright (c) 2017 JingPiao Chen <chenjingpiao@gmail.com>
 * Copyright (c) 2016-2017 The strace developers.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "defs.h"
#include "netlink_route.h"
#include "print_fields.h"

#include "netlink.h"
#include <linux/rtnetlink.h>
#ifdef HAVE_LINUX_IF_ADDR_H
# include <linux/if_addr.h>
#endif

#include "xlat/ifaddrflags.h"
#include "xlat/routing_scopes.h"

DECL_NETLINK_ROUTE_DECODER(decode_ifaddrmsg)
{
	struct ifaddrmsg ifaddr = { .ifa_family = family };
	const size_t offset = sizeof(ifaddr.ifa_family);

	PRINT_FIELD_XVAL("{", ifaddr, ifa_family, addrfams, "AF_???");

	tprints(", ");
	if (len >= sizeof(ifaddr)) {
		if (!umoven_or_printaddr(tcp, addr + offset,
					 sizeof(ifaddr) - offset,
					 (void *) &ifaddr + offset)) {
			PRINT_FIELD_U("", ifaddr, ifa_prefixlen);
			PRINT_FIELD_FLAGS(", ", ifaddr, ifa_flags,
					  ifaddrflags, "IFA_F_???");
			PRINT_FIELD_XVAL(", ", ifaddr, ifa_scope,
					 routing_scopes, NULL);
			PRINT_FIELD_IFINDEX(", ", ifaddr, ifa_index);
		}
	} else
		tprints("...");
	tprints("}");
}