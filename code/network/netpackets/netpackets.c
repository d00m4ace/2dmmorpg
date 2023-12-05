bool NETPACKET_CLIENT_HI_WRITE(NETPACKET_BLOB* dst, NETPACKET_CLIENT_HI* pack)
{
	uint16 packet_size = NETPACKET_CLIENT_HI_SIZE(), packet_id = NETPACKET_CLIENT_HI_ID;
	if(!netpacket_write_uint16(dst, &packet_size)) return false;
	if(!netpacket_write_uint16(dst, &packet_id)) return false;
	if(!netpacket_write_array_uint8(dst, &pack->clnt_vers,sizeof(pack->clnt_vers))) return false;
	if(!netpacket_write_array_uint8(dst, &pack->clnt_platform_id,sizeof(pack->clnt_platform_id))) return false;
	if(!netpacket_write_array_uint8(dst, &pack->clnt_send_limit,sizeof(pack->clnt_send_limit))) return false;
	if(!netpacket_write_array_uint8(dst, &pack->clnt_job_id,sizeof(pack->clnt_job_id))) return false;
	return true;
}

bool NETPACKET_CLIENT_HANDSHAKE_WRITE(NETPACKET_BLOB* dst, NETPACKET_CLIENT_HANDSHAKE* pack)
{
	uint16 packet_size = NETPACKET_CLIENT_HANDSHAKE_SIZE(), packet_id = NETPACKET_CLIENT_HANDSHAKE_ID;
	if(!netpacket_write_uint16(dst, &packet_size)) return false;
	if(!netpacket_write_uint16(dst, &packet_id)) return false;
	if(!netpacket_write_array_uint8(dst, &pack->clnt_handshake,sizeof(pack->clnt_handshake))) return false;
	return true;
}

bool NETPACKET_SERVER_HI_WRITE(NETPACKET_BLOB* dst, NETPACKET_SERVER_HI* pack)
{
	uint16 packet_size = NETPACKET_SERVER_HI_SIZE(), packet_id = NETPACKET_SERVER_HI_ID;
	if(!netpacket_write_uint16(dst, &packet_size)) return false;
	if(!netpacket_write_uint16(dst, &packet_id)) return false;
	if(!netpacket_write_array_uint8(dst, &pack->srvr_vers,sizeof(pack->srvr_vers))) return false;
	if(!netpacket_write_array_uint8(dst, &pack->srvr_platform_id,sizeof(pack->srvr_platform_id))) return false;
	if(!netpacket_write_array_uint8(dst, &pack->srvr_send_limit,sizeof(pack->srvr_send_limit))) return false;
	if(!netpacket_write_array_uint8(dst, &pack->srvr_handshake,sizeof(pack->srvr_handshake))) return false;
	return true;
}

bool NETPACKET_SERVER_WELCOME_WRITE(NETPACKET_BLOB* dst, NETPACKET_SERVER_WELCOME* pack)
{
	uint16 packet_size = NETPACKET_SERVER_WELCOME_SIZE(), packet_id = NETPACKET_SERVER_WELCOME_ID;
	if(!netpacket_write_uint16(dst, &packet_size)) return false;
	if(!netpacket_write_uint16(dst, &packet_id)) return false;
	return true;
}

bool NETPACKET_CLIENT_ECHO_WRITE(NETPACKET_BLOB* dst, NETPACKET_CLIENT_ECHO* pack)
{
	uint16 packet_size = NETPACKET_CLIENT_ECHO_SIZE(), packet_id = NETPACKET_CLIENT_ECHO_ID;
	if(!netpacket_write_uint16(dst, &packet_size)) return false;
	if(!netpacket_write_uint16(dst, &packet_id)) return false;
	if(!netpacket_write_array_uint8(dst, &pack->echo_data,sizeof(pack->echo_data))) return false;
	return true;
}

bool NETPACKET_SERVER_ECHO_WRITE(NETPACKET_BLOB* dst, NETPACKET_SERVER_ECHO* pack)
{
	uint16 packet_size = NETPACKET_SERVER_ECHO_SIZE(), packet_id = NETPACKET_SERVER_ECHO_ID;
	if(!netpacket_write_uint16(dst, &packet_size)) return false;
	if(!netpacket_write_uint16(dst, &packet_id)) return false;
	if(!netpacket_write_array_uint8(dst, &pack->echo_data,sizeof(pack->echo_data))) return false;
	return true;
}

bool NETPACKET_GET_FILE_INFO_WRITE(NETPACKET_BLOB* dst, NETPACKET_GET_FILE_INFO* pack)
{
	uint16 packet_size = NETPACKET_GET_FILE_INFO_SIZE(), packet_id = NETPACKET_GET_FILE_INFO_ID;
	if(!netpacket_write_uint16(dst, &packet_size)) return false;
	if(!netpacket_write_uint16(dst, &packet_id)) return false;
	if(!netpacket_write_array_uint8(dst, &pack->file_name,sizeof(pack->file_name))) return false;
	return true;
}

bool NETPACKET_FILE_INFO_WRITE(NETPACKET_BLOB* dst, NETPACKET_FILE_INFO* pack)
{
	uint16 packet_size = NETPACKET_FILE_INFO_SIZE(), packet_id = NETPACKET_FILE_INFO_ID;
	if(!netpacket_write_uint16(dst, &packet_size)) return false;
	if(!netpacket_write_uint16(dst, &packet_id)) return false;
	if(!netpacket_write_array_uint8(dst, &pack->file_name,sizeof(pack->file_name))) return false;
	if(!netpacket_write_array_uint8(dst, &pack->modified_timestamp,sizeof(pack->modified_timestamp))) return false;
	return true;
}

bool NETPACKET_DOWNLOAD_FILE_WRITE(NETPACKET_BLOB* dst, NETPACKET_DOWNLOAD_FILE* pack)
{
	uint16 packet_size = NETPACKET_DOWNLOAD_FILE_SIZE(), packet_id = NETPACKET_DOWNLOAD_FILE_ID;
	if(!netpacket_write_uint16(dst, &packet_size)) return false;
	if(!netpacket_write_uint16(dst, &packet_id)) return false;
	if(!netpacket_write_array_uint8(dst, &pack->file_name,sizeof(pack->file_name))) return false;
	return true;
}

bool NETPACKET_ON_INFO_WRITE(NETPACKET_BLOB* dst, NETPACKET_ON_INFO* pack)
{
	uint16 packet_size = NETPACKET_ON_INFO_SIZE(), packet_id = NETPACKET_ON_INFO_ID;
	if(!netpacket_write_uint16(dst, &packet_size)) return false;
	if(!netpacket_write_uint16(dst, &packet_id)) return false;
	return true;
}

bool NETPACKET_ON_IDLE_WRITE(NETPACKET_BLOB* dst, NETPACKET_ON_IDLE* pack)
{
	uint16 packet_size = NETPACKET_ON_IDLE_SIZE(), packet_id = NETPACKET_ON_IDLE_ID;
	if(!netpacket_write_uint16(dst, &packet_size)) return false;
	if(!netpacket_write_uint16(dst, &packet_id)) return false;
	return true;
}

bool NETPACKET_ON_UPDATE_WRITE(NETPACKET_BLOB* dst, NETPACKET_ON_UPDATE* pack)
{
	uint16 packet_size = NETPACKET_ON_UPDATE_SIZE(), packet_id = NETPACKET_ON_UPDATE_ID;
	if(!netpacket_write_uint16(dst, &packet_size)) return false;
	if(!netpacket_write_uint16(dst, &packet_id)) return false;
	return true;
}

bool NETPACKET_ON_CONTINUE_WRITE(NETPACKET_BLOB* dst, NETPACKET_ON_CONTINUE* pack)
{
	uint16 packet_size = NETPACKET_ON_CONTINUE_SIZE(), packet_id = NETPACKET_ON_CONTINUE_ID;
	if(!netpacket_write_uint16(dst, &packet_size)) return false;
	if(!netpacket_write_uint16(dst, &packet_id)) return false;
	return true;
}

bool NETPACKET_ON_COMPLETE_WRITE(NETPACKET_BLOB* dst, NETPACKET_ON_COMPLETE* pack)
{
	uint16 packet_size = NETPACKET_ON_COMPLETE_SIZE(), packet_id = NETPACKET_ON_COMPLETE_ID;
	if(!netpacket_write_uint16(dst, &packet_size)) return false;
	if(!netpacket_write_uint16(dst, &packet_id)) return false;
	return true;
}

bool NETPACKET_DATASET_5000_WRITE(NETPACKET_BLOB* dst, NETPACKET_DATASET_5000* pack)
{
	uint16 packet_size = NETPACKET_DATASET_5000_SIZE(), packet_id = NETPACKET_DATASET_5000_ID;
	if(!netpacket_write_uint16(dst, &packet_size)) return false;
	if(!netpacket_write_uint16(dst, &packet_id)) return false;
	if(!netpacket_write_array_uint8(dst, &pack->data_size,sizeof(pack->data_size))) return false;
	if(!netpacket_write_array_uint8(dst, &pack->data,sizeof(pack->data))) return false;
	return true;
}

bool NETPACKET_DATASET_1000_WRITE(NETPACKET_BLOB* dst, NETPACKET_DATASET_1000* pack)
{
	uint16 packet_size = NETPACKET_DATASET_1000_SIZE(), packet_id = NETPACKET_DATASET_1000_ID;
	if(!netpacket_write_uint16(dst, &packet_size)) return false;
	if(!netpacket_write_uint16(dst, &packet_id)) return false;
	if(!netpacket_write_array_uint8(dst, &pack->data_size,sizeof(pack->data_size))) return false;
	if(!netpacket_write_array_uint8(dst, &pack->data,sizeof(pack->data))) return false;
	return true;
}

bool NETPACKET_DATASET_256_WRITE(NETPACKET_BLOB* dst, NETPACKET_DATASET_256* pack)
{
	uint16 packet_size = NETPACKET_DATASET_256_SIZE(), packet_id = NETPACKET_DATASET_256_ID;
	if(!netpacket_write_uint16(dst, &packet_size)) return false;
	if(!netpacket_write_uint16(dst, &packet_id)) return false;
	if(!netpacket_write_array_uint8(dst, &pack->data_size,sizeof(pack->data_size))) return false;
	if(!netpacket_write_array_uint8(dst, &pack->data,sizeof(pack->data))) return false;
	return true;
}

