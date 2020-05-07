#ifndef LIBSG_H
#define LIBSG_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>

#include <linux/types.h>

#include <pthread.h>
#include <time.h>
#include <sys/time.h>
#include <sys/select.h>
#include <sys/stat.h>
#include <string.h>
#include <fcntl.h>
#include <signal.h>

#include <sys/ioctl.h>

#include <pthread.h>
#include <sys/types.h>
#include <termios.h>

#include <sys/statfs.h>
#include <sys/vfs.h>
#include <sys/sysinfo.h>
#include <errno.h>

#include <time.h>

#include <sys/socket.h>
#include <linux/if_ether.h>
//#include <linux/in.h>


#define TARGET	"Lib Sgitg"

#if 1
#define NLDBG
#endif
#ifdef NLDBG
#define DBGP(format, ...)	printf (format, ##__VA_ARGS__)
#else
#define DBGP(format, ...)	
#endif

/* Err type */
enum errtype {
	ERR_NOOPT	= -99,
	ERR_SHCMD	= -98,

	/* ---  wwan_errs  --- */
	/* GPIO */
	ERR_UNKWWAN	= -13,
	ERR_WWAN	= -22,
	ERR_USBID	= -21,
	ERR_WWANPW	= -20,
	ERR_WWANRST	= -19,
	ERR_WWANONF	= -18,
	/* USB */
	ERR_NOWWAN	= -17,
	/* TTY */
	ERR_WWANTTY	= -16,
	/* Operations */
	ERR_OPERATION	= -15,
	ERR_DIAL	= -14,
	ERR_DISCONN	= -13,


	ERR_NULLVAL	= -6,
	ERR_GPIO	= -5,
	ERR_THREAD	= -4,
	ERR_FORK	= -3,
	ERR_IDX		= -2,

	ERR_START	= -1,
	SUCCESS = 0,
};

/************************************************
 *
 *	sggpio 
 *
 ************************************************/
int gpio_sta(int gpio_idx);
int gpio_intrsta(int intr_idx);
int gpio_opt(int gpio_idx, uint16_t val);

#define DRVNAME		"gpio_sg"

#define GPIODRV		"gpio_sg"

#define	TGPIO_IOC_MAGIC		'T'

#define TGPIO_IOC_GETVAL	_IOR(TGPIO_IOC_MAGIC, 1, __u8)
#define TGPIO_IOC_GETSTA	_IOR(TGPIO_IOC_MAGIC, 2, __u8)
#define TGPIO_IOC_SETVAL	_IOW(TGPIO_IOC_MAGIC, 1, __u8)

#define GPIO_SYSNODE	"gpio_ttu"
#define GH	1
#define GL	0

enum tgpio_flagemu {
	ltepw		= 0,
	lterst		,
	lteonf		,
	ltepctrl	,
	ledrun		,
	ledrune		= 5,
	ledconn		,
	ledwwan		,
	uart1ms		,
	uart2ms		,
	mcu1rst		= 10,
	mcu2rst		,
	dogfeed		,
	dogen		,
	zbrst		,
	leduartm1	= 15,
	leduartm2	,
	gpiomax		= 17,
};


/* ------------------------------- */
/* GPIO interrupt */
/* ------------------------------- */

#define INTR_SYSNODE	"gpio_intr"

enum intr_idxenu {
	uart1msk	= 0,
	uart2msk	= 1,
	plcplg		,
	pressk1		,
	pressk2		,
	lteplg		= 5,
	pwfail		,
	intrmax		= 7,
};

struct intr_gpio_data {
	char intr_nm[16];
	int intr_idx;	
	int pio_hdle;	
	int state;
	int pre_state;
	unsigned int gpio_flag;
};

/************************************
 *
 * 	 sgutils 
 *
 *************************************/

/*
 * 从 strs 中找出 argv 的位置
 *
 */


typedef  struct statfs DISK,*pDISK;

typedef struct ifInfo {
	int idx;
	char ifName[16];
	char ifMac[18];
	char ifIP[16];
	char ifMsk[16];
	char ifBrd[16];
	char type[12];

} ifInfo_t;
int findidx(char* argv, char strs[][32], int max);
int seldly(int sec, int msec, int usec);

/* netutils */
int getLocalInfo(void);
int get_MAC(char *ethname, char *buf);

void ethdump_startCapture(uint8_t *alive, char *ifName);

/* ********************************
 *
 *	wwan
 *
 * *******************************/

#define TRUE 0
#define FALSE -1

#define	PCA_GPIO_IOC_MAGIC		'z'

#define PCA95_VENDOR_CODE	_IOR(PCA_GPIO_IOC_MAGIC, 1, unsigned char)
#define PCA95_VENEER_TYPE	_IOR(PCA_GPIO_IOC_MAGIC, 2, unsigned char) 
#define PCA95_VENEER_VERSION	_IOR(PCA_GPIO_IOC_MAGIC, 3, unsigned char) 

#define PCA95_IOCTL_OUT_High	_IOW(PCA_GPIO_IOC_MAGIC, 1, unsigned char)
#define PCA95_IOCTL_OUT_Low	_IOW(PCA_GPIO_IOC_MAGIC, 2, unsigned char)

#define MainReset	7
#define MainPower	8
#define WwanLed		10
#define	Lte_2G		11 	
#define	Lte_3G		12 	

#define PCA95_AcSoc_TYPE	_IOR(TGPIO_IOC_MAGIC, 4, __u8) 

#define MainResetAc	7
#define MainPowerAc	8

#define PPPCALL		"wwan_call"
#define PEERFILE	"/etc/ppp/peers/wwan_call"
#define PPPFILE		"/etc/ppp/wwan-connect-chat"

#define WWAN_ON		0x0
#define WWAN_OFF	0x1

#define WWAN_RSTPRS	0x0
#define WWAN_RSTREL	0x1

int wwan_gpio_check(void);
int cmd_init_huawei(int fd);
int cmd_init_neoway(int fd);

typedef struct wwan_data {
	int devidx;
	char usbID[10];
	char dbgtty[16];
	char dialtty[16];
	int  (*cmd_init)(int fd);
} wwan_data_t;

/* --- USB devices --- */
enum wwanDevTypeEnu {
	neoway		= 0,
	huawei		,
	wwandevIDmax	= 2,
};

/* --- AT cmds --- */
enum cmdidx {
	test_at		= 0,
	cmd_show	= 1,
	cmd_hide	,
	set_lang	,
	get_lang	,
	get_cops	= 5,
	get_ccid	,
	get_csqq	= 7,
	get_copsq	= 7,
	cmdmax		= 8,
};

/* --- operations --- */
typedef struct operation_data {
	int opidx;
	char apn[16];
	char atdt[16];
	char name[32];
} operation_t;

enum operationenu {
	cm	= 0,
	cu	,
	opmax	= 2,
};

typedef struct wwan_keepalive_data {
	wwan_data_t *wwan_dev;
	pthread_t pid_dial;
	pthread_t pid_keepalive;
	uint8_t pppdsta;
	uint8_t ppp0sta;
	uint8_t dialsta;
	uint8_t isalive;
} wwan_keepalive_t;

int wwan_restart(void);

#endif /* End of LIBSG_H */
