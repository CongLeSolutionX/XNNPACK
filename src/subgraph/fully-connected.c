// Copyright 2020 Google LLC
//
// This source code is licensed under the BSD-style license found in the
// LICENSE file in the root directory of this source tree.

#include <math.h>
#include <stddef.h>
#include <stdint.h>

#include <xnnpack.h>
#include <xnnpack/log.h>
#include <xnnpack/params.h>
#include <xnnpack/subgraph.h>


static enum xnn_status create_fully_connected_operator(
  const struct xnn_node* node,
  const struct xnn_value* values,
  size_t num_values,
  struct xnn_operator_data* opdata)
{
  assert(node->num_inputs >= 2);
  assert(node->num_inputs <= 3);
  const uint32_t input_id = node->inputs[0];
  assert(input_id != XNN_INVALID_VALUE_ID);
  assert(input_id < num_values);
  const uint32_t filter_id = node->inputs[1];
  assert(filter_id != XNN_INVALID_VALUE_ID);
  assert(filter_id < num_values);

  assert(node->num_outputs == 1);
  const uint32_t output_id = node->outputs[0];
  assert(output_id != XNN_INVALID_VALUE_ID);
  assert(output_id < num_values);

  const size_t num_input_elements = xnn_shape_multiply_all_dims(&values[node->inputs[0]].shape);
  size_t output_channels, input_channels;
  if (node->flags & XNN_FLAG_TRANSPOSE_WEIGHTS) {
    input_channels = values[node->inputs[1]].shape.dim[0];
    output_channels = values[node->inputs[1]].shape.dim[1];
  } else {
    output_channels = values[node->inputs[1]].shape.dim[0];
    input_channels = values[node->inputs[1]].shape.dim[1];
  }

  const void* filter_data = values[filter_id].data;
  assert(filter_data != NULL);

  const void* bias_data = NULL;
  if (node->num_inputs > 2) {
    const uint32_t bias_id = node->inputs[2];
    assert(bias_id != XNN_INVALID_VALUE_ID);
    assert(bias_id < num_values);

    bias_data = values[bias_id].data;
    assert(bias_data != NULL);
  }

  enum xnn_status status;
  switch (values[node->outputs[0]].datatype) {
    case xnn_datatype_fp32:
      status = xnn_create_fully_connected_nc_f32(
        input_channels,
        output_channels,
        input_channels /* input stride */,
        output_channels /* output stride */,
        filter_data,
        bias_data,
        node->activation.output_min,
        node->activation.output_max,
        node->flags /* flags */,
        &opdata->operator_object);
      break;
#ifndef XNN_NO_QS8_OPERATORS
    case xnn_datatype_qint8:
    {
      const float output_scale = values[output_id].quantization.scale;
      const int32_t output_zero_point = values[output_id].quantization.zero_point;
      const int8_t output_min =
        (int8_t) lrintf(fminf(fmaxf(node->activation.output_min / output_scale + (float) output_zero_point, -128.0f), 127.0f));
      const int8_t output_max =
        (int8_t) lrintf(fminf(fmaxf(node->activation.output_max / output_scale + (float) output_zero_point, -128.0f), 127.0f));
      status = xnn_create_fully_connected_nc_qs8(
        input_channels,
        output_channels,
        input_channels /* input stride */,
        output_channels /* output stride */,
        (int8_t) values[input_id].quantization.zero_point,
        values[input_id].quantization.scale,
        values[filter_id].quantization.scale,
        filter_data,
        bias_data,
        (int8_t) output_zero_point,
        output_scale, output_min, output_max,
        node->flags /* flags */,
        &opdata->operator_object);
      break;
    }
#endif  // !defined(XNN_NO_QS8_OPERATORS)
#ifndef XNN_NO_QU8_OPERATORS
    case xnn_datatype_quint8:
    {
      const float output_scale = values[output_id].quantization.scale;
      const int32_t output_zero_point = values[output_id].quantization.zero_point;
      const uint8_t output_min =
        (uint8_t) lrintf(fminf(fmaxf(node->activation.output_min / output_scale + (float) output_zero_point, 0.0f), 255.0f));
      const uint8_t output_max =
        (uint8_t) lrintf(fminf(fmaxf(node->activation.output_max / output_scale + (float) output_zero_point, 0.0f), 255.0f));
      status = xnn_create_fully_connected_nc_qu8(
        input_channels,
        output_channels,
        input_channels /* input stride */,
        output_channels /* output stride */,
        (uint8_t) values[input_id].quantization.zero_point,
        values[input_id].quantization.scale,
        (uint8_t) values[filter_id].quantization.zero_point,
        values[filter_id].quantization.scale,
        filter_data,
        bias_data,
        (uint8_t) output_zero_point,
        output_scale, output_min, output_max,
        node->flags /* flags */,
        &opdata->operator_object);
      break;
    }
#endif  // !defined(XNN_NO_QU8_OPERATORS)
    default:
      XNN_UNREACHABLE;
  }
  if (status == xnn_status_success) {
    opdata->batch_size = num_input_elements / input_channels;
    opdata->inputs[0] = input_id;
    opdata->outputs[0] = output_id;
  }
  return status;
}

