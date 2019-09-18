#include "FastAES.h"
#include "WzArchive.h"
#include "WzMappedFileStream.h"
#include <codecvt>
#include <intrin.h> //For VS 2017.

unsigned char kaWzFileAESKey_TWMS[4096] =
{
	0xAB, 0x65, 0x49, 0x05, 0x67, 0xCD, 0x57, 0x0A, 0x98, 0x7B, 0x87, 0x0A, 0xEC, 0x65, 0x07, 0x8B, 0x95, 0x74, 0x36, 0x6C, 0xB3, 0x59, 0x29, 0x5E, 0xA2, 0x59, 0x03, 0x7E, 0xBF, 0x7D, 0xCB, 0x74, 0x90, 0x53, 0x26, 0x94, 0x18, 0x6F, 0x61, 0x87, 0xD0, 0xD6, 0x79, 0x1E, 0xAC, 0x52, 0xE9, 0x52, 0x82, 0x7A, 0x1E, 0xE8, 0x06, 0x90, 0xEF, 0x4B, 0x03, 0xFF, 0xD3, 0xF3, 0x70, 0x4A, 0x39, 0xD3, 0x16, 0x99, 0xB8, 0x11, 0x94, 0x1C, 0x81, 0x1F, 0xFD, 0xE2, 0xF1, 0x37, 0x6A, 0x4B, 0xB7, 0xFA, 0x51, 0xD0, 0x0D, 0x6E, 0xAC, 0xAA, 0xE4, 0x6E, 0x2A, 0xFC, 0xC2, 0xDF, 0x40, 0x4F, 0xF7, 0xF3, 0x30, 0xCF, 0xC8, 0xEC, 0x07, 0x30, 0xFD, 0xA9, 0x00, 0x01, 0xE6, 0x80, 0xA9, 0xC5, 0x57, 0x16, 0xB0, 0x97, 0x87, 0xA4, 0x5A, 0xCD, 0x98, 0xD7, 0x99, 0x6F, 0xBC, 0x62, 0x63, 0xEB, 0xD5, 0xD4, 0x13, 0xF3, 0x4D, 0x43, 0x8A, 0x00, 0x47, 0x77, 0xD1, 0x41, 0x5B, 0xC0, 0x7B, 0x2B, 0xF9, 0xB6, 0xF7, 0x3A, 0x09, 0xD0, 0x18, 0x93, 0x9B, 0x15, 0x60, 0xF6, 0x6F, 0xF8, 0x53, 0xEF, 0x32, 0x81, 0x97, 0x4E, 0x29, 0xCF, 0x68, 0x5F, 0xC3, 0x2D, 0xEE, 0xDF, 0x95, 0xD7, 0x67, 0x44, 0x40, 0x97, 0x92, 0xEB, 0x91, 0xE9, 0x7C, 0xB0, 0x42, 0xF9, 0x40, 0xE0, 0xA3, 0xFA, 0xC3, 0x61, 0x6E, 0xC2, 0x67, 0x3B, 0xF9, 0xC6, 0x91, 0xFB, 0x28, 0xC4, 0x37, 0x86, 0x3D, 0x5C, 0x89, 0xA3, 0x24, 0x0E, 0xFC, 0x68, 0x17, 0xB7, 0x37, 0x49, 0x1C, 0x0B, 0x58, 0xF6, 0x7F, 0x7A, 0x4B, 0xC2, 0xF8, 0x7C, 0xAA, 0x95, 0x15, 0xC2, 0xC6, 0x26, 0x71, 0xD4, 0x9D, 0xBC, 0x49, 0x08, 0x51, 0x35, 0x0D, 0xD6, 0x93, 0x61, 0xBA, 0x65, 0x7C, 0x43, 0xE9, 0xD0, 0xC0, 0x08, 0x2E, 0x78, 0x5C, 0x0A, 0xBE, 0x99, 0x75, 0xCA, 0x2A, 0x47, 0x47, 0xED, 0xD0, 0xB2, 0xD9, 0x29, 0x4F, 0x32, 0xBA, 0x55, 0x2A, 0xBB, 0xAC, 0x47, 0xE4, 0xD4, 0x86, 0x30, 0xCA, 0xC7, 0x80, 0xD3, 0xF7, 0x1F, 0x12, 0xDC, 0xC3, 0x6F, 0x06, 0xEC, 0x8A, 0x8D, 0x1F, 0x8A, 0x3E, 0x68, 0xDB, 0xD7, 0x93, 0xFE, 0x27, 0x9A, 0x8F, 0x91, 0x13, 0xB8, 0x29, 0xD2, 0xC9, 0x7F, 0xB4, 0xD0, 0xFF, 0x71, 0xFF, 0xEC, 0xE8, 0xB0, 0x39, 0xDA, 0xAD, 0x94, 0xC6, 0x26, 0xF7, 0x3E, 0x48, 0xFC, 0x8E, 0xE0, 0x03, 0xE3, 0x18, 0xA2, 0x6B, 0xA8, 0x6B, 0xD4, 0xD7, 0xA6, 0x6A, 0xD1, 0x02, 0x4F, 0x0B, 0xE7, 0xE3, 0xE1, 0x33, 0x13, 0x2A, 0x5A, 0xC2, 0x4F, 0x20, 0x92, 0xFF, 0xF7, 0xD9, 0x21, 0x03, 0xCA, 0x68, 0x3D, 0xB9, 0x8D, 0x79, 0xDC, 0x76, 0x71, 0xB6, 0xB0, 0xD0, 0x59, 0x3B, 0xB7, 0xDF, 0xA7, 0xF7, 0xEB, 0x6D, 0xAE, 0x97, 0xA0, 0x7E, 0x5F, 0x9E, 0x33, 0x1F, 0x8D, 0x51, 0x58, 0xA2, 0xF7, 0x8D, 0xFF, 0x95, 0x31, 0x6E, 0x28, 0x54, 0xDC, 0x01, 0x54, 0xF6, 0xC2, 0xAB, 0xAC, 0x1B, 0xB7, 0x69, 0x89, 0x00, 0x1B, 0x91, 0x54, 0xC6, 0xF4, 0xBB, 0x7A, 0x69, 0xD2, 0x2D, 0xF3, 0x4C, 0x23, 0xAE, 0xBA, 0xAF, 0x7A, 0x9D, 0xE3, 0x0E, 0x3F, 0x3D, 0x4B, 0x77, 0x32, 0x14, 0xD9, 0xB6, 0x3B, 0xC8, 0xC7, 0x06, 0x03, 0xDA, 0x00, 0xDA, 0xAF, 0xAB, 0x59, 0x54, 0xED, 0x1D, 0xF9, 0x4D, 0x5E, 0x1C, 0x11, 0x2B, 0xC0, 0x7C, 0x62, 0x4A, 0x64, 0x54, 0x5C, 0x4E, 0xAD, 0x10, 0x93, 0x8D, 0x23, 0xD3, 0xA7, 0xF1, 0xE5, 0xA2, 0x46, 0x0B, 0x04, 0x9B, 0xB9, 0x3E, 0xF1, 0x2B, 0x35, 0x6F, 0x31, 0x53, 0x67, 0x7B, 0x09, 0xD7, 0xE5, 0x84, 0x81, 0x95, 0xE3, 0x6A, 0xDE, 0x84, 0xA4, 0xEA, 0xD7, 0xE7, 0xC1, 0x25, 0xCA, 0xE7, 0xA9, 0xA3, 0x1E, 0xCB, 0x72, 0x78, 0x25, 0xD7, 0x83, 0x82, 0x48, 0x42, 0x7A, 0x58, 0xF5, 0x94, 0x86, 0x82, 0x19, 0x19, 0xDA, 0xDA, 0x83, 0x0E, 0xAA, 0x47, 0x57, 0x7A, 0x3B, 0x44, 0xBA, 0x8C, 0xD2, 0x3E, 0xAB, 0x62, 0x90, 0xED, 0xEA, 0xD8, 0xE6, 0xD9, 0x72, 0x75, 0x93, 0xDB, 0xC3, 0xF6, 0x76, 0xA1, 0xA8, 0x5C, 0x64, 0x3F, 0xFC, 0x6D, 0x16, 0xE6, 0x6F, 0x43, 0x5A, 0x14, 0xDA, 0xDB, 0x83, 0xAF, 0xD4, 0xCD, 0x57, 0x79, 0x36, 0x6A, 0x1A, 0x15, 0xD9, 0x9A, 0xDE, 0xFD, 0xAE, 0xE3, 0x69, 0xFE, 0x45, 0x0A, 0xAF, 0x3D, 0xB6, 0x39, 0x54, 0x39, 0x5C, 0x76, 0xCE, 0xC4, 0x28, 0x68, 0x39, 0x52, 0xD3, 0x73, 0x32, 0xF8, 0x86, 0xBB, 0x96, 0x0E, 0xDD, 0x28, 0x71, 0x38, 0xFB, 0xF0, 0xED, 0x26, 0x0D, 0x89, 0xE0, 0x0E, 0xE1, 0x66, 0x7B, 0x7C, 0x8D, 0x34, 0x1C, 0x4F, 0x42, 0x1B, 0x2F, 0xF6, 0xFC, 0x29, 0x1C, 0xDA, 0x4B, 0x06, 0x8C, 0x68, 0x31, 0xB3, 0x4D, 0x13, 0x51, 0xD6, 0x59, 0x7B, 0x20, 0x5B, 0x09, 0x58, 0xA2, 0xB2, 0x43, 0x6E, 0x7E, 0x05, 0xB5, 0x35, 0x7A, 0xD9, 0x4E, 0x6E, 0xA2, 0x36, 0xFD, 0x7F, 0xD3, 0x00, 0x90, 0xF4, 0xFC, 0x06, 0xF8, 0xD8, 0x1F, 0x6D, 0x57, 0x7C, 0xD6, 0xBC, 0x2B, 0xC5, 0x7A, 0x6C, 0x0A, 0x44, 0xD6, 0x41, 0x96, 0x96, 0x28, 0x07, 0x66, 0xCC, 0xBB, 0xAF, 0x3B, 0x00, 0xC9, 0x7F, 0x06, 0xF8, 0xD9, 0x08, 0x32, 0xF3, 0xF1, 0x92, 0xA9, 0xA5, 0x98, 0x67, 0x6B, 0xFD, 0x85, 0x36, 0x24, 0xA7, 0xD1, 0xC4, 0xB3, 0xD8, 0x2D, 0xA1, 0x57, 0x07, 0x35, 0x18, 0x1C, 0x62, 0xB7, 0x3F, 0x22, 0x87, 0x13, 0x05, 0xA6, 0x8B, 0x17, 0xAF, 0xB1, 0x84, 0x36, 0x92, 0x0D, 0x8A, 0x88, 0xB3, 0x8D, 0x8C, 0xA3, 0x1B, 0xB5, 0xAB, 0x2F, 0x8E, 0x62, 0xF2, 0x73, 0x10, 0xEC, 0x13, 0x21, 0x7B, 0xC7, 0x71, 0x60, 0xA4, 0x6C, 0xFB, 0xF0, 0x92, 0x84, 0x3B, 0x44, 0x72, 0xF7, 0x56, 0x51, 0x08, 0x43, 0x6D, 0x81, 0x46, 0x95, 0x3A, 0x08, 0x2E, 0x52, 0xF2, 0xB4, 0xD3, 0x50, 0x5D, 0x93, 0xA5, 0x9A, 0xC2, 0xE1, 0x5B, 0xAD, 0x3C, 0x0A, 0xFA, 0x9C, 0x7D, 0x34, 0xDE, 0x67, 0xDA, 0x03, 0x12, 0x9F, 0xAC, 0xA2, 0x00, 0x49, 0x7A, 0x73, 0x0A, 0xFD, 0x5D, 0xB4, 0x43, 0xA4, 0x05, 0xB5, 0xCC, 0xF4, 0x09, 0x34, 0x51, 0x74, 0x11, 0xE8, 0x52, 0xA0, 0x24, 0x4A, 0xDB, 0x8E, 0x43, 0x1B, 0x2A, 0xE1, 0x95, 0x40, 0x9B, 0x11, 0x8D, 0x17, 0x26, 0x44, 0x51, 0xA8, 0x2E, 0x69, 0xCF, 0xAB, 0x92, 0xED, 0xBB, 0xA1, 0x57, 0x91, 0xAB, 0x46, 0xC3, 0x3B, 0x7C, 0x44, 0xC8, 0x92, 0xC1, 0xAC, 0x4D, 0x1B, 0x4F, 0x41, 0x49, 0xDB, 0xA3, 0x2F, 0xFC, 0x1A, 0x6F, 0x6D, 0x29, 0xFB, 0xA0, 0x6A, 0x84, 0xDA, 0x9D, 0x7A, 0x23, 0x15, 0x0C, 0x73, 0x61, 0x73, 0x7D, 0x7C, 0x22, 0x26, 0xA3, 0x64, 0xBC, 0x14, 0x18, 0xAA, 0xB2, 0x0F, 0xBF, 0x17, 0x41, 0x88, 0x1E, 0x80, 0x7E, 0xAD, 0x85, 0x15, 0xD6, 0x65, 0xFB, 0xAA, 0xBE, 0x57, 0x81, 0xC9, 0xFC, 0x2D, 0x76, 0x7E, 0x23, 0x62, 0x74, 0x1D, 0xAC, 0x30, 0x8E, 0xD4, 0xEA, 0xA3, 0xD3, 0x9E, 0x8B, 0x92, 0x1D, 0x38, 0x18, 0x90, 0xC3, 0xF2, 0xAA, 0xDA, 0x0B, 0xC5, 0x78, 0xBB, 0x18, 0xDF, 0x8F, 0x19, 0x44, 0x82, 0x0B, 0x3C, 0xEC, 0x4F, 0xBD, 0x8C, 0x4C, 0x17, 0x40, 0x6F, 0xCF, 0xB3, 0x0F, 0x76, 0x96, 0xAF, 0xBA, 0x96, 0xD7, 0x69, 0x11, 0xD5, 0x30, 0x6B, 0x06, 0x9A, 0x10, 0x85, 0x02, 0x68, 0x16, 0x35, 0xE1, 0xB6, 0xB5, 0x86, 0xBE, 0x23, 0x09, 0x37, 0x6B, 0x00, 0xFC, 0xD9, 0x25, 0x94, 0x39, 0x97, 0x6C, 0x16, 0x40, 0x41, 0xCF, 0xD4, 0x87, 0x6F, 0xB1, 0x8B, 0x1A, 0x0C, 0xBC, 0xFD, 0x8D, 0xD7, 0xF5, 0x73, 0x8D, 0x9E, 0x70, 0x1E, 0x37, 0x2A, 0xC0, 0xBC, 0x58, 0xB3, 0x07, 0x41, 0xC0, 0xE0, 0x2A, 0xC6, 0x2B, 0xEA, 0x84, 0x1D, 0x7F, 0x82, 0x1D, 0xB8, 0x0B, 0x02, 0x33, 0xB4, 0xB0, 0x0C, 0x83, 0x37, 0x94, 0xDD, 0xC8, 0xA9, 0xC8, 0x1C, 0xD2, 0x35, 0xA0, 0xA3, 0x65, 0xC5, 0xF1, 0xEA, 0xA5, 0x62, 0x70, 0x21, 0x3E, 0x32, 0xC5, 0xBB, 0x19, 0x64, 0x3A, 0xEF, 0x57, 0x3D, 0x8D, 0xA1, 0x1A, 0xBC, 0xC7, 0x65, 0xDD, 0x93, 0x1D, 0x74, 0x94, 0x31, 0x3E, 0xC4, 0xB8, 0x2E, 0x3D, 0x23, 0xA9, 0x49, 0x68, 0xF0, 0x64, 0x2E, 0x13, 0x53, 0x0E, 0xF4, 0xE8, 0x1F, 0xBE, 0x71, 0x30, 0x5D, 0x40, 0xB6, 0x70, 0x9B, 0xE2, 0xD5, 0x17, 0xFF, 0x56, 0x99, 0x0C, 0x97, 0x9D, 0xBB, 0xBE, 0xAE, 0x6F, 0xA8, 0x9D, 0xF2, 0x91, 0x08, 0xCD, 0xB7, 0x7F, 0xCF, 0x3A, 0x5D, 0xD6, 0xD1, 0xFA, 0xC5, 0xD1, 0x99, 0x99, 0x51, 0x2D, 0x80, 0x05, 0xAA, 0x63, 0x4B, 0x27, 0x74, 0x74, 0x5A, 0x9D, 0xDB, 0xEC, 0xD4, 0xC6, 0x25, 0xE1, 0xD5, 0x60, 0xF2, 0x37, 0x97, 0xD7, 0x0D, 0x2B, 0xA3, 0x35, 0x6D, 0xF2, 0xA0, 0x00, 0x46, 0x5F, 0x14, 0x69, 0x59, 0x64, 0x9D, 0x50, 0x74, 0x58, 0x56, 0x2E, 0xF6, 0x17, 0xE1, 0x8A, 0x85, 0x7E, 0xC5, 0xF6, 0xB3, 0x60, 0x54, 0x02, 0xD3, 0xFA, 0xC9, 0xFF, 0x5C, 0x83, 0xAB, 0x28, 0x53, 0x98, 0xA1, 0x9B, 0xD6, 0xA0, 0x70, 0x8D, 0x3F, 0xE7, 0x64, 0xDA, 0x38, 0x8D, 0xFD, 0xBE, 0x1F, 0xB6, 0xC1, 0xA4, 0x05, 0x77, 0x44, 0xCE, 0x5E, 0xEF, 0xCF, 0x7F, 0x54, 0xE4, 0x8F, 0x6D, 0x07, 0x14, 0x66, 0x37, 0x37, 0x0B, 0x02, 0x56, 0x48, 0xFE, 0x88, 0x0F, 0x21, 0x0F, 0x96, 0x3F, 0x3B, 0x3E, 0x55, 0x7C, 0x7B, 0xF8, 0xCE, 0xD0, 0x7E, 0xA6, 0x98, 0x76, 0x18, 0x78, 0x0B, 0xF2, 0x1F, 0x43, 0x5B, 0x77, 0x7B, 0x6B, 0x95, 0xE1, 0x3C, 0x4D, 0x7F, 0xAD, 0x73, 0x68, 0x64, 0xBD, 0xB0, 0x60, 0x72, 0x11, 0xEF, 0xCC, 0x88, 0x2D, 0x78, 0x1C, 0x95, 0x13, 0x07, 0xB2, 0x8A, 0x17, 0xAC, 0x20, 0x4F, 0x51, 0x25, 0x11, 0xC1, 0x99, 0x4F, 0x09, 0x0D, 0x4A, 0x91, 0x42, 0xAB, 0xE6, 0xEC, 0x87, 0x6D, 0xA9, 0xE0, 0xD6, 0x81, 0xE6, 0x2B, 0xDF, 0x87, 0xC6, 0x00, 0xCE, 0xA5, 0x1A, 0x17, 0x4C, 0x3E, 0x74, 0x53, 0x3F, 0x46, 0xF0, 0x07, 0xB4, 0x76, 0x47, 0xE0, 0x5D, 0xB9, 0x5F, 0x77, 0x57, 0xEA, 0xC6, 0x77, 0x34, 0x64, 0xAD, 0x60, 0xAC, 0xFC, 0x3E, 0xB4, 0x89, 0x1A, 0x3D, 0x7B, 0x35, 0x6B, 0x53, 0x7B, 0x82, 0xB5, 0xB1, 0x23, 0x06, 0xBC, 0x57, 0x4C, 0x56, 0xC5, 0x68, 0x0B, 0xD4, 0xE4, 0xBB, 0x5B, 0xB3, 0x97, 0xC3, 0xD3, 0xB5, 0xC7, 0x7A, 0x73, 0x0D, 0xD2, 0x02, 0xD9, 0x6E, 0x53, 0xFB, 0x0B, 0x30, 0xAA, 0x30, 0x9E, 0x40, 0x87, 0x1F, 0x65, 0xE1, 0x8E, 0x49, 0xA9, 0x7E, 0x05, 0x85, 0xEE, 0xE1, 0xF8, 0x7B, 0xDD, 0x9D, 0x98, 0x24, 0x17, 0x19, 0x59, 0x98, 0x3E, 0x87, 0x70, 0xFC, 0x28, 0x29, 0x5B, 0x01, 0x7D, 0x84, 0x44, 0xB3, 0x17, 0x07, 0x74, 0xE7, 0xB3, 0x00, 0x2D, 0xD4, 0xCE, 0xEE, 0xAC, 0xBB, 0x1C, 0xB2, 0x59, 0xE7, 0x68, 0x61, 0x7B, 0x8A, 0xA1, 0x46, 0x2E, 0x48, 0xD1, 0x3C, 0xF0, 0x59, 0x50, 0xBC, 0xC7, 0xF8, 0x6E, 0x6E, 0xCC, 0x78, 0x94, 0xFB, 0x35, 0x1E, 0xED, 0x78, 0xFA, 0x8C, 0x72, 0x71, 0x04, 0xB9, 0xBA, 0x35, 0xC5, 0x4E, 0x58, 0x11, 0xF8, 0x3F, 0xB8, 0x5F, 0xE8, 0xF9, 0xCD, 0x09, 0x5C, 0x71, 0xC8, 0xAA, 0x05, 0x33, 0xD4, 0xF2, 0x08, 0x5D, 0xDD, 0xD9, 0x15, 0x94, 0xFA, 0x57, 0xDE, 0x09, 0xFE, 0xEB, 0xAB, 0x6F, 0xD3, 0xD2, 0xA6, 0xB0, 0x7C, 0x46, 0x9C, 0x08, 0xA8, 0xD2, 0x55, 0x7C, 0x28, 0x2F, 0xFE, 0xED, 0x29, 0xB5, 0xD4, 0xDB, 0x7C, 0x00, 0xF0, 0xCF, 0x0F, 0x0D, 0x4E, 0xD6, 0x4D, 0x70, 0x2B, 0x0C, 0x68, 0x97, 0x3B, 0x51, 0x06, 0xF6, 0x95, 0xB5, 0xAF, 0xBC, 0x25, 0x71, 0x7E, 0xCD, 0xBA, 0x73, 0xB4, 0x26, 0xE7, 0x8D, 0xB5, 0x01, 0x6C, 0x67, 0x59, 0xE3, 0xAD, 0x73, 0x6A, 0xB4, 0x01, 0xBB, 0x89, 0xAF, 0xCB, 0xF4, 0xC9, 0xC4, 0x88, 0xE7, 0x08, 0x74, 0xA8, 0xD2, 0x1D, 0x78, 0xCD, 0x81, 0x17, 0xE1, 0x04, 0x78, 0x72, 0xB6, 0xDF, 0x97, 0x1C, 0xCE, 0x3C, 0x2E, 0x31, 0x5A, 0x42, 0x61, 0x12, 0xFF, 0x0E, 0xA0, 0x9D, 0x30, 0x80, 0x8C, 0xCE, 0xD2, 0xD2, 0x7F, 0x9E, 0xAB, 0xFB, 0x61, 0x56, 0x90, 0x31, 0x1C, 0x75, 0x94, 0x21, 0x59, 0xEE, 0xF1, 0x12, 0xCC, 0x37, 0x95, 0xA6, 0x93, 0x1F, 0x36, 0x1E, 0x82, 0xC6, 0xEE, 0xD4, 0x21, 0x93, 0xB1, 0x09, 0x01, 0x32, 0xCA, 0x8A, 0xF1, 0xDD, 0xC4, 0x8D, 0x4A, 0xD4, 0xE2, 0x65, 0xD6, 0xFB, 0x6E, 0x5F, 0xE9, 0x8F, 0x8F, 0x06, 0xC8, 0x27, 0xA5, 0xCC, 0x8C, 0x3F, 0xD9, 0xD7, 0xE2, 0x98, 0x17, 0x7F, 0xE4, 0x8C, 0xD1, 0x2B, 0xD3, 0x1F, 0x86, 0x1D, 0xA4, 0x7F, 0xC1, 0x86, 0x52, 0xC4, 0x71, 0xC4, 0xD1, 0xA4, 0xD6, 0x22, 0x9D, 0x74, 0x9A, 0x13, 0x16, 0x2E, 0xE7, 0x1F, 0x4B, 0x38, 0xE0, 0xFE, 0xF8, 0xC6, 0x33, 0x1D, 0x4C, 0x88, 0x35, 0x8D, 0x10, 0xA1, 0xA0, 0x26, 0x02, 0x17, 0xA8, 0xD9, 0x92, 0x79, 0x49, 0x59, 0x1C, 0xF1, 0x88, 0x5D, 0x04, 0x75, 0x00, 0x5C, 0x50, 0xE7, 0x49, 0xD9, 0xF1, 0xDA, 0x0F, 0x88, 0x6B, 0x6E, 0x33, 0xCC, 0xA4, 0x51, 0x26, 0x5C, 0x28, 0x78, 0xAD, 0xD5, 0xBF, 0x68, 0x35, 0xCC, 0xD5, 0xD5, 0x2A, 0x7A, 0xB1, 0xC3, 0x9C, 0x08, 0x7E, 0x16, 0xC3, 0x01, 0xED, 0xFD, 0x83, 0xE9, 0xB8, 0x66, 0x23, 0xC7, 0x42, 0x68, 0xDB, 0x76, 0xDB, 0x09, 0x9E, 0x79, 0xB9, 0x79, 0xB8, 0x74, 0x06, 0xAB, 0x5E, 0xCB, 0x9E, 0x49, 0xFF, 0x42, 0x1A, 0x91, 0x44, 0xC6, 0x4C, 0x5F, 0x40, 0x45, 0xDE, 0xE3, 0xBD, 0xE4, 0x65, 0x21, 0x80, 0x2D, 0xD1, 0x2B, 0xF0, 0xAC, 0x83, 0x13, 0x9D, 0x3A, 0x90, 0x13, 0x9D, 0xD5, 0x82, 0x74, 0x45, 0xDE, 0x4A, 0xE3, 0x5F, 0xE8, 0x06, 0xC6, 0xD0, 0xC0, 0x41, 0x3C, 0xE2, 0xB3, 0x1F, 0x73, 0x56, 0xDF, 0xEE, 0xAF, 0xF7, 0x46, 0x17, 0xC1, 0xF1, 0x79, 0x00, 0x56, 0x03, 0xCC, 0xB3, 0x7A, 0xB5, 0x5C, 0x29, 0xD1, 0xC7, 0xFA, 0xE7, 0x16, 0x00, 0x81, 0x56, 0xB0, 0x87, 0x07, 0x57, 0x24, 0xEC, 0x94, 0xC8, 0xC6, 0xEB, 0xAC, 0xE6, 0xD5, 0x05, 0xBD, 0x1A, 0x9E, 0xDC, 0xA0, 0x6C, 0x7E, 0xD4, 0x3B, 0x7F, 0x3C, 0xED, 0xC9, 0x43, 0x7A, 0x18, 0x5E, 0x43, 0x02, 0x48, 0x29, 0xA2, 0xFA, 0x5A, 0x54, 0x62, 0x2C, 0xBE, 0xB1, 0x43, 0x95, 0xFA, 0x07, 0x27, 0x65, 0xA3, 0x8A, 0x83, 0xFF, 0x75, 0x7B, 0x2E, 0x34, 0x15, 0x47, 0xC9, 0xCE, 0xC4, 0x42, 0xFC, 0xCD, 0xEF, 0xDC, 0x87, 0x28, 0x9B, 0x5E, 0x7E, 0xD3, 0x60, 0x7E, 0x06, 0x39, 0xEA, 0x58, 0x9D, 0xED, 0x66, 0x09, 0xB3, 0xD6, 0x3F, 0x3E, 0xE7, 0x0F, 0xD8, 0x29, 0xBC, 0x4C, 0x35, 0xF5, 0xFA, 0x1B, 0x33, 0x51, 0x5B, 0x28, 0xD3, 0x85, 0xAF, 0x04, 0xFB, 0x5F, 0x8B, 0x7E, 0x1A, 0x4C, 0x20, 0x85, 0x91, 0x2A, 0x86, 0xE9, 0x8C, 0xD8, 0xBD, 0x9E, 0x89, 0xD5, 0xFD, 0x52, 0x2B, 0x09, 0xE5, 0x31, 0xB5, 0x1C, 0xC1, 0x5A, 0xFD, 0xBB, 0x87, 0x19, 0xC6, 0x15, 0xCE, 0x2F, 0xB7, 0xF6, 0xBA, 0xAF, 0xAD, 0xFA, 0xE8, 0x6F, 0x91, 0x56, 0xE0, 0xF5, 0xDF, 0x2F, 0xDB, 0x65, 0x52, 0x5C, 0xE6, 0xCF, 0xED, 0x2A, 0xC2, 0x50, 0x14, 0x01, 0xFB, 0xBA, 0x3A, 0x8D, 0x25, 0x63, 0xE3, 0x02, 0x50, 0x0B, 0xAD, 0xE1, 0x3F, 0x08, 0x80, 0x57, 0xDB, 0x69, 0x57, 0x68, 0x77, 0x85, 0x58, 0x43, 0x62, 0xB9, 0x19, 0xD9, 0x67, 0xCD, 0xBA, 0x05, 0xDE, 0xD7, 0xC9, 0x81, 0x26, 0x24, 0xE2, 0xF6, 0xAC, 0x7D, 0xB6, 0x53, 0x49, 0xA2, 0xC9, 0xDE, 0x4D, 0xBA, 0xF5, 0xCF, 0x86, 0x85, 0x50, 0x2A, 0x12, 0x9B, 0x16, 0xA2, 0x85, 0x61, 0x11, 0xB7, 0x12, 0xF3, 0x42, 0xD5, 0x8B, 0x49, 0x1F, 0x5C, 0xCB, 0xEB, 0x0E, 0x90, 0xCD, 0x16, 0x5B, 0xBB, 0xB1, 0x55, 0x77, 0x5B, 0xEB, 0x96, 0x09, 0x08, 0x53, 0x5C, 0x33, 0x59, 0x16, 0x74, 0x5D, 0x94, 0x8D, 0xBC, 0xED, 0x7E, 0x6B, 0xE0, 0x2C, 0xCF, 0xA2, 0x96, 0xF3, 0x00, 0xC0, 0x07, 0x75, 0xB4, 0x8F, 0x69, 0x9E, 0x1C, 0x23, 0x82, 0x05, 0x4D, 0x7E, 0x4B, 0x04, 0x91, 0x93, 0x0D, 0x82, 0x08, 0xF1, 0x02, 0x23, 0xFE, 0xE8, 0xF0, 0xF0, 0x99, 0x07, 0xC5, 0x0B, 0xD2, 0xC5, 0x03, 0x5C, 0xEB, 0x23, 0xD5, 0x91, 0xF3, 0x1D, 0x60, 0x63, 0x5C, 0xF2, 0xCF, 0x49, 0xEF, 0xBD, 0xA4, 0xD0, 0xC2, 0x4F, 0x87, 0xD8, 0x33, 0x97, 0xCF, 0xE2, 0xD2, 0x87, 0xA1, 0xCF, 0x8F, 0x0C, 0x4A, 0xA5, 0xB1, 0xDA, 0x97, 0xA5, 0x3C, 0xAA, 0xE8, 0xED, 0xAE, 0x08, 0xC6, 0x07, 0x9C, 0xC4, 0x9B, 0xF3, 0xDD, 0x21, 0x20, 0x16, 0x02, 0x06, 0x17, 0x11, 0x45, 0xD4, 0x16, 0xF1, 0xE7, 0xA2, 0xD0, 0x4C, 0x43, 0x0C, 0x16, 0xE1, 0xDF, 0x7C, 0x49, 0x1A, 0x97, 0x05, 0xCA, 0xF7, 0x5D, 0xFE, 0xAF, 0x62, 0xD1, 0x84, 0xE8, 0xFC, 0x1C, 0xAF, 0x9F, 0x78, 0xED, 0x2D, 0xC4, 0xC7, 0x9D, 0xA8, 0x70, 0xFB, 0x1B, 0x34, 0x65, 0x78, 0x03, 0xBD, 0xC6, 0xE1, 0x2E, 0x76, 0x28, 0x81, 0x3D, 0x01, 0x9A, 0xDB, 0xE6, 0x09, 0xF2, 0xEA, 0x37, 0x72, 0x85, 0x69, 0x51, 0x2A, 0xCC, 0x94, 0x2A, 0xE8, 0xA6, 0xA5, 0x4B, 0xF8, 0xD4, 0xBF, 0x21, 0x41, 0xA0, 0x9B, 0xD4, 0xA1, 0xC0, 0xCC, 0x8F, 0xAD, 0xFB, 0x8B, 0x1D, 0x77, 0x05, 0x2B, 0xAB, 0x01, 0x48, 0x38, 0x46, 0x37, 0x1B, 0xC2, 0x01, 0x37, 0x45, 0x48, 0x75, 0x2A, 0xCD, 0x37, 0xA5, 0xB2, 0xB4, 0xA6, 0x21, 0xD7, 0xC5, 0x48, 0x1A, 0x52, 0x8B, 0xFD, 0xF4, 0xE8, 0xB2, 0x07, 0xB7, 0x75, 0xE9, 0x9F, 0x9C, 0x68, 0x24, 0xBA, 0xC8, 0x14, 0x1E, 0x3E, 0xF8, 0x3B, 0x97, 0x1C, 0xE8, 0x74, 0x91, 0x6B, 0x06, 0x70, 0xC4, 0x5A, 0x35, 0x54, 0x2B, 0x8C, 0xBB, 0x8B, 0x5F, 0x42, 0x81, 0x12, 0xDA, 0x80, 0x7E, 0x53, 0x4C, 0xC1, 0xA3, 0x9A, 0x95, 0xDE, 0xAA, 0x05, 0xA6, 0xA9, 0x6F, 0xE5, 0x3B, 0x12, 0x3C, 0xD4, 0x34, 0x16, 0x66, 0xB7, 0x09, 0x3C, 0x31, 0xE8, 0x8F, 0xA9, 0x0A, 0x22, 0x0D, 0x70, 0xFC, 0x28, 0xD6, 0x51, 0x44, 0xE4, 0x55, 0x9E, 0x01, 0x2B, 0xEA, 0x1D, 0x41, 0x59, 0x88, 0x90, 0x84, 0x7D, 0x08, 0xBB, 0x32, 0x82, 0x87, 0xA7, 0x4B, 0x2C, 0x74, 0x8B, 0xB2, 0xF6, 0x44, 0xFA, 0xC7, 0x24, 0x05, 0xA7, 0xA4, 0xDC, 0x42, 0x2B, 0x08, 0x80, 0x8A, 0x9B, 0x5D, 0xE9, 0xF4, 0xD6, 0x01, 0xB2, 0xE8, 0xE3, 0xE2, 0x8B, 0x76, 0x3F, 0x11, 0x8F, 0x63, 0x22, 0x46, 0x5B, 0x71, 0xDF, 0x92, 0xFD, 0xF4, 0x93, 0x04, 0x57, 0xBE, 0x7D, 0xF4, 0x4C, 0x6F, 0xDB, 0xCF, 0x3D, 0x65, 0x32, 0xE1, 0x21, 0x90, 0xE8, 0xBC, 0x0F, 0xFA, 0x2F, 0x02, 0x19, 0xE0, 0xFD, 0xA2, 0x14, 0xA1, 0x5D, 0xFC, 0xE6, 0x03, 0x73, 0x4A, 0x00, 0x7C, 0x06, 0xE7, 0x80, 0xB5, 0xE5, 0x9A, 0xC2, 0x70, 0x8D, 0x97, 0x7B, 0x93, 0x38, 0x8C, 0x47, 0x6E, 0x22, 0x31, 0x3E, 0xB9, 0xCF, 0x85, 0xF2, 0xB1, 0x18, 0x41, 0x8E, 0x73, 0xC8, 0x7C, 0xA4, 0xA2, 0x98, 0xA3, 0xDA, 0xC0, 0x8F, 0xCA, 0xEC, 0x16, 0x47, 0xF6, 0x10, 0x45, 0x6D, 0x10, 0x11, 0x5E, 0xC2, 0x7C, 0x29, 0x38, 0xA4, 0x3E, 0xEB, 0x00, 0x69, 0x6B, 0xBF, 0x0A, 0x00, 0xD1, 0x7E, 0x57, 0xB7, 0x3C, 0x3B, 0x1B, 0xF8, 0xFE, 0xBE, 0x14, 0x9F, 0x44, 0x4F, 0x3E, 0xC7, 0x54, 0xAB, 0x5F, 0x04, 0xF4, 0x99, 0x03, 0x7B, 0xCF, 0x57, 0x85, 0x61, 0xAB, 0x3F, 0xE3, 0x96, 0x61, 0x5E, 0x68, 0xD1, 0x11, 0x1A, 0x2A, 0xB3, 0x8D, 0x03, 0xBA, 0x55, 0x02, 0x73, 0xDF, 0xB8, 0xC4, 0x38, 0x55, 0x37, 0xA5, 0x5E, 0x02, 0x3D, 0x33, 0xCA, 0x79, 0x54, 0x02, 0x7A, 0xB3, 0xA0, 0xF8, 0x82, 0xE5, 0x6E, 0xCC, 0x70, 0x6E, 0x4A, 0x0E, 0x6B, 0x68, 0x99, 0xC3, 0xCD, 0x33, 0x68, 0x8E, 0x3D, 0x6E, 0xCB, 0xB8, 0x65, 0xFE, 0x87, 0x01, 0xC0, 0x15, 0xA2, 0xF1, 0x21, 0x56, 0xFA, 0x9F, 0x6E, 0x38, 0x81, 0x80, 0x30, 0xFF, 0x5E, 0x9A, 0x2C, 0x1A, 0x6B, 0x90, 0xED, 0xC6, 0x45, 0x64, 0x61, 0x71, 0x49, 0x8C, 0x30, 0x83, 0x53, 0x38, 0x96, 0x87, 0x39, 0xC7, 0xEB, 0x65, 0x3A, 0xA9, 0xF0, 0xDD, 0x1A, 0xB1, 0x26, 0xC0, 0x41, 0xD8, 0xA4, 0xEB, 0x67, 0x69, 0xFB, 0x11, 0x72, 0x94, 0x31, 0x78, 0x34, 0x25, 0x3C, 0x93, 0xA0, 0xAC, 0x16, 0x5D, 0xC8, 0xC4, 0x10, 0xF5, 0x2D, 0xD0, 0x63, 0x91, 0xDD, 0x36, 0x5F, 0xE9, 0x45, 0xAC, 0x15, 0x27, 0x47, 0x8E, 0x3D, 0xC8, 0x7A, 0x56, 0xA7, 0x3D, 0xEC, 0x1B, 0x1B, 0xA2, 0xFA, 0xF9, 0x03, 0x93, 0x31, 0xAF, 0xF7, 0xB6, 0x76, 0x67, 0x6C, 0x22, 0x39, 0xB7, 0x2C, 0x97, 0x54, 0xF2, 0x2B, 0x20, 0x44, 0xA3, 0x9B, 0x64, 0x28, 0xEF, 0x61, 0xF2, 0x6E, 0x05, 0xD1, 0x5B, 0x2C, 0x4E, 0x82, 0x95, 0x50, 0x9B, 0xDB, 0x0C, 0x41, 0xEF, 0xD7, 0xCB, 0x16, 0xA3, 0x1B, 0x48, 0x05, 0xEA, 0xF3, 0x6F, 0x9F, 0xD0, 0x04, 0x9F, 0x0E, 0xD3, 0x23, 0x13, 0x8E, 0xA7, 0x20, 0x6F, 0xEF, 0x76, 0xC3, 0x5F, 0xFF, 0x8D, 0x0B, 0xED, 0x91, 0xD9, 0x5B, 0xF5, 0x59, 0x6E, 0xCC, 0xBE, 0x20, 0x04, 0xED, 0xBF, 0x69, 0xCD, 0x29, 0x63, 0x3B, 0xFB, 0xDA, 0x37, 0x0D, 0x5A, 0x70, 0xFB, 0x48, 0x40, 0x6D, 0x4C, 0xAD, 0x81, 0xA1, 0x72, 0x85, 0xA8, 0x5F, 0xBF, 0x44, 0x73, 0x05, 0xD3, 0xC0, 0xA6, 0xC9, 0x92, 0x2F, 0xAB, 0x20, 0x67, 0x36, 0x8E, 0xCF, 0xCD, 0x24, 0x21, 0xB3, 0x62, 0x47, 0xF8, 0x61, 0x29, 0x13, 0x9E, 0x57, 0xA3, 0xDC, 0x26, 0x70, 0x47, 0x10, 0x93, 0xB6, 0x58, 0xD1, 0xFF, 0xAA, 0xBA, 0xED, 0xE7, 0xCB, 0x0E, 0xDE, 0xB5, 0x72, 0xFC, 0x01, 0x40, 0x59, 0xDE, 0x03, 0x54, 0xC4, 0x4C, 0x41, 0xC2, 0x5F, 0x33, 0x32, 0xDE, 0x26, 0xBE, 0x4A, 0xC3, 0x17, 0x31, 0xA9, 0x49, 0x47, 0x83, 0x0B, 0x63, 0xEA, 0x2A, 0x74, 0xB1, 0xA1, 0x7B, 0xAD, 0xE4, 0x9A, 0x97, 0xEA, 0xC8, 0xC4, 0x71, 0x7B, 0xE3, 0xF3, 0x4D, 0x39, 0xC8, 0xA4, 0x96, 0x01, 0x11, 0xD8, 0x38, 0x8D, 0xF3, 0xF7, 0xCE, 0x82, 0xE3, 0x0D, 0x22, 0xA9, 0xC9, 0xE3, 0xFC, 0x9A, 0x83, 0x61, 0xD2, 0x12, 0x97, 0x83, 0x5A, 0x5D, 0x70, 0x9B, 0x64, 0xF7, 0x08, 0x15, 0x6D, 0x76, 0x1B, 0x30, 0x71, 0x3E, 0x80, 0x02, 0xD0, 0xB9, 0x00, 0xA5, 0xD3, 0x23, 0x80, 0xE3, 0x1E, 0x19, 0xC9, 0xA5, 0x2E, 0xB1, 0xFF, 0x74, 0x61, 0xB8, 0x67, 0xA7, 0x57, 0xBD, 0xCF, 0xB6, 0xC7, 0xEF, 0x8D, 0x61, 0xFE, 0xEB, 0x7E, 0xFB, 0xA7, 0x4F, 0x16, 0xFF, 0x6B, 0x03, 0x78, 0xF9, 0x76, 0xC2, 0xEF, 0xF7, 0xB2, 0xA7, 0xA7, 0xA0, 0x9A, 0x18, 0xE9, 0x26, 0x11, 0xAC, 0xBE, 0xED, 0xBC, 0x76, 0x5A, 0xA0, 0xA3, 0x81, 0x82, 0x42, 0xB0, 0x5B, 0x01, 0x7F, 0x4A, 0xB7, 0xB6, 0x53, 0x12, 0x01, 0x77, 0xFE, 0x71, 0x0A, 0x2F, 0xD1, 0xA0, 0x73, 0x7C, 0x26, 0x3E, 0xE7, 0xEF, 0x7A, 0x85, 0xF8, 0x09, 0xEE, 0x7C, 0x38, 0xD6, 0xA9, 0x33, 0x35, 0x72, 0x8A, 0x8C, 0xE1, 0x50, 0x8B, 0x68, 0x98, 0x31, 0x5A, 0x97, 0xAC, 0xDD, 0xE7, 0x47, 0xED, 0x80, 0xCE, 0x38, 0x96, 0x8A, 0x14, 0x1B, 0x68, 0x66, 0x1F, 0x6A, 0x09, 0x4A, 0xEB, 0x76, 0x12, 0x65, 0x46, 0x82, 0xB3, 0xE8, 0x67, 0xC5, 0xA0, 0xB6, 0x30, 0x0F, 0x27, 0x3C, 0xB1, 0x9C, 0xA2, 0xEC, 0xF3, 0xF0, 0xBA, 0x6C, 0x16, 0xB1, 0x1A, 0x69, 0x56, 0x54, 0xBC, 0xCE, 0x2A, 0x94, 0x2E, 0xDB, 0x34, 0xA4, 0x22, 0x6F, 0x42, 0xBF, 0x20, 0x78, 0x82, 0xA0, 0xBA, 0xAF, 0x53, 0x22, 0xD1, 0xDC, 0x1F, 0x2F, 0xEB, 0x30, 0xFC, 0xDD, 0x31, 0x4A, 0x1F, 0x6D, 0xA8, 0xD2, 0xEC, 0xC5, 0x8A, 0xDA, 0xDF, 0x36, 0xFA, 0x0E, 0xAE, 0xDC, 0xB2, 0xA5, 0x9A, 0xB6, 0xE0, 0xDF, 0x11, 0xC3, 0x7A, 0xBC, 0xD6, 0x8A, 0x68, 0x27, 0xE9, 0x95, 0xD5, 0x02, 0x9E, 0x95, 0x9D, 0xFE, 0xE6, 0x25, 0x08, 0xFB, 0xAE, 0x6D, 0xCC, 0xEF, 0xAF, 0xCA, 0x72, 0xED, 0xC2, 0x35, 0x27, 0x91, 0x88, 0x82, 0x61, 0x68, 0x71, 0xD9, 0x9B, 0x00, 0x1E, 0xC5, 0xD1, 0xEA, 0x7A, 0x1A, 0x90, 0x6B, 0x07, 0xFE, 0x78, 0x22, 0x6F, 0xE5, 0x4E, 0xD0, 0x7B, 0x81, 0xA0, 0x96, 0x8D, 0x77, 0x5C, 0xEB, 0x4D, 0x19, 0xF4, 0xB1, 0xE8, 0xE1, 0x5A, 0x90, 0x2B, 0x57, 0x9D, 0xA5, 0x0F, 0x82, 0xD5, 0xC7, 0xCF, 0x0A, 0xB0, 0x56, 0x22, 0xF0, 0x38, 0x7B, 0x78, 0xD0, 0x89, 0x52, 0x9C, 0x17, 0x55, 0xBC, 0x2D, 0xFE, 0x95, 0x5A, 0x54, 0x31, 0x21, 0x2E, 0xA6, 0x59, 0x53, 0x72, 0x2C, 0x7C, 0xE5, 0xCA, 0x4A, 0x8B, 0x40, 0xBF, 0x6E, 0x65, 0x5A, 0x90, 0xCF, 0x57, 0x5D, 0xD2, 0x95, 0xCE, 0x7E, 0x32, 0x6E, 0xE9, 0xC8, 0x45, 0xA2, 0xA5, 0x85, 0xBD, 0xBC, 0xA6, 0x22, 0x98, 0xCD, 0x5B, 0x57, 0x42, 0xEF, 0x6D, 0x39, 0x93, 0x55, 0x8A, 0x0C, 0x90, 0xFC, 0x5A, 0x9A, 0x1B, 0x3B, 0xF0, 0x8F, 0x7C, 0xFB, 0x9E, 0x37, 0x91, 0xDF, 0x0A, 0x87, 0x57, 0xD2, 0x52, 0x54, 0x6B, 0xE4, 0x12, 0x65, 0x4F, 0x10, 0x8F, 0xDC, 0xAD, 0x4E, 0x90, 0xFD, 0x2C, 0x75, 0xF2, 0x2A, 0x04, 0x61, 0x05, 0x24, 0x13, 0x4A, 0xAD, 0x19, 0xF7, 0xBA, 0xDC, 0x06, 0xE1, 0xFD, 0x45, 0xF7, 0x7A, 0x77, 0x39, 0xE7, 0x99, 0xCB, 0xD3, 0x59, 0x88, 0x35, 0x2A, 0x6E, 0xB7, 0xEC, 0xB5, 0xAD, 0x75, 0xCD, 0x2D, 0x26, 0x50, 0xD8, 0x4F, 0xEC, 0x54, 0x4D, 0x4B, 0xBD, 0x5A, 0x69, 0xC8, 0x70, 0x05, 0x67, 0xE8, 0x24, 0x61, 0x45, 0x10, 0x61, 0xAA, 0xC7, 0x40, 0xC1, 0xDD, 0x95, 0x41, 0xC3, 0xB5, 0xAA, 0xF7, 0xAB, 0x41, 0x0A, 0x11, 0x33, 0xCE, 0x99, 0x25, 0xD0, 0x16, 0x63, 0xA3, 0x65, 0x50, 0x44, 0x6E, 0xE4, 0x53, 0x3E, 0xD3, 0xAA, 0xFE, 0x06, 0xDF, 0xCF, 0x13, 0xFB, 0x72, 0xA7, 0xE7, 0x9A, 0x87, 0x7C, 0xA9, 0x3A, 0x94, 0x3B, 0x24, 0x86, 0x83, 0xEA, 0x83, 0xEB, 0x79, 0xB7, 0x3E, 0xE8, 0xEA, 0x67, 0xAE, 0x25, 0x0A, 0x05, 0xC6, 0x4A, 0xFD, 0xFE, 0xD0, 0xCA, 0x3A, 0xB7, 0x97, 0xAB, 0xC3, 0x36, 0x21, 0x6E, 0x4D, 0xB3, 0x10, 0xB5, 0x52, 0x5E, 0x20, 0xF8, 0x90, 0xDA, 0x75, 0x2E, 0x9B, 0x84, 0x12, 0xC2, 0x47, 0x68, 0x88, 0x43, 0x1A, 0xC9, 0xB2, 0x30, 0x5A, 0x48, 0x6D, 0x5E, 0x08, 0xBE, 0x90, 0x0F, 0x98, 0x7E, 0x72, 0xDD, 0xF5, 0xB2, 0xEB, 0x93, 0x17, 0x38, 0x16, 0x56, 0x55, 0xB1, 0x35, 0x2E, 0x8A, 0xE2, 0xF3, 0x85, 0x8F, 0x37, 0xBB, 0xF3, 0x29, 0x94, 0x3D, 0x85, 0x89, 0x6B, 0xEB, 0x0D, 0xFA, 0x49, 0xE3, 0x43, 0x6F, 0xC4, 0x4E, 0xB5, 0xDE, 0x92, 0x5D, 0x2A, 0xC4, 0xAE, 0x29, 0x77, 0xA1, 0xB2, 0xA8, 0x60, 0xC3, 0xAE, 0x54, 0xFB, 0x9B, 0xDE, 0x05, 0x1A, 0x34, 0xB7, 0xD0, 0x97, 0x08, 0x06, 0x29, 0xFF, 0xA1, 0x66, 0x34, 0x78, 0x7B, 0xD1, 0x77, 0xA6, 0x0A, 0xE0, 0xEC, 0xD3, 0xBC, 0x42, 0x0C, 0x42, 0xDF, 0x18, 0x2B, 0x1D, 0xB0, 0x35, 0xCE, 0x6A, 0x16, 0x18, 0xE6, 0x14, 0xF0, 0x83, 0x6A, 0x11, 0x29, 0x61, 0xC8, 0xB9, 0xD0, 0x52, 0xB7, 0x52, 0x52, 0xDF, 0xB5, 0x1F, 0xE9, 0xB4, 0x1D, 0x0D, 0x1C, 0x70, 0x17, 0x11, 0x0B, 0xE6, 0xC7, 0x31, 0xD0, 0x4F, 0x43, 0x26, 0x3B, 0x33, 0xE5, 0xD0, 0xB3, 0xFE, 0xFB, 0xE8, 0xA5, 0x7C, 0x92, 0xE0, 0xD5, 0x49, 0xB3, 0x7A, 0x9F, 0xE1, 0xF4, 0xF4, 0x23, 0xA0, 0xC4, 0x11, 0xF9, 0x3E, 0x94, 0x8E, 0x75, 0x3D, 0x2C, 0x62, 0xBD, 0x2E, 0xEF, 0xAD, 0x3B, 0x50, 0x8D, 0x2E, 0xF5, 0x37, 0x5F, 0x19, 0xB6, 0xE3, 0x09, 0x23, 0x0F, 0x60, 0x86, 0x8F, 0x4E, 0xFF, 0xB2, 0xD6, 0x11, 0xB1, 0xE8, 0x89, 0xD8, 0xBE, 0xB8, 0xBB, 0x09, 0x75, 0xA6, 0x98, 0x14, 0xAB, 0xDB, 0x02, 0x89, 0x2D, 0xB3, 0xA7, 0x84, 0xB2, 0xAA, 0x5F, 0x2F, 0xB9, 0x02, 0x20, 0x65, 0x2E, 0x6D, 0x62, 0xB6, 0x6D, 0x58, 0x71, 0x26, 0xE0, 0xF0, 0x9D, 0x80, 0xC4, 0x3D, 0x05, 0x86, 0x6F, 0x12, 0x69, 0x2B, 0x43, 0x8C, 0x15, 0xFC, 0xCC, 0x3F, 0xBD, 0x84, 0xD0, 0x84, 0xCD, 0xEC, 0xB2, 0x5C, 0x88, 0x42, 0xBD, 0xFC, 0x90, 0x55, 0xE2, 0x76, 0x10, 0xB0, 0x0F, 0xF7, 0x76, 0x62, 0x84, 0x98, 0x1C, 0x95, 0x30, 0xB6, 0x93, 0x1C, 0x84, 0x1B, 0x32, 0xB8, 0x1B, 0x27, 0x57, 0xF1, 0x23, 0xDE, 0xE3, 0xCB, 0x8D, 0x97, 0xDD, 0x19, 0x66, 0x2B, 0xB8, 0xAD, 0x73, 0x88, 0xB6, 0x53, 0x85, 0xB3, 0xB3, 0x5D, 0xAD, 0xD5, 0xF5, 0x91, 0x44, 0x6C, 0x4E, 0xB6, 0x99, 0x5A, 0x8E, 0xCE, 0x05, 0xA1, 0x87, 0x59, 0x20, 0xE4, 0xA7, 0xE0, 0x3D, 0x92, 0x60, 0x77, 0x22, 0x2B, 0x12, 0xC6, 0xA2, 0xD8, 0x51, 0xF4, 0x97, 0x77, 0xAD, 0x89, 0x21, 0xE5, 0xD8, 0xBE, 0x06, 0x4B, 0x03, 0x9B, 0xF8, 0xFA, 0x23, 0x24, 0x90, 0xDD, 0x0B, 0xFA, 0x0C, 0x1D, 0x46, 0x1B, 0x14, 0xBD, 0x63, 0x9C, 0xC4, 0x5C, 0x49, 0x0F, 0xE9, 0xCC, 0xA4, 0xE9, 0x13, 0x71, 0xCB, 0xB5, 0x31, 0xDF, 0xB5, 0x0B, 0x52, 0x33, 0x32, 0x7B, 0xD1, 0xFD, 0x75, 0xDB, 0xD9, 0x20, 0x54, 0x36, 0xBE, 0x1C, 0x40, 0x8F, 0xA2, 0x3E, 0x3C, 0xAF, 0x52, 0x09, 0x07, 0xF0, 0x7A, 0xE4, 0x0D, 0x1F, 0xFC, 0x6A, 0x7B, 0xC1, 0xF1, 0x85, 0x41, 0xC3, 0xA1, 0xAF, 0x2F, 0x5C, 0xBD, 0x26, 0x97, 0x2D, 0xAC, 0x99, 0x44, 0x81, 0x40, 0x9D, 0x67, 0xED, 0xD1, 0x34, 0x02, 0x0F, 0x17, 0xDE, 0xC4, 0x74, 0x0C, 0x3D, 0x33, 0xE3, 0xA4, 0x42, 0x30, 0x41, 0xA5, 0x9D, 0x45, 0x8E, 0x38, 0x93, 0x4C, 0x6C, 0xF5, 0xD1, 0x99, 0x5E, 0x6C, 0x8A, 0xFE, 0x1A, 0x64, 0x17, 0x51, 0x87, 0x87, 0x5B
};

