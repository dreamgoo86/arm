

void sprd_spinlock_init()
{
	REG32(0x402e0000) |=BIT_22;
}


int sprd_spinlock_get(unsigned int num)
{
	if(num>31) return -1;
	return REG32(0x40060800+num*4);
}

int sprd_spinlock_release(unsigned int num)
{
	if(num>31) return -1;
	
	REG32(0x40060800+num*4) = 0x55aa10c5;
	
	return 0;
}