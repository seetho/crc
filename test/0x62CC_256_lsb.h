// 256-element lsb-1st LUT for generator polynomial 0x62CC
uint8_t lut256_lsb[][2] = {
    {0x00,0x00},{0x22,0x64},{0xE7,0x51},{0xC5,0x35},{0x6D,0x3A},{0x4F,0x5E},{0x8A,0x6B},{0xA8,0x0F},
    {0xDA,0x74},{0xF8,0x10},{0x3D,0x25},{0x1F,0x41},{0xB7,0x4E},{0x95,0x2A},{0x50,0x1F},{0x72,0x7B},
    {0x17,0x70},{0x35,0x14},{0xF0,0x21},{0xD2,0x45},{0x7A,0x4A},{0x58,0x2E},{0x9D,0x1B},{0xBF,0x7F},
    {0xCD,0x04},{0xEF,0x60},{0x2A,0x55},{0x08,0x31},{0xA0,0x3E},{0x82,0x5A},{0x47,0x6F},{0x65,0x0B},
    {0x8D,0x79},{0xAF,0x1D},{0x6A,0x28},{0x48,0x4C},{0xE0,0x43},{0xC2,0x27},{0x07,0x12},{0x25,0x76},
    {0x57,0x0D},{0x75,0x69},{0xB0,0x5C},{0x92,0x38},{0x3A,0x37},{0x18,0x53},{0xDD,0x66},{0xFF,0x02},
    {0x9A,0x09},{0xB8,0x6D},{0x7D,0x58},{0x5F,0x3C},{0xF7,0x33},{0xD5,0x57},{0x10,0x62},{0x32,0x06},
    {0x40,0x7D},{0x62,0x19},{0xA7,0x2C},{0x85,0x48},{0x2D,0x47},{0x0F,0x23},{0xCA,0x16},{0xE8,0x72},
    {0xB9,0x6A},{0x9B,0x0E},{0x5E,0x3B},{0x7C,0x5F},{0xD4,0x50},{0xF6,0x34},{0x33,0x01},{0x11,0x65},
    {0x63,0x1E},{0x41,0x7A},{0x84,0x4F},{0xA6,0x2B},{0x0E,0x24},{0x2C,0x40},{0xE9,0x75},{0xCB,0x11},
    {0xAE,0x1A},{0x8C,0x7E},{0x49,0x4B},{0x6B,0x2F},{0xC3,0x20},{0xE1,0x44},{0x24,0x71},{0x06,0x15},
    {0x74,0x6E},{0x56,0x0A},{0x93,0x3F},{0xB1,0x5B},{0x19,0x54},{0x3B,0x30},{0xFE,0x05},{0xDC,0x61},
    {0x34,0x13},{0x16,0x77},{0xD3,0x42},{0xF1,0x26},{0x59,0x29},{0x7B,0x4D},{0xBE,0x78},{0x9C,0x1C},
    {0xEE,0x67},{0xCC,0x03},{0x09,0x36},{0x2B,0x52},{0x83,0x5D},{0xA1,0x39},{0x64,0x0C},{0x46,0x68},
    {0x23,0x63},{0x01,0x07},{0xC4,0x32},{0xE6,0x56},{0x4E,0x59},{0x6C,0x3D},{0xA9,0x08},{0x8B,0x6C},
    {0xF9,0x17},{0xDB,0x73},{0x1E,0x46},{0x3C,0x22},{0x94,0x2D},{0xB6,0x49},{0x73,0x7C},{0x51,0x18},
    {0xD1,0x4C},{0xF3,0x28},{0x36,0x1D},{0x14,0x79},{0xBC,0x76},{0x9E,0x12},{0x5B,0x27},{0x79,0x43},
    {0x0B,0x38},{0x29,0x5C},{0xEC,0x69},{0xCE,0x0D},{0x66,0x02},{0x44,0x66},{0x81,0x53},{0xA3,0x37},
    {0xC6,0x3C},{0xE4,0x58},{0x21,0x6D},{0x03,0x09},{0xAB,0x06},{0x89,0x62},{0x4C,0x57},{0x6E,0x33},
    {0x1C,0x48},{0x3E,0x2C},{0xFB,0x19},{0xD9,0x7D},{0x71,0x72},{0x53,0x16},{0x96,0x23},{0xB4,0x47},
    {0x5C,0x35},{0x7E,0x51},{0xBB,0x64},{0x99,0x00},{0x31,0x0F},{0x13,0x6B},{0xD6,0x5E},{0xF4,0x3A},
    {0x86,0x41},{0xA4,0x25},{0x61,0x10},{0x43,0x74},{0xEB,0x7B},{0xC9,0x1F},{0x0C,0x2A},{0x2E,0x4E},
    {0x4B,0x45},{0x69,0x21},{0xAC,0x14},{0x8E,0x70},{0x26,0x7F},{0x04,0x1B},{0xC1,0x2E},{0xE3,0x4A},
    {0x91,0x31},{0xB3,0x55},{0x76,0x60},{0x54,0x04},{0xFC,0x0B},{0xDE,0x6F},{0x1B,0x5A},{0x39,0x3E},
    {0x68,0x26},{0x4A,0x42},{0x8F,0x77},{0xAD,0x13},{0x05,0x1C},{0x27,0x78},{0xE2,0x4D},{0xC0,0x29},
    {0xB2,0x52},{0x90,0x36},{0x55,0x03},{0x77,0x67},{0xDF,0x68},{0xFD,0x0C},{0x38,0x39},{0x1A,0x5D},
    {0x7F,0x56},{0x5D,0x32},{0x98,0x07},{0xBA,0x63},{0x12,0x6C},{0x30,0x08},{0xF5,0x3D},{0xD7,0x59},
    {0xA5,0x22},{0x87,0x46},{0x42,0x73},{0x60,0x17},{0xC8,0x18},{0xEA,0x7C},{0x2F,0x49},{0x0D,0x2D},
    {0xE5,0x5F},{0xC7,0x3B},{0x02,0x0E},{0x20,0x6A},{0x88,0x65},{0xAA,0x01},{0x6F,0x34},{0x4D,0x50},
    {0x3F,0x2B},{0x1D,0x4F},{0xD8,0x7A},{0xFA,0x1E},{0x52,0x11},{0x70,0x75},{0xB5,0x40},{0x97,0x24},
    {0xF2,0x2F},{0xD0,0x4B},{0x15,0x7E},{0x37,0x1A},{0x9F,0x15},{0xBD,0x71},{0x78,0x44},{0x5A,0x20},
    {0x28,0x5B},{0x0A,0x3F},{0xCF,0x0A},{0xED,0x6E},{0x45,0x61},{0x67,0x05},{0xA2,0x30},{0x80,0x54}
};