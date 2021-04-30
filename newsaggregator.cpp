/* Add ALL of your code to this file. */

#include <string>
#include <utility>
using namespace std;

namespace std {

    template <class Key, // map::key_type
        class T>   // map::mapped_type
        class map {
        public:
            typedef std::pair<const Key, T> value_type;
            typedef value_type* iterator;

            /**
             * Returns an iterator referring to the first element in the map
             * container.
             */
            iterator begin();

            /**
             * Returns an iterator referring to the past-the-end element in the map
             * container.
             */
            iterator end() const;

            /**
             * If k matches the key of an element in the container, the function
             * returns a reference to its mapped value.
             *
             * If k does not match the key of any element in the container, the
             * function inserts a new element with that key and returns a
             * reference to its mapped value. Notice that this always increases
             * the container size by one, even if no mapped value is assigned to
             * the element (the element is constructed using its default
             * constructor).
             */
            T& operator[](const Key& k);

            /**
             * Extends the container by inserting new elements, effectively
             * increasing the container size by the number of elements inserted.
             *
             * Because element keys in a map are unique, the insertion operation
             * checks whether each inserted element has a key equivalent to the
             * one of an element already in the container, and if so, the
             * element is not inserted, returning an iterator to this existing
             * element (if the function returns a value).
             */
            pair<iterator, bool> insert(const value_type& val);

            /**
             * Returns a reference to the mapped value of the element identified
             * with key k.
             *
             * If k does not match the key of any element in the container, the
             * function throws an out_of_range exception.
             */
            T& at(const Key& k);

            /**
             * Searches the container for elements with a key equivalent to k and
             * returns the number of matches.
             *
             * Because all elements in a map container are unique, the function
             * can only return 1 (if the element is found) or zero (otherwise).
             */
            size_t count(const Key& k) const;
    };
} // namespace std

/* Add your code below this line. */

// Question a
class Source; // forward declaration
class Category;

class NewsItem{
private:
    std::string title;
    std::string content;
    std::string author;
    Source* source;
    int viewCount = 0;
    Category* category;
public:
    NewsItem() {};
    NewsItem(std::string title, std::string content, std::string author, Source* source, Category* category)
        : title(title), content(content), author(author), source(source), category(category) {};

    Source* getSource() const {return source;}

    std::string getTitle() {return title;}

    int getViewCount() {return viewCount;}

    Category* getCategory() {return category;}

    std::string getContent() {
        viewCount++; // increment viewCount
        return content;
    }
};

class Source {
private:
    std::string url; 
    map<const std::string, NewsItem*> newsItemMap;   // Map NewsItem title to the item itself
    NewsItem* newsItemArray[1000];
    static const int maxSources = 100;
public:
    static Source* sources[maxSources];
    static int sourceCount;
    int count = 0;
    Category* hottestCategory;

    Source() {};
    Source(std::string url) {
        // check if source has already been added
        if (sourceCount > 0) {
            // throw is already added
            for (int i = 0; i < sourceCount; i++) {
                if (url == sources[i]->getURL()) throw;
            }
        }
        // throw exception if sourceCount==100
        else if (sourceCount == maxSources) throw;
        // all ok
        url = url;
        sourceCount++;
    };

    void addNewsItem(NewsItem* item) {
        // check count
        if (count == 1000) return;

        // add to array
        std::pair<std::string, NewsItem*> itemPair(item->getTitle(), item);
        newsItemMap.insert(itemPair);
        newsItemArray[count] = item;
        count++;
    }

    std::string getURL() const {
        return url;
    }

    // throws an out of range exception
    NewsItem* getNewsItem(std::string title) {
        return newsItemMap.at(title);
    }

    int maxViewCount() {
        int maxViewCount = 0;
        std::pair<const std::string, NewsItem*>* it; // it is a reference to pairs in map
        for (it = newsItemMap.begin(); it != newsItemMap.end(); it++) {
            if (((*it).second)->getViewCount() > maxViewCount) {
                maxViewCount = ((*it).second)->getViewCount();
                hottestCategory = ((*it).second)->getCategory();
            }
        }
        return maxViewCount;
    }
};

class Category {
private:
    std::string category;
    Category* parent_category = nullptr;
    static const int maxCategory = 500;
public:
    static Category* categories[maxCategory];
    static int categoryCount;

    Category() {};
    Category(std::string category, Category* parent_category = nullptr){
        // check if category has already been added
        if (categoryCount > 0) {
            for (int i = 0; i < categoryCount; i++) {
                if (category == categories[i]->getCategory()) throw;
            }
        }
        // throw exception if categoryCount==100
        else if (categoryCount == maxCategory) throw;
        // all ok
        category = category;
        parent_category = parent_category;
        categoryCount++;
    };

    std::string getCategory() {
        return category;
    }

};


// Question b
void insertNewsItem(std::string title, std::string text, std::string author, 
                    std::string categoryName, std::string sourceWebpage){
    Category* category = new Category(categoryName);
    Source* source = new Source(sourceWebpage);
    NewsItem* newsitem = new NewsItem(title, text, author, source, category);
    source->addNewsItem(newsitem);
}

// Question c
// utility function to retrieve a source
Source* getSource(std::string url) {
    if (Source::sourceCount > 0) {
        // throw is already added
        for (int i = 0; i < Source::sourceCount; i++) {
            if (url == Source::sources[i]->getURL) return Source::sources[i];
        }
    }
    else if (Source::sourceCount == 0) return nullptr;
}

std::string getContent(std::string sourceWebpage, std::string title) {
    Source* source = getSource(sourceWebpage);
    // if source doesnt exist
    if (!source) return;

    // if source is found
    NewsItem* newsitem = source->getNewsItem(title);
    return newsitem->getContent();
}


// Question d
Category getHottestCategory() {
    int maxViewCount = 0;
    Category hottestCategory;
    for (int i = 0; i < Source::sourceCount; i++) { // loop through each source
        if (Source::sources[i]->maxViewCount() > maxViewCount) {
            maxViewCount = Source::sources[i]->maxViewCount();
            hottestCategory = Source::sources[i]->hottestCategory;
        }
    }
    return hottestCategory.getCategory();
}

/* Here are examples of how the functions should be used (copied from
     exam paper). */

     /* DO NOT CHANGE ANYTHING BELOW THIS LINE!!! */

void usage1() {
    auto title = "Something Happened";
    auto text = "London is a city where things happen all the time";
    auto author = "A random stranger";
    auto categoryName = "London News";
    auto sourceWebpage = "http://www.blameberg.com";

    insertNewsItem(title, text, author, categoryName, sourceWebpage);
}

void usage2() {
    auto sourceWebpage = "http://www.blameberg.com";
    auto title = "Something Happened";

    string content = getContent(sourceWebpage, title);
}

void usage3() { //
    auto categoryName = getHottestCategory();
}

int main(int argc, char* argv[]) {
    usage1();
    usage2();
    usage3();
    return 0;
}
