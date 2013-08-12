/*
 * stack.c
 *
 *  Created on: Jun 10, 2012
 *      Author: carolina
 */
#include <stdlib.h>

#include "stack.h"
#include "net/rime/crime/amodule.h"
#include "net/rime.h"
#include "net/rime/crime/c_channel.h"
#include "net/rime/crime/c_abc.h"
#include "net/rime/crime/c_broadcast.h"
#include "net/rime/crime/c_unicast.h"
#include "net/rime/crime/c_polite.h"
#include "net/rime/crime/c_multihop.h"
#include "net/rime/crime/c_trickle.h"
#include "net/rime/crime/c_netflood.h"
#include "net/rime/crime/c_mesh.h"
#include "net/rime/crime/c_route_discovery.h"
#include "net/rime/crime/c_echo_app.h"

#define DEBUG 1
#if DEBUG
#include <stdio.h>
#define PRINTF(...) printf(__VA_ARGS__)
#else
#define PRINTF(...)
#endif

void * calloc (size_t nm, size_t es){ return malloc(nm*es);}

void
printaddr(int stack_id)
{
  rimeaddr_t *sender = get_node_addr(stack_id, OUT, 0);

  rimeaddr_t *esender = get_node_addr(stack_id, OUT, 1);

  rimeaddr_t *receiver = get_node_addr(stack_id, OUT, 2);

  rimeaddr_t *ereceiver = get_node_addr(stack_id, OUT, 3);

  PRINTF("out: s%d.%d es%d.%d r%d.%d er%d.%d\n",
         sender->u8[0], sender->u8[1],
         esender->u8[0], esender->u8[1],
         receiver->u8[0], receiver->u8[1],
         ereceiver->u8[0], ereceiver->u8[1]);

  sender = get_node_addr(stack_id, IN, 0);
  esender = get_node_addr(stack_id, IN, 1);
  receiver = get_node_addr(stack_id, IN, 2);
  ereceiver = get_node_addr(stack_id, IN, 3);
  PRINTF("in: s%d.%d es%d.%d r%d.%d er%d.%d\n",
         sender->u8[0], sender->u8[1],
         esender->u8[0], esender->u8[1],
         receiver->u8[0], receiver->u8[1],
         ereceiver->u8[0], ereceiver->u8[1]);
}

