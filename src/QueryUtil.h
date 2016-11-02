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

#ifndef QUERYUTIL_H_
#define QUERYUTIL_H_

#include <gumbo.h>

#include <iostream>
#include <string>
#include <vector>

#define ISSELFCLOSETAG_USING_GUMBO 1

class CQueryUtil {
public:
    class CIndenter {
    public:
	explicit CIndenter(const std::string& ind = "\t")
	    : mIndent(ind), mLevel(0u)
	{
	}
	~CIndenter() {}
    public:
	void push() { this->mLevel++; }
	void pop()
	{
	    if (this->mLevel) {
		this->mLevel--;
	    }
	}

	const std::string& get() const { return mIndent; }
	void print(std::ostream& o) const
	{
	    for (size_t i = 0; i != this->mLevel; ++i) {
		o << mIndent;
	    }
	}

    private:
	std::string mIndent;
	size_t mLevel;
    };

    class CIndentHelper {
    public:
	CIndentHelper(CIndenter& ind) : mIndent(ind) { mIndent.push(); }
	~CIndentHelper() { mIndent.pop(); }
    private:
	CIndenter& mIndent;
    };

public:
    // NOTE 负数，表示从尾部开始数！
    // -1 表示最后一个；
    inline static GumboNode* nthChild(const GumboNode* pnode, int i)
    {
	if (!pnode) {
	    return 0;
	}
	void* ret = 0;
	switch (pnode->type) {
	    case GUMBO_NODE_ELEMENT:
		if (i < 0) {
		    i = int(pnode->v.element.children.length) - i;
		}
		if (0 <= i && i < int(pnode->v.element.children.length)) {
		    ret = pnode->v.element.children.data[i];
		}
		break;

	    case GUMBO_NODE_DOCUMENT:
		if (i < 0) {
		    i = int(pnode->v.document.children.length) - i;
		}
		if (0 <= i && i < int(pnode->v.document.children.length)) {
		    ret = pnode->v.document.children.data[i];
		}
		break;

	    default:
		break;
	};
	return reinterpret_cast<GumboNode*>(ret);
    }

    inline static size_t childNum(const GumboNode* pnode)
    {
	if (!pnode) {
	    return 0;
	}
	switch (pnode->type) {
	    case GUMBO_NODE_ELEMENT:
		return pnode->v.element.children.length;
		break;

	    case GUMBO_NODE_DOCUMENT:
		return pnode->v.document.children.length;

	    default:
		return 0;
		break;
	};
    }

    inline static GumboTag eleTagID(const GumboNode* pnode)
    {
	GumboTag t = GUMBO_TAG_UNKNOWN;
	if (isGumboType(pnode, GUMBO_NODE_ELEMENT)) {
	    t = pnode->v.element.tag;
	}
	return t;
    }

    // NOTE
    // 自闭合标签列表，不同的html版本，该标签列表是不同的！
    // 具体得看dtd！
    // http://www.w3.org/TR/html-markup/syntax.html
    // area, base, br, col, command, embed, hr, img, input, keygen, link, meta,
    // param, source, track, wbr

    //! HTML5
    // There are five different kinds of elements: void elements, raw text
    // elements, escapable raw text elements, foreign elements, and normal
    // elements.
    //
    // Void elements
    //     area, base, br, col, embed, hr, img, input, keygen, link, meta,
    //     param, source, track, wbr
    // Raw text elements
    //     script, style
    // escapable raw text elements
    //     textarea, title
    // Foreign elements
    //     Elements from the MathML namespace and the SVG namespace.
    // Normal elements
    //     All other allowed HTML elements are normal elements.

    // NOTE gumbo-parser在解析html文本的时候，对于自闭和标签，以及缺少end_tag，
    // 但是可以被html的容错性，接纳的标签，读取之后，都会让
    //
    // 有如下几种情况：
    // 1. 自闭和标签；此时end_pos.offset == start_pos.offset;
    // original_end_tag.length == 0;
    // 2. 闭合标签，缺失，但是，有解析器补足；此时end_pos.offset !=
    // start_pos.offset; original_end_tag.length == 0
    // 3. 正常闭合的成对标签；此时end_pos.offset != start_pos.offset;
    // original_end_tag.length != 0;
    inline static bool isSelfCloseTag(const GumboNode* pnode)
    {
#ifdef ISSELFCLOSETAG_USING_GUMBO
	return pnode && isGumboType(pnode, GUMBO_NODE_ELEMENT) &&
	       pnode->v.element.end_pos.offset ==
		   pnode->v.element.start_pos.offset;
#else
	switch (eleTagID(pnode)) {
	    case GUMBO_TAG_AREA:
	    case GUMBO_TAG_BASE:
	    case GUMBO_TAG_BR:
	    case GUMBO_TAG_COL:
	    case GUMBO_TAG_EMBED:
	    case GUMBO_TAG_HR:
	    case GUMBO_TAG_IMG:
	    case GUMBO_TAG_INPUT:
	    case GUMBO_TAG_KEYGEN:
	    case GUMBO_TAG_LINK:
	    case GUMBO_TAG_META:
	    case GUMBO_TAG_PARAM:
	    case GUMBO_TAG_SOURCE:
	    case GUMBO_TAG_TRACK:
	    case GUMBO_TAG_WBR:
		return true;

	    default:
		return false;
	}
#endif
    }

