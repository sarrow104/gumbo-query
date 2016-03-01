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

#include "QueryUtil.h"
#include "Node.h"
#include "DocType.h"

#include <sstream>
#include <iostream>

//#define _DEBUG_

std::string CQueryUtil::tolower(std::string s)
{
	for (unsigned int i = 0; i < s.size(); i++)
	{
		char c = s[i];
		if (c >= 'A' && c <= 'Z')
		{
			c = 'a' + c - 'A';
			s[i] = c;
		}
	}

	return s;
}

std::vector<GumboNode*> CQueryUtil::unionNodes(std::vector<GumboNode*> aNodes1,
		std::vector<GumboNode*> aNodes2)
{
	for (std::vector<GumboNode*>::iterator it = aNodes2.begin(); it != aNodes2.end(); it++)
	{
		GumboNode* pNode = *it;
		if (nodeExists(aNodes1, pNode))
		{
			continue;
		}

		aNodes1.push_back(pNode);
	}

	return aNodes1;
}

bool CQueryUtil::nodeExists(std::vector<GumboNode*> aNodes, GumboNode* apNode)
{
	for (std::vector<GumboNode*>::iterator it = aNodes.begin(); it != aNodes.end(); it++)
	{
		GumboNode* pNode = *it;
		if (pNode == apNode)
		{
			return true;
		}
	}
	return false;
}

std::string CQueryUtil::nodeText(GumboNode* apNode)
{
	std::string text;
	writeNodeText(apNode, text);
	return text;
}


std::string CQueryUtil::nodeTextNeat(GumboNode* apNode)
{
	std::string text;
	writeNodeTextNeat(apNode, text);
	return text;
}

std::string CQueryUtil::nodeOwnText(GumboNode* apNode)
{
	std::string text;
	if (!isGumboType(apNode, GUMBO_NODE_ELEMENT))
	{
		return text;
	}

	for (unsigned int i = 0; i < childNum(apNode); i++)
	{
		GumboNode* child = nthChild(apNode, i);
		if (isGumboType(child, GUMBO_NODE_TEXT))
		{
			text.append(getText(child));
		}
	}

	return text;
}

// 内部所有节点：
void CQueryUtil::writeInnerHtml(std::ostream& o, GumboNode * apNode, const char * ind)
{
	CIndenter indent(ind);
	writeInnerHtml_impl(o, apNode, indent);
}

// 先自己；然后内部
void CQueryUtil::writeOuterHtml(std::ostream& o, GumboNode * apNode, const char * ind)
{
	CIndenter indent(ind);
	writeOuterHtml_impl(o, apNode, indent);
}

void CQueryUtil::writeInnerHtml_impl(std::ostream& o, GumboNode * apNode, CIndenter& ind)
{
	if (!isGumboType(apNode, GUMBO_NODE_ELEMENT)) {
		return;
	}
	for (size_t i = 0; i != childNum(apNode); ++i) {
		writeOuterHtml_impl(o, nthChild(apNode, i), ind);
	}
}

