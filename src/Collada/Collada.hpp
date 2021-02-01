#ifndef COLLADACPP_COLLADA_HPP_
#define COLLADACPP_COLLADA_HPP_

#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/dom/DOM.hpp>
#include <xercesc/sax/HandlerBase.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/util/PlatformUtils.hpp>

#include "../GameData/ObjectData.hpp"
#include "../GameData/SceneData.hpp"

#include <istream>
#include <memory>

using namespace xercesc;
using namespace std;

class Collada
{
public:
    Collada();
    ~Collada();
    DOMDocument *load(const string &filename);

private:
    XercesDOMParser *parser_;
    DOMNodeList *getElementsByTagName(const DOMElement *element, string tag);
    DOMElement *getElementByTagName(const DOMElement *element, string tag);
    shared_ptr<SceneData> loadScene(const DOMDocument *element);
};

#endif