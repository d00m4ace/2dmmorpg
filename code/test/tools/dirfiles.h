//--------------------------------------------------------------------------------------
// File path list
typedef struct FilePathList
{
	unsigned int capacity;          // Filepaths max entries
	unsigned int count;             // Filepaths entries count
	char** paths;                   // Filepaths entries
} FilePathList;

// Files management functions
unsigned char* LoadFileData(const char* fileName, unsigned int* bytesRead);       // Load file data as byte array (read)
void UnloadFileData(unsigned char* data);                   // Unload file data allocated by LoadFileData()
bool SaveFileData(const char* fileName, void* data, unsigned int bytesToWrite);   // Save data to file from byte array (write), returns true on success
bool ExportDataAsCode(const char* data, unsigned int size, const char* fileName); // Export data to code (.h), returns true on success
char* LoadFileText(const char* fileName);                   // Load text data from file (read), returns a '\0' terminated string
void UnloadFileText(char* text);                            // Unload file text data allocated by LoadFileText()
bool SaveFileText(const char* fileName, char* text);        // Save text data to file (write), string must be '\0' terminated, returns true on success
bool FileExists(const char* fileName);                      // Check if file exists
bool DirectoryExists(const char* dirPath);                  // Check if a directory path exists
bool IsFileExtension(const char* fileName, const char* ext); // Check file extension (including point: .png, .wav)
int GetFileLength(const char* fileName);                    // Get file length in bytes (NOTE: GetFileSize() conflicts with windows.h)
const char* GetFileExtension(const char* fileName);         // Get pointer to extension for a filename string (includes dot: '.png')
const char* GetFileName(const char* filePath);              // Get pointer to filename for a path string
const char* GetFileNameWithoutExt(const char* filePath);    // Get filename string without extension (uses static string)
const char* GetDirectoryPath(const char* filePath);         // Get full path for a given fileName with path (uses static string)
const char* GetPrevDirectoryPath(const char* dirPath);      // Get previous directory path for a given path (uses static string)
const char* GetWorkingDirectory(void);                      // Get current working directory (uses static string)
const char* GetApplicationDirectory(void);                  // Get the directory if the running application (uses static string)
bool ChangeDirectory(const char* dir);                      // Change working directory, return true on success
bool IsPathFile(const char* path);                          // Check if a given path is a file or a directory
FilePathList LoadDirectoryFiles(const char* dirPath);       // Load directory filepaths
FilePathList LoadDirectoryFilesEx(const char* basePath, const char* filter, bool scanSubdirs); // Load directory filepaths with extension filtering and recursive directory scan
void UnloadDirectoryFiles(FilePathList files);              // Unload filepaths
bool IsFileDropped(void);                                   // Check if a file has been dropped into window
FilePathList LoadDroppedFiles(void);                        // Load dropped filepaths
void UnloadDroppedFiles(FilePathList files);                // Unload dropped filepaths
long GetFileModTime(const char* fileName);                  // Get file modification time (last write time)
//--------------------------------------------------------------------------------------

// Pixel formats
// NOTE: Support depends on OpenGL version and platform
typedef enum
{
    PIXELFORMAT_UNCOMPRESSED_GRAYSCALE = 1, // 8 bit per pixel (no alpha)
    PIXELFORMAT_UNCOMPRESSED_GRAY_ALPHA,    // 8*2 bpp (2 channels)
    PIXELFORMAT_UNCOMPRESSED_R5G6B5,        // 16 bpp
    PIXELFORMAT_UNCOMPRESSED_R8G8B8,        // 24 bpp
    PIXELFORMAT_UNCOMPRESSED_R5G5B5A1,      // 16 bpp (1 bit alpha)
    PIXELFORMAT_UNCOMPRESSED_R4G4B4A4,      // 16 bpp (4 bit alpha)
    PIXELFORMAT_UNCOMPRESSED_R8G8B8A8,      // 32 bpp
    PIXELFORMAT_UNCOMPRESSED_R32,           // 32 bpp (1 channel - float)
    PIXELFORMAT_UNCOMPRESSED_R32G32B32,     // 32*3 bpp (3 channels - float)
    PIXELFORMAT_UNCOMPRESSED_R32G32B32A32,  // 32*4 bpp (4 channels - float)
    PIXELFORMAT_COMPRESSED_DXT1_RGB,        // 4 bpp (no alpha)
    PIXELFORMAT_COMPRESSED_DXT1_RGBA,       // 4 bpp (1 bit alpha)
    PIXELFORMAT_COMPRESSED_DXT3_RGBA,       // 8 bpp
    PIXELFORMAT_COMPRESSED_DXT5_RGBA,       // 8 bpp
    PIXELFORMAT_COMPRESSED_ETC1_RGB,        // 4 bpp
    PIXELFORMAT_COMPRESSED_ETC2_RGB,        // 4 bpp
    PIXELFORMAT_COMPRESSED_ETC2_EAC_RGBA,   // 8 bpp
    PIXELFORMAT_COMPRESSED_PVRT_RGB,        // 4 bpp
    PIXELFORMAT_COMPRESSED_PVRT_RGBA,       // 4 bpp
    PIXELFORMAT_COMPRESSED_ASTC_4x4_RGBA,   // 8 bpp
    PIXELFORMAT_COMPRESSED_ASTC_8x8_RGBA    // 2 bpp
} PixelFormat;

// Image, pixel data stored in CPU memory (RAM)
typedef struct Image
{
    void* data;             // Image raw data
    int width;              // Image base width
    int height;             // Image base height
    int mipmaps;            // Mipmap levels, 1 by default
    int format;             // Data format (PixelFormat type)
} Image;

// Color, 4 components, R8G8B8A8 (32bit)
typedef struct Color
{
    unsigned char r;        // Color red value
    unsigned char g;        // Color green value
    unsigned char b;        // Color blue value
    unsigned char a;        // Color alpha value
} Color;

Image LoadImage(const char* fileName);
void UnloadImage(Image image);

void ImageFormat(Image* image, int newFormat);

bool ExportImage(Image image, const char* fileName);

Image GenImageColor(int width, int height, Color color);