    // 内联元素(inline element)
    inline static bool isDisplayInline(const GumboNode* pnode)
    {
	switch (eleTagID(pnode)) {
	    case GUMBO_TAG_A:
	    case GUMBO_TAG_ABBR:
	    case GUMBO_TAG_ACRONYM:
	    case GUMBO_TAG_B:
	    case GUMBO_TAG_BDO:
	    case GUMBO_TAG_BIG:
	    case GUMBO_TAG_BR:
	    case GUMBO_TAG_CITE:
	    case GUMBO_TAG_CODE:
	    case GUMBO_TAG_DFN:
	    case GUMBO_TAG_EM:
	    case GUMBO_TAG_FONT:
	    case GUMBO_TAG_I:
	    case GUMBO_TAG_IMG:
	    case GUMBO_TAG_INPUT:
	    case GUMBO_TAG_KBD:
	    case GUMBO_TAG_LABEL:
	    case GUMBO_TAG_Q:
	    case GUMBO_TAG_S:
	    case GUMBO_TAG_SAMP:
	    case GUMBO_TAG_SELECT:
	    case GUMBO_TAG_SMALL:
	    case GUMBO_TAG_SPAN:
	    case GUMBO_TAG_STRIKE:
	    case GUMBO_TAG_STRONG:
	    case GUMBO_TAG_SUB:
	    case GUMBO_TAG_SUP:
	    case GUMBO_TAG_TEXTAREA:
	    case GUMBO_TAG_TT:
	    case GUMBO_TAG_U:
	    case GUMBO_TAG_VAR:
		return true;
		break;

	    default:
		return false;
	}
    }

    // 块元素(block element)m]
    inline static bool isDisplayBlock(const GumboNode* pnode)
    {
	switch (eleTagID(pnode)) {
	    case GUMBO_TAG_ADDRESS:
	    case GUMBO_TAG_BLOCKQUOTE:
	    case GUMBO_TAG_CENTER:
	    case GUMBO_TAG_DIR:
	    case GUMBO_TAG_DIV:
	    case GUMBO_TAG_DL:
	    case GUMBO_TAG_FIELDSET:
	    case GUMBO_TAG_FORM:
	    case GUMBO_TAG_H1:
	    case GUMBO_TAG_H2:
	    case GUMBO_TAG_H3:
	    case GUMBO_TAG_H4:
	    case GUMBO_TAG_H5:
	    case GUMBO_TAG_H6:
	    case GUMBO_TAG_HR:
	    case GUMBO_TAG_ISINDEX:
	    case GUMBO_TAG_MENU:
	    case GUMBO_TAG_NOFRAMES:
	    case GUMBO_TAG_NOSCRIPT:
	    case GUMBO_TAG_OL:
	    case GUMBO_TAG_P:
	    case GUMBO_TAG_PRE:
	    case GUMBO_TAG_TABLE:
	    case GUMBO_TAG_UL:
		return true;

	    default:
		return false;
	}
    }

    // 可变元素
    // 可变元素为根据上下文语境决定该元素为块元素或者内联元素。
    inline static bool isDisplayVairable(const GumboNode* pnode)
    {
	switch (eleTagID(pnode)) {
	    case GUMBO_TAG_APPLET:
	    case GUMBO_TAG_BUTTON:
	    case GUMBO_TAG_DEL:
	    case GUMBO_TAG_IFRAME:
	    case GUMBO_TAG_INS:
	    case GUMBO_TAG_MAP:
	    case GUMBO_TAG_OBJECT:
	    case GUMBO_TAG_SCRIPT:
		return true;

	    default:
		return false;
	}
    }

    inline static size_t attrNum(const GumboNode* pnode)
    {
	return pnode && pnode->type == GUMBO_NODE_ELEMENT
		   ? pnode->v.element.attributes.length
		   : 0;
    }

    // 一个节点的属性，不可能成百上千个，所以，不支持利用负数，反向选择！
    inline static GumboAttribute* nthAttr(const GumboNode* pnode, int i)
    {
	if (i >= 0 && i < int(attrNum(pnode))) {
	    return reinterpret_cast<GumboAttribute*>(
		pnode->v.element.attributes.data[i]);
	}
	else {
	    return 0;
	}
    }

    inline static GumboNodeType getGumboType(const GumboNode* pnode)
    {
	return pnode->type;
    }

    inline static bool isGumboType(const GumboNode* pnode, GumboNodeType type)
    {
	return pnode->type == type;
    }

    inline static const char* getText(const GumboNode* pnode)
    {
	return pnode->v.text.text;
    }

public:
    static std::string tolower(std::string s);

    static std::vector<GumboNode*> unionNodes(std::vector<GumboNode*> aNodes1,
					      std::vector<GumboNode*> aNode2);

    static bool nodeExists(std::vector<GumboNode*> aNodes, GumboNode* apNode);

    static std::string nodeText(GumboNode* apNode);

    static std::string nodeTextNeat(GumboNode* apNode);

    static std::string nodeOwnText(GumboNode* apNode);

public:
    static void writeInnerHtml(std::ostream& o, GumboNode* apNode,
			       const char* ind = "\t");
    static void writeOuterHtml(std::ostream& o, GumboNode* apNode,
			       const char* ind = "\t");

private:
    static void writeInnerHtml_impl(std::ostream& o, GumboNode* apNode,
				    CIndenter& ind);
    static void writeOuterHtml_impl(std::ostream& o, GumboNode* apNode,
				    CIndenter& ind);

    static void writeNodeText(GumboNode* apNode, std::string& aText);

    static void writeNodeTextNeat(GumboNode* apNode, std::string& aText);
};

inline std::ostream& operator<<(std::ostream& o,
				const CQueryUtil::CIndenter& ind)
{
    ind.print(o);
    return o;
}

#endif /* QUERYUTIL_H_ */

/* vim: set ts=8 sw=4 sts=4 tw=100 noet: */
