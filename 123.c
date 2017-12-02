/******************************************************************************
 *cr
 *cr         (C) Copyright 2010-2013 The Board of Trustees of the
 *cr                        University of Illinois
 *cr                         All Rights Reserved
 *cr
 ******************************************************************************/

__constant__ float M_c[FILTER_SIZE][FILTER_SIZE];

__global__ void convolution(Matrix N, Matrix P) {

	/************************************************************************
     * Determine input and output indexes of each thread                    *
     * Load a tile of the input image to shared memory                      *
     * Apply the filter on the input image tile                             *
     * Write the compute values to the output image at the correct indexes  *
     * Use OUTPUT_TILE_SIZE and INPUT_TILE_SIZE defined in support.h        *
	 ************************************************************************/

    //INSERT KERNEL CODE HERE

    __shared__ float N_ds[INPUT_TILE_SIZE][INPUT_TILE_SIZE];
   int i,j;
   int tx = threadIdx.x;
   int ty = threadIdx.y;
   int row_o = blockIdx.y * OUTPUT_TILE_SIZE + ty;
   int col_o = blockIdx.x * OUTPUT_TILE_SIZE + tx;

   int row_i = row_o - (FILTER_SIZE-1)/2;
   int col_i = col_o - (FILTER_SIZE-1)/2;
   
   float output = 0.0f;

   if((row_i >=0) && (row_i < N.height) && (col_i >= 0) && (col_i < N.width))
   {
	N_ds[ty][tx] = N.elements[row_i * N.width + col_i];
   }
   else
   {
	N_ds[ty][tx] = 0.0f;
   }
    
   if(ty < OUTPUT_TILE_SIZE && tx < OUTPUT_TILE_SIZE)
   {
	for(i = 0; i < FILTER_SIZE; i++)
	{
	    for(j = 0; j < FILTER_SIZE; j++)
	    {
		output += M_c[i][j] * N_ds[i+ty][j+tx];
		__syncthreads();
	    }

   	}
    }

   if(row_o < P.height && col_o < P.width && ty < OUTPUT_TILE_SIZE && tx < OUTPUT_TILE_SIZE )
   {
	P.elements[row_o * P.width + col_o] = output;
   }

}