static enum xnn_status setup_fully_connected_operator(
  const struct xnn_operator_data* opdata,
  const struct xnn_blob* blobs,
  size_t num_blobs,
  pthreadpool_t threadpool)
{
  const uint32_t input_id = opdata->inputs[0];
  assert(input_id != XNN_INVALID_VALUE_ID);
  assert(input_id < num_blobs);

  const uint32_t output_id = opdata->outputs[0];
  assert(output_id != XNN_INVALID_VALUE_ID);
  assert(output_id < num_blobs);

  const struct xnn_blob* input_blob = blobs + input_id;
  const void* input_data = input_blob->data;
  assert(input_data != NULL);

  const struct xnn_blob* output_blob = blobs + output_id;
  void* output_data = output_blob->data;
  assert(output_data != NULL);

  switch (opdata->operator_object->type) {
    case xnn_operator_type_fully_connected_nc_f32:
      return xnn_setup_fully_connected_nc_f32(
        opdata->operator_object,
        opdata->batch_size,
        input_data,
        output_data,
        threadpool);
#ifndef XNN_NO_QS8_OPERATORS
    case xnn_operator_type_fully_connected_nc_qs8:
      return xnn_setup_fully_connected_nc_qs8(
        opdata->operator_object,
        opdata->batch_size,
        input_data,
        output_data,
        threadpool);
#endif  // !defined(XNN_NO_QS8_OPERATORS)
#ifndef XNN_NO_QU8_OPERATORS
    case xnn_operator_type_fully_connected_nc_qu8:
      return xnn_setup_fully_connected_nc_qu8(
        opdata->operator_object,
        opdata->batch_size,
        input_data,
        output_data,
        threadpool);
#endif  // !defined(XNN_NO_QU8_OPERATORS)
    default:
      XNN_UNREACHABLE;
  }
}

static inline bool check_datatypes_with_bias(
  enum xnn_datatype input_datatype,
  enum xnn_datatype filter_datatype,
  enum xnn_datatype bias_datatype,
  enum xnn_datatype output_datatype)
{
  switch (output_datatype) {
    case xnn_datatype_fp32:
      return input_datatype == xnn_datatype_fp32 &&
        filter_datatype == xnn_datatype_fp32 && bias_datatype == xnn_datatype_fp32;
#ifndef XNN_NO_QS8_OPERATORS
    case xnn_datatype_qint8:
      return input_datatype == xnn_datatype_qint8 &&
        filter_datatype == xnn_datatype_qint8 && bias_datatype == xnn_datatype_qint32;
#endif  // !defined(XNN_NO_QS8_OPERATORS)
#ifndef XNN_NO_QU8_OPERATORS
    case xnn_datatype_quint8:
      return input_datatype == xnn_datatype_quint8 &&
        filter_datatype == xnn_datatype_quint8 && bias_datatype == xnn_datatype_qint32;
#endif  // !defined(XNN_NO_QU8_OPERATORS)
    default:
      XNN_UNREACHABLE;
  }
}

