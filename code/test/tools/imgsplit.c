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

static int32 i_tile;
static int32 i_image_width, i_image_height;
//--------------------------------------------------------------------------------------
int main(int argc, char** argv)
{
	//--------------------------------------------------------------------------------------
	// Initialize 
	//--------------------------------------------------------------------------------------
	hal_system_init();
	//--------------------------------------------------------------------------------------
	RD2IMG splt_img;

	{
		Image img = LoadImage("resources/test/chars/chars0.png");
		ImageFormat(&img, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8);
		splt_img = rd2_img_init(img.width, img.height);
		rd2_img_raw_copy(&splt_img, img.data, img.width * 4, img.width, img.height);
		UnloadImage(img);
	}

	int32 tile_size = 24;

	int32 tile_x = splt_img.width / tile_size;
	int32 tile_y = splt_img.height / tile_size;

	int32 img_c = 0;
	int32 img_c_start = 140;
	int32 img_c_end = 170;

	for(int32 y = 0; y < tile_y; y++)
		for(int32 x = 0; x < tile_x; x++)
		{
			RD2IMG img = rd2_img_init(tile_size, tile_size);
			rd2_img_fill(&img, 0, 0, tile_size, tile_size, ARGB32(255, 255, 255, 0));
			rd2_img_blit(&img, &splt_img, 0, 0, x * tile_size, y * tile_size, tile_size, tile_size);

			if(img_c >= img_c_start && img_c < img_c_end)
			{
				Image e_img = GenImageColor(img.width, img.height, (Color) { 0, 0, 0, 0 });
				ImageFormat(&e_img, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8);

				rd2_raw_img_copy(&img, e_img.data, img.width * 4, img.width, img.height);

				bool res = ExportImage(e_img, txt_format("output/chars/%03d.png", img_c - img_c_start));

				UnloadImage(e_img);
			}

			img_c++;
		}

	//--------------------------------------------------------------------------------------
	// De-Initialization
	//--------------------------------------------------------------------------------------
	hal_system_shutdown();
	//--------------------------------------------------------------------------------------
	return 0;
}
//--------------------------------------------------------------------------------------
