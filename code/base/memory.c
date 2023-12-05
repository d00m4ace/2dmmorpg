//----------------------------------------------------------------------------------
VMEM vmem_init(int32 max_size)
{
	VMEM vm;
	vm.data = CALLOC(1, max_size);
	vm.max_size = max_size;
	vm.size = 0;
	vm.align = 4;
	return vm;
}

void vmem_free(VMEM* vm)
{
	CFREE(vm->data);
	vm->data = NULL;
	vm->max_size = 0;
	vm->size = 0;
}

void* vmem_elm_alloc(VMEM* vm, int32 size)
{
	ASSERT(vm != NULL);
	ASSERT(vm->size + (size + (size % vm->align)) < vm->max_size);

	void* data = (char*)vm->data + vm->size;
	vm->size += size + (size % vm->align);
	return data;
}

void vmem_clear(VMEM* vm)
{
	ASSERT(vm != NULL);

	vm->size = 0;
}
//----------------------------------------------------------------------------------
VPTR vptr_init(int32 max_size)
{
	VPTR vp;
	vp.vptr = CALLOC(max_size, sizeof(void*));
	vp.max_size = max_size;
	vp.count = 0;
	return vp;
}

void vptr_free(VPTR* vp)
{
	CFREE(vp->vptr);
	vp->vptr = NULL;
	vp->max_size = 0;
	vp->count = 0;
}

int32 vptr_count(VPTR* vp)
{
	return vp->count;
}

void vptr_grow(VPTR* vp, int32 max_size)
{
	void** vptr = CALLOC(max_size, sizeof(void*));

	if(vp->count > max_size)
		vp->count = max_size;

	if(vp->count > 0)
		memcpy(vptr, vp->vptr, vp->count * sizeof(void*));

	CFREE(vp->vptr);
	vp->vptr = vptr;
	vp->max_size = max_size;
}

void vptr_elm_insert(VPTR* vp, int32 indx_start, int32 count)
{
	ASSERT(vp != NULL);
	ASSERT(count > 0);
	ASSERT(indx_start >= 0 && indx_start <= vp->count);

	if(vp->count + count > vp->max_size)
	{
		int32 grow_sz = vp->max_size + (vp->max_size >> 1);

		if(vp->count + count > grow_sz)
			grow_sz = vp->max_size + count;

		vptr_grow(vp, grow_sz);
	}

	{
		uint8* src = ((uint8*)vp->vptr) + indx_start * sizeof(void*);
		uint8* dst = ((uint8*)vp->vptr) + (indx_start + count) * sizeof(void*);

		if(vp->count - indx_start > 0)
			memmove(dst, src, (vp->count - indx_start) * sizeof(void*));

		memset(src, 0, count * sizeof(void*));
		vp->count += count;
	}
}

void vptr_elm_delete(VPTR* vp, int32 indx_start, int32 count)
{
	ASSERT(vp != NULL);

	if(vp->count == 0)
		return;

	if(indx_start + count > vp->count)
		count = vp->count - indx_start;

	ASSERT(count > 0);
	ASSERT(indx_start >= 0 && indx_start <= vp->count);

	{
		uint8* dst = ((uint8*)vp->vptr) + indx_start * sizeof(void*);
		uint8* src = ((uint8*)vp->vptr) + (indx_start + count) * sizeof(void*);

		if(vp->count - indx_start > 0)
			memmove(dst, src, (vp->count - indx_start) * sizeof(void*));

		vp->count -= count;
	}
}

void vptr_elm_set_at(VPTR* vp, int32 indx, void* vptr_elm)
{
	ASSERT(vp != NULL);
	ASSERT(indx >= 0 && indx < vp->count);

	vp->vptr[indx] = vptr_elm;
}

