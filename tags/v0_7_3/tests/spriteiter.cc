// Test iterating a sprite file

#include "tbx/sprite.h"
#include <iostream>

int main(int argc, char *argv[])
{
   tbx::SpriteArea area;
   if (!area.load("<TBX$Dir>.!Sprites"))
   {
      std::cerr << "Unable to load sprite area <TBX$Dir>.!Sprites" << std::endl;
      return -1;
   }

   std::cout << "List of sprites in <TBX$Dir>.!Sprites" << std::endl;
   for (tbx::SpriteArea::iterator i = area.begin(); i != area.end(); ++i)
   {
      std::cout << "  " << (*i).name() << std::endl;
   }

   return 0;
}
