
/**
 * \addtogroup rimeuc
 * @{
 */

/*
 * Copyright (c) 2006, Swedish Institute of Computer Science.
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
 * 3. Neither the name of the Institute nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE INSTITUTE AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE INSTITUTE OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * This file is part of the Contiki operating system.
 *
 * $Id: unicast.c,v 1.4 2010/02/23 18:38:05 adamdunkels Exp $
 */

/**
 * \file
 *         Single-hop unicast
 * \author
 *         Adam Dunkels <adam@sics.se>
 */

#include "net/rime.h"
#include "net/rime/crime/c_unicast.h"
#include <string.h>


#define DEBUG 0
#if DEBUG
#include <stdio.h>
#define PRINTF(...) printf(__VA_ARGS__)
#else
#define PRINTF(...)
#endif

void c_unicast_close(struct pipe *p, struct stackmodule_i *module){
	PRINTF("c_unicast_close\n");
}
/*---------------------------------------------------------------------------*/
void
c_unicast_open(struct pipe *p, struct stackmodule_i *module)
{
	 channel_set_attributes(p->channel_no, p->attrlist);
	  PRINTF("c_unicast_open\n");
}
/*---------------------------------------------------------------------------*/
int
c_unicast_send(struct pipe *p, struct stackmodule_i *module)
{
	PRINTF("c_unicast_send \n");
	printaddr(module->stack_id);

	rimeaddr_t *tmpaddr = get_node_addr(module->stack_id, 0, 2);
  packetbuf_set_addr(PACKETBUF_ADDR_RECEIVER, tmpaddr);
  return 1;
}

/*---------------------------------------------------------------------------*/
void
c_unicast_recv(struct pipe *p, struct stackmodule_i *module)
{
	 PRINTF("c_unicast_recv \n");
	rimeaddr_t tmpaddr;
	rimeaddr_copy(&tmpaddr, packetbuf_addr(PACKETBUF_ADDR_RECEIVER));
	set_node_addr(module->stack_id, 1, 2, &tmpaddr);
  printaddr(module->stack_id);

  if(rimeaddr_cmp(packetbuf_addr(PACKETBUF_ADDR_RECEIVER), &rimeaddr_node_addr)) {
	  PRINTF("c_unicast_recv: %d.%d received packet from %d.%d\n",
	  	 rimeaddr_node_addr.u8[0], rimeaddr_node_addr.u8[1],
	  	 packetbuf_addr(PACKETBUF_ADDR_SENDER)->u8[0],
	  	 packetbuf_addr(PACKETBUF_ADDR_SENDER)->u8[1]);
  }
  PRINTF("~c_unicast_recv \n");
}
/*---------------------------------------------------------------------------*/
void
c_unicast_sent(struct pipe *p, struct stackmodule_i *module)
{
	  PRINTF("c_unicast_sent: %d.%d to %d.%d\n",
	  	 rimeaddr_node_addr.u8[0], rimeaddr_node_addr.u8[1],
	  	 packetbuf_addr(PACKETBUF_ADDR_RECEIVER)->u8[0],
	  	 packetbuf_addr(PACKETBUF_ADDR_RECEIVER)->u8[1]);
}
/*---------------------------------------------------------------------------*/
/** @} */