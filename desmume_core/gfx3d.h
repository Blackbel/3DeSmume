/*
	Copyright (C) 2008 DeSmuME team

    This file is part of DeSmuME

    DeSmuME is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    DeSmuME is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with DeSmuME; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#ifndef _GFX3D_H_
#define _GFX3D_H_

#include "types.h"

void gfx3d_init();
void gfx3d_reset();

#define CACHE_ALIGN 

typedef struct {
	int type; //tri or quad
	u16 vertIndexes[4]; //up to four verts can be referenced by this poly
	u32 polyAttr, texParam, texPalette; //the hardware rendering params
	int projIndex; //the index into the projlist that this poly uses
	u32 pad;
}POLY;

#define POLYLIST_SIZE 100000
//#define POLYLIST_SIZE 2048
typedef struct {
	POLY *list[POLYLIST_SIZE];
	int count;
}POLYLIST;

#define PROJLIST_SIZE 1000

typedef struct {
	float projMatrix[PROJLIST_SIZE][16];
	int count;
}PROJLIST;

typedef struct {
	float coord[4];
	float texcoord[2];
	u32 depth;
	u8 color[4];
}VERT;

#define VERTLIST_SIZE 400000
//#define VERTLIST_SIZE 10000
typedef struct {
	VERT *list[VERTLIST_SIZE];
	int count;
}VERTLIST;



//used to communicate state to the renderer
typedef struct
{

	int enableTexturing;
	int enableAlphaTest;
	int enableAlphaBlending;
	int enableAntialiasing; 
	int enableEdgeMarking;

	u32 shading;

	POLYLIST *polylist;
	VERTLIST *vertlist;
	PROJLIST *projlist;
	int indexlist[POLYLIST_SIZE];

	BOOL wbuffer, sortmode;

	float alphaTestRef;

	int x;
    int y;
    int width;
    int height;

	float clearColor[4];
	float clearDepth;
	float fogColor[4];
	float fogOffset;


	u32 rgbToonTable[32];
}GFX3D;
//extern GFX3D gfx3d;

static const u32 TOON = 0;
static const u32 HIGHLIGHT = 1;
	

//---------------------

//produce a 32bpp color from a DS RGB16
#define RGB16TO32(col,alpha) (((alpha)<<24) | ((((col) & 0x7C00)>>7)<<16) | ((((col) & 0x3E0)>>2)<<8) | (((col) & 0x1F)<<3))

//produce a 32bpp color from a ds RGB15 plus an 8bit alpha, using a table
#define RGB15TO32(col,alpha8) ( ((alpha8)<<24) | color_15bit_to_24bit[col] )

//produce a 32bpp color from a ds RGB15 plus an 8bit alpha, not using a table (but using other tables)
#define RGB15TO32_DIRECT(col,alpha8) ( ((alpha8)<<24) | (material_5bit_to_8bit[((col)>>10)&0x1F]<<16) | (material_5bit_to_8bit[((col)>>5)&0x1F]<<8) | material_5bit_to_8bit[(col)&0x1F] )

CACHE_ALIGN u32 color_15bit_to_24bit[32768];
CACHE_ALIGN u8 mixTable555[32][32][32];
CACHE_ALIGN const int material_5bit_to_31bit[32];
CACHE_ALIGN const u8 material_5bit_to_8bit[32];
CACHE_ALIGN const u8 material_3bit_to_8bit[8];

//GE commands:
void gfx3d_glViewPort(unsigned long v);
void gfx3d_glClearColor(unsigned long v);
void gfx3d_glFogColor(unsigned long v);
void gfx3d_glFogOffset (unsigned long v);
void gfx3d_glClearDepth(unsigned long v);
void gfx3d_glMatrixMode(unsigned long v);
void gfx3d_glLoadIdentity();
BOOL gfx3d_glLoadMatrix4x4(signed long v);
BOOL gfx3d_glLoadMatrix4x3(signed long v);
void gfx3d_glStoreMatrix(unsigned long v);
void gfx3d_glRestoreMatrix(unsigned long v);
void gfx3d_glPushMatrix(void);
void gfx3d_glPopMatrix(signed long i);
BOOL gfx3d_glTranslate(signed long v);
BOOL gfx3d_glScale(signed long v);
BOOL gfx3d_glMultMatrix3x3(signed long v);
BOOL gfx3d_glMultMatrix4x3(signed long v);
BOOL gfx3d_glMultMatrix4x4(signed long v);
void gfx3d_glBegin(unsigned long v);
void gfx3d_glEnd(void);
void gfx3d_glColor3b(unsigned long v);
BOOL gfx3d_glVertex16b(unsigned int v);
void gfx3d_glVertex10b(unsigned long v);
void gfx3d_glVertex3_cord(unsigned int one, unsigned int two, unsigned int v);
void gfx3d_glVertex_rel(unsigned long v);
void gfx3d_glSwapScreen(unsigned int screen);
int gfx3d_GetNumPolys();
int gfx3d_GetNumVertex();
void gfx3d_glPolygonAttrib (unsigned long val);
void gfx3d_glMaterial0(unsigned long val);
void gfx3d_glMaterial1(unsigned long val);
BOOL gfx3d_glShininess (unsigned long val);
void gfx3d_UpdateToonTable(void* toonTable);
void gfx3d_glTexImage(unsigned long val);
void gfx3d_glTexPalette(unsigned long val);
void gfx3d_glTexCoord(unsigned long val);
void gfx3d_glNormal(unsigned long v);
signed long gfx3d_GetClipMatrix (unsigned int index);
signed long gfx3d_GetDirectionalMatrix (unsigned int index);
void gfx3d_glLightDirection (unsigned long v);
void gfx3d_glLightColor (unsigned long v);
void gfx3d_glAlphaFunc(unsigned long v);
BOOL gfx3d_glBoxTest(unsigned long v);
BOOL gfx3d_glPosTest(unsigned long v);
void gfx3d_glVecTest(unsigned long v);
void gfx3d_glGetPosRes(unsigned int index);
void gfx3d_glGetVecRes(unsigned int index);
void gfx3d_glFlush(unsigned long v);
void gfx3d_VBlankSignal();
void gfx3d_VBlankEndSignal();
void gfx3d_Control(unsigned long v);
u32 gfx3d_GetGXstatus();
void gfx3d_sendCommandToFIFO(u32 val);
void gfx3d_sendCommand(u32 cmd, u32 param);

//other misc stuff
void gfx3d_glGetMatrix(unsigned int mode, int index, float* dest);
void gfx3d_glGetLightDirection(unsigned int index, unsigned int* dest);
void gfx3d_glGetLightColor(unsigned int index, unsigned int* dest);

#endif
