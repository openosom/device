/* dnw2 linux main file. This depends on libusb.
 *
 * Author: 	Fox <hulifox008@163.com>
 * 		Richad Nee <richard.nee.cn@gmail.com>
 * License:	GPL
 *
 * TODO:	1. Please install libusb and libusb-dev before building.
 *		2. Use 'lsusb' to find out the actual vender ID & product ID of board,
 *		   and add them in the 'struct usb_id list_usb_id[] = {' .
 *
 */



#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <usb.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>

#if 0
#define QQ2440_VENDOR_ID	0x5345
#define QQ2440_PRODUCT_ID	0x1234

#define FS2410_VENDOR_ID	0x5345
#define FS2410_PRODUCT_ID	0x1234

#define EZ6410_VENDOR_ID	0x5345
#define EZ6410_PRODUCT_ID	0x1234

#define REAL6410_VENDOR_ID	0x04e8
#define REAL6410_PRODUCT_ID	0x1234
#endif

#define S3C6410_RAM_BASE	0x50200000   
#define S3C2410_RAM_BASE	0x30200000
#define DEFAULT_RAM_BASE	S3C2410_RAM_BASE


struct usb_id {
	int idVendor;
	int idProduct;
};

struct usb_id list_usb_id[] = {
	[0] = {
		.idVendor	= 0x5345,	// for fs2410/mini2440/qq2440/mini6410/ez6410 ...
		.idProduct	= 0x1234,
	},
	[1] = {
		.idVendor	= 0x04e8,	// for real6410
		.idProduct	= 0x1234,
	}
};


struct usb_dev_handle * open_port()
{
	struct usb_bus *busses, *bus;

	usb_init();
	usb_find_busses();
	usb_find_devices();

	busses = usb_get_busses();


// scan the list_usb_id, to find the device
    int k;
    for ( k=0; k < sizeof(list_usb_id)/sizeof(struct usb_id) ; k++) {

	for(bus=busses;bus;bus=bus->next)
	{
	 	struct usb_device *dev;
		for(dev=bus->devices;dev;dev=dev->next)
		{
			if( list_usb_id[k].idVendor  == dev->descriptor.idVendor
			&&  list_usb_id[k].idProduct == dev->descriptor.idProduct)
			{
				printf("Target usb device found!\n");
				struct usb_dev_handle *hdev = usb_open(dev);
				if(!hdev) 
				{
					perror("Cannot open device");	
				}
				else
				{
					if(0!=usb_claim_interface(hdev, 0))
					{
						perror("Cannot claim interface");
						usb_close(hdev);
						hdev = NULL;
					}
				}
				return hdev;
			}
		}
	}

    }
	
	printf("Target usb device not found!\n");

	return NULL;
}

void usage()
{
	printf("Usage: dnw2 [options] file [address]\n\n");
	printf("Options:\n");
	printf("  -h, --help - This help text.\n\n");
	printf("address - hexadecimal notation, e.g. 0x30200000, default is 0x30200000\n");
}

// calculate checksum
static u_int16_t ace_csum(const unsigned char *data, u_int32_t len)
{
	u_int16_t csum = 0;
	int j;

	for (j = 0; j < len; j ++) {
		csum += data[j];
	}

	return csum;
}

unsigned char* prepare_write_buf(char *filename, unsigned int *len, long dl_addr)
{
	unsigned char *write_buf = NULL;
	struct stat fs;

	int fd = open(filename, O_RDONLY);
	if(-1==fd)
	{
		perror("Cannot open file");
		return NULL;
	}
	if(-1==fstat(fd, &fs))
	{
		perror("Cannot get file size");
		goto error;
	}
	write_buf = (unsigned char*)malloc(fs.st_size+10);
	if(NULL==write_buf)
	{
		perror("malloc failed");
		goto error;
	}

	if(fs.st_size != read(fd, write_buf+8, fs.st_size))
	{
		perror("Reading file failed");
		goto error;
	}

	printf("Filename : %s\n", filename);
	printf("Filesize : %lu bytes\n", fs.st_size);

	*((u_int32_t*)write_buf) = dl_addr;		//download address
	*((u_int32_t*)write_buf+1) = fs.st_size + 10;	//download size;

	*len = fs.st_size + 10;

// calculate checksum value
	u_int16_t csum = ace_csum(write_buf+8, fs.st_size);
	*(write_buf+fs.st_size+8) = csum & 0xff;
	*(write_buf+fs.st_size+9) = (csum >> 8) & 0xff;

	return write_buf;

error:
	if(fd!=-1) close(fd);
	if(NULL!=write_buf) free(write_buf);
	fs.st_size = 0;
	return NULL;
	
}

int main(int argc, char *argv[])
{
	long dl_addr;

	if (1 == argc || argc > 3){
		usage();
		return 1;
	}else if (3 == argc){
		dl_addr = strtol(argv[2], (char**) NULL, 16);
	}else{
		if (!strcmp(argv[1], "-h") || !strcmp(argv[1], "--help")) {
			usage();
			return 0;
		}

		dl_addr = DEFAULT_RAM_BASE;
	}


	struct usb_dev_handle *hdev = open_port();
	if(!hdev)
	{
		return 1;
	}

	unsigned int len = 0;
	unsigned char* write_buf = prepare_write_buf(argv[1], &len, dl_addr);
	if(NULL==write_buf) return 1;

	unsigned int remain = len;
	unsigned int towrite;

	printf("Writing data ...\n");
	while(remain)
	{
		towrite = remain>512 ? 512 : remain; 

//TODO: ENDPOINT= s3c2440/2410 =>0x03, s3c6410 => 0x02
		if(towrite != usb_bulk_write(hdev, 0x03, write_buf+(len-remain), towrite, 3000)) {
			if(towrite != usb_bulk_write(hdev, 0x02, write_buf+(len-remain), towrite, 3000)) {
				perror("usb_bulk_write failed");
				break;
			}
		}
		remain-=towrite;
		printf("\r%d%%\t %d bytes     ", (len-remain)*100/len, len-remain);
		fflush(stdout);
	}

	if(0==remain) {
		printf("Done!\n");
		usb_release_interface(hdev, 0);
	}

	free(write_buf);
	usb_close(hdev);

	return 0;
}
