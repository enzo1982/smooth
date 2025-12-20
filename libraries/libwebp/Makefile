########## smooth directory makefile ##########

# Change these variables to fit this location:
SMOOTH_PATH = ../..

include $(dir $(firstword $(MAKEFILE_LIST)))/$(SMOOTH_PATH)/Makefile-options

# Change these variables to fit your project:
GLOBALLIB   = libwebp.a
MYCCOPTS    = -I"$(SRCDIR)"

# Enter object files here:
OBJECTS	    = src/dec/alpha_dec.o src/dec/buffer_dec.o src/dec/frame_dec.o src/dec/idec_dec.o src/dec/io_dec.o src/dec/quant_dec.o src/dec/tree_dec.o src/dec/vp8_dec.o src/dec/vp8l_dec.o src/dec/webp_dec.o
OBJECTS	   += src/demux/anim_decode.o src/demux/demux.o
OBJECTS	   += src/dsp/alpha_processing.o src/dsp/cost.o src/dsp/cpu.o src/dsp/dec.o src/dsp/dec_clip_tables.o src/dsp/enc.o src/dsp/filters.o src/dsp/lossless.o src/dsp/lossless_enc.o src/dsp/rescaler.o src/dsp/ssim.o src/dsp/upsampling.o src/dsp/yuv.o
OBJECTS	   += src/dsp/alpha_processing_neon.o src/dsp/cost_neon.o src/dsp/dec_neon.o src/dsp/enc_neon.o src/dsp/filters_neon.o src/dsp/lossless_neon.o src/dsp/lossless_enc_neon.o src/dsp/rescaler_neon.o src/dsp/upsampling_neon.o src/dsp/yuv_neon.o
OBJECTS	   += src/dsp/alpha_processing_sse2.o src/dsp/cost_sse2.o src/dsp/dec_sse2.o src/dsp/enc_sse2.o src/dsp/filters_sse2.o src/dsp/lossless_sse2.o src/dsp/lossless_enc_sse2.o src/dsp/rescaler_sse2.o src/dsp/ssim_sse2.o src/dsp/upsampling_sse2.o src/dsp/yuv_sse2.o
OBJECTS	   += src/dsp/alpha_processing_sse41.o src/dsp/dec_sse41.o src/dsp/enc_sse41.o src/dsp/lossless_sse41.o src/dsp/lossless_enc_sse41.o src/dsp/upsampling_sse41.o src/dsp/yuv_sse41.o
OBJECTS	   += src/dsp/lossless_avx2.o src/dsp/lossless_enc_avx2.o
OBJECTS	   += src/enc/alpha_enc.o src/enc/analysis_enc.o src/enc/backward_references_cost_enc.o src/enc/backward_references_enc.o src/enc/config_enc.o src/enc/cost_enc.o src/enc/filter_enc.o src/enc/frame_enc.o src/enc/histogram_enc.o src/enc/iterator_enc.o src/enc/near_lossless_enc.o src/enc/picture_csp_enc.o src/enc/picture_enc.o src/enc/picture_psnr_enc.o src/enc/picture_rescale_enc.o src/enc/picture_tools_enc.o src/enc/predictor_enc.o src/enc/quant_enc.o src/enc/syntax_enc.o src/enc/token_enc.o src/enc/tree_enc.o src/enc/vp8l_enc.o src/enc/webp_enc.o
OBJECTS	   += src/utils/bit_reader_utils.o src/utils/bit_writer_utils.o src/utils/color_cache_utils.o src/utils/filters_utils.o src/utils/huffman_encode_utils.o src/utils/huffman_utils.o src/utils/palette.o src/utils/quant_levels_dec_utils.o src/utils/quant_levels_utils.o src/utils/random_utils.o src/utils/rescaler_utils.o src/utils/thread_utils.o src/utils/utils.o

OBJECTS    += sharpyuv/sharpyuv.o sharpyuv/sharpyuv_cpu.o sharpyuv/sharpyuv_csp.o sharpyuv/sharpyuv_dsp.o sharpyuv/sharpyuv_gamma.o sharpyuv/sharpyuv_neon.o sharpyuv/sharpyuv_sse2.o

## Do not change anything below this line. ##

include $(dir $(firstword $(MAKEFILE_LIST)))/$(SMOOTH_PATH)/Makefile-commands
