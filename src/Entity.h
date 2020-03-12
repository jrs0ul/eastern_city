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
#ifndef _ENTITY_H
#define _ENTITY_H

#include "Vectors.h"


class Entity{

    Vector3D _pos;
    Vector3D _direction;
    Vector3D _up;
    Vector3D _right;

public:
    Entity();
    Vector3D pos(){return _pos;}
    Vector3D dir(){return _direction;}
    Vector3D up(){return _up;}
    Vector3D right(){return _right;}
    void setpos(Vector3D newpos);
    void setdir(Vector3D newdir){_direction = newdir;}
    void setup(Vector3D newup){_up = newup;}
    void setright(Vector3D newright){_right = newright;}

    void move(float d);
    void strife(float d);
    void yaw(float angle, bool use_up = true);
    void pitch(float angle, bool use_right = true);
    void roll(float angle, bool use_dir = true);
    void fly(float d, bool use_up = true);
    void makeMatrix(float* matrix);
    void makeBillBoard(Vector3D objectPos, float * matrix);


};



#endif //_ENTITY_H