void vptr_elm_add(VPTR* vp, void* vptr_elm)
{
	ASSERT(vp != NULL);

	if(vp->count + 1 > vp->max_size)
		vptr_grow(vp, vp->max_size + (vp->max_size >> 1)); // grow to 1.5 max_size

	ASSERT(vp->count + 1 <= vp->max_size);

	vp->vptr[vp->count] = vptr_elm;
	vp->count++;
}

void vptr_elm_add_at(VPTR* vp, int32 indx, void* vptr_elm)
{
	ASSERT(vp != NULL);
	ASSERT(indx >= 0 && indx <= vp->count);

	vptr_elm_insert(vp, indx, 1);

	vp->vptr[indx] = vptr_elm;
}

void vptr_insert(VPTR* vp_dst, VPTR* vp_src, int32 dst_start, int32 src_start, int32 count)
{
	for(int32 i = 0; i < count; i++)
		vptr_elm_add_at(vp_dst, dst_start + i, vptr_elm(vp_src, src_start + i));
}

void vptr_copy(VPTR* vp_dst, VPTR* vp_src)
{
	vptr_clear(vp_dst);

	if(vp_src->count > vp_dst->max_size)
		vptr_grow(vp_dst, vp_dst->max_size + vp_src->count);

	for(int32 i = 0; i < vp_src->count; i++)
		vp_dst->vptr[i] = vp_src->vptr[i];

	vp_dst->count = vp_src->count;
}

int32 vptr_elm_indx(VPTR* vp, void* vptr_elm)
{
	ASSERT(vp != NULL);

	for(int i = 0; i < vp->count; i++)
		if(vp->vptr[i] == vptr_elm)
			return i;

	return -1;
}

void* vptr_elm(VPTR* vp, int32 indx)
{
	ASSERT(vp != NULL);
	ASSERT(indx >= 0 && indx < vp->count);

	return vp->vptr[indx];
}

void vptr_clear(VPTR* vp)
{
	ASSERT(vp != NULL);

	vp->count = 0;
}
//----------------------------------------------------------------------------------
VECM vecm_init(int32 buf_size, int32 str_count)
{
	VECM vs;
	vs.vm = vmem_init(buf_size);
	vs.vp = vptr_init(str_count);
	return vs;
}

void vecm_free(VECM* vs)
{
	vmem_free(&vs->vm);
	vptr_free(&vs->vp);
}

int32 vecm_count(VECM* vs)
{
	return vs->vp.count;
}

void vecm_elm_add_str(VECM* vs, const char* str, int32 str_size)
{
	if(str_size == -1)
		str_size = (int32)strlen(str);

	char* sptr = vmem_elm_alloc(&vs->vm, str_size + 1);
	memcpy(sptr, str, str_size);
	sptr[str_size] = 0;
	vptr_elm_add(&vs->vp, sptr);
}

void vecm_elm_add(VECM* vs, void* data, int32 data_size)
{
	char* dptr = vmem_elm_alloc(&vs->vm, data_size);
	memcpy(dptr, data, data_size);
	vptr_elm_add(&vs->vp, dptr);
}

void* vecm_elm(VECM* vs, int32 indx)
{
	ASSERT(vs != NULL);
	ASSERT(indx >= 0 && indx < vs->vp.count);

	return vs->vp.vptr[indx];
}

bool vecm_elm_equal_str(VECM* vs, int32 indx, const char* str)
{
	if(indx < 0 || indx >= vs->vp.count)
		return false;

	return (strcmp(vecm_elm(vs, indx), str) == 0);
}

bool vecm_elm_equal_chr(VECM* vs, int32 indx, char c)
{
	if(indx < 0 || indx >= vs->vp.count)
		return false;

	const char* str = vecm_elm(vs, indx);
	bool rb = (str[0] == c && str[1] == 0);
	return rb;
}

bool vecm_elm_is_digit(VECM* vs, int32 indx)
{
	if(indx < 0 || indx >= vs->vp.count)
		return false;

	const char* str = vecm_elm(vs, indx);
	bool rb = (chr_is_digit(*str) ||
		((*str == '-' || *str == '+') && (chr_is_digit(*(str + 1)) || (*(str + 1) == '.' && chr_is_digit(*(str + 2))))) ||
		(*str == '.' && chr_is_digit(*(str + 1))));
	return rb;
}

