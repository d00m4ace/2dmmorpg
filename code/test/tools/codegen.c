//--------------------------------------------------------------------------------------
#include "dirfiles.h"
//--------------------------------------------------------------------------------------
#include "strs.c"
//--------------------------------------------------------------------------------------
#define MAX_PATH_SIZE 1024

static char i_gen_src_file[MAX_PATH_SIZE] = { 0 };
static char i_gen_dst_file[MAX_PATH_SIZE] = { 0 };

static char i_gen_enum_name[MAX_PATH_SIZE] = { 0 };

#define MAX_GEN_FIELD_SIZE 80

typedef struct STRUCT_GEN_FIELD
{
	char field_type[MAX_GEN_FIELD_SIZE];
	char field_name[MAX_GEN_FIELD_SIZE];
	char field_array_x_size[MAX_GEN_FIELD_SIZE];
	char field_array_y_size[MAX_GEN_FIELD_SIZE];
} STRUCT_GEN_FIELD;

typedef struct c_vec_of(STRUCT_GEN_FIELD) c_vec_struct_gen_field;

typedef struct STRUCT_GEN_STRUCT
{
	char struct_name[MAX_GEN_FIELD_SIZE];
	char struct_enmum_id[MAX_GEN_FIELD_SIZE];
	c_vec_struct_gen_field struct_fields;
} STRUCT_GEN_STRUCT;

typedef struct c_vec_of(STRUCT_GEN_STRUCT) c_vec_struct_gen_struct;

