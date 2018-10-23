#ifdef AARCH32
#include <types.h>
#endif
#ifdef AARCH64
#include <types64.h>
#endif
#include <dma.h>


static uint32_t dma_chn_state = 0;//dma chn state   1 used  0 free

void dma_module_enable()
{
    REG32(0x20e00000) |= BIT_5;
}

void dma_module_reset()
{
    REG32(0x20e00004) |= BIT_8;
    int i=0;
    for(i=0;i<10;i++);
    REG32(0x20e00004) &= (~BIT_8);
}

void dma_full_chn_init(int ch)
{
    int i=0;
    for(i=0;i<CHN_SIZE;i +=4){
        REG32(DMA_BASE+CHN1_BASE+CHN_SIZE*(ch-1)+i) = 0;
    }
}


//size max=128K-1  byte
//data_size  0  1  2
void dma_full_chn_config_fragmemt(int ch,uint32_t src_addr,
                    uint32_t dest_addr,int size,fix_sel fix,int data_size)
{
    REG32(DMA_BASE+CHN1_BASE+CHN_SIZE*(ch-1)+FULL_CHN_SRC_ADDR ) = src_addr;
    REG32(DMA_BASE+CHN1_BASE+CHN_SIZE*(ch-1)+FULL_CHN_DEST_ADDR ) = dest_addr;
    REG32(DMA_BASE+CHN1_BASE+CHN_SIZE*(ch-1)+FULL_CHN_FRAG_LEN) = (data_size<<30)|(data_size<<28)|size|(fix<<20);
    REG32(DMA_BASE+CHN1_BASE+CHN_SIZE*(ch-1)+FULL_CHN_BLK_LEN) = size;
    REG32(DMA_BASE+CHN1_BASE+CHN_SIZE*(ch-1)+FULL_CHN_TRSC_LEN) = size;

    int step = 1;
    int i = data_size;
    while(i--)
        step *=2;
    REG32(DMA_BASE+CHN1_BASE+CHN_SIZE*(ch-1)+FULL_CHN_TRSF_STEP) = step|(step<<16);

    REG32(DMA_BASE+CHN1_BASE+CHN_SIZE*(ch-1)+FULL_CHN_CFG) |= BIT_0;//enable
}


//size max=256M-1  byte
//data_size  0  1  2
void dma_full_chn_config_transaction(int ch,uint32_t src_addr,
                    uint32_t dest_addr,int size,int trans_size,fix_sel fix,int data_size)
{
    REG32(DMA_BASE+CHN1_BASE+CHN_SIZE*(ch-1)+FULL_CHN_SRC_ADDR ) = src_addr;
    REG32(DMA_BASE+CHN1_BASE+CHN_SIZE*(ch-1)+FULL_CHN_DEST_ADDR ) = dest_addr;
    REG32(DMA_BASE+CHN1_BASE+CHN_SIZE*(ch-1)+FULL_CHN_FRAG_LEN) = (data_size<<30)|(data_size<<28)|size|(fix<<20)|(2<<24);
    REG32(DMA_BASE+CHN1_BASE+CHN_SIZE*(ch-1)+FULL_CHN_BLK_LEN) = size;
    REG32(DMA_BASE+CHN1_BASE+CHN_SIZE*(ch-1)+FULL_CHN_TRSC_LEN) = trans_size;

    int step = 1;
    int i = data_size;
    while(i--)
        step *=2;
    REG32(DMA_BASE+CHN1_BASE+CHN_SIZE*(ch-1)+FULL_CHN_TRSF_STEP) = step|(step<<16);

    REG32(DMA_BASE+CHN1_BASE+CHN_SIZE*(ch-1)+FULL_CHN_CFG) |= BIT_0;//enable
}

void dma_full_chn_sw_req(int ch)
{
    REG32(DMA_BASE+CHN1_BASE+CHN_SIZE*(ch-1)+FULL_CHN_REQ) |= BIT_0;//req
}



int hw_dma_chn_request(int reques_id)
{
    int i = 0;
    for(i=0;i<32;i++)
    {
        if(!(dma_chn_state>>i)&BIT_0)
        {
            dma_chn_state |= (BIT_0<<i);
            REG32(DMA_BASE+CID_BASE+reques_id*4) = i+1;
            return (i+1);
        }
    }
    
    return -1;
}

void hw_dma_chn_release(int reques_id)
{
    dma_chn_state &= ~(BIT_0<< (REG32(DMA_BASE+CID_BASE+reques_id*4)-1));
    REG32(DMA_BASE+CID_BASE+reques_id*4) = 0;
}


