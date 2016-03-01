#include "DocType.h"
#include "QueryUtil.h"

#include <iostream>

CDocType::CDocType()
    : mpDoctype(0)
{
}

CDocType::CDocType(GumboDocument * pdoctype)
    : mpDoctype(pdoctype)
{
}

CDocType::CDocType(CNode n)
    : mpDoctype(0)
{
    if (CQueryUtil::isGumboType(n.mpNode, GUMBO_NODE_DOCUMENT)) {
        mpDoctype = &n.mpNode->v.document;
    }
}

CDocType::~CDocType()
{
}

bool CDocType::isOK() const
{
    return this->mpDoctype && this->mpDoctype->has_doctype;
}

const char * CDocType::getName() const
{
    return this->mpDoctype ? this->mpDoctype->name : "";
}

const char * CDocType::getPublicIdentifier() const
{
    return this->mpDoctype ? this->mpDoctype->public_identifier : "";
}

const char * CDocType::getSystemIdentifier() const
{
    return this->mpDoctype ? this->mpDoctype->system_identifier : "";
}

void CDocType::print(std::ostream& o) const
{
    if (this->isOK()) {
        o << "<!DOCTYPE " << this->getName();
        if (this->getPublicIdentifier()[0]) {
            o << " \"" << this->getPublicIdentifier() << "\"";
        }
        if (this->getSystemIdentifier()[0]) {
            o << " \"" << this->getSystemIdentifier() << "\"";
        }
        o << ">";
    }
}
