/*
* @Author: nhantt
* @Date:   2019-12-13
* @Last Modified by:   nhantt
* @Last Modified time: 2019-12-13
*/

#ifndef __BITMAP_H__
#define __BITMAP_H__

#include <stdint.h>
#include "main.h"

/* bitmap struct */
typedef struct {
	uint8_t width, height;
	const uint8_t *data;
} BITMAP;


/* variable declare*/
extern const BITMAP bmp_logo_ph;
extern const BITMAP bmp_logo_smartph;

extern const BITMAP bmp_pump;
extern const BITMAP bmp_distributor;
extern const BITMAP bmp_setting;
extern const BITMAP bmp_io_setting;
extern const BITMAP bmp_database;
extern const BITMAP bmp_system;
extern const BITMAP bmp_tools;
extern const BITMAP bmp_user_setting;
extern const BITMAP bmp_samling;
extern const BITMAP bmp_program_setting;
extern const BITMAP bmp_cool_temp;
extern const BITMAP bmp_history;
extern const BITMAP bmp_system_setting;

#endif