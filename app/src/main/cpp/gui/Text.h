/***************************************************************************
 *   Copyright (C) 2008 by jrs0ul                                          *
 *   jrs0ul@gmail.com                                                      *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#ifndef _BITMAP_TEXT_H
#define _BITMAP_TEXT_H

#include "../Colors.h"
#include "../TextureLoader.h"


//writes text
void WriteText(unsigned int x, unsigned int y,
               PicsContainer& pics, int font, const char* s,
               float scalex=1.0f, float scaley=1.0f,
               COLOR c1 = COLOR(1.0f, 1.0f, 1.0f, 1.0f),
               COLOR c2 = COLOR(1.0f, 1.0f, 1.0f, 1.0f));
void WriteShadedText(unsigned int x, unsigned int y,
               PicsContainer& pics, int font, const char* s,
               float scalex=1.0f, float scaley=1.0f,
               COLOR c1 = COLOR(1.0f, 1.0f, 1.0f, 1.0f),
               COLOR c2 = COLOR(1.0f, 1.0f, 1.0f, 1.0f),
               COLOR shade = COLOR(0.0, 0.0, 0.0, 1.0f));

void DrawNumber(int x, int y, int number, PicsContainer& pics, unsigned index);

#endif //_BITMAP_TEXT_H

