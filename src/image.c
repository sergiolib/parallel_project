//
// Created by sliberman on 3/26/19.
//

#include <stdlib.h>
#include "image.h"

Image *create_image(int width, int height) {
    Image *result = (Image *)(malloc(sizeof(Image)));
    result->width = width;
    result->height = height;
    result->data = malloc(sizeof(result->data) * width * height);
    return result;
}

void delete_image(Image *img) {
    free(img->data);
    free(img);
}