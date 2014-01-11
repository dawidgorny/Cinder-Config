// Cinder Config Block
//
// Copyright (c) 2012, Dawid Górny dawidgorny.com
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to
// deal in the Software without restriction, including without limitation the
// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
// sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.
//
// This code is intended to be used with the Cinder C++ library, http://libcinder.org

#pragma once

//#include "cinder/app/App.h"
#include "cinder/Cinder.h"
#include "cinder/Color.h"
#include "cinder/Quaternion.h"
#include "cinder/Function.h"
#include "cinder/params/Params.h"
#include "cinder/Xml.h"

#include <string>
#include <vector>
#include <iterator>

#include <boost/algorithm/string.hpp>

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
        name = aName;
        
		// === Convert name to alphanum and replace spaces
        // trim spaces
        boost::trim(name);
        // remove multiple spaces
        std::function<bool(char,char)> isConsecutiveWhiteSpace = []( char a, char b ) { return isspace(a) && isspace(b); };
        name.erase( std::unique(name.begin(), name.end(), isConsecutiveWhiteSpace), name.end() );
        // replace spaces with _
        std::replace(name.begin(), name.end(), ' ', '_');
        // remove all not valid chars
        std::function<int(int)> isNameChar = [](char c){ return ( isalnum(c) || c == '_' ); };
        name.erase( std::remove_if(name.begin(), name.end(), not1( isNameChar ) ), name.end() );
        // convert name to lowercase
        std::transform(name.begin(), name.end(), name.begin(), ::tolower); 
        
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

class Config;
typedef std::shared_ptr<Config> ConfigRef;
    
class Config {
public:
    static ConfigRef create() { return std::make_shared<Config>(); }
    
    Config() { mParamsInitialized = false; };
    Config(params::InterfaceGlRef	paramsInterfaceGl) { mParams = paramsInterfaceGl; mParamsInitialized = true; };
    
    //-----------------------------------------------------------------------------
    
    void    save(fs::path filePath);
	void    load(fs::path filePath);
    
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
    params::InterfaceGlRef        mParams;
    bool                        mParamsInitialized;
};   
    
} } // namespace cinder::config
