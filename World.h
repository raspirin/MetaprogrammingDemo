#pragma once
#include <functional>
#include <vector>

#include "Entity.h"

template <typename... QueryParams>
class Query;

template <typename T>
struct is_query
{
    static constexpr bool value = false;
};

template <typename... QueryParams>
struct is_query<Query<QueryParams...>>
{
    static constexpr bool value = true;
};

template <typename T>
constexpr bool is_query_v = is_query<T>::value;

template <typename S>
constexpr bool non_system_failure = false;

template <typename T>
struct system_traits
{
    static_assert(non_system_failure<T>, "Not a system");
};

template <typename R, typename... Args>
struct system_traits<R(*)(Args...)>
{
    using return_type = R;
    using args_type = std::tuple<Args...>;
};

template <typename R, typename... Args>
struct system_traits<R(&)(Args...)>
{
    using return_type = R;
    using args_type = std::tuple<Args...>;
};

template <typename T>
concept IsSystemParam = is_query_v<T>;

template <typename SP>
struct is_system_params
{
    static_assert(non_system_failure<SP>, "Invalid System param");
};

template <typename... SystemParams>
struct is_system_params<std::tuple<SystemParams...>>
{
    static constexpr bool value = (IsSystemParam<SystemParams> && ...);
};

template <typename SP>
constexpr bool is_system_params_v = is_system_params<SP>::value;

template <typename F>
concept IsSystem = std::same_as<typename system_traits<F>::return_type, void> &&
    is_system_params_v<typename system_traits<F>::args_type>;

class World
{
public:
    template <typename Component>
    bool is_with_component(Entity entity) const;

    template <typename Component>
    const Component& get_component(Entity entity) const;

    template <typename F>
        requires IsSystem<F>
    void add_system(F&& system);

    void update();

    template <typename... F>
        requires (IsSystem<F> && ...)
    void add_system(F&&... system);

private:
    std::vector<std::function<void(World&)>> systems;
};

template <typename F> requires IsSystem<F>
void World::add_system(F&& system)
{

}

template <typename... F> requires (IsSystem<F> && ...)
void World::add_system(F&&... system)
{
    ( add_system(system) , ... );
}
