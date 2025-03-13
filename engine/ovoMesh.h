#pragma once

class OvMesh
{
public:
   /**
    * @brief Enumeration representing the kind of mesh.
    */
   enum class Subtype : int
   {
      DEFAULT = 0,        ///< Default mesh type.
      NORMALMAPPED,       ///< Normal-mapped mesh type.
      TESSELLATED,        ///< Tessellated mesh type.
      LAST                ///< Terminator to indicate the last subtype.
   };
};