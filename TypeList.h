#pragma once

struct NullType;

template <typename T, typename U>
struct TypeList
{
    using Head = T;
    using Tail = U;
};

template <typename TList>
struct Length;

template <>
struct Length<NullType>
{
    enum { value = 0 };
};

template <typename T, typename U>
struct Length<TypeList<T, U>>
{
    enum { value = 1 + Length<U>::value };
};

template <typename TList, unsigned int index>
struct TypeAt;

template <typename Head, typename Tail>
struct TypeAt<TypeList<Head, Tail>, 0>
{
    using Result = Head;
};

template <typename Head, typename Tail, unsigned int i>
struct TypeAt<TypeList<Head, Tail>, i>
{
    // using Result = typename TypeAt<Tail, i - 1>::Result;
    typedef typename TypeAt<Tail, i - 1>::Result Result;
};

template <typename TList, typename T>
struct IndexOf;

template <typename T>
struct IndexOf<NullType, T>
{
    enum { value = -1 };
};

template <typename Head, typename Tail>
struct IndexOf<TypeList<Head, Tail>, Head>
{
    enum { value = 0 };
};

template <typename Head, typename Tail, typename T>
struct IndexOf<TypeList<Head, Tail>, T>
{
private:
    enum { tmp = IndexOf<Tail, T>::value };
public:
    enum { value = tmp == -1 ? -1 : 1 + tmp };
};

template <typename TList, template <typename> class Unit>
class GenScatterHierarchy;

template <typename T1, typename T2, template <typename> class Unit>
class GenScatterHierarchy<TypeList<T1, T2>, Unit>
    : public GenScatterHierarchy<T1, Unit>
    , public GenScatterHierarchy<T2, Unit>
{
public:
    typedef TypeList<T1, T2> TList;
    typedef GenScatterHierarchy<T1, Unit> LeftBase;
    typedef GenScatterHierarchy<T2, Unit> RightBase;
};

template <typename AtomicType, template <typename> class Unit>
class GenScatterHierarchy: public Unit<AtomicType>
{
    typedef Unit<AtomicType> LeftBase;
};

template <template <typename> class Unit>
class GenScatterHierarchy<NullType, Unit>
{
};

template <typename T>
struct Holder
{
    T value_;
};