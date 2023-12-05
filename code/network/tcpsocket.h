bool tcpsocket_recv(NETSOCKET_STATE* socket_state, uint16 max_recv);
bool tcpsocket_send(NETSOCKET_STATE* socket_state, uint16 max_send);

uint16 tcpsocket_recv_packet_size(NETSOCKET_STATE* socket_state);
uint16 tcpsocket_send_packet_size(NETSOCKET_STATE* socket_state);

void tcpsocket_closesocket(NETSOCKET_STATE* socket_state);

void tcpsocket_init();
void tcpsocket_free();

#ifdef USE_LINUX

#define SOCKET_ERROR (-1)
#define INVALID_SOCKET (-1)

#define closesocket(network_socket) close(network_socket)
#endif
