#ifndef  __DOCTYPE_H_1447119806__
#define  __DOCTYPE_H_1447119806__

#include "Node.h"

#include <gumbo.h>
#include <string>

#include <iosfwd>


class CDocType
{
public:
    CDocType();
    explicit CDocType(GumboDocument * pdoctype);
    explicit CDocType(CNode n);

    virtual ~CDocType();

public:
    bool isOK() const;

    const char * getName() const;
    const char * getPublicIdentifier() const;
    const char * getSystemIdentifier() const;

    void print(std::ostream& o) const;

private:
    GumboDocument * mpDoctype;
};

inline std::ostream& operator << (std::ostream& o, const CDocType& dt)
{
    dt.print(o);
    return o;
}

#endif  /* __DOCTYPE_H_1447119806__ */