void
stack_init()
{
  PRINTF("stack init\n");
  //init the stacks structure (columns of the matrix, branches of the tree)
  stack = (struct stack_i *)calloc(STACKNO, sizeof(struct stack_i));
  rimeaddr_t addr;
  
  
  //@defStack
  struct pipe *pi0; 
  pi0 = (struct pipe*) calloc(1, sizeof(struct pipe)); 
  struct channel *ch0; 
  ch0 = (struct channel*) calloc(1, sizeof(struct channel));
 /* struct mesh_p *msh_p;
  msh_p = (struct mesh_p*) calloc(1, sizeof(struct mesh_p));
  struct multihop_p *mhop_p;
  mhop_p = (struct multihop_p*) calloc(1, sizeof(struct multihop_p));*/
  stack[0].pip = pi0; 
  stack[0].pip->channel = ch0; 
  //stack[0].pip->mesh_param = msh_p; 
  //stack[0].pip->multihop_param = mhop_p; 
  stack[0].modno = 6; 
  struct stackmodule_i *amodule0; 
  amodule0 = (struct stackmodule_i*) calloc( 
      stack[0].modno, sizeof(struct stackmodule_i)); 
  addr.u8[0]=rimeaddr_node_addr.u8[0];
  addr.u8[1]=rimeaddr_node_addr.u8[1];
  //addr.u8[0] = 0; addr.u8[1] = 0; 
  set_node_addr(0, OUT, SENDER, &addr); 

  static struct packetbuf_attrlist c_attributes0[] = 
  { 
      C_MESH_ATTRIBUTES PACKETBUF_ATTR_LAST 
  }; 

  stack[0].pip->channel_no = 0; 
  stack[0].pip->attrlist = c_attributes0; 
  stack[0].pip->channel->channelno = stack[0].pip->channel_no; 
  stack[0].pip->channel->attrlist = stack[0].pip->attrlist; 
  stack[0].amodule = amodule0; 

  amodule0[0].stack_id = 0; 
  amodule0[0].module_id = 0; 
  amodule0[0].parent = NULL;
  stack[0].pip->channel_no = 111;
  amodule0[0].c_open = c_channel_open;
  amodule0[0].c_close = c_channel_close;
  amodule0[0].c_recv = c_abc_input;
  amodule0[0].c_send = c_rime_output;

  amodule0[1].stack_id = 0; 
  amodule0[1].module_id = 1; 
  amodule0[1].parent = NULL;
  amodule0[1].time_trigger_flg = 0;
  addr.u8[0]=rimeaddr_node_addr.u8[0];
  addr.u8[1]=rimeaddr_node_addr.u8[1];
  //addr.u8[0] = 1; addr.u8[1] = 0; 
  set_node_addr(0, OUT, SENDER, &addr);
  amodule0[1].c_open = c_broadcast_open;
  amodule0[1].c_close = c_broadcast_close;
  amodule0[1].c_send = c_broadcast_send;
  amodule0[1].c_sent = c_broadcast_sent;
  amodule0[1].c_recv = c_broadcast_recv;

  amodule0[2].stack_id = 0; 
  amodule0[2].module_id = 2; 
  amodule0[2].parent = NULL;
  amodule0[2].time_trigger_flg = 0;
  addr.u8[0] = 3; addr.u8[1] = 0;
  packetbuf_set_addr(PACKETBUF_ADDR_RECEIVER, &addr);
  set_node_addr(0, OUT, RECEIVER, &addr);
  amodule0[2].c_open = c_unicast_open;
  amodule0[2].c_close = c_unicast_close;
  amodule0[2].c_send = c_unicast_send;
  amodule0[2].c_sent = c_unicast_sent;
  amodule0[2].c_recv = c_unicast_recv;

  amodule0[3].stack_id = 0; 
  amodule0[3].module_id = 3; 
  amodule0[3].parent = NULL;
  amodule0[3].time_trigger_flg = 0;
  addr.u8[0]=3;addr.u8[1]=0;
  //addr.u8[0] = 1; addr.u8[1] = 0;  
  set_node_addr(0, OUT, ERECEIVER, &addr);
  addr.u8[0]=rimeaddr_node_addr.u8[0];
  addr.u8[1]=rimeaddr_node_addr.u8[1];
  //addr.u8[0] = 2; addr.u8[1] = 0; 
  set_node_addr(0, OUT, ESENDER, &addr);
  amodule0[3].c_open = c_multihop_open;
  amodule0[3].c_close = c_multihop_close;
  amodule0[3].c_send = c_multihop_send;
  amodule0[3].c_sent = c_multihop_sent;
  amodule0[3].c_recv = c_multihop_recv;
  //rimeaddr_copy(amodule0[3].c_forward, c_multihop_forward);
  amodule0[3].c_forward = c_multihop_forward;

  amodule0[4].stack_id = 0; 
  amodule0[4].module_id = 4; 
  amodule0[4].parent = NULL;
  amodule0[4].time_trigger_flg = 0;
  amodule0[4].c_open = c_mesh_open;
  amodule0[4].c_close = c_mesh_close;
  amodule0[4].c_send = c_mesh_send;
  amodule0[4].c_sent = c_mesh_sent;
  amodule0[4].c_recv = c_mesh_recv;
  amodule0[4].c_forward = c_mesh_forward;
  //rimeaddr_copy(amodule0[4].c_forward, c_mesh_forward);
  amodule0[4].c_timed_out = c_mesh_timedout;

  amodule0[5].stack_id = 0; 
  amodule0[5].module_id = 5; 
  amodule0[5].parent = NULL;
  amodule0[5].time_trigger_flg = 0;
  amodule0[5].c_open = c_echo_app_open;
  amodule0[5].c_close = c_echo_app_close;
  amodule0[5].c_send = c_echo_app_send;
  amodule0[5].c_recv = c_echo_app_recv;
  amodule0[5].c_sent = c_echo_app_sent;
  rimeaddr_copy(amodule0[5].c_forward, c_echo_app_forward);
  //amodule0[5].c_forward = c_echo_app_forward;
  amodule0[5].c_timed_out = c_echo_app_timedout;

  

  struct pipe *pi1; 
  pi1 = (struct pipe*) calloc(1, sizeof(struct pipe)); 
  struct channel *ch1; 
  ch1 = (struct channel*) calloc(1, sizeof(struct channel));
  /*struct polite_p *poli_p;
  poli_p = (struct polite_p*) calloc(1, sizeof(struct polite_p));
  struct netflood_p *netfl_p;
  netfl_p = (struct netflood_p*) calloc(1, sizeof(struct netflood_p));
  struct route_discovery_p *route_discov_p;
  route_discov_p = (struct route_discovery_p*) calloc(1, sizeof(struct route_discovery_p));*/
  stack[1].pip = pi1; 
  stack[1].pip->channel = ch1;
  /*stack[1].pip->polite_param = poli_p;
  stack[1].pip->netflood_param = netfl_p;
  stack[1].pip->route_discovery_param = route_discov_p;*/
  stack[1].modno = 5; 
  struct stackmodule_i *amodule1; 
  amodule1 = (struct stackmodule_i*) calloc( 
      stack[1].modno, sizeof(struct stackmodule_i)); 
  addr.u8[0]=rimeaddr_node_addr.u8[0];
  addr.u8[1]=rimeaddr_node_addr.u8[1];
  //addr.u8[0] = 0; addr.u8[1] = 0; 
  set_node_addr(0, OUT, SENDER, &addr); 

  static struct packetbuf_attrlist c_attributes1[] = 
  { 
      C_ROUTE_DISCOVERY_ATTRIBUTES PACKETBUF_ATTR_LAST 
  }; 

  stack[1].pip->channel_no = 0; 
  stack[1].pip->attrlist = c_attributes1; 
  stack[1].pip->channel->channelno = stack[1].pip->channel_no; 
  stack[1].pip->channel->attrlist = stack[1].pip->attrlist; 
  stack[1].amodule = amodule1; 

  amodule1[0].stack_id = 1; 
  amodule1[0].module_id = 0; 
  amodule1[0].parent = NULL;
  stack[1].pip->channel_no = 112;
  amodule1[0].c_open = c_channel_open;
  amodule1[0].c_close = c_channel_close;
  amodule1[0].c_recv = c_abc_input;
  amodule1[0].c_send = c_rime_output;

  amodule1[1].stack_id = 1; 
  amodule1[1].module_id = 1; 
  amodule1[1].parent = NULL;
  amodule1[1].time_trigger_flg = 0;
  addr.u8[0]=rimeaddr_node_addr.u8[0];
  addr.u8[1]=rimeaddr_node_addr.u8[1];
  //addr.u8[0] = 1; addr.u8[1] = 0; 
  set_node_addr(1, OUT, SENDER, &addr);
  amodule1[1].c_open = c_broadcast_open;
  amodule1[1].c_close = c_broadcast_close;
  amodule1[1].c_send = c_broadcast_send;
  amodule1[1].c_sent = c_broadcast_sent;
  amodule1[1].c_recv = c_broadcast_recv;

  amodule1[2].stack_id = 1; 
  amodule1[2].module_id = 2; 
  amodule1[2].parent = NULL;
  amodule1[2].time_trigger_flg = 0;
  amodule1[2].c_open = c_polite_open;
  amodule1[2].c_close = c_polite_close;
  amodule1[2].c_send = c_polite_send;
  amodule1[2].c_sent = c_polite_sent;
  amodule1[2].c_recv = c_polite_recv;
  amodule1[2].c_dropped = c_polite_dropped;

  amodule1[3].stack_id = 1; 
  amodule1[3].module_id = 3; 
  amodule1[3].parent = NULL;
  amodule1[3].time_trigger_flg = 0;
  amodule1[3].c_close = c_netflood_close;
  amodule1[3].c_send = c_netflood_send;
  amodule1[3].c_sent = c_netflood_sent;
  amodule1[3].c_recv = c_netflood_recv;
  amodule1[3].c_dropped = c_netflood_dropped;

  amodule1[4].stack_id = 1; 
  amodule1[4].module_id = 4; 
  amodule1[4].parent = &amodule0[4];
  amodule1[4].time_trigger_flg = 0;
  //addr.u8[0] = 3; addr.u8[1] = 0;
  //set_node_addr(1, OUT, ERECEIVER, &addr);
  amodule1[4].c_open = c_route_discovery_open;
  amodule1[4].c_close = c_route_discovery_close;
  amodule1[4].c_send = c_route_discovery_discover;
  amodule1[4].c_recv = c_route_discovery_recv;
  amodule1[4].c_timed_out = c_route_discovery_timedout;

  struct pipe *pi2; 
  pi2 = (struct pipe*) calloc(1, sizeof(struct pipe)); 
  struct channel *ch2; 
  ch2 = (struct channel*) calloc(1, sizeof(struct channel)); 
  stack[2].pip = pi2; 
  stack[2].pip->channel = ch2; 
  stack[2].modno = 3;
  struct stackmodule_i *amodule2; 
  amodule2 = (struct stackmodule_i*) calloc( 
      stack[2].modno, sizeof(struct stackmodule_i));
  addr.u8[0]=rimeaddr_node_addr.u8[0];
  addr.u8[1]=rimeaddr_node_addr.u8[1];
  //addr.u8[0] = 0; addr.u8[1] = 0; 
  set_node_addr(0, OUT, SENDER, &addr); 

  static struct packetbuf_attrlist c_attributes2[] = 
  { 
      C_UNICAST_ATTRIBUTES PACKETBUF_ATTR_LAST 
  }; 

  stack[2].pip->channel_no = 0; 
  stack[2].pip->attrlist = c_attributes2; 
  stack[2].pip->channel->channelno = stack[2].pip->channel_no; 
  stack[2].pip->channel->attrlist = stack[2].pip->attrlist; 
  stack[2].amodule = amodule2; 

  amodule2[0].stack_id = 2; 
  amodule2[0].module_id = 0; 
  amodule2[0].parent = NULL;
  stack[2].pip->channel_no = 113;
  amodule2[0].c_open = c_channel_open;
  amodule2[0].c_close = c_channel_close;
  amodule2[0].c_recv = c_abc_input;
  amodule2[0].c_send = c_rime_output;

  amodule2[1].stack_id = 2; 
  amodule2[1].module_id = 1; 
  amodule2[1].parent = NULL;
  amodule2[1].time_trigger_flg = 0;
  addr.u8[0]=rimeaddr_node_addr.u8[0];
  addr.u8[1]=rimeaddr_node_addr.u8[1];
  //addr.u8[0] = 1; addr.u8[1] = 0; 
  set_node_addr(2, OUT, SENDER, &addr);
  amodule2[1].c_open = c_broadcast_open;
  amodule2[1].c_close = c_broadcast_close;
  amodule2[1].c_send = c_broadcast_send;
  amodule2[1].c_sent = c_broadcast_sent;
  amodule2[1].c_recv = c_broadcast_recv;

  amodule2[2].stack_id = 2; 
  amodule2[2].module_id = 2; 
  amodule2[2].parent = &amodule1[4];
  amodule2[2].time_trigger_flg = 0;
  addr.u8[0] = 3; addr.u8[1] = 0; 
  set_node_addr(2, OUT, RECEIVER, &addr);
  amodule2[2].c_open = c_unicast_open;
  amodule2[2].c_close = c_unicast_close;
  amodule2[2].c_send = c_unicast_send;
  amodule2[2].c_sent = c_unicast_sent;
  amodule2[2].c_recv = c_unicast_recv;

}

