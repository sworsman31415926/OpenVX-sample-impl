#include <iostream>
#include <VX/vx.h>

void print_convolution(vx_convolution convolution, const char *message)
{
    std::cout << "===============================" << std::endl;
    std::cout << message << std::endl;

    vx_size rows;
    vxQueryConvolution(convolution, (vx_enum)VX_CONVOLUTION_ROWS, &rows, sizeof(rows));

    vx_size cols;
    vxQueryConvolution(convolution, (vx_enum)VX_CONVOLUTION_COLUMNS, &cols, sizeof(cols));

    vx_uint32 scale;
    vxQueryConvolution(convolution, (vx_enum)VX_CONVOLUTION_SCALE, &scale, sizeof(scale));

    vx_size size;
    vxQueryConvolution(convolution, (vx_enum)VX_CONVOLUTION_SIZE, &size, sizeof(size));

    std::cout << "rows      : " << rows << std::endl;
    std::cout << "cols      : " << cols << std::endl;
    std::cout << "scale     : " << scale << std::endl;
    std::cout << "size      : " << size << std::endl;
    std::cout << "item size : " << size / (rows * cols) << std::endl;

    void* buffer = malloc(size);
    // 在用户内存中，卷积系数以行主轴的方式进行存储
    vxCopyConvolutionCoefficients(convolution, buffer, VX_READ_ONLY, VX_MEMORY_TYPE_HOST);
    std::cout << "convolution coefficients" << std::endl;
    for (int row = 0; row < rows; ++row) {
        std::cout << "\t";
        for (int col = 0; col < cols; ++col) {
            std::cout << ((vx_int16*)buffer)[row * cols + col] << "\t";
        }
        std::cout << std::endl;
    }
    free(buffer);
}

int main(int argc, char *argv[])
{
    (void) argc;
    (void) argv;

    vx_status status;
    vx_context context = vxCreateContext();

    // 参数 columns 与 rows 必须为基数，且要在范围 [3, VX_CONTEXT_CONVOLUTION_MAX_DIMENSION)
    vx_size cols = 7U;
    vx_size rows = 5U;
    vx_convolution convolution = vxCreateConvolution(context, cols, rows);
    print_convolution(convolution, "create");

    // 更新元素
    vx_int16 *coefficients = (vx_int16*)malloc(rows * cols * sizeof (vx_int16));
    for (int i = 0; i < rows * cols; ++i) {
        coefficients[i] = i;
    }
    vxCopyConvolutionCoefficients(convolution, coefficients, VX_WRITE_ONLY, VX_MEMORY_TYPE_HOST);
    print_convolution(convolution, "update items");

    // 设置属性
    vx_uint32 scale = 2;
    vxSetConvolutionAttribute(convolution, (vx_enum)VX_CONVOLUTION_SCALE, &scale, sizeof (scale));
    print_convolution(convolution, "update scale");

    free(coefficients);
    vxReleaseConvolution(&convolution);

    vxReleaseContext(&context);
    return EXIT_SUCCESS;
}
