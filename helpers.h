#ifndef HELPERS_H
#define HELPERS_H

class helpers {
public:
    int getSkipJackIndex(uint8_t value);

    uint64_t leftRotate(uint64_t v, int n);

    std::string Convert64ToString(uint64_t value);
};

#endif