bool NETPACKET_DATASET_128_WRITE(NETPACKET_BLOB* dst, NETPACKET_DATASET_128* pack)
{
	uint16 packet_size = NETPACKET_DATASET_128_SIZE(), packet_id = NETPACKET_DATASET_128_ID;
	if(!netpacket_write_uint16(dst, &packet_size)) return false;
	if(!netpacket_write_uint16(dst, &packet_id)) return false;
	if(!netpacket_write_array_uint8(dst, &pack->data_size,sizeof(pack->data_size))) return false;
	if(!netpacket_write_array_uint8(dst, &pack->data,sizeof(pack->data))) return false;
	return true;
}

bool NETPACKET_DATASET_64_WRITE(NETPACKET_BLOB* dst, NETPACKET_DATASET_64* pack)
{
	uint16 packet_size = NETPACKET_DATASET_64_SIZE(), packet_id = NETPACKET_DATASET_64_ID;
	if(!netpacket_write_uint16(dst, &packet_size)) return false;
	if(!netpacket_write_uint16(dst, &packet_id)) return false;
	if(!netpacket_write_array_uint8(dst, &pack->data_size,sizeof(pack->data_size))) return false;
	if(!netpacket_write_array_uint8(dst, &pack->data,sizeof(pack->data))) return false;
	return true;
}

bool NETPACKET_DATASET_32_WRITE(NETPACKET_BLOB* dst, NETPACKET_DATASET_32* pack)
{
	uint16 packet_size = NETPACKET_DATASET_32_SIZE(), packet_id = NETPACKET_DATASET_32_ID;
	if(!netpacket_write_uint16(dst, &packet_size)) return false;
	if(!netpacket_write_uint16(dst, &packet_id)) return false;
	if(!netpacket_write_array_uint8(dst, &pack->data_size,sizeof(pack->data_size))) return false;
	if(!netpacket_write_array_uint8(dst, &pack->data,sizeof(pack->data))) return false;
	return true;
}

bool NETPACKET_DATASET_16_WRITE(NETPACKET_BLOB* dst, NETPACKET_DATASET_16* pack)
{
	uint16 packet_size = NETPACKET_DATASET_16_SIZE(), packet_id = NETPACKET_DATASET_16_ID;
	if(!netpacket_write_uint16(dst, &packet_size)) return false;
	if(!netpacket_write_uint16(dst, &packet_id)) return false;
	if(!netpacket_write_array_uint8(dst, &pack->data_size,sizeof(pack->data_size))) return false;
	if(!netpacket_write_array_uint8(dst, &pack->data,sizeof(pack->data))) return false;
	return true;
}

bool NETPACKET_CLIENT_HI_READ(NETPACKET_BLOB* dst, NETPACKET_CLIENT_HI* pack)
{
	uint16 packet_size, packet_id;
	uint32 dstpos = dst->pos;
	if(!netpacket_read_uint16(dst, &packet_size)) return false;
	if(!netpacket_read_uint16(dst, &packet_id)) return false;
	if(packet_id != NETPACKET_CLIENT_HI_ID) return false;
	if(packet_size > NETPACKET_CLIENT_HI_SIZE()) return false;
	if(!netpacket_read_array_uint8(dst, &pack->clnt_vers,sizeof(pack->clnt_vers))) return false;
	if(!netpacket_read_array_uint8(dst, &pack->clnt_platform_id,sizeof(pack->clnt_platform_id))) return false;
	if(!netpacket_read_array_uint8(dst, &pack->clnt_send_limit,sizeof(pack->clnt_send_limit))) return false;
	if(!netpacket_read_array_uint8(dst, &pack->clnt_job_id,sizeof(pack->clnt_job_id))) return false;
	dst->pos = dstpos + packet_size;
	return true;
}

bool NETPACKET_CLIENT_HANDSHAKE_READ(NETPACKET_BLOB* dst, NETPACKET_CLIENT_HANDSHAKE* pack)
{
	uint16 packet_size, packet_id;
	uint32 dstpos = dst->pos;
	if(!netpacket_read_uint16(dst, &packet_size)) return false;
	if(!netpacket_read_uint16(dst, &packet_id)) return false;
	if(packet_id != NETPACKET_CLIENT_HANDSHAKE_ID) return false;
	if(packet_size > NETPACKET_CLIENT_HANDSHAKE_SIZE()) return false;
	if(!netpacket_read_array_uint8(dst, &pack->clnt_handshake,sizeof(pack->clnt_handshake))) return false;
	dst->pos = dstpos + packet_size;
	return true;
}

bool NETPACKET_SERVER_HI_READ(NETPACKET_BLOB* dst, NETPACKET_SERVER_HI* pack)
{
	uint16 packet_size, packet_id;
	uint32 dstpos = dst->pos;
	if(!netpacket_read_uint16(dst, &packet_size)) return false;
	if(!netpacket_read_uint16(dst, &packet_id)) return false;
	if(packet_id != NETPACKET_SERVER_HI_ID) return false;
	if(packet_size > NETPACKET_SERVER_HI_SIZE()) return false;
	if(!netpacket_read_array_uint8(dst, &pack->srvr_vers,sizeof(pack->srvr_vers))) return false;
	if(!netpacket_read_array_uint8(dst, &pack->srvr_platform_id,sizeof(pack->srvr_platform_id))) return false;
	if(!netpacket_read_array_uint8(dst, &pack->srvr_send_limit,sizeof(pack->srvr_send_limit))) return false;
	if(!netpacket_read_array_uint8(dst, &pack->srvr_handshake,sizeof(pack->srvr_handshake))) return false;
	dst->pos = dstpos + packet_size;
	return true;
}

bool NETPACKET_SERVER_WELCOME_READ(NETPACKET_BLOB* dst, NETPACKET_SERVER_WELCOME* pack)
{
	uint16 packet_size, packet_id;
	uint32 dstpos = dst->pos;
	if(!netpacket_read_uint16(dst, &packet_size)) return false;
	if(!netpacket_read_uint16(dst, &packet_id)) return false;
	if(packet_id != NETPACKET_SERVER_WELCOME_ID) return false;
	if(packet_size > NETPACKET_SERVER_WELCOME_SIZE()) return false;
	dst->pos = dstpos + packet_size;
	return true;
}

bool NETPACKET_CLIENT_ECHO_READ(NETPACKET_BLOB* dst, NETPACKET_CLIENT_ECHO* pack)
{
	uint16 packet_size, packet_id;
	uint32 dstpos = dst->pos;
	if(!netpacket_read_uint16(dst, &packet_size)) return false;
	if(!netpacket_read_uint16(dst, &packet_id)) return false;
	if(packet_id != NETPACKET_CLIENT_ECHO_ID) return false;
	if(packet_size > NETPACKET_CLIENT_ECHO_SIZE()) return false;
	if(!netpacket_read_array_uint8(dst, &pack->echo_data,sizeof(pack->echo_data))) return false;
	dst->pos = dstpos + packet_size;
	return true;
}

bool NETPACKET_SERVER_ECHO_READ(NETPACKET_BLOB* dst, NETPACKET_SERVER_ECHO* pack)
{
	uint16 packet_size, packet_id;
	uint32 dstpos = dst->pos;
	if(!netpacket_read_uint16(dst, &packet_size)) return false;
	if(!netpacket_read_uint16(dst, &packet_id)) return false;
	if(packet_id != NETPACKET_SERVER_ECHO_ID) return false;
	if(packet_size > NETPACKET_SERVER_ECHO_SIZE()) return false;
	if(!netpacket_read_array_uint8(dst, &pack->echo_data,sizeof(pack->echo_data))) return false;
	dst->pos = dstpos + packet_size;
	return true;
}

bool NETPACKET_GET_FILE_INFO_READ(NETPACKET_BLOB* dst, NETPACKET_GET_FILE_INFO* pack)
{
	uint16 packet_size, packet_id;
	uint32 dstpos = dst->pos;
	if(!netpacket_read_uint16(dst, &packet_size)) return false;
	if(!netpacket_read_uint16(dst, &packet_id)) return false;
	if(packet_id != NETPACKET_GET_FILE_INFO_ID) return false;
	if(packet_size > NETPACKET_GET_FILE_INFO_SIZE()) return false;
	if(!netpacket_read_array_uint8(dst, &pack->file_name,sizeof(pack->file_name))) return false;
	dst->pos = dstpos + packet_size;
	return true;
}

bool NETPACKET_FILE_INFO_READ(NETPACKET_BLOB* dst, NETPACKET_FILE_INFO* pack)
{
	uint16 packet_size, packet_id;
	uint32 dstpos = dst->pos;
	if(!netpacket_read_uint16(dst, &packet_size)) return false;
	if(!netpacket_read_uint16(dst, &packet_id)) return false;
	if(packet_id != NETPACKET_FILE_INFO_ID) return false;
	if(packet_size > NETPACKET_FILE_INFO_SIZE()) return false;
	if(!netpacket_read_array_uint8(dst, &pack->file_name,sizeof(pack->file_name))) return false;
	if(!netpacket_read_array_uint8(dst, &pack->modified_timestamp,sizeof(pack->modified_timestamp))) return false;
	dst->pos = dstpos + packet_size;
	return true;
}

bool NETPACKET_DOWNLOAD_FILE_READ(NETPACKET_BLOB* dst, NETPACKET_DOWNLOAD_FILE* pack)
{
	uint16 packet_size, packet_id;
	uint32 dstpos = dst->pos;
	if(!netpacket_read_uint16(dst, &packet_size)) return false;
	if(!netpacket_read_uint16(dst, &packet_id)) return false;
	if(packet_id != NETPACKET_DOWNLOAD_FILE_ID) return false;
	if(packet_size > NETPACKET_DOWNLOAD_FILE_SIZE()) return false;
	if(!netpacket_read_array_uint8(dst, &pack->file_name,sizeof(pack->file_name))) return false;
	dst->pos = dstpos + packet_size;
	return true;
}

