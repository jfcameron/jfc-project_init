#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <ctime>

enum class project_type
{
	executable,
	library
};

static const unsigned char icon_png[] = {
	0x89, 0x50, 0x4e, 0x47, 0x0d, 0x0a, 0x1a, 0x0a, 0x00, 0x00, 0x00, 0x0d,
	0x49, 0x48, 0x44, 0x52, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x20,
	0x08, 0x06, 0x00, 0x00, 0x00, 0x73, 0x7a, 0x7a, 0xf4, 0x00, 0x00, 0x00,
	0x20, 0x63, 0x48, 0x52, 0x4d, 0x00, 0x00, 0x7a, 0x25, 0x00, 0x00, 0x80,
	0x83, 0x00, 0x00, 0xf9, 0xff, 0x00, 0x00, 0x80, 0xe8, 0x00, 0x00, 0x52,
	0x08, 0x00, 0x01, 0x15, 0x58, 0x00, 0x00, 0x3a, 0x97, 0x00, 0x00, 0x17,
	0x6f, 0xd7, 0x5a, 0x1f, 0x90, 0x00, 0x00, 0x00, 0x09, 0x70, 0x48, 0x59,
	0x73, 0x00, 0x00, 0x0b, 0x11, 0x00, 0x00, 0x0b, 0x11, 0x01, 0x7f, 0x64,
	0x5f, 0x91, 0x00, 0x00, 0x00, 0x18, 0x74, 0x45, 0x58, 0x74, 0x53, 0x6f,
	0x66, 0x74, 0x77, 0x61, 0x72, 0x65, 0x00, 0x70, 0x61, 0x69, 0x6e, 0x74,
	0x2e, 0x6e, 0x65, 0x74, 0x20, 0x34, 0x2e, 0x30, 0x2e, 0x35, 0x65, 0x85,
	0x32, 0x65, 0x00, 0x00, 0x07, 0x37, 0x49, 0x44, 0x41, 0x54, 0x58, 0x47,
	0xcd, 0x97, 0x7b, 0x70, 0x54, 0xe5, 0x19, 0xc6, 0x3f, 0x08, 0xc9, 0x86,
	0x64, 0x37, 0xb7, 0xdd, 0x44, 0x82, 0x49, 0x6c, 0x02, 0x86, 0x9b, 0xd0,
	0x60, 0x40, 0x52, 0x0c, 0x01, 0x6b, 0x21, 0x1d, 0x0c, 0x97, 0xe0, 0xe0,
	0x10, 0xc0, 0x94, 0xd2, 0x7f, 0x3a, 0x83, 0xa0, 0x4d, 0xa7, 0x2d, 0x55,
	0x6a, 0x24, 0x69, 0x6b, 0x60, 0xc2, 0x25, 0x02, 0x55, 0x2c, 0x01, 0xc4,
	0x0a, 0x5a, 0xaa, 0x71, 0x2d, 0x8a, 0x82, 0x82, 0x58, 0x07, 0xaa, 0x14,
	0x2c, 0x16, 0x10, 0x59, 0xa9, 0x80, 0x8c, 0x0c, 0xd3, 0x99, 0x96, 0xb1,
	0x76, 0xda, 0xb1, 0xd8, 0xa7, 0xef, 0xef, 0xcb, 0x85, 0x4d, 0x68, 0x4b,
	0xfd, 0xc7, 0x76, 0x67, 0x1e, 0xf2, 0xe6, 0xfd, 0x9e, 0xf7, 0xf7, 0x7c,
	0x39, 0xe7, 0xec, 0x39, 0x07, 0xf7, 0x7f, 0xf1, 0x19, 0x1b, 0x76, 0xa1,
	0x92, 0x7e, 0x2e, 0x7b, 0x60, 0xb0, 0x5d, 0xd4, 0xf4, 0x3a, 0x96, 0xbb,
	0x7d, 0x46, 0xf7, 0x77, 0xe1, 0x92, 0x0e, 0x5f, 0xbc, 0xe8, 0xb1, 0xd6,
	0x61, 0xeb, 0xf6, 0xb9, 0x2a, 0xbf, 0x3c, 0x3f, 0x69, 0x4d, 0x79, 0x5e,
	0xe0, 0xc2, 0xc8, 0x70, 0x92, 0x17, 0x35, 0xbd, 0x8e, 0xe5, 0xae, 0xcf,
	0x78, 0xe7, 0x92, 0x2b, 0xae, 0x4b, 0x38, 0x5a, 0x96, 0x9b, 0xe8, 0x7d,
	0x37, 0x46, 0xda, 0x45, 0x4d, 0x8f, 0x35, 0x3c, 0x1d, 0xf6, 0xae, 0xcf,
	0x7f, 0xe4, 0xdf, 0x5c, 0x10, 0xd8, 0x5a, 0xdc, 0xb7, 0xcf, 0xf9, 0xaf,
	0x95, 0xa7, 0x68, 0xd7, 0xba, 0xb0, 0x76, 0xad, 0x0d, 0xeb, 0x9b, 0x93,
	0x52, 0x35, 0x32, 0x9c, 0x78, 0xfe, 0xe6, 0xfc, 0xc0, 0xd6, 0x4e, 0x8d,
	0x2b, 0x48, 0xd8, 0x3a, 0xaa, 0x5f, 0xa8, 0x2d, 0x3b, 0x65, 0x94, 0x96,
	0xcc, 0x2b, 0xd4, 0x9e, 0x87, 0x33, 0xb4, 0x63, 0x65, 0xd8, 0x8b, 0x9a,
	0x1e, 0x6b, 0x78, 0xf0, 0xc6, 0xcf, 0xc2, 0x82, 0x09, 0x9b, 0x0c, 0xb2,
	0xc8, 0x24, 0xdb, 0x8d, 0xcb, 0x4f, 0xd6, 0x9c, 0xb1, 0x89, 0xda, 0xfe,
	0xa3, 0x14, 0xe9, 0x64, 0x58, 0xfa, 0xf0, 0x1a, 0x35, 0xd6, 0x26, 0xab,
	0x24, 0xe4, 0x74, 0x6b, 0x41, 0xaf, 0x2e, 0x4d, 0x2c, 0x70, 0x1a, 0x9b,
	0x9b, 0x2c, 0xe7, 0xa6, 0x6b, 0xf3, 0x83, 0xc3, 0xa5, 0xb3, 0x21, 0x7d,
	0xfa, 0xbb, 0xb0, 0x17, 0x35, 0x3d, 0xd6, 0xf0, 0xe0, 0x8d, 0x9f, 0x85,
	0x05, 0x13, 0x36, 0x19, 0x64, 0x91, 0x49, 0xb6, 0xb3, 0x43, 0xa8, 0x9d,
	0x6b, 0xf3, 0xa4, 0x53, 0x43, 0xf4, 0xc7, 0x37, 0xae, 0xd7, 0xb9, 0xbd,
	0xf9, 0xba, 0xfb, 0xf6, 0x1c, 0x0d, 0xcd, 0xc9, 0xd2, 0x4d, 0x85, 0x91,
	0x2e, 0x95, 0x15, 0x65, 0xa8, 0x24, 0xbf, 0x9f, 0x85, 0x54, 0xe9, 0xd1,
	0x86, 0x31, 0xd2, 0x89, 0x22, 0x5d, 0x7c, 0xb3, 0xd8, 0x8b, 0x9a, 0x1e,
	0x6b, 0x78, 0xf0, 0xc6, 0xcf, 0xc2, 0x82, 0x09, 0x9b, 0x0c, 0xb2, 0xc8,
	0x24, 0xdb, 0x36, 0xe0, 0x14, 0xdd, 0x54, 0x27, 0xfd, 0x7d, 0xb7, 0xde,
	0x3b, 0xb2, 0x45, 0x83, 0xae, 0x9b, 0xa6, 0x94, 0x61, 0x75, 0x4a, 0xb8,
	0x6d, 0x8d, 0x12, 0x2b, 0x57, 0x5e, 0xd6, 0xe4, 0x16, 0xf5, 0x19, 0xdf,
	0xe4, 0xff, 0xca, 0x0d, 0xab, 0x9b, 0xa4, 0x3f, 0x47, 0x75, 0xf1, 0x83,
	0xed, 0x5e, 0xd4, 0xf4, 0x58, 0xc3, 0x83, 0x37, 0x7e, 0x16, 0x16, 0x4c,
	0xd8, 0x64, 0x90, 0x45, 0x26, 0xd9, 0xae, 0x34, 0x6c, 0x1b, 0xd8, 0x5c,
	0x2f, 0xe9, 0x88, 0x4e, 0x1e, 0xd9, 0xa1, 0xbe, 0x09, 0xb7, 0xcb, 0x4d,
	0xfe, 0x85, 0xdc, 0xea, 0x0f, 0xe5, 0x7e, 0x7c, 0xee, 0xb2, 0x9a, 0xed,
	0xf7, 0xc5, 0xc7, 0xe5, 0x7a, 0xdd, 0xa1, 0xd6, 0xd5, 0x0f, 0x49, 0x7f,
	0x3d, 0xa0, 0x8b, 0xe7, 0x5f, 0xf5, 0xa2, 0xa6, 0xc7, 0x9a, 0xf7, 0xe0,
	0x8d, 0x9f, 0x85, 0x65, 0x4c, 0xd8, 0x64, 0x90, 0x45, 0x26, 0xd9, 0xdd,
	0x36, 0x10, 0x7b, 0x7b, 0x87, 0x22, 0x29, 0xd3, 0xe5, 0xbe, 0xbe, 0x4f,
	0x2e, 0x2a, 0xb9, 0x27, 0xe2, 0xf4, 0xb4, 0x69, 0xdd, 0x5f, 0xe4, 0x7a,
	0xd7, 0xa8, 0x75, 0x55, 0xcb, 0x95, 0x1b, 0xb0, 0x1e, 0x6b, 0xde, 0x83,
	0x37, 0x7e, 0x16, 0x96, 0x31, 0x61, 0x93, 0xd1, 0x7d, 0x03, 0x9c, 0x82,
	0x8d, 0x9c, 0x82, 0x5d, 0x8a, 0x1d, 0x7e, 0xcc, 0x4c, 0x53, 0xe5, 0x6a,
	0xf7, 0xc8, 0xd9, 0x91, 0x75, 0x9b, 0xe3, 0xb4, 0xcd, 0xb4, 0xfa, 0x4f,
	0xed, 0x1b, 0x58, 0xf1, 0xc3, 0x2b, 0x4e, 0x01, 0x3d, 0xbf, 0x01, 0x3c,
	0x78, 0xe3, 0x67, 0x61, 0x19, 0x13, 0x36, 0x19, 0x64, 0x91, 0x49, 0xb6,
	0x6d, 0x20, 0x49, 0xd1, 0x95, 0xd7, 0x4a, 0xef, 0x17, 0x2b, 0xf6, 0xe2,
	0x60, 0x45, 0xfa, 0xde, 0x6a, 0xbb, 0xdd, 0x2f, 0xd7, 0x66, 0x43, 0x8f,
	0xc7, 0x09, 0xc8, 0xda, 0x8f, 0xe5, 0x12, 0xe6, 0xa9, 0x75, 0xc9, 0x0d,
	0x57, 0x5c, 0x84, 0xf4, 0x58, 0xf3, 0x1e, 0xbc, 0xf1, 0xb3, 0xb0, 0x8c,
	0x09, 0x9b, 0x0c, 0xb2, 0xc8, 0x24, 0xdb, 0x95, 0x66, 0x07, 0x14, 0x6d,
	0x4e, 0x93, 0x62, 0xe9, 0x8a, 0xed, 0xc8, 0xb1, 0x5d, 0x4e, 0x94, 0x1b,
	0xf7, 0x53, 0xb9, 0xba, 0xd7, 0xe5, 0x16, 0xbc, 0x7a, 0x59, 0xf7, 0xbc,
	0x26, 0x37, 0x6f, 0xa7, 0x9d, 0xe7, 0xb9, 0x16, 0x56, 0x24, 0x1d, 0x0d,
	0xea, 0xe2, 0xfe, 0x4c, 0x2f, 0x6a, 0x7a, 0xac, 0x79, 0x0f, 0xde, 0xf8,
	0x59, 0x58, 0xc6, 0x84, 0x4d, 0x06, 0x59, 0x64, 0x92, 0x6d, 0xdf, 0x82,
	0x80, 0x9e, 0x5b, 0x65, 0xdf, 0xe5, 0xd3, 0xd7, 0xe8, 0xd4, 0x4b, 0xf9,
	0x8a, 0x04, 0x2b, 0xe5, 0x86, 0x34, 0xca, 0xcd, 0xd8, 0x22, 0x37, 0x65,
	0xc3, 0x65, 0x4d, 0xdf, 0x24, 0xf7, 0x95, 0x75, 0x76, 0xa5, 0xcf, 0xd2,
	0xc6, 0xa5, 0xf6, 0x57, 0xbc, 0x1b, 0xd1, 0x47, 0x07, 0x73, 0xbd, 0xa8,
	0xe9, 0xb1, 0xe6, 0x3d, 0x78, 0xe3, 0x67, 0x61, 0x19, 0x13, 0x36, 0x19,
	0x64, 0x91, 0x49, 0xb6, 0x3f, 0x02, 0x4f, 0x37, 0x65, 0xe9, 0x93, 0x63,
	0x39, 0x3a, 0xde, 0x96, 0xa7, 0xac, 0x54, 0xdb, 0x40, 0x71, 0x83, 0x5c,
	0xd5, 0x46, 0xb9, 0xca, 0xf5, 0xa6, 0x47, 0xda, 0x35, 0xd9, 0x8e, 0xca,
	0x78, 0xbb, 0xd2, 0x2d, 0x64, 0xfd, 0xbd, 0x83, 0xf5, 0xc9, 0x5b, 0x61,
	0xfd, 0x61, 0x5f, 0x3f, 0x2f, 0x6a, 0x7a, 0x7e, 0x03, 0x78, 0xf0, 0x76,
	0xce, 0xc1, 0x80, 0x65, 0x4c, 0xd8, 0x64, 0x90, 0x45, 0xa6, 0x3f, 0x02,
	0xa3, 0xfb, 0x05, 0xb4, 0x62, 0x6e, 0x48, 0x6f, 0x2c, 0x4f, 0xd3, 0xb6,
	0xba, 0x1c, 0x05, 0x12, 0xab, 0x0c, 0x64, 0x17, 0x93, 0x9b, 0xdd, 0x0e,
	0x74, 0x33, 0x4c, 0xd5, 0x71, 0x9a, 0xa9, 0xc5, 0xd5, 0x83, 0x74, 0xa8,
	0x39, 0xa8, 0x3d, 0x8d, 0x59, 0x5e, 0xd4, 0xf4, 0x58, 0xeb, 0xee, 0x65,
	0x16, 0x06, 0xac, 0x1a, 0xcf, 0x26, 0x83, 0x2c, 0x32, 0xc9, 0x76, 0x37,
	0xe5, 0x06, 0xd4, 0x34, 0x33, 0xa8, 0x97, 0xef, 0x4b, 0xd7, 0xf6, 0x7b,
	0x32, 0xb5, 0x78, 0xda, 0x40, 0xad, 0x5a, 0x34, 0x58, 0x9b, 0x96, 0x0e,
	0x50, 0xeb, 0x03, 0x23, 0xd5, 0xda, 0xd2, 0xa4, 0xd6, 0x35, 0xeb, 0xda,
	0xf5, 0x50, 0x93, 0xb6, 0x2c, 0x1b, 0xad, 0xef, 0x56, 0x67, 0xa8, 0xb6,
	0x34, 0xa0, 0xf9, 0x5f, 0x4a, 0xf1, 0xa2, 0xa6, 0xc7, 0x1a, 0x9e, 0x2e,
	0x3f, 0xb3, 0xc6, 0x80, 0x05, 0x13, 0x36, 0x19, 0x64, 0x91, 0x49, 0xb6,
	0x1d, 0x81, 0x24, 0xad, 0x98, 0x1d, 0xd2, 0xbe, 0xa5, 0x99, 0x7a, 0xc9,
	0x16, 0x5e, 0xab, 0x0f, 0xea, 0xa3, 0x3d, 0x59, 0xfe, 0xfe, 0xae, 0xd3,
	0x03, 0xa5, 0x4b, 0x2f, 0xda, 0xf7, 0xf6, 0xb7, 0xed, 0xfa, 0xd4, 0xea,
	0x0b, 0xd7, 0xeb, 0x7b, 0x33, 0x13, 0xd4, 0xdf, 0xf5, 0xd1, 0xb0, 0xf4,
	0x24, 0x2f, 0x6a, 0x7a, 0xac, 0x79, 0x4f, 0xa7, 0x9f, 0x59, 0x18, 0xc6,
	0x82, 0x09, 0x9b, 0x0c, 0xb2, 0xc8, 0x24, 0xdb, 0x1f, 0x81, 0x86, 0x19,
	0x41, 0x3d, 0xfb, 0x9d, 0x0c, 0x3d, 0xb9, 0x28, 0x5d, 0xdb, 0xee, 0x4a,
	0xd3, 0xe1, 0x4d, 0x61, 0x9d, 0x7b, 0x25, 0x43, 0x67, 0x76, 0x17, 0xe9,
	0xcc, 0xdb, 0x1b, 0x74, 0xe6, 0x58, 0x9b, 0xd7, 0x59, 0xab, 0xcf, 0xef,
	0x1b, 0xa0, 0xfa, 0xb9, 0x49, 0x9a, 0x50, 0x14, 0x50, 0xd5, 0xf0, 0xbe,
	0x5e, 0xd4, 0xf4, 0x58, 0xc3, 0xd3, 0xe9, 0xf7, 0xb3, 0xc6, 0x80, 0x05,
	0x13, 0x36, 0x19, 0x64, 0x91, 0xe9, 0x8f, 0x80, 0x3d, 0x2e, 0x55, 0xf1,
	0x85, 0x64, 0x4d, 0x28, 0x4c, 0xd6, 0x2d, 0xa6, 0xca, 0xe2, 0x44, 0x55,
	0x0d, 0x49, 0xd0, 0xd4, 0x21, 0xbd, 0x34, 0x65, 0x68, 0x82, 0xa6, 0xdc,
	0x90, 0x66, 0xca, 0xf0, 0xaa, 0x1a, 0x1a, 0xd2, 0xc4, 0xc2, 0xde, 0x7a,
	0xae, 0x39, 0x5d, 0x1f, 0xff, 0x26, 0x57, 0x17, 0xec, 0x02, 0x44, 0xd4,
	0xf4, 0x58, 0xc3, 0xd3, 0xe9, 0xf7, 0xb3, 0xc6, 0x80, 0x05, 0x13, 0x36,
	0x19, 0x64, 0x91, 0x49, 0xb6, 0x2b, 0xeb, 0x1f, 0xd0, 0x82, 0x5b, 0x52,
	0xb5, 0x6a, 0x4e, 0xaa, 0xee, 0x9d, 0x92, 0xa1, 0xdc, 0x94, 0x12, 0x85,
	0x06, 0xcc, 0x56, 0xb0, 0x6c, 0x81, 0x82, 0x23, 0xbf, 0xa1, 0x60, 0xc9,
	0x9d, 0xa6, 0xb9, 0x0a, 0x7e, 0x11, 0xd5, 0x2a, 0x79, 0xd4, 0x42, 0xd5,
	0xcc, 0x18, 0xa1, 0xe5, 0xf3, 0xfb, 0xaa, 0x61, 0x5e, 0x9a, 0x17, 0x35,
	0x3d, 0xd6, 0xf0, 0x78, 0x2f, 0x33, 0xcc, 0xc2, 0x30, 0x16, 0x4c, 0xd8,
	0x64, 0x90, 0x45, 0x26, 0xd9, 0xfe, 0x1a, 0x58, 0x65, 0x57, 0xe4, 0x21,
	0xbb, 0x32, 0xb7, 0xd7, 0x65, 0x2b, 0xb5, 0xcf, 0x64, 0xb9, 0x11, 0x2b,
	0xe4, 0xe6, 0xef, 0xb6, 0x1b, 0xca, 0x41, 0xb9, 0xbb, 0x3b, 0x44, 0xbd,
	0xf0, 0x4d, 0xb9, 0x59, 0xcf, 0x2b, 0xaf, 0x78, 0x9c, 0x6e, 0x0c, 0x3a,
	0x8d, 0xc8, 0x4c, 0xf2, 0xa2, 0xa6, 0xc7, 0x9a, 0xf7, 0xf4, 0x9c, 0x83,
	0x65, 0x4c, 0xd8, 0x64, 0x90, 0x45, 0x66, 0xd7, 0x35, 0xb0, 0x7c, 0x56,
	0x48, 0xaf, 0x37, 0xa6, 0xe9, 0x67, 0x0b, 0xb3, 0x95, 0x9e, 0x34, 0x49,
	0x6e, 0x70, 0xbd, 0xdd, 0xbd, 0x0e, 0xc8, 0x6d, 0x8d, 0xbb, 0x9d, 0x52,
	0x6f, 0xb8, 0x24, 0x57, 0xd3, 0xa6, 0x41, 0x43, 0xc7, 0x68, 0x42, 0x7f,
	0xa7, 0xf2, 0x82, 0x64, 0x2f, 0x6a, 0x7a, 0xac, 0x79, 0x4f, 0xcf, 0x39,
	0x58, 0xc6, 0x84, 0x4d, 0x06, 0x59, 0x64, 0xfa, 0x6b, 0xe0, 0xdf, 0x6e,
	0xe0, 0x5b, 0xf6, 0x3c, 0x00, 0xd0, 0xf9, 0x40, 0xa1, 0x5e, 0xff, 0xb7,
	0xab, 0x6f, 0x00, 0x4f, 0xcf, 0x39, 0x58, 0x9f, 0x79, 0x03, 0xdf, 0xfe,
	0xb5, 0xdc, 0x53, 0x36, 0xdc, 0xf9, 0x48, 0xa5, 0xde, 0xf8, 0x8f, 0xab,
	0x6f, 0x00, 0x4f, 0xcf, 0x39, 0x58, 0x57, 0xdf, 0x40, 0x48, 0x8f, 0xdf,
	0x95, 0xad, 0x44, 0xf7, 0x55, 0xb9, 0x81, 0x0f, 0xda, 0x79, 0xb3, 0x77,
	0x82, 0x07, 0x3e, 0x90, 0xfb, 0xc1, 0x29, 0xd3, 0x7b, 0x72, 0xf5, 0xf6,
	0xf3, 0xfb, 0x27, 0xec, 0x3e, 0xbf, 0x5d, 0x05, 0x03, 0x4b, 0x55, 0x96,
	0xe9, 0x34, 0x2a, 0xbb, 0x5d, 0xd4, 0xf4, 0x58, 0xf3, 0x1e, 0xbc, 0xcc,
	0x30, 0x0b, 0x03, 0x96, 0x31, 0x61, 0x93, 0x41, 0x56, 0xd7, 0x06, 0xb8,
	0x10, 0xf8, 0xe5, 0x57, 0x0d, 0x69, 0x7a, 0x62, 0x51, 0x44, 0x85, 0xd9,
	0x5f, 0x56, 0xa4, 0xfc, 0x7e, 0x45, 0xee, 0xfc, 0x89, 0x22, 0x77, 0xb4,
	0x28, 0x32, 0xab, 0xd5, 0xb4, 0xb1, 0xfd, 0x67, 0x8d, 0xfd, 0xac, 0x7d,
	0x52, 0x63, 0x2a, 0x26, 0x69, 0xea, 0xb0, 0x74, 0xdd, 0x36, 0x22, 0xc7,
	0x8b, 0x9a, 0x1e, 0x6b, 0xde, 0x13, 0x3f, 0x03, 0x03, 0x96, 0x31, 0x61,
	0x93, 0x41, 0x16, 0x99, 0xfe, 0x22, 0xe4, 0x9f, 0x95, 0x73, 0x42, 0xda,
	0x7b, 0x7f, 0xa6, 0x5e, 0x58, 0x92, 0xa9, 0x77, 0xa2, 0xd7, 0xea, 0xd4,
	0x2b, 0x45, 0x8a, 0xbd, 0x9c, 0xa7, 0xd8, 0xde, 0x12, 0xc5, 0xde, 0x79,
	0x56, 0xb1, 0x93, 0x7b, 0x15, 0x3b, 0xf1, 0xbc, 0xe9, 0x05, 0xc5, 0xde,
	0xdd, 0xa9, 0xf7, 0x8f, 0x3f, 0xa3, 0x33, 0x47, 0x9f, 0xd2, 0xe9, 0x63,
	0x3f, 0xf7, 0xa2, 0xa6, 0xc7, 0x9a, 0xf7, 0xe0, 0x65, 0x86, 0x59, 0x18,
	0xc6, 0x82, 0x09, 0x9b, 0x0c, 0xb2, 0xc8, 0xf4, 0x1b, 0xa8, 0xb0, 0x73,
	0x58, 0x6d, 0x77, 0xb3, 0x86, 0xea, 0xa0, 0xf6, 0x2f, 0xb3, 0x5b, 0xf0,
	0x31, 0x7b, 0x75, 0xfe, 0x7d, 0xb6, 0xbd, 0x64, 0x64, 0xd8, 0x63, 0xd6,
	0xea, 0xb3, 0x95, 0xf6, 0xc6, 0x63, 0x6f, 0x31, 0x97, 0xde, 0xb2, 0x37,
	0x99, 0x03, 0xa6, 0xfd, 0x56, 0x1f, 0xb4, 0x5b, 0xee, 0x61, 0xd3, 0xa1,
	0x0e, 0x59, 0x4d, 0x8f, 0x35, 0x3c, 0x78, 0x99, 0x61, 0x16, 0x06, 0x2c,
	0x98, 0xc6, 0x26, 0x83, 0x2c, 0x32, 0xc9, 0xe6, 0x4e, 0xb8, 0x75, 0x78,
	0x56, 0xe2, 0xe9, 0x69, 0xc3, 0x92, 0xd5, 0x62, 0x37, 0x95, 0x67, 0xec,
	0x31, 0xf9, 0x4b, 0x7b, 0x56, 0x47, 0x57, 0x98, 0x9a, 0x33, 0x14, 0x5d,
	0xd6, 0x5b, 0xd1, 0x47, 0x66, 0x28, 0xba, 0xa5, 0xd1, 0xde, 0xe3, 0xee,
	0xfb, 0xef, 0x84, 0x97, 0x19, 0x66, 0x61, 0x18, 0x0b, 0x26, 0x6c, 0x32,
	0xc8, 0x22, 0x93, 0x6c, 0xff, 0xbf, 0xa3, 0xf2, 0xbc, 0xa4, 0xc7, 0x2a,
	0x0a, 0x02, 0x1a, 0x6d, 0x17, 0x45, 0x69, 0x8e, 0xc9, 0x9e, 0xd3, 0xdd,
	0x14, 0xe9, 0xed, 0xdf, 0xdf, 0x3e, 0x9b, 0x6c, 0xa6, 0x27, 0xc7, 0xd8,
	0x64, 0x90, 0x45, 0xa6, 0x0f, 0xb7, 0xc7, 0xd6, 0xff, 0x54, 0xff, 0xb2,
	0xf9, 0xf9, 0x49, 0xee, 0x9f, 0xee, 0x74, 0x09, 0x07, 0xf2, 0x76, 0x5c,
	0x16, 0x00, 0x00, 0x00, 0x00, 0x49, 0x45, 0x4e, 0x44, 0xae, 0x42, 0x60,
	0x82
};

