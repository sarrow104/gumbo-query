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

#ifndef DOCUMENT_H_
#define DOCUMENT_H_

#include <gumbo.h>
#include <string>
#include <iosfwd>
#include "Selection.h"

class CDocType;

class CNode;

class CDocument: public CObject
{
	public:

		CDocument();

		virtual ~CDocument();

	public:

		void parse(const std::string& aInput);

		void print(std::ostream& o) const;

		int errorCount() const;

		bool isOK() const;

		CSelection find(std::string aSelector);

	public:
		CDocType doctype() const;
		CNode    root()    const;
		CNode    document()    const;

	private:
		// NOTE 不支持拷贝与复制；
		// 或者使用支持释放语句的使用计数智能指针
		CDocument & operator = (const CDocument&);
		CDocument(const CDocument&);

	private:

		void reset();

	private:

		GumboOutput* mpOutput;
};

#endif /* DOCUMENT_H_ */

/* vim: set ts=4 sw=4 sts=4 tw=100 noet: */
