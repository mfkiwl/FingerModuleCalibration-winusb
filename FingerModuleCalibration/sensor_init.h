#include "stdafx.h"

#define _301_ID		0xA9
#define _301_ID2	0xAC
#define _301_ID3	0x6A
#define _301_ID4	0x6E
#define _301_ID5	0xAD
#define _301_ID6	0x23

#define _305_ID		0x93	// 301c
#define _305_ID2	0x81
#define _305_ID3	0x92

#define _301_FRAME_WIDTH        128
#define _301_FRAME_HEIGHT       128
#define _301_FRAME_SWIPE_HEIGHT	  8
#define _301_FRAME_SIZE         (_301_FRAME_WIDTH * _301_FRAME_HEIGHT)
#define _301_SWIPE_FRAME_SIZE	(_301_FRAME_WIDTH * _301_FRAME_HEIGHT)

#define _305_FRAME_WIDTH		120
#define _305_FRAME_HEIGHT		120
#define _305_FRAME_SIZE			(_305_FRAME_WIDTH * _305_FRAME_HEIGHT)

#define _501_FRAME_WIDTH		166
#define _501_FRAME_HEIGHT		170
#define _501_FRAME_SIZE			(_501_FRAME_WIDTH * _501_FRAME_HEIGHT)


static UCHAR reset_table[] = {
	0xD2, 0x10,  // sleep mode
	0xD1, 0x80   // soft reset, delay must put here
};

static UCHAR _301_regs_table[] = {
	0xE0, 0x02,  // AMP
	0xE1, 0x1A,  // VRT 1.60V
	0xE2, 0x10,  // VRB 0.100V + 0.025V * 0x21
	0xE6, 0x12   // DC offset, VCC=3.0V use 0x10, VCC=3.3V use 0x15
};

static UCHAR _301_touch_init_table[] = {
	0xD2, 0x10,
	0xC0, 0x00,
	0xC1, 0x40,
	0xC2, 0x7F,
	0xC3, 0x00,
	0xC4, 0x7F,
	0xC8, 0x00,
	0xC9, 0x20,
	0xD4, 0x22,
	0xD5, 0x10,
	0xD6, 0x20,
	0xD2, 0x13
};

static UCHAR _301_swipe_init_table[] = {
	0xD2, 0x10,		/* sleep mode */
	0xC0, 0x3C,		/* Row start */
	0xC1, 0x40,		/* Row offset */
	0xC2, 0x43,		/* Row end */
	0xC3, 0x00,		/* Column start */
	0xC4, 0x7F,		/* Column end */
	0xC8, 0x00,		/* Low byte of total pixels */
	0xC9, 0x02,		/* High byte of total pixels */
	0xD4, 0x20,		/* FPS area select */
	0xD5, 0x10,
	0xD6, 0x00,
	0xD2, 0x13		/* Sensor mode */
};

static UCHAR _305_touch_init_table[] = {
	0xD2, 0x10,
	0x30, 0x0A,
	0x31, 0x0A,
	0x32, 0x07,
	0x35, 0x08,
	0x36, 0x8C,
	0x37, 0x64,
	0x38, 0x07,
	0xE0, 0x02,  // AMP
	0xE1, 0x1A,  // VRT 1.60V
	0xE2, 0x10,  // VRB 0.100V + 0.025V * 0x21
	0xE6, 0x12,  // DC offset, VCC=3.0V use 0x10, VCC=3.3V use 0x15
	0x3A, 0x80,
	0x3B, 0x02,
	0xD2, 0x13,
	0xD3, 0x00   // clear status, disable DVR, VRB calibration
};

static UCHAR _501_touch_init_table[] = {
	0xD3, 0x00,  // clear status, disable DVR, VRB calibration
	0xD7, 0x51,  // ?
	0xD4, 0x52,  // ?
	0xDF, 0x10,  // bit4: EN_Hold_tgen 1 Frame rate up
	0xF0, 0x01,  // engineer mode
	0xE0, 0x02,  // gain
	0xE1, 0x35,  // vrt
	0xE2, 0x40,  // vrb
	0xE3, 0x04,  // ?
	0xE4, 0x10,  // ?
	0xE5, 0x6C,  // ?
	0xE6, 0x10,  // dc offset

	0xE7, 0xF5,  // ?
	0xF2, 0x08,  // ?
	0xF3, 0x80,  // ?
	0xF4, 0x10,  // ?
	0xD5, 0x20,  // ?
	0xC0, 0x00,  // start row (0)
	0xC1, 0x53,  // middle row (83)
	0xC2, 0xA5,  // end row (165)
	0xC3, 0x00,  // start col (0)
	0xC4, 0xA9,  // end col (169)
	0xD6, 0x00,  // ?
	0xD2, 0x13,  // sensor mode
	0xE9, 0x03,  // ?
	0xED, 0x5A   // ?
};

static UCHAR _501_swipe_init_table[] = {
	0xD2, 0x10,  // sleep mode
	0xD4, 0x50,  // ?
	0xDF, 0x10,  // bit4: EN_Hold_tgen 1 Frame rate up
	0xC0, 0x4F,  // start row (79)
	0xC1, 0x53,  // middle row (83)
	0xC2, 0x56,  // end row (86)
	0xC3, 0x00,  // start col (0)
	0xC4, 0xA9,  // end col (169)
	0xC9, 0x02,  // ?
	0xC8, 0xA8,  // ?
	0xD2, 0x13,  // sensor mode
	0xE0, 0x02,  // gain
	0xE1, 0x35,  // vrt
	0xE2, 0x40,  // vrb
	0xE6, 0x10   // dc offset
};