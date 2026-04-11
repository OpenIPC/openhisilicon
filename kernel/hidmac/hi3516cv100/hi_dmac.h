/* ./arch/arm/mach-hi3511_v100_f01/dmac.h
 *
 *
 * History:
 *      17-August-2006 create this file
 */
#ifndef __HI_DMAC_H__
#define __HI_DMAC_H__

#define OSDRV_MODULE_VERSION_STRING "HIDMAC-MDC030003 @Hi3511v110_OSDrv_1_0_0_7 2009-03-18 20:52:37"

	#define  dmac_writew(addr,value)      ((*(volatile unsigned int *)(addr)) = (value))
	#define  dmac_readw(addr,v)           (v =(*(volatile unsigned int *)(addr)))


	#define DDRAM_ADRS              0x80000000              /* fixed */
	#define DDRAM_SIZE              0x3FFFFFFF              /* 1GB DDR. */

/*不确定flash的基址*/
	#define FLASH_BASE              0x50000000
	#define FLASH_SIZE              0x04000000              /* (32MB) */

	#define ITCM_BASE               0x0
	#define ITCM_SIZE               0x800


	#define DMAC_CONFIGURATIONx_HALT_DMA_ENABLE     (0x01L<<18)
	#define DMAC_CONFIGURATIONx_ACTIVE              (0x01L<<17)
	#define DMAC_CONFIGURATIONx_CHANNEL_ENABLE       1
	#define DMAC_CONFIGURATIONx_CHANNEL_DISABLE      0


	#define DMAC_BASE_REG                           0x100D0000
#if HICHIP==0x35310100
    #define DMAC_INT                                61
#elif HICHIP==0x35320100
    #define DMAC_INT                                14
#elif HICHIP==0x35210100
    #define DMAC_INT                                61
#elif HICHIP==0x3520A100
    #define DMAC_INT                                61    
#elif HICHIP==0x35180100
    #define DMAC_INT                                14
