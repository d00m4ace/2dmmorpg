//----------------------------------------------------------------------------------
static inline void* CALLOC(int32 count, int32 size) { return calloc(count, size); }
static inline void CFREE(void* mptr) { if(mptr != NULL) free(mptr); }
//----------------------------------------------------------------------------------
typedef struct VMEM
{
	void* data;
	int32 max_size;
	int32 size;
	int32 align;
} VMEM;

VMEM vmem_init(int32 max_size);
void vmem_free(VMEM* vm);

void* vmem_elm_alloc(VMEM* vm, int32 size);

void vmem_clear(VMEM* vm);
//----------------------------------------------------------------------------------
typedef struct VPTR
{
	void** vptr;
	int32 max_size;
	int32 count;
} VPTR;

VPTR vptr_init(int32 max_size);
void vptr_free(VPTR* vp);
int32 vptr_count(VPTR* vp);

void vptr_grow(VPTR* vp, int32 max_size);
void vptr_elm_insert(VPTR* vp, int32 indx_start, int32 count);
void vptr_elm_delete(VPTR* vp, int32 indx_start, int32 count);

void vptr_elm_set_at(VPTR* vp, int32 indx, void* vptr_elm);

void vptr_elm_add(VPTR* vp, void* vptr_elm);
void vptr_elm_add_at(VPTR* vp, int32 indx, void* vptr_elm);

void vptr_insert(VPTR* vp_dst, VPTR* vp_src, int32 dst_start, int32 src_start, int32 count);
void vptr_copy(VPTR* vp_dst, VPTR* vp_src);

int32 vptr_elm_indx(VPTR* vp, void* vptr_elm);

void* vptr_elm(VPTR* vp, int32 indx);
void vptr_clear(VPTR* vp);
//----------------------------------------------------------------------------------
typedef struct VECM
{
	VPTR vp;
	VMEM vm;
} VECM;

VECM vecm_init(int32 buf_size, int32 str_count);
void vecm_free(VECM* vs);
int32 vecm_count(VECM* vs);

void vecm_elm_add(VECM* vs, void* data, int32 data_size);
void vecm_elm_add_str(VECM* vs, const char* str, int32 str_size); //str_size = -1 call strlen

void* vecm_elm(VECM* vs, int32 indx);

bool vecm_elm_equal_str(VECM* vs, int32 indx, const char* str);
bool vecm_elm_equal_chr(VECM* vs, int32 indx, char c);
bool vecm_elm_is_digit(VECM* vs, int32 indx);
bool vecm_elm_is_word(VECM* vs, int32 indx);
bool vecm_elm_is_space(VECM* vs, int32 indx);

void vecm_clear(VECM* vs);
//----------------------------------------------------------------------------------
#define MEM_VECM_ARR_MAX_SIZE 8
#define MEM_VECM_MAX_SIZE (1024 * 16)
#define MEM_VECM_MAX_COUNT (1024 * 6)

#define MEM_VECM_ID_FONT_TEXT (MEM_VECM_ARR_MAX_SIZE-1)
#define MEM_VECM_ID_FONT_TEXT_ARG (MEM_VECM_ARR_MAX_SIZE-2)
#define MEM_VECM_ID_TEXT_TMP0 (MEM_VECM_ARR_MAX_SIZE-3)
#define MEM_VECM_ID_TEXT_TMP1 (MEM_VECM_ARR_MAX_SIZE-4)

void mem_init(void);
VECM* mem_get_vecm(int32 arr_indx);

int32 mem_gen_tokens(VECM* vstr, const char* str);
//----------------------------------------------------------------------------------
