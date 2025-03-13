#pragma once

class OvLight
{
public:
   /**
    * @brief Enumeration representing the kind of light.
    */
   enum class Subtype : int
   {
      OMNI = 0,           ///< Omni light type.
      DIRECTIONAL,        ///< Directional light type.
      SPOT,               ///< Spot light type.
      LAST                ///< Terminator to indicate the last subtype.
   };
};
