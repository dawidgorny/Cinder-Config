//
//  CinderConfig.h
//  Cinder Config Block
//
//  Created by Dawid Górny on 5/21/12.
//  Copyright (c) 2012 Dawid Górny. All rights reserved.
//

#pragma once

#include "cinder/app/AppBasic.h"
#include "cinder/Cinder.h"
#include "cinder/Color.h"
#include "cinder/Quaternion.h"
#include "cinder/Function.h"
#include "cinder/params/Params.h"
#include "cinder/Xml.h"

#include <string>
#include <vector>
#include <iterator>

#include <boost/regex.hpp>

namespace cinder { namespace config {
 
//-----------------------------------------------------------------------------

enum ConfigParamTypes
{
	_NODE,
    _BOOL,
    _FLOAT,
    _DOUBLE,
    _INT,
    _VEC3F,
    _QUATF,
    _COLOR,
    _COLORA,
    _STRING
};

//-----------------------------------------------------------------------------

class ConfigParam
{
public:
    ConfigParam(const std::string &aName, void* aParam, ConfigParamTypes aType) 
    {
		// === Convert name to alphanum
        boost::regex alphaNumTest("\\W+", boost::regex::perl); // \W matches any non-word character alphanumeric plus _
		name = boost::regex_replace( aName, alphaNumTest, "_", boost::match_default | boost::format_perl ) ;
        std::transform(name.begin(), name.end(), name.begin(), ::tolower); // convert name to lowercase

        param = aParam;
        type = aType;
    }
	~ConfigParam() 
	{ 
		param = NULL;
	}
    
    std::string name;
    void* param;
    ConfigParamTypes type;
};

//-----------------------------------------------------------------------------

class Config {
public:
    Config() { mParamsInitialized = false; };
    Config(params::InterfaceGl*	paramsInterfaceGl) { mParams = paramsInterfaceGl; mParamsInitialized = true; };
    
    //-----------------------------------------------------------------------------
    
    void    save(std::string filePath);
	void    load(std::string filePath);
    
    //-----------------------------------------------------------------------------
    
    void	addParam( const std::string &name, bool *boolParam, const std::string &optionsStr = "", bool readOnly = false, const std::string &keyName = "" );
	void	addParam( const std::string &name, float *floatParam, const std::string &optionsStr = "", bool readOnly = false, const std::string &keyName = "" );
	void	addParam( const std::string &name, double *doubleParam, const std::string &optionsStr = "", bool readOnly = false, const std::string &keyName = "" );
	void	addParam( const std::string &name, int32_t *intParam, const std::string &optionsStr = "", bool readOnly = false, const std::string &keyName = "" );
	void	addParam( const std::string &name, Vec3f *vectorParam, const std::string &optionsStr = "", bool readOnly = false, const std::string &keyName = "" );
	void	addParam( const std::string &name, Quatf *quatParam, const std::string &optionsStr = "", bool readOnly = false, const std::string &keyName = "" );
	void	addParam( const std::string &name, Color *quatParam, const std::string &optionsStr = "", bool readOnly = false, const std::string &keyName = "" );
	void	addParam( const std::string &name, ColorA *quatParam, const std::string &optionsStr = "", bool readOnly = false, const std::string &keyName = "" );
	void	addParam( const std::string &name, std::string *strParam, const std::string &optionsStr = "", bool readOnly = false, const std::string &keyName = "" );
	void	addParam( const std::string &name, const std::vector<std::string> &enumNames, int *param, const std::string &optionsStr = "", bool readOnly = false, const std::string &keyName = "" );
	
	void	newNode( const std::string &name, const std::string &optionsStr = "" );

protected:
    void    addConfigParam( const std::string &name, const std::string &keyName, void* param, ConfigParamTypes type);
    
    std::vector<ConfigParam>    mConfigParameters;
    params::InterfaceGl*        mParams;
    bool                        mParamsInitialized;
};   
    
} } // namespace cinder::config
