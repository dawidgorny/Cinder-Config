#include "cinder/app/AppNative.h"
#include "cinder/params/Params.h"

#include "CinderConfig.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class _TBOX_PREFIX_App : public AppNative {
public:
    void setup();
	void draw();
    
    void saveConfig();
    void loadConfig();
    
    params::InterfaceGlRef mParams;
    config::Config*     mConfig;
    
	string	configFilename;
    
};

void _TBOX_PREFIX_App::setup()
{
	setWindowSize(800, 600);
    
	configFilename = "config.xml";
    
	//-----------------------------------------------------------------------------
    
    mParams = params::InterfaceGl::create( getWindow(), "Settings", toPixels( Vec2i( 400, 550 ) ) );
    mConfig = new config::Config(mParams);
    
    mParams->addParam("Configuration file name", &configFilename);
    mParams->addButton("Save config", bind(&_TBOX_PREFIX_App::saveConfig, this));
    mParams->addButton("Load config", bind(&_TBOX_PREFIX_App::loadConfig, this));
    mParams->addSeparator();
    
    // node: general
    //mConfig->addParam("Background color", &bgColor);
    //mParams->addSeparator();
    
    // node: section1
    //mConfig->newNode("section1");
    //mConfig->addParam("Show rectangle", &showRect);
    
}

void _TBOX_PREFIX_App::draw()
{
	gl::setMatricesWindow( getWindowSize() );
	gl::clear(Color::black());
    
	mParams->draw();
}

void _TBOX_PREFIX_App::saveConfig()
{
    mConfig->save( getAppPath() / fs::path(configFilename) );
}

void _TBOX_PREFIX_App::loadConfig()
{
    mConfig->load( getAppPath() / fs::path(configFilename) );
}

CINDER_APP_NATIVE( _TBOX_PREFIX_App, RendererGl )