#include <istream>

#include "./Collada.hpp"

#include "./../Debug/console.h"

Collada::Collada()
{
}

Collada::~Collada()
{
}

DOMDocument *Collada::load(const string &filename)
{
    try
    {
        XMLPlatformUtils::Initialize();
    }
    catch (const XMLException &e)
    {
        char *errMsg = XMLString::transcode(e.getMessage());
        ERROR("Xerces XML ERROR: %s", errMsg);
        XMLString::release(&errMsg);
        return NULL;
    }

    parser_ = new XercesDOMParser();

    ErrorHandler *errHandler = (ErrorHandler *)new HandlerBase();
    parser_->setErrorHandler(errHandler);

    try
    {
        parser_->parse(filename.c_str());
    }
    catch (const XMLException &e)
    {
        char *errMsg = XMLString::transcode(e.getMessage());
        ERROR("Xerces XML ERROR: %s", errMsg);
        XMLString::release(&errMsg);
        return NULL;
    }
    catch (const DOMException &e)
    {
        char *errMsg = XMLString::transcode(e.msg);
        ERROR("Xerces DOM ERROR: %s", errMsg);
        XMLString::release(&errMsg);
        return NULL;
    }
    catch (...)
    {
        ERROR("Unknown Xerces ERROR...");
        return NULL;
    }

    DOMDocument *xmlDoc = parser_->getDocument();
    DOMElement *elementRoot = xmlDoc->getDocumentElement();
    if (!elementRoot)
    {
        ERROR("Empty XML document...");
        return NULL;
    }

    loadScene(xmlDoc);

    delete errHandler;

    return xmlDoc;
}

DOMNodeList *Collada::getElementsByTagName(const DOMElement *element, string tag)
{
    DEBUG_M("Entering function...");
    XMLCh *xtag = XMLString::transcode(tag.c_str());
    DOMNodeList *nl = element->getElementsByTagName(xtag);
    XMLString::release(&xtag);
    return nl;
}

DOMElement *Collada::getElementByTagName(const DOMElement *element, string tag)
{
    DOMNodeList *elements = getElementsByTagName(element, tag);
    if (!elements)
    {
        ERROR("No node with tag '%s' found...", tag.c_str());
        return NULL;
    }

    DOMNode *node = elements->item(0);
    if (!node)
    {
        return NULL;
    }

    return dynamic_cast<xercesc::DOMElement *>(elements->item(0));
}

shared_ptr<SceneData> Collada::loadScene(const DOMDocument *xmlDoc){
    shared_ptr<SceneData> sceneData;

    DOMElement *node = getElementByTagName(xmlDoc->getDocumentElement(), "scene");

    DOMNodeList *children = node->getChildNodes();
    int length = children->getLength();
    for (int i = 0; i < length; i++)
    {
        DOMNode *currentNode = children->item(i);
        if (currentNode->getNodeType() && currentNode->getNodeType() == DOMNode::ELEMENT_NODE)
        {
            DOMElement *currentElement = dynamic_cast<xercesc::DOMElement *>(currentNode);

            char *tagName = XMLString::transcode(currentElement->getTagName());
            XMLCh *url_x = XMLString::transcode("url");
            char *url = XMLString::transcode(currentElement->getAttribute(url_x));

            if (XMLString::compareIString(tagName, "instance_visual_scene") == 0)
            {
                printf("instance_visual_scene: %s", url);
            }
        }
    }

    return sceneData;
}

/*shared_ptr<VisualScene> Collada::loadVisualScene(const DOMElement *element)
{
   DEBUG_M("Entering function...");

   shared_ptr<VisualScene> visualScene(new VisualScene());

   DOMNodeList *children = element->getChildNodes();
   int length = children->getLength();
   for (int i = 0; i < length; i++)
   {
      DOMNode *currentNode = children->item(i);
      if (currentNode->getNodeType() && currentNode->getNodeType() == DOMNode::ELEMENT_NODE)
      {
         DOMElement *currentElement = dynamic_cast<xercesc::DOMElement *>(currentNode);
         ColladaObjectPtr colladaObject(getColladaObjectById(getAttribute(currentElement, "id")));
         shared_ptr<ColladaNode> node(static_pointer_cast<ColladaNode, ColladaObject>(colladaObject));
         visualScene->addColladaNode(node);
      }
   }
   return visualScene;
}*/