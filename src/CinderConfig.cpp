#include "CinderConfig.h"

using namespace ci;
using namespace std;

namespace cinder { namespace config {

//-----------------------------------------------------------------------------

void Config::save(std::string filePath)
{
    std::string myXmlStr( "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>" );
	XmlTree doc(myXmlStr);
	XmlTree* node;
	node = new XmlTree();
	node->setTag("general");

	for(std::vector<ConfigParam>::iterator it = mConfigParameters.begin(); it!=mConfigParameters.end(); ++it)
	{
		XmlTree pn;
		pn.setTag(it->name);

		switch(it->type)
		{
		case _NODE:
			doc.push_back( *node );
			node = new XmlTree();
			node->setTag(it->name);
			break;
		case _BOOL:
			pn.setValue<string>( toString(*((bool*)it->param)) );
			node->push_back( pn );
			break;
		case _FLOAT:
			pn.setValue<float>( *((float*)it->param) );
			node->push_back( pn );
			break;
		case _DOUBLE:
			pn.setValue<double>( *((double*)it->param) );
			node->push_back( pn );
			break;
		case _INT:
			pn.setValue<int>( *((int*)it->param) );
			node->push_back( pn );
			break;
		case _VEC3F:
			pn.setAttribute<float>("x", (*((Vec3f*)it->param)).x);
			pn.setAttribute<float>("y", (*((Vec3f*)it->param)).y);
			pn.setAttribute<float>("z", (*((Vec3f*)it->param)).z);
			node->push_back( pn );
			break;
		case _QUATF:
			pn.setAttribute<float>("w", (*((Quatf*)it->param)).w);
			pn.setAttribute<float>("x", (*((Quatf*)it->param)).v.x);
			pn.setAttribute<float>("y", (*((Quatf*)it->param)).v.y);
			pn.setAttribute<float>("z", (*((Quatf*)it->param)).v.z);
			node->push_back( pn );
			break;
		case _COLOR:
			pn.setAttribute<float>("r", (*((Color*)it->param)).r);
			pn.setAttribute<float>("g", (*((Color*)it->param)).g);
			pn.setAttribute<float>("b", (*((Color*)it->param)).b);
			node->push_back( pn );
			break;
		case _COLORA:
			pn.setAttribute<float>("r", (*((ColorA*)it->param)).r);
			pn.setAttribute<float>("g", (*((ColorA*)it->param)).g);
			pn.setAttribute<float>("b", (*((ColorA*)it->param)).b);
			pn.setAttribute<float>("a", (*((ColorA*)it->param)).a);
			node->push_back( pn );
			break;
		case _STRING:
			pn.setValue<std::string>( *((std::string*)it->param) );
			node->push_back( pn );
			break;
		}
	}
	
	doc.push_back( *node );
	doc.write(writeFile(filePath));

	node = NULL;
}
    
void Config::load(std::string filePath)
{
    try
	{
		XmlTree doc(loadFile(filePath));
		XmlTree node;
		node = doc.getChild("general");

		for(std::vector<ConfigParam>::iterator it = mConfigParameters.begin(); it!=mConfigParameters.end(); ++it)
		{
			switch(it->type)
			{
			case _NODE:
				node = doc.getChild(it->name);
				break;
			case _BOOL:
				*((bool*)it->param) = node.getChild(it->name).getValue<bool>();
				break;
			case _FLOAT:
				*((float*)it->param) = node.getChild(it->name).getValue<float>();
				break;
			case _DOUBLE:
				*((double*)it->param) = node.getChild(it->name).getValue<double>();
				break;
			case _INT:	
				*((int*)it->param) = node.getChild(it->name).getValue<int>();
				break;
			case _VEC3F:
				(*((Vec3f*)it->param)).x = node.getChild(it->name).getAttributeValue<float>("x");
				(*((Vec3f*)it->param)).y = node.getChild(it->name).getAttributeValue<float>("y");
				(*((Vec3f*)it->param)).z = node.getChild(it->name).getAttributeValue<float>("z");
				break;
			case _QUATF:
				(*((Quatf*)it->param)).w = node.getChild(it->name).getAttributeValue<float>("w");
				(*((Quatf*)it->param)).v.x = node.getChild(it->name).getAttributeValue<float>("x");
				(*((Quatf*)it->param)).v.y = node.getChild(it->name).getAttributeValue<float>("y");
				(*((Quatf*)it->param)).v.z = node.getChild(it->name).getAttributeValue<float>("z");
				break;
			case _COLOR:
				(*((Color*)it->param)).r = node.getChild(it->name).getAttributeValue<float>("r");
				(*((Color*)it->param)).g = node.getChild(it->name).getAttributeValue<float>("g");
				(*((Color*)it->param)).b = node.getChild(it->name).getAttributeValue<float>("b");
				break;
			case _COLORA:
				(*((ColorA*)it->param)).r = node.getChild(it->name).getAttributeValue<float>("r");
				(*((ColorA*)it->param)).g = node.getChild(it->name).getAttributeValue<float>("g");
				(*((ColorA*)it->param)).b = node.getChild(it->name).getAttributeValue<float>("b");
				(*((ColorA*)it->param)).a = node.getChild(it->name).getAttributeValue<float>("a");
				break;
			case _STRING:
				*((std::string*)it->param) = node.getChild(it->name).getValue<std::string>();
				break;		
			}
		}
	}
	catch(Exception e) {
		std::cout << "ERROR loading/reading config file." << std::endl;
	}
}

//-----------------------------------------------------------------------------

void Config::addParam( const std::string &name, bool *param, const std::string &optionsStr, bool readOnly, const std::string &keyName )
{
    if(mParamsInitialized)
        mParams->addParam(name, param, optionsStr, readOnly);
    addConfigParam(name, keyName, param, _BOOL);
} 

void Config::addParam( const std::string &name, float *param, const std::string &optionsStr, bool readOnly, const std::string &keyName )
{
    if(mParamsInitialized)
        mParams->addParam(name, param, optionsStr, readOnly);
    addConfigParam(name, keyName, param, _FLOAT);
} 

void Config::addParam( const std::string &name, double *param, const std::string &optionsStr, bool readOnly, const std::string &keyName )
{
    if(mParamsInitialized)
        mParams->addParam(name, param, optionsStr, readOnly);
    addConfigParam(name, keyName, param, _DOUBLE);
} 

void Config::addParam( const std::string &name, int32_t *param, const std::string &optionsStr, bool readOnly, const std::string &keyName )
{
    if(mParamsInitialized)
        mParams->addParam(name, param, optionsStr, readOnly);
    addConfigParam(name, keyName, param, _INT);
} 

void Config::addParam( const std::string &name, Vec3f *param, const std::string &optionsStr, bool readOnly, const std::string &keyName )
{
    if(mParamsInitialized)
        mParams->addParam(name, param, optionsStr, readOnly);
    addConfigParam(name, keyName, param, _VEC3F);
} 

void Config::addParam( const std::string &name, Quatf *param, const std::string &optionsStr, bool readOnly, const std::string &keyName )
{
    if(mParamsInitialized)
        mParams->addParam(name, param, optionsStr, readOnly);
    addConfigParam(name, keyName, param, _QUATF);
} 

void Config::addParam( const std::string &name, Color *param, const std::string &optionsStr, bool readOnly, const std::string &keyName )
{
    if(mParamsInitialized)
        mParams->addParam(name, param, optionsStr, readOnly);
    addConfigParam(name, keyName, param, _COLOR);
} 

void Config::addParam( const std::string &name, ColorA *param, const std::string &optionsStr, bool readOnly, const std::string &keyName )
{
    if(mParamsInitialized)
        mParams->addParam(name, param, optionsStr, readOnly);
    addConfigParam(name, keyName, param, _COLORA);
} 

void Config::addParam( const std::string &name, std::string *param, const std::string &optionsStr, bool readOnly, const std::string &keyName )
{
    if(mParamsInitialized)
        mParams->addParam(name, param, optionsStr, readOnly);
    addConfigParam(name, keyName, param, _STRING);
} 

void Config::addParam( const std::string &name, const std::vector<std::string> &enumNames, int *param, const std::string &optionsStr, bool readOnly, const std::string &keyName )
{
	if(mParamsInitialized)
        mParams->addParam(name, enumNames, param, optionsStr, readOnly);
    addConfigParam(name, keyName, param, _INT);
}

void Config::newNode( const std::string &name, const std::string &optionsStr )
{
	if(mParamsInitialized)
		mParams->addText(name, optionsStr);
	mConfigParameters.push_back( ConfigParam(name, 0, _NODE) );
}

//-----------------------------------------------------------------------------

void Config::addConfigParam( const std::string &name, const std::string &keyName, void* param, ConfigParamTypes type)
{
    std::string _keyName;
    (keyName == "") ? _keyName = name : _keyName = keyName;
    mConfigParameters.push_back( ConfigParam(_keyName, param, type) );
}

    
} } // namespace cinder::config
