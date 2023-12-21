#include "player_char.h"
#include "networker/pchar_networker.h"

#include "logic/validation.c"

static char g_nt_bufer[NETSESSION_STATE_BUF_SIZE];
static NETPACKET_BLOB g_np_blob = { g_nt_bufer, 0, NETSESSION_STATE_BUF_SIZE };

#include "player_char.c"
#include "networker/pchar_networker.c"