#endif

    #define DMAC_INTTCCLEAR                         IO_DMAC_ADDRESS(DMAC_BASE_REG + 0X08)


	#define DMAC_INTSTATUS 	                        IO_DMAC_ADDRESS(DMAC_BASE_REG + 0X00)
	#define DMAC_INTTCSTATUS                        IO_DMAC_ADDRESS(DMAC_BASE_REG + 0X04)
	#define DMAC_INTERRORSTATUS                     IO_DMAC_ADDRESS(DMAC_BASE_REG + 0X0C)

	#define DMAC_INTERRCLR                          IO_DMAC_ADDRESS(DMAC_BASE_REG + 0X10)
	#define DMAC_RAWINTTCSTATUS                     IO_DMAC_ADDRESS(DMAC_BASE_REG + 0X14)
	#define DMAC_RAWINTERRORSTATUS                  IO_DMAC_ADDRESS(DMAC_BASE_REG + 0X18)
	#define DMAC_ENBLDCHNS                          IO_DMAC_ADDRESS(DMAC_BASE_REG + 0X1C)
	#define DMAC_CONFIG                             IO_DMAC_ADDRESS(DMAC_BASE_REG + 0X30)
	#define DMAC_SYNC                               IO_DMAC_ADDRESS(DMAC_BASE_REG + 0X34)

	#define DMAC_MAXTRANSFERSIZE                    0x0fff /*the max length is denoted by 0-11bit*/
	#define MAXTRANSFERSIZE                         DMAC_MAXTRANSFERSIZE
	#define DMAC_CxDISABLE                          0x00
	#define DMAC_CxENABLE                           0x01

	/*the definition for DMAC channel register*/

	#define DMAC_CxBASE(i)                          IO_DMAC_ADDRESS(DMAC_BASE_REG + 0x100+i*0x20)
	#define DMAC_CxSRCADDR(i)                       DMAC_CxBASE(i)
	#define DMAC_CxDESTADDR(i)                      (DMAC_CxBASE(i)+0x04)
	#define DMAC_CxLLI(i)                           (DMAC_CxBASE(i)+0x08)
	#define DMAC_CxCONTROL(i)                       (DMAC_CxBASE(i)+0x0C)
	#define DMAC_CxCONFIG(i)                        (DMAC_CxBASE(i)+0x10)

	/*the means the bit in the channel control register*/

	#define DMAC_CxCONTROL_M2M                      0x8f489000  /* Dwidth=32,burst size=4 */
	#define DMAC_CxCONTROL_LLIM2M                   0x0f480000  /* Dwidth=32,burst size=1 */
	#define DMAC_CxLLI_LM                           0x01

	#define DMAC_CxCONFIG_M2M                       0xc000
	#define DMAC_CxCONFIG_LLIM2M                    0xc000

	/*#define DMAC_CxCONFIG_M2M  0x4001*/

	#define DMAC_CHANNEL_ENABLE                     1
	#define DMAC_CHANNEL_DISABLE                    0xfffffffe

	#define DMAC_CxCONTROL_P2M                      0x89409000
	#define DMAC_CxCONFIG_P2M                       0xd000

	#define DMAC_CxCONTROL_M2P                      0x86089000
	#define DMAC_CxCONFIG_M2P                       0xc800

    #define DMAC_CxCONFIG_SIO_P2M                   0x0000d000
    #define DMAC_CxCONFIG_SIO_M2P                   0x0000c800

	/*default the config and sync regsiter for DMAC controller*/
	#define DMAC_CONFIG_VAL                         0x01    /*M1,M2 little endian, enable DMAC*/
	#define DMAC_SYNC_VAL                           0x0     /*enable the sync logic for the 16 peripheral*/

	/*definition for the return value*/

	#define DMAC_ERROR_BASE                         100
	#define DMAC_CHANNEL_INVALID                    (DMAC_ERROR_BASE+1)

	#define DMAC_TRXFERSIZE_INVALID                 (DMAC_ERROR_BASE+2)
	#define DMAC_SOURCE_ADDRESS_INVALID             (DMAC_ERROR_BASE+3)
	#define DMAC_DESTINATION_ADDRESS_INVALID        (DMAC_ERROR_BASE+4)
	#define DMAC_MEMORY_ADDRESS_INVALID             (DMAC_ERROR_BASE+5)
	#define DMAC_PERIPHERAL_ID_INVALID              (DMAC_ERROR_BASE+6)
	#define DMAC_DIRECTION_ERROR                    (DMAC_ERROR_BASE+7)
	#define DMAC_TRXFER_ERROR                       (DMAC_ERROR_BASE+8)
	#define DMAC_LLIHEAD_ERROR                      (DMAC_ERROR_BASE+9)
	#define DMAC_SWIDTH_ERROR                       (DMAC_ERROR_BASE+0xa)
	#define DMAC_LLI_ADDRESS_INVALID                (DMAC_ERROR_BASE+0xb)
	#define DMAC_TRANS_CONTROL_INVALID              (DMAC_ERROR_BASE+0xc)
	#define DMAC_MEMORY_ALLOCATE_ERROR              (DMAC_ERROR_BASE+0xd)
	#define DMAC_NOT_FINISHED                       (DMAC_ERROR_BASE+0xe)

	#define DMAC_TIMEOUT	                        (DMAC_ERROR_BASE+0xf)
	#define DMAC_CHN_SUCCESS                        (DMAC_ERROR_BASE+0x10)
	#define DMAC_CHN_ERROR                          (DMAC_ERROR_BASE+0x11)
	#define DMAC_CHN_TIMEOUT                        (DMAC_ERROR_BASE+0x12)
	#define DMAC_CHN_ALLOCAT                        (DMAC_ERROR_BASE+0x13)
	#define DMAC_CHN_VACANCY                        (DMAC_ERROR_BASE+0x14)

	#define DMAC_CONFIGURATIONx_ACTIVE_NOT           0

	#define DMAC_MAX_PERIPHERALS	                 16
	#define MEM_MAX_NUM                              3
	
#if  HICHIP==0x35180100
	#define DMAC_MAX_CHANNELS 	                     4
#else
	#define DMAC_MAX_CHANNELS 	                     8
#endif 

	/*define the Data register for the pepherial */
#if HICHIP==0x35180100
    #define SIO0_BASE_REG                            0x10040000
    #define SIO0_TX_LEFT_FIFO                        (SIO0_BASE_REG+0x4c)
    #define SIO0_TX_RIGHT_FIFO                       (SIO0_BASE_REG+0x50)
    #define SIO0_RX_LEFT_FIFO                        (SIO0_BASE_REG+0x54)
    #define SIO0_RX_RIGHT_FIFO                       (SIO0_BASE_REG+0x58)
    #define SIO0_TX_FIFO                             (SIO0_BASE_REG+0xc0)
    #define SIO0_RX_FIFO                             (SIO0_BASE_REG+0xa0)