FastAES::FastAES()
{
}


FastAES::~FastAES()
{
}

void FastAES::Init()
{
#pragma warning(disable:4309)
	memcpy(aKeyStore, kaWzFileAESKey_TWMS, 4096);

	__m128i amask = _mm_setr_epi8(
		0xAA, 0xAB, 0xAC, 0xAD, 0xAE, 0xAF, 0xB0, 0xB1, 0xB2, 0xB3, 0xB4, 0xB5, 0xB6, 0xB7, 0xB8, 0xB9
	);
	__m128i aplus = _mm_set1_epi8(0x10);
	__m128i *ak0 = (__m128i *)aWzKey;

	__m128i wmask = _mm_setr_epi16(
		0xAAAA, 0xAAAB, 0xAAAC, 0xAAAD, 0xAAAE, 0xAAAF, 0xAAA0, 0xAAA1
	);
	__m128i wplus = _mm_set1_epi16(0x0008);
	__m128i *wk0 = (__m128i *)aWideWzKey;
	__m128i *k0 = (__m128i *)aKeyStore;

	for (int i = 0x1000; i > 0; --i) 
	{
		_mm_storeu_si128(ak0, _mm_xor_si128(amask, _mm_loadu_si128(k0)));
		_mm_storeu_si128(wk0, _mm_xor_si128(wmask, _mm_loadu_si128(wk0)));
		++ak0;
		++wk0;
		++k0;
		amask = _mm_add_epi8(amask, aplus);
		wmask = _mm_add_epi8(wmask, wplus);
	}
}

