#pragma once

template<typename Key, typename Iterator>
const Iterator linear_search(const Iterator& cbegin, const Iterator& cend, const Key& key)
{
    for (Iterator i = cbegin; i != cend; ++i)
    {
        if (*i == key)
        {
            return i;
        }
    }

    return cend;
}