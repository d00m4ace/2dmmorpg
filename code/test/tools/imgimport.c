//--------------------------------------------------------------------------------------
#include "dirfiles.h"
//--------------------------------------------------------------------------------------
#include "strs.c"
//--------------------------------------------------------------------------------------
#define MAX_PATH_SIZE 1024

static char i_src_dir[MAX_PATH_SIZE] = { 0 };
static char i_dst_dir[MAX_PATH_SIZE] = { 0 };
static char i_enums_old_file[MAX_PATH_SIZE] = { 0 };
static char i_enums_prefix[MAX_PATH_SIZE] = { 0 };
static char i_imgname[MAX_PATH_SIZE] = { 0 };
static char i_enmname[MAX_PATH_SIZE] = { 0 };

static char i_color_map_file[MAX_PATH_SIZE] = { 0 };
static char i_color_pal_file[MAX_PATH_SIZE] = { 0 };

static char i_enum_start_id[MAX_PATH_SIZE] = { 0 };

static int32 i_tile;
static int32 i_image_width, i_image_height;
//--------------------------------------------------------------------------------------
// -s=resources/test/gameart/mobs/48 -tile=48 -d=output/test -enm_old=output/test/img_mobs_48_old_enums.txt -enm_prefix=IMG_MOBS_48_ -img=img_mobs_48 -color_map=output/test/color_map.txt -color_pal=output/test/color_palette.txt -enm=img_mobs_48_enums.txt -tint=234290 -tfloat=-.325 
//--------------------------------------------------------------------------------------
int main(int argc, char** argv)
{
	//--------------------------------------------------------------------------------------
	// Initialize 
	//--------------------------------------------------------------------------------------
	hal_system_init();
	//--------------------------------------------------------------------------------------
	if(argc < 7)
	{
		PRINT("USAGE: imgimport.exe [options]");
		PRINT("EXAMPLE: imgimport.exe -s=output/test/mobs/48 -tile=48 -d=output/test -enm_old=output/test/img_mobs_48_enums.txt -enm_prefix=IMG_MOBS_48_ -img=img_mobs_48 -color_map=output/test/color_map.txt -color_pal=output/test/color_palette.txt -enm=img_mobs_48_enums.txt -enm_start_id=IMG_MOBS_48_ENUMS_OFFSET -tint=234290 -tfloat=-.325");
		exit(-1);
	}
	//--------------------------------------------------------------------------------------
	for(size_t i = 1; i < argc; i++)
	{
		if(parse_opt_str(argv[i], "-s=", i_src_dir)) continue;
		if(parse_opt_str(argv[i], "-d=", i_dst_dir)) continue;
		if(parse_opt_str(argv[i], "-enm_old=", i_enums_old_file)) continue;
		if(parse_opt_str(argv[i], "-enm_prefix=", i_enums_prefix)) continue;
		if(parse_opt_str(argv[i], "-img=", i_imgname)) continue;
		if(parse_opt_str(argv[i], "-enm=", i_enmname)) continue;
		if(parse_opt_str(argv[i], "-enm_start_id=", i_enum_start_id))  continue;
		if(parse_opt_str(argv[i], "-color_map=", i_color_map_file)) continue;
		if(parse_opt_str(argv[i], "-color_pal=", i_color_pal_file)) continue;
		if(parse_opt_int(argv[i], "-tile=", &i_tile))  continue;
		PRINT("is not an option:%s", argv[i]);
	}
	//--------------------------------------------------------------------------------------
	i_image_width = i_tile;
	i_image_height = i_tile;
	//--------------------------------------------------------------------------------------
	c_vec_str src_dir_files;
	c_vec_init(&src_dir_files);

	c_vec_str strs_enums_old;
	c_vec_init(&strs_enums_old);

	c_vec_str strs_enums;
	c_vec_init(&strs_enums);

	c_vec_str strs_color_map;
	c_vec_init(&strs_color_map);

	c_vec_str strs_color_pal;
	c_vec_init(&strs_color_pal);

	c_vec_img enums_imgs;
	c_vec_init(&enums_imgs);

	c_vec_uint32 rd2c_ori;
	c_vec_init(&rd2c_ori);

	c_vec_uint32 rd2c_rep;
	c_vec_init(&rd2c_rep);

	c_vec_uint32 rd2c_pal;
	c_vec_init(&rd2c_pal);
	//--------------------------------------------------------------------------------------
	win32_dir_read(&src_dir_files, i_src_dir, ".png", DIR_FLG_SCAN_SUBDIRS);

	PRINT("directory [%s] scanned, %d files found", i_src_dir, src_dir_files.count);

	c_vec_foreach(&src_dir_files, const char* value)
	{
		//PRINT("%s", value);

		int32 chr_at = txt_str_check_filename(value);

		if(chr_at != -1)
		{
			PRINT("ERROR: filename [%s] has an invalid character at [%d].\a", value, chr_at);
			exit(-1);
		}
	}

	//--------------------------------------------------------------------------------------
	if(*i_color_map_file)
	{
		{
			uint32 bytes_read = 0;
			uint8* bytes = LoadFileData(i_color_map_file, &bytes_read);

			strs_from_bytes(&strs_color_map, bytes, bytes_read, STRS_FLG_SKIP_EMPTY_STRINGS);

			UnloadFileData(bytes);
		}

		c_vec_foreach(&strs_color_map, const char* value)
			PRINT("%s", value);

		{
			const char* str_delimiters = ",>";

			c_vec_str strs_split;
			c_vec_init(&strs_split);

			for(int32 i = 0; i < strs_color_map.count; i++)
			{
				const char* str = c_vec_get(&strs_color_map, i);

				c_vec_clear(&strs_split);
				strs_split_str(&strs_split, str, str_delimiters, STRS_FLG_SKIP_EMPTY_STRINGS | STRS_FLG_REMOVE_DOUBLE_QUOTES | STRS_FLG_SKIP_DELIMITERS);

				if(strs_split.count < 8)
				{
					PRINT("ERROR: color_map line:%d [%s] does not have enough parameters, it is necessary to have 8 ints.\a", i, str);
					exit(-1);
				}

				rd2c col_ori = ARGB32(
					txt_str_to_int(c_vec_get(&strs_split, 0), NULL),
					txt_str_to_int(c_vec_get(&strs_split, 1), NULL),
					txt_str_to_int(c_vec_get(&strs_split, 2), NULL),
					txt_str_to_int(c_vec_get(&strs_split, 3), NULL));

				rd2c col_rep = ARGB32(
					txt_str_to_int(c_vec_get(&strs_split, 4), NULL),
					txt_str_to_int(c_vec_get(&strs_split, 5), NULL),
					txt_str_to_int(c_vec_get(&strs_split, 6), NULL),
					txt_str_to_int(c_vec_get(&strs_split, 7), NULL));

				c_vec_push(&rd2c_ori, col_ori);
				c_vec_push(&rd2c_rep, col_rep);
			}
		}
	}
	//--------------------------------------------------------------------------------------
	if(*i_color_pal_file)
	{
		{
			uint32 bytes_read = 0;
			uint8* bytes = LoadFileData(i_color_pal_file, &bytes_read);

			strs_from_bytes(&strs_color_pal, bytes, bytes_read, STRS_FLG_SKIP_EMPTY_STRINGS);

			UnloadFileData(bytes);
		}

		c_vec_foreach(&strs_color_pal, const char* value)
			PRINT("%s", value);

		{
			const char* str_delimiters = ",";

			c_vec_str strs_split;
			c_vec_init(&strs_split);

			for(int32 i = 0; i < strs_color_pal.count; i++)
			{
				const char* str = c_vec_get(&strs_color_pal, i);

				c_vec_clear(&strs_split);
				strs_split_str(&strs_split, str, str_delimiters, STRS_FLG_SKIP_EMPTY_STRINGS | STRS_FLG_REMOVE_DOUBLE_QUOTES | STRS_FLG_SKIP_DELIMITERS);

				if(strs_split.count < 4)
				{
					PRINT("ERROR: color_pal line:%d [%s] does not have enough parameters, it is necessary to have 4 ints.\a", i, str);
					exit(-1);
				}

				rd2c col = ARGB32(
					txt_str_to_int(c_vec_get(&strs_split, 0), NULL),
					txt_str_to_int(c_vec_get(&strs_split, 1), NULL),
					txt_str_to_int(c_vec_get(&strs_split, 2), NULL),
					txt_str_to_int(c_vec_get(&strs_split, 3), NULL));

				c_vec_push(&rd2c_pal, col);
			}
		}
	}
	//--------------------------------------------------------------------------------------
	{
		uint32 i_enums_prefix_len = strlen(i_enums_prefix);

		{
			uint32 bytes_read = 0;
			uint8* bytes = LoadFileData(i_enums_old_file, &bytes_read);

			strs_from_bytes(&strs_enums_old, bytes, bytes_read, STRS_FLG_SKIP_EMPTY_STRINGS);

			UnloadFileData(bytes);
		}

		c_vec_foreach(&strs_enums_old, const char* value)
			PRINT("%s", value);

		{
			const char* str_delimiters = "()+-=,";

			c_vec_str strs_split;
			c_vec_init(&strs_split);

			for(int32 i = 0; i < strs_enums_old.count; i++)
			{
				const char* str = c_vec_get(&strs_enums_old, i);

				c_vec_clear(&strs_split);
				strs_split_str(&strs_split, str, str_delimiters, STRS_FLG_SKIP_EMPTY_STRINGS | STRS_FLG_REMOVE_DOUBLE_QUOTES | STRS_FLG_SKIP_DELIMITERS);

				if(strs_split.count > 0)
				{
					const char* enum_str = c_vec_get(&strs_split, 0);

					if(txt_str_find_str(enum_str, i_enums_prefix, 0) != 0)
					{
						PRINT("ERROR: Enum prefix [%s] not found in [%s].\a", i_enums_prefix, enum_str);
						exit(-1);
					}

					char* file_name = txt_str_part(enum_str, i_enums_prefix_len, -1);
					txt_str_to_small(file_name);

					c_vec_push(&strs_enums, txt_str_new(file_name));
				}
			}
		}

		c_vec_foreach(&strs_enums, const char* value)
			PRINT("%s", value);

		{
			int32 i_src_dir_len = strlen(i_src_dir);

			for(int32 i = 0; i < src_dir_files.count; i++)
			{
				const char* file_name = src_dir_files.values[i];

				int32 ext_start = txt_str_find_str_reverse(file_name, ".", -1);

				char* str_enum = txt_str_part(file_name, i_src_dir_len + 1, ext_start - i_src_dir_len - 1);

				int32 replaced_at = chr_str_replace_chr_all(str_enum, "/!", '_', 0);

				int32 found_indx = -1;

				for(int32 j = 0; j < strs_enums.count; j++)
				{
					if(strcmp(str_enum, strs_enums.values[j]) == 0)
					{
						found_indx = j;
						break;
					}
				}

				if(found_indx != -1)
					PRINT("found %s:%d at %d", file_name, i, found_indx);

				if(found_indx == -1)
					c_vec_push(&strs_enums, txt_str_new(str_enum));
			}
		}

		char* src_dir_path = txt_str_new(txt_str_cat(i_src_dir, "/"));

		char* enums_file_path = txt_format("%s/%s", i_dst_dir, i_enmname);

		FILE* enums_file = fopen(enums_file_path, "w");
		if(enums_file == NULL)
		{
			PRINT("ERROR: Could not open enums out file: %s\a", enums_file_path);
			exit(-1);
		}

		bool is_frst_enm = true;
		c_vec_foreach(&strs_enums, const char* value)
		{
			//PRINT("%s", value);

			char* enum_name = txt_str_new(txt_str_cat(i_enums_prefix, value));
			txt_str_to_title(enum_name);

			if(is_frst_enm && *i_enum_start_id)
			{
				fprintf(enums_file, "%s = %s,\n", enum_name, i_enum_start_id);
				is_frst_enm = false;
			}
			else
				fprintf(enums_file, "%s,\n", enum_name);

			char* filename = txt_str_new(txt_str_cat(txt_str_cat(src_dir_path, value), ".png"));

			bool is_file_exist = false;

			while(!is_file_exist)
			{
				is_file_exist = FileExists(filename);

				if(!is_file_exist)
				{
					int32 replace_at = chr_str_replace_chr(filename, "_", '/', 0);

					if(replace_at == -1)
						break;
				}
			}

			if(is_file_exist)
				PRINT("%s > %s", enum_name, filename);
			else
			{
				PRINT("ERROR: File not found! %s > %s\a", enum_name, filename);
				exit(-1);
			}

			Image img = LoadImage(filename);

			if(img.data == NULL)
			{
				PRINT("ERROR: Image load failed! %s > %s\a", enum_name, filename);
				exit(-1);
			}

			ImageFormat(&img, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8);

			if(img.format != PIXELFORMAT_UNCOMPRESSED_R8G8B8A8)
			{
				PRINT("ERROR: Image format is not 32bpp! %s > %s\a", enum_name, filename);
				exit(-1);
			}

			if(img.width != i_image_width || img.height != i_image_height)
			{
				PRINT("ERROR: Image size %d/%d is not %d/%d! %s > %s\a", enum_name, filename, img.width, img.height, i_image_width, i_image_height);
				exit(-1);
			}

			{
				RD2IMG r2img = rd2_img_init(img.width, img.height);
				rd2_img_raw_copy(&r2img, img.data, img.width * 4, img.width, img.height);
				c_vec_push(&enums_imgs, r2img);
			}

			UnloadImage(img);
		}

		fclose(enums_file);

		{
			int32 img_map_width = 16 * (i_image_width + 1) + 1;
			int32 img_map_height = (enums_imgs.count / 16 + 1) * (i_image_height + 1) + 1;

			RD2IMG img_map = rd2_img_init(img_map_width, img_map_height);

			rd2_img_fill(&img_map, 0, 0, img_map_width, img_map_height, ARGB32(255, 0, 255, 255));

			{
				int32 tile_x = 16;
				int32 tile_y = (enums_imgs.count / 16 + 1);

				for(int32 y = 0; y < tile_y; y++)
					for(int32 x = 0; x < tile_x; x++)
						rd2_img_fill(&img_map, 1 + x * (i_image_width + 1), 1 + y * (i_image_height + 1), i_image_width, i_image_height, ARGB32(255, 255, 255, 0));
			}

			int32 img_x = 1, img_y = 1;

			for(int32 i = 0; i < enums_imgs.count; i++)
			{
				if(i != 0 && (i % 16) == 0)
				{
					img_x = 1;
					img_y += i_image_height + 1;
				}

				RD2IMG img = enums_imgs.values[i];

				rd2_img_blit(&img_map, &img, img_x, img_y, 0, 0, img.width, img.height); // blit with transparent cut on A less than 8

				img_x += i_image_width + 1;
			}

			if(rd2c_ori.count && rd2c_rep.count)
				rd2_img_color_replace(&img_map, rd2c_ori.values, rd2c_rep.values, rd2c_ori.count);

			if(rd2c_pal.count)
				rd2_img_color_match_replace(&img_map, rd2c_pal.values, rd2c_pal.count);

			{
				Image img = GenImageColor(img_map.width, img_map.height, (Color) { 0, 0, 0, 0 });
				ImageFormat(&img, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8);

				rd2_raw_img_copy(&img_map, img.data, img.width * 4, img.width, img.height);

				bool res = ExportImage(img, txt_format("%s/%s.png", i_dst_dir, i_imgname));

				UnloadImage(img);
			}
		}
	}
	//--------------------------------------------------------------------------------------
	// De-Initialization
	//--------------------------------------------------------------------------------------
	hal_system_shutdown();
	//--------------------------------------------------------------------------------------
	return 0;
}
//--------------------------------------------------------------------------------------
