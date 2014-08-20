#ifndef JCAPI_H
#define JCAPI_H

#ifdef __cplusplus
extern "C" {
#endif 

jpeg_compress_info *
     jpeg_create_compress (void);   
void jpeg_destory_compress(jpeg_compress_info *cinfo);

void jpeg_set_default     (jpeg_compress_info *cinfo);
void jpeg_calc_value      (jpeg_compress_info *cinfo);

void jpeg_start_compress  (jpeg_compress_info *cinfo);
void jpeg_finish_compress (jpeg_compress_info *cinfo);

void jpeg_write_scanline  (jpeg_compress_info *cinfo, JSAMPLE *samp_row);

#ifdef __cplusplus
}
#endif 

#endif /*JCAPI_H*/


