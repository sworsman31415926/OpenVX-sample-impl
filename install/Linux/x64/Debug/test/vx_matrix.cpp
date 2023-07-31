#include <iostream>
#include <VX/vx.h>

const char* get_pattern_string(vx_pattern_e pattern)
{
    switch (pattern) {
        case VX_PATTERN_BOX:return "VX_PATTERN_BOX";
        case VX_PATTERN_CROSS:return "VX_PATTERN_CROSS";
        case VX_PATTERN_DISK:return "VX_PATTERN_DISK";
        case VX_PATTERN_OTHER:return "VX_PATTERN_OTHER";
    }
}

void print_matrix(vx_matrix matrix, const char *message)
{
    std::cout << "===============================" << std::endl;
    std::cout << message << std::endl;

    vx_enum type;
    vxQueryMatrix(matrix, (vx_enum)VX_MATRIX_TYPE, &type, sizeof(type));

    vx_size rows;
    vxQueryMatrix(matrix, (vx_enum)VX_MATRIX_ROWS, &rows, sizeof(rows));

    vx_size cols;
    vxQueryMatrix(matrix, (vx_enum)VX_MATRIX_COLUMNS, &cols, sizeof(cols));

    vx_size size;
    vxQueryMatrix(matrix, (vx_enum)VX_MATRIX_SIZE, &size, sizeof(size));

    vx_coordinates2d_t origin;
    vxQueryMatrix(matrix, (vx_enum)VX_MATRIX_ORIGIN, &origin, sizeof(origin));

    vx_enum pattern;
    vxQueryMatrix(matrix, (vx_enum)VX_MATRIX_PATTERN, &pattern, sizeof(pattern));

    std::cout << "type      : " << type << std::endl;
    std::cout << "rows      : " << rows << std::endl;
    std::cout << "cols      : " << cols << std::endl;
    std::cout << "size      : " << size << std::endl;
    std::cout << "origin x  : " << origin.x << std::endl;
    std::cout << "origin y  : " << origin.y << std::endl;
    std::cout << "pattern   : " << get_pattern_string((vx_pattern_e)pattern) << std::endl;

    std::cout << "item      : " << std::endl;
    void* ptr = malloc(size);
    vxCopyMatrix(matrix, ptr, VX_READ_ONLY, VX_MEMORY_TYPE_HOST);
    for (int row = 0; row < rows; ++row) {
        std::cout << "\t";
        for (int col = 0; col <cols; ++col) {
            std::cout << (int)((vx_uint8*)ptr)[row * cols + col] << "\t";
        }
        std::cout << std::endl;
    }
    free(ptr);
}

int main(int argc, char *argv[])
{
    (void)argc;
    (void)argv;

    vx_context context = vxCreateContext();

    vx_size rows = 4;
    vx_size cols = 5;
    vx_matrix matrix = vxCreateMatrix(context, VX_TYPE_UINT8, cols, rows);
    print_matrix(matrix, "create");

    vx_uint8 *array = new vx_uint8 [rows * cols];
    for (int i = 0; i < rows * cols; ++i)
        array[i] = i;
    vxCopyMatrix(matrix, array, VX_WRITE_ONLY, VX_MEMORY_TYPE_HOST);
    print_matrix(matrix, "update");

    delete[] array;
    vxReleaseMatrix(&matrix);

    vx_matrix matrix_pattern = vxCreateMatrixFromPattern(context, VX_PATTERN_BOX, cols, rows);
    print_matrix(matrix_pattern, "matrix_pattern");
    vxReleaseMatrix(&matrix_pattern);

    vx_matrix matrix_pattern_origin = vxCreateMatrixFromPatternAndOrigin(context, VX_PATTERN_BOX, cols, rows, 3, 3);
    print_matrix(matrix_pattern_origin, "matrix_pattern_origin");
    vxReleaseMatrix(&matrix_pattern_origin);

    vxReleaseContext(&context);
    return EXIT_SUCCESS;
}
/*
./vx_matrix 
===============================
create
type      : 3
rows      : 4
cols      : 5
size      : 20
origin x  : 2
origin y  : 2
pattern   : VX_PATTERN_OTHER
item      : 
	0	0	0	0	0	
	0	0	0	0	0	
	0	0	0	0	0	
	0	0	0	0	0	
===============================
update
type      : 3
rows      : 4
cols      : 5
size      : 20
origin x  : 2
origin y  : 2
pattern   : VX_PATTERN_OTHER
item      : 
	0	1	2	3	4	
	5	6	7	8	9	
	10	11	12	13	14	
	15	16	17	18	19	
===============================
matrix_pattern
type      : 3
rows      : 4
cols      : 5
size      : 20
origin x  : 2
origin y  : 2
pattern   : VX_PATTERN_BOX
item      : 
	255	255	255	255	255	
	255	255	255	255	255	
	255	255	255	255	255	
	255	255	255	255	255	
===============================
matrix_pattern_origin
type      : 3
rows      : 4
cols      : 5
size      : 20
origin x  : 3
origin y  : 3
pattern   : VX_PATTERN_BOX
item      : 
	255	255	255	255	255	
	255	255	255	255	255	
	255	255	255	255	255	
	255	255	255	255	255	

*/
