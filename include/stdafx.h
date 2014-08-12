///////////////////////////////////////////////////////////////////////////////
/// @file	stdafx.h
/// @date	2014/08/01	
/// @author	Masayuki
/// @brief	プリコンパイルヘッダー
///////////////////////////////////////////////////////////////////////////////

#ifndef STDAFX_H_
#define STDAFX_H_

#include <stdlib.h>
#include <boost/foreach.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/replace.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <memory>
#include <list>
#include <vector>
class mxSprite;
typedef std::auto_ptr<mxSprite> SpritePtr;
typedef std::list<SpritePtr> SpriteList;
typedef std::vector<SpritePtr> SpriteVector;

#endif /* STDAFX_H_ */