void CQueryUtil::writeOuterHtml_impl(std::ostream& o, GumboNode * apNode, CIndenter& ind)
{
	switch (apNode->type)
	{
	case GUMBO_NODE_TEXT:
		if (childNum(apNode->parent) >= 2) {
			o << ind;
		}

		if (eleTagID(apNode->parent) == GUMBO_TAG_SCRIPT) {
			o << getText(apNode);
		}
		else {
			for (const char * s = getText(apNode); s && s[0]; ++s) {
				switch (s[0]) {
				case '<':
					o << "&lt;";
					break;

				case '>':
					o << "&gt;";
					break;

				default:
					o << s[0];
					break;
				}
			}
		}
		break;

	case GUMBO_NODE_ELEMENT:
		{
			bool is_self_close = isSelfCloseTag(apNode);
			// bool is_neat_print = isDisplayVairable(apNode) || isDisplayInline(apNode);
			bool is_neat_print = (childNum(apNode) == 1 && childNum(nthChild(apNode, 0)) == 0);

			o << ind << "<" << CNode(apNode).tag();
			for (size_t i  = 0; i != attrNum(apNode); ++i) {
				o << " " << nthAttr(apNode, i)->name << "=\"" << nthAttr(apNode, i)->value << "\"";
			}
			if (childNum(apNode))
			{
				o << ">";
				if (!is_neat_print) {
					o << std::endl;
				}
				CIndentHelper ih(ind);
				for (unsigned int i = 0; i < childNum(apNode); i++)
				{
					if (isGumboType(nthChild(apNode, i), GUMBO_NODE_WHITESPACE)) {
						continue;
					}
					if (is_neat_print) {
						writeOuterHtml(o, nthChild(apNode, i), ind.get().c_str());
					}
					else {
						writeOuterHtml_impl(o, nthChild(apNode, i), ind);
						o << std::endl;
					}
				}
			}

			if (is_self_close) {
				o << "/>";
			}
			else {
				if (!childNum(apNode)) {
					o << ">";
				}
				else {
					if (!is_neat_print) {
						o << ind;
					}
				}
				o << "</" << CNode(apNode).tag() << ">";
			}
		}
		break;

	case GUMBO_NODE_CDATA:
		o << ind << "<!CDATA[" << getText(apNode) << "]]>";
		break;

	case GUMBO_NODE_COMMENT:
		o << ind << "<!--" << getText(apNode) << "-->";
		break;

	case GUMBO_NODE_TEMPLATE:
		o << ind << "<!-- GUMBO_NODE_TEMPLATE not implement -->";
		break;

	case GUMBO_NODE_DOCUMENT:
		o << ind << CDocType(&apNode->v.document) << std::endl;
		std::cout << childNum(apNode) << std::endl;
		for (unsigned int i = 0; i < childNum(apNode); i++)
		{
			writeOuterHtml_impl(o, nthChild(apNode, i), ind);
		}
		break;

	case GUMBO_NODE_WHITESPACE:
		//o << ind << " ";
		break;

	default:
		{
			std::ostringstream oss;
			oss << "unknown GumboNodeType value " << apNode->type;
			throw oss.str();
		}
		break;
	}
}

void CQueryUtil::writeNodeText(GumboNode* apNode, std::string& aText)
{
	switch (apNode->type)
	{
	case GUMBO_NODE_TEXT:
		aText.append(getText(apNode));
		break;

	case GUMBO_NODE_ELEMENT:
		{
			for (unsigned int i = 0; i < childNum(apNode); i++)
			{
				writeNodeText(nthChild(apNode, i), aText);
			}
			break;
		}
	default:
		break;
	}
}

void CQueryUtil::writeNodeTextNeat(GumboNode* apNode, std::string& aText)
{
	switch (apNode->type)
	{
	case GUMBO_NODE_TEXT:
		aText.append(getText(apNode));
		break;

	case GUMBO_NODE_WHITESPACE:
#if 1
#else
		aText.append(" "); // 0xA0
#endif
		break;

	case GUMBO_NODE_ELEMENT:
		{
			switch (eleTagID(apNode)) {
			case GUMBO_TAG_SCRIPT:
				break;

			case GUMBO_TAG_BR:
			case GUMBO_TAG_LI:
				aText.append("\n");
				break;

			case GUMBO_TAG_P:
			case GUMBO_TAG_BLOCKQUOTE:
			case GUMBO_TAG_DIV:
			case GUMBO_TAG_TR:
			case GUMBO_TAG_TABLE:
			case GUMBO_TAG_DL:
			case GUMBO_TAG_H1:
			case GUMBO_TAG_H2:
			case GUMBO_TAG_H3:
			case GUMBO_TAG_H4:
			case GUMBO_TAG_H5:
			case GUMBO_TAG_H6:
			case GUMBO_TAG_HR:
			case GUMBO_TAG_OL:
			case GUMBO_TAG_UL:
				aText.append("\n\n");
				break;
			}

			for (unsigned int i = 0; i < childNum(apNode); i++)
			{
				writeNodeTextNeat(nthChild(apNode, i), aText);
			}
			break;
		}

	default:
		break;
	}
}

/* vim: set ts=4 sw=4 sts=4 tw=100 noet: */
