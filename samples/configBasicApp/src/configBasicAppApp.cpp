#include "cinder/app/AppNative.h"
#include "cinder/params/Params.h"

#include "CinderConfig.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class BasicApp : public AppNative {
public:
    void setup();
    void shutdown();
	void draw();
    
    void saveConfig();
    void loadConfig();
    
    params::InterfaceGlRef mParams;
    config::ConfigRef     mConfig;
    
	string	configFilename;
    
	Color	bgColor;
	bool	showRect,showCircle;
    float	rectRot,circleRadius;
	ColorA	rectColor,circleColor;
	Vec3f	rectPosition,circlePosition;
    
	string	text;
    
	double	doubleParam;
	int		intParam;
	Quatf	quatfParam;
    
	vector<string>	enumNames;
	int				enumValue;
};

void BasicApp::setup()
{
	setWindowSize(800, 600);
    
	configFilename = "config.xml";
	bgColor = Color::black();
    
	showRect = true;
	rectRot = 0.f;
	rectColor = ColorA::gray(0.5f);
	rectPosition = Vec3f::zero();
    
	showCircle = true;
	circleRadius = 70.f;
	circleColor = ColorA::gray(0.7f);
	circlePosition = Vec3f::zero();
    
	text = "Config Block for Cinder";
    
	doubleParam = 0.0; intParam = 0; quatfParam = Quatf();
	
	enumNames.push_back( "Orange" );
	enumNames.push_back( "Apple" );
	enumNames.push_back( "Banana" );
	enumValue = 0;
    
    
    
	//-----------------------------------------------------------------------------
    
    mParams = params::InterfaceGl::create( getWindow(), "Settings", toPixels( Vec2i( 400, 550 ) ) );
    mConfig = config::Config::create(mParams);
    
    mParams->addParam("Configuration file name", &configFilename);
    mParams->addButton("Save config", bind(&BasicApp::saveConfig, this));
    mParams->addButton("Load config", bind(&BasicApp::loadConfig, this));
    mParams->addSeparator();
	
    mConfig->addParam("Background color", &bgColor);
    mConfig->addParam("Text", &text);
    mParams->addSeparator();
    
	mConfig->newNode("Rectangle");
    mConfig->addParam("Show rectangle", &showRect);
    mConfig->addParam("Rectangle rotation", &rectRot).min(0).max(360).step(1.0f);  // we can use the new params API
    mConfig->addParam("Rectangle color", &rectColor);
    mConfig->addParam("Rectangle position", &rectPosition);
    mParams->addSeparator();
    
	mConfig->newNode("Circle");
	mConfig->addParam("Show circle", &showCircle);
    mConfig->addParam("Circle radius", &circleRadius);
    mConfig->addParam("Circle color", &circleColor);
	mConfig->addParam("Circle position", &circlePosition);
    
	mConfig->newNode("Other");
	mConfig->addParam("Double type parameter", &doubleParam);
	mConfig->addParam("Int type parameter", &intParam);
	mConfig->addParam("Quatf type parameter", &quatfParam);
	mConfig->addParam("Enum type parameter", enumNames, &enumValue);
    
    if(fs::exists( getAppPath() / fs::path(configFilename) ))
        loadConfig();
    
}

void BasicApp::shutdown()
{
    saveConfig();
}

void BasicApp::draw()
{
	gl::enableAlphaBlending();
	gl::setMatricesWindow( getWindowSize() );
	gl::clear( bgColor );
    
	if(showRect) {
		gl::color( rectColor );
		gl::pushMatrices();
		gl::rotate(rectRot);
		gl::drawSolidRect( Rectf(rectPosition.xy(), rectPosition.xy() + Vec2f(100.f,100.f)) );
		gl::popMatrices();
	}
    
	if(showCircle) {
		gl::color( circleColor );
		gl::drawSolidCircle(circlePosition.xy(), circleRadius);
	}
    
	mParams->draw();
}


void BasicApp::saveConfig()
{
    mConfig->save( getAppPath() / fs::path(configFilename) );
}

void BasicApp::loadConfig()
{
    mConfig->load( getAppPath() / fs::path(configFilename) );
}

CINDER_APP_NATIVE( BasicApp, RendererGl )