#include "CinderConfig.h"

using namespace ci;
using namespace std;

namespace cinder { namespace config {

//-----------------------------------------------------------------------------

void Config::save(fs::path filePath)
{
    std::string myXmlStr( "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>" );
	XmlTree doc(myXmlStr);
    XmlTree node;
	node.setTag("general");

	for(std::vector<ConfigParam>::iterator it = mConfigParameters.begin(); it!=mConfigParameters.end(); ++it)
	{
		XmlTree pn;
		pn.setTag(it->name);

		switch(it->type)
		{
		case _NODE:
			doc.push_back( node );
			node = XmlTree();
			node.setTag(it->name);
			break;
		case _BOOL:
            if( *((bool*)it->param) ) {
                pn.setValue<int>( 1 );
            } else {
                pn.setValue<int>( 0 );
            }
			node.push_back( pn );
			break;
		case _FLOAT:
			pn.setValue<float>( *((float*)it->param) );
			node.push_back( pn );
			break;
		case _DOUBLE:
			pn.setValue<double>( *((double*)it->param) );
			node.push_back( pn );
			break;
		case _INT:
			pn.setValue<int>( *((int*)it->param) );
			node.push_back( pn );
			break;
		case _VEC3F:
			pn.setAttribute<float>("x", (*((glm::fvec3*)it->param)).x);
			pn.setAttribute<float>("y", (*((glm::fvec3*)it->param)).y);
			pn.setAttribute<float>("z", (*((glm::fvec3*)it->param)).z);
			node.push_back( pn );
			break;
		case _QUATF:
			pn.setAttribute<float>("w", (*((glm::quat*)it->param)).w);
			pn.setAttribute<float>("x", (*((glm::quat*)it->param)).x);
			pn.setAttribute<float>("y", (*((glm::quat*)it->param)).y);
			pn.setAttribute<float>("z", (*((glm::quat*)it->param)).z);
			node.push_back( pn );
			break;
		case _COLOR:
			pn.setAttribute<float>("r", (*((Color*)it->param)).r);
			pn.setAttribute<float>("g", (*((Color*)it->param)).g);
			pn.setAttribute<float>("b", (*((Color*)it->param)).b);
			node.push_back( pn );
			break;
		case _COLORA:
			pn.setAttribute<float>("r", (*((ColorA*)it->param)).r);
			pn.setAttribute<float>("g", (*((ColorA*)it->param)).g);
			pn.setAttribute<float>("b", (*((ColorA*)it->param)).b);
			pn.setAttribute<float>("a", (*((ColorA*)it->param)).a);
			node.push_back( pn );
			break;
		case _STRING:
			pn.setValue<std::string>( *((std::string*)it->param) );
			node.push_back( pn );
			break;
		}
	}
	
	doc.push_back( node );
	doc.write(writeFile(filePath));
}
    
void Config::load(fs::path filePath)
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
				(*((glm::fvec3*)it->param)).x = node.getChild(it->name).getAttributeValue<float>("x");
				(*((glm::fvec3*)it->param)).y = node.getChild(it->name).getAttributeValue<float>("y");
				(*((glm::fvec3*)it->param)).z = node.getChild(it->name).getAttributeValue<float>("z");
				break;
			case _QUATF:
				(*((glm::quat*)it->param)).w = node.getChild(it->name).getAttributeValue<float>("w");
				(*((glm::quat*)it->param)).x = node.getChild(it->name).getAttributeValue<float>("x");
				(*((glm::quat*)it->param)).y = node.getChild(it->name).getAttributeValue<float>("y");
				(*((glm::quat*)it->param)).z = node.getChild(it->name).getAttributeValue<float>("z");
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
    
// New Params API -------------------------------------------------------------------

// some types might not work well right now (char, uint...)
// using the types specified in AntTweakBar.h (TW_TYPE_FLOAT, instead of _FLOAT)
// might be more a more accurate match to what's Params.h supports

template <> params::InterfaceGl::Options<bool>      Config::addParam( const std::string &name, bool *param, bool readOnly, const std::string &keyName )		{ return addParamImpl( name, param, _BOOL, readOnly, keyName ); }
template <> params::InterfaceGl::Options<char>		Config::addParam( const std::string &name, char *param, bool readOnly, const std::string &keyName )		{ return addParamImpl( name, param, _STRING, readOnly, keyName ); }
template <> params::InterfaceGl::Options<int8_t>	Config::addParam( const std::string &name, int8_t *param, bool readOnly, const std::string &keyName )	{ return addParamImpl( name, param, _INT, readOnly, keyName ); }
template <> params::InterfaceGl::Options<uint8_t>	Config::addParam( const std::string &name, uint8_t *param, bool readOnly, const std::string &keyName )	{ return addParamImpl( name, param, _INT, readOnly, keyName ); }
template <> params::InterfaceGl::Options<int16_t>	Config::addParam( const std::string &name, int16_t *param, bool readOnly, const std::string &keyName )	{ return addParamImpl( name, param, _INT, readOnly, keyName ); }
template <> params::InterfaceGl::Options<uint16_t>	Config::addParam( const std::string &name, uint16_t *param, bool readOnly, const std::string &keyName )	{ return addParamImpl( name, param, _INT, readOnly, keyName ); }
template <> params::InterfaceGl::Options<int32_t>	Config::addParam( const std::string &name, int32_t *param, bool readOnly, const std::string &keyName )	{ return addParamImpl( name, param, _INT, readOnly, keyName  ); }
template <> params::InterfaceGl::Options<uint32_t>	Config::addParam( const std::string &name, uint32_t *param, bool readOnly, const std::string &keyName )	{ return addParamImpl( name, param, _INT, readOnly, keyName  ); }
template <> params::InterfaceGl::Options<float>		Config::addParam( const std::string &name, float *param, bool readOnly, const std::string &keyName )	{ return addParamImpl( name, param, _FLOAT, readOnly, keyName  ); }
template <> params::InterfaceGl::Options<double>	Config::addParam( const std::string &name, double *param, bool readOnly, const std::string &keyName )	{ return addParamImpl( name, param, _DOUBLE, readOnly, keyName  ); }
template <> params::InterfaceGl::Options<string>	Config::addParam( const std::string &name, string *param, bool readOnly, const std::string &keyName )	{ return addParamImpl( name, param, _STRING, readOnly, keyName  ); }
template <> params::InterfaceGl::Options<Color>		Config::addParam( const std::string &name, Color *param, bool readOnly, const std::string &keyName )	{ return addParamImpl( name, param, _COLOR, readOnly, keyName  ); }
template <> params::InterfaceGl::Options<ColorA>	Config::addParam( const std::string &name, ColorA *param, bool readOnly, const std::string &keyName )	{ return addParamImpl( name, param, _COLORA, readOnly, keyName  ); }
template <> params::InterfaceGl::Options<glm::quat>	Config::addParam( const std::string &name, glm::quat *param, bool readOnly, const std::string &keyName )	{ return addParamImpl( name, param, _QUATF, readOnly, keyName  ); }
template <> params::InterfaceGl::Options<glm::fvec3>Config::addParam( const std::string &name, glm::fvec3 *param, bool readOnly, const std::string &keyName )	{ return addParamImpl( name, param, _VEC3F, readOnly, keyName  ); }
template <> params::InterfaceGl::Options<glm::dvec3>Config::addParam( const std::string &name, glm::dvec3 *param, bool readOnly, const std::string &keyName )	{ return addParamImpl( name, param, _VEC3F, readOnly, keyName  ); }

template <typename T>

params::InterfaceGl::Options<T>	Config::addParamImpl( const std::string &name, T *target, ConfigParamTypes aType, bool readOnly, const std::string &keyName )
{
    // should check for params initialized?
    addConfigParam(name, keyName, target, aType);
    return mParams->addParam(name, target, readOnly);
}


// Deprecated API -----------------------------------------------------------------------------

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

void Config::addParam( const std::string &name, glm::fvec3 *param, const std::string &optionsStr, bool readOnly, const std::string &keyName )
{
    if(mParamsInitialized)
        mParams->addParam(name, param, optionsStr, readOnly);
    addConfigParam(name, keyName, param, _VEC3F);
} 

void Config::addParam( const std::string &name, glm::quat *param, const std::string &optionsStr, bool readOnly, const std::string &keyName )
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
