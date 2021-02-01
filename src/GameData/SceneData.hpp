#ifndef COLLADACPP_SCENEDATA_HPP_
#define COLLADACPP_SCENEDATA_HPP_

#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/dom/DOM.hpp>
#include <xercesc/sax/HandlerBase.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/util/PlatformUtils.hpp>

#include "../GameData/ObjectData.hpp"

#include <istream>
#include <memory>

using namespace xercesc;
using namespace std;

class SceneData
{
public:
    SceneData();
    ~SceneData();
    DOMDocument *load(const string &filename);

private:
    XercesDOMParser *parser_;
    DOMNodeList *getElementsByTagName(const DOMElement *element, string tag);
    DOMElement *getElementByTagName(const DOMElement *element, string tag);
    ObjectData *objectsData_;
};

#endif