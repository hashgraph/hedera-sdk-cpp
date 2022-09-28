// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: token_unfreeze_account.proto

#ifndef GOOGLE_PROTOBUF_INCLUDED_token_5funfreeze_5faccount_2eproto
#define GOOGLE_PROTOBUF_INCLUDED_token_5funfreeze_5faccount_2eproto

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
#define PROTOBUF_INTERNAL_EXPORT_token_5funfreeze_5faccount_2eproto
PROTOBUF_NAMESPACE_OPEN
namespace internal {
class AnyMetadata;
}  // namespace internal
PROTOBUF_NAMESPACE_CLOSE

// Internal implementation detail -- do not use these members.
struct TableStruct_token_5funfreeze_5faccount_2eproto {
  static const uint32_t offsets[];
};
extern const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_token_5funfreeze_5faccount_2eproto;
namespace proto {
class TokenUnfreezeAccountTransactionBody;
struct TokenUnfreezeAccountTransactionBodyDefaultTypeInternal;
extern TokenUnfreezeAccountTransactionBodyDefaultTypeInternal _TokenUnfreezeAccountTransactionBody_default_instance_;
}  // namespace proto
PROTOBUF_NAMESPACE_OPEN
template<> ::proto::TokenUnfreezeAccountTransactionBody* Arena::CreateMaybeMessage<::proto::TokenUnfreezeAccountTransactionBody>(Arena*);
PROTOBUF_NAMESPACE_CLOSE
namespace proto {

// ===================================================================

class TokenUnfreezeAccountTransactionBody final :
    public ::PROTOBUF_NAMESPACE_ID::Message /* @@protoc_insertion_point(class_definition:proto.TokenUnfreezeAccountTransactionBody) */ {
 public:
  inline TokenUnfreezeAccountTransactionBody() : TokenUnfreezeAccountTransactionBody(nullptr) {}
  ~TokenUnfreezeAccountTransactionBody() override;
  explicit PROTOBUF_CONSTEXPR TokenUnfreezeAccountTransactionBody(::PROTOBUF_NAMESPACE_ID::internal::ConstantInitialized);

  TokenUnfreezeAccountTransactionBody(const TokenUnfreezeAccountTransactionBody& from);
  TokenUnfreezeAccountTransactionBody(TokenUnfreezeAccountTransactionBody&& from) noexcept
    : TokenUnfreezeAccountTransactionBody() {
    *this = ::std::move(from);
  }

  inline TokenUnfreezeAccountTransactionBody& operator=(const TokenUnfreezeAccountTransactionBody& from) {
    CopyFrom(from);
    return *this;
  }
  inline TokenUnfreezeAccountTransactionBody& operator=(TokenUnfreezeAccountTransactionBody&& from) noexcept {
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
  static const TokenUnfreezeAccountTransactionBody& default_instance() {
    return *internal_default_instance();
  }
  static inline const TokenUnfreezeAccountTransactionBody* internal_default_instance() {
    return reinterpret_cast<const TokenUnfreezeAccountTransactionBody*>(
               &_TokenUnfreezeAccountTransactionBody_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    0;

  friend void swap(TokenUnfreezeAccountTransactionBody& a, TokenUnfreezeAccountTransactionBody& b) {
    a.Swap(&b);
  }
  inline void Swap(TokenUnfreezeAccountTransactionBody* other) {
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
  void UnsafeArenaSwap(TokenUnfreezeAccountTransactionBody* other) {
    if (other == this) return;
    GOOGLE_DCHECK(GetOwningArena() == other->GetOwningArena());
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  TokenUnfreezeAccountTransactionBody* New(::PROTOBUF_NAMESPACE_ID::Arena* arena = nullptr) const final {
    return CreateMaybeMessage<TokenUnfreezeAccountTransactionBody>(arena);
  }
  using ::PROTOBUF_NAMESPACE_ID::Message::CopyFrom;
  void CopyFrom(const TokenUnfreezeAccountTransactionBody& from);
  using ::PROTOBUF_NAMESPACE_ID::Message::MergeFrom;
  void MergeFrom( const TokenUnfreezeAccountTransactionBody& from) {
    TokenUnfreezeAccountTransactionBody::MergeImpl(*this, from);
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
  void InternalSwap(TokenUnfreezeAccountTransactionBody* other);

  private:
  friend class ::PROTOBUF_NAMESPACE_ID::internal::AnyMetadata;
  static ::PROTOBUF_NAMESPACE_ID::StringPiece FullMessageName() {
    return "proto.TokenUnfreezeAccountTransactionBody";
  }
  protected:
  explicit TokenUnfreezeAccountTransactionBody(::PROTOBUF_NAMESPACE_ID::Arena* arena,
                       bool is_message_owned = false);
  public:

  static const ClassData _class_data_;
  const ::PROTOBUF_NAMESPACE_ID::Message::ClassData*GetClassData() const final;

  ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadata() const final;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  enum : int {
    kTokenFieldNumber = 1,
    kAccountFieldNumber = 2,
  };
  // .proto.TokenID token = 1;
  bool has_token() const;
  private:
  bool _internal_has_token() const;
  public:
  void clear_token();
  const ::proto::TokenID& token() const;
  PROTOBUF_NODISCARD ::proto::TokenID* release_token();
  ::proto::TokenID* mutable_token();
  void set_allocated_token(::proto::TokenID* token);
  private:
  const ::proto::TokenID& _internal_token() const;
  ::proto::TokenID* _internal_mutable_token();
  public:
  void unsafe_arena_set_allocated_token(
      ::proto::TokenID* token);
  ::proto::TokenID* unsafe_arena_release_token();

  // .proto.AccountID account = 2;
  bool has_account() const;
  private:
  bool _internal_has_account() const;
  public:
  void clear_account();
  const ::proto::AccountID& account() const;
  PROTOBUF_NODISCARD ::proto::AccountID* release_account();
  ::proto::AccountID* mutable_account();
  void set_allocated_account(::proto::AccountID* account);
  private:
  const ::proto::AccountID& _internal_account() const;
  ::proto::AccountID* _internal_mutable_account();
  public:
  void unsafe_arena_set_allocated_account(
      ::proto::AccountID* account);
  ::proto::AccountID* unsafe_arena_release_account();

  // @@protoc_insertion_point(class_scope:proto.TokenUnfreezeAccountTransactionBody)
 private:
  class _Internal;

  template <typename T> friend class ::PROTOBUF_NAMESPACE_ID::Arena::InternalHelper;
  typedef void InternalArenaConstructable_;
  typedef void DestructorSkippable_;
  struct Impl_ {
    ::proto::TokenID* token_;
    ::proto::AccountID* account_;
    mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
  };
  union { Impl_ _impl_; };
  friend struct ::TableStruct_token_5funfreeze_5faccount_2eproto;
};
// ===================================================================


// ===================================================================

#ifdef __GNUC__
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // __GNUC__
// TokenUnfreezeAccountTransactionBody

// .proto.TokenID token = 1;
inline bool TokenUnfreezeAccountTransactionBody::_internal_has_token() const {
  return this != internal_default_instance() && _impl_.token_ != nullptr;
}
inline bool TokenUnfreezeAccountTransactionBody::has_token() const {
  return _internal_has_token();
}
inline const ::proto::TokenID& TokenUnfreezeAccountTransactionBody::_internal_token() const {
  const ::proto::TokenID* p = _impl_.token_;
  return p != nullptr ? *p : reinterpret_cast<const ::proto::TokenID&>(
      ::proto::_TokenID_default_instance_);
}
inline const ::proto::TokenID& TokenUnfreezeAccountTransactionBody::token() const {
  // @@protoc_insertion_point(field_get:proto.TokenUnfreezeAccountTransactionBody.token)
  return _internal_token();
}
inline void TokenUnfreezeAccountTransactionBody::unsafe_arena_set_allocated_token(
    ::proto::TokenID* token) {
  if (GetArenaForAllocation() == nullptr) {
    delete reinterpret_cast<::PROTOBUF_NAMESPACE_ID::MessageLite*>(_impl_.token_);
  }
  _impl_.token_ = token;
  if (token) {
    
  } else {
    
  }
  // @@protoc_insertion_point(field_unsafe_arena_set_allocated:proto.TokenUnfreezeAccountTransactionBody.token)
}
inline ::proto::TokenID* TokenUnfreezeAccountTransactionBody::release_token() {
  
  ::proto::TokenID* temp = _impl_.token_;
  _impl_.token_ = nullptr;
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
inline ::proto::TokenID* TokenUnfreezeAccountTransactionBody::unsafe_arena_release_token() {
  // @@protoc_insertion_point(field_release:proto.TokenUnfreezeAccountTransactionBody.token)
  
  ::proto::TokenID* temp = _impl_.token_;
  _impl_.token_ = nullptr;
  return temp;
}
inline ::proto::TokenID* TokenUnfreezeAccountTransactionBody::_internal_mutable_token() {
  
  if (_impl_.token_ == nullptr) {
    auto* p = CreateMaybeMessage<::proto::TokenID>(GetArenaForAllocation());
    _impl_.token_ = p;
  }
  return _impl_.token_;
}
inline ::proto::TokenID* TokenUnfreezeAccountTransactionBody::mutable_token() {
  ::proto::TokenID* _msg = _internal_mutable_token();
  // @@protoc_insertion_point(field_mutable:proto.TokenUnfreezeAccountTransactionBody.token)
  return _msg;
}
inline void TokenUnfreezeAccountTransactionBody::set_allocated_token(::proto::TokenID* token) {
  ::PROTOBUF_NAMESPACE_ID::Arena* message_arena = GetArenaForAllocation();
  if (message_arena == nullptr) {
    delete reinterpret_cast< ::PROTOBUF_NAMESPACE_ID::MessageLite*>(_impl_.token_);
  }
  if (token) {
    ::PROTOBUF_NAMESPACE_ID::Arena* submessage_arena =
        ::PROTOBUF_NAMESPACE_ID::Arena::InternalGetOwningArena(
                reinterpret_cast<::PROTOBUF_NAMESPACE_ID::MessageLite*>(token));
    if (message_arena != submessage_arena) {
      token = ::PROTOBUF_NAMESPACE_ID::internal::GetOwnedMessage(
          message_arena, token, submessage_arena);
    }
    
  } else {
    
  }
  _impl_.token_ = token;
  // @@protoc_insertion_point(field_set_allocated:proto.TokenUnfreezeAccountTransactionBody.token)
}

// .proto.AccountID account = 2;
inline bool TokenUnfreezeAccountTransactionBody::_internal_has_account() const {
  return this != internal_default_instance() && _impl_.account_ != nullptr;
}
inline bool TokenUnfreezeAccountTransactionBody::has_account() const {
  return _internal_has_account();
}
inline const ::proto::AccountID& TokenUnfreezeAccountTransactionBody::_internal_account() const {
  const ::proto::AccountID* p = _impl_.account_;
  return p != nullptr ? *p : reinterpret_cast<const ::proto::AccountID&>(
      ::proto::_AccountID_default_instance_);
}
inline const ::proto::AccountID& TokenUnfreezeAccountTransactionBody::account() const {
  // @@protoc_insertion_point(field_get:proto.TokenUnfreezeAccountTransactionBody.account)
  return _internal_account();
}
inline void TokenUnfreezeAccountTransactionBody::unsafe_arena_set_allocated_account(
    ::proto::AccountID* account) {
  if (GetArenaForAllocation() == nullptr) {
    delete reinterpret_cast<::PROTOBUF_NAMESPACE_ID::MessageLite*>(_impl_.account_);
  }
  _impl_.account_ = account;
  if (account) {
    
  } else {
    
  }
  // @@protoc_insertion_point(field_unsafe_arena_set_allocated:proto.TokenUnfreezeAccountTransactionBody.account)
}
inline ::proto::AccountID* TokenUnfreezeAccountTransactionBody::release_account() {
  
  ::proto::AccountID* temp = _impl_.account_;
  _impl_.account_ = nullptr;
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
inline ::proto::AccountID* TokenUnfreezeAccountTransactionBody::unsafe_arena_release_account() {
  // @@protoc_insertion_point(field_release:proto.TokenUnfreezeAccountTransactionBody.account)
  
  ::proto::AccountID* temp = _impl_.account_;
  _impl_.account_ = nullptr;
  return temp;
}
inline ::proto::AccountID* TokenUnfreezeAccountTransactionBody::_internal_mutable_account() {
  
  if (_impl_.account_ == nullptr) {
    auto* p = CreateMaybeMessage<::proto::AccountID>(GetArenaForAllocation());
    _impl_.account_ = p;
  }
  return _impl_.account_;
}
inline ::proto::AccountID* TokenUnfreezeAccountTransactionBody::mutable_account() {
  ::proto::AccountID* _msg = _internal_mutable_account();
  // @@protoc_insertion_point(field_mutable:proto.TokenUnfreezeAccountTransactionBody.account)
  return _msg;
}
inline void TokenUnfreezeAccountTransactionBody::set_allocated_account(::proto::AccountID* account) {
  ::PROTOBUF_NAMESPACE_ID::Arena* message_arena = GetArenaForAllocation();
  if (message_arena == nullptr) {
    delete reinterpret_cast< ::PROTOBUF_NAMESPACE_ID::MessageLite*>(_impl_.account_);
  }
  if (account) {
    ::PROTOBUF_NAMESPACE_ID::Arena* submessage_arena =
        ::PROTOBUF_NAMESPACE_ID::Arena::InternalGetOwningArena(
                reinterpret_cast<::PROTOBUF_NAMESPACE_ID::MessageLite*>(account));
    if (message_arena != submessage_arena) {
      account = ::PROTOBUF_NAMESPACE_ID::internal::GetOwnedMessage(
          message_arena, account, submessage_arena);
    }
    
  } else {
    
  }
  _impl_.account_ = account;
  // @@protoc_insertion_point(field_set_allocated:proto.TokenUnfreezeAccountTransactionBody.account)
}

#ifdef __GNUC__
  #pragma GCC diagnostic pop
#endif  // __GNUC__

// @@protoc_insertion_point(namespace_scope)

}  // namespace proto

// @@protoc_insertion_point(global_scope)

#include <google/protobuf/port_undef.inc>
#endif  // GOOGLE_PROTOBUF_INCLUDED_GOOGLE_PROTOBUF_INCLUDED_token_5funfreeze_5faccount_2eproto
