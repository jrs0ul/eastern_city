/***************************************************************************
 *   Copyright (C) 2008 by jrs0ul   *
 *   jrs0ul@gmail.com   *
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

#ifndef PARTICLES2D_H
#define PARTICLES2D_H

#include "TextureLoader.h"
#include "Vectors.h"
#include "Matrix.h"


struct Particle{
    Vector3D pos;
    COLOR c;
    float size;
    Vector3D velocity;
    int age;
    bool isdead;

    Particle(){
        pos = Vector3D(0,0,0);
        c = COLOR(1.0f, 0.0f, 0.0f, 0.8f);
        velocity=Vector3D(0,0,0);
        age=0;
        size = 1.0f;
        isdead=false;
    }
};


class Particle2DSystem{

    DArray<Particle> particles;
    float centerx;
    float centery;
    float centerz;
    int particleLifetime;

    Vector3D mainVelocity;
    int halfangle;
    Vector3D begin;
    //float velmat[16];

    COLOR start;
    COLOR end;
    float startSize;
    float endSize;
    
    int maxSystemLifetime;
    int defSystemLifetime;
    bool _isDead;

public:
    Particle2DSystem(){
        _isDead = true;
        halfangle = 180;
        maxSystemLifetime = 20;
    }

    void setPos(float x, float y, float z);
    void setParticleLifetime(int lifetime);
    void setSystemLifetime(int lifetime);
    void setDirIntervals(Vector3D  vel, int hangle);
    void setColors(COLOR _start,
                   COLOR _end);
    void setSizes(float _start, float _end);
    void updateSystem();
    void drawParticles(PicsContainer& pics,
                       int picIndex, Vector3D shift = Vector3D(0,0,0));
    void destroy();
    bool isDead(){return _isDead;}
    void revive(){_isDead = false;}

};


#endif //PARTICLES2D_H
