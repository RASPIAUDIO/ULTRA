#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <stdint.h>
#include <stdio.h>

int file_i2c; 
char *filename = "/dev/i2c-1";
int addr = 0x1A;
int init_i2c(void)
{
	if((file_i2c=open(filename,O_RDWR)) < 0) return -1;
	if(ioctl(file_i2c, I2C_SLAVE, addr) < 0) return -2;
	return 0;
}
int write_i2c(uint8_t r, uint16_t v)
{
        printf(" %03x => R%02d\n",v,r);
	uint8_t b[2];
	b[0] = ((v&0x0100)>>8) | (r<<1);
	b[1] = (uint8_t)(v&0xff);

	if(write(file_i2c, b, 2) != 2) 
	{
	    printf("i2c error...\n");
	    return -3;
	}    
	return 0;
}
void main()
{
        usleep(200000);
	int st;
	st = init_i2c();
	if(st != 0) printf("Error # %d\n",st); 
        else printf("init i2c OK\n");
	//reset
	st += write_i2c(15, 0x00);
	usleep(10000);
	//Power
	st += write_i2c(25, 0x1FC);
	usleep(500);	
	st += write_i2c(26, 0x1F9);
	usleep(500);	
	st += write_i2c(47, 0x03C);
	usleep(10000);	
	//Clock PLL
	st += write_i2c(4, 0x001);
	usleep(10000);	
	st += write_i2c(52, 0x027);
	usleep(10000);	
	st += write_i2c(53, 0x086);
	usleep(10000);	
	st += write_i2c(54, 0x0C2);
	usleep(10000);	
	st += write_i2c(55, 0x026);
	usleep(10000);
	//ADC/DAC
	st += write_i2c(5, 0x000);
	usleep(10000);	
	st += write_i2c(7, 0x002);
	usleep(10000);	
	//ALC and Noise control
	
	st += write_i2c(20, 0x0F9);
	usleep(10000);
        st += write_i2c(17, 0x1FB);
        usleep(10000);  
        st += write_i2c(18, 0x000);
        usleep(10000);  
        st += write_i2c(19, 0x032);
        usleep(10000);  

	
	//OUT1 volume
	st += write_i2c(2, 0x16F);
	usleep(500);	
	st += write_i2c(3, 0x16F);
	usleep(500);	
	//SPK volume
	st += write_i2c(40, 0x17F);
	usleep(500);
	st += write_i2c(41, 0x178);
	usleep(500);
        st += write_i2c(51, 0x08D);
	// input volume
	st += write_i2c(0, 0x13F);
	usleep(10000);	
	st += write_i2c(1, 0x13F);
	usleep(10000);
	// INPUTS
	st += write_i2c(32, 0x138);
	usleep(10000);	
	st += write_i2c(33, 0x138);
	usleep(10000);	
	// OUTPUTS
	st += write_i2c(49, 0x0F7);
	usleep(10000);	
	st += write_i2c(10, 0x1FF);
	usleep(10000);
	st += write_i2c(11, 0x1FF);
	usleep(10000);

	st += write_i2c(34, 0x100);
	usleep(10000);
	
 	st += write_i2c(37, 0x100);
	usleep(10000);

    
    if(st != 0) printf("Errors while initializing...\n");          
    else printf("Done...\n");
    close(file_i2c);
}	
	


