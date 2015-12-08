/***************************************************************************
 *
 * $Id$
 *
 **************************************************************************/

/**
 * @file $HeadURL$
 * @author $Author$(hoping@baimashi.com)
 * @date $Date$
 * @version $Revision$
 * @brief
 *
 **/

#include "Document.h"
#include "DocType.h"
#include "Node.h"
#include "QueryUtil.h"

CDocument::CDocument()
{
	mpOutput = NULL;
}

CDocument::~CDocument()
{
	reset();
}

void CDocument::parse(const std::string& aInput)
{
	reset();
	mpOutput = gumbo_parse(aInput.c_str());
}

void CDocument::print(std::ostream& o) const
{
	CQueryUtil::writeOuterHtml(o, this->mpOutput->document);
}

int CDocument::errorCount() const
{
	return this->mpOutput ? this->mpOutput->errors.length : 0;
}

bool CDocument::isOK() const
{
	return this->mpOutput && 0 == this->mpOutput->errors.length;
}

CSelection CDocument::find(std::string aSelector)
{
	if (mpOutput == NULL)
	{
		throw "document not initialized";
	}
	if (!this->isOK())
	{
		throw "document initialized failed";
	}

	CSelection sel(mpOutput->root);
	return sel.find(aSelector);
}

CDocType CDocument::doctype() const
{
	if (this->mpOutput) {
		return CDocType(&this->mpOutput->document->v.document);
	}
	else {
		return CDocType();
	}
}

CNode    CDocument::root()    const
{
	if (this->mpOutput) {
		return CNode(this->mpOutput->root);
	}
	else {
		return CNode();
	}
}

CNode    CDocument::document()    const
{
	if (this->mpOutput) {
		return CNode(this->mpOutput->document);
	}
	else {
		return CNode();
	}
}

void CDocument::reset()
{
	if (mpOutput != NULL)
	{
		gumbo_destroy_output(&kGumboDefaultOptions, mpOutput);
		mpOutput = NULL;
	}
}

/* vim: set ts=4 sw=4 sts=4 tw=100 noet: */