std::string FastAES::DecodeString(WzArchive *pArchive)
{
	static std::codecvt_utf8<char16_t> conv;
	static char in[0x10000];
	static char16_t ws[0x8000];
	static char ns[0x10000];
	int nLen = 0;

	pArchive->Read((char*)&nLen, 1);
	char cLen = ((char*)&nLen)[0];
	if (cLen > 0)
	{
		if (cLen == 127)
			pArchive->Read((char*)&nLen, 4);
		nLen *= 2;

#ifndef USE_MAPPING_FILE
		pArchive->Read(in, nLen);
#endif
		__m128i 
			*m1 = reinterpret_cast<__m128i *>(ws), 
#ifdef USE_MAPPING_FILE
			//Reading buffer from mapping file.
			*m2 = reinterpret_cast<__m128i *>(pArchive->GetStream()->GetStreamPtr()),
#else
			//Reading buffer through fstream
			*m2 = reinterpret_cast<__m128i *>(in),
#endif
			*m3 = reinterpret_cast<__m128i *>(aWideWzKey);

		for (int i = 0; i <= nLen >> 3; ++i)
			_mm_storeu_si128(m1 + i, _mm_xor_si128(_mm_loadu_si128(m2 + i), _mm_loadu_si128(m3 + i)));

		mbstate_t state;
		const char16_t * fnext;
		char * tnext;
		conv.out(state, ws, ws + nLen, fnext, ns, ns + 0x10000, tnext);
	}
	else
	{
		if (cLen == -128)
			pArchive->Read((char*)&nLen, 4);
		else
			nLen = cLen * -1;

#ifndef USE_MAPPING_FILE
		pArchive->Read(in, nLen);
#endif

		__m128i 
			*m1 = reinterpret_cast<__m128i *>(ns),
#ifdef USE_MAPPING_FILE
			//Reading buffer from mapping file.
			*m2 = reinterpret_cast<__m128i *>(pArchive->GetStream()->GetStreamPtr()),
#else
			//Reading buffer through fstream
			*m2 = reinterpret_cast<__m128i *>(in),
#endif
			*m3 = reinterpret_cast<__m128i *>(aWzKey);

		for (int i = 0; i <= nLen >> 4; ++i)
			_mm_storeu_si128(m1 + i, _mm_xor_si128(_mm_loadu_si128(m2 + i), _mm_loadu_si128(m3 + i)));
	}

#ifdef USE_MAPPING_FILE
	//Offset the stream position if you use MappingFile (Becuase there is no call to "Read").
	pArchive->GetStream()->SetPosition(pArchive->GetStream()->GetPosition() + nLen);
#endif
	ns[nLen] = 0;
	return std::string(ns, nLen);
}