struct file_contents
{
	project_type m_Type;
    std::string m_ProjectNamespace;
    std::string m_ProjectName;
    std::string m_ProjectDescription;

    file_contents(project_type type, 
		std::string anamespace, 
		std::string aprojectname, 
		std::string adescription)
	: m_Type(type)
    , m_ProjectNamespace(anamespace)
    , m_ProjectName(aprojectname)
    , m_ProjectDescription(adescription)
    {}

std::string replace(std::string str, std::string sub1, const std::string& sub2)
{
    if (sub1.empty()) return str; 
    
    std::size_t pos;

    while ((pos = str.find(sub1)) != std::string::npos) str.replace(pos, sub1.size(), sub2);

    return str;
}

std::string replace_token(std::string str, std::string sub1, const std::string& sub2)
{
    sub1 = "{{" + sub1 + "}}";

    return replace(str, sub1, sub2);
}

std::string process_string_template(std::string template_string)
{
    std::string project_name = m_ProjectName;
    std::string project_short_description = m_ProjectDescription;

    auto now = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t(now);
    struct tm *parts = std::localtime(&now_c);

    template_string = replace_token(template_string, "year", std::to_string(1900 + parts->tm_year));

    std::string project_name_sanitized(replace(replace(project_name, "_", ""), "-", ""));

    template_string = replace_token(template_string, "project_namespace", m_ProjectNamespace);
    template_string = replace_token(template_string, "project_name", project_name);
    template_string = replace_token(template_string, "project_name_sanitized", project_name_sanitized);
    template_string = replace_token(template_string, "project_short_description", project_short_description);

    return template_string;
}


const std::string git_attributes = (process_string_template(
R"V0G0N(* text=auto
thirdparty/*/ linguist-vendored
)V0G0N"));

const std::string git_ignore = (process_string_template(
R"V0G0N(workspace/*

!.keep
!CMakeLists.txt
!*.sh

*.swp
)V0G0N"));

//TODO: parameterize the date etc
const std::string travis = (process_string_template(
R"V0G0N(# © {{year}} Joseph Cameron - All Rights Reserved
# {{project_name}} ci script

dist: bionic 
sudo: required
language: cpp

matrix:
  include:
    # Build scripts
    - os: linux
      name: build linux gcc
      compiler: gcc
      addons:
        apt:
          sources:
            - ubuntu-toolchain-r-test
          packages:
            - cmake
            - git
      script:
        - |
          cd workspace
          cmake .. -DJFC_BUILD_DOCS=OFF -DCMAKE_CXX_FLAGS="-pedantic -Wall -Wextra -Ofast -flto -funroll-loops -m64 -march=native"
          make
          ctest --extra-verbose

    - os: linux
      dist: bionic 
      arch: arm64
      name: build linux gcc
      compiler: gcc
      addons:
        apt:
          sources:
            - ubuntu-toolchain-r-test
          packages:
            - cmake
            - git
      script:
        - |
          cd workspace
          cmake .. -DJFC_BUILD_DOCS=OFF -DCMAKE_CXX_FLAGS="-pedantic -Wall -Wextra -Ofast -flto -funroll-loops -m64 -march=native"
          make
          ctest --extra-verbose

    - os: osx
      name: build macos clang
      osx_image: xcode12.2
      compiler: clang
      script:
        - |
          cd workspace
          cmake .. -DJFC_BUILD_DOCS=OFF -DCMAKE_CXX_FLAGS="-pedantic -Weverything -Wno-c++98-compat -Wno-padded -Ofast -flto -funroll-loops -m64 -march=native -std=c++17 -stdlib=libc++"
          make
          ctest --extra-verbose

    - os: windows
      name: build windows msvc
      compiler: MSVC
      script: 
        - |
          cd workspace
          cmake .. -DJFC_BUILD_DOCS=OFF #-DCMAKE_CXX_FLAGS=" /W4 /EHsc /WX /std:c++17 /permissive-"
          cmake --build .

#    - os: windows
#      name: build windows mingw
#      compiler: mingw64
#      install: choco install mingw
#      script: 
#        - |
#          cd workspace
#          cmake .. -G "MinGW Makefiles" -DJFC_BUILD_DOCS=OFF -DCMAKE_CXX_FLAGS="-pedantic -Wall -Wextra -Ofast -flto -funroll-loops -m64 -march=native"
#          cmake --build .

#    # Documentation 
#    - os: linux
#      name: generate doxygen, upload to gh-pages
#      if: branch = master
#      compiler: gcc
#      addons:
#        apt:
#          packages:
#            - doxygen
#      script:
#        - |
#          cd workspace;git submodule update --init --recursive
#          cmake .. -DJFC_BUILD_DEMO=OFF -DJFC_BUILD_DOCS=ON -DJFC_BUILD_TESTS=OFF
#          mv docs ~
#          REMOTE_URL=$(git config --get remote.origin.url | sed -e "s/^https:\/\///")
#          cd ~
#          git clone --branch=gh-pages "https://${GITHUB_PUBLIC_REPO_TOKEN}@$REMOTE_URL" gh-pages
#          cd gh-pages
#          mv ~/docs/* .
#          git add --all
#          git commit -m "updating docs"
#          git push
    
#    # Coverage
#    - os: linux
#      name: generate coverage, upload to coveralls
#      if: branch = master
#      compiler: gcc
#      before_install: pip install --user cpp-coveralls
#      script:
#        - |
#          cd workspace
#          cmake .. -DJFC_BUILD_DOCS=OFF -DCMAKE_CXX_FLAGS="-g -O0 -Wall -fprofile-arcs -ftest-coverage"
#          make
#          make test
#          cd ..
#          after_success:
#          - |
#            coveralls --exclude test --exclude workspace/CMakeFiles --exclude cmake --exclude src --gcov-options '\-lp'

before_script: 
  - |
    CURRENT_COMMIT_HASH="$(git rev-parse HEAD)"
)V0G0N"));

//TODO: parameterize proejct name, date, etc.
const std::string cmakelists_root = (process_string_template(m_Type == project_type::library
? R"V0G0N(# © {{year}} Joseph Cameron - All Rights Reserved

cmake_minimum_required(VERSION 3.9 FATAL_ERROR)

project(null)

include("${CMAKE_CURRENT_SOURCE_DIR}/cmake/jfc-cmake/jfclib.cmake")

option(JFC_BUILD_DEMO "Build the demo" ON)
option(JFC_BUILD_DOCS "Build documentation" OFF)
option(JFC_BUILD_TESTS "Build unit tests" ON)

add_subdirectory(thirdparty)

jfc_project(library
    NAME "{{project_name_sanitized}}"
    VERSION 0.0
    DESCRIPTION "{{project_short_description}}"
    C++_STANDARD 17
    C_STANDARD 90
    TYPE STATIC

    PUBLIC_INCLUDE_DIRECTORIES
        ${CMAKE_CURRENT_SOURCE_DIR}/include

    PRIVATE_INCLUDE_DIRECTORIES
        ${CMAKE_CURRENT_SOURCE_DIR}/src/include

    SOURCE_LIST
        ${CMAKE_CURRENT_SOURCE_DIR}/src/stub.cpp

#    DEPENDENCIES
#        "example"
)

if (JFC_BUILD_DEMO)
    add_subdirectory(demo)
endif()

if (JFC_BUILD_TESTS)
    add_subdirectory(test)
endif()

if (JFC_BUILD_DOCS)
    add_subdirectory(docs)
endif()
)V0G0N"
:
R"V0G0N(# © {{year}} Joseph Cameron - All Rights Reserved

cmake_minimum_required(VERSION 3.9 FATAL_ERROR)

project(null)

include("${CMAKE_CURRENT_SOURCE_DIR}/cmake/jfc-cmake/jfclib.cmake")

option(JFC_BUILD_DOCS "Build documentation" OFF)
option(JFC_BUILD_TESTS "Build unit tests" OFF)

add_subdirectory(thirdparty)

jfc_project(executable
    NAME "{{project_name_sanitized}}"
    VERSION 0.0
    DESCRIPTION "{{project_short_description}}"
    C++_STANDARD 17
    C_STANDARD 90

    PUBLIC_INCLUDE_DIRECTORIES
        ${CMAKE_CURRENT_SOURCE_DIR}/include
#        ${dependencyname_INCLUDE_DIRECTORIES} 


    PRIVATE_INCLUDE_DIRECTORIES
        ${CMAKE_CURRENT_SOURCE_DIR}/src/include

#    LIBRARIES
#        ${dependencyname_LIBRARIES} 

    SOURCE_LIST
        ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp

#    DEPENDENCIES
#        "dependencyname"
)

if (JFC_BUILD_TESTS)
    add_subdirectory(test)
endif()

if (JFC_BUILD_DOCS)
    add_subdirectory(docs)
endif()
)V0G0N"));

//param name and copyright year
const std::string license = (process_string_template(
R"V0G0N(MIT License

Copyright (c) {{year}} Joseph Cameron

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
)V0G0N"));

// need project name etc.
const std::string readme = (process_string_template(
R"V0G0N([![Build Status](https://travis-ci.org/jfcameron/{{project_name}}.svg?branch=master)](https://travis-ci.org/jfcameron/{{project_name}}) [![Coverage Status](https://coveralls.io/repos/github/jfcameron/{{project_name}}/badge.svg?branch=master)](https://coveralls.io/github/jfcameron/{{project_name}}?branch=master) [![Documentation](https://img.shields.io/badge/documentation-doxygen-blue.svg)](https://jfcameron.github.io/{{project_name}}/)

## {{project_name}}

{{project_short_description}}. See demo/ for usage.
)V0G0N"));

// need project name etc.
const std::string cmake_demo = (process_string_template(
R"V0G0N(# © {{year}} Joseph Cameron - All Rights Reserved

cmake_minimum_required(VERSION 3.9 FATAL_ERROR)

jfc_project(executable
    NAME "{{project_name_sanitized}}-demo"
    VERSION 1.0
    DESCRIPTION "{{project_name}} demonstration"
    C++_STANDARD 17
    C_STANDARD 90

    SOURCE_LIST
        ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp

    PRIVATE_INCLUDE_DIRECTORIES
        "${{{project_name_sanitized}}_INCLUDE_DIRECTORIES}"

    LIBRARIES
        "${{{project_name_sanitized}}_LIBRARIES}"

    DEPENDENCIES
        "{{project_name_sanitized}}"
)

)V0G0N"));

const std::string demo = (process_string_template(R"V0G0N(// © {{year}} Joseph Cameron - All Rights Reserved

#include <cstdlib>

int main(int count, char **args)
{
    return EXIT_SUCCESS;
}
)V0G0N"));

const std::string cmake_docs = std::string(process_string_template(
R"V0G0N(# © {{year}} Joseph Cameron - All Rights Reserved

cmake_minimum_required(VERSION 3.9 FATAL_ERROR)

jfc_generate_documentation_doxygen(
    PROJECT_LOGO ${CMAKE_CURRENT_LIST_DIR}/icon.png

    README_PATH  ../README.md

    INCLUDE_DIRECTORIES
        ../include/
)
)V0G0N"));


const std::string cmake_test = (process_string_template(R"V0G0N(# © {{year}} Joseph Cameron - All Rights Reserved

jfc_add_tests(
    C++_STANDARD 17
    C_STANDARD 90

    TEST_SOURCE_FILES
        "${CMAKE_CURRENT_LIST_DIR}/example_test.cpp"

    INCLUDE_DIRECTORIES
        "${${PROJECT_NAME}_INCLUDE_DIRECTORIES}"

    LIBRARIES
        "${${PROJECT_NAME}_LIBRARIES}")
)V0G0N"));

const std::string test = (process_string_template(R"V0G0N(// © {{year}} Joseph Cameron - All Rights Reserved

#include <jfc/catch.hpp>
#include <jfc/types.h>

//using namespace {{project_namespace}};

TEST_CASE( "{{project_namespace}}::example test", "[{{project_namespace}}::example]" )
{
    SECTION("example does something correctly")
    {
        REQUIRE(true);
    }
}
)V0G0N"));

const std::string cmake_thirdparty = (process_string_template(R"V0G0N(# © {{year}} Joseph Cameron - All Rights Reserved

#set(dependencies
#    "example_dependency"
#)
#jfc_add_dependencies(${dependencies})

#add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/example_submodule")

)V0G0N"));

};

namespace fs = std::filesystem;

class writer
{
    //parameterize
    std::string m_project_name = "jfc-project";
    std::string m_project_namespace = "jfc";

    /// \brief creates directory, returns path to it
    fs::path create_dir(std::vector<std::string> aDirectory)
    {
        fs::path path = {m_project_name};

        for (const auto &name : aDirectory) path+= "/" + name;

        fs::create_directories(path);

        path += "/";

        return path;
    }

public:
    /// \brief writes binary to a file
    /// \remark creates the path to the file if the directories do not already exist
    void create_file(std::vector<std::string> aDirectory, std::string aFileName, std::vector<unsigned char> contents)
    {
        auto path = create_dir(aDirectory) += aFileName;

        std::ofstream ofs(path, std::ios_base::binary);

        for (const auto &a : contents) ofs << a;
    }

    /// \brief writes a string to a file
    /// \remark creates the path to the file if the directories do not already exist
    void create_file(std::vector<std::string> aDirectory, std::string aFileName, std::string contents)
    {
        auto path = create_dir(aDirectory) += aFileName;

        std::ofstream ofs(path);

        ofs << contents;
    }

    /// \brief runs a command from the specified directory
    /// \remark creates the path to the directory if it does not already exist
    void run_command(std::vector<std::string> aDirectory, std::string aCommand)
    {
        auto cwd = fs::current_path();
        
        fs::current_path(create_dir(aDirectory));

        if (auto rcode = std::system(aCommand.c_str())) 
            throw std::invalid_argument("command failed: " + 
                aCommand + ", return code: " + 
                std::to_string(rcode));
        
        fs::current_path(cwd);
    }
    
    writer(std::string aproject_name, std::string aproject_namespace)
    : m_project_name(aproject_name)
    , m_project_namespace(aproject_namespace)
    {}
};

int main(int count, char **args)
{
    try
    {
        if (count != 5) throw std::invalid_argument("needs 4 parameters: "
                "1: namespace, "
                "2: project name, "
                "3: single sentence description, "
                "4: [lib|exe]");

        std::string project_namespace(args[1]);
        std::string project_name(project_namespace + "-" + std::string(args[2]));
        std::string project_description(args[3]);

        if (project_description.size() > 40) throw std::invalid_argument("description too long");

        std::string project_type_raw(args[4]);

        project_type type;

        if (project_type_raw == "lib") type = project_type::library;
        else if (project_type_raw == "exe") type = project_type::executable;
        else throw std::invalid_argument("type must be lib or exe");

        writer w(project_name, project_namespace);
        file_contents c(type, project_namespace, project_name, project_description);

        w.run_command({}, "git init");

        //populate root
        w.create_file({}, ".gitattributes", c.git_attributes);
        w.create_file({}, ".gitignore", c.git_ignore);
        w.create_file({}, ".travis.yml", c.travis);
        w.create_file({}, "CMakeLists.txt", c.cmakelists_root);
        w.create_file({}, "LICENSE", c.license);
        w.create_file({}, "README.md", c.readme);

        //populate cmake
        w.run_command({"cmake"}, "git submodule add https://github.com/jfcameron/jfc-cmake.git");

        if (type == project_type::library)
        {
            //populate demo
            w.create_file({ "demo" }, "CMakeLists.txt", c.cmake_demo);
            w.create_file({ "demo" }, "main.cpp", c.demo);
        }

        //populate docs
        w.create_file({"docs"}, "CMakeLists.txt", c.cmake_docs);

        w.create_file({"docs"}, "icon.png", std::vector<unsigned char>(icon_png, icon_png + sizeof(icon_png) / sizeof(icon_png[0])));

        //populate include
        w.create_file({"include", project_namespace}, ".keep", "");

        //populate src
        w.create_file({"src", "include", project_namespace}, ".keep", "");

        if (type == project_type::library)
        {
            w.create_file({ "src" }, "stub.cpp", "");
        }
        else if (type == project_type::executable)
        {
            w.create_file({ "src" }, "main.cpp", c.demo);
        }

        //populate test
        w.create_file({"test"}, "CMakeLists.txt", c.cmake_test);
        w.create_file({"test"}, "example_test.cpp", c.test);

        //pop thirdparty
        w.create_file({"thirdparty"}, "CMakeLists.txt", c.cmake_thirdparty);

        //populate workspace
        w.create_file({"workspace"}, ".keep", "");

        w.run_command({}, "git add --all");
        w.run_command({}, "git commit -m \"initial commit\"");
    }
    catch (const std::runtime_error &e)
    {
        std::cerr << "error: " << e.what() << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << "error: " << e.what() << std::endl;
    }
    catch (...)
    {
        std::cerr << "catastrophic error" << std::endl;
    }

    return EXIT_SUCCESS;
}

