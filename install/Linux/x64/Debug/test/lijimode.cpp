#include <stdio.h>
#include <stdlib.h>
#include <VX/vx.h>
#include <VX/vxu.h>
//错误检测函数
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
//图像创建函数
vx_image makeInputImage(vx_context context)
{
  //创建一个100*100的image对象
  vx_image image = vxCreateImage(context, 100U, 100U, VX_DF_IMAGE_U8);
  //坐标
  vx_rectangle_t rect = {
    .start_x = 20, .start_y = 40, .end_x=80, .end_y = 60
  };
  if (VX_SUCCESS == vxGetStatus((vx_reference)image))
  {
  	//创建一个ROI区域
    vx_image roi = vxCreateImageFromROI(image, &rect);
    vx_pixel_value_t pixel_white, pixel_black;
    pixel_white.U8 = 255;
    pixel_black.U8 = 0;
    //像素填充
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
  //创建上下文
  vx_context context = vxCreateContext();
  errorCheck(&context, vxGetStatus((vx_reference)context), "Could not create a vx_context\n");
 //创建测试图像
  vx_image image1 = makeInputImage(context);
  errorCheck(&context, vxGetStatus((vx_reference)image1), "Could not create image");
 //快速角点检测阈值
  vx_float32 strength_thresh_value = 128.0;
  vx_scalar strength_thresh = vxCreateScalar(context, VX_TYPE_FLOAT32, &strength_thresh_value);
  vx_array corners = vxCreateArray(context, VX_TYPE_KEYPOINT, 100);
  vx_array corners1 = vxCreateArray(context, VX_TYPE_KEYPOINT, 100);
  vx_size num_corners_value = 0;
  vx_scalar num_corners = vxCreateScalar(context, VX_TYPE_SIZE, &num_corners_value);
  vx_scalar num_corners1 = vxCreateScalar(context, VX_TYPE_SIZE, &num_corners_value);
  vx_keypoint_t *kp = (vx_keypoint_t*)calloc( 100, sizeof(vx_keypoint_t));

  errorCheck(&context,
             kp == NULL ||
             vxGetStatus((vx_reference)strength_thresh) ||
             vxGetStatus((vx_reference)corners) ||
             vxGetStatus((vx_reference)num_corners) ||
             vxGetStatus((vx_reference)corners1) ||
             vxGetStatus((vx_reference)num_corners1),
             "Could not create parameters for FastCorners");
//采用非极大值移抑制的Fast角点检测算子
  errorCheck(&context, vxuFastCorners(context, image1, strength_thresh, vx_true_e, corners, num_corners), "Fast Corners function failed");
//不采用非极大值移抑制的Fast角点检测算子
  errorCheck(&context, vxuFastCorners(context, image1, strength_thresh, vx_false_e, corners1, num_corners1), "Fast Corners function failed");
//角点个数
  errorCheck(&context, vxCopyScalar(num_corners, &num_corners_value, VX_READ_ONLY, VX_MEMORY_TYPE_HOST), "vxCopyScalar failed");
  printf("Found %zu corners with non-max suppression\n", num_corners_value);
//坐标保存数组
  errorCheck(&context, vxCopyArrayRange( corners, 0, num_corners_value, sizeof(vx_keypoint_t), kp,VX_READ_ONLY, VX_MEMORY_TYPE_HOST), "vxCopyArrayRange failed");
//显示坐标                                      
  for (int i=0; i<num_corners_value; ++i)
  {
    printf("Entry %3d: x = %d, y = %d\n", i, kp[i].x, kp[i].y);
  }
//角点个数
  errorCheck(&context, vxCopyScalar(num_corners1, &num_corners_value, VX_READ_ONLY, VX_MEMORY_TYPE_HOST), "vxCopyScalar failed");
  printf("Found %zu corners without non-max suppression\n", num_corners_value);
//坐标保存数组
  errorCheck(&context, vxCopyArrayRange( corners1, 0, num_corners_value, sizeof(vx_keypoint_t), kp,VX_READ_ONLY, VX_MEMORY_TYPE_HOST), "vxCopyArrayRange failed");
  //显示坐标
  for (int i=0; i<num_corners_value; ++i)
  {
    printf("Entry %3d: x = %d, y = %d\n", i, kp[i].x, kp[i].y);
  }

  free(kp);
  vxReleaseContext(&context);
  return 0;
}

/*

g++ -I include test/lijimode.cpp bin/libopenvx.so bin/libvxu.so -o test/lijimode
./test/lijimode
Found 4 corners with non-max suppression
Entry   0: x = 20, y = 42
Entry   1: x = 79, y = 42
Entry   2: x = 22, y = 59
Entry   3: x = 79, y = 59
Found 24 corners without non-max suppression
Entry   0: x = 20, y = 40
Entry   1: x = 21, y = 40
Entry   2: x = 22, y = 40
Entry   3: x = 77, y = 40
Entry   4: x = 78, y = 40
Entry   5: x = 79, y = 40
Entry   6: x = 20, y = 41
Entry   7: x = 21, y = 41
Entry   8: x = 78, y = 41
Entry   9: x = 79, y = 41
Entry  10: x = 20, y = 42
Entry  11: x = 79, y = 42
Entry  12: x = 20, y = 57
Entry  13: x = 79, y = 57
Entry  14: x = 20, y = 58
Entry  15: x = 21, y = 58
Entry  16: x = 78, y = 58
Entry  17: x = 79, y = 58
Entry  18: x = 20, y = 59
Entry  19: x = 21, y = 59
Entry  20: x = 22, y = 59
Entry  21: x = 77, y = 59
Entry  22: x = 78, y = 59
Entry  23: x = 79, y = 59

*/