bool NETPACKET_ON_INFO_READ(NETPACKET_BLOB* dst, NETPACKET_ON_INFO* pack)
{
	uint16 packet_size, packet_id;
	uint32 dstpos = dst->pos;
	if(!netpacket_read_uint16(dst, &packet_size)) return false;
	if(!netpacket_read_uint16(dst, &packet_id)) return false;
	if(packet_id != NETPACKET_ON_INFO_ID) return false;
	if(packet_size > NETPACKET_ON_INFO_SIZE()) return false;
	dst->pos = dstpos + packet_size;
	return true;
}

bool NETPACKET_ON_IDLE_READ(NETPACKET_BLOB* dst, NETPACKET_ON_IDLE* pack)
{
	uint16 packet_size, packet_id;
	uint32 dstpos = dst->pos;
	if(!netpacket_read_uint16(dst, &packet_size)) return false;
	if(!netpacket_read_uint16(dst, &packet_id)) return false;
	if(packet_id != NETPACKET_ON_IDLE_ID) return false;
	if(packet_size > NETPACKET_ON_IDLE_SIZE()) return false;
	dst->pos = dstpos + packet_size;
	return true;
}

bool NETPACKET_ON_UPDATE_READ(NETPACKET_BLOB* dst, NETPACKET_ON_UPDATE* pack)
{
	uint16 packet_size, packet_id;
	uint32 dstpos = dst->pos;
	if(!netpacket_read_uint16(dst, &packet_size)) return false;
	if(!netpacket_read_uint16(dst, &packet_id)) return false;
	if(packet_id != NETPACKET_ON_UPDATE_ID) return false;
	if(packet_size > NETPACKET_ON_UPDATE_SIZE()) return false;
	dst->pos = dstpos + packet_size;
	return true;
}

bool NETPACKET_ON_CONTINUE_READ(NETPACKET_BLOB* dst, NETPACKET_ON_CONTINUE* pack)
{
	uint16 packet_size, packet_id;
	uint32 dstpos = dst->pos;
	if(!netpacket_read_uint16(dst, &packet_size)) return false;
	if(!netpacket_read_uint16(dst, &packet_id)) return false;
	if(packet_id != NETPACKET_ON_CONTINUE_ID) return false;
	if(packet_size > NETPACKET_ON_CONTINUE_SIZE()) return false;
	dst->pos = dstpos + packet_size;
	return true;
}

bool NETPACKET_ON_COMPLETE_READ(NETPACKET_BLOB* dst, NETPACKET_ON_COMPLETE* pack)
{
	uint16 packet_size, packet_id;
	uint32 dstpos = dst->pos;
	if(!netpacket_read_uint16(dst, &packet_size)) return false;
	if(!netpacket_read_uint16(dst, &packet_id)) return false;
	if(packet_id != NETPACKET_ON_COMPLETE_ID) return false;
	if(packet_size > NETPACKET_ON_COMPLETE_SIZE()) return false;
	dst->pos = dstpos + packet_size;
	return true;
}

bool NETPACKET_DATASET_5000_READ(NETPACKET_BLOB* dst, NETPACKET_DATASET_5000* pack)
{
	uint16 packet_size, packet_id;
	uint32 dstpos = dst->pos;
	if(!netpacket_read_uint16(dst, &packet_size)) return false;
	if(!netpacket_read_uint16(dst, &packet_id)) return false;
	if(packet_id != NETPACKET_DATASET_5000_ID) return false;
	if(packet_size > NETPACKET_DATASET_5000_SIZE()) return false;
	if(!netpacket_read_array_uint8(dst, &pack->data_size,sizeof(pack->data_size))) return false;
	if(!netpacket_read_array_uint8(dst, &pack->data,sizeof(pack->data))) return false;
	dst->pos = dstpos + packet_size;
	return true;
}

bool NETPACKET_DATASET_1000_READ(NETPACKET_BLOB* dst, NETPACKET_DATASET_1000* pack)
{
	uint16 packet_size, packet_id;
	uint32 dstpos = dst->pos;
	if(!netpacket_read_uint16(dst, &packet_size)) return false;
	if(!netpacket_read_uint16(dst, &packet_id)) return false;
	if(packet_id != NETPACKET_DATASET_1000_ID) return false;
	if(packet_size > NETPACKET_DATASET_1000_SIZE()) return false;
	if(!netpacket_read_array_uint8(dst, &pack->data_size,sizeof(pack->data_size))) return false;
	if(!netpacket_read_array_uint8(dst, &pack->data,sizeof(pack->data))) return false;
	dst->pos = dstpos + packet_size;
	return true;
}

bool NETPACKET_DATASET_256_READ(NETPACKET_BLOB* dst, NETPACKET_DATASET_256* pack)
{
	uint16 packet_size, packet_id;
	uint32 dstpos = dst->pos;
	if(!netpacket_read_uint16(dst, &packet_size)) return false;
	if(!netpacket_read_uint16(dst, &packet_id)) return false;
	if(packet_id != NETPACKET_DATASET_256_ID) return false;
	if(packet_size > NETPACKET_DATASET_256_SIZE()) return false;
	if(!netpacket_read_array_uint8(dst, &pack->data_size,sizeof(pack->data_size))) return false;
	if(!netpacket_read_array_uint8(dst, &pack->data,sizeof(pack->data))) return false;
	dst->pos = dstpos + packet_size;
	return true;
}

bool NETPACKET_DATASET_128_READ(NETPACKET_BLOB* dst, NETPACKET_DATASET_128* pack)
{
	uint16 packet_size, packet_id;
	uint32 dstpos = dst->pos;
	if(!netpacket_read_uint16(dst, &packet_size)) return false;
	if(!netpacket_read_uint16(dst, &packet_id)) return false;
	if(packet_id != NETPACKET_DATASET_128_ID) return false;
	if(packet_size > NETPACKET_DATASET_128_SIZE()) return false;
	if(!netpacket_read_array_uint8(dst, &pack->data_size,sizeof(pack->data_size))) return false;
	if(!netpacket_read_array_uint8(dst, &pack->data,sizeof(pack->data))) return false;
	dst->pos = dstpos + packet_size;
	return true;
}

bool NETPACKET_DATASET_64_READ(NETPACKET_BLOB* dst, NETPACKET_DATASET_64* pack)
{
	uint16 packet_size, packet_id;
	uint32 dstpos = dst->pos;
	if(!netpacket_read_uint16(dst, &packet_size)) return false;
	if(!netpacket_read_uint16(dst, &packet_id)) return false;
	if(packet_id != NETPACKET_DATASET_64_ID) return false;
	if(packet_size > NETPACKET_DATASET_64_SIZE()) return false;
	if(!netpacket_read_array_uint8(dst, &pack->data_size,sizeof(pack->data_size))) return false;
	if(!netpacket_read_array_uint8(dst, &pack->data,sizeof(pack->data))) return false;
	dst->pos = dstpos + packet_size;
	return true;
}

bool NETPACKET_DATASET_32_READ(NETPACKET_BLOB* dst, NETPACKET_DATASET_32* pack)
{
	uint16 packet_size, packet_id;
	uint32 dstpos = dst->pos;
	if(!netpacket_read_uint16(dst, &packet_size)) return false;
	if(!netpacket_read_uint16(dst, &packet_id)) return false;
	if(packet_id != NETPACKET_DATASET_32_ID) return false;
	if(packet_size > NETPACKET_DATASET_32_SIZE()) return false;
	if(!netpacket_read_array_uint8(dst, &pack->data_size,sizeof(pack->data_size))) return false;
	if(!netpacket_read_array_uint8(dst, &pack->data,sizeof(pack->data))) return false;
	dst->pos = dstpos + packet_size;
	return true;
}

bool NETPACKET_DATASET_16_READ(NETPACKET_BLOB* dst, NETPACKET_DATASET_16* pack)
{
	uint16 packet_size, packet_id;
	uint32 dstpos = dst->pos;
	if(!netpacket_read_uint16(dst, &packet_size)) return false;
	if(!netpacket_read_uint16(dst, &packet_id)) return false;
	if(packet_id != NETPACKET_DATASET_16_ID) return false;
	if(packet_size > NETPACKET_DATASET_16_SIZE()) return false;
	if(!netpacket_read_array_uint8(dst, &pack->data_size,sizeof(pack->data_size))) return false;
	if(!netpacket_read_array_uint8(dst, &pack->data,sizeof(pack->data))) return false;
	dst->pos = dstpos + packet_size;
	return true;
}

int32 NETPACKET_CLIENT_HI_SIZE(void)
{
	static NETPACKET_CLIENT_HI pack = {0};
	int32 packet_size = 2+2;
	packet_size += sizeof(pack.clnt_vers);
	packet_size += sizeof(pack.clnt_platform_id);
	packet_size += sizeof(pack.clnt_send_limit);
	packet_size += sizeof(pack.clnt_job_id);
	return packet_size;
}

int32 NETPACKET_CLIENT_HANDSHAKE_SIZE(void)
{
	static NETPACKET_CLIENT_HANDSHAKE pack = {0};
	int32 packet_size = 2+2;
	packet_size += sizeof(pack.clnt_handshake);
	return packet_size;
}

int32 NETPACKET_SERVER_HI_SIZE(void)
{
	static NETPACKET_SERVER_HI pack = {0};
	int32 packet_size = 2+2;
	packet_size += sizeof(pack.srvr_vers);
	packet_size += sizeof(pack.srvr_platform_id);
	packet_size += sizeof(pack.srvr_send_limit);
	packet_size += sizeof(pack.srvr_handshake);
	return packet_size;
}

int32 NETPACKET_SERVER_WELCOME_SIZE(void)
{
	static NETPACKET_SERVER_WELCOME pack = {0};
	int32 packet_size = 2+2;
	return packet_size;
}

int32 NETPACKET_CLIENT_ECHO_SIZE(void)
{
	static NETPACKET_CLIENT_ECHO pack = {0};
	int32 packet_size = 2+2;
	packet_size += sizeof(pack.echo_data);
	return packet_size;
}

int32 NETPACKET_SERVER_ECHO_SIZE(void)
{
	static NETPACKET_SERVER_ECHO pack = {0};
	int32 packet_size = 2+2;
	packet_size += sizeof(pack.echo_data);
	return packet_size;
}