static inline bool check_datatypes_without_bias(
  enum xnn_datatype input_datatype,
  enum xnn_datatype filter_datatype,
  enum xnn_datatype output_datatype)
{
  switch (output_datatype) {
    case xnn_datatype_fp32:
      return input_datatype == xnn_datatype_fp32 && filter_datatype == xnn_datatype_fp32;
#ifndef XNN_NO_QS8_OPERATORS
    case xnn_datatype_qint8:
      return input_datatype == xnn_datatype_qint8 && filter_datatype == xnn_datatype_qint8;
#endif  // !defined(XNN_NO_QS8_OPERATORS)
#ifndef XNN_NO_QU8_OPERATORS
    case xnn_datatype_quint8:
      return input_datatype == xnn_datatype_quint8 && filter_datatype == xnn_datatype_quint8;
#endif  // !defined(XNN_NO_QU8_OPERATORS)
    default:
      XNN_UNREACHABLE;
  }
}

enum xnn_status xnn_define_fully_connected(
  xnn_subgraph_t subgraph,
  float output_min,
  float output_max,
  uint32_t input_id,
  uint32_t filter_id,
  uint32_t bias_id,
  uint32_t output_id,
  uint32_t flags)
{
  if ((xnn_params.init_flags & XNN_INIT_FLAG_XNNPACK) == 0) {
    xnn_log_error("failed to define %s operator: XNNPACK is not initialized",
      xnn_node_type_to_string(xnn_node_type_fully_connected));
    return xnn_status_uninitialized;
  }

  if (isnan(output_min)) {
    xnn_log_error(
      "failed to define %s operator with NaN output lower bound: lower bound must be non-NaN",
      xnn_node_type_to_string(xnn_node_type_fully_connected));
    return xnn_status_invalid_parameter;
  }

  if (isnan(output_max)) {
    xnn_log_error(
      "failed to define %s operator with NaN output upper bound: upper bound must be non-NaN",
      xnn_node_type_to_string(xnn_node_type_fully_connected));
    return xnn_status_invalid_parameter;
  }

  if (output_min >= output_max) {
    xnn_log_error(
      "failed to define %s operator with [%.7g, %.7g] output range: lower bound must be below upper bound",
      xnn_node_type_to_string(xnn_node_type_fully_connected), output_min, output_max);
    return xnn_status_invalid_parameter;
  }

  if (input_id >= subgraph->num_values) {
    xnn_log_error(
      "failed to define %s operator with input ID #%" PRIu32 ": invalid Value ID",
      xnn_node_type_to_string(xnn_node_type_fully_connected), input_id);
    return xnn_status_invalid_parameter;
  }

  const struct xnn_value* input_value = &subgraph->values[input_id];
  if (input_value->type != xnn_value_type_dense_tensor) {
    xnn_log_error(
      "failed to define %s operator with input ID #%" PRIu32 ": unsupported Value type %d (expected dense tensor)",
      xnn_node_type_to_string(xnn_node_type_fully_connected), input_id, input_value->type);
    return xnn_status_invalid_parameter;
  }

  switch (input_value->datatype) {
    case xnn_datatype_fp32:
#ifndef XNN_NO_QS8_OPERATORS
    case xnn_datatype_qint8:
#endif  // !defined(XNN_NO_QS8_OPERATORS)
#ifndef XNN_NO_QU8_OPERATORS
    case xnn_datatype_quint8:
#endif  // !defined(XNN_NO_QS8_OPERATORS)
      break;
    default:
      xnn_log_error(
        "failed to define %s operator with input ID #%" PRIu32 ": unsupported Value datatype %s (%d)",
        xnn_node_type_to_string(xnn_node_type_fully_connected), input_id,
        xnn_datatype_to_string(input_value->datatype), input_value->datatype);
      return xnn_status_invalid_parameter;
  }

  if (filter_id >= subgraph->num_values) {
    xnn_log_error(
      "failed to define %s operator with filter ID #%" PRIu32 ": invalid Value ID",
      xnn_node_type_to_string(xnn_node_type_fully_connected), filter_id);
    return xnn_status_invalid_parameter;
  }

  const struct xnn_value* filter_value = &subgraph->values[filter_id];
  if (filter_value->type != xnn_value_type_dense_tensor) {
    xnn_log_error(
      "failed to define %s operator with filter ID #%" PRIu32 ": unsupported Value type %d (expected dense tensor)",
      xnn_node_type_to_string(xnn_node_type_fully_connected), filter_id, filter_value->type);
    return xnn_status_invalid_parameter;
  }

  if (filter_value->data == NULL) {
    xnn_log_error(
      "failed to define %s operator with filter ID #%" PRIu32 ": non-static Value",
      xnn_node_type_to_string(xnn_node_type_fully_connected), filter_id);
    return xnn_status_invalid_parameter;
  }

  switch (filter_value->datatype) {
    case xnn_datatype_fp32:
      break;
#ifndef XNN_NO_QS8_OPERATORS
    case xnn_datatype_qint8:
      if (filter_value->quantization.zero_point != 0) {
        xnn_log_error(
          "failed to define %s operator with filter ID #%" PRIu32 ": unsupported quantization zero point %" PRId32 " for datatype %s",
          xnn_node_type_to_string(xnn_node_type_convolution_2d), filter_id,
          filter_value->quantization.zero_point, xnn_datatype_to_string(filter_value->datatype));
      }
      break;
#endif  // !defined(XNN_NO_QS8_OPERATORS)
#ifndef XNN_NO_QU8_OPERATORS
    case xnn_datatype_quint8:
      break;
#endif  // !defined(XNN_NO_QU8_OPERATORS)
    default:
      xnn_log_error(
        "failed to define %s operator with filter ID #%" PRIu32 ": unsupported Value datatype %s (%d)",
        xnn_node_type_to_string(xnn_node_type_fully_connected), filter_id,
        xnn_datatype_to_string(filter_value->datatype), filter_value->datatype);
      return xnn_status_invalid_parameter;
  }

  const struct xnn_value* bias_value = NULL;
  if (bias_id != XNN_INVALID_VALUE_ID) {
    if (bias_id >= subgraph->num_values) {
      xnn_log_error(
        "failed to define %s operator with bias ID #%" PRIu32 ": invalid Value ID",
        xnn_node_type_to_string(xnn_node_type_fully_connected), bias_id);
      return xnn_status_invalid_parameter;
    }

    bias_value = &subgraph->values[bias_id];
    if (bias_value->type != xnn_value_type_dense_tensor) {
      xnn_log_error(
        "failed to define %s operator with bias ID #%" PRIu32 ": unsupported Value type %d (expected dense tensor)",
        xnn_node_type_to_string(xnn_node_type_fully_connected), bias_id, bias_value->type);
      return xnn_status_invalid_parameter;
    }

    if (bias_value->data == NULL) {
      xnn_log_error(
        "failed to define %s operator with bias ID #%" PRIu32 ": non-static Value",
        xnn_node_type_to_string(xnn_node_type_fully_connected), bias_id);
      return xnn_status_invalid_parameter;
    }

    switch (bias_value->datatype) {
      case xnn_datatype_fp32:
#if !defined(XNN_NO_QS8_OPERATORS) || !defined(XNN_NO_QU8_OPERATORS)
      case xnn_datatype_qint32:
#endif  // !defined(XNN_NO_QS8_OPERATORS) || !defined(XNN_NO_QU8_OPERATORS)
        break;
      default:
        xnn_log_error(
          "failed to define %s operator with bias ID #%" PRIu32 ": unsupported Value datatype %s (%d)",
          xnn_node_type_to_string(xnn_node_type_fully_connected), bias_id,
          xnn_datatype_to_string(bias_value->datatype), bias_value->datatype);
        return xnn_status_invalid_parameter;
    }
  }

  if (output_id >= subgraph->num_values) {
    xnn_log_error(
      "failed to define %s operator with output ID #%" PRIu32 ": invalid Value ID",
      xnn_node_type_to_string(xnn_node_type_fully_connected), output_id);
    return xnn_status_invalid_parameter;
  }

  const struct xnn_value* output_value = &subgraph->values[output_id];
  if (output_value->type != xnn_value_type_dense_tensor) {
    xnn_log_error(
      "failed to define %s operator with output ID #%" PRIu32 ": unsupported Value type %d (expected dense tensor)",
      xnn_node_type_to_string(xnn_node_type_fully_connected), output_id, output_value->type);
    return xnn_status_invalid_parameter;
  }

  switch (output_value->datatype) {
    case xnn_datatype_fp32:
#ifndef XNN_NO_QS8_OPERATORS
    case xnn_datatype_qint8:
#endif  // !defined(XNN_NO_QS8_OPERATORS)
#ifndef XNN_NO_QU8_OPERATORS
    case xnn_datatype_quint8:
#endif  // !defined(XNN_NO_QU8_OPERATORS)
      break;
    default:
      xnn_log_error(
        "failed to define %s operator with output ID #%" PRIu32 ": unsupported Value datatype %s (%d)",
        xnn_node_type_to_string(xnn_node_type_fully_connected), output_id,
        xnn_datatype_to_string(output_value->datatype), output_value->datatype);
      return xnn_status_invalid_parameter;
  }

  if (bias_value != NULL) {
    if (!check_datatypes_with_bias(input_value->datatype, filter_value->datatype, bias_value->datatype, output_value->datatype)) {
      xnn_log_error(
        "failed to define %s operator with input ID #%" PRIu32 ", filter ID #%" PRIu32 ", bias ID #%" PRIu32 ", and output ID #%" PRIu32
        ": mismatching datatypes across input (%s), filter (%s), bias (%s), and output (%s)",
        xnn_node_type_to_string(xnn_node_type_fully_connected), input_id, filter_id, bias_id, output_id,
        xnn_datatype_to_string(input_value->datatype),
        xnn_datatype_to_string(filter_value->datatype),
        xnn_datatype_to_string(bias_value->datatype),
        xnn_datatype_to_string(output_value->datatype));
      return xnn_status_invalid_parameter;
    }
  } else {
    if (!check_datatypes_without_bias(input_value->datatype, filter_value->datatype, output_value->datatype)) {
      xnn_log_error(
        "failed to define %s operator with input ID #%" PRIu32 ", filter ID #%" PRIu32 ", and output ID #%" PRIu32
        ": mismatching datatypes across input (%s), filter (%s), and output (%s)",
        xnn_node_type_to_string(xnn_node_type_fully_connected), input_id, filter_id, output_id,
        xnn_datatype_to_string(input_value->datatype),
        xnn_datatype_to_string(filter_value->datatype),
        xnn_datatype_to_string(output_value->datatype));
      return xnn_status_invalid_parameter;
    }
  }

  struct xnn_node* node = xnn_subgraph_new_node(subgraph);
  if (node == NULL) {
    return xnn_status_out_of_memory;
  }

  node->type = xnn_node_type_fully_connected;
  node->activation.output_min = output_min;
  node->activation.output_max = output_max;
  node->num_inputs = 2 + (size_t) (bias_id != XNN_INVALID_VALUE_ID);
  node->inputs[0] = input_id;
  node->inputs[1] = filter_id;
  node->inputs[2] = bias_id;
  node->num_outputs = 1;
  node->outputs[0] = output_id;
  node->flags = flags;

  node->create = create_fully_connected_operator;
  node->setup = setup_fully_connected_operator;

  return xnn_status_success;
}
