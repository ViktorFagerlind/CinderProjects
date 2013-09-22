#pragma once

#include "cinder/Cinder.h"
#include "cinder/app/App.h"
#include "cinder/Xml.h"
#include "cinder/DataSource.h"
#include "SpriteData.h"
#include "SpriteSheet.h"

class SpriteDataParser{
public:
	static std::vector<SpriteData> parseSpriteData (ci::DataSourceRef dataSource, int format);

private:
	static ci::XmlTree spriteXml;
	static std::vector<SpriteData> sprites;
};
