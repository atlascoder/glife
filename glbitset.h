#ifndef GLBITSET_H
#define GLBITSET_H

#include <memory>
#include <vector>

class glbitset
{
public:
    glbitset(size_t size);
    glbitset(const glbitset& other);
    glbitset(glbitset&& other);
    glbitset& operator=(const glbitset& other);
    glbitset& operator=(glbitset&& other);

    bool at(size_t pos) const;
private:
    size_t mLength;
    std::unique_ptr<std::vector<int>> mStorage;
};

#endif // GLBITSET_H