#else
    #define SIO0_BASE_REG                            0x10030000
    #define SIO0_TX_LEFT_FIFO                        (SIO0_BASE_REG+0x4c)
    #define SIO0_TX_RIGHT_FIFO                       (SIO0_BASE_REG+0x50)
    #define SIO0_RX_LEFT_FIFO                        (SIO0_BASE_REG+0x54)
    #define SIO0_RX_RIGHT_FIFO                       (SIO0_BASE_REG+0x58)
    #define SIO0_TX_FIFO                             (SIO0_BASE_REG+0xc0)
    #define SIO0_RX_FIFO                             (SIO0_BASE_REG+0xa0)

    #define SIO1_BASE_REG                            0x10040000
    #define SIO1_TX_LEFT_FIFO                        (SIO1_BASE_REG+0x4c)
    #define SIO1_TX_RIGHT_FIFO                       (SIO1_BASE_REG+0x50)
    #define SIO1_RX_LEFT_FIFO                        (SIO1_BASE_REG+0x54)
    #define SIO1_RX_RIGHT_FIFO                       (SIO1_BASE_REG+0x58)
    #define SIO1_TX_FIFO                             (SIO1_BASE_REG+0xc0)
    #define SIO1_RX_FIFO                             (SIO1_BASE_REG+0xa0)

    #define SIO2_BASE_REG                            0x10050000
    #define SIO2_TX_LEFT_FIFO                        (SIO2_BASE_REG+0x4c)
    #define SIO2_TX_RIGHT_FIFO                       (SIO2_BASE_REG+0x50)
    #define SIO2_RX_LEFT_FIFO                        (SIO2_BASE_REG+0x54)
    #define SIO2_RX_RIGHT_FIFO                       (SIO2_BASE_REG+0x58)
    #define SIO2_TX_FIFO                             (SIO2_BASE_REG+0xc0)
    #define SIO2_RX_FIFO                             (SIO2_BASE_REG+0xa0)

    #define SIO3_BASE_REG                            0x10060000
    #define SIO3_TX_LEFT_FIFO                        (SIO3_BASE_REG+0x4c)
    #define SIO3_TX_RIGHT_FIFO                       (SIO3_BASE_REG+0x50)
    #define SIO3_RX_LEFT_FIFO                        (SIO3_BASE_REG+0x54)
    #define SIO3_RX_RIGHT_FIFO                       (SIO3_BASE_REG+0x58)
    #define SIO3_TX_FIFO                             (SIO3_BASE_REG+0xc0)
    #define SIO3_RX_FIFO                             (SIO3_BASE_REG+0xa0)

    #define SIO4_BASE_REG                            0x100E0000
    #define SIO4_TX_LEFT_FIFO                        (SIO4_BASE_REG+0x4c)
    #define SIO4_TX_RIGHT_FIFO                       (SIO4_BASE_REG+0x50)
    #define SIO4_RX_LEFT_FIFO                        (SIO4_BASE_REG+0x54)
    #define SIO4_RX_RIGHT_FIFO                       (SIO4_BASE_REG+0x58)
    #define SIO4_TX_FIFO                             (SIO4_BASE_REG+0xc0)
    #define SIO4_RX_FIFO                             (SIO4_BASE_REG+0xa0)

    #define SIO5_BASE_REG                            0x100F0000
    #define SIO5_TX_LEFT_FIFO                        (SIO5_BASE_REG+0x4c)
    #define SIO5_TX_RIGHT_FIFO                       (SIO5_BASE_REG+0x50)
    #define SIO5_RX_LEFT_FIFO                        (SIO5_BASE_REG+0x54)
    #define SIO5_RX_RIGHT_FIFO                       (SIO5_BASE_REG+0x58)
    #define SIO5_TX_FIFO                             (SIO5_BASE_REG+0xc0)
    #define SIO5_RX_FIFO                             (SIO5_BASE_REG+0xa0)
#endif

#if 0
	#define SSP_BASE_REG                             0x200e0000
	#define SSP_DATA_REG                             (SSP_BASE_REG+0x08)
