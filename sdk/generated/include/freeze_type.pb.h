// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: freeze_type.proto

#ifndef GOOGLE_PROTOBUF_INCLUDED_freeze_5ftype_2eproto
#define GOOGLE_PROTOBUF_INCLUDED_freeze_5ftype_2eproto

#include <limits>
#include <string>

#include <google/protobuf/port_def.inc>
#if PROTOBUF_VERSION < 3021000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers. Please update
#error your headers.
#endif
#if 3021005 < PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers. Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/port_undef.inc>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/arena.h>
#include <google/protobuf/arenastring.h>
#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/metadata_lite.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/repeated_field.h>  // IWYU pragma: export
#include <google/protobuf/extension_set.h>  // IWYU pragma: export
#include <google/protobuf/generated_enum_reflection.h>
// @@protoc_insertion_point(includes)
#include <google/protobuf/port_def.inc>
#define PROTOBUF_INTERNAL_EXPORT_freeze_5ftype_2eproto
PROTOBUF_NAMESPACE_OPEN
namespace internal {
class AnyMetadata;
}  // namespace internal
PROTOBUF_NAMESPACE_CLOSE

// Internal implementation detail -- do not use these members.
struct TableStruct_freeze_5ftype_2eproto {
  static const uint32_t offsets[];
};
extern const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_freeze_5ftype_2eproto;
PROTOBUF_NAMESPACE_OPEN
PROTOBUF_NAMESPACE_CLOSE
namespace proto {

enum FreezeType : int {
  UNKNOWN_FREEZE_TYPE = 0,
  FREEZE_ONLY = 1,
  PREPARE_UPGRADE = 2,
  FREEZE_UPGRADE = 3,
  FREEZE_ABORT = 4,
  TELEMETRY_UPGRADE = 5,
  FreezeType_INT_MIN_SENTINEL_DO_NOT_USE_ = std::numeric_limits<int32_t>::min(),
  FreezeType_INT_MAX_SENTINEL_DO_NOT_USE_ = std::numeric_limits<int32_t>::max()
};
bool FreezeType_IsValid(int value);
constexpr FreezeType FreezeType_MIN = UNKNOWN_FREEZE_TYPE;
constexpr FreezeType FreezeType_MAX = TELEMETRY_UPGRADE;
constexpr int FreezeType_ARRAYSIZE = FreezeType_MAX + 1;

const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor* FreezeType_descriptor();
template<typename T>
inline const std::string& FreezeType_Name(T enum_t_value) {
  static_assert(::std::is_same<T, FreezeType>::value ||
    ::std::is_integral<T>::value,
    "Incorrect type passed to function FreezeType_Name.");
  return ::PROTOBUF_NAMESPACE_ID::internal::NameOfEnum(
    FreezeType_descriptor(), enum_t_value);
}
inline bool FreezeType_Parse(
    ::PROTOBUF_NAMESPACE_ID::ConstStringParam name, FreezeType* value) {
  return ::PROTOBUF_NAMESPACE_ID::internal::ParseNamedEnum<FreezeType>(
    FreezeType_descriptor(), name, value);
}
// ===================================================================


// ===================================================================


// ===================================================================

#ifdef __GNUC__
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // __GNUC__
#ifdef __GNUC__
  #pragma GCC diagnostic pop
#endif  // __GNUC__

// @@protoc_insertion_point(namespace_scope)

}  // namespace proto

PROTOBUF_NAMESPACE_OPEN

template <> struct is_proto_enum< ::proto::FreezeType> : ::std::true_type {};
template <>
inline const EnumDescriptor* GetEnumDescriptor< ::proto::FreezeType>() {
  return ::proto::FreezeType_descriptor();
}

PROTOBUF_NAMESPACE_CLOSE

// @@protoc_insertion_point(global_scope)

#include <google/protobuf/port_undef.inc>
#endif  // GOOGLE_PROTOBUF_INCLUDED_GOOGLE_PROTOBUF_INCLUDED_freeze_5ftype_2eproto
