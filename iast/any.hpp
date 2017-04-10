#pragma once

#include <type_traits>
#include <typeinfo>
#include <memory>
#include <exception>
#include <ciso646>

class Any
    {
public:
    inline Any();
    inline Any(const Any& other);
    inline Any& operator = (const Any& other);
    inline Any& operator = (Any&& other);
    template <typename T> inline Any(const T& value);
    inline ~Any();

    inline Any& swap(Any& other);

    template <typename T> inline Any& operator = (T&& value);

    inline const std::type_info& getType() const;
    inline bool isEmpty() const;

    template <typename T> inline T* getAsHelper() const;
    template <typename T> inline T getAs() const;
private:
    class DataType;
    template <typename T> class AnyData;

    std::unique_ptr<DataType> mData;
    };

class Any::DataType
    {
public:
    virtual ~DataType() = default;

    virtual const std::type_info& getType() const = 0;
    virtual std::unique_ptr<DataType> clone() const = 0;
    };

template <typename ValueType>
class Any::AnyData : public DataType
    {
public:
    inline AnyData(const ValueType& value);
    virtual inline const std::type_info& getType() const override;
    virtual inline std::unique_ptr<DataType> clone() const override;
    inline ValueType& getValue();
private:
    ValueType mValue;
    };

class AnyException : std::exception
    {
public:
    virtual inline const char* what() const noexcept;
    };

#include "any_impl.hpp"
