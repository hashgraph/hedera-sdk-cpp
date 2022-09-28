// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: file_delete.proto

#ifndef GOOGLE_PROTOBUF_INCLUDED_file_5fdelete_2eproto
#define GOOGLE_PROTOBUF_INCLUDED_file_5fdelete_2eproto

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
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>  // IWYU pragma: export
#include <google/protobuf/extension_set.h>  // IWYU pragma: export
#include <google/protobuf/unknown_field_set.h>
#include "basic_types.pb.h"
// @@protoc_insertion_point(includes)
#include <google/protobuf/port_def.inc>
#define PROTOBUF_INTERNAL_EXPORT_file_5fdelete_2eproto
PROTOBUF_NAMESPACE_OPEN
namespace internal {
class AnyMetadata;
}  // namespace internal
PROTOBUF_NAMESPACE_CLOSE

// Internal implementation detail -- do not use these members.
struct TableStruct_file_5fdelete_2eproto {
  static const uint32_t offsets[];
};
extern const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_file_5fdelete_2eproto;
namespace proto {
class FileDeleteTransactionBody;
struct FileDeleteTransactionBodyDefaultTypeInternal;
extern FileDeleteTransactionBodyDefaultTypeInternal _FileDeleteTransactionBody_default_instance_;
}  // namespace proto
PROTOBUF_NAMESPACE_OPEN
template<> ::proto::FileDeleteTransactionBody* Arena::CreateMaybeMessage<::proto::FileDeleteTransactionBody>(Arena*);
PROTOBUF_NAMESPACE_CLOSE
namespace proto {

// ===================================================================

class FileDeleteTransactionBody final :
    public ::PROTOBUF_NAMESPACE_ID::Message /* @@protoc_insertion_point(class_definition:proto.FileDeleteTransactionBody) */ {
 public:
  inline FileDeleteTransactionBody() : FileDeleteTransactionBody(nullptr) {}
  ~FileDeleteTransactionBody() override;
  explicit PROTOBUF_CONSTEXPR FileDeleteTransactionBody(::PROTOBUF_NAMESPACE_ID::internal::ConstantInitialized);

  FileDeleteTransactionBody(const FileDeleteTransactionBody& from);
  FileDeleteTransactionBody(FileDeleteTransactionBody&& from) noexcept
    : FileDeleteTransactionBody() {
    *this = ::std::move(from);
  }

  inline FileDeleteTransactionBody& operator=(const FileDeleteTransactionBody& from) {
    CopyFrom(from);
    return *this;
  }
  inline FileDeleteTransactionBody& operator=(FileDeleteTransactionBody&& from) noexcept {
    if (this == &from) return *this;
    if (GetOwningArena() == from.GetOwningArena()
  #ifdef PROTOBUF_FORCE_COPY_IN_MOVE
        && GetOwningArena() != nullptr
  #endif  // !PROTOBUF_FORCE_COPY_IN_MOVE
    ) {
      InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }

  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* descriptor() {
    return GetDescriptor();
  }
  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* GetDescriptor() {
    return default_instance().GetMetadata().descriptor;
  }
  static const ::PROTOBUF_NAMESPACE_ID::Reflection* GetReflection() {
    return default_instance().GetMetadata().reflection;
  }
  static const FileDeleteTransactionBody& default_instance() {
    return *internal_default_instance();
  }
  static inline const FileDeleteTransactionBody* internal_default_instance() {
    return reinterpret_cast<const FileDeleteTransactionBody*>(
               &_FileDeleteTransactionBody_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    0;

  friend void swap(FileDeleteTransactionBody& a, FileDeleteTransactionBody& b) {
    a.Swap(&b);
  }
  inline void Swap(FileDeleteTransactionBody* other) {
    if (other == this) return;
  #ifdef PROTOBUF_FORCE_COPY_IN_SWAP
    if (GetOwningArena() != nullptr &&
        GetOwningArena() == other->GetOwningArena()) {
   #else  // PROTOBUF_FORCE_COPY_IN_SWAP
    if (GetOwningArena() == other->GetOwningArena()) {
  #endif  // !PROTOBUF_FORCE_COPY_IN_SWAP
      InternalSwap(other);
    } else {
      ::PROTOBUF_NAMESPACE_ID::internal::GenericSwap(this, other);
    }
  }
  void UnsafeArenaSwap(FileDeleteTransactionBody* other) {
    if (other == this) return;
    GOOGLE_DCHECK(GetOwningArena() == other->GetOwningArena());
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  FileDeleteTransactionBody* New(::PROTOBUF_NAMESPACE_ID::Arena* arena = nullptr) const final {
    return CreateMaybeMessage<FileDeleteTransactionBody>(arena);
  }
  using ::PROTOBUF_NAMESPACE_ID::Message::CopyFrom;
  void CopyFrom(const FileDeleteTransactionBody& from);
  using ::PROTOBUF_NAMESPACE_ID::Message::MergeFrom;
  void MergeFrom( const FileDeleteTransactionBody& from) {
    FileDeleteTransactionBody::MergeImpl(*this, from);
  }
  private:
  static void MergeImpl(::PROTOBUF_NAMESPACE_ID::Message& to_msg, const ::PROTOBUF_NAMESPACE_ID::Message& from_msg);
  public:
  PROTOBUF_ATTRIBUTE_REINITIALIZES void Clear() final;
  bool IsInitialized() const final;

  size_t ByteSizeLong() const final;
  const char* _InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) final;
  uint8_t* _InternalSerialize(
      uint8_t* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const final;
  int GetCachedSize() const final { return _impl_._cached_size_.Get(); }

  private:
  void SharedCtor(::PROTOBUF_NAMESPACE_ID::Arena* arena, bool is_message_owned);
  void SharedDtor();
  void SetCachedSize(int size) const final;
  void InternalSwap(FileDeleteTransactionBody* other);

  private:
  friend class ::PROTOBUF_NAMESPACE_ID::internal::AnyMetadata;
  static ::PROTOBUF_NAMESPACE_ID::StringPiece FullMessageName() {
    return "proto.FileDeleteTransactionBody";
  }
  protected:
  explicit FileDeleteTransactionBody(::PROTOBUF_NAMESPACE_ID::Arena* arena,
                       bool is_message_owned = false);
  public:

  static const ClassData _class_data_;
  const ::PROTOBUF_NAMESPACE_ID::Message::ClassData*GetClassData() const final;

  ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadata() const final;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  enum : int {
    kFileIDFieldNumber = 2,
  };
  // .proto.FileID fileID = 2;
  bool has_fileid() const;
  private:
  bool _internal_has_fileid() const;
  public:
  void clear_fileid();
  const ::proto::FileID& fileid() const;
  PROTOBUF_NODISCARD ::proto::FileID* release_fileid();
  ::proto::FileID* mutable_fileid();
  void set_allocated_fileid(::proto::FileID* fileid);
  private:
  const ::proto::FileID& _internal_fileid() const;
  ::proto::FileID* _internal_mutable_fileid();
  public:
  void unsafe_arena_set_allocated_fileid(
      ::proto::FileID* fileid);
  ::proto::FileID* unsafe_arena_release_fileid();

  // @@protoc_insertion_point(class_scope:proto.FileDeleteTransactionBody)
 private:
  class _Internal;

  template <typename T> friend class ::PROTOBUF_NAMESPACE_ID::Arena::InternalHelper;
  typedef void InternalArenaConstructable_;
  typedef void DestructorSkippable_;
  struct Impl_ {
    ::proto::FileID* fileid_;
    mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
  };
  union { Impl_ _impl_; };
  friend struct ::TableStruct_file_5fdelete_2eproto;
};
// ===================================================================


// ===================================================================

#ifdef __GNUC__
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // __GNUC__
// FileDeleteTransactionBody

// .proto.FileID fileID = 2;
inline bool FileDeleteTransactionBody::_internal_has_fileid() const {
  return this != internal_default_instance() && _impl_.fileid_ != nullptr;
}
inline bool FileDeleteTransactionBody::has_fileid() const {
  return _internal_has_fileid();
}
inline const ::proto::FileID& FileDeleteTransactionBody::_internal_fileid() const {
  const ::proto::FileID* p = _impl_.fileid_;
  return p != nullptr ? *p : reinterpret_cast<const ::proto::FileID&>(
      ::proto::_FileID_default_instance_);
}
inline const ::proto::FileID& FileDeleteTransactionBody::fileid() const {
  // @@protoc_insertion_point(field_get:proto.FileDeleteTransactionBody.fileID)
  return _internal_fileid();
}
inline void FileDeleteTransactionBody::unsafe_arena_set_allocated_fileid(
    ::proto::FileID* fileid) {
  if (GetArenaForAllocation() == nullptr) {
    delete reinterpret_cast<::PROTOBUF_NAMESPACE_ID::MessageLite*>(_impl_.fileid_);
  }
  _impl_.fileid_ = fileid;
  if (fileid) {
    
  } else {
    
  }
  // @@protoc_insertion_point(field_unsafe_arena_set_allocated:proto.FileDeleteTransactionBody.fileID)
}
inline ::proto::FileID* FileDeleteTransactionBody::release_fileid() {
  
  ::proto::FileID* temp = _impl_.fileid_;
  _impl_.fileid_ = nullptr;
#ifdef PROTOBUF_FORCE_COPY_IN_RELEASE
  auto* old =  reinterpret_cast<::PROTOBUF_NAMESPACE_ID::MessageLite*>(temp);
  temp = ::PROTOBUF_NAMESPACE_ID::internal::DuplicateIfNonNull(temp);
  if (GetArenaForAllocation() == nullptr) { delete old; }
#else  // PROTOBUF_FORCE_COPY_IN_RELEASE
  if (GetArenaForAllocation() != nullptr) {
    temp = ::PROTOBUF_NAMESPACE_ID::internal::DuplicateIfNonNull(temp);
  }
#endif  // !PROTOBUF_FORCE_COPY_IN_RELEASE
  return temp;
}
inline ::proto::FileID* FileDeleteTransactionBody::unsafe_arena_release_fileid() {
  // @@protoc_insertion_point(field_release:proto.FileDeleteTransactionBody.fileID)
  
  ::proto::FileID* temp = _impl_.fileid_;
  _impl_.fileid_ = nullptr;
  return temp;
}
inline ::proto::FileID* FileDeleteTransactionBody::_internal_mutable_fileid() {
  
  if (_impl_.fileid_ == nullptr) {
    auto* p = CreateMaybeMessage<::proto::FileID>(GetArenaForAllocation());
    _impl_.fileid_ = p;
  }
  return _impl_.fileid_;
}
inline ::proto::FileID* FileDeleteTransactionBody::mutable_fileid() {
  ::proto::FileID* _msg = _internal_mutable_fileid();
  // @@protoc_insertion_point(field_mutable:proto.FileDeleteTransactionBody.fileID)
  return _msg;
}
inline void FileDeleteTransactionBody::set_allocated_fileid(::proto::FileID* fileid) {
  ::PROTOBUF_NAMESPACE_ID::Arena* message_arena = GetArenaForAllocation();
  if (message_arena == nullptr) {
    delete reinterpret_cast< ::PROTOBUF_NAMESPACE_ID::MessageLite*>(_impl_.fileid_);
  }
  if (fileid) {
    ::PROTOBUF_NAMESPACE_ID::Arena* submessage_arena =
        ::PROTOBUF_NAMESPACE_ID::Arena::InternalGetOwningArena(
                reinterpret_cast<::PROTOBUF_NAMESPACE_ID::MessageLite*>(fileid));
    if (message_arena != submessage_arena) {
      fileid = ::PROTOBUF_NAMESPACE_ID::internal::GetOwnedMessage(
          message_arena, fileid, submessage_arena);
    }
    
  } else {
    
  }
  _impl_.fileid_ = fileid;
  // @@protoc_insertion_point(field_set_allocated:proto.FileDeleteTransactionBody.fileID)
}

#ifdef __GNUC__
  #pragma GCC diagnostic pop
#endif  // __GNUC__

// @@protoc_insertion_point(namespace_scope)

}  // namespace proto

// @@protoc_insertion_point(global_scope)

#include <google/protobuf/port_undef.inc>
#endif  // GOOGLE_PROTOBUF_INCLUDED_GOOGLE_PROTOBUF_INCLUDED_file_5fdelete_2eproto
