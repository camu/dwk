// See LICENSE for copyrite stuff

#ifndef __IMG_H
#define __IMG_H

#include <X11/Xutil.h>

/* return value:
	0 on success
	1 if the file (name) sucks (i.e. it's named "picture" or "picture.stupidfiletype"; only "picture.okfiletype" is ok)
	2 if the file couldn be loaded by fopen
	3 if the size is not valid png
	4 if bad color type
*/
int load_img( const char *_file, XImage **_ximg );

int load_png( const char *_file, XImage **_ximg );

#endif
