/**
\file
\brief cipher 
\copyright Shenzhen Hisilicon Co., Ltd.
\date 2008-2018
\version draft
\author QuYaxin 46153
\date 2009-11-3
*/

#ifndef __CIPHER_REG_H__
#define __CIPHER_REG_H__

/* add include here */


#ifdef __cplusplus
extern "C"
{
#endif
/***************************** Macro Definition ******************************/

#define  REG_SYS_CIPHER_CLK_ADDR_PHY     0x2003007C

/* CIPHER register addr define */
#define  REG_CI_BASE_ADDR_PHY      0x100c0000
#define  REG_CI_BASE_SIZE          0x2000

#define  REG_CI_CHAN0_CIPHER_DOUT(id)       (REG_CI_BASE_ADDR + 0x0000 + (id)*0)
#define  REG_CI_CHAN0_CIPHER_IVOUT(id)      (REG_CI_BASE_ADDR + 0x0010 + (id)*0)
#define  REG_CI_CHAN_CIPHER_IVOUT(id)       (REG_CI_BASE_ADDR + 0x0010 + (id)*32)
#define  REG_CI_CIPHER_KEY(id)              (REG_CI_BASE_ADDR + 0x0090 + (id)*32)
#define  REG_CI_CTRL_ST0                    (REG_CI_BASE_ADDR + 0x0200)
#define  REG_CI_CTRL_ST1                    (REG_CI_BASE_ADDR + 0x0204)
#define  REG_CI_SRAM_ST0                    (REG_CI_BASE_ADDR + 0x0208)
#define  REG_CI_CHAN0_CIPHER_CTRL           (REG_CI_BASE_ADDR + 0x1000)
#define  REG_CI_CHAN0_CIPHER_IVIN(id)       (REG_CI_BASE_ADDR + 0x1004 + (id)*0)
#define  REG_CI_CHAN0_CIPHER_DIN(id)        (REG_CI_BASE_ADDR + 0x1014 + (id)*0)
#define  REG_CI_CHANn_IBUF_NUM(id)          (REG_CI_BASE_ADDR + 0x1000 + (id)*128 )
#define  REG_CI_CHANn_IBUF_CNT(id)          (REG_CI_BASE_ADDR + 0x1000 + (id)*128 + 0x4 )
#define  REG_CI_CHANn_IEMPTY_CNT(id)        (REG_CI_BASE_ADDR + 0x1000 + (id)*128 + 0x8 )
#define  REG_CI_CHANn_INT_ICNTCFG(id)       (REG_CI_BASE_ADDR + 0x1000 + (id)*128 + 0xC )
#define  REG_CI_CHANn_CIPHER_CTRL(id)       (REG_CI_BASE_ADDR + 0x1000 + (id)*128 + 0x10)
#define  REG_CI_CHANn_SRC_LST_SADDR(id)     (REG_CI_BASE_ADDR + 0x1000 + (id)*128 + 0x14)
#define  REG_CI_CHANn_IAGE_TIMER(id)        (REG_CI_BASE_ADDR + 0x1000 + (id)*128 + 0x18)
#define  REG_CI_CHANn_IAGE_CNT(id)          (REG_CI_BASE_ADDR + 0x1000 + (id)*128 + 0x1C)
#define  REG_CI_CHANn_SRC_LST_RADDR(id)     (REG_CI_BASE_ADDR + 0x1000 + (id)*128 + 0x20)
#define  REG_CI_CHANn_SRC_ADDR(id)          (REG_CI_BASE_ADDR + 0x1000 + (id)*128 + 0x24)
#define  REG_CI_CHANn_SRC_LENGTH(id)        (REG_CI_BASE_ADDR + 0x1000 + (id)*128 + 0x28)
#define  REG_CI_CHANn_IN_LEFT_BYTE0(id)     (REG_CI_BASE_ADDR + 0x1000 + (id)*128 + 0x2C)
#define  REG_CI_CHANn_IN_LEFT_WORD0(id)     (REG_CI_BASE_ADDR + 0x1000 + (id)*128 + 0x30)
#define  REG_CI_CHANn_IN_LEFT_WORD1(id)     (REG_CI_BASE_ADDR + 0x1000 + (id)*128 + 0x34)
#define  REG_CI_CHANn_IN_LEFT_WORD2(id)     (REG_CI_BASE_ADDR + 0x1000 + (id)*128 + 0x38)
#define  REG_CI_CHANn_OBUF_NUM(id)          (REG_CI_BASE_ADDR + 0x1000 + (id)*128 + 0x3C)
#define  REG_CI_CHANn_OBUF_CNT(id)          (REG_CI_BASE_ADDR + 0x1000 + (id)*128 + 0x40)
#define  REG_CI_CHANn_OFULL_CNT(id)         (REG_CI_BASE_ADDR + 0x1000 + (id)*128 + 0x44)
#define  REG_CI_CHANn_INT_OCNTCFG(id)       (REG_CI_BASE_ADDR + 0x1000 + (id)*128 + 0x48)
#define  REG_CI_CHANn_DEST_LST_SADDR(id)    (REG_CI_BASE_ADDR + 0x1000 + (id)*128 + 0x4C)
#define  REG_CI_CHANn_OAGE_TIMER(id)        (REG_CI_BASE_ADDR + 0x1000 + (id)*128 + 0x50)
#define  REG_CI_CHANn_OAGE_CNT(id)          (REG_CI_BASE_ADDR + 0x1000 + (id)*128 + 0x54)
#define  REG_CI_CHANn_DEST_LST_RADDR(id)    (REG_CI_BASE_ADDR + 0x1000 + (id)*128 + 0x58)
#define  REG_CI_CHANn_DEST_ADDR(id)         (REG_CI_BASE_ADDR + 0x1000 + (id)*128 + 0x5C)
#define  REG_CI_CHANn_DEST_LENGTH(id)       (REG_CI_BASE_ADDR + 0x1000 + (id)*128 + 0x60)
#define  REG_CI_CHANn_OUT_LEFT_BYTE(id)     (REG_CI_BASE_ADDR + 0x1000 + (id)*128 + 0x64)
#define  REG_CI_INT_STATUS                  (REG_CI_BASE_ADDR + 0x1400 )
#define  REG_CI_INT_EN                      (REG_CI_BASE_ADDR + 0x1404 )
#define  REG_CI_INT_RAW                     (REG_CI_BASE_ADDR + 0x1408 )
#define  REG_CI_CHAN0_CFG                   (REG_CI_BASE_ADDR + 0x1410 )
#define  REG_CI_BRAM_ST0                    (REG_CI_BASE_ADDR + 0x1420 )
#define  REG_CI_IN_ST0                      (REG_CI_BASE_ADDR + 0x1424 )
#define  REG_CI_IN_ST1                      (REG_CI_BASE_ADDR + 0x1428 )
#define  REG_CI_IN_ST2                      (REG_CI_BASE_ADDR + 0x142C )
#define  REG_CI_IN_ST3                      (REG_CI_BASE_ADDR + 0x1430 )
#define  REG_CI_OUT_ST0                     (REG_CI_BASE_ADDR + 0x1434 )
#define  REG_CI_OUT_ST1                     (REG_CI_BASE_ADDR + 0x1438 )
#define  REG_CI_OUT_ST2                     (REG_CI_BASE_ADDR + 0x143C )


/*************************** Structure Definition ****************************/



/********************** Global Variable declaration **************************/



/******************************* API declaration *****************************/



#ifdef __cplusplus
}
#endif
#endif /* __CIPHER_REG_H__ */
