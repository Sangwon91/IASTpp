#pragma once

template<typename ValueType>
inline
Any::AnyData<ValueType>::AnyData(const ValueType& value) : mValue {value}
    {
    }

template <typename ValueType>
inline
const std::type_info&
Any::AnyData<ValueType>::getType() const
    {
    return typeid(mValue);
    }

template <typename ValueType>
inline
std::unique_ptr<Any::DataType>
Any::AnyData<ValueType>::clone() const
    {
    return std::unique_ptr<AnyData<ValueType>> {new AnyData {mValue}};
    }

template <typename ValueType>
inline
ValueType&
Any::AnyData<ValueType>::getValue()
    {
    return mValue;
    }

inline
Any::Any() : mData {nullptr}
    {
    }

inline
Any::Any(const Any& other) :
    mData {other.isEmpty() ? nullptr : other.mData->clone()}
    {
    }

inline
Any&
Any::operator = (const Any& other)
    {
    Any {other}.swap(*this);
    return *this;
    }

inline
Any&
Any::operator = (Any&& other)
    {
    other.swap(*this);
    Any {}.swap(other);
    return *this;
    }

#define PURE(T) typename std::remove_cv< typename std::decay<T>::type >::type

template <typename T>
inline
Any::Any(const T& value) :
    mData {std::unique_ptr< AnyData<PURE(T)> >{new AnyData<PURE(T)> {value}}}
    {
    }

#undef PURE

inline
Any::~Any()
    {
    }

inline
Any&
Any::swap(Any& other)
    {
    std::swap(mData, other.mData);
    return *this;
    }

template <typename T>
inline
Any& Any::operator = (T&& value)
    {
    Any {std::forward<T>(value)}.swap(*this);
    return *this;
    }

inline
const std::type_info&
Any::getType() const
    {
    return this->isEmpty() ? typeid(void) : mData->getType();
    }

inline
bool
Any::isEmpty() const
    {
    return mData == nullptr;
    }

template <typename T>
inline
T* Any::getAsHelper() const
    {
    return this->isEmpty() or this->getType() != typeid(T) ?
        nullptr :
        &static_cast<
            AnyData<typename std::remove_cv<T>::type>* >(mData.get())->getValue();
    }

template <typename T>
inline
T Any::getAs() const
    {
    using NonRef = typename std::remove_reference<T>::type;

    NonRef* result = getAsHelper<NonRef>();

    if (result == nullptr)
        throw AnyException {};

    // To prevent temperary class.
    using RefType = typename std::add_lvalue_reference<T>::type;

    return static_cast<RefType>(*result);
    }

inline
const char*
AnyException::what() const noexcept
    {
    return "Any::getAs() fails.";
    }
