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

#ifndef NODE_H_
#define NODE_H_

#include <gumbo.h>
#include <string>
#include <iosfwd>

class CSelection;

class CQueryUtil;

class CDocType;

class CNode
{
	friend class CQueryUtil;
	friend class CDocType;
	public:

		CNode(GumboNode* apNode = NULL);

		virtual ~CNode();

	public:
		void * get() const {
			return mpNode;
		}

		GumboNodeType nodeType() const;

		bool valid();

		size_t indexWithinParent() const;

		CNode parent();

		CNode nextSibling();

		CNode prevSibling();

		unsigned int childNum();

		CNode childAt(size_t i);

		bool isGumboType(GumboNodeType type);

		std::string attribute(std::string key);

		size_t attrNum() const;
		const char * attrNameAt(size_t i) const;
		const char * attrValueAt(size_t i) const;

		const char * textGumbo() const;

		std::string text() const;

		std::string textNeat() const;

		std::string ownText() const;

		// "整洁"地文本输出
		// 忽略<script>
		// <br> -> "\n"
		// <div> 等block属性的，自动添加空行；
		void printNeat() const;

		size_t startPos() const;

		size_t endPos() const;

		size_t startPosOuter() const;

		size_t endPosOuter() const;

		std::string tag() const;

		CSelection find(std::string aSelector);

	private:

		GumboNode* mpNode;
};

#endif /* NODE_H_ */

/* vim: set ts=4 sw=4 sts=4 tw=100 noet: */
