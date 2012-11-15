#ifndef ISERIALIZABLECONTAINER_H
#define ISERIALIZABLECONTAINER_H

#include<iostream>
#include"iserializable.h"

namespace hax {

template<class value_type> class ISerializableContainer : public ISerializable
{
  protected:
    virtual void ElementToString(std::ostream& out, const value_type& elem) const = 0;
    virtual void ElementFromString(std::istream& in) = 0;
    virtual void ReLink(const std::map<std::size_t, std::size_t>& converter) = 0;
};
}
#endif
