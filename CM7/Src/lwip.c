#include "lwip.h"
#include "lwip/init.h"
#include "lwip/netif.h"
#if defined ( __CC_ARM )  /* MDK ARM Compiler */
#include "lwip/sio.h"
#endif /* MDK ARM Compiler */
#include "ethernetif.h"


static void ethernet_link_status_updated(struct netif *netif);
static void Ethernet_Link_Periodic_Handle(struct netif *netif);
void Error_Handler(void);


uint32_t EthernetLinkTimer;

struct netif gnetif;
ip4_addr_t ipaddr;
ip4_addr_t netmask;
ip4_addr_t gw;
uint8_t IP_ADDRESS[4];
uint8_t NETMASK_ADDRESS[4];
uint8_t GATEWAY_ADDRESS[4];


/**
  * LwIP initialization function
  */
void MX_LWIP_Init(void)
{
	/* IP addresses initialization */
	IP_ADDRESS[0] = IP_ADDR0;
	IP_ADDRESS[1] = IP_ADDR1;
	IP_ADDRESS[2] = IP_ADDR2;
	IP_ADDRESS[3] = IP_ADDR3;

	NETMASK_ADDRESS[0] = NETMASK_ADDR0;
	NETMASK_ADDRESS[1] = NETMASK_ADDR1;
	NETMASK_ADDRESS[2] = NETMASK_ADDR2;
	NETMASK_ADDRESS[3] = NETMASK_ADDR3;

	GATEWAY_ADDRESS[0] = GW_ADDR0;
	GATEWAY_ADDRESS[1] = GW_ADDR1;
	GATEWAY_ADDRESS[2] = GW_ADDR2;
	GATEWAY_ADDRESS[3] = GW_ADDR3;

	/* Initilialize the LwIP stack without RTOS */
	lwip_init();

	/* IP addresses initialization without DHCP (IPv4) */
	IP4_ADDR(&ipaddr, IP_ADDRESS[0], IP_ADDRESS[1], IP_ADDRESS[2], IP_ADDRESS[3]);
	IP4_ADDR(&netmask, NETMASK_ADDRESS[0], NETMASK_ADDRESS[1] , NETMASK_ADDRESS[2], NETMASK_ADDRESS[3]);
	IP4_ADDR(&gw, GATEWAY_ADDRESS[0], GATEWAY_ADDRESS[1], GATEWAY_ADDRESS[2], GATEWAY_ADDRESS[3]);

	/* add the network interface (IPv4/IPv6) without RTOS */
	netif_add(&gnetif, &ipaddr, &netmask, &gw, NULL, &ethernetif_init, &ethernet_input);

	/* Registers the default network interface */
	netif_set_default(&gnetif);

	if(netif_is_link_up(&gnetif))
	{
		/* When the netif is fully configured this function must be called */
		netif_set_up(&gnetif);
	}
	else
	{
		/* When the netif link is down this function must be called */
		netif_set_down(&gnetif);
	}

	/* Set the link callback function, this function is called on change of link status*/
	netif_set_link_callback(&gnetif, ethernet_link_status_updated);

}

#ifdef USE_OBSOLETE_USER_CODE_SECTION_4
/* Kept to help code migration. (See new 4_1, 4_2... sections) */
/* Avoid to use this user section which will become obsolete. */
/* USER CODE BEGIN 4 */
/* USER CODE END 4 */
#endif

/**
  * @brief  Ethernet Link periodic check
  * @param  netif 
  * @retval None
  */
static void Ethernet_Link_Periodic_Handle(struct netif *netif)
{
	/* Ethernet Link every 100ms */
	if (HAL_GetTick() - EthernetLinkTimer >= 100)
	{
		EthernetLinkTimer = HAL_GetTick();
		ethernet_link_check_state(netif);
	}
}

/**
 * ----------------------------------------------------------------------
 * Function given to help user to continue LwIP Initialization
 * Up to user to complete or change this function ...
 * Up to user to call this function in main.c in while (1) of main(void) 
 *-----------------------------------------------------------------------
 * Read a received packet from the Ethernet buffers 
 * Send it to the lwIP stack for handling
 * Handle timeouts if LWIP_TIMERS is set and without RTOS
 * Handle the llink status if LWIP_NETIF_LINK_CALLBACK is set and without RTOS 
 */
void MX_LWIP_Process(void)
{
	ethernetif_input(&gnetif);
	sys_check_timeouts();
	Ethernet_Link_Periodic_Handle(&gnetif);
}

/**
  * @brief  Notify the User about the network interface config status 
  * @param  netif: the network interface
  * @retval None
  */
static void ethernet_link_status_updated(struct netif *netif) 
{
	if(netif_is_up(netif))
	{

	}
	else /* netif is down */
	{

	}
}

#if defined ( __CC_ARM )  /* MDK ARM Compiler */
/**
 * Opens a serial device for communication.
 *
 * @param devnum device number
 * @return handle to serial device if successful, NULL otherwise
 */
sio_fd_t sio_open(u8_t devnum)
{
	sio_fd_t sd;

	sd = 0; // dummy code
	
	return sd;
}

/**
 * Sends a single character to the serial device.
 *
 * @param c character to send
 * @param fd serial device handle
 *
 * @note This function will block until the character can be sent.
 */
void sio_send(u8_t c, sio_fd_t fd)
{

}

/**
 * Reads from the serial device.
 *
 * @param fd serial device handle
 * @param data pointer to data buffer for receiving
 * @param len maximum length (in bytes) of data to receive
 * @return number of bytes actually received - may be 0 if aborted by sio_read_abort
 *
 * @note This function will block until data can be received. The blocking
 * can be cancelled by calling sio_read_abort().
 */
u32_t sio_read(sio_fd_t fd, u8_t *data, u32_t len)
{
	u32_t recved_bytes;

	recved_bytes = 0; // dummy code

	return recved_bytes;
}

/**
 * Tries to read from the serial device. Same as sio_read but returns
 * immediately if no data is available and never blocks.
 *
 * @param fd serial device handle
 * @param data pointer to data buffer for receiving
 * @param len maximum length (in bytes) of data to receive
 * @return number of bytes actually received
 */
u32_t sio_tryread(sio_fd_t fd, u8_t *data, u32_t len)
{
	u32_t recved_bytes;

	recved_bytes = 0; // dummy code

	return recved_bytes;
}
#endif
