#include "row.h"
#include "category.h"

Row::Row()
    : category_{std::make_shared<Category>()}
{
    /*
     * At this moment, for simplicity, the content of category is hardcoded.
     */
    fillCategories();
}

void Row::fillCategories()
{
    category_->addCategory("tconst");
    category_->addCategory("titleType");
    category_->addCategory("primaryTitle");
    category_->addCategory("originalTitle");
    category_->addCategory("isAdult");
    category_->addCategory("startYear");
    category_->addCategory("endYear");
    category_->addCategory("runtimeMinutes");
    category_->addCategory("genres");
}
