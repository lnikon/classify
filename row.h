#ifndef ROW_H
#define ROW_H

#include <string>
#include <memory>

class Category;
class Row
{
public:
    Row();

private:
    std::shared_ptr<Category> category_{nullptr};

    void fillCategories();
};

#endif // ROW_H
