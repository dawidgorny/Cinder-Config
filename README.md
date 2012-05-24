Cinder-Config
=============

This is a configuration file loader and saver class designed for use with the open-source C++ library Cinder: http://libcinder.org

Usage example
------------

    params::InterfaceGl mParams;
    config::Config*     mConfig;
    
    mParams = params::InterfaceGl( "Settings", Vec2i( 400, 550 ) );
    mConfig = new config::Config(&mParams);
    
    mConfig->addParam("Bool type parameter", &boolParam);
    mConfig->addParam("Float type parameter", &floatParam);
    mConfig->addParam("Double type parameter", &doubleParam);
    mConfig->addParam("Int type parameter", &intParam);
    mConfig->addParam("Quatf type parameter", &quatfParam);
    mConfig->addParam("Enum type parameter", enumNames, &enumValue);
    ...
    mConfig->save( getAppPath().string() + "config.xml" );
    ...
    mConfig->load( getAppPath().string() + "config.xml" );

See sample in samples directory.