void
stack_open(struct stack_i *stack)
{
  PRINTF("stack_open \n");
  int p;

  for(p = 0; p < STACKNO; p++) {
    int modno = 0;

    PRINTF("%d \n", stack[p].amodule[0].stack_id);
    if(stack[p].amodule[modno].c_open != NULL) {
      c_open(stack[p].pip, stack[p].amodule, modno);
      PRINTF("~%d \n", stack[p].amodule[0].stack_id);
    }
  }
  PRINTF("~stack_open \n");
}

void
stack_close(struct stack_i *stack)
{
  PRINTF("stack_close \n");
  int p;

  for(p = 0; p < STACKNO; p++) {
    int modno = stack[p].modno - 1;

    if(stack[p].amodule[modno].c_close != NULL) {
      c_close(stack[p].pip, stack[p].amodule, modno);
    }
  }
  PRINTF("~stack_close \n");
}

int
stack_send(struct stack_i *stack, uint8_t module_id)
{
  PRINTF("stack_send %d\n", module_id);
  int p;
  int sent_flag = 0;

  for(p = 0; p < STACKNO; p++) {
    int modno;

    if(module_id == -1) {
      modno = stack[p].modno - 1;
    } else {
      modno = module_id;
    }
    if(stack[p].amodule[modno].c_send != NULL) {
      sent_flag = c_send(stack[p].pip, stack[p].amodule, modno);
    }
    break;
  }
  PRINTF("~stack_send \n");
  return sent_flag;
}

