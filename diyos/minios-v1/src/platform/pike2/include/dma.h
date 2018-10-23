#ifndef __DMA_H__
#define __DMA_H__

#define DMA_BASE 0x20100000
#define DMA_REQ_UART0_RX 16
#define DMA_REQ_UART0_TX 17


#define DMA_PAUSE  0x0000
#define DMA_FRAG_WAIT 0x0004
#define REQ_PEND0_EN  0x0008
#define REG_PEND1_EN  0x000c
#define DMA_INT_RAW_STATUS 0x0010
#define DMA_INT_MASK_STATUS 0x0014
#define DMA_REQ_STATUS    0x0018
#define DMA_EN_STATUS    0x001c
#define DMA_DEBUG_STATUS 0x0020
#define DMA_ARB_SEL_STATUS   0x0024

#define CHN1_BASE 0x1000
#define CHN_SIZE 0x40
#define CID_BASE 0x2000

#define STD_CHN_PAUSE 0x0000
#define STD_CHN_REQ   0x0004    //bit0
#define STD_CHN_CFG   0x0008    //bit0 enable chn 
#define STD_CHN_INT   0x000c    //bit0  fragment done interrupt enable
#define STD_CHN_SRC_ADDR 0x0010
#define STD_CHN_DEST_ADDR 0x0014
#define STD_CHN_FRAG_LEN  0x0018
#define STD_CHN_BLK_EN    0x001c

#define FULL_CHN_PAUSE 0x0000
#define FULL_CHN_REQ   0x0004    //bit0
#define FULL_CHN_CFG   0x0008    //bit0 enable chn 
#define FULL_CHN_INT   0x000c
#define FULL_CHN_SRC_ADDR 0x0010
#define FULL_CHN_DEST_ADDR 0x0014
#define FULL_CHN_FRAG_LEN  0x0018
#define FULL_CHN_BLK_LEN    0x001c
#define FULL_CHN_TRSC_LEN  0x0020
#define FULL_CHN_TRSF_STEP 0x0024
#define FULL_CHN_WRAP_PTR 0x0028
#define FULL_CHN_WRAP_TO 0x002c
#define FULL_CHN_LLIST_PTR 0x0030
#define FULL_CHN_FRAG_STEP 0x0034
#define FULL_CHN_SRC_BLK_STEP 0x0038
#define FULL_CHN_DEST_BLK_STEP 0x003c


typedef enum {
    NONE_FIX = 0,
    SRC_FIX = 1,
    DEST_FIX =3,

} fix_sel;

void dma_module_enable();
void dma_module_reset();
void dma_full_chn_init(int ch);
void dma_full_chn_config_fragmemt(int ch,uint32_t src_addr,
            uint32_t dest_addr,int size,fix_sel fix,int data_size);
void dma_full_chn_sw_req(int ch);
void dma_full_chn_config_transaction(int ch,uint32_t src_addr,
                    uint32_t dest_addr,int size,int trans_size,fix_sel fix,int data_size);
                    
#endif