#endif

	#define MMC_BASE_REG                             0x10030000
	#define MMC_RX_REG                               (MMC_BASE_REG+0x100)
	#define MMC_TX_REG                               (MMC_BASE_REG+0x100)

	#define UART0_BASE_REG                           0x20080000
	#define UART0_DATA_REG                           (UART0_BASE_REG+0x0)

	#define UART1_BASE_REG                           0x20090000
	#define UART1_DATA_REG                           (UART1_BASE_REG+0x0)

    #define UART2_BASE_REG                           0x200A0000
	#define UART2_DATA_REG                           (UART2_BASE_REG+0x0)

    #define UART3_BASE_REG                           0x200B0000
	#define UART3_DATA_REG                           (UART3_BASE_REG+0x0)
	/*the transfer control and configuration value for different peripheral*/

    /* 修改burst的长度保证一次DMA传输能传输完256bits数据，避免音频输入串通道， 
    将RX_CONTROL由0x85489000改为0x8a492000 */
#if (HICHIP==0x35310100 || HICHIP==0x35320100) 
    #define SIO0_RX_CONTROL                          0x8a492000
	#define SIO0_RX_CONFIG                           (DMAC_CxCONFIG_SIO_P2M|(0x00<<1))

	#define SIO1_RX_CONTROL                          0x8a492000
	#define SIO1_RX_CONFIG                           (DMAC_CxCONFIG_SIO_P2M|(0x01<<1))

	#define SIO2_RX_CONTROL                          0x8a492000
	#define SIO2_RX_CONFIG                           (DMAC_CxCONFIG_SIO_P2M|(0x02<<1))

	#define SIO3_RX_CONTROL                          0x8a492000
	#define SIO3_RX_CONFIG                           (DMAC_CxCONFIG_SIO_P2M|(0x03<<1))

	#define SIO4_RX_CONTROL                          0x8a492000
	#define SIO4_RX_CONFIG                           (DMAC_CxCONFIG_SIO_P2M|(0x04<<1))

	#define SIO4_TX_CONTROL                          0x85489000
	#define SIO4_TX_CONFIG                           (DMAC_CxCONFIG_SIO_M2P|(0x05<<6))

	#define SIO5_TX_CONTROL                          0x85489000
	#define SIO5_TX_CONFIG                           (DMAC_CxCONFIG_SIO_M2P|(0x06<<6))
    
#elif (HICHIP==0x35210100 || HICHIP==0x3520A100) 

    #define SIO0_RX_CONTROL                          0x8a492000
	#define SIO0_RX_CONFIG                           (DMAC_CxCONFIG_SIO_P2M|(0x00<<1))

	#define SIO1_RX_CONTROL                          0x8a492000
	#define SIO1_RX_CONFIG                           (DMAC_CxCONFIG_SIO_P2M|(0x01<<1))

	#define SIO2_RX_CONTROL                          0x8a492000
	#define SIO2_RX_CONFIG                           (DMAC_CxCONFIG_SIO_P2M|(0x02<<1))

    #define SIO2_TX_CONTROL                          0x85489000
	#define SIO2_TX_CONFIG                           (DMAC_CxCONFIG_SIO_M2P|(0x03<<6))
    
	#define SIO3_TX_CONTROL                          0x85489000
	#define SIO3_TX_CONFIG                           (DMAC_CxCONFIG_SIO_M2P|(0x04<<6))

#elif (HICHIP==0x35180100) 

	//#define SIO0_RX_CONTROL                          0x8a492000
	#define SIO0_RX_CONTROL                          0x8a489000
	#define SIO0_RX_CONFIG                           (DMAC_CxCONFIG_SIO_P2M|(0x00<<1))

	#define SIO0_TX_CONTROL                          0x85489000
	#define SIO0_TX_CONFIG                           (DMAC_CxCONFIG_SIO_M2P|(0x01<<6))

#endif

	/*DMAC peripheral structure*/
	typedef struct dmac_peripheral
	{
	   	unsigned int         peri_id;          /* peripherial ID*/
	   	void                 *pperi_addr;      /*peripheral data register address*/
	   	unsigned int         transfer_ctrl;    /*default channel control word*/
	   	unsigned int         transfer_cfg;     /*default channel configuration word*/
	}dmac_peripheral;

	typedef struct mem_addr
	{
	    	unsigned int addr_base;
	    	unsigned int size;
	}mem_addr;


    extern int    g_channel_status[DMAC_MAX_CHANNELS];  //by H
#endif /* End of #ifndef __HI_INC_ECSDMACC_H__ */

