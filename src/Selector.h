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

#ifndef SELECTOR_H_
#define SELECTOR_H_

#include <gumbo.h>
#include <string>
#include <vector>
#include "Object.h"

/**
 * 选择器-基类
 */
class CSelector: public CObject
{

	public:
		typedef enum
		{
			/**
			 * place holer
			 */
			EDummy,
			/**
			 * match none element node which has no sub-text or sub-elements!
			 */
			EEmpty,
			/**
			 * the only leaf child of parent
			 */
			EOnlyChild,
			/**
			 * the Nth child [mA, mB)
			 */
			ENthChild,
			/**
			 * math node by exactly tag-name
			 */
			ETag,
		} TOperator;
	public:

		CSelector(TOperator aOp = EDummy)
		{
			init();
			mOp = aOp;
		}

		CSelector(bool aOfType)
		{
			init();
			mOp = EOnlyChild;
			mOfType = aOfType;
		}

		CSelector(unsigned int aA, unsigned int aB, bool aLast, bool aOfType)
		{
			init();
			mOp = ENthChild;
			mA = aA;
			mB = aB;
			mLast = aLast;
			mOfType = aOfType;
		}

		CSelector(GumboTag aTag)
		{
			init();
			mOp = ETag;
			mTag = aTag;
		}

		virtual ~CSelector()
		{
		}

	public:

		virtual bool match(GumboNode* apNode);

		std::vector<GumboNode*> filter(std::vector<GumboNode*> nodes);

		std::vector<GumboNode*> matchAll(GumboNode* apNode);

	private:

		void init()
		{
			mOfType = false;
			mA = 0;
			mB = 0;
			mLast = false;
			mTag = GumboTag(0);
		}

		void matchAllInto(GumboNode* apNode, std::vector<GumboNode*>& nodes);

	private:

		TOperator mOp;

		bool mOfType;

		unsigned int mA;

		unsigned int mB;

		bool mLast;

		GumboTag mTag;
};

/**
 * 一元选择器
 */
class CUnarySelector: public CSelector
{
	public:
		typedef enum
		{
			/**
			 * 不满足选择条件的节点；取反；
			 */
			ENot,
			/**
			 * 含有满足条件的任意子孙节点的节点
			 */
			EHasDescendant,
			/**
			 * 含有满足条件孩子节点的节点
			 */
			EHasChild,
		} TOperator;

	public:

		CUnarySelector(TOperator aOp, CSelector* apS);

		virtual ~CUnarySelector();

	public:

		virtual bool match(GumboNode* apNode);

	private:

		bool hasDescendantMatch(GumboNode* apNode, CSelector* apS);

		bool hasChildMatch(GumboNode* apNode, CSelector* apS);

	private:

		CSelector* mpS;

		TOperator mOp;
};

/**
 * 二元选择器
 */
class CBinarySelector: public CSelector
{
	public:
		typedef enum
		{
			/**
			 * || 二选一，或者，操作符
			 */
			EUnion,
			/**
			 * && 交集，同时符合两个条件 操作符
			 */
			EIntersection,
			/**
			 * 父子关系同时满足 操作符
			 */
			EChild,
			/**
			 * 某祖先满足，当前也满足 操作符
			 */
			EDescendant,
			/**
			 * 兄长节点以及当前，同时满足 操作符<br />
			 *  mAdjacent 用来决定是否挨在一起
			 */
			EAdjacent,
		} TOperator;

	public:

		CBinarySelector(TOperator aOp, CSelector* apS1, CSelector* apS2);

		CBinarySelector(CSelector* apS1, CSelector* apS2, bool aAdjacent);

		~CBinarySelector();

	public:

		virtual bool match(GumboNode* apNode);

	private:

		CSelector* mpS1;

		CSelector* mpS2;

		TOperator mOp;

		bool mAdjacent;
};

/**
 * 属性选择器<br />
 * 某名字的属性，是否符合值的的特点
 */
class CAttributeSelector: public CSelector
{
	public:
		typedef enum
		{
			/**
			 * 是否存在任何属性
			 */
			EExists,
			/**
			 * 属性是否与特征串相等
			 */
			EEquals,
			/**
			 * 属性值 是否包含 特征串
			 */
			EIncludes,
			/**
			 * 属性值是否形如 "特征串-"
			 */
			EDashMatch,
			/**
			 * 是否以特征串为前缀
			 */
			EPrefix,
			/**
			 * 是否以特征串为后缀
			 */
			ESuffix,
			/**
			 * 是否以子串包含特征串
			 */
			ESubString,
		} TOperator;

	public:

		CAttributeSelector(TOperator aOp, std::string aKey, std::string aValue = "");

	public:

		virtual bool match(GumboNode* apNode);

	private:

		std::string mKey;

		std::string mValue;

		TOperator mOp;
};

/**
 * 按内部文本是否符合提供值，检索；
 */
class CTextSelector: public CSelector
{
	public:
		typedef enum
		{
			/**
			 * 仅当前文本符合条件
			 */
			EOwnContains,
			/**
			 * 内部所有文本，满足条件
			 */
			EContains,
		} TOperator;

	public:
		CTextSelector(TOperator aOp, std::string aValue)
		{
			mValue = aValue;
			mOp = aOp;
		}

		~CTextSelector()
		{
		}

	public:

		virtual bool match(GumboNode* apNode);

	private:

	private:

		std::string mValue;

		TOperator mOp;
};

#endif /* SELECTOR_H_ */

/* vim: set ts=4 sw=4 sts=4 tw=100 noet: */
