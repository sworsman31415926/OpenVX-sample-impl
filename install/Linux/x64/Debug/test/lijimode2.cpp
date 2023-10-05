#include <stdio.h>
#include <stdlib.h>
#include "VX/vx.h"
#include "VX/vxu.h"

void errorCheck(vx_context *context_p, vx_status status, const char *message)
{
    if (status)
    {
      puts("ERROR! ");
      puts(message);
      vxReleaseContext(context_p);
      exit(1);
    }
}

vx_image makeInputImage(vx_context context)
{
  vx_image image = vxCreateImage(context, 100U, 100U, VX_DF_IMAGE_U8);
  vx_rectangle_t rect = {
    .start_x = 20, .start_y = 40, .end_x=80, .end_y = 60
  };

  if (VX_SUCCESS == vxGetStatus((vx_reference)image))
  {
    vx_image roi = vxCreateImageFromROI(image, &rect);
    vx_pixel_value_t pixel_white, pixel_black;
    pixel_white.U8 = 255;
    pixel_black.U8 = 0;
    if (VX_SUCCESS == vxGetStatus((vx_reference)roi) &&
        VX_SUCCESS == vxSetImagePixelValues(image, &pixel_black) &&
        VX_SUCCESS == vxSetImagePixelValues(roi, &pixel_white))
      vxReleaseImage(&roi);
    else
      vxReleaseImage(&image);
  }

  return image;
}

int main(void)
{
  vx_context context = vxCreateContext();

  errorCheck(&context, vxGetStatus((vx_reference)context), "Could not create a vx_context\n");

  vx_image image1 = makeInputImage(context);

  errorCheck(&context, vxGetStatus((vx_reference)image1), "Could not create first image");

  vx_image image2 = vxCreateImage(context, 100U, 100U, VX_DF_IMAGE_U8);
  vx_image image3 = vxCreateImage(context, 100U, 100U, VX_DF_IMAGE_U8);
  vx_image grad_x = vxCreateImage(context, 100U, 100U, VX_DF_IMAGE_S16);
  vx_image grad_y = vxCreateImage(context, 100U, 100U, VX_DF_IMAGE_S16);
  vx_image magnitude = vxCreateImage(context, 100U, 100U, VX_DF_IMAGE_S16);
  vx_image converted = vxCreateImage(context, 100U, 100U, VX_DF_IMAGE_U8);
  vx_image dilated = vxCreateImage(context, 100U, 100U, VX_DF_IMAGE_U8);
  vx_image image4 = vxCreateImage(context, 100U, 100U, VX_DF_IMAGE_U8);
  vx_matrix warp_matrix = vxCreateMatrix(context, VX_TYPE_FLOAT32, 2U, 3U);
  vx_float32 matrix_values[3][2] = {       /* Rotate through 90 degrees */
      {0.0, 1.0},         /* x coefficients */
      {1.0, 0.0},         /* y coefficients */
      {0.0, 0.0}          /* offsets */
  };
  vx_float32 strength_thresh_value = 128.0;
  vx_scalar strength_thresh = vxCreateScalar(context, VX_TYPE_FLOAT32, &strength_thresh_value);
  vx_array corners = vxCreateArray(context, VX_TYPE_KEYPOINT, 100);
  vx_size num_corners_value = 0;
  vx_scalar num_corners = vxCreateScalar(context, VX_TYPE_SIZE, &num_corners_value);
  vx_keypoint_t *kp = (vx_keypoint_t*)calloc( 100, sizeof(vx_keypoint_t));

  errorCheck(&context,
             kp == NULL ||
             vxGetStatus((vx_reference)strength_thresh) ||
             vxGetStatus((vx_reference)corners) ||
             vxGetStatus((vx_reference)num_corners) ||
             vxGetStatus((vx_reference)image2) ||
             vxGetStatus((vx_reference)image3) ||
             vxGetStatus((vx_reference)grad_x) ||
             vxGetStatus((vx_reference)grad_y) ||
             vxGetStatus((vx_reference)magnitude) ||
             vxGetStatus((vx_reference)converted) ||
             vxGetStatus((vx_reference)dilated) ||
             vxGetStatus((vx_reference)image4) ||
             vxGetStatus((vx_reference)warp_matrix),
             "Could not create objects");

  errorCheck(&context, vxCopyMatrix(warp_matrix, matrix_values, VX_WRITE_ONLY, VX_MEMORY_TYPE_HOST), "Could not initialise the matrix");

  /* Now image2 set to image 1 rotated */
  vx_status status = vxuWarpAffine(context, image1, warp_matrix, VX_INTERPOLATION_NEAREST_NEIGHBOR, image2);
  /* image3 set to logical OR of images 1 and 2 and then processed as described above */
  errorCheck(&context, vxuOr(context, image1, image2, image3) ||
                                   vxuSobel3x3(context, image3, grad_x, grad_y) ||
                                   vxuMagnitude(context, grad_x, grad_y, magnitude) ||
                                   vxuConvertDepth(context, magnitude, converted, VX_CONVERT_POLICY_SATURATE, 1) ||
                                   vxuDilate3x3(context, converted, dilated) ||
                                   /* And now count the corners */
                                   vxuFastCorners(context, dilated, strength_thresh, vx_true_e, corners, num_corners),
                                   "Image functions failed");

  errorCheck(&context, vxCopyScalar(num_corners, &num_corners_value, VX_READ_ONLY, VX_MEMORY_TYPE_HOST), "vxCopyScalar failed");
  printf("Found %zu corners with non-max suppression\n", num_corners_value);

  errorCheck(&context, vxCopyArrayRange( corners, 0, num_corners_value, sizeof(vx_keypoint_t), kp,
                                        VX_READ_ONLY, VX_MEMORY_TYPE_HOST), "vxCopyArrayRange failed");
  for (int i=0; i<num_corners_value; ++i)
  {
    printf("Entry %3d: x = %d, y = %d\n", i, kp[i].x, kp[i].y);
  }

  free(kp);
  vxReleaseContext(&context);
  return 0;
}

/*
g++ -I include test/lijimode2.cpp bin/libopenvx.so bin/libvxu.so -o test/lijimode2
./test/lijimode2
Found 24 corners with non-max suppression
Entry   0: x = 36, y = 18
Entry   1: x = 59, y = 18
Entry   2: x = 40, y = 22
Entry   3: x = 55, y = 22
Entry   4: x = 35, y = 35
Entry   5: x = 62, y = 35
Entry   6: x = 16, y = 38
Entry   7: x = 79, y = 38
Entry   8: x = 38, y = 39
Entry   9: x = 58, y = 39
Entry  10: x = 20, y = 42
Entry  11: x = 75, y = 42
Entry  12: x = 22, y = 55
Entry  13: x = 75, y = 55
Entry  14: x = 39, y = 58
Entry  15: x = 56, y = 58
Entry  16: x = 18, y = 59
Entry  17: x = 78, y = 59
Entry  18: x = 35, y = 62
Entry  19: x = 60, y = 62
Entry  20: x = 42, y = 75
Entry  21: x = 55, y = 75
Entry  22: x = 38, y = 79
Entry  23: x = 58, y = 79

*/