int32 NETPACKET_GET_FILE_INFO_SIZE(void)
{
	static NETPACKET_GET_FILE_INFO pack = {0};
	int32 packet_size = 2+2;
	packet_size += sizeof(pack.file_name);
	return packet_size;
}

int32 NETPACKET_FILE_INFO_SIZE(void)
{
	static NETPACKET_FILE_INFO pack = {0};
	int32 packet_size = 2+2;
	packet_size += sizeof(pack.file_name);
	packet_size += sizeof(pack.modified_timestamp);
	return packet_size;
}

int32 NETPACKET_DOWNLOAD_FILE_SIZE(void)
{
	static NETPACKET_DOWNLOAD_FILE pack = {0};
	int32 packet_size = 2+2;
	packet_size += sizeof(pack.file_name);
	return packet_size;
}

int32 NETPACKET_ON_INFO_SIZE(void)
{
	static NETPACKET_ON_INFO pack = {0};
	int32 packet_size = 2+2;
	return packet_size;
}

int32 NETPACKET_ON_IDLE_SIZE(void)
{
	static NETPACKET_ON_IDLE pack = {0};
	int32 packet_size = 2+2;
	return packet_size;
}

int32 NETPACKET_ON_UPDATE_SIZE(void)
{
	static NETPACKET_ON_UPDATE pack = {0};
	int32 packet_size = 2+2;
	return packet_size;
}

int32 NETPACKET_ON_CONTINUE_SIZE(void)
{
	static NETPACKET_ON_CONTINUE pack = {0};
	int32 packet_size = 2+2;
	return packet_size;
}

int32 NETPACKET_ON_COMPLETE_SIZE(void)
{
	static NETPACKET_ON_COMPLETE pack = {0};
	int32 packet_size = 2+2;
	return packet_size;
}

int32 NETPACKET_DATASET_5000_SIZE(void)
{
	static NETPACKET_DATASET_5000 pack = {0};
	int32 packet_size = 2+2;
	packet_size += sizeof(pack.data_size);
	packet_size += sizeof(pack.data);
	return packet_size;
}

int32 NETPACKET_DATASET_1000_SIZE(void)
{
	static NETPACKET_DATASET_1000 pack = {0};
	int32 packet_size = 2+2;
	packet_size += sizeof(pack.data_size);
	packet_size += sizeof(pack.data);
	return packet_size;
}

int32 NETPACKET_DATASET_256_SIZE(void)
{
	static NETPACKET_DATASET_256 pack = {0};
	int32 packet_size = 2+2;
	packet_size += sizeof(pack.data_size);
	packet_size += sizeof(pack.data);
	return packet_size;
}

int32 NETPACKET_DATASET_128_SIZE(void)
{
	static NETPACKET_DATASET_128 pack = {0};
	int32 packet_size = 2+2;
	packet_size += sizeof(pack.data_size);
	packet_size += sizeof(pack.data);
	return packet_size;
}

int32 NETPACKET_DATASET_64_SIZE(void)
{
	static NETPACKET_DATASET_64 pack = {0};
	int32 packet_size = 2+2;
	packet_size += sizeof(pack.data_size);
	packet_size += sizeof(pack.data);
	return packet_size;
}

int32 NETPACKET_DATASET_32_SIZE(void)
{
	static NETPACKET_DATASET_32 pack = {0};
	int32 packet_size = 2+2;
	packet_size += sizeof(pack.data_size);
	packet_size += sizeof(pack.data);
	return packet_size;
}

int32 NETPACKET_DATASET_16_SIZE(void)
{
	static NETPACKET_DATASET_16 pack = {0};
	int32 packet_size = 2+2;
	packet_size += sizeof(pack.data_size);
	packet_size += sizeof(pack.data);
	return packet_size;
}

