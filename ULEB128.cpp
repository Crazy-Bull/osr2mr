static void ULEB128Encode(unsigned long long val, unsigned char *s) {
    unsigned char c;
    bool flag;
    do {
        c = val & 0x7f;
        val >>= 7;
        flag = val != 0;
        *s++ = c | (flag ? 0x80 : 0);
    } while (flag);
}
static unsigned long long ULEB128Decode(unsigned char *s) 
{
    unsigned long long val = 0;
    int shift = 0;
    bool flag;
    unsigned char c;
    do {
        c = *s++;
        val |= (c & 0x7f) << shift;
        shift += 7;
        flag = c & 0x80;
    } while (flag);
    
    return val;
}
