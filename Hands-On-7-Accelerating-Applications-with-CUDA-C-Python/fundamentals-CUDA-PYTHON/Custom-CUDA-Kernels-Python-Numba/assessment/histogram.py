# Add your solution here
@cuda.jit
def cuda_histogram(x, xmin, xmax, histogram_out):
    start = cuda.grid(1) 
    stride = cuda.gridsize(1)  
    nbins = histogram_out.shape[0]
    bin_width = (xmax - xmin) / nbins
    
    for i in range(start,x.shape[0],stride):
        bin_number= np.int32((x[i]-xmin)/bin_width)
        if bin_number >= 0 and bin_number < histogram_out.shape[0]:
            cuda.atomic.add(histogram_out, bin_number, 1)
            
d_x = cuda.to_device(x)
d_histogram_out = cuda.to_device(np.zeros(shape=10, dtype=np.int32))

blocks = 128
threads_per_block = 64

cuda_histogram[blocks, threads_per_block](d_x, xmin, xmax, d_histogram_out)

print(d_histogram_out.copy_to_host())
