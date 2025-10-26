#include "Query.h"
#include "World.h"

template <typename ... Components>
bool TupleQueryFilter<Components...>::is_match(Entity entity, const World& world)
{
    return (world.is_with_component<Components>(entity) && ...);
}

template <typename ... Components, IsQueryFilter QueryFilter>
std::tuple<const Components&...> Query<std::tuple<Components...>, QueryFilter>::Iterator::unpack(Entity entity)
{
    return std::tuple<const Components&...>{world.get_component<std::decay_t<Components>>()...};
}

