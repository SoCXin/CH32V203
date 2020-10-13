#define RAM_CODE_START_ADDR     0x20000400
#define RAM_CODE_LENGTH         2496

const unsigned char RAM_CODE[RAM_CODE_LENGTH] =
{
    0x68, 0x1A, 0x00, 0x20, 0xB5, 0x04, 0x00, 0x20, 0x00, 0xF0, 0x02, 0xF8, 0x00, 0xF0, 0x3E, 0xF8,
    0x0C, 0xA0, 0x30, 0xC8, 0x08, 0x38, 0x24, 0x18, 0x2D, 0x18, 0xA2, 0x46, 0x67, 0x1E, 0xAB, 0x46,
    0x54, 0x46, 0x5D, 0x46, 0xAC, 0x42, 0x01, 0xD1, 0x00, 0xF0, 0x30, 0xF8, 0x7E, 0x46, 0x0F, 0x3E,
    0x0F, 0xCC, 0xB6, 0x46, 0x01, 0x26, 0x33, 0x42, 0x00, 0xD0, 0xFB, 0x1A, 0xA2, 0x46, 0xAB, 0x46,
    0x33, 0x43, 0x18, 0x47, 0x54, 0x09, 0x00, 0x00, 0x74, 0x09, 0x00, 0x00, 0x10, 0x3A, 0x02, 0xD3,
    0x78, 0xC8, 0x78, 0xC1, 0xFA, 0xD8, 0x52, 0x07, 0x01, 0xD3, 0x30, 0xC8, 0x30, 0xC1, 0x01, 0xD5,
    0x04, 0x68, 0x0C, 0x60, 0x70, 0x47, 0x00, 0x00, 0x00, 0x23, 0x00, 0x24, 0x00, 0x25, 0x00, 0x26,
    0x10, 0x3A, 0x01, 0xD3, 0x78, 0xC1, 0xFB, 0xD8, 0x52, 0x07, 0x00, 0xD3, 0x30, 0xC1, 0x00, 0xD5,
    0x0B, 0x60, 0x70, 0x47, 0x1F, 0xB5, 0x1F, 0xBD, 0x10, 0xB5, 0x10, 0xBD, 0x00, 0xF0, 0xF0, 0xF8,
    0x11, 0x46, 0xFF, 0xF7, 0xF7, 0xFF, 0x00, 0xF0, 0xAF, 0xFA, 0x00, 0xF0, 0x08, 0xF9, 0x03, 0xB4,
    0xFF, 0xF7, 0xF2, 0xFF, 0x03, 0xBC, 0x00, 0xF0, 0x0F, 0xF9, 0x00, 0x00, 0x80, 0xF3, 0x08, 0x88,
    0x70, 0x47, 0x00, 0x00, 0x0B, 0x48, 0x80, 0xF3, 0x08, 0x88, 0x0B, 0x48, 0x01, 0x68, 0x09, 0x0E,
    0x0A, 0x4A, 0x91, 0x42, 0x05, 0xD1, 0x0A, 0x48, 0x0A, 0x49, 0x01, 0x60, 0x0A, 0x48, 0x0B, 0x49,
    0x01, 0x60, 0x0B, 0x48, 0x00, 0x47, 0x00, 0x00, 0x0A, 0x48, 0x0B, 0x49, 0x09, 0x4A, 0x0B, 0x4B,
    0x70, 0x47, 0x00, 0x00, 0x68, 0x1A, 0x00, 0x20, 0x04, 0x00, 0x00, 0x00, 0x1F, 0x00, 0x00, 0x00,
    0x18, 0x10, 0x02, 0x40, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x40, 0x00, 0x00, 0x00, 0x00,
    0x09, 0x04, 0x00, 0x20, 0x68, 0x19, 0x00, 0x20, 0x68, 0x1A, 0x00, 0x20, 0x68, 0x19, 0x00, 0x20,
    0x00, 0x22, 0x03, 0x09, 0x8B, 0x42, 0x2C, 0xD3, 0x03, 0x0A, 0x8B, 0x42, 0x11, 0xD3, 0x00, 0x23,
    0x9C, 0x46, 0x4E, 0xE0, 0x03, 0x46, 0x0B, 0x43, 0x3C, 0xD4, 0x00, 0x22, 0x43, 0x08, 0x8B, 0x42,
    0x31, 0xD3, 0x03, 0x09, 0x8B, 0x42, 0x1C, 0xD3, 0x03, 0x0A, 0x8B, 0x42, 0x01, 0xD3, 0x94, 0x46,
    0x3F, 0xE0, 0xC3, 0x09, 0x8B, 0x42, 0x01, 0xD3, 0xCB, 0x01, 0xC0, 0x1A, 0x52, 0x41, 0x83, 0x09,
    0x8B, 0x42, 0x01, 0xD3, 0x8B, 0x01, 0xC0, 0x1A, 0x52, 0x41, 0x43, 0x09, 0x8B, 0x42, 0x01, 0xD3,
    0x4B, 0x01, 0xC0, 0x1A, 0x52, 0x41, 0x03, 0x09, 0x8B, 0x42, 0x01, 0xD3, 0x0B, 0x01, 0xC0, 0x1A,
    0x52, 0x41, 0xC3, 0x08, 0x8B, 0x42, 0x01, 0xD3, 0xCB, 0x00, 0xC0, 0x1A, 0x52, 0x41, 0x83, 0x08,
    0x8B, 0x42, 0x01, 0xD3, 0x8B, 0x00, 0xC0, 0x1A, 0x52, 0x41, 0x43, 0x08, 0x8B, 0x42, 0x01, 0xD3,
    0x4B, 0x00, 0xC0, 0x1A, 0x52, 0x41, 0x41, 0x1A, 0x00, 0xD2, 0x01, 0x46, 0x52, 0x41, 0x10, 0x46,
    0x70, 0x47, 0x5D, 0xE0, 0xCA, 0x0F, 0x00, 0xD0, 0x49, 0x42, 0x03, 0x10, 0x00, 0xD3, 0x40, 0x42,
    0x53, 0x40, 0x00, 0x22, 0x9C, 0x46, 0x03, 0x09, 0x8B, 0x42, 0x2D, 0xD3, 0x03, 0x0A, 0x8B, 0x42,
    0x12, 0xD3, 0xFC, 0x22, 0x89, 0x01, 0x12, 0xBA, 0x03, 0x0A, 0x8B, 0x42, 0x0C, 0xD3, 0x89, 0x01,
    0x92, 0x11, 0x8B, 0x42, 0x08, 0xD3, 0x89, 0x01, 0x92, 0x11, 0x8B, 0x42, 0x04, 0xD3, 0x89, 0x01,
    0x3A, 0xD0, 0x92, 0x11, 0x00, 0xE0, 0x89, 0x09, 0xC3, 0x09, 0x8B, 0x42, 0x01, 0xD3, 0xCB, 0x01,
    0xC0, 0x1A, 0x52, 0x41, 0x83, 0x09, 0x8B, 0x42, 0x01, 0xD3, 0x8B, 0x01, 0xC0, 0x1A, 0x52, 0x41,
    0x43, 0x09, 0x8B, 0x42, 0x01, 0xD3, 0x4B, 0x01, 0xC0, 0x1A, 0x52, 0x41, 0x03, 0x09, 0x8B, 0x42,
    0x01, 0xD3, 0x0B, 0x01, 0xC0, 0x1A, 0x52, 0x41, 0xC3, 0x08, 0x8B, 0x42, 0x01, 0xD3, 0xCB, 0x00,
    0xC0, 0x1A, 0x52, 0x41, 0x83, 0x08, 0x8B, 0x42, 0x01, 0xD3, 0x8B, 0x00, 0xC0, 0x1A, 0x52, 0x41,
    0xD9, 0xD2, 0x43, 0x08, 0x8B, 0x42, 0x01, 0xD3, 0x4B, 0x00, 0xC0, 0x1A, 0x52, 0x41, 0x41, 0x1A,
    0x00, 0xD2, 0x01, 0x46, 0x63, 0x46, 0x52, 0x41, 0x5B, 0x10, 0x10, 0x46, 0x01, 0xD3, 0x40, 0x42,
    0x00, 0x2B, 0x00, 0xD5, 0x49, 0x42, 0x70, 0x47, 0x63, 0x46, 0x5B, 0x10, 0x00, 0xD3, 0x40, 0x42,
    0x01, 0xB5, 0x00, 0x20, 0xC0, 0x46, 0xC0, 0x46, 0x02, 0xBD, 0x70, 0x47, 0x70, 0x47, 0x70, 0x47,
    0x75, 0x46, 0x00, 0xF0, 0x25, 0xF8, 0xAE, 0x46, 0x05, 0x00, 0x69, 0x46, 0x53, 0x46, 0xC0, 0x08,
    0xC0, 0x00, 0x85, 0x46, 0x18, 0xB0, 0x20, 0xB5, 0xFF, 0xF7, 0x26, 0xFF, 0x60, 0xBC, 0x00, 0x27,
    0x49, 0x08, 0xB6, 0x46, 0x00, 0x26, 0xC0, 0xC5, 0xC0, 0xC5, 0xC0, 0xC5, 0xC0, 0xC5, 0xC0, 0xC5,
    0xC0, 0xC5, 0xC0, 0xC5, 0xC0, 0xC5, 0x40, 0x3D, 0x49, 0x00, 0x8D, 0x46, 0x70, 0x47, 0x10, 0xB5,
    0x04, 0x46, 0xC0, 0x46, 0xC0, 0x46, 0x20, 0x46, 0xFF, 0xF7, 0xF1, 0xFE, 0x10, 0xBD, 0x00, 0x00,
    0x00, 0x48, 0x70, 0x47, 0x08, 0x19, 0x00, 0x20, 0x01, 0x49, 0x18, 0x20, 0xAB, 0xBE, 0xFE, 0xE7,
    0x26, 0x00, 0x02, 0x00, 0x70, 0x47, 0x00, 0x00, 0x10, 0xB5, 0x00, 0xF0, 0x55, 0xF8, 0x0E, 0x48,
    0xC1, 0x68, 0x14, 0x23, 0x19, 0x43, 0xC1, 0x60, 0x01, 0x69, 0x04, 0x22, 0x11, 0x43, 0x01, 0x61,
    0x01, 0x69, 0x40, 0x24, 0x21, 0x43, 0x01, 0x61, 0xC1, 0x68, 0xC9, 0x07, 0xFC, 0xD1, 0x01, 0x69,
    0x91, 0x43, 0x01, 0x61, 0xC1, 0x68, 0x19, 0x42, 0x04, 0xD0, 0xC1, 0x68, 0x19, 0x43, 0xC1, 0x60,
    0x01, 0x20, 0x10, 0xBD, 0x00, 0x20, 0x10, 0xBD, 0x00, 0x20, 0x02, 0x40, 0x10, 0xB5, 0x03, 0x46,
    0x00, 0xF0, 0x32, 0xF8, 0x17, 0x49, 0xC8, 0x68, 0x14, 0x22, 0x10, 0x43, 0xC8, 0x60, 0x4B, 0x61,
    0x1C, 0x0E, 0x40, 0x20, 0x08, 0x2C, 0x19, 0xD0, 0x1B, 0x0D, 0xDC, 0x1F, 0xFF, 0x3C, 0xF9, 0x3C,
    0x12, 0xD1, 0x0C, 0x69, 0x20, 0x23, 0x1C, 0x43, 0x0C, 0x61, 0x0C, 0x69, 0x04, 0x43, 0x0C, 0x61,
    0xC8, 0x68, 0xC0, 0x07, 0xFC, 0xD1, 0x08, 0x69, 0x98, 0x43, 0x08, 0x61, 0xC8, 0x68, 0x10, 0x40,
    0x03, 0xD0, 0xC8, 0x68, 0x10, 0x43, 0xC8, 0x60, 0x01, 0x20, 0x10, 0xBD, 0x0C, 0x69, 0x02, 0x23,
    0x1C, 0x43, 0x0C, 0x61, 0x0C, 0x69, 0x04, 0x43, 0x0C, 0x61, 0xC8, 0x68, 0xC0, 0x07, 0xFC, 0xD1,
    0xE9, 0xE7, 0x00, 0x00, 0x00, 0x20, 0x02, 0x40, 0x04, 0x48, 0x03, 0x49, 0x41, 0x60, 0x04, 0x4A,
    0x42, 0x60, 0x81, 0x60, 0x82, 0x60, 0x70, 0x47, 0x23, 0x01, 0x67, 0x45, 0x00, 0x20, 0x02, 0x40,
    0xAB, 0x89, 0xEF, 0xCD, 0xBF, 0xF3, 0x4F, 0x8F, 0x03, 0x49, 0x02, 0x48, 0xC8, 0x60, 0xBF, 0xF3,
    0x4F, 0x8F, 0xFE, 0xE7, 0x04, 0x00, 0xFA, 0x05, 0x00, 0xED, 0x00, 0xE0, 0xF0, 0xB5, 0x14, 0x46,
    0x0D, 0x46, 0x03, 0x46, 0xFF, 0xF7, 0xE0, 0xFF, 0x1B, 0x48, 0xC1, 0x68, 0x04, 0x22, 0x11, 0x43,
    0xC1, 0x60, 0x10, 0x21, 0x14, 0x26, 0x2A, 0xE0, 0x1A, 0x0E, 0x08, 0x2A, 0x17, 0xD0, 0x1A, 0x0D,
    0xD7, 0x1F, 0xFF, 0x3F, 0xF9, 0x3F, 0x0A, 0xD1, 0x02, 0x69, 0x0A, 0x43, 0x02, 0x61, 0x22, 0x88,
    0x1A, 0x80, 0xC2, 0x68, 0xD2, 0x07, 0xFC, 0xD1, 0x02, 0x69, 0x8A, 0x43, 0x02, 0x61, 0xC2, 0x68,
    0x32, 0x42, 0x11, 0xD0, 0xC1, 0x68, 0x31, 0x43, 0xC1, 0x60, 0x01, 0x20, 0xF0, 0xBD, 0x02, 0x69,
    0x01, 0x27, 0x3A, 0x43, 0x02, 0x61, 0x22, 0x88, 0x1A, 0x80, 0xC2, 0x68, 0xD2, 0x07, 0xFC, 0xD1,
    0x02, 0x69, 0x52, 0x08, 0x52, 0x00, 0xE9, 0xE7, 0x9B, 0x1C, 0xA4, 0x1C, 0xAD, 0x1E, 0x00, 0x2D,
    0xD2, 0xD1, 0x00, 0x20, 0xF0, 0xBD, 0x00, 0x00, 0x00, 0x20, 0x02, 0x40, 0x03, 0x48, 0x81, 0x68,
    0x89, 0x07, 0xFC, 0xD5, 0x40, 0x68, 0xC0, 0xB2, 0x70, 0x47, 0x00, 0x00, 0x00, 0x38, 0x01, 0x40,
    0x02, 0x49, 0x08, 0x60, 0x88, 0x68, 0xC0, 0x07, 0xFC, 0xD0, 0x70, 0x47, 0x00, 0x38, 0x01, 0x40,
    0xF8, 0xB5, 0x09, 0x21, 0xC9, 0x06, 0x4B, 0x68, 0xFF, 0x22, 0x12, 0x01, 0x93, 0x43, 0x11, 0x22,
    0xD2, 0x01, 0x9A, 0x18, 0x4A, 0x60, 0xCA, 0x68, 0x03, 0x23, 0x5B, 0x02, 0x1A, 0x43, 0xCA, 0x60,
    0x11, 0x4C, 0xA1, 0x69, 0x4D, 0x08, 0x01, 0x01, 0x6D, 0x00, 0x0F, 0x46, 0x0F, 0x48, 0xA5, 0x61,
    0xFF, 0xF7, 0x3E, 0xFE, 0x0A, 0x21, 0xFF, 0xF7, 0x3B, 0xFE, 0x0E, 0x46, 0x0C, 0x48, 0x39, 0x46,
    0xFF, 0xF7, 0x36, 0xFE, 0x05, 0x2E, 0x00, 0xD9, 0x40, 0x1C, 0x20, 0x62, 0x01, 0x20, 0x05, 0x43,
    0x09, 0x20, 0xC0, 0x06, 0xA5, 0x61, 0x41, 0x68, 0xFF, 0x22, 0x12, 0x01, 0x91, 0x43, 0x8B, 0x22,
    0x12, 0x01, 0x89, 0x18, 0x41, 0x60, 0xF8, 0xBD, 0x00, 0x38, 0x01, 0x40, 0x00, 0x70, 0x38, 0x39,
    0x00, 0xD8, 0xB8, 0x05, 0x10, 0xB5, 0x09, 0x20, 0xC0, 0x06, 0x81, 0x68, 0x49, 0x05, 0xFC, 0xD4,
    0x81, 0x68, 0x49, 0x05, 0xFC, 0xD5, 0x0A, 0x4A, 0x08, 0x4C, 0x54, 0x61, 0x00, 0x23, 0x93, 0x61,
    0x07, 0x21, 0x11, 0x61, 0x81, 0x68, 0x49, 0x05, 0xFC, 0xD4, 0x90, 0x69, 0x93, 0x61, 0x21, 0x1A,
    0x13, 0x61, 0x04, 0x48, 0xFF, 0xF7, 0x04, 0xFE, 0x10, 0xBD, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x00,
    0x00, 0xE0, 0x00, 0xE0, 0x00, 0xE8, 0x0D, 0x28, 0x08, 0xB5, 0x64, 0x20, 0x00, 0x90, 0x41, 0x1E,
    0x00, 0x91, 0x01, 0xD3, 0x08, 0x46, 0xFA, 0xE7, 0x08, 0xBD, 0x00, 0x00, 0x70, 0xB5, 0x24, 0x4C,
    0x00, 0x25, 0x20, 0x68, 0x40, 0x02, 0x41, 0x0F, 0x20, 0x68, 0x49, 0x1C, 0x80, 0x0E, 0x40, 0x1C,
    0xFF, 0xF7, 0xE6, 0xFD, 0x1F, 0x49, 0x08, 0x70, 0x20, 0x68, 0x01, 0x21, 0x08, 0x43, 0x20, 0x60,
    0x20, 0x68, 0x80, 0x07, 0xFC, 0xD5, 0x01, 0x20, 0x80, 0x02, 0x60, 0x60, 0x60, 0x68, 0x01, 0x21,
    0x09, 0x04, 0x88, 0x43, 0x60, 0x60, 0x20, 0x68, 0x09, 0x02, 0x88, 0x43, 0x20, 0x60, 0x20, 0x68,
    0x1F, 0x22, 0x92, 0x06, 0x90, 0x43, 0x20, 0x60, 0x20, 0x68, 0x07, 0x22, 0x92, 0x06, 0x10, 0x43,
    0x20, 0x60, 0x11, 0x48, 0x02, 0x68, 0xF8, 0x23, 0x1A, 0x40, 0x02, 0x60, 0x02, 0x68, 0x03, 0x23,
    0x1A, 0x43, 0x02, 0x60, 0x20, 0x68, 0x08, 0x43, 0x20, 0x60, 0x20, 0x68, 0x80, 0x01, 0xFC, 0xD5,
    0x60, 0x68, 0x80, 0x08, 0x80, 0x00, 0x60, 0x60, 0x60, 0x68, 0x02, 0x21, 0x08, 0x43, 0x60, 0x60,
    0x02, 0xE0, 0x60, 0x68, 0x00, 0x07, 0x85, 0x0F, 0x02, 0x2D, 0xFA, 0xD1, 0x70, 0xBD, 0x00, 0x00,
    0x00, 0x10, 0x02, 0x40, 0x00, 0x18, 0x00, 0x20, 0x00, 0x20, 0x02, 0x40, 0x10, 0xB5, 0x08, 0x4A,
    0x01, 0x68, 0x11, 0x40, 0x01, 0x22, 0x52, 0x07, 0x91, 0x42, 0x07, 0xD1, 0x41, 0x68, 0x05, 0x4C,
    0x61, 0x60, 0x00, 0x68, 0xFF, 0xF7, 0x62, 0xFD, 0x60, 0x68, 0x80, 0x47, 0x10, 0xBD, 0x00, 0x00,
    0x00, 0x00, 0xFE, 0x2F, 0x00, 0x18, 0x00, 0x20, 0x08, 0xB5, 0xFF, 0xF7, 0x97, 0xFF, 0xDF, 0x49,
    0x88, 0x69, 0x01, 0x22, 0x92, 0x03, 0x10, 0x43, 0x88, 0x61, 0xFF, 0xF7, 0x63, 0xFF, 0xFF, 0xF7,
    0x27, 0xFF, 0xDB, 0x49, 0xC8, 0x69, 0x03, 0x22, 0x10, 0x43, 0xC8, 0x61, 0xFF, 0xF7, 0x7C, 0xFF,
    0x79, 0x20, 0xFF, 0xF7, 0x15, 0xFF, 0xD7, 0x48, 0xD7, 0x4D, 0x00, 0x78, 0xC0, 0x43, 0xC1, 0xB2,
    0x01, 0x20, 0x00, 0x29, 0x00, 0xD0, 0x68, 0x70, 0xFF, 0xF7, 0x00, 0xFF, 0x31, 0x28, 0x73, 0xD0,
    0x0C, 0xDC, 0x02, 0x28, 0x5F, 0xD0, 0x04, 0xDC, 0x00, 0x28, 0x12, 0xD0, 0x01, 0x28, 0x6C, 0xD1,
    0x4A, 0xE0, 0x11, 0x28, 0x6A, 0xD0, 0x21, 0x28, 0xF9, 0xD1, 0x68, 0xE0, 0x43, 0x28, 0x7C, 0xD0,
    0x7F, 0x28, 0x04, 0xD0, 0x82, 0x28, 0x79, 0xD0, 0x92, 0x28, 0xF0, 0xD1, 0x6E, 0xE1, 0x79, 0x20,
    0x56, 0xE1, 0xFF, 0xF7, 0xE3, 0xFE, 0xFF, 0x28, 0x0A, 0xD0, 0x1F, 0x20, 0xFF, 0xF7, 0xE8, 0xFE,
    0xC0, 0x48, 0x00, 0x78, 0xC0, 0x43, 0x00, 0x06, 0x00, 0x0E, 0xD4, 0xD0, 0x01, 0x20, 0xD2, 0xE7,
    0x79, 0x20, 0xFF, 0xF7, 0xDD, 0xFE, 0x0B, 0x20, 0xFF, 0xF7, 0xDA, 0xFE, 0x22, 0x20, 0xFF, 0xF7,
    0xD7, 0xFE, 0x00, 0x20, 0xFF, 0xF7, 0xD4, 0xFE, 0x01, 0x20, 0xFF, 0xF7, 0xD1, 0xFE, 0x02, 0x20,
    0xFF, 0xF7, 0xCE, 0xFE, 0x11, 0x20, 0xFF, 0xF7, 0xCB, 0xFE, 0x21, 0x20, 0xFF, 0xF7, 0xC8, 0xFE,
    0x31, 0x20, 0xFF, 0xF7, 0xC5, 0xFE, 0x43, 0x20, 0xFF, 0xF7, 0xC2, 0xFE, 0x63, 0x20, 0xFF, 0xF7,
    0xBF, 0xFE, 0x73, 0x20, 0xFF, 0xF7, 0xBC, 0xFE, 0x82, 0x20, 0xFF, 0xF7, 0xB9, 0xFE, 0x92, 0x20,
    0xFF, 0xF7, 0xB6, 0xFE, 0x79, 0x20, 0xC9, 0xE7, 0xFF, 0xF7, 0xA8, 0xFE, 0xFE, 0x28, 0xAE, 0xD1,
    0x79, 0x20, 0xFF, 0xF7, 0xAD, 0xFE, 0x22, 0x20, 0xFF, 0xF7, 0xAA, 0xFE, 0x00, 0x20, 0xFF, 0xF7,
    0xA7, 0xFE, 0x00, 0x20, 0x0D, 0xE0, 0xFF, 0xF7, 0x99, 0xFE, 0xFD, 0x28, 0x9F, 0xD1, 0x79, 0x20,
    0xFF, 0xF7, 0x9E, 0xFE, 0x01, 0x20, 0xFF, 0xF7, 0x9B, 0xFE, 0x04, 0x20, 0xFF, 0xF7, 0x98, 0xFE,
    0x10, 0x20, 0xFF, 0xF7, 0x95, 0xFE, 0xA2, 0xE7, 0x7D, 0xE0, 0xF8, 0xE0, 0x2D, 0xE0, 0xFF, 0xF7,
    0x85, 0xFE, 0xDE, 0x28, 0x8B, 0xD1, 0x68, 0x78, 0x00, 0x28, 0x88, 0xD1, 0x79, 0x20, 0xFF, 0xF7,
    0x87, 0xFE, 0xFF, 0xF7, 0x7B, 0xFE, 0x03, 0x06, 0xFF, 0xF7, 0x78, 0xFE, 0x02, 0x04, 0x1A, 0x43,
    0xFF, 0xF7, 0x74, 0xFE, 0x03, 0x02, 0x13, 0x43, 0x01, 0xE0, 0xA4, 0xE0, 0xE3, 0xE0, 0xFF, 0xF7,
    0x6D, 0xFE, 0x02, 0x46, 0x1A, 0x43, 0x10, 0x0A, 0x50, 0x40, 0x11, 0x0C, 0x48, 0x40, 0x11, 0x0E,
    0x48, 0x40, 0xC3, 0xB2, 0xFF, 0xF7, 0x62, 0xFE, 0x98, 0x42, 0xB8, 0xD1, 0x79, 0x20, 0xFF, 0xF7,
    0x67, 0xFE, 0x10, 0x46, 0xFF, 0xF7, 0x22, 0xFF, 0x56, 0xE7, 0xFF, 0xF7, 0x57, 0xFE, 0xEE, 0x28,
    0xAD, 0xD1, 0x68, 0x78, 0x00, 0x28, 0xAA, 0xD1, 0x79, 0x20, 0xFF, 0xF7, 0x59, 0xFE, 0xFF, 0xF7,
    0x4D, 0xFE, 0x03, 0x06, 0xFF, 0xF7, 0x4A, 0xFE, 0x02, 0x04, 0x1A, 0x43, 0xFF, 0xF7, 0x46, 0xFE,
    0x03, 0x02, 0x13, 0x43, 0xFF, 0xF7, 0x42, 0xFE, 0x02, 0x46, 0x1A, 0x43, 0x10, 0x0A, 0x50, 0x40,
    0x11, 0x0C, 0x48, 0x40, 0x11, 0x0E, 0x48, 0x40, 0xC3, 0xB2, 0xFF, 0xF7, 0x37, 0xFE, 0x98, 0x42,
    0x8D, 0xD1, 0x79, 0x20, 0xFF, 0xF7, 0x3C, 0xFE, 0xFF, 0xF7, 0x30, 0xFE, 0x03, 0x46, 0xFF, 0xF7,
    0x2D, 0xFE, 0xFF, 0x21, 0x48, 0x40, 0x98, 0x42, 0x90, 0xD1, 0x79, 0x20, 0xFF, 0xF7, 0x30, 0xFE,
    0x66, 0x4C, 0x66, 0x4E, 0x67, 0x1C, 0x20, 0x3E, 0x5B, 0x1C, 0x0E, 0xE0, 0x10, 0x78, 0xA2, 0x42,
    0x05, 0xD1, 0x30, 0x8F, 0x80, 0x05, 0x80, 0x0F, 0x00, 0x01, 0x10, 0x30, 0x02, 0xE0, 0xBA, 0x42,
    0x00, 0xD1, 0x00, 0x20, 0xFF, 0xF7, 0x1C, 0xFE, 0x52, 0x1C, 0x18, 0x46, 0x5B, 0x1E, 0x9B, 0xB2,
    0x00, 0x28, 0xB1, 0xD0, 0xEA, 0xE7, 0xFF, 0xF7, 0x09, 0xFE, 0xCE, 0x28, 0x77, 0xD1, 0x68, 0x78,
    0x00, 0x28, 0x74, 0xD1, 0x79, 0x20, 0xFF, 0xF7, 0x0B, 0xFE, 0xFF, 0xF7, 0xFF, 0xFD, 0x02, 0x06,
    0xFF, 0xF7, 0xFC, 0xFD, 0x03, 0x04, 0x13, 0x43, 0xFF, 0xF7, 0xF8, 0xFD, 0x02, 0x02, 0x1A, 0x43,
    0xFF, 0xF7, 0xF4, 0xFD, 0x04, 0x46, 0x14, 0x43, 0x20, 0x0A, 0x60, 0x40, 0x21, 0x0C, 0x48, 0x40,
    0x21, 0x0E, 0x48, 0x40, 0xC2, 0xB2, 0xFF, 0xF7, 0xE9, 0xFD, 0x90, 0x42, 0x7F, 0xD1, 0x79, 0x20,
    0xFF, 0xF7, 0xEE, 0xFD, 0xFF, 0xF7, 0xE2, 0xFD, 0x00, 0x22, 0x03, 0x46, 0x46, 0x1C, 0x44, 0x4F,
    0x05, 0xE0, 0xFF, 0xF7, 0xDB, 0xFD, 0xB8, 0x54, 0x52, 0x1C, 0x92, 0xB2, 0x43, 0x40, 0xB2, 0x42,
    0xF7, 0xD3, 0xFF, 0xF7, 0xD3, 0xFD, 0x98, 0x42, 0x69, 0xD1, 0x3D, 0x4A, 0x31, 0x46, 0x20, 0x46,
    0xFF, 0xF7, 0x8C, 0xFD, 0x38, 0xE0, 0xFF, 0xF7, 0xC9, 0xFD, 0xBC, 0x28, 0x5F, 0xD1, 0x68, 0x78,
    0x00, 0x28, 0x5C, 0xD1, 0x79, 0x20, 0xFF, 0xF7, 0xCB, 0xFD, 0xFF, 0xF7, 0xBF, 0xFD, 0x06, 0x46,
    0xFF, 0x28, 0x23, 0xD0, 0x00, 0x22, 0x04, 0x46, 0x31, 0x4B, 0xFF, 0xF7, 0xB7, 0xFD, 0x98, 0x54,
    0x44, 0x40, 0x10, 0x46, 0x52, 0x1C, 0x92, 0xB2, 0xB0, 0x42, 0xF6, 0xD3, 0xFF, 0xF7, 0xAE, 0xFD,
    0xA0, 0x42, 0x44, 0xD1, 0x00, 0x27, 0x3C, 0x46, 0x29, 0x48, 0x00, 0x5D, 0x81, 0x02, 0x01, 0x20,
    0xC0, 0x06, 0x08, 0x18, 0xFF, 0xF7, 0x12, 0xFD, 0x38, 0x43, 0xC7, 0xB2, 0x20, 0x46, 0x64, 0x1C,
    0xA4, 0xB2, 0xB0, 0x42, 0xF0, 0xD3, 0x00, 0x2F, 0x31, 0xD1, 0xB0, 0xE6, 0xFF, 0xF7, 0x96, 0xFD,
    0x00, 0x28, 0xFA, 0xD1, 0xFF, 0xF7, 0xE0, 0xFC, 0x00, 0x28, 0x28, 0xD1, 0xA7, 0xE6, 0x26, 0xE0,
    0xFF, 0xF7, 0x96, 0xFD, 0x88, 0xE6, 0xFF, 0xF7, 0x89, 0xFD, 0x7D, 0x28, 0x1F, 0xD1, 0x68, 0x78,
    0x00, 0x28, 0x1C, 0xD1, 0x17, 0xA0, 0x00, 0x68, 0x00, 0x90, 0x79, 0x20, 0xFF, 0xF7, 0x88, 0xFD,
    0x6A, 0x46, 0x04, 0x21, 0x0F, 0x48, 0xFF, 0xF7, 0x39, 0xFD, 0x0E, 0xE0, 0xFF, 0xF7, 0x76, 0xFD,
    0x6D, 0x28, 0x0C, 0xD1, 0x79, 0x20, 0xFF, 0xF7, 0x7B, 0xFD, 0xFF, 0xF7, 0xBD, 0xFC, 0x04, 0x46,
    0x08, 0x48, 0xFF, 0xF7, 0xDB, 0xFC, 0x20, 0x43, 0xC0, 0xB2, 0x00, 0x28, 0x01, 0xD0, 0x1F, 0x20,
    0xD6, 0xE7, 0x79, 0x20, 0xFF, 0xF7, 0x6C, 0xFD, 0xFF, 0xF7, 0x14, 0xFD, 0x00, 0x10, 0x02, 0x40,
    0x00, 0x38, 0x01, 0x40, 0x00, 0x00, 0xFE, 0x1F, 0x00, 0x18, 0x00, 0x20, 0xE0, 0xF7, 0xFF, 0x1F,
    0x08, 0x18, 0x00, 0x20, 0x80, 0x7F, 0x00, 0xFF, 0xB8, 0x0D, 0x00, 0x20, 0x00, 0x18, 0x00, 0x20,
    0x08, 0x00, 0x00, 0x00, 0x4C, 0x04, 0x00, 0x20, 0xC0, 0x0D, 0x00, 0x20, 0x08, 0x18, 0x00, 0x20,
    0x60, 0x02, 0x00, 0x00, 0x68, 0x04, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};





