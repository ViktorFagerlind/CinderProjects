#include "SpriteDataParser.h"
#include "Macros.h"
#include <list>

using namespace std;
using namespace ci;

vector<SpriteData> SpriteDataParser::parseSpriteData(DataSourceRef dataSource, int format)
{
	cinder::XmlTree mainXml(dataSource);

	vector<SpriteData> spriteData;

	if (format == SpriteSheet::FORMAT_TEXTUREPACKER_GENERIC_XML)
  {
    for (auto item = mainXml.begin(); item != mainXml.end(); ++item ) 
    {
			XmlTree::Container& children = item->getChildren();

      for (auto iter = children.begin();iter != children.end(); iter++)
      {
				std::string tagName = (*iter)->getTag();

				if (tagName == "sprite")
        {
					int x=0, y=0, w=0, h=0, oX=0, oY=0, oW=0, oH=0;
					string name;

					std::list<XmlTree::Attr>& attributes = (*iter)->getAttributes();
					for(auto attr = attributes.begin();attr != attributes.end();attr ++)
          {
						if (attr->getName()=="n")
							name = attr->getValue();
						if (attr->getName()=="x")
							x = attr->getValue<int>();
						if (attr->getName()=="y")
							y = attr->getValue<int>();
						if (attr->getName()=="w")
							w = attr->getValue<int>();
						if (attr->getName()=="h")
							h = attr->getValue<int>();
						if (attr->getName()=="oX")
							oX = attr->getValue<int>();
						if (attr->getName()=="oY")
							oY = attr->getValue<int>();
						if (attr->getName()=="oW")
							oW = attr->getValue<int>();
						if (attr->getName()=="oH")
							oH = attr->getValue<int>();
					}

					SpriteData sprite = SpriteData();

					sprite.name = name;
					sprite.x = x;
					sprite.y = y;
					sprite.w = w;
					sprite.h = h;
					sprite.oX = oX;
					sprite.oY = oY;
					sprite.oW = oW;
					sprite.oH = oH;

					spriteData.push_back(sprite);
				}
			}
		}
	}
	
	return spriteData;
}