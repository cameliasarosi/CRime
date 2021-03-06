/**
 * \addtogroup crimec_abc
 * @{
 */

/**
 * \defgroup crime c_channel Cannel abstraction for CRime
 * @{
 *
 * The c_channel module abstracts the channel and has no influence on the packets.
 *
 * \section channels Channels
 *
 * The c_channel module uses 1 channel.
 *
 */

/*
 * Copyright (c) 2012, Jozef Stefan Institute.
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
 *
 */

/**
 * \file
 *        File for CRime's channel abstraction
 * \author
 *         Carolina Fortuna <carolina.fortuna@ijs.si>
 */

#include "net/rime/chameleon.h"
#include "net/rime/crime/amodule.h"
#include "net/rime.h"
#include "lib/list.h"


#define DEBUG 0
#if DEBUG
#include <stdio.h>
#define PRINTF(...) printf(__VA_ARGS__)
#else
#define PRINTF(...)
#endif


#define EVAL 0
#if EVAL
#include <stdio.h>
#define START_TM vsnTime_freeRunTime()
#define DURATION_TM(x) vsnTime_freeRunTimeDiff(x)
#define PRINTFE(...) printf(__VA_ARGS__)
#else
#define START_TM 0
#define DURATION_TM(x) 0
#define PRINTFE(...)
#endif

/*---------------------------------------------------------------------------*/
void
c_channel_open(struct pipe *p, struct stackmodule_i *module)
{
	int start_tm = START_TM;
	channel_open(p->channel, p->channel_no);
	PRINTF("~c_channel_open: chno %d %d \n", p->channel->channelno,
         p->channel_no);
	PRINTFE("\n %d \n", DURATION_TM(start_tm));
}

/*---------------------------------------------------------------------------*/
void
c_channel_close(struct pipe *p, struct stackmodule_i *module)
{
	int start_tm = START_TM;
	channel_close(p->channel);
    PRINTF("~c_channel_close \n");
    PRINTFE("\n %d \n", DURATION_TM(start_tm));
}

/*---------------------------------------------------------------------------*/
void c_abc_input(struct pipe *p, struct stackmodule_i *module)
{
	int start_tm = START_TM;
	PRINTF("~c_abc_input: packet '%s' on channel %d\n",
			(char *)packetbuf_dataptr(), p->channel->channelno);
	PRINTFE("\n %d \n", DURATION_TM(start_tm));
}

/*---------------------------------------------------------------------------*/

/** @} */
/** @} */
