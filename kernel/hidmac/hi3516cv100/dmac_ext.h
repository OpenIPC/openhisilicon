#ifndef __DMAC_EXT_H__
#define __DMAC_EXT_H__

/*the defination for the peripheral*/
#define UUID_HI_DMAC_V_1_0_0_0 "hi-dmac-1.0.0.0"

#if (HICHIP==0x35310100 || HICHIP==0x35320100) 

#define DMAC_SIO0_RX_REQ  	                 0
#define DMAC_SIO1_RX_REQ  	                 1
#define DMAC_SIO2_RX_REQ  	                 2
#define DMAC_SIO3_RX_REQ  	                 3
#define DMAC_SIO4_RX_REQ                     4
#define DMAC_SIO4_TX_REQ                     5
#define DMAC_SIO5_TX_REQ  	                 6

#define DMAC_SIO0_RX_CHN                   0
#define DMAC_SIO1_RX_CHN                   1
#define DMAC_SIO2_RX_CHN                   2
#define DMAC_SIO3_RX_CHN                   3
#define DMAC_SIO4_RX_CHN                   4
#define DMAC_SIO4_TX_CHN                   5
#define DMAC_SIO5_TX_CHN                   6

#elif (HICHIP==0x35210100 || HICHIP==0x3520A100) 
#define DMAC_SIO0_RX_REQ  	                 0
#define DMAC_SIO1_RX_REQ  	                 1
#define DMAC_SIO2_RX_REQ  	                 2
#define DMAC_SIO2_TX_REQ  	                 3
#define DMAC_SIO3_TX_REQ  	                 4

#define DMAC_SIO0_RX_CHN                   0
#define DMAC_SIO1_RX_CHN                   1
#define DMAC_SIO2_RX_CHN                   2
#define DMAC_SIO2_TX_CHN                   3
#define DMAC_SIO3_TX_CHN                   4

#elif (HICHIP==0x35180100) 
#define DMAC_SIO0_RX_REQ  	                 0
#define DMAC_SIO0_TX_REQ  	                 1

#endif


#define DMAC_UART_RX_REQ                    12
#define DMAC_UART_TX_REQ                    13
#define DMAC_SSP_RX_REQ  	                14
#define DMAC_SSP_TX_REQ  	                15

/*sturcture for LLI*/
typedef struct dmac_lli
{
        unsigned int src_addr;                 /*source address*/
        unsigned int dst_addr;                 /*destination address*/
        unsigned int next_lli;                 /*pointer to next LLI*/
        unsigned int lli_transfer_ctrl;            /*control word*/
}dmac_lli;

extern int dma_driver_init(void);
extern int dmac_m2p_transfer(unsigned int *pmemaddr, unsigned int uwperipheralid, unsigned int  uwtransfersize);
extern int dmac_m2m_transfer(unsigned int *psource, unsigned int *pdest, unsigned int  uwtransfersize);

extern int dmac_channelclose(unsigned int channel);
extern int dmac_register_isr(unsigned int channel,void *pisr);
extern int dmac_channel_free(unsigned int channel);
extern int free_dmalli_space(unsigned int *ppheadlli, unsigned int page_num);
extern int dmac_start_llim2p(unsigned int channel, unsigned int *pfirst_lli, unsigned int uwperipheralid);
extern int dmac_buildllim2m(unsigned int * ppheadlli, unsigned int pdest, unsigned int psource, unsigned int totaltransfersize, unsigned int uwnumtransfers);
extern int dmac_channelstart(unsigned int u32channel);
extern int dmac_start_llim2m(unsigned int channel, unsigned int *pfirst_lli);
extern int  dmac_channel_allocate(void *pisr);
extern int allocate_dmalli_space(unsigned int *ppheadlli, unsigned int page_num);
extern int  dmac_buildllim2p( unsigned int *ppheadlli, unsigned int *pmemaddr,
                       unsigned int uwperipheralid, unsigned int totaltransfersize,
                       unsigned int uwnumtransfers ,unsigned int burstsize);
extern int dmac_start_m2p(unsigned int channel, unsigned int pmemaddr, unsigned int uwperipheralid, unsigned int  uwnumtransfers,unsigned int next_lli_addr);
extern int dmac_start_m2m(unsigned int channel, unsigned int psource, unsigned int pdest, unsigned int uwnumtransfers);
extern int dmac_wait(unsigned int channel);

#endif

