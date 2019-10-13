#include "category.h"

void Category::addCategory(const std::string &category)
{
    categories_.emplace(category);
}

bool Category::categoryPresent(const std::string &category) const
{
    return categories_.find(category) != std::end(categories_);
}

void Category::removeCategory(const std::string &category)
{
    categories_.erase(categories_.find(category));
}
