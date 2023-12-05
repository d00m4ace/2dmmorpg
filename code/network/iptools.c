////////////////////////////////////////////////////
#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#define _CRT_SECURE_NO_WARNINGS
#define _CRT_NONSTDC_NO_DEPRECATE
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <winsock2.h>
#include <ws2tcpip.h>
#include <mswsock.h>
#include <strsafe.h>

#include <iphlpapi.h>

#include <windows.h>
#pragma comment(lib, "IPHLPAPI.lib")
#endif
////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <assert.h>
#include <stdint.h>

#include "../config.h"

////////////////////////////////////////////////////
#ifdef USE_LINUX
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/ioctl.h>
#include <net/if.h>

#include <netdb.h>
#include <ifaddrs.h>
#endif
////////////////////////////////////////////////////

#include "../base/cstl.h"
#include "../base/types.h"
#include "../base/rnd.h"

#include "../threads/atomic.h"
#include "../threads/kernel.h"

#include "iptools.h"

#define MALLOC(x) malloc(x)
#define FREE(x) free(x)

static char g_host_name[256];
static char g_host_ip[256];

char* iptools_host_name()
{
	return g_host_name;
}

char* iptools_host_ip()
{
	return g_host_ip;
}

#if defined(_WIN32)
void iptools_init()
{
	//to find ip address on windows
	//start cmd
	//ipconfig
	// 
	// Get the local hostname & ip
	gethostname(g_host_name, 255);
	struct hostent* host_entry;
	host_entry = gethostbyname(g_host_name);
	//strcpy(g_host_ip, inet_ntoa(*(struct in_addr*)*host_entry->h_addr_list));

	{
		// It is possible for an adapter to have multiple
		// IPv4 addresses, gateways, and secondary WINS servers
		// assigned to the adapter. 
		//
		// Note that this sample code only prints out the 
		// first entry for the IP address/mask, and gateway, and
		// the primary and secondary WINS server for each adapter. 

		PIP_ADAPTER_INFO pAdapterInfo;
		PIP_ADAPTER_INFO pAdapter = NULL;
		DWORD dwRetVal = 0;

		ULONG ulOutBufLen = sizeof(IP_ADAPTER_INFO);
		pAdapterInfo = (IP_ADAPTER_INFO*)MALLOC(sizeof(IP_ADAPTER_INFO));
		if(pAdapterInfo == NULL)
		{
			printf("Error allocating memory needed to call GetAdaptersinfo\n");
			return;
		}
		// Make an initial call to GetAdaptersInfo to get
		// the necessary size into the ulOutBufLen variable
		if(GetAdaptersInfo(pAdapterInfo, &ulOutBufLen) == ERROR_BUFFER_OVERFLOW)
		{
			FREE(pAdapterInfo);
			pAdapterInfo = (IP_ADAPTER_INFO*)MALLOC(ulOutBufLen);
			if(pAdapterInfo == NULL)
			{
				printf("Error allocating memory needed to call GetAdaptersinfo\n");
				return;
			}
		}

		if((dwRetVal = GetAdaptersInfo(pAdapterInfo, &ulOutBufLen)) == NO_ERROR)
		{
			pAdapter = pAdapterInfo;
			while(pAdapter)
			{
				if(!strncmp(pAdapter->GatewayList.IpAddress.String, "0.0.0.0", 7))
				{
					pAdapter = pAdapter->Next;
					continue;
				}

				strcpy(g_host_ip, pAdapter->IpAddressList.IpAddress.String);

/*
				g_host_phys_addr = 0;
				uint8* host_phys_addr_bytes = &g_host_phys_addr;

				for(int i = 0; i < pAdapter->AddressLength && i < 8; i++)
					host_phys_addr_bytes[i] = pAdapter->Address[i];

				g_host_phys_addr ^= IPTOOLS_PHYS_ADDR_KEFIR;
*/

				pAdapter = pAdapter->Next;
			}
		}
		else
		{
			PRINT("GetAdaptersInfo failed with error: %d\n", dwRetVal);

		}

		if(pAdapterInfo)
			FREE(pAdapterInfo);
	}
}
#endif

#ifdef USE_LINUX

void iptools_init()
{
	struct ifaddrs* ifaddr, * ifa;
	int family, s;
	char host[NI_MAXHOST];

	if(getifaddrs(&ifaddr) == -1)
	{
		perror("getifaddrs");
		exit(EXIT_FAILURE);
	}

	for(ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next)
	{
		if(ifa->ifa_addr == NULL)
			continue;

		s = getnameinfo(ifa->ifa_addr, sizeof(struct sockaddr_in), host, NI_MAXHOST, NULL, 0, NI_NUMERICHOST);

		if(ifa->ifa_addr->sa_family == AF_INET) //&&(strcmp(ifa->ifa_name,"wlan0")==0) 
		{
			if(s != 0)
			{
				PRINT("getnameinfo() failed: %s", gai_strerror(s));
				exit(EXIT_FAILURE);
			}
			PRINT("\tInterface : <%s>", ifa->ifa_name);
			PRINT("\t  Address : <%s>", host);

			if(strcmp(host, "127.0.0.1"))
			{
				sprintf(g_host_ip, "%s", host);
				sprintf(g_host_name, "%s", ifa->ifa_name);
			}
		}
	}

	freeifaddrs(ifaddr);
}

/*
void iptools_init()
{
	// Read out "hostname -I" command output
	FILE* fd = popen("hostname -I", "r");
	if(fd == NULL)
	{
		PRINT("iptools_init: Could not open pipe.");
		return;
	}

#define IP_BUFFER_LEN 256

	// Put output into a string (static memory)
	static char buffer[IP_BUFFER_LEN];
	fgets(buffer, IP_BUFFER_LEN, fd);

	PRINT("IP_BUFFER:[%s]", buffer);

	// Only keep the first ip.
	for(int i = 0; i < IP_BUFFER_LEN; ++i)
	{
		if(buffer[i] == ' ')
		{
			buffer[i] = '\0';
			break;
		}
	}

	char* ret = malloc(strlen(buffer) + 1);
	memcpy(ret, buffer, strlen(buffer));
	ret[strlen(buffer)] = '\0';
	PRINT("iptools_init:[%s]", ret);

	sprintf(g_host_ip, "%s", ret);
	sprintf(g_host_name, "%s", "LINUX");
}
*/

/*
void iptools_init()
{
	int fd;
	struct ifreq ifr;

	fd = socket(AF_INET, SOCK_DGRAM, 0);

	// I want to get an IPv4 IP address
	ifr.ifr_addr.sa_family = AF_INET;

	// I want IP address attached to "eth0"
	strncpy(ifr.ifr_name, "eth0", IFNAMSIZ - 1);

	ioctl(fd, SIOCGIFADDR, &ifr);

	close(fd);

	sprintf(g_host_ip, "%s", inet_ntoa(((struct sockaddr_in*)&ifr.ifr_addr)->sin_addr));
	sprintf(g_host_name, "%s", "LINUX");
}
*/
#endif // USE_LINUX

/*
static uint64 g_host_phys_addr;

uint64 iptools_host_phys_addr()
{
	return g_host_phys_addr;
}

char* iptools_phys_addr_to_str(uint64 phys_addr)
{
	static char p8_str[3 * 8 + 1];
	uint8* p8 = &phys_addr;

	for(int i = 0; i < 8; i++)
		sprintf(p8_str + i * 3, "%.2X:", p8[i]);

	p8_str[8 * 3 - 1] = 0;

	return p8_str;
}
*/