//--------------------------------------------------------------------------------------
int main(int argc, char** argv)
{
	//--------------------------------------------------------------------------------------
	// Initialize 
	//--------------------------------------------------------------------------------------
	hal_system_init();
	//--------------------------------------------------------------------------------------
	if(argc < 2)
	{
		PRINT("USAGE: codegen.exe [options]");
		PRINT("EXAMPLE: codegen.exe -gen_src_file=output/gen/netpackets.txt -gen_dst_file=output/gen/netpackets -enum_name=ENUM_NETPACKETS");
		exit(-1);
	}
	//--------------------------------------------------------------------------------------
	for(size_t i = 1; i < argc; i++)
	{
		if(parse_opt_str(argv[i], "-gen_src_file=", i_gen_src_file)) continue;
		if(parse_opt_str(argv[i], "-gen_dst_file=", i_gen_dst_file)) continue;
		if(parse_opt_str(argv[i], "-enum_name=", i_gen_enum_name)) continue;
		PRINT("is not an option:%s", argv[i]);
	}
	//--------------------------------------------------------------------------------------
	c_vec_str strs_src_file;
	c_vec_init(&strs_src_file);

	c_vec_struct_gen_struct vec_struct_gen_structs;
	c_vec_init(&vec_struct_gen_structs);

	c_vec_str vec_strs_tmp;
	c_vec_init(&vec_strs_tmp);
	//--------------------------------------------------------------------------------------
	{
		strs_from_file(&strs_src_file, i_gen_src_file, 0);

		{
			int32 struct_indx = -1;

			for(int32 i = 0; i < strs_src_file.count; i++)
			{
				char* str_val = c_vec_get(&strs_src_file, i);
				int32 str_val_len = strlen(str_val);

				if(str_val_len == 0 && struct_indx != -1)
				{
					struct_indx = -1;
					continue;
				}

				if(struct_indx == -1)
				{
					if(str_val_len)
					{
						struct_indx = vec_struct_gen_structs.count;

						STRUCT_GEN_STRUCT struct_gen_struct = { 0 };
						c_vec_init(&struct_gen_struct.struct_fields);

						c_vec_clear(&vec_strs_tmp);
						strs_split_str(&vec_strs_tmp, str_val, "=;", STRS_FLG_SKIP_EMPTY_STRINGS);

						if(vec_strs_tmp.count)
						{
							int32 c = 0;

							strcpy(struct_gen_struct.struct_name, c_vec_get(&vec_strs_tmp, c++));

							if(c + 1 < vec_strs_tmp.count && !strcmp("=", c_vec_get(&vec_strs_tmp, c)))
								strcpy(struct_gen_struct.struct_enmum_id, c_vec_get(&vec_strs_tmp, c + 1)), c++;

							c_vec_push(&vec_struct_gen_structs, struct_gen_struct);
						}
						continue;
					}
				}
				else
				{
					c_vec_clear(&vec_strs_tmp);
					strs_split_str(&vec_strs_tmp, str_val, "[]=;", STRS_FLG_SKIP_EMPTY_STRINGS);

					if(vec_strs_tmp.count)
					{
						STRUCT_GEN_FIELD struct_gen_field = { 0 };

						int32 c = 0;

						{
							char* str = c_vec_get(&vec_strs_tmp, c++);
							strcpy(struct_gen_field.field_type, str);
						}

						{
							char* str = c_vec_get(&vec_strs_tmp, c++);
							strcpy(struct_gen_field.field_name, str);
						}

						{
							char* str = c_vec_get(&vec_strs_tmp, c++);
							if(*str == '[')
							{
								char* str = c_vec_get(&vec_strs_tmp, c++);
								strcpy(struct_gen_field.field_array_x_size, str);
								c++;
							}
						}

						{
							char* str = c_vec_get(&vec_strs_tmp, c++);
							if(*str == '[')
							{
								char* str = c_vec_get(&vec_strs_tmp, c++);
								strcpy(struct_gen_field.field_array_y_size, str);
								c++;
							}
						}

						STRUCT_GEN_STRUCT* struct_gen_struct = c_vec_get_ptr(&vec_struct_gen_structs, struct_indx);

						c_vec_push(&struct_gen_struct->struct_fields, struct_gen_field);

						int kkkk = 0;
					}
				}
			}

			c_vec_foreach(&vec_struct_gen_structs, STRUCT_GEN_STRUCT struct_gen)
			{
				PRINT("%s=%s", struct_gen.struct_name, struct_gen.struct_enmum_id);
				c_vec_foreach(&struct_gen.struct_fields, STRUCT_GEN_FIELD struct_gen_field)
				{
					PRINT("%s %s[%s][%s]", struct_gen_field.field_type, struct_gen_field.field_name, struct_gen_field.field_array_x_size, struct_gen_field.field_array_y_size);
				}
			}
		}
	}

	{
		FILE* fp = fopen(txt_str_cat(i_gen_dst_file, ".h"), "w+");

		c_vec_foreach(&vec_struct_gen_structs, STRUCT_GEN_STRUCT struct_gen)
		{
			fprintf(fp, "typedef struct %s\n", struct_gen.struct_name);
			fprintf(fp, "{\n");
			c_vec_foreach(&struct_gen.struct_fields, STRUCT_GEN_FIELD struct_gen_field)
			{
				fprintf(fp, "\t%s %s", struct_gen_field.field_type, struct_gen_field.field_name);

				if(*struct_gen_field.field_array_x_size)
					fprintf(fp, "[%s]", struct_gen_field.field_array_x_size);

				if(*struct_gen_field.field_array_y_size)
					fprintf(fp, "[%s]", struct_gen_field.field_array_y_size);

				fprintf(fp, ";\n");
			}

			if(struct_gen.struct_fields.count == 0)
				fprintf(fp, "\tuint32 nothing;\n");

			fprintf(fp, "} %s;\n\n", struct_gen.struct_name);

			fprintf(fp, "typedef struct c_vec_of(%s) C_VEC_%s;\n\n", struct_gen.struct_name, struct_gen.struct_name);
		}

		c_vec_foreach(&vec_struct_gen_structs, STRUCT_GEN_STRUCT struct_gen)
		{
			fprintf(fp, "bool %s_WRITE(NETPACKET_BLOB* dst, %s* pack);\n", struct_gen.struct_name, struct_gen.struct_name);
			fprintf(fp, "bool %s_READ(NETPACKET_BLOB* dst, %s* pack);\n", struct_gen.struct_name, struct_gen.struct_name);
			fprintf(fp, "int32 %s_SIZE(void);\n\n", struct_gen.struct_name);

			fprintf(fp, "int32 C_VEC_%s_SIZE(C_VEC_%s* vec);\n", struct_gen.struct_name, struct_gen.struct_name);
			fprintf(fp, "bool C_VEC_%s_WRITE(NETPACKET_BLOB* dst, C_VEC_%s* vec);\n", struct_gen.struct_name, struct_gen.struct_name);
			fprintf(fp, "bool C_VEC_%s_READ(NETPACKET_BLOB* dst, C_VEC_%s* vec);\n\n", struct_gen.struct_name, struct_gen.struct_name);
		}

		fprintf(fp, "const char* netpackets_get_enum_string_by_id(int id);\n");

		fclose(fp);
	}

	{
		FILE* fp = fopen(txt_str_cat(i_gen_dst_file, ".c"), "w+");

		c_vec_foreach(&vec_struct_gen_structs, STRUCT_GEN_STRUCT struct_gen)
		{
			fprintf(fp, "bool %s_WRITE(NETPACKET_BLOB* dst, %s* pack)\n", struct_gen.struct_name, struct_gen.struct_name);
			fprintf(fp, "{\n");

			fprintf(fp, "\tuint16 packet_size = %s_SIZE(), packet_id = %s_ID;\n", struct_gen.struct_name, struct_gen.struct_name);

			fprintf(fp, "\tif(!netpacket_write_uint16(dst, &packet_size)) return false;\n");
			fprintf(fp, "\tif(!netpacket_write_uint16(dst, &packet_id)) return false;\n");

			c_vec_foreach(&struct_gen.struct_fields, STRUCT_GEN_FIELD struct_gen_field)
				fprintf(fp, "\tif(!netpacket_write_array_uint8(dst, &pack->%s,sizeof(pack->%s))) return false;\n", struct_gen_field.field_name, struct_gen_field.field_name);

			fprintf(fp, "\treturn true;\n");
			fprintf(fp, "}\n\n");
		}

		c_vec_foreach(&vec_struct_gen_structs, STRUCT_GEN_STRUCT struct_gen)
		{
			fprintf(fp, "bool %s_READ(NETPACKET_BLOB* dst, %s* pack)\n", struct_gen.struct_name, struct_gen.struct_name);
			fprintf(fp, "{\n");

			fprintf(fp, "\tuint16 packet_size, packet_id;\n");
			fprintf(fp, "\tuint32 dstpos = dst->pos;\n");

			fprintf(fp, "\tif(!netpacket_read_uint16(dst, &packet_size)) return false;\n");
			fprintf(fp, "\tif(!netpacket_read_uint16(dst, &packet_id)) return false;\n");

			fprintf(fp, "\tif(packet_id != %s_ID) return false;\n", struct_gen.struct_name);
			fprintf(fp, "\tif(packet_size > %s_SIZE()) return false;\n", struct_gen.struct_name);

			c_vec_foreach(&struct_gen.struct_fields, STRUCT_GEN_FIELD struct_gen_field)
				fprintf(fp, "\tif(!netpacket_read_array_uint8(dst, &pack->%s,sizeof(pack->%s))) return false;\n", struct_gen_field.field_name, struct_gen_field.field_name);
			fprintf(fp, "\tdst->pos = dstpos + packet_size;\n");
			fprintf(fp, "\treturn true;\n");
			fprintf(fp, "}\n\n");
		}

		c_vec_foreach(&vec_struct_gen_structs, STRUCT_GEN_STRUCT struct_gen)
		{
			fprintf(fp, "int32 %s_SIZE(void)\n", struct_gen.struct_name);
			fprintf(fp, "{\n");

			fprintf(fp, "\tstatic %s pack = {0};\n", struct_gen.struct_name);

			fprintf(fp, "\tint32 packet_size = 2+2;\n");

			c_vec_foreach(&struct_gen.struct_fields, STRUCT_GEN_FIELD struct_gen_field)
				fprintf(fp, "\tpacket_size += sizeof(pack.%s);\n", struct_gen_field.field_name);

			fprintf(fp, "\treturn packet_size;\n");
			fprintf(fp, "}\n\n");
		}

		c_vec_foreach(&vec_struct_gen_structs, STRUCT_GEN_STRUCT struct_gen)
		{
			fprintf(fp, "int32 C_VEC_%s_SIZE(C_VEC_%s* vec)\n", struct_gen.struct_name, struct_gen.struct_name);
			fprintf(fp, "{\n");
			fprintf(fp, "\tint32 packet_size = 4 + 4 + 4;\n");
			fprintf(fp, "\tpacket_size += vec->count * %s_SIZE();\n", struct_gen.struct_name);
			fprintf(fp, "\treturn packet_size;\n");
			fprintf(fp, "}\n");

			fprintf(fp, "bool C_VEC_%s_WRITE(NETPACKET_BLOB* dst, C_VEC_%s* vec)\n", struct_gen.struct_name, struct_gen.struct_name);
			fprintf(fp, "{\n");
			fprintf(fp, "\tuint32 packet_size = C_VEC_%s_SIZE(vec), packet_id = %s_ID, packet_count = vec->count;\n", struct_gen.struct_name, struct_gen.struct_name);

			fprintf(fp, "\tif(!netpacket_write_uint32(dst, &packet_size)) return false;\n");
			fprintf(fp, "\tif(!netpacket_write_uint32(dst, &packet_id)) return false;\n");
			fprintf(fp, "\tif(!netpacket_write_uint32(dst, &packet_count)) return false;\n");

			fprintf(fp, "\tc_vec_foreach_ptr(vec, %s * pack)\n", struct_gen.struct_name);
			fprintf(fp, "\t{\n");
			fprintf(fp, "\t\tif(!%s_WRITE(dst, pack)) return false;\n", struct_gen.struct_name);
			fprintf(fp, "\t}\n");

			fprintf(fp, "\treturn true;\n");
			fprintf(fp, "}\n");

			fprintf(fp, "bool C_VEC_%s_READ(NETPACKET_BLOB* dst, C_VEC_%s* vec)\n", struct_gen.struct_name, struct_gen.struct_name);
			fprintf(fp, "{\n");
			fprintf(fp, "\tuint32 packet_size = 0, packet_id = %s_ID, packet_count;\n", struct_gen.struct_name);
			fprintf(fp, "\tuint32 dstpos = dst->pos;\n");

			fprintf(fp, "\tif(!netpacket_read_uint32(dst, &packet_size)) return false;\n");
			fprintf(fp, "\tif(!netpacket_read_uint32(dst, &packet_id)) return false;\n");
			fprintf(fp, "\tif(!netpacket_read_uint32(dst, &packet_count)) return false;\n");

			fprintf(fp, "\tif(packet_id != %s_ID) return false;\n", struct_gen.struct_name);
			fprintf(fp, "\tif(packet_size > (4+4+4 + packet_count * %s_SIZE())) return false;\n", struct_gen.struct_name);

			fprintf(fp, "\tfor(int32 i=0; i < packet_count; i++)\n");
			fprintf(fp, "\t{\n");
			fprintf(fp, "\t\t%s pack = { 0 };\n", struct_gen.struct_name);
			fprintf(fp, "\t\tif(!%s_READ(dst, &pack)) return false;\n", struct_gen.struct_name);
			fprintf(fp, "\t\tc_vec_push(vec,pack);\n");
			fprintf(fp, "\t}\n");

			fprintf(fp, "\tdst->pos = dstpos + packet_size;\n");
			fprintf(fp, "\treturn true;\n");
			fprintf(fp, "}\n");
		}

		{
			fprintf(fp, "const char* netpackets_get_enum_string_by_id(int id)\n");
			fprintf(fp, "{\n");

			fprintf(fp, "\tif(id < NETPACKETS_START_ID)\n");
			fprintf(fp, "\t\treturn common_netpackets_get_enum_string_by_id(id);\n\n");

			fprintf(fp, "\tswitch(id)\n");
			fprintf(fp, "\t{\n");

			c_vec_foreach(&vec_struct_gen_structs, STRUCT_GEN_STRUCT struct_gen)
			{
				fprintf(fp, "\tcase %s_ID:\n", struct_gen.struct_name);
				fprintf(fp, "\t\treturn \"%s_ID\";\n", struct_gen.struct_name);
			}

			fprintf(fp, "\tdefault:\n");
			fprintf(fp, "\t\treturn \"Unknown packet id\";\n");

			fprintf(fp, "\t}\n");
			fprintf(fp, "}\n");
		}

		fclose(fp);
	}

	{
		FILE* fp = fopen(txt_str_cat(i_gen_dst_file, "_enums.h"), "w+");

		fprintf(fp, "typedef enum\n");
		fprintf(fp, "{\n");

		c_vec_foreach(&vec_struct_gen_structs, STRUCT_GEN_STRUCT struct_gen)
		{
			fprintf(fp, "\t%s_ID", struct_gen.struct_name);

			if(strlen(struct_gen.struct_enmum_id))
				fprintf(fp, " = %s", struct_gen.struct_enmum_id);

			fprintf(fp, ",\n");
		}

		fprintf(fp, "} %s;\n", i_gen_enum_name);

		fclose(fp);
	}
	//--------------------------------------------------------------------------------------
	// De-Initialization
	//--------------------------------------------------------------------------------------
	hal_system_shutdown();
	//--------------------------------------------------------------------------------------
	return 0;
}
//--------------------------------------------------------------------------------------
