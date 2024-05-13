// Auto-generated file. Do not edit!
//   Template: src/f16-f32acc-rdsum/neon.c.in
//   Generator: tools/xngen
//
// Copyright 2024 Google LLC
//
// This source code is licensed under the BSD-style license found in the
// LICENSE file in the root directory of this source tree.

#include <assert.h>

#include <arm_neon.h>

#include <xnnpack/common.h>
#include <xnnpack/reduce.h>
#include <xnnpack/math.h>


void xnn_f16_f32acc_rdsum_ukernel_7p7x__neonfp16arith_c32(
    size_t rows,
    size_t channels,
    const void* input,
    size_t input_stride,
    const void* zero,
    void* output,
    const union xnn_f16_f32acc_scale_params params[restrict XNN_MIN_ELEMENTS(1)])
{
  assert(rows != 0);
  assert(channels != 0);
  assert(input != NULL);
  assert(output != NULL);

  const float32x4_t vscale = vdupq_n_f32(params->scalar.scale);

  size_t input_increment = 7 * input_stride;
  for (; channels >= 32; channels -= 32) {
    const uint16_t* i0 = input;
    const uint16_t* i1 = (const uint16_t*) ((uintptr_t) input + 1 * input_stride);
    const uint16_t* i2 = (const uint16_t*) ((uintptr_t) input + 2 * input_stride);
    const uint16_t* i3 = (const uint16_t*) ((uintptr_t) input + 3 * input_stride);
    const uint16_t* i4 = (const uint16_t*) ((uintptr_t) input + 4 * input_stride);
    const uint16_t* i5 = (const uint16_t*) ((uintptr_t) input + 5 * input_stride);
    const uint16_t* i6 = (const uint16_t*) ((uintptr_t) input + 6 * input_stride);

    float32x4_t vacc0 = vdupq_n_f32(0.f);
    float32x4_t vacc1 = vdupq_n_f32(0.f);
    float32x4_t vacc2 = vdupq_n_f32(0.f);
    float32x4_t vacc3 = vdupq_n_f32(0.f);
    float32x4_t vacc4 = vdupq_n_f32(0.f);
    float32x4_t vacc5 = vdupq_n_f32(0.f);
    float32x4_t vacc6 = vdupq_n_f32(0.f);
    float32x4_t vacc7 = vdupq_n_f32(0.f);

    for (int r = rows; r > 0; r -= 7) {
      if XNN_UNPREDICTABLE(r < 2) {
        i1 = zero;
      }
      if XNN_UNPREDICTABLE(r <= 2) {
        i2 = zero;
      }
      if XNN_UNPREDICTABLE(r < 4) {
        i3 = zero;
      }
      if XNN_UNPREDICTABLE(r <= 4) {
        i4 = zero;
      }
      if XNN_UNPREDICTABLE(r < 6) {
        i5 = zero;
      }
      if XNN_UNPREDICTABLE(r <= 6) {
        i6 = zero;
      }
      float32x4_t vin0;
      float32x4_t vin1;
      float32x4_t vin2;
      float32x4_t vin3;
      float32x4_t vin4;
      float32x4_t vin5;
      float32x4_t vin6;
      float32x4_t vin7;
      vin0 = vcvt_f32_f16(vreinterpret_f16_u16(vld1_u16(&i0[0])));
      vin1 = vcvt_f32_f16(vreinterpret_f16_u16(vld1_u16(&i0[4])));
      vin2 = vcvt_f32_f16(vreinterpret_f16_u16(vld1_u16(&i0[8])));
      vin3 = vcvt_f32_f16(vreinterpret_f16_u16(vld1_u16(&i0[12])));
      vin4 = vcvt_f32_f16(vreinterpret_f16_u16(vld1_u16(&i0[16])));
      vin5 = vcvt_f32_f16(vreinterpret_f16_u16(vld1_u16(&i0[20])));
      vin6 = vcvt_f32_f16(vreinterpret_f16_u16(vld1_u16(&i0[24])));
      vin7 = vcvt_f32_f16(vreinterpret_f16_u16(vld1_u16(&i0[28])));
      vacc0 = vaddq_f32(vin0, vacc0);
      vacc1 = vaddq_f32(vin1, vacc1);
      vacc2 = vaddq_f32(vin2, vacc2);
      vacc3 = vaddq_f32(vin3, vacc3);
      vacc4 = vaddq_f32(vin4, vacc4);
      vacc5 = vaddq_f32(vin5, vacc5);
      vacc6 = vaddq_f32(vin6, vacc6);
      vacc7 = vaddq_f32(vin7, vacc7);
      vin0 = vcvt_f32_f16(vreinterpret_f16_u16(vld1_u16(&i1[0])));
      vin1 = vcvt_f32_f16(vreinterpret_f16_u16(vld1_u16(&i1[4])));
      vin2 = vcvt_f32_f16(vreinterpret_f16_u16(vld1_u16(&i1[8])));
      vin3 = vcvt_f32_f16(vreinterpret_f16_u16(vld1_u16(&i1[12])));
      vin4 = vcvt_f32_f16(vreinterpret_f16_u16(vld1_u16(&i1[16])));
      vin5 = vcvt_f32_f16(vreinterpret_f16_u16(vld1_u16(&i1[20])));
      vin6 = vcvt_f32_f16(vreinterpret_f16_u16(vld1_u16(&i1[24])));
      vin7 = vcvt_f32_f16(vreinterpret_f16_u16(vld1_u16(&i1[28])));
      vacc0 = vaddq_f32(vin0, vacc0);
      vacc1 = vaddq_f32(vin1, vacc1);
      vacc2 = vaddq_f32(vin2, vacc2);
      vacc3 = vaddq_f32(vin3, vacc3);
      vacc4 = vaddq_f32(vin4, vacc4);
      vacc5 = vaddq_f32(vin5, vacc5);
      vacc6 = vaddq_f32(vin6, vacc6);
      vacc7 = vaddq_f32(vin7, vacc7);
      vin0 = vcvt_f32_f16(vreinterpret_f16_u16(vld1_u16(&i2[0])));
      vin1 = vcvt_f32_f16(vreinterpret_f16_u16(vld1_u16(&i2[4])));
      vin2 = vcvt_f32_f16(vreinterpret_f16_u16(vld1_u16(&i2[8])));
      vin3 = vcvt_f32_f16(vreinterpret_f16_u16(vld1_u16(&i2[12])));
      vin4 = vcvt_f32_f16(vreinterpret_f16_u16(vld1_u16(&i2[16])));
      vin5 = vcvt_f32_f16(vreinterpret_f16_u16(vld1_u16(&i2[20])));
      vin6 = vcvt_f32_f16(vreinterpret_f16_u16(vld1_u16(&i2[24])));
      vin7 = vcvt_f32_f16(vreinterpret_f16_u16(vld1_u16(&i2[28])));
      vacc0 = vaddq_f32(vin0, vacc0);
      vacc1 = vaddq_f32(vin1, vacc1);
      vacc2 = vaddq_f32(vin2, vacc2);
      vacc3 = vaddq_f32(vin3, vacc3);
      vacc4 = vaddq_f32(vin4, vacc4);
      vacc5 = vaddq_f32(vin5, vacc5);
      vacc6 = vaddq_f32(vin6, vacc6);
      vacc7 = vaddq_f32(vin7, vacc7);
      vin0 = vcvt_f32_f16(vreinterpret_f16_u16(vld1_u16(&i3[0])));
      vin1 = vcvt_f32_f16(vreinterpret_f16_u16(vld1_u16(&i3[4])));
      vin2 = vcvt_f32_f16(vreinterpret_f16_u16(vld1_u16(&i3[8])));
      vin3 = vcvt_f32_f16(vreinterpret_f16_u16(vld1_u16(&i3[12])));
      vin4 = vcvt_f32_f16(vreinterpret_f16_u16(vld1_u16(&i3[16])));
      vin5 = vcvt_f32_f16(vreinterpret_f16_u16(vld1_u16(&i3[20])));
      vin6 = vcvt_f32_f16(vreinterpret_f16_u16(vld1_u16(&i3[24])));
      vin7 = vcvt_f32_f16(vreinterpret_f16_u16(vld1_u16(&i3[28])));
      vacc0 = vaddq_f32(vin0, vacc0);
      vacc1 = vaddq_f32(vin1, vacc1);
      vacc2 = vaddq_f32(vin2, vacc2);
      vacc3 = vaddq_f32(vin3, vacc3);
      vacc4 = vaddq_f32(vin4, vacc4);
      vacc5 = vaddq_f32(vin5, vacc5);
      vacc6 = vaddq_f32(vin6, vacc6);
      vacc7 = vaddq_f32(vin7, vacc7);
      vin0 = vcvt_f32_f16(vreinterpret_f16_u16(vld1_u16(&i4[0])));
      vin1 = vcvt_f32_f16(vreinterpret_f16_u16(vld1_u16(&i4[4])));
      vin2 = vcvt_f32_f16(vreinterpret_f16_u16(vld1_u16(&i4[8])));
      vin3 = vcvt_f32_f16(vreinterpret_f16_u16(vld1_u16(&i4[12])));
      vin4 = vcvt_f32_f16(vreinterpret_f16_u16(vld1_u16(&i4[16])));
      vin5 = vcvt_f32_f16(vreinterpret_f16_u16(vld1_u16(&i4[20])));
      vin6 = vcvt_f32_f16(vreinterpret_f16_u16(vld1_u16(&i4[24])));
      vin7 = vcvt_f32_f16(vreinterpret_f16_u16(vld1_u16(&i4[28])));
      vacc0 = vaddq_f32(vin0, vacc0);
      vacc1 = vaddq_f32(vin1, vacc1);
      vacc2 = vaddq_f32(vin2, vacc2);
      vacc3 = vaddq_f32(vin3, vacc3);
      vacc4 = vaddq_f32(vin4, vacc4);
      vacc5 = vaddq_f32(vin5, vacc5);
      vacc6 = vaddq_f32(vin6, vacc6);
      vacc7 = vaddq_f32(vin7, vacc7);
      vin0 = vcvt_f32_f16(vreinterpret_f16_u16(vld1_u16(&i5[0])));
      vin1 = vcvt_f32_f16(vreinterpret_f16_u16(vld1_u16(&i5[4])));
      vin2 = vcvt_f32_f16(vreinterpret_f16_u16(vld1_u16(&i5[8])));
      vin3 = vcvt_f32_f16(vreinterpret_f16_u16(vld1_u16(&i5[12])));
      vin4 = vcvt_f32_f16(vreinterpret_f16_u16(vld1_u16(&i5[16])));
      vin5 = vcvt_f32_f16(vreinterpret_f16_u16(vld1_u16(&i5[20])));
      vin6 = vcvt_f32_f16(vreinterpret_f16_u16(vld1_u16(&i5[24])));
      vin7 = vcvt_f32_f16(vreinterpret_f16_u16(vld1_u16(&i5[28])));
      vacc0 = vaddq_f32(vin0, vacc0);
      vacc1 = vaddq_f32(vin1, vacc1);
      vacc2 = vaddq_f32(vin2, vacc2);
      vacc3 = vaddq_f32(vin3, vacc3);
      vacc4 = vaddq_f32(vin4, vacc4);
      vacc5 = vaddq_f32(vin5, vacc5);
      vacc6 = vaddq_f32(vin6, vacc6);
      vacc7 = vaddq_f32(vin7, vacc7);
      vin0 = vcvt_f32_f16(vreinterpret_f16_u16(vld1_u16(&i6[0])));
      vin1 = vcvt_f32_f16(vreinterpret_f16_u16(vld1_u16(&i6[4])));
      vin2 = vcvt_f32_f16(vreinterpret_f16_u16(vld1_u16(&i6[8])));
      vin3 = vcvt_f32_f16(vreinterpret_f16_u16(vld1_u16(&i6[12])));
      vin4 = vcvt_f32_f16(vreinterpret_f16_u16(vld1_u16(&i6[16])));
      vin5 = vcvt_f32_f16(vreinterpret_f16_u16(vld1_u16(&i6[20])));
      vin6 = vcvt_f32_f16(vreinterpret_f16_u16(vld1_u16(&i6[24])));
      vin7 = vcvt_f32_f16(vreinterpret_f16_u16(vld1_u16(&i6[28])));
      vacc0 = vaddq_f32(vin0, vacc0);
      vacc1 = vaddq_f32(vin1, vacc1);
      vacc2 = vaddq_f32(vin2, vacc2);
      vacc3 = vaddq_f32(vin3, vacc3);
      vacc4 = vaddq_f32(vin4, vacc4);
      vacc5 = vaddq_f32(vin5, vacc5);
      vacc6 = vaddq_f32(vin6, vacc6);
      vacc7 = vaddq_f32(vin7, vacc7);
      i0 = (const uint16_t*) ((uintptr_t) i0 + input_increment);
      i1 = (const uint16_t*) ((uintptr_t) i1 + input_increment);
      i2 = (const uint16_t*) ((uintptr_t) i2 + input_increment);
      i3 = (const uint16_t*) ((uintptr_t) i3 + input_increment);
      i4 = (const uint16_t*) ((uintptr_t) i4 + input_increment);
      i5 = (const uint16_t*) ((uintptr_t) i5 + input_increment);
      i6 = (const uint16_t*) ((uintptr_t) i6 + input_increment);
    }
    vacc0 = vmulq_f32(vacc0, vscale);
    vacc1 = vmulq_f32(vacc1, vscale);
    vacc2 = vmulq_f32(vacc2, vscale);
    vacc3 = vmulq_f32(vacc3, vscale);
    vacc4 = vmulq_f32(vacc4, vscale);
    vacc5 = vmulq_f32(vacc5, vscale);
    vacc6 = vmulq_f32(vacc6, vscale);
    vacc7 = vmulq_f32(vacc7, vscale);

    const uint16_t* o = (const uint16_t*) output;
    float16x4_t vo0 = vreinterpret_f16_u16(vld1_u16(o)); o += 4;
    float16x4_t vo1 = vreinterpret_f16_u16(vld1_u16(o)); o += 4;
    float16x4_t vo2 = vreinterpret_f16_u16(vld1_u16(o)); o += 4;
    float16x4_t vo3 = vreinterpret_f16_u16(vld1_u16(o)); o += 4;
    float16x4_t vo4 = vreinterpret_f16_u16(vld1_u16(o)); o += 4;
    float16x4_t vo5 = vreinterpret_f16_u16(vld1_u16(o)); o += 4;
    float16x4_t vo6 = vreinterpret_f16_u16(vld1_u16(o)); o += 4;
    float16x4_t vo7 = vreinterpret_f16_u16(vld1_u16(o)); o += 4;
    float16x4_t vfp16_out0 = vadd_f16(vo0, vcvt_f16_f32(vacc0));
    float16x4_t vfp16_out1 = vadd_f16(vo1, vcvt_f16_f32(vacc1));
    float16x4_t vfp16_out2 = vadd_f16(vo2, vcvt_f16_f32(vacc2));
    float16x4_t vfp16_out3 = vadd_f16(vo3, vcvt_f16_f32(vacc3));
    float16x4_t vfp16_out4 = vadd_f16(vo4, vcvt_f16_f32(vacc4));
    float16x4_t vfp16_out5 = vadd_f16(vo5, vcvt_f16_f32(vacc5));
    float16x4_t vfp16_out6 = vadd_f16(vo6, vcvt_f16_f32(vacc6));
    float16x4_t vfp16_out7 = vadd_f16(vo7, vcvt_f16_f32(vacc7));
    vst1_u16(output, vreinterpret_u16_f16(vfp16_out0)); output = (void*) ((uintptr_t) output + 4 * sizeof(uint16_t));
    vst1_u16(output, vreinterpret_u16_f16(vfp16_out1)); output = (void*) ((uintptr_t) output + 4 * sizeof(uint16_t));
    vst1_u16(output, vreinterpret_u16_f16(vfp16_out2)); output = (void*) ((uintptr_t) output + 4 * sizeof(uint16_t));
    vst1_u16(output, vreinterpret_u16_f16(vfp16_out3)); output = (void*) ((uintptr_t) output + 4 * sizeof(uint16_t));
    vst1_u16(output, vreinterpret_u16_f16(vfp16_out4)); output = (void*) ((uintptr_t) output + 4 * sizeof(uint16_t));
    vst1_u16(output, vreinterpret_u16_f16(vfp16_out5)); output = (void*) ((uintptr_t) output + 4 * sizeof(uint16_t));
    vst1_u16(output, vreinterpret_u16_f16(vfp16_out6)); output = (void*) ((uintptr_t) output + 4 * sizeof(uint16_t));
    vst1_u16(output, vreinterpret_u16_f16(vfp16_out7)); output = (void*) ((uintptr_t) output + 4 * sizeof(uint16_t));

    input = (const uint16_t*) ((uintptr_t) input + 32 * sizeof(uint16_t));
  }
  if (channels != 0) {
    input_increment = 7 * input_stride;
    const uint16_t* i0 = input;
    const uint16_t* i1 = (const uint16_t*) ((uintptr_t) input + 1 * input_stride);
    const uint16_t* i2 = (const uint16_t*) ((uintptr_t) input + 2 * input_stride);
    const uint16_t* i3 = (const uint16_t*) ((uintptr_t) input + 3 * input_stride);
    const uint16_t* i4 = (const uint16_t*) ((uintptr_t) input + 4 * input_stride);
    const uint16_t* i5 = (const uint16_t*) ((uintptr_t) input + 5 * input_stride);
    const uint16_t* i6 = (const uint16_t*) ((uintptr_t) input + 6 * input_stride);
    float32x4_t vacc[8];
    vacc[0] = vdupq_n_f32(0.f);
    vacc[1] = vdupq_n_f32(0.f);
    vacc[2] = vdupq_n_f32(0.f);
    vacc[3] = vdupq_n_f32(0.f);
    vacc[4] = vdupq_n_f32(0.f);
    vacc[5] = vdupq_n_f32(0.f);
    vacc[6] = vdupq_n_f32(0.f);
    vacc[7] = vdupq_n_f32(0.f);

    const size_t num_chunks = round_up_po2(channels, 4) >> 2;
    const size_t num_full_chunks = channels >> 2;
    for (int r = rows; r > 0; r -= 7) {
      if XNN_UNPREDICTABLE(r < 2) {
        i1 = zero;
      }
      if XNN_UNPREDICTABLE(r <= 2) {
        i2 = zero;
      }
      if XNN_UNPREDICTABLE(r < 4) {
        i3 = zero;
      }
      if XNN_UNPREDICTABLE(r <= 4) {
        i4 = zero;
      }
      if XNN_UNPREDICTABLE(r < 6) {
        i5 = zero;
      }
      if XNN_UNPREDICTABLE(r <= 6) {
        i6 = zero;
      }
      for (int i = 0; i < num_chunks; ++i) {
        vacc[i] = vaddq_f32(vcvt_f32_f16(vreinterpret_f16_u16(vld1_u16(&i0[i*4]))), vacc[i]);
        vacc[i] = vaddq_f32(vcvt_f32_f16(vreinterpret_f16_u16(vld1_u16(&i1[i*4]))), vacc[i]);
        vacc[i] = vaddq_f32(vcvt_f32_f16(vreinterpret_f16_u16(vld1_u16(&i2[i*4]))), vacc[i]);
        vacc[i] = vaddq_f32(vcvt_f32_f16(vreinterpret_f16_u16(vld1_u16(&i3[i*4]))), vacc[i]);
        vacc[i] = vaddq_f32(vcvt_f32_f16(vreinterpret_f16_u16(vld1_u16(&i4[i*4]))), vacc[i]);
        vacc[i] = vaddq_f32(vcvt_f32_f16(vreinterpret_f16_u16(vld1_u16(&i5[i*4]))), vacc[i]);
        vacc[i] = vaddq_f32(vcvt_f32_f16(vreinterpret_f16_u16(vld1_u16(&i6[i*4]))), vacc[i]);
      }
      i0 = (const uint16_t*) ((uintptr_t) i0 + input_increment);
      i1 = (const uint16_t*) ((uintptr_t) i1 + input_increment);
      i2 = (const uint16_t*) ((uintptr_t) i2 + input_increment);
      i3 = (const uint16_t*) ((uintptr_t) i3 + input_increment);
      i4 = (const uint16_t*) ((uintptr_t) i4 + input_increment);
      i5 = (const uint16_t*) ((uintptr_t) i5 + input_increment);
      i6 = (const uint16_t*) ((uintptr_t) i6 + input_increment);
    }
    for (int i = 0; i < (channels + 4) >> 2; ++i) {
      vacc[i] = vmulq_f32(vacc[i], vscale);
    }

    float16x4_t vo[8];
    const uint16_t* o = (const uint16_t*) output;
    for (int i = 0; i < num_full_chunks; ++i) {
      vo[i] = vreinterpret_f16_u16(vld1_u16(o)); o += 4;
    }
    float16x4_t vfp16_out[8];
    for (int i = 0; i < num_full_chunks; ++i) {
      vfp16_out[i] = vadd_f16(vo[i], vcvt_f16_f32(vacc[i]));
    }
    for (int i = 0; i < num_full_chunks; ++i) {
      vst1_u16(output, vreinterpret_u16_f16(vfp16_out[i])); output = (void*) ((uintptr_t) output + 4 * sizeof(uint16_t));
    }

    const size_t pos = channels >> 2;
    channels &= 0x3;
    float16x4_t vacc_low = vcvt_f16_f32(vacc[pos]);
    if (channels & 2) {
      vst1_lane_u32(output, vreinterpret_u32_f16(vadd_f16(vacc_low, vreinterpret_f16_u32(vld1_dup_u32(output)))), 0); output = (void*) ((uintptr_t) output + 2 * sizeof(uint16_t));
      vacc_low = vext_f16(vacc_low, vacc_low, 2);
    }
    if (channels & 1) {
      vst1_lane_u16(output, vreinterpret_u16_f16(vadd_f16(vacc_low, vreinterpret_f16_u16(vld1_dup_u16(output)))), 0);
    }
  }
}
