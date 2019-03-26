//
// Created by sliberman on 3/26/19.
//

#ifndef PARALLELGENETIC_IMAGE_H
#define PARALLELGENETIC_IMAGE_H

#endif //PARALLELGENETIC_IMAGE_H

typedef struct {
    unsigned short* data;
    int width;
    int height;
} Image;

Image *create_image(int width, int height);
void delete_image(Image *img);