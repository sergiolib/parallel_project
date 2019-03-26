//
// Created by sliberman on 3/26/19.
//

#include <stdio.h>
#include "image.h"

int main(int argc, char **argv) {
    Image* img = create_image(1024, 768);
    delete_image(img);
    printf("Hello world!\n");
    return 0;
}