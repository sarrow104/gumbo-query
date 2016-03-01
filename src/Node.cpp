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

#include "Node.h"
#include "Selection.h"
#include "QueryUtil.h"

CNode::CNode(GumboNode* apNode)
{
	mpNode = apNode;
}

CNode::~CNode()
{
}

GumboNodeType CNode::nodeType() const
{
	return mpNode->type;
}

size_t CNode::indexWithinParent() const
{
	return mpNode->index_within_parent;
}

CNode CNode::parent()
{
	return CNode(mpNode->parent);
}

CNode CNode::nextSibling()
{
	return parent().childAt(mpNode->index_within_parent + 1);
}

CNode CNode::prevSibling()
{
	return parent().childAt(mpNode->index_within_parent - 1);
}

unsigned int CNode::childNum()
{
	return CQueryUtil::childNum(mpNode);
}

bool CNode::valid()
{
	return mpNode != NULL;
}

CNode CNode::childAt(size_t i)
{
	return CNode(CQueryUtil::nthChild(mpNode, i));
}

bool CNode::isGumboType(GumboNodeType type)
{
	return this->mpNode && this->mpNode->type == type;
}

std::string CNode::attribute(std::string key)
{
	if (mpNode->type != GUMBO_NODE_ELEMENT)
	{
		return "";
	}

	for (unsigned int i = 0; i < CQueryUtil::attrNum(mpNode); i++)
	{
		GumboAttribute* attr = CQueryUtil::nthAttr(mpNode, i);
		if (key == attr->name)
		{
			return attr->value;
		}
	}

	return "";
}

size_t CNode::attrNum() const
{
	return CQueryUtil::attrNum(mpNode);
}

const char * CNode::attrNameAt(size_t i) const
{
	GumboAttribute * attr = CQueryUtil::nthAttr(mpNode, i);
	return attr ? attr->name : 0;
}

const char * CNode::attrValueAt(size_t i) const
{
	GumboAttribute * attr = CQueryUtil::nthAttr(mpNode, i);
	return attr ? attr->value : 0;
}

const char * CNode::textGumbo() const
{
	switch (CQueryUtil::getGumboType(mpNode)) {
	case GUMBO_NODE_TEXT:
	case GUMBO_NODE_CDATA:
	case GUMBO_NODE_COMMENT:
	case GUMBO_NODE_WHITESPACE:
		return CQueryUtil::getText(mpNode);

	default:
		return 0;
	}
}


std::string CNode::text() const
{
	return CQueryUtil::nodeText(mpNode);
}


std::string CNode::textNeat() const
{
	return CQueryUtil::nodeTextNeat(mpNode);
}

std::string CNode::ownText() const
{
	return CQueryUtil::nodeOwnText(mpNode);
}

size_t CNode::startPos() const
{
	switch(mpNode->type)
	{
	case GUMBO_NODE_ELEMENT:
		return mpNode->v.element.start_pos.offset + mpNode->v.element.original_tag.length;

	case GUMBO_NODE_TEXT:
		return mpNode->v.text.start_pos.offset;

	default:
		return 0;
	}
}

size_t CNode::endPos() const
{
	switch(mpNode->type)
	{
	case GUMBO_NODE_ELEMENT:
		return mpNode->v.element.end_pos.offset;

	case GUMBO_NODE_TEXT:
		return mpNode->v.text.original_text.length + startPos();

	default:
		return 0;
	}
}

size_t CNode::startPosOuter() const
{
	switch(mpNode->type)
	{
	case GUMBO_NODE_ELEMENT:
		return mpNode->v.element.start_pos.offset;

	case GUMBO_NODE_TEXT:
		return mpNode->v.text.start_pos.offset;

	default:
		return 0;
	}
}

size_t CNode::endPosOuter() const
{
	switch(mpNode->type)
	{
	case GUMBO_NODE_ELEMENT:
		return mpNode->v.element.end_pos.offset + mpNode->v.element.original_end_tag.length;

	case GUMBO_NODE_TEXT:
		return mpNode->v.text.original_text.length + startPos();

	default:
		return 0;
	}
}

std::string CNode::tag() const
{
	if (mpNode->type != GUMBO_NODE_ELEMENT)
	{
		return "";
	}

	return gumbo_normalized_tagname(mpNode->v.element.tag);
}

CSelection CNode::find(std::string aSelector)
{
	CSelection c(mpNode);
	return c.find(aSelector);
}
/* vim: set ts=4 sw=4 sts=4 tw=100 noet: */
