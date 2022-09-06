// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: throttle_definitions.proto

#ifndef GOOGLE_PROTOBUF_INCLUDED_throttle_5fdefinitions_2eproto
#define GOOGLE_PROTOBUF_INCLUDED_throttle_5fdefinitions_2eproto

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
#define PROTOBUF_INTERNAL_EXPORT_throttle_5fdefinitions_2eproto
PROTOBUF_NAMESPACE_OPEN
namespace internal {
class AnyMetadata;
}  // namespace internal
PROTOBUF_NAMESPACE_CLOSE

// Internal implementation detail -- do not use these members.
struct TableStruct_throttle_5fdefinitions_2eproto {
  static const uint32_t offsets[];
};
extern const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_throttle_5fdefinitions_2eproto;
namespace proto {
class ThrottleBucket;
struct ThrottleBucketDefaultTypeInternal;
extern ThrottleBucketDefaultTypeInternal _ThrottleBucket_default_instance_;
class ThrottleDefinitions;
struct ThrottleDefinitionsDefaultTypeInternal;
extern ThrottleDefinitionsDefaultTypeInternal _ThrottleDefinitions_default_instance_;
class ThrottleGroup;
struct ThrottleGroupDefaultTypeInternal;
extern ThrottleGroupDefaultTypeInternal _ThrottleGroup_default_instance_;
}  // namespace proto
PROTOBUF_NAMESPACE_OPEN
template<> ::proto::ThrottleBucket* Arena::CreateMaybeMessage<::proto::ThrottleBucket>(Arena*);
template<> ::proto::ThrottleDefinitions* Arena::CreateMaybeMessage<::proto::ThrottleDefinitions>(Arena*);
template<> ::proto::ThrottleGroup* Arena::CreateMaybeMessage<::proto::ThrottleGroup>(Arena*);
PROTOBUF_NAMESPACE_CLOSE
namespace proto {

// ===================================================================

class ThrottleGroup final :
    public ::PROTOBUF_NAMESPACE_ID::Message /* @@protoc_insertion_point(class_definition:proto.ThrottleGroup) */ {
 public:
  inline ThrottleGroup() : ThrottleGroup(nullptr) {}
  ~ThrottleGroup() override;
  explicit PROTOBUF_CONSTEXPR ThrottleGroup(::PROTOBUF_NAMESPACE_ID::internal::ConstantInitialized);

  ThrottleGroup(const ThrottleGroup& from);
  ThrottleGroup(ThrottleGroup&& from) noexcept
    : ThrottleGroup() {
    *this = ::std::move(from);
  }

  inline ThrottleGroup& operator=(const ThrottleGroup& from) {
    CopyFrom(from);
    return *this;
  }
  inline ThrottleGroup& operator=(ThrottleGroup&& from) noexcept {
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
  static const ThrottleGroup& default_instance() {
    return *internal_default_instance();
  }
  static inline const ThrottleGroup* internal_default_instance() {
    return reinterpret_cast<const ThrottleGroup*>(
               &_ThrottleGroup_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    0;

  friend void swap(ThrottleGroup& a, ThrottleGroup& b) {
    a.Swap(&b);
  }
  inline void Swap(ThrottleGroup* other) {
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
  void UnsafeArenaSwap(ThrottleGroup* other) {
    if (other == this) return;
    GOOGLE_DCHECK(GetOwningArena() == other->GetOwningArena());
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  ThrottleGroup* New(::PROTOBUF_NAMESPACE_ID::Arena* arena = nullptr) const final {
    return CreateMaybeMessage<ThrottleGroup>(arena);
  }
  using ::PROTOBUF_NAMESPACE_ID::Message::CopyFrom;
  void CopyFrom(const ThrottleGroup& from);
  using ::PROTOBUF_NAMESPACE_ID::Message::MergeFrom;
  void MergeFrom( const ThrottleGroup& from) {
    ThrottleGroup::MergeImpl(*this, from);
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
  void InternalSwap(ThrottleGroup* other);

  private:
  friend class ::PROTOBUF_NAMESPACE_ID::internal::AnyMetadata;
  static ::PROTOBUF_NAMESPACE_ID::StringPiece FullMessageName() {
    return "proto.ThrottleGroup";
  }
  protected:
  explicit ThrottleGroup(::PROTOBUF_NAMESPACE_ID::Arena* arena,
                       bool is_message_owned = false);
  public:

  static const ClassData _class_data_;
  const ::PROTOBUF_NAMESPACE_ID::Message::ClassData*GetClassData() const final;

  ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadata() const final;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  enum : int {
    kOperationsFieldNumber = 1,
    kMilliOpsPerSecFieldNumber = 2,
  };
  // repeated .proto.HederaFunctionality operations = 1;
  int operations_size() const;
  private:
  int _internal_operations_size() const;
  public:
  void clear_operations();
  private:
  ::proto::HederaFunctionality _internal_operations(int index) const;
  void _internal_add_operations(::proto::HederaFunctionality value);
  ::PROTOBUF_NAMESPACE_ID::RepeatedField<int>* _internal_mutable_operations();
  public:
  ::proto::HederaFunctionality operations(int index) const;
  void set_operations(int index, ::proto::HederaFunctionality value);
  void add_operations(::proto::HederaFunctionality value);
  const ::PROTOBUF_NAMESPACE_ID::RepeatedField<int>& operations() const;
  ::PROTOBUF_NAMESPACE_ID::RepeatedField<int>* mutable_operations();

  // uint64 milliOpsPerSec = 2;
  void clear_milliopspersec();
  uint64_t milliopspersec() const;
  void set_milliopspersec(uint64_t value);
  private:
  uint64_t _internal_milliopspersec() const;
  void _internal_set_milliopspersec(uint64_t value);
  public:

  // @@protoc_insertion_point(class_scope:proto.ThrottleGroup)
 private:
  class _Internal;

  template <typename T> friend class ::PROTOBUF_NAMESPACE_ID::Arena::InternalHelper;
  typedef void InternalArenaConstructable_;
  typedef void DestructorSkippable_;
  struct Impl_ {
    ::PROTOBUF_NAMESPACE_ID::RepeatedField<int> operations_;
    mutable std::atomic<int> _operations_cached_byte_size_;
    uint64_t milliopspersec_;
    mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
  };
  union { Impl_ _impl_; };
  friend struct ::TableStruct_throttle_5fdefinitions_2eproto;
};
// -------------------------------------------------------------------

class ThrottleBucket final :
    public ::PROTOBUF_NAMESPACE_ID::Message /* @@protoc_insertion_point(class_definition:proto.ThrottleBucket) */ {
 public:
  inline ThrottleBucket() : ThrottleBucket(nullptr) {}
  ~ThrottleBucket() override;
  explicit PROTOBUF_CONSTEXPR ThrottleBucket(::PROTOBUF_NAMESPACE_ID::internal::ConstantInitialized);

  ThrottleBucket(const ThrottleBucket& from);
  ThrottleBucket(ThrottleBucket&& from) noexcept
    : ThrottleBucket() {
    *this = ::std::move(from);
  }

  inline ThrottleBucket& operator=(const ThrottleBucket& from) {
    CopyFrom(from);
    return *this;
  }
  inline ThrottleBucket& operator=(ThrottleBucket&& from) noexcept {
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
  static const ThrottleBucket& default_instance() {
    return *internal_default_instance();
  }
  static inline const ThrottleBucket* internal_default_instance() {
    return reinterpret_cast<const ThrottleBucket*>(
               &_ThrottleBucket_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    1;

  friend void swap(ThrottleBucket& a, ThrottleBucket& b) {
    a.Swap(&b);
  }
  inline void Swap(ThrottleBucket* other) {
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
  void UnsafeArenaSwap(ThrottleBucket* other) {
    if (other == this) return;
    GOOGLE_DCHECK(GetOwningArena() == other->GetOwningArena());
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  ThrottleBucket* New(::PROTOBUF_NAMESPACE_ID::Arena* arena = nullptr) const final {
    return CreateMaybeMessage<ThrottleBucket>(arena);
  }
  using ::PROTOBUF_NAMESPACE_ID::Message::CopyFrom;
  void CopyFrom(const ThrottleBucket& from);
  using ::PROTOBUF_NAMESPACE_ID::Message::MergeFrom;
  void MergeFrom( const ThrottleBucket& from) {
    ThrottleBucket::MergeImpl(*this, from);
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
  void InternalSwap(ThrottleBucket* other);

  private:
  friend class ::PROTOBUF_NAMESPACE_ID::internal::AnyMetadata;
  static ::PROTOBUF_NAMESPACE_ID::StringPiece FullMessageName() {
    return "proto.ThrottleBucket";
  }
  protected:
  explicit ThrottleBucket(::PROTOBUF_NAMESPACE_ID::Arena* arena,
                       bool is_message_owned = false);
  public:

  static const ClassData _class_data_;
  const ::PROTOBUF_NAMESPACE_ID::Message::ClassData*GetClassData() const final;

  ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadata() const final;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  enum : int {
    kThrottleGroupsFieldNumber = 3,
    kNameFieldNumber = 1,
    kBurstPeriodMsFieldNumber = 2,
  };
  // repeated .proto.ThrottleGroup throttleGroups = 3;
  int throttlegroups_size() const;
  private:
  int _internal_throttlegroups_size() const;
  public:
  void clear_throttlegroups();
  ::proto::ThrottleGroup* mutable_throttlegroups(int index);
  ::PROTOBUF_NAMESPACE_ID::RepeatedPtrField< ::proto::ThrottleGroup >*
      mutable_throttlegroups();
  private:
  const ::proto::ThrottleGroup& _internal_throttlegroups(int index) const;
  ::proto::ThrottleGroup* _internal_add_throttlegroups();
  public:
  const ::proto::ThrottleGroup& throttlegroups(int index) const;
  ::proto::ThrottleGroup* add_throttlegroups();
  const ::PROTOBUF_NAMESPACE_ID::RepeatedPtrField< ::proto::ThrottleGroup >&
      throttlegroups() const;

  // string name = 1;
  void clear_name();
  const std::string& name() const;
  template <typename ArgT0 = const std::string&, typename... ArgT>
  void set_name(ArgT0&& arg0, ArgT... args);
  std::string* mutable_name();
  PROTOBUF_NODISCARD std::string* release_name();
  void set_allocated_name(std::string* name);
  private:
  const std::string& _internal_name() const;
  inline PROTOBUF_ALWAYS_INLINE void _internal_set_name(const std::string& value);
  std::string* _internal_mutable_name();
  public:

  // uint64 burstPeriodMs = 2;
  void clear_burstperiodms();
  uint64_t burstperiodms() const;
  void set_burstperiodms(uint64_t value);
  private:
  uint64_t _internal_burstperiodms() const;
  void _internal_set_burstperiodms(uint64_t value);
  public:

  // @@protoc_insertion_point(class_scope:proto.ThrottleBucket)
 private:
  class _Internal;

  template <typename T> friend class ::PROTOBUF_NAMESPACE_ID::Arena::InternalHelper;
  typedef void InternalArenaConstructable_;
  typedef void DestructorSkippable_;
  struct Impl_ {
    ::PROTOBUF_NAMESPACE_ID::RepeatedPtrField< ::proto::ThrottleGroup > throttlegroups_;
    ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr name_;
    uint64_t burstperiodms_;
    mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
  };
  union { Impl_ _impl_; };
  friend struct ::TableStruct_throttle_5fdefinitions_2eproto;
};
// -------------------------------------------------------------------

class ThrottleDefinitions final :
    public ::PROTOBUF_NAMESPACE_ID::Message /* @@protoc_insertion_point(class_definition:proto.ThrottleDefinitions) */ {
 public:
  inline ThrottleDefinitions() : ThrottleDefinitions(nullptr) {}
  ~ThrottleDefinitions() override;
  explicit PROTOBUF_CONSTEXPR ThrottleDefinitions(::PROTOBUF_NAMESPACE_ID::internal::ConstantInitialized);

  ThrottleDefinitions(const ThrottleDefinitions& from);
  ThrottleDefinitions(ThrottleDefinitions&& from) noexcept
    : ThrottleDefinitions() {
    *this = ::std::move(from);
  }

  inline ThrottleDefinitions& operator=(const ThrottleDefinitions& from) {
    CopyFrom(from);
    return *this;
  }
  inline ThrottleDefinitions& operator=(ThrottleDefinitions&& from) noexcept {
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
  static const ThrottleDefinitions& default_instance() {
    return *internal_default_instance();
  }
  static inline const ThrottleDefinitions* internal_default_instance() {
    return reinterpret_cast<const ThrottleDefinitions*>(
               &_ThrottleDefinitions_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    2;

  friend void swap(ThrottleDefinitions& a, ThrottleDefinitions& b) {
    a.Swap(&b);
  }
  inline void Swap(ThrottleDefinitions* other) {
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
  void UnsafeArenaSwap(ThrottleDefinitions* other) {
    if (other == this) return;
    GOOGLE_DCHECK(GetOwningArena() == other->GetOwningArena());
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  ThrottleDefinitions* New(::PROTOBUF_NAMESPACE_ID::Arena* arena = nullptr) const final {
    return CreateMaybeMessage<ThrottleDefinitions>(arena);
  }
  using ::PROTOBUF_NAMESPACE_ID::Message::CopyFrom;
  void CopyFrom(const ThrottleDefinitions& from);
  using ::PROTOBUF_NAMESPACE_ID::Message::MergeFrom;
  void MergeFrom( const ThrottleDefinitions& from) {
    ThrottleDefinitions::MergeImpl(*this, from);
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
  void InternalSwap(ThrottleDefinitions* other);

  private:
  friend class ::PROTOBUF_NAMESPACE_ID::internal::AnyMetadata;
  static ::PROTOBUF_NAMESPACE_ID::StringPiece FullMessageName() {
    return "proto.ThrottleDefinitions";
  }
  protected:
  explicit ThrottleDefinitions(::PROTOBUF_NAMESPACE_ID::Arena* arena,
                       bool is_message_owned = false);
  public:

  static const ClassData _class_data_;
  const ::PROTOBUF_NAMESPACE_ID::Message::ClassData*GetClassData() const final;

  ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadata() const final;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  enum : int {
    kThrottleBucketsFieldNumber = 1,
  };
  // repeated .proto.ThrottleBucket throttleBuckets = 1;
  int throttlebuckets_size() const;
  private:
  int _internal_throttlebuckets_size() const;
  public:
  void clear_throttlebuckets();
  ::proto::ThrottleBucket* mutable_throttlebuckets(int index);
  ::PROTOBUF_NAMESPACE_ID::RepeatedPtrField< ::proto::ThrottleBucket >*
      mutable_throttlebuckets();
  private:
  const ::proto::ThrottleBucket& _internal_throttlebuckets(int index) const;
  ::proto::ThrottleBucket* _internal_add_throttlebuckets();
  public:
  const ::proto::ThrottleBucket& throttlebuckets(int index) const;
  ::proto::ThrottleBucket* add_throttlebuckets();
  const ::PROTOBUF_NAMESPACE_ID::RepeatedPtrField< ::proto::ThrottleBucket >&
      throttlebuckets() const;

  // @@protoc_insertion_point(class_scope:proto.ThrottleDefinitions)
 private:
  class _Internal;

  template <typename T> friend class ::PROTOBUF_NAMESPACE_ID::Arena::InternalHelper;
  typedef void InternalArenaConstructable_;
  typedef void DestructorSkippable_;
  struct Impl_ {
    ::PROTOBUF_NAMESPACE_ID::RepeatedPtrField< ::proto::ThrottleBucket > throttlebuckets_;
    mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
  };
  union { Impl_ _impl_; };
  friend struct ::TableStruct_throttle_5fdefinitions_2eproto;
};
// ===================================================================


// ===================================================================

#ifdef __GNUC__
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // __GNUC__
// ThrottleGroup

// repeated .proto.HederaFunctionality operations = 1;
inline int ThrottleGroup::_internal_operations_size() const {
  return _impl_.operations_.size();
}
inline int ThrottleGroup::operations_size() const {
  return _internal_operations_size();
}
inline void ThrottleGroup::clear_operations() {
  _impl_.operations_.Clear();
}
inline ::proto::HederaFunctionality ThrottleGroup::_internal_operations(int index) const {
  return static_cast< ::proto::HederaFunctionality >(_impl_.operations_.Get(index));
}
inline ::proto::HederaFunctionality ThrottleGroup::operations(int index) const {
  // @@protoc_insertion_point(field_get:proto.ThrottleGroup.operations)
  return _internal_operations(index);
}
inline void ThrottleGroup::set_operations(int index, ::proto::HederaFunctionality value) {
  _impl_.operations_.Set(index, value);
  // @@protoc_insertion_point(field_set:proto.ThrottleGroup.operations)
}
inline void ThrottleGroup::_internal_add_operations(::proto::HederaFunctionality value) {
  _impl_.operations_.Add(value);
}
inline void ThrottleGroup::add_operations(::proto::HederaFunctionality value) {
  _internal_add_operations(value);
  // @@protoc_insertion_point(field_add:proto.ThrottleGroup.operations)
}
inline const ::PROTOBUF_NAMESPACE_ID::RepeatedField<int>&
ThrottleGroup::operations() const {
  // @@protoc_insertion_point(field_list:proto.ThrottleGroup.operations)
  return _impl_.operations_;
}
inline ::PROTOBUF_NAMESPACE_ID::RepeatedField<int>*
ThrottleGroup::_internal_mutable_operations() {
  return &_impl_.operations_;
}
inline ::PROTOBUF_NAMESPACE_ID::RepeatedField<int>*
ThrottleGroup::mutable_operations() {
  // @@protoc_insertion_point(field_mutable_list:proto.ThrottleGroup.operations)
  return _internal_mutable_operations();
}

// uint64 milliOpsPerSec = 2;
inline void ThrottleGroup::clear_milliopspersec() {
  _impl_.milliopspersec_ = uint64_t{0u};
}
inline uint64_t ThrottleGroup::_internal_milliopspersec() const {
  return _impl_.milliopspersec_;
}
inline uint64_t ThrottleGroup::milliopspersec() const {
  // @@protoc_insertion_point(field_get:proto.ThrottleGroup.milliOpsPerSec)
  return _internal_milliopspersec();
}
inline void ThrottleGroup::_internal_set_milliopspersec(uint64_t value) {
  
  _impl_.milliopspersec_ = value;
}
inline void ThrottleGroup::set_milliopspersec(uint64_t value) {
  _internal_set_milliopspersec(value);
  // @@protoc_insertion_point(field_set:proto.ThrottleGroup.milliOpsPerSec)
}

// -------------------------------------------------------------------

// ThrottleBucket

// string name = 1;
inline void ThrottleBucket::clear_name() {
  _impl_.name_.ClearToEmpty();
}
inline const std::string& ThrottleBucket::name() const {
  // @@protoc_insertion_point(field_get:proto.ThrottleBucket.name)
  return _internal_name();
}
template <typename ArgT0, typename... ArgT>
inline PROTOBUF_ALWAYS_INLINE
void ThrottleBucket::set_name(ArgT0&& arg0, ArgT... args) {
 
 _impl_.name_.Set(static_cast<ArgT0 &&>(arg0), args..., GetArenaForAllocation());
  // @@protoc_insertion_point(field_set:proto.ThrottleBucket.name)
}
inline std::string* ThrottleBucket::mutable_name() {
  std::string* _s = _internal_mutable_name();
  // @@protoc_insertion_point(field_mutable:proto.ThrottleBucket.name)
  return _s;
}
inline const std::string& ThrottleBucket::_internal_name() const {
  return _impl_.name_.Get();
}
inline void ThrottleBucket::_internal_set_name(const std::string& value) {
  
  _impl_.name_.Set(value, GetArenaForAllocation());
}
inline std::string* ThrottleBucket::_internal_mutable_name() {
  
  return _impl_.name_.Mutable(GetArenaForAllocation());
}
inline std::string* ThrottleBucket::release_name() {
  // @@protoc_insertion_point(field_release:proto.ThrottleBucket.name)
  return _impl_.name_.Release();
}
inline void ThrottleBucket::set_allocated_name(std::string* name) {
  if (name != nullptr) {
    
  } else {
    
  }
  _impl_.name_.SetAllocated(name, GetArenaForAllocation());
#ifdef PROTOBUF_FORCE_COPY_DEFAULT_STRING
  if (_impl_.name_.IsDefault()) {
    _impl_.name_.Set("", GetArenaForAllocation());
  }
#endif // PROTOBUF_FORCE_COPY_DEFAULT_STRING
  // @@protoc_insertion_point(field_set_allocated:proto.ThrottleBucket.name)
}

// uint64 burstPeriodMs = 2;
inline void ThrottleBucket::clear_burstperiodms() {
  _impl_.burstperiodms_ = uint64_t{0u};
}
inline uint64_t ThrottleBucket::_internal_burstperiodms() const {
  return _impl_.burstperiodms_;
}
inline uint64_t ThrottleBucket::burstperiodms() const {
  // @@protoc_insertion_point(field_get:proto.ThrottleBucket.burstPeriodMs)
  return _internal_burstperiodms();
}
inline void ThrottleBucket::_internal_set_burstperiodms(uint64_t value) {
  
  _impl_.burstperiodms_ = value;
}
inline void ThrottleBucket::set_burstperiodms(uint64_t value) {
  _internal_set_burstperiodms(value);
  // @@protoc_insertion_point(field_set:proto.ThrottleBucket.burstPeriodMs)
}

// repeated .proto.ThrottleGroup throttleGroups = 3;
inline int ThrottleBucket::_internal_throttlegroups_size() const {
  return _impl_.throttlegroups_.size();
}
inline int ThrottleBucket::throttlegroups_size() const {
  return _internal_throttlegroups_size();
}
inline void ThrottleBucket::clear_throttlegroups() {
  _impl_.throttlegroups_.Clear();
}
inline ::proto::ThrottleGroup* ThrottleBucket::mutable_throttlegroups(int index) {
  // @@protoc_insertion_point(field_mutable:proto.ThrottleBucket.throttleGroups)
  return _impl_.throttlegroups_.Mutable(index);
}
inline ::PROTOBUF_NAMESPACE_ID::RepeatedPtrField< ::proto::ThrottleGroup >*
ThrottleBucket::mutable_throttlegroups() {
  // @@protoc_insertion_point(field_mutable_list:proto.ThrottleBucket.throttleGroups)
  return &_impl_.throttlegroups_;
}
inline const ::proto::ThrottleGroup& ThrottleBucket::_internal_throttlegroups(int index) const {
  return _impl_.throttlegroups_.Get(index);
}
inline const ::proto::ThrottleGroup& ThrottleBucket::throttlegroups(int index) const {
  // @@protoc_insertion_point(field_get:proto.ThrottleBucket.throttleGroups)
  return _internal_throttlegroups(index);
}
inline ::proto::ThrottleGroup* ThrottleBucket::_internal_add_throttlegroups() {
  return _impl_.throttlegroups_.Add();
}
inline ::proto::ThrottleGroup* ThrottleBucket::add_throttlegroups() {
  ::proto::ThrottleGroup* _add = _internal_add_throttlegroups();
  // @@protoc_insertion_point(field_add:proto.ThrottleBucket.throttleGroups)
  return _add;
}
inline const ::PROTOBUF_NAMESPACE_ID::RepeatedPtrField< ::proto::ThrottleGroup >&
ThrottleBucket::throttlegroups() const {
  // @@protoc_insertion_point(field_list:proto.ThrottleBucket.throttleGroups)
  return _impl_.throttlegroups_;
}

// -------------------------------------------------------------------

// ThrottleDefinitions

// repeated .proto.ThrottleBucket throttleBuckets = 1;
inline int ThrottleDefinitions::_internal_throttlebuckets_size() const {
  return _impl_.throttlebuckets_.size();
}
inline int ThrottleDefinitions::throttlebuckets_size() const {
  return _internal_throttlebuckets_size();
}
inline void ThrottleDefinitions::clear_throttlebuckets() {
  _impl_.throttlebuckets_.Clear();
}
inline ::proto::ThrottleBucket* ThrottleDefinitions::mutable_throttlebuckets(int index) {
  // @@protoc_insertion_point(field_mutable:proto.ThrottleDefinitions.throttleBuckets)
  return _impl_.throttlebuckets_.Mutable(index);
}
inline ::PROTOBUF_NAMESPACE_ID::RepeatedPtrField< ::proto::ThrottleBucket >*
ThrottleDefinitions::mutable_throttlebuckets() {
  // @@protoc_insertion_point(field_mutable_list:proto.ThrottleDefinitions.throttleBuckets)
  return &_impl_.throttlebuckets_;
}
inline const ::proto::ThrottleBucket& ThrottleDefinitions::_internal_throttlebuckets(int index) const {
  return _impl_.throttlebuckets_.Get(index);
}
inline const ::proto::ThrottleBucket& ThrottleDefinitions::throttlebuckets(int index) const {
  // @@protoc_insertion_point(field_get:proto.ThrottleDefinitions.throttleBuckets)
  return _internal_throttlebuckets(index);
}
inline ::proto::ThrottleBucket* ThrottleDefinitions::_internal_add_throttlebuckets() {
  return _impl_.throttlebuckets_.Add();
}
inline ::proto::ThrottleBucket* ThrottleDefinitions::add_throttlebuckets() {
  ::proto::ThrottleBucket* _add = _internal_add_throttlebuckets();
  // @@protoc_insertion_point(field_add:proto.ThrottleDefinitions.throttleBuckets)
  return _add;
}
inline const ::PROTOBUF_NAMESPACE_ID::RepeatedPtrField< ::proto::ThrottleBucket >&
ThrottleDefinitions::throttlebuckets() const {
  // @@protoc_insertion_point(field_list:proto.ThrottleDefinitions.throttleBuckets)
  return _impl_.throttlebuckets_;
}

#ifdef __GNUC__
  #pragma GCC diagnostic pop
#endif  // __GNUC__
// -------------------------------------------------------------------

// -------------------------------------------------------------------


// @@protoc_insertion_point(namespace_scope)

}  // namespace proto

// @@protoc_insertion_point(global_scope)

#include <google/protobuf/port_undef.inc>
#endif  // GOOGLE_PROTOBUF_INCLUDED_GOOGLE_PROTOBUF_INCLUDED_throttle_5fdefinitions_2eproto
