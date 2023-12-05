#ifndef NETSOCKET_STATE_BUF_SIZE
#define NETSOCKET_STATE_BUF_SIZE (1024)
#endif

#ifndef NETSOCKET_HANDSHAKE_FUNC
#define NETSOCKET_HANDSHAKE_FUNC
void netsocket_handshake_func(uint16* digit_handshake, uint16* digit_handshake_out)
{
	digit_handshake_out[0] = (digit_handshake[0] << 2);
	digit_handshake_out[1] = (digit_handshake[1] >> 3) + (digit_handshake_out[0] << 5);
	digit_handshake_out[2] = (digit_handshake[2] << 5) + (digit_handshake_out[1] >> 7);
	digit_handshake_out[3] = (digit_handshake[3] >> 2) + (digit_handshake_out[2] << 4);
	digit_handshake_out[0] = (digit_handshake_out[0] >> 2) + (digit_handshake_out[3] << 4);
}
#endif

bool netsocket_handshake_cmp(uint16* digit_handshake, uint16* digit_handshake_cmp)
{
	if(digit_handshake[0] != digit_handshake_cmp[0]) false;
	if(digit_handshake[1] != digit_handshake_cmp[1]) false;
	if(digit_handshake[2] != digit_handshake_cmp[2]) false;
	if(digit_handshake[3] != digit_handshake_cmp[3]) false;

	return true;
}
