#ifndef CATEGORY_H
#define CATEGORY_H

#include <unordered_set>
#include <string>
#include <optional>

class Category
{
public:

    void addCategory(const std::string& category);
    bool categoryPresent(const std::string& category) const;
    void removeCategory(const std::string& category);

private:
    std::unordered_set<std::string> categories_;
};

#endif // CATEGORY_H
