#include <trios.h>

#include "trios_io.h"

#include "minunit.h"

UTEST(create_image) 
    img_t *img  = img_create(5, 4, 1, sz8BIT);
    img_set_pixel(img, 2, 1, 0, 44);
    mu_assert("Image width:", 4 == img_get_height(img));
    mu_assert("Image height:", 5 == img_get_width(img));
    mu_assert("Image nbands:", 1 == img_get_nbands(img));
    mu_assert("Image get pixel:", 44 == img_get_pixel(img, 2, 1, 0));
    img_free(img);
TEST_END

UTEST(image_16_bit) 
    img_t *img  = img_create(5, 4, 1, sz16BIT);
    img_set_pixel(img, 0, 1, 0, 128);
    img_set_pixel(img, 2, 1, 0, 454);
    img_set_pixel(img, 2, 0, 0, 65535);
    mu_assert("Image get pixel:", 128 == img_get_pixel(img, 0, 1, 0));
    mu_assert("Image get pixel:", 454 == img_get_pixel(img, 2, 1, 0));
    mu_assert("Image get pixel:", 65535 == img_get_pixel(img, 2, 0, 0));
    img_free(img);
TEST_END

UTEST(image_32_bit) 
    img_t *img  = img_create(5, 4, 1, sz32BIT);
    img_set_pixel(img, 0, 1, 0, 128);
    img_set_pixel(img, 2, 1, 0, 454);
    img_set_pixel(img, 2, 0, 0, 65535);
    img_set_pixel(img, 1, 3, 0, 65539);
    mu_assert("Image get pixel:", 65539 == img_get_pixel(img, 1, 3, 0));
    mu_assert("Image get pixel:", 128 == img_get_pixel(img, 0, 1, 0));
    mu_assert("Image get pixel:", 454 == img_get_pixel(img, 2, 1, 0));
    mu_assert("Image get pixel:", 65535 == img_get_pixel(img, 2, 0, 0));
    img_free(img);
TEST_END

UTEST(write_image) 
    img_t *img  = img_create(5, 4, 1, sz8BIT);
    img_set_pixel(img, 0, 0, 0, 197);
    img_set_pixel(img, 2, 1, 0, 240);
    img_set_pixel(img, 3, 4, 0, 255);
    img_writePGM("test.pgm", img);
    img_free(img);
TEST_END

UTEST(read_image) 
    img_t *img  = img_readPGM("test_img/test_read.pgm");
    mu_assert("read: width fail", img_get_width(img) == 3);
    mu_assert("read: height fail", img_get_height(img) == 2);
    mu_assert("read: pixel fail", img_get_pixel(img, 0, 0, 0) == 20);
    mu_assert("read: pixel fail", img_get_pixel(img, 0, 1, 0) == 128);
    mu_assert("read: pixel fail", img_get_pixel(img, 0, 2, 0) == 255);
    mu_assert("read: pixel fail", img_get_pixel(img, 1, 0, 0) == 255);
    mu_assert("read: pixel fail", img_get_pixel(img, 1, 1, 0) == 128);
    mu_assert("read: pixel fail", img_get_pixel(img, 1, 2, 0) == 20);
	    img_free(img);
TEST_END

UTEST(read_real_image) {
    img_t *img  = img_readPGM("test_img/input1.pgm");
    img_free(img);
    img  = img_readPGM("test_img/input2.pgm");
    img_free(img);
    img  = img_readPGM("test_img/input3.pgm");
    img_free(img);
    img  = img_readPGM("test_img/ideal1.pgm");
    img_free(img);
    img  = img_readPGM("test_img/ideal2.pgm");
    img_free(img);
    img  = img_readPGM("test_img/ideal3.pgm");
    img_free(img);
} TEST_END

#include "runner.h"

