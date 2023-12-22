#define MAX_SALT_LENGTH 16
#define HASH_ITERATIONS 50000

unsigned long long basic_hash(const char* str)
{
    unsigned long long hash = 5381;
    int c;

    while((c = *str++))
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash;
}

void generate_salt(char* salt, size_t length)
{
    const char* chars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    size_t chars_len = strlen(chars);
    srand((unsigned int)time(NULL));

    for(size_t i = 0; i < length; ++i)
    {
        salt[i] = chars[rand() % chars_len];
    }

    salt[length] = '\0';
}

void hmac_hash(const char* key, const char* data, char* output)
{
    char key_ipad[64] = { 0 };
    char key_opad[64] = { 0 };
    char temp[256];
    unsigned long long hash;

    strncpy(key_ipad, key, 64);
    strncpy(key_opad, key, 64);

    for(int i = 0; i < 64; ++i)
    {
        key_ipad[i] ^= 0x36;
        key_opad[i] ^= 0x5c;
    }

    snprintf(temp, sizeof(temp), "%s%s", key_ipad, data);
    hash = basic_hash(temp);

    snprintf(temp, sizeof(temp), "%s%lx", key_opad, hash);
    hash = basic_hash(temp);

    sprintf(output, "%llx", hash);
}

void hash_password(const char* password, const char* salt, char* output)
{
    char salted_password[256];
    char hashed[256];

    snprintf(salted_password, sizeof(salted_password), "%s%s", password, salt);
    hmac_hash(salted_password, password, hashed);

    for(int i = 0; i < HASH_ITERATIONS; ++i)
    {
        hmac_hash(hashed, password, hashed);
    }

    strcpy(output, hashed);
}