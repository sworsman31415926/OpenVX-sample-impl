#include "stdio.h"
#include "VX/vx.h"
#include "VX/vxu.h"

#define VX_SCALE_UNITY 1024
#define PATCH_DIM_X 640
#define PATCH_DIM_Y 480

int main()
{
    vx_status status = VX_SUCCESS;
    void *base_ptr = NULL;
    vx_uint32 width = 640, height = 480, plane = 0;
    vx_context contextObj = vxCreateContext();

    printf("before create image\n");
    vx_image image = vxCreateImage(contextObj, width, height,VX_DF_IMAGE_U8);
    printf("after create image\n");

    vx_rectangle_t rect;
    vx_imagepatch_addressing_t addr;
    vx_map_id map_id;
    rect.start_x = rect.start_y = 0;
    rect.end_x = PATCH_DIM_X;
    rect.end_y = PATCH_DIM_Y;

    printf("before map image patch\n");
    status = vxMapImagePatch(image, &rect, plane, &map_id, &addr, &base_ptr,
                            VX_READ_AND_WRITE, VX_MEMORY_TYPE_HOST, 0);
    printf("after map image patch\n");
    if (status == VX_SUCCESS)
    {
        vx_uint32 x,y,i,j;
        vx_uint8 pixel = 1;

        printf("before image modified by 1d linear addressing access\n");
        /* a couple addressing options */
        /* use linear addressing function/macro */
        for (i = 0; i < addr.dim_x*addr.dim_y; i++) {
            vx_uint8 *ptr2 = (vx_uint8*)vxFormatImagePatchAddress1d(base_ptr,i, &addr);
            *ptr2 = pixel;
        }
        printf("after image modified by linear addressing access\n");
        for (i = 0; i < addr.dim_x*addr.dim_y; i++) {
            vx_uint8 *ptr2 = (vx_uint8*)vxFormatImagePatchAddress1d(base_ptr,i, &addr);
            printf("%02x ", *ptr2);
            if((i+1)%16 == 0)
                printf("\n");
        }
        printf("\n");

        printf("before image modified by 2d specific addressing access, step(%d,%d)\n", addr.step_x, addr.step_y);
        pixel = 2;
        /* 2d addressing option */
        for (y = 0; y < addr.dim_y; y+=addr.step_y) {
            for (x = 0; x < addr.dim_x; x+=addr.step_x) {
                vx_uint8 *ptr2 = (vx_uint8*)vxFormatImagePatchAddress2d(base_ptr, x, y, &addr);
                *ptr2 = pixel;
            }
        }
        printf("after image modified by 2d specific addressing access\n");
        for (i = 0; i < addr.dim_x*addr.dim_y; i++) {
            vx_uint8 *ptr2 = (vx_uint8*)vxFormatImagePatchAddress1d(base_ptr,i, &addr);
            printf("%02x ", *ptr2);
            if((i+1)%16 == 0)
                printf("\n");
        }
        printf("\n");

        printf("before image modified by direct addressing access, dim(%d,%d) stride(%d,%d),scale(%d,%d)\n",
                 addr.dim_x,  addr.dim_y, addr.stride_x, addr.stride_y, addr.scale_x, addr.scale_y);
        pixel = 3;
        /* direct addressing by client
        * for subsampled planes, scale will change
        */
        for (y = 0; y < addr.dim_y; y+=addr.step_y) {
            for (x = 0; x < addr.dim_x; x+=addr.step_x) {
                vx_uint8 *tmp = (vx_uint8 *)base_ptr;
                i = ((addr.stride_y*y*addr.scale_y) /VX_SCALE_UNITY) +((addr.stride_x*x*addr.scale_x) /VX_SCALE_UNITY);
                tmp[i] = pixel;
            }
        }
        printf("after image modified by direct addressing access, dim(%d,%d) stride(%d,%d),scale(%d,%d)\n",
                 addr.dim_x,  addr.dim_y, addr.stride_x, addr.stride_y, addr.scale_x, addr.scale_y);
        for (i = 0; i < addr.dim_x*addr.dim_y; i++) {
            vx_uint8 *ptr2 = (vx_uint8*)vxFormatImagePatchAddress1d(base_ptr,i, &addr);
            printf("%02x ", *ptr2);
            if((i+1)%16 == 0)
                printf("\n");
        }
        printf("\n");

        /* more efficient direct addressing by client.
        * for subsampled planes, scale will change.
        */
        printf("before image modified by more efficient direct addressing access, dim(%d,%d) stride(%d,%d),scale(%d,%d)\n",
                 addr.dim_x,  addr.dim_y, addr.stride_x, addr.stride_y, addr.scale_x, addr.scale_y);
        pixel = 4;
        for (y = 0; y < addr.dim_y; y+=addr.step_y) {
            j = (addr.stride_y*y*addr.scale_y)/VX_SCALE_UNITY;
            for (x = 0; x < addr.dim_x; x+=addr.step_x) {
                vx_uint8 *tmp = (vx_uint8 *)base_ptr;
                i = j + (addr.stride_x*x*addr.scale_x)/VX_SCALE_UNITY;
                tmp[i] = pixel;
            }
        }
        printf("after image modified by more efficient direct addressing access, dim(%d,%d) stride(%d,%d),scale(%d,%d)\n",
                 addr.dim_x,  addr.dim_y, addr.stride_x, addr.stride_y, addr.scale_x, addr.scale_y);
        for (i = 0; i < addr.dim_x*addr.dim_y; i++) {
            vx_uint8 *ptr2 = (vx_uint8*)vxFormatImagePatchAddress1d(base_ptr,i, &addr);
            printf("%02x ", *ptr2);
            if((i+1)%16 == 0)
                printf("\n");
        }
        printf("\n");

        /* this commits the data back to the image.
        */
        status = vxUnmapImagePatch(image, map_id);
    }

    {
        vx_uint32 x,y,i,j;
        vx_uint8 pixel = 1;

        vx_image imageB = vxCreateImage(contextObj, width, height,VX_DF_IMAGE_U8);
        status = vxMapImagePatch(imageB, &rect, plane, &map_id, &addr, &base_ptr,
                            VX_READ_AND_WRITE, VX_MEMORY_TYPE_HOST, 0);
        pixel = 1;
        for (y = 0; y < addr.dim_y; y+=addr.step_y) 
        {
            j = (addr.stride_y*y*addr.scale_y)/VX_SCALE_UNITY;
            for (x = 0; x < addr.dim_x; x+=addr.step_x) 
            {
                vx_uint8 *tmp = (vx_uint8 *)base_ptr;
                i = j + (addr.stride_x*x*addr.scale_x)/VX_SCALE_UNITY;
                tmp[i] = pixel;
            }
        }
        status = vxUnmapImagePatch(imageB, map_id);

        vx_image imageC = vxCreateImage(contextObj, width, height,VX_DF_IMAGE_U8);
        vxuAbsDiff(contextObj, image, imageB, imageC);

        //print imageC
        status = vxMapImagePatch(imageC, &rect, plane, &map_id, &addr, (void**)&base_ptr,
                            VX_READ_AND_WRITE, VX_MEMORY_TYPE_HOST, 0);
        for (i = 0; i < addr.dim_x*addr.dim_y; i++) 
        {
            vx_uint8 *ptr2 = (vx_uint8*)vxFormatImagePatchAddress1d(base_ptr,i, &addr);
            printf("%02x ", *ptr2);
            if((i+1)%16 == 0)
                printf("\n");
        }
        printf("\n");
        status = vxUnmapImagePatch(imageC, map_id);
    
        vxReleaseImage(&imageC);
        vxReleaseImage(&imageB);
    }

    vxReleaseImage(&image);

    return 0;
}
/*
g++ -I . test_second.c ../../../x64/Debug/bin/libopenvx.so ../../../x64/Debug/bin/libvxu.so -o test_second

*/
/*
...
03 03 03 03 03 03 03 03 03 03 03 03 03 03 03 03 
03 03 03 03 03 03 03 03 03 03 03 03 03 03 03 03 
03 03 03 03 03 03 03 03 03 03 03 03 03 03 03 03 
03 03 03 03 03 03 03 03 03 03 03 03 03 03 03 03 
03 03 03 03 03 03 03 03 03 03 03 03 03 03 03 03 
03 03 03 03 03 03 03 03 03 03 03 03 03 03 03 03 
03 03 03 03 03 03 03 03 03 03 03 03 03 03 03 03 
03 03 03 03 03 03 03 03 03 03 03 03 03 03 03 03 
03 03 03 03 03 03 03 03 03 03 03 03 03 03 03 03 
03 03 03 03 03 03 03 03 03 03 03 03 03 03 03 03 
03 03 03 03 03 03 03 03 03 03 03 03 03 03 03 03 
03 03 03 03 03 03 03 03 03 03 03 03 03 03 03 03 
03 03 03 03 03 03 03 03 03 03 03 03 03 03 03 03 
03 03 03 03 03 03 03 03 03 03 03 03 03 03 03 03 
03 03 03 03 03 03 03 03 03 03 03 03 03 03 03 03 
03 03 03 03 03 03 03 03 03 03 03 03 03 03 03 03 
03 03 03 03 03 03 03 03 03 03 03 03 03 03 03 03
... 
*/