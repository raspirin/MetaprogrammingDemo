#pragma once
#include <tuple>

#include "Entity.h"

class World;

template <typename...>
constexpr bool dependent_false = false;

class AbstractComponent {};

template <typename T>
concept IsComponent = std::is_base_of_v<AbstractComponent, T>;

class AbstractQueryFilter
{
protected:
    virtual bool is_match(Entity entity, const World& world) = 0;

public:
    virtual bool check(Entity entity, const World& world) = 0;
};

class TrueQueryFilter: public AbstractQueryFilter
{
protected:
    constexpr bool is_match(Entity entity, const World& world) override { return true; }

public:
    constexpr bool check(Entity entity, const World& world) override { return true; }
};

template <typename... Components>
class TupleQueryFilter: public AbstractQueryFilter
{
protected:
    bool is_match(Entity entity, const World& world) override;
};

template <typename... Components>
class With: public TupleQueryFilter<Components...>
{
public:
    bool check(Entity entity, const World& world) override
    {
        return this->is_match(entity, world);
    }
};

template <typename... Components>
class Without: public TupleQueryFilter<Components...>
{
public:
    bool check(Entity entity, const World& world) override
    {
        return !this->is_match(entity, world);
    }
};

template <typename Derived>
concept IsQueryFilter = std::is_base_of_v<AbstractQueryFilter, Derived>;

template <typename... QueryParams>
class Query
{
    static_assert(dependent_false<QueryParams...>, "You should use Query with `std::tuple` and one optional `With` or `Without` filter");
};

template <typename... Components, IsQueryFilter QueryFilter>
class Query<std::tuple<Components...>, QueryFilter>
{
public:
    using CType = std::tuple<Components...>;
    using FType = QueryFilter;

    template<std::size_t N>
    using CTypeAt = std::tuple_element_t<N, CType>;

    static constexpr size_t query_size_v = std::tuple_size_v<std::tuple<Components...>>;

    class Iterator
    {
    private:
        std::tuple<const Components&...> unpack(Entity entity);

    private:
        const World& world;
    };
};

template <typename... Components>
class Query<std::tuple<Components...>>: public Query<std::tuple<Components...>, TrueQueryFilter> {};

template <IsComponent Component, IsQueryFilter QueryFilter>
class Query<Component, QueryFilter>: public Query<std::tuple<Component>, QueryFilter> {};

template <IsComponent Component>
class Query<Component>: public Query<std::tuple<Component>> {};