void
stack_dropped(struct stack_i *stack, uint8_t module_id)
{
  PRINTF("stack_dropped \n");
  if (module_id < 0) {return; }
  if (module_id <= stack->modno)
  {
    int modno = module_id;
    if(stack->amodule[modno].c_dropped != NULL) 
    {
      c_dropped(stack->pip, stack->amodule, modno);
    }
  }
  PRINTF("~stack_dropped \n");
}

void
stack_timedout(struct stackmodule_i *module)
{
  PRINTF("stack_timedout \n");
  uint8_t stack_id = module->stack_id;

  uint8_t mod_id = module->module_id;

  int modno = stack[stack_id].modno - 1;

  if(mod_id <= modno) {
    if(stack[stack_id].amodule[modno].c_timed_out != NULL) {
      c_timed_out(stack[stack_id].pip, stack[stack_id].amodule, modno);
    }
  }
  if(stack[stack_id].amodule[mod_id].parent != NULL) {
    stack_timedout(stack[stack_id].amodule[mod_id].parent);
  }
  PRINTF("~stack_timedout \n");
}

void
stack_recv(struct stackmodule_i *module)
{
  PRINTF("stack_recv \n");
  PRINTF("stack_id: %d\n",module->stack_id);
  uint8_t stack_id = module->stack_id;
  uint8_t mod_id = module->module_id;
  
  if(stack[stack_id].not_dest_flg == 1) {
    return;
  }
  
  if(stack[module->stack_id].resend_flg == 1) {
    return;
  }
  
  int modno = stack[stack_id].modno - 1;
  
  if(mod_id <= modno) {
    if(stack[stack_id].amodule[modno].c_recv != NULL) {
      c_recv(stack[stack_id].pip, stack[stack_id].amodule, mod_id);
    }
  }

  if(module->module_id == 0 && stack[module->stack_id].not_dest_flg == 1) {
    stack[stack_id].not_dest_flg = 0; 
    return;
  }

  if(stack[stack_id].amodule[modno].parent != NULL) {
    stack[stack_id].merged_flg = 1;
    uint8_t parent_stack_id = stack[stack_id].amodule[modno].parent->stack_id;
    uint8_t parent_mod_id = stack[stack_id].amodule[modno].parent->module_id;
    stack_recv(&stack[parent_stack_id].amodule[parent_mod_id]);
  }
  PRINTF("~stack_recv \n");
}
/*
void stack_discover(struct stack_i *stack){
	PRINTF("stack_discover \n");
		int p;
		for (p = 0; p < STACKNO; p++) {
			int modno = stack[p].modno - 1;
			if (stack[p].amodule[modno].c_discover != NULL){
				c_discover(stack[p].pip, stack[p].amodule, modno - 1);
			}
		}
		PRINTF("~stack_dropped \n");
}
 */
