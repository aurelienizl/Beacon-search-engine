#include "store_server.h"

void compress_data(struct data *data)
{
    unsigned long output_size = compressBound(data->data_size);
    unsigned char *output = (unsigned char *)malloc(output_size);

    if (compress2(output, &output_size, data->data, data->data_size, Z_BEST_COMPRESSION) != Z_OK)
    {
        fprintf(stderr, "Compression failed\n");
        exit(1);
    }

    free(data->data);
    data->data = (unsigned char *)output;
    data->data_size = output_size;
}

void decompress_data(struct data *data)
{
   unsigned char *decompressed = (unsigned char *)malloc(data->data_capacity);

    if (uncompress(decompressed, &(data->data_capacity), data->data, data->data_size) != Z_OK)
    {
        fprintf(stderr, "Decompression failed\n");
        free(decompressed);
        exit(1);
    }

    free(data->data);
    data->data = decompressed;
    data->data_size = data->data_capacity;

}