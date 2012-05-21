#include <paclib.h>

#include "minunit.h"

TEST(create_img_set) {
    imgset_t *set = imgset_create(1, 2);
    imgset_set_fname(set, 1, 1, "./test_img/input1.pgm");
    imgset_set_fname(set, 2, 1, "./test_img/ideal1.pgm");
    imgset_free(set);

    img_t *im1, *im2, *im3;
    lcollec("a", "b", "c", BB, BB, 1, "sdf", "sdf");

} TEST_END

#include "runner.h"