int32 C_VEC_NETPACKET_CLIENT_HI_SIZE(C_VEC_NETPACKET_CLIENT_HI* vec)
{
	int32 packet_size = 4 + 4 + 4;
	packet_size += vec->count * NETPACKET_CLIENT_HI_SIZE();
	return packet_size;
}
bool C_VEC_NETPACKET_CLIENT_HI_WRITE(NETPACKET_BLOB* dst, C_VEC_NETPACKET_CLIENT_HI* vec)
{
	uint32 packet_size = C_VEC_NETPACKET_CLIENT_HI_SIZE(vec), packet_id = NETPACKET_CLIENT_HI_ID, packet_count = vec->count;
	if(!netpacket_write_uint32(dst, &packet_size)) return false;
	if(!netpacket_write_uint32(dst, &packet_id)) return false;
	if(!netpacket_write_uint32(dst, &packet_count)) return false;
	c_vec_foreach_ptr(vec, NETPACKET_CLIENT_HI * pack)
	{
		if(!NETPACKET_CLIENT_HI_WRITE(dst, pack)) return false;
	}
	return true;
}
bool C_VEC_NETPACKET_CLIENT_HI_READ(NETPACKET_BLOB* dst, C_VEC_NETPACKET_CLIENT_HI* vec)
{
	uint32 packet_size = 0, packet_id = NETPACKET_CLIENT_HI_ID, packet_count;
	uint32 dstpos = dst->pos;
	if(!netpacket_read_uint32(dst, &packet_size)) return false;
	if(!netpacket_read_uint32(dst, &packet_id)) return false;
	if(!netpacket_read_uint32(dst, &packet_count)) return false;
	if(packet_id != NETPACKET_CLIENT_HI_ID) return false;
	if(packet_size > (4+4+4 + packet_count * NETPACKET_CLIENT_HI_SIZE())) return false;
	for(int32 i=0; i < packet_count; i++)
	{
		NETPACKET_CLIENT_HI pack = { 0 };
		if(!NETPACKET_CLIENT_HI_READ(dst, &pack)) return false;
		c_vec_push(vec,pack);
	}
	dst->pos = dstpos + packet_size;
	return true;
}
int32 C_VEC_NETPACKET_CLIENT_HANDSHAKE_SIZE(C_VEC_NETPACKET_CLIENT_HANDSHAKE* vec)
{
	int32 packet_size = 4 + 4 + 4;
	packet_size += vec->count * NETPACKET_CLIENT_HANDSHAKE_SIZE();
	return packet_size;
}
bool C_VEC_NETPACKET_CLIENT_HANDSHAKE_WRITE(NETPACKET_BLOB* dst, C_VEC_NETPACKET_CLIENT_HANDSHAKE* vec)
{
	uint32 packet_size = C_VEC_NETPACKET_CLIENT_HANDSHAKE_SIZE(vec), packet_id = NETPACKET_CLIENT_HANDSHAKE_ID, packet_count = vec->count;
	if(!netpacket_write_uint32(dst, &packet_size)) return false;
	if(!netpacket_write_uint32(dst, &packet_id)) return false;
	if(!netpacket_write_uint32(dst, &packet_count)) return false;
	c_vec_foreach_ptr(vec, NETPACKET_CLIENT_HANDSHAKE * pack)
	{
		if(!NETPACKET_CLIENT_HANDSHAKE_WRITE(dst, pack)) return false;
	}
	return true;
}
bool C_VEC_NETPACKET_CLIENT_HANDSHAKE_READ(NETPACKET_BLOB* dst, C_VEC_NETPACKET_CLIENT_HANDSHAKE* vec)
{
	uint32 packet_size = 0, packet_id = NETPACKET_CLIENT_HANDSHAKE_ID, packet_count;
	uint32 dstpos = dst->pos;
	if(!netpacket_read_uint32(dst, &packet_size)) return false;
	if(!netpacket_read_uint32(dst, &packet_id)) return false;
	if(!netpacket_read_uint32(dst, &packet_count)) return false;
	if(packet_id != NETPACKET_CLIENT_HANDSHAKE_ID) return false;
	if(packet_size > (4+4+4 + packet_count * NETPACKET_CLIENT_HANDSHAKE_SIZE())) return false;
	for(int32 i=0; i < packet_count; i++)
	{
		NETPACKET_CLIENT_HANDSHAKE pack = { 0 };
		if(!NETPACKET_CLIENT_HANDSHAKE_READ(dst, &pack)) return false;
		c_vec_push(vec,pack);
	}
	dst->pos = dstpos + packet_size;
	return true;
}
int32 C_VEC_NETPACKET_SERVER_HI_SIZE(C_VEC_NETPACKET_SERVER_HI* vec)
{
	int32 packet_size = 4 + 4 + 4;
	packet_size += vec->count * NETPACKET_SERVER_HI_SIZE();
	return packet_size;
}
bool C_VEC_NETPACKET_SERVER_HI_WRITE(NETPACKET_BLOB* dst, C_VEC_NETPACKET_SERVER_HI* vec)
{
	uint32 packet_size = C_VEC_NETPACKET_SERVER_HI_SIZE(vec), packet_id = NETPACKET_SERVER_HI_ID, packet_count = vec->count;
	if(!netpacket_write_uint32(dst, &packet_size)) return false;
	if(!netpacket_write_uint32(dst, &packet_id)) return false;
	if(!netpacket_write_uint32(dst, &packet_count)) return false;
	c_vec_foreach_ptr(vec, NETPACKET_SERVER_HI * pack)
	{
		if(!NETPACKET_SERVER_HI_WRITE(dst, pack)) return false;
	}
	return true;
}
bool C_VEC_NETPACKET_SERVER_HI_READ(NETPACKET_BLOB* dst, C_VEC_NETPACKET_SERVER_HI* vec)
{
	uint32 packet_size = 0, packet_id = NETPACKET_SERVER_HI_ID, packet_count;
	uint32 dstpos = dst->pos;
	if(!netpacket_read_uint32(dst, &packet_size)) return false;
	if(!netpacket_read_uint32(dst, &packet_id)) return false;
	if(!netpacket_read_uint32(dst, &packet_count)) return false;
	if(packet_id != NETPACKET_SERVER_HI_ID) return false;
	if(packet_size > (4+4+4 + packet_count * NETPACKET_SERVER_HI_SIZE())) return false;
	for(int32 i=0; i < packet_count; i++)
	{
		NETPACKET_SERVER_HI pack = { 0 };
		if(!NETPACKET_SERVER_HI_READ(dst, &pack)) return false;
		c_vec_push(vec,pack);
	}
	dst->pos = dstpos + packet_size;
	return true;
}
int32 C_VEC_NETPACKET_SERVER_WELCOME_SIZE(C_VEC_NETPACKET_SERVER_WELCOME* vec)
{
	int32 packet_size = 4 + 4 + 4;
	packet_size += vec->count * NETPACKET_SERVER_WELCOME_SIZE();
	return packet_size;
}
bool C_VEC_NETPACKET_SERVER_WELCOME_WRITE(NETPACKET_BLOB* dst, C_VEC_NETPACKET_SERVER_WELCOME* vec)
{
	uint32 packet_size = C_VEC_NETPACKET_SERVER_WELCOME_SIZE(vec), packet_id = NETPACKET_SERVER_WELCOME_ID, packet_count = vec->count;
	if(!netpacket_write_uint32(dst, &packet_size)) return false;
	if(!netpacket_write_uint32(dst, &packet_id)) return false;
	if(!netpacket_write_uint32(dst, &packet_count)) return false;
	c_vec_foreach_ptr(vec, NETPACKET_SERVER_WELCOME * pack)
	{
		if(!NETPACKET_SERVER_WELCOME_WRITE(dst, pack)) return false;
	}
	return true;
}
bool C_VEC_NETPACKET_SERVER_WELCOME_READ(NETPACKET_BLOB* dst, C_VEC_NETPACKET_SERVER_WELCOME* vec)
{
	uint32 packet_size = 0, packet_id = NETPACKET_SERVER_WELCOME_ID, packet_count;
	uint32 dstpos = dst->pos;
	if(!netpacket_read_uint32(dst, &packet_size)) return false;
	if(!netpacket_read_uint32(dst, &packet_id)) return false;
	if(!netpacket_read_uint32(dst, &packet_count)) return false;
	if(packet_id != NETPACKET_SERVER_WELCOME_ID) return false;
	if(packet_size > (4+4+4 + packet_count * NETPACKET_SERVER_WELCOME_SIZE())) return false;
	for(int32 i=0; i < packet_count; i++)
	{
		NETPACKET_SERVER_WELCOME pack = { 0 };
		if(!NETPACKET_SERVER_WELCOME_READ(dst, &pack)) return false;
		c_vec_push(vec,pack);
	}
	dst->pos = dstpos + packet_size;
	return true;
}
int32 C_VEC_NETPACKET_CLIENT_ECHO_SIZE(C_VEC_NETPACKET_CLIENT_ECHO* vec)
{
	int32 packet_size = 4 + 4 + 4;
	packet_size += vec->count * NETPACKET_CLIENT_ECHO_SIZE();
	return packet_size;
}
bool C_VEC_NETPACKET_CLIENT_ECHO_WRITE(NETPACKET_BLOB* dst, C_VEC_NETPACKET_CLIENT_ECHO* vec)
{
	uint32 packet_size = C_VEC_NETPACKET_CLIENT_ECHO_SIZE(vec), packet_id = NETPACKET_CLIENT_ECHO_ID, packet_count = vec->count;
	if(!netpacket_write_uint32(dst, &packet_size)) return false;
	if(!netpacket_write_uint32(dst, &packet_id)) return false;
	if(!netpacket_write_uint32(dst, &packet_count)) return false;
	c_vec_foreach_ptr(vec, NETPACKET_CLIENT_ECHO * pack)
	{
		if(!NETPACKET_CLIENT_ECHO_WRITE(dst, pack)) return false;
	}
	return true;
}
bool C_VEC_NETPACKET_CLIENT_ECHO_READ(NETPACKET_BLOB* dst, C_VEC_NETPACKET_CLIENT_ECHO* vec)
{
	uint32 packet_size = 0, packet_id = NETPACKET_CLIENT_ECHO_ID, packet_count;
	uint32 dstpos = dst->pos;
	if(!netpacket_read_uint32(dst, &packet_size)) return false;
	if(!netpacket_read_uint32(dst, &packet_id)) return false;
	if(!netpacket_read_uint32(dst, &packet_count)) return false;
	if(packet_id != NETPACKET_CLIENT_ECHO_ID) return false;
	if(packet_size > (4+4+4 + packet_count * NETPACKET_CLIENT_ECHO_SIZE())) return false;
	for(int32 i=0; i < packet_count; i++)
	{
		NETPACKET_CLIENT_ECHO pack = { 0 };
		if(!NETPACKET_CLIENT_ECHO_READ(dst, &pack)) return false;
		c_vec_push(vec,pack);
	}
	dst->pos = dstpos + packet_size;
	return true;
}
int32 C_VEC_NETPACKET_SERVER_ECHO_SIZE(C_VEC_NETPACKET_SERVER_ECHO* vec)
{
	int32 packet_size = 4 + 4 + 4;
	packet_size += vec->count * NETPACKET_SERVER_ECHO_SIZE();
	return packet_size;
}
bool C_VEC_NETPACKET_SERVER_ECHO_WRITE(NETPACKET_BLOB* dst, C_VEC_NETPACKET_SERVER_ECHO* vec)
{
	uint32 packet_size = C_VEC_NETPACKET_SERVER_ECHO_SIZE(vec), packet_id = NETPACKET_SERVER_ECHO_ID, packet_count = vec->count;
	if(!netpacket_write_uint32(dst, &packet_size)) return false;
	if(!netpacket_write_uint32(dst, &packet_id)) return false;
	if(!netpacket_write_uint32(dst, &packet_count)) return false;
	c_vec_foreach_ptr(vec, NETPACKET_SERVER_ECHO * pack)
	{
		if(!NETPACKET_SERVER_ECHO_WRITE(dst, pack)) return false;
	}
	return true;
}
bool C_VEC_NETPACKET_SERVER_ECHO_READ(NETPACKET_BLOB* dst, C_VEC_NETPACKET_SERVER_ECHO* vec)
{
	uint32 packet_size = 0, packet_id = NETPACKET_SERVER_ECHO_ID, packet_count;
	uint32 dstpos = dst->pos;
	if(!netpacket_read_uint32(dst, &packet_size)) return false;
	if(!netpacket_read_uint32(dst, &packet_id)) return false;
	if(!netpacket_read_uint32(dst, &packet_count)) return false;
	if(packet_id != NETPACKET_SERVER_ECHO_ID) return false;
	if(packet_size > (4+4+4 + packet_count * NETPACKET_SERVER_ECHO_SIZE())) return false;
	for(int32 i=0; i < packet_count; i++)
	{
		NETPACKET_SERVER_ECHO pack = { 0 };
		if(!NETPACKET_SERVER_ECHO_READ(dst, &pack)) return false;
		c_vec_push(vec,pack);
	}
	dst->pos = dstpos + packet_size;
	return true;
}
int32 C_VEC_NETPACKET_GET_FILE_INFO_SIZE(C_VEC_NETPACKET_GET_FILE_INFO* vec)
{
	int32 packet_size = 4 + 4 + 4;
	packet_size += vec->count * NETPACKET_GET_FILE_INFO_SIZE();
	return packet_size;
}
bool C_VEC_NETPACKET_GET_FILE_INFO_WRITE(NETPACKET_BLOB* dst, C_VEC_NETPACKET_GET_FILE_INFO* vec)
{
	uint32 packet_size = C_VEC_NETPACKET_GET_FILE_INFO_SIZE(vec), packet_id = NETPACKET_GET_FILE_INFO_ID, packet_count = vec->count;
	if(!netpacket_write_uint32(dst, &packet_size)) return false;
	if(!netpacket_write_uint32(dst, &packet_id)) return false;
	if(!netpacket_write_uint32(dst, &packet_count)) return false;
	c_vec_foreach_ptr(vec, NETPACKET_GET_FILE_INFO * pack)
	{
		if(!NETPACKET_GET_FILE_INFO_WRITE(dst, pack)) return false;
	}
	return true;
}
bool C_VEC_NETPACKET_GET_FILE_INFO_READ(NETPACKET_BLOB* dst, C_VEC_NETPACKET_GET_FILE_INFO* vec)
{
	uint32 packet_size = 0, packet_id = NETPACKET_GET_FILE_INFO_ID, packet_count;
	uint32 dstpos = dst->pos;
	if(!netpacket_read_uint32(dst, &packet_size)) return false;
	if(!netpacket_read_uint32(dst, &packet_id)) return false;
	if(!netpacket_read_uint32(dst, &packet_count)) return false;
	if(packet_id != NETPACKET_GET_FILE_INFO_ID) return false;
	if(packet_size > (4+4+4 + packet_count * NETPACKET_GET_FILE_INFO_SIZE())) return false;
	for(int32 i=0; i < packet_count; i++)
	{
		NETPACKET_GET_FILE_INFO pack = { 0 };
		if(!NETPACKET_GET_FILE_INFO_READ(dst, &pack)) return false;
		c_vec_push(vec,pack);
	}
	dst->pos = dstpos + packet_size;
	return true;
}
int32 C_VEC_NETPACKET_FILE_INFO_SIZE(C_VEC_NETPACKET_FILE_INFO* vec)
{
	int32 packet_size = 4 + 4 + 4;
	packet_size += vec->count * NETPACKET_FILE_INFO_SIZE();
	return packet_size;
}
bool C_VEC_NETPACKET_FILE_INFO_WRITE(NETPACKET_BLOB* dst, C_VEC_NETPACKET_FILE_INFO* vec)
{
	uint32 packet_size = C_VEC_NETPACKET_FILE_INFO_SIZE(vec), packet_id = NETPACKET_FILE_INFO_ID, packet_count = vec->count;
	if(!netpacket_write_uint32(dst, &packet_size)) return false;
	if(!netpacket_write_uint32(dst, &packet_id)) return false;
	if(!netpacket_write_uint32(dst, &packet_count)) return false;
	c_vec_foreach_ptr(vec, NETPACKET_FILE_INFO * pack)
	{
		if(!NETPACKET_FILE_INFO_WRITE(dst, pack)) return false;
	}
	return true;
}
bool C_VEC_NETPACKET_FILE_INFO_READ(NETPACKET_BLOB* dst, C_VEC_NETPACKET_FILE_INFO* vec)
{
	uint32 packet_size = 0, packet_id = NETPACKET_FILE_INFO_ID, packet_count;
	uint32 dstpos = dst->pos;
	if(!netpacket_read_uint32(dst, &packet_size)) return false;
	if(!netpacket_read_uint32(dst, &packet_id)) return false;
	if(!netpacket_read_uint32(dst, &packet_count)) return false;
	if(packet_id != NETPACKET_FILE_INFO_ID) return false;
	if(packet_size > (4+4+4 + packet_count * NETPACKET_FILE_INFO_SIZE())) return false;
	for(int32 i=0; i < packet_count; i++)
	{
		NETPACKET_FILE_INFO pack = { 0 };
		if(!NETPACKET_FILE_INFO_READ(dst, &pack)) return false;
		c_vec_push(vec,pack);
	}
	dst->pos = dstpos + packet_size;
	return true;
}
int32 C_VEC_NETPACKET_DOWNLOAD_FILE_SIZE(C_VEC_NETPACKET_DOWNLOAD_FILE* vec)
{
	int32 packet_size = 4 + 4 + 4;
	packet_size += vec->count * NETPACKET_DOWNLOAD_FILE_SIZE();
	return packet_size;
}
bool C_VEC_NETPACKET_DOWNLOAD_FILE_WRITE(NETPACKET_BLOB* dst, C_VEC_NETPACKET_DOWNLOAD_FILE* vec)
{
	uint32 packet_size = C_VEC_NETPACKET_DOWNLOAD_FILE_SIZE(vec), packet_id = NETPACKET_DOWNLOAD_FILE_ID, packet_count = vec->count;
	if(!netpacket_write_uint32(dst, &packet_size)) return false;
	if(!netpacket_write_uint32(dst, &packet_id)) return false;
	if(!netpacket_write_uint32(dst, &packet_count)) return false;
	c_vec_foreach_ptr(vec, NETPACKET_DOWNLOAD_FILE * pack)
	{
		if(!NETPACKET_DOWNLOAD_FILE_WRITE(dst, pack)) return false;
	}
	return true;
}
bool C_VEC_NETPACKET_DOWNLOAD_FILE_READ(NETPACKET_BLOB* dst, C_VEC_NETPACKET_DOWNLOAD_FILE* vec)
{
	uint32 packet_size = 0, packet_id = NETPACKET_DOWNLOAD_FILE_ID, packet_count;
	uint32 dstpos = dst->pos;
	if(!netpacket_read_uint32(dst, &packet_size)) return false;
	if(!netpacket_read_uint32(dst, &packet_id)) return false;
	if(!netpacket_read_uint32(dst, &packet_count)) return false;
	if(packet_id != NETPACKET_DOWNLOAD_FILE_ID) return false;
	if(packet_size > (4+4+4 + packet_count * NETPACKET_DOWNLOAD_FILE_SIZE())) return false;
	for(int32 i=0; i < packet_count; i++)
	{
		NETPACKET_DOWNLOAD_FILE pack = { 0 };
		if(!NETPACKET_DOWNLOAD_FILE_READ(dst, &pack)) return false;
		c_vec_push(vec,pack);
	}
	dst->pos = dstpos + packet_size;
	return true;
}
int32 C_VEC_NETPACKET_ON_INFO_SIZE(C_VEC_NETPACKET_ON_INFO* vec)
{
	int32 packet_size = 4 + 4 + 4;
	packet_size += vec->count * NETPACKET_ON_INFO_SIZE();
	return packet_size;
}
bool C_VEC_NETPACKET_ON_INFO_WRITE(NETPACKET_BLOB* dst, C_VEC_NETPACKET_ON_INFO* vec)
{
	uint32 packet_size = C_VEC_NETPACKET_ON_INFO_SIZE(vec), packet_id = NETPACKET_ON_INFO_ID, packet_count = vec->count;
	if(!netpacket_write_uint32(dst, &packet_size)) return false;
	if(!netpacket_write_uint32(dst, &packet_id)) return false;
	if(!netpacket_write_uint32(dst, &packet_count)) return false;
	c_vec_foreach_ptr(vec, NETPACKET_ON_INFO * pack)
	{
		if(!NETPACKET_ON_INFO_WRITE(dst, pack)) return false;
	}
	return true;
}
bool C_VEC_NETPACKET_ON_INFO_READ(NETPACKET_BLOB* dst, C_VEC_NETPACKET_ON_INFO* vec)
{
	uint32 packet_size = 0, packet_id = NETPACKET_ON_INFO_ID, packet_count;
	uint32 dstpos = dst->pos;
	if(!netpacket_read_uint32(dst, &packet_size)) return false;
	if(!netpacket_read_uint32(dst, &packet_id)) return false;
	if(!netpacket_read_uint32(dst, &packet_count)) return false;
	if(packet_id != NETPACKET_ON_INFO_ID) return false;
	if(packet_size > (4+4+4 + packet_count * NETPACKET_ON_INFO_SIZE())) return false;
	for(int32 i=0; i < packet_count; i++)
	{
		NETPACKET_ON_INFO pack = { 0 };
		if(!NETPACKET_ON_INFO_READ(dst, &pack)) return false;
		c_vec_push(vec,pack);
	}
	dst->pos = dstpos + packet_size;
	return true;
}
int32 C_VEC_NETPACKET_ON_IDLE_SIZE(C_VEC_NETPACKET_ON_IDLE* vec)
{
	int32 packet_size = 4 + 4 + 4;
	packet_size += vec->count * NETPACKET_ON_IDLE_SIZE();
	return packet_size;
}
bool C_VEC_NETPACKET_ON_IDLE_WRITE(NETPACKET_BLOB* dst, C_VEC_NETPACKET_ON_IDLE* vec)
{
	uint32 packet_size = C_VEC_NETPACKET_ON_IDLE_SIZE(vec), packet_id = NETPACKET_ON_IDLE_ID, packet_count = vec->count;
	if(!netpacket_write_uint32(dst, &packet_size)) return false;
	if(!netpacket_write_uint32(dst, &packet_id)) return false;
	if(!netpacket_write_uint32(dst, &packet_count)) return false;
	c_vec_foreach_ptr(vec, NETPACKET_ON_IDLE * pack)
	{
		if(!NETPACKET_ON_IDLE_WRITE(dst, pack)) return false;
	}
	return true;
}
bool C_VEC_NETPACKET_ON_IDLE_READ(NETPACKET_BLOB* dst, C_VEC_NETPACKET_ON_IDLE* vec)
{
	uint32 packet_size = 0, packet_id = NETPACKET_ON_IDLE_ID, packet_count;
	uint32 dstpos = dst->pos;
	if(!netpacket_read_uint32(dst, &packet_size)) return false;
	if(!netpacket_read_uint32(dst, &packet_id)) return false;
	if(!netpacket_read_uint32(dst, &packet_count)) return false;
	if(packet_id != NETPACKET_ON_IDLE_ID) return false;
	if(packet_size > (4+4+4 + packet_count * NETPACKET_ON_IDLE_SIZE())) return false;
	for(int32 i=0; i < packet_count; i++)
	{
		NETPACKET_ON_IDLE pack = { 0 };
		if(!NETPACKET_ON_IDLE_READ(dst, &pack)) return false;
		c_vec_push(vec,pack);
	}
	dst->pos = dstpos + packet_size;
	return true;
}
int32 C_VEC_NETPACKET_ON_UPDATE_SIZE(C_VEC_NETPACKET_ON_UPDATE* vec)
{
	int32 packet_size = 4 + 4 + 4;
	packet_size += vec->count * NETPACKET_ON_UPDATE_SIZE();
	return packet_size;
}
bool C_VEC_NETPACKET_ON_UPDATE_WRITE(NETPACKET_BLOB* dst, C_VEC_NETPACKET_ON_UPDATE* vec)
{
	uint32 packet_size = C_VEC_NETPACKET_ON_UPDATE_SIZE(vec), packet_id = NETPACKET_ON_UPDATE_ID, packet_count = vec->count;
	if(!netpacket_write_uint32(dst, &packet_size)) return false;
	if(!netpacket_write_uint32(dst, &packet_id)) return false;
	if(!netpacket_write_uint32(dst, &packet_count)) return false;
	c_vec_foreach_ptr(vec, NETPACKET_ON_UPDATE * pack)
	{
		if(!NETPACKET_ON_UPDATE_WRITE(dst, pack)) return false;
	}
	return true;
}
bool C_VEC_NETPACKET_ON_UPDATE_READ(NETPACKET_BLOB* dst, C_VEC_NETPACKET_ON_UPDATE* vec)
{
	uint32 packet_size = 0, packet_id = NETPACKET_ON_UPDATE_ID, packet_count;
	uint32 dstpos = dst->pos;
	if(!netpacket_read_uint32(dst, &packet_size)) return false;
	if(!netpacket_read_uint32(dst, &packet_id)) return false;
	if(!netpacket_read_uint32(dst, &packet_count)) return false;
	if(packet_id != NETPACKET_ON_UPDATE_ID) return false;
	if(packet_size > (4+4+4 + packet_count * NETPACKET_ON_UPDATE_SIZE())) return false;
	for(int32 i=0; i < packet_count; i++)
	{
		NETPACKET_ON_UPDATE pack = { 0 };
		if(!NETPACKET_ON_UPDATE_READ(dst, &pack)) return false;
		c_vec_push(vec,pack);
	}
	dst->pos = dstpos + packet_size;
	return true;
}
int32 C_VEC_NETPACKET_ON_CONTINUE_SIZE(C_VEC_NETPACKET_ON_CONTINUE* vec)
{
	int32 packet_size = 4 + 4 + 4;
	packet_size += vec->count * NETPACKET_ON_CONTINUE_SIZE();
	return packet_size;
}
bool C_VEC_NETPACKET_ON_CONTINUE_WRITE(NETPACKET_BLOB* dst, C_VEC_NETPACKET_ON_CONTINUE* vec)
{
	uint32 packet_size = C_VEC_NETPACKET_ON_CONTINUE_SIZE(vec), packet_id = NETPACKET_ON_CONTINUE_ID, packet_count = vec->count;
	if(!netpacket_write_uint32(dst, &packet_size)) return false;
	if(!netpacket_write_uint32(dst, &packet_id)) return false;
	if(!netpacket_write_uint32(dst, &packet_count)) return false;
	c_vec_foreach_ptr(vec, NETPACKET_ON_CONTINUE * pack)
	{
		if(!NETPACKET_ON_CONTINUE_WRITE(dst, pack)) return false;
	}
	return true;
}
bool C_VEC_NETPACKET_ON_CONTINUE_READ(NETPACKET_BLOB* dst, C_VEC_NETPACKET_ON_CONTINUE* vec)
{
	uint32 packet_size = 0, packet_id = NETPACKET_ON_CONTINUE_ID, packet_count;
	uint32 dstpos = dst->pos;
	if(!netpacket_read_uint32(dst, &packet_size)) return false;
	if(!netpacket_read_uint32(dst, &packet_id)) return false;
	if(!netpacket_read_uint32(dst, &packet_count)) return false;
	if(packet_id != NETPACKET_ON_CONTINUE_ID) return false;
	if(packet_size > (4+4+4 + packet_count * NETPACKET_ON_CONTINUE_SIZE())) return false;
	for(int32 i=0; i < packet_count; i++)
	{
		NETPACKET_ON_CONTINUE pack = { 0 };
		if(!NETPACKET_ON_CONTINUE_READ(dst, &pack)) return false;
		c_vec_push(vec,pack);
	}
	dst->pos = dstpos + packet_size;
	return true;
}
int32 C_VEC_NETPACKET_ON_COMPLETE_SIZE(C_VEC_NETPACKET_ON_COMPLETE* vec)
{
	int32 packet_size = 4 + 4 + 4;
	packet_size += vec->count * NETPACKET_ON_COMPLETE_SIZE();
	return packet_size;
}
bool C_VEC_NETPACKET_ON_COMPLETE_WRITE(NETPACKET_BLOB* dst, C_VEC_NETPACKET_ON_COMPLETE* vec)
{
	uint32 packet_size = C_VEC_NETPACKET_ON_COMPLETE_SIZE(vec), packet_id = NETPACKET_ON_COMPLETE_ID, packet_count = vec->count;
	if(!netpacket_write_uint32(dst, &packet_size)) return false;
	if(!netpacket_write_uint32(dst, &packet_id)) return false;
	if(!netpacket_write_uint32(dst, &packet_count)) return false;
	c_vec_foreach_ptr(vec, NETPACKET_ON_COMPLETE * pack)
	{
		if(!NETPACKET_ON_COMPLETE_WRITE(dst, pack)) return false;
	}
	return true;
}
bool C_VEC_NETPACKET_ON_COMPLETE_READ(NETPACKET_BLOB* dst, C_VEC_NETPACKET_ON_COMPLETE* vec)
{
	uint32 packet_size = 0, packet_id = NETPACKET_ON_COMPLETE_ID, packet_count;
	uint32 dstpos = dst->pos;
	if(!netpacket_read_uint32(dst, &packet_size)) return false;
	if(!netpacket_read_uint32(dst, &packet_id)) return false;
	if(!netpacket_read_uint32(dst, &packet_count)) return false;
	if(packet_id != NETPACKET_ON_COMPLETE_ID) return false;
	if(packet_size > (4+4+4 + packet_count * NETPACKET_ON_COMPLETE_SIZE())) return false;
	for(int32 i=0; i < packet_count; i++)
	{
		NETPACKET_ON_COMPLETE pack = { 0 };
		if(!NETPACKET_ON_COMPLETE_READ(dst, &pack)) return false;
		c_vec_push(vec,pack);
	}
	dst->pos = dstpos + packet_size;
	return true;
}
int32 C_VEC_NETPACKET_DATASET_5000_SIZE(C_VEC_NETPACKET_DATASET_5000* vec)
{
	int32 packet_size = 4 + 4 + 4;
	packet_size += vec->count * NETPACKET_DATASET_5000_SIZE();
	return packet_size;
}
bool C_VEC_NETPACKET_DATASET_5000_WRITE(NETPACKET_BLOB* dst, C_VEC_NETPACKET_DATASET_5000* vec)
{
	uint32 packet_size = C_VEC_NETPACKET_DATASET_5000_SIZE(vec), packet_id = NETPACKET_DATASET_5000_ID, packet_count = vec->count;
	if(!netpacket_write_uint32(dst, &packet_size)) return false;
	if(!netpacket_write_uint32(dst, &packet_id)) return false;
	if(!netpacket_write_uint32(dst, &packet_count)) return false;
	c_vec_foreach_ptr(vec, NETPACKET_DATASET_5000 * pack)
	{
		if(!NETPACKET_DATASET_5000_WRITE(dst, pack)) return false;
	}
	return true;
}
bool C_VEC_NETPACKET_DATASET_5000_READ(NETPACKET_BLOB* dst, C_VEC_NETPACKET_DATASET_5000* vec)
{
	uint32 packet_size = 0, packet_id = NETPACKET_DATASET_5000_ID, packet_count;
	uint32 dstpos = dst->pos;
	if(!netpacket_read_uint32(dst, &packet_size)) return false;
	if(!netpacket_read_uint32(dst, &packet_id)) return false;
	if(!netpacket_read_uint32(dst, &packet_count)) return false;
	if(packet_id != NETPACKET_DATASET_5000_ID) return false;
	if(packet_size > (4+4+4 + packet_count * NETPACKET_DATASET_5000_SIZE())) return false;
	for(int32 i=0; i < packet_count; i++)
	{
		NETPACKET_DATASET_5000 pack = { 0 };
		if(!NETPACKET_DATASET_5000_READ(dst, &pack)) return false;
		c_vec_push(vec,pack);
	}
	dst->pos = dstpos + packet_size;
	return true;
}
int32 C_VEC_NETPACKET_DATASET_1000_SIZE(C_VEC_NETPACKET_DATASET_1000* vec)
{
	int32 packet_size = 4 + 4 + 4;
	packet_size += vec->count * NETPACKET_DATASET_1000_SIZE();
	return packet_size;
}
bool C_VEC_NETPACKET_DATASET_1000_WRITE(NETPACKET_BLOB* dst, C_VEC_NETPACKET_DATASET_1000* vec)
{
	uint32 packet_size = C_VEC_NETPACKET_DATASET_1000_SIZE(vec), packet_id = NETPACKET_DATASET_1000_ID, packet_count = vec->count;
	if(!netpacket_write_uint32(dst, &packet_size)) return false;
	if(!netpacket_write_uint32(dst, &packet_id)) return false;
	if(!netpacket_write_uint32(dst, &packet_count)) return false;
	c_vec_foreach_ptr(vec, NETPACKET_DATASET_1000 * pack)
	{
		if(!NETPACKET_DATASET_1000_WRITE(dst, pack)) return false;
	}
	return true;
}
bool C_VEC_NETPACKET_DATASET_1000_READ(NETPACKET_BLOB* dst, C_VEC_NETPACKET_DATASET_1000* vec)
{
	uint32 packet_size = 0, packet_id = NETPACKET_DATASET_1000_ID, packet_count;
	uint32 dstpos = dst->pos;
	if(!netpacket_read_uint32(dst, &packet_size)) return false;
	if(!netpacket_read_uint32(dst, &packet_id)) return false;
	if(!netpacket_read_uint32(dst, &packet_count)) return false;
	if(packet_id != NETPACKET_DATASET_1000_ID) return false;
	if(packet_size > (4+4+4 + packet_count * NETPACKET_DATASET_1000_SIZE())) return false;
	for(int32 i=0; i < packet_count; i++)
	{
		NETPACKET_DATASET_1000 pack = { 0 };
		if(!NETPACKET_DATASET_1000_READ(dst, &pack)) return false;
		c_vec_push(vec,pack);
	}
	dst->pos = dstpos + packet_size;
	return true;
}
int32 C_VEC_NETPACKET_DATASET_256_SIZE(C_VEC_NETPACKET_DATASET_256* vec)
{
	int32 packet_size = 4 + 4 + 4;
	packet_size += vec->count * NETPACKET_DATASET_256_SIZE();
	return packet_size;
}
bool C_VEC_NETPACKET_DATASET_256_WRITE(NETPACKET_BLOB* dst, C_VEC_NETPACKET_DATASET_256* vec)
{
	uint32 packet_size = C_VEC_NETPACKET_DATASET_256_SIZE(vec), packet_id = NETPACKET_DATASET_256_ID, packet_count = vec->count;
	if(!netpacket_write_uint32(dst, &packet_size)) return false;
	if(!netpacket_write_uint32(dst, &packet_id)) return false;
	if(!netpacket_write_uint32(dst, &packet_count)) return false;
	c_vec_foreach_ptr(vec, NETPACKET_DATASET_256 * pack)
	{
		if(!NETPACKET_DATASET_256_WRITE(dst, pack)) return false;
	}
	return true;
}
bool C_VEC_NETPACKET_DATASET_256_READ(NETPACKET_BLOB* dst, C_VEC_NETPACKET_DATASET_256* vec)
{
	uint32 packet_size = 0, packet_id = NETPACKET_DATASET_256_ID, packet_count;
	uint32 dstpos = dst->pos;
	if(!netpacket_read_uint32(dst, &packet_size)) return false;
	if(!netpacket_read_uint32(dst, &packet_id)) return false;
	if(!netpacket_read_uint32(dst, &packet_count)) return false;
	if(packet_id != NETPACKET_DATASET_256_ID) return false;
	if(packet_size > (4+4+4 + packet_count * NETPACKET_DATASET_256_SIZE())) return false;
	for(int32 i=0; i < packet_count; i++)
	{
		NETPACKET_DATASET_256 pack = { 0 };
		if(!NETPACKET_DATASET_256_READ(dst, &pack)) return false;
		c_vec_push(vec,pack);
	}
	dst->pos = dstpos + packet_size;
	return true;
}
int32 C_VEC_NETPACKET_DATASET_128_SIZE(C_VEC_NETPACKET_DATASET_128* vec)
{
	int32 packet_size = 4 + 4 + 4;
	packet_size += vec->count * NETPACKET_DATASET_128_SIZE();
	return packet_size;
}
bool C_VEC_NETPACKET_DATASET_128_WRITE(NETPACKET_BLOB* dst, C_VEC_NETPACKET_DATASET_128* vec)
{
	uint32 packet_size = C_VEC_NETPACKET_DATASET_128_SIZE(vec), packet_id = NETPACKET_DATASET_128_ID, packet_count = vec->count;
	if(!netpacket_write_uint32(dst, &packet_size)) return false;
	if(!netpacket_write_uint32(dst, &packet_id)) return false;
	if(!netpacket_write_uint32(dst, &packet_count)) return false;
	c_vec_foreach_ptr(vec, NETPACKET_DATASET_128 * pack)
	{
		if(!NETPACKET_DATASET_128_WRITE(dst, pack)) return false;
	}
	return true;
}
bool C_VEC_NETPACKET_DATASET_128_READ(NETPACKET_BLOB* dst, C_VEC_NETPACKET_DATASET_128* vec)
{
	uint32 packet_size = 0, packet_id = NETPACKET_DATASET_128_ID, packet_count;
	uint32 dstpos = dst->pos;
	if(!netpacket_read_uint32(dst, &packet_size)) return false;
	if(!netpacket_read_uint32(dst, &packet_id)) return false;
	if(!netpacket_read_uint32(dst, &packet_count)) return false;
	if(packet_id != NETPACKET_DATASET_128_ID) return false;
	if(packet_size > (4+4+4 + packet_count * NETPACKET_DATASET_128_SIZE())) return false;
	for(int32 i=0; i < packet_count; i++)
	{
		NETPACKET_DATASET_128 pack = { 0 };
		if(!NETPACKET_DATASET_128_READ(dst, &pack)) return false;
		c_vec_push(vec,pack);
	}
	dst->pos = dstpos + packet_size;
	return true;
}
int32 C_VEC_NETPACKET_DATASET_64_SIZE(C_VEC_NETPACKET_DATASET_64* vec)
{
	int32 packet_size = 4 + 4 + 4;
	packet_size += vec->count * NETPACKET_DATASET_64_SIZE();
	return packet_size;
}
bool C_VEC_NETPACKET_DATASET_64_WRITE(NETPACKET_BLOB* dst, C_VEC_NETPACKET_DATASET_64* vec)
{
	uint32 packet_size = C_VEC_NETPACKET_DATASET_64_SIZE(vec), packet_id = NETPACKET_DATASET_64_ID, packet_count = vec->count;
	if(!netpacket_write_uint32(dst, &packet_size)) return false;
	if(!netpacket_write_uint32(dst, &packet_id)) return false;
	if(!netpacket_write_uint32(dst, &packet_count)) return false;
	c_vec_foreach_ptr(vec, NETPACKET_DATASET_64 * pack)
	{
		if(!NETPACKET_DATASET_64_WRITE(dst, pack)) return false;
	}
	return true;
}
bool C_VEC_NETPACKET_DATASET_64_READ(NETPACKET_BLOB* dst, C_VEC_NETPACKET_DATASET_64* vec)
{
	uint32 packet_size = 0, packet_id = NETPACKET_DATASET_64_ID, packet_count;
	uint32 dstpos = dst->pos;
	if(!netpacket_read_uint32(dst, &packet_size)) return false;
	if(!netpacket_read_uint32(dst, &packet_id)) return false;
	if(!netpacket_read_uint32(dst, &packet_count)) return false;
	if(packet_id != NETPACKET_DATASET_64_ID) return false;
	if(packet_size > (4+4+4 + packet_count * NETPACKET_DATASET_64_SIZE())) return false;
	for(int32 i=0; i < packet_count; i++)
	{
		NETPACKET_DATASET_64 pack = { 0 };
		if(!NETPACKET_DATASET_64_READ(dst, &pack)) return false;
		c_vec_push(vec,pack);
	}
	dst->pos = dstpos + packet_size;
	return true;
}
int32 C_VEC_NETPACKET_DATASET_32_SIZE(C_VEC_NETPACKET_DATASET_32* vec)
{
	int32 packet_size = 4 + 4 + 4;
	packet_size += vec->count * NETPACKET_DATASET_32_SIZE();
	return packet_size;
}
bool C_VEC_NETPACKET_DATASET_32_WRITE(NETPACKET_BLOB* dst, C_VEC_NETPACKET_DATASET_32* vec)
{
	uint32 packet_size = C_VEC_NETPACKET_DATASET_32_SIZE(vec), packet_id = NETPACKET_DATASET_32_ID, packet_count = vec->count;
	if(!netpacket_write_uint32(dst, &packet_size)) return false;
	if(!netpacket_write_uint32(dst, &packet_id)) return false;
	if(!netpacket_write_uint32(dst, &packet_count)) return false;
	c_vec_foreach_ptr(vec, NETPACKET_DATASET_32 * pack)
	{
		if(!NETPACKET_DATASET_32_WRITE(dst, pack)) return false;
	}
	return true;
}
bool C_VEC_NETPACKET_DATASET_32_READ(NETPACKET_BLOB* dst, C_VEC_NETPACKET_DATASET_32* vec)
{
	uint32 packet_size = 0, packet_id = NETPACKET_DATASET_32_ID, packet_count;
	uint32 dstpos = dst->pos;
	if(!netpacket_read_uint32(dst, &packet_size)) return false;
	if(!netpacket_read_uint32(dst, &packet_id)) return false;
	if(!netpacket_read_uint32(dst, &packet_count)) return false;
	if(packet_id != NETPACKET_DATASET_32_ID) return false;
	if(packet_size > (4+4+4 + packet_count * NETPACKET_DATASET_32_SIZE())) return false;
	for(int32 i=0; i < packet_count; i++)
	{
		NETPACKET_DATASET_32 pack = { 0 };
		if(!NETPACKET_DATASET_32_READ(dst, &pack)) return false;
		c_vec_push(vec,pack);
	}
	dst->pos = dstpos + packet_size;
	return true;
}
int32 C_VEC_NETPACKET_DATASET_16_SIZE(C_VEC_NETPACKET_DATASET_16* vec)
{
	int32 packet_size = 4 + 4 + 4;
	packet_size += vec->count * NETPACKET_DATASET_16_SIZE();
	return packet_size;
}
bool C_VEC_NETPACKET_DATASET_16_WRITE(NETPACKET_BLOB* dst, C_VEC_NETPACKET_DATASET_16* vec)
{
	uint32 packet_size = C_VEC_NETPACKET_DATASET_16_SIZE(vec), packet_id = NETPACKET_DATASET_16_ID, packet_count = vec->count;
	if(!netpacket_write_uint32(dst, &packet_size)) return false;
	if(!netpacket_write_uint32(dst, &packet_id)) return false;
	if(!netpacket_write_uint32(dst, &packet_count)) return false;
	c_vec_foreach_ptr(vec, NETPACKET_DATASET_16 * pack)
	{
		if(!NETPACKET_DATASET_16_WRITE(dst, pack)) return false;
	}
	return true;
}
bool C_VEC_NETPACKET_DATASET_16_READ(NETPACKET_BLOB* dst, C_VEC_NETPACKET_DATASET_16* vec)
{
	uint32 packet_size = 0, packet_id = NETPACKET_DATASET_16_ID, packet_count;
	uint32 dstpos = dst->pos;
	if(!netpacket_read_uint32(dst, &packet_size)) return false;
	if(!netpacket_read_uint32(dst, &packet_id)) return false;
	if(!netpacket_read_uint32(dst, &packet_count)) return false;
	if(packet_id != NETPACKET_DATASET_16_ID) return false;
	if(packet_size > (4+4+4 + packet_count * NETPACKET_DATASET_16_SIZE())) return false;
	for(int32 i=0; i < packet_count; i++)
	{
		NETPACKET_DATASET_16 pack = { 0 };
		if(!NETPACKET_DATASET_16_READ(dst, &pack)) return false;
		c_vec_push(vec,pack);
	}
	dst->pos = dstpos + packet_size;
	return true;
}
const char* netpackets_get_enum_string_by_id(int id)
{
	if(id < NETPACKETS_START_ID)
		return common_netpackets_get_enum_string_by_id(id);

	switch(id)
	{
	case NETPACKET_CLIENT_HI_ID:
		return "NETPACKET_CLIENT_HI_ID";
	case NETPACKET_CLIENT_HANDSHAKE_ID:
		return "NETPACKET_CLIENT_HANDSHAKE_ID";
	case NETPACKET_SERVER_HI_ID:
		return "NETPACKET_SERVER_HI_ID";
	case NETPACKET_SERVER_WELCOME_ID:
		return "NETPACKET_SERVER_WELCOME_ID";
	case NETPACKET_CLIENT_ECHO_ID:
		return "NETPACKET_CLIENT_ECHO_ID";
	case NETPACKET_SERVER_ECHO_ID:
		return "NETPACKET_SERVER_ECHO_ID";
	case NETPACKET_GET_FILE_INFO_ID:
		return "NETPACKET_GET_FILE_INFO_ID";
	case NETPACKET_FILE_INFO_ID:
		return "NETPACKET_FILE_INFO_ID";
	case NETPACKET_DOWNLOAD_FILE_ID:
		return "NETPACKET_DOWNLOAD_FILE_ID";
	case NETPACKET_ON_INFO_ID:
		return "NETPACKET_ON_INFO_ID";
	case NETPACKET_ON_IDLE_ID:
		return "NETPACKET_ON_IDLE_ID";
	case NETPACKET_ON_UPDATE_ID:
		return "NETPACKET_ON_UPDATE_ID";
	case NETPACKET_ON_CONTINUE_ID:
		return "NETPACKET_ON_CONTINUE_ID";
	case NETPACKET_ON_COMPLETE_ID:
		return "NETPACKET_ON_COMPLETE_ID";
	case NETPACKET_DATASET_5000_ID:
		return "NETPACKET_DATASET_5000_ID";
	case NETPACKET_DATASET_1000_ID:
		return "NETPACKET_DATASET_1000_ID";
	case NETPACKET_DATASET_256_ID:
		return "NETPACKET_DATASET_256_ID";
	case NETPACKET_DATASET_128_ID:
		return "NETPACKET_DATASET_128_ID";
	case NETPACKET_DATASET_64_ID:
		return "NETPACKET_DATASET_64_ID";
	case NETPACKET_DATASET_32_ID:
		return "NETPACKET_DATASET_32_ID";
	case NETPACKET_DATASET_16_ID:
		return "NETPACKET_DATASET_16_ID";
	default:
		return "Unknown packet id";
	}
}
