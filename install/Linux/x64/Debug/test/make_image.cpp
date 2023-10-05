#include <VX/vx.h>
#include <stdio.h>

vx_image makeInputImage(vx_context context)
{
    // 创建一个100*100 vx_image对象
    vx_image image = vxCreateImage(context, 100U, 100U, VX_DF_IMAGE_U8);
    vx_rectangle_t rect = {
        .start_x = 20, .start_y = 40, .end_x = 80, .end_y = 60
    };

    if (VX_SUCCESS == vxGetStatus((vx_reference)image))
    {
        // 获取image的ROI
        vx_image roi = vxCreateImageFromROI(image, &rect);
        vx_pixel_value_t pixel_white, pixel_black;
        pixel_white.U8 = 255;
        pixel_black.U8 = 0;

        if (VX_SUCCESS == vxGetStatus((vx_reference)roi))
        {
            // 设置ROI的像素值
            if (VX_SUCCESS == vxSetImagePixelValues(image, &pixel_black) &&
                VX_SUCCESS == vxSetImagePixelValues(roi, &pixel_white))
            {
                vxReleaseImage(&roi);
                return image;
            }
        }
        // 出错情况下释放roi和image
        vxReleaseImage(&roi);
    }
    
    // 出错情况下释放image
    vxReleaseImage(&image);
    return NULL;
}

int main()
{
    vx_context context = vxCreateContext();
    vx_image image = makeInputImage(context);

    if (image != NULL)
    {
        // 此处可以对创建的图像进行进一步操作

        // 释放图像资源
        vxReleaseImage(&image);
        
    }
    else
    {
        printf("Failed to create the image.\n");
    }
     printf(" create the image ok.\n");
    // 释放上下文
    vxReleaseContext(&context);

    return 0;
}
/*
g++ -I include test/make_image.cpp bin/libopenvx.so bin/libvxu.so -o test/make_image
./test/make_image
create the image ok.
*/
