//
// Created by dilin on 1/8/18.
//

#ifndef CLIENT_CORE_H
#define CLIENT_CORE_H

#include <string.h>
#include <math.h>
#include <cmath>

#define IMG_H 240
#define IMG_W 320
#define IMG_SIZE IMG_W*IMG_H
#define MODELS 2
#define PARTS 128

using namespace std;

//typedef ap_fixed<24,16> data_t;
typedef float data_t;
typedef unsigned char uint8_t;

int backsub(uint8_t data_array[IMG_SIZE], uint8_t out_frame[IMG_SIZE],
            bool init, data_t parameters[IMG_SIZE*MODELS*3]);

uint8_t EM_ALGO(uint8_t pixel,int pos, data_t parameters[(IMG_SIZE/PARTS)*MODELS*3], int x);


#endif //CLIENT_CORE_H
