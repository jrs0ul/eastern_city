/*
 The Disarray 
 by jrs0ul(jrs0ul ^at^ gmail ^dot^ com) 2010
 -------------------------------------------
 Color data
 mod. 2010.09.27
 */
#ifndef _COLORS_H__
#define _COLORS_H__

#include <cstring>

struct COLOR{

    float c[4];
    
    COLOR(float nr, float ng, float nb, float na = 1.0f){
        c[0] = nr; c[1] = ng; c[2] = nb; c[3] = na;
    }

    COLOR(const float* nc){
        if (nc)
            memcpy(c, nc, sizeof(float)*4);
    }

    COLOR(){
        c[0] = 1.0f; c[1] = 1.0f; c[2] = 1.0f; c[3] = 1.0f;
    }

    float r() {return c[0];}
    float g() {return c[1];}
    float b() {return c[2];}
    float a() {return c[3];}
};
#endif //_COLORS_H__