bool vecm_elm_is_word(VECM* vs, int32 indx)
{
	if(indx < 0 || indx >= vs->vp.count)
		return false;

	const char* str = vecm_elm(vs, indx);
	bool rb = (chr_is_char(*str));
	return rb;
}

bool vecm_elm_is_space(VECM* vs, int32 indx)
{
	if(indx < 0 || indx >= vs->vp.count)
		return false;

	const char* str = vecm_elm(vs, indx);
	bool rb = (chr_is_space(*str));
	return rb;
}

void vecm_clear(VECM* vs)
{
	vmem_clear(&vs->vm);
	vptr_clear(&vs->vp);
}
//----------------------------------------------------------------------------------

static VECM mem_tmp_vstr[MEM_VECM_ARR_MAX_SIZE];

void mem_init(void)
{
	for(int32 i = 0; i < MEM_VECM_ARR_MAX_SIZE; i++)
		mem_tmp_vstr[i] = vecm_init(MEM_VECM_MAX_SIZE, MEM_VECM_MAX_COUNT);

	mem_tmp_vstr[MEM_VECM_ID_FONT_TEXT].vm.align = 1;
	mem_tmp_vstr[MEM_VECM_ID_FONT_TEXT_ARG].vm.align = 1;
}

VECM* mem_get_vecm(int32 arr_indx)
{
	ASSERT(arr_indx >= 0 && arr_indx < MEM_VECM_ARR_MAX_SIZE);

	return &mem_tmp_vstr[arr_indx];
}

#define TOKEN_FLG_IS_WORD (1U<<0)
#define TOKEN_FLG_IS_DIGIT (1U<<1)
#define TOKEN_FLG_IS_SPEC (1U<<2)
#define TOKEN_FLG_IS_DIGIT_ONE_DOT (1U<<3)

int32 mem_gen_tokens(VECM* vstr, const char* str)
{
	int32 tokens_count = 0;
	const char* str_cur;
	const char* str_start;
	uint8 flags = 0;

	str_start = str_cur = str;

	for(;; str_cur++)
	{
		if(str_cur == str_start)
		{
			if(chr_is_char(*str_cur))
			{
				flags |= TOKEN_FLG_IS_WORD;
				continue;
			}
			else if(
				chr_is_digit(*str_cur) ||
				((*str_cur == '-' || *str_cur == '+') && (chr_is_digit(*(str_cur + 1)) || (*(str_cur + 1) == '.' && chr_is_digit(*(str_cur + 2))))) ||
				(*str_cur == '.' && chr_is_digit(*(str_cur + 1)))
				)
			{
				flags |= TOKEN_FLG_IS_DIGIT;
				continue;
			}
			else
			{
				flags |= TOKEN_FLG_IS_SPEC;
				continue;
			}
		}

		if(*str_cur)
		{
			if((flags & TOKEN_FLG_IS_WORD) && chr_is_alphanumeric(*str_cur))
				continue;
			else if(flags & TOKEN_FLG_IS_DIGIT)
			{
				if(chr_is_digit(*str_cur))
					continue;

				if(*str_cur == '.' && (!(flags & TOKEN_FLG_IS_DIGIT_ONE_DOT)))
				{
					flags |= TOKEN_FLG_IS_DIGIT_ONE_DOT;
					continue;
				}
			}
			else if(flags & TOKEN_FLG_IS_SPEC)
			{
			}
		}

		vecm_elm_add_str(vstr, str_start, (int32)(str_cur - str_start));
		tokens_count++;

		if(!*str_cur)
			break;

		str_start = str_cur;
		flags = 0;
		str_cur--;
	}

	return tokens_count;
}

//----------------------------------------------------------------------------------