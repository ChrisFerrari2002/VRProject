/**
 * @file		ovr.h
 * @brief	Self-contained helper class for interfacing OpenGL and OpenVR. Shortened version of Overvision's OvVR module.
 *
 * @author	Achille Peternier (C) SUPSI [achille.peternier@supsi.ch]
 */
#pragma once

#include "engine.h"

 /**
  * @brief OpenGL-OpenVR interface.
  */
class ENG_API OvVR
{
public:
   struct Controller;

   enum OvEye
   {
      EYE_LEFT = 0,
      EYE_RIGHT = 1,
      EYE_LAST
   };

   OvVR();
   ~OvVR();

   bool init();
   bool free();
   std::string getTrackingSysName();
   bool printRenderModels();
   std::string getManufacturerName();
   std::string getModelNumber();
   unsigned int getHmdIdealHorizRes();
   unsigned int getHmdIdealVertRes();

   bool update();
   glm::mat4 getProjMatrix(OvEye eye, float nearPlane, float farPlane);
   glm::mat4 getEye2HeadMatrix(OvEye eye);
   glm::mat4 getModelviewMatrix();
   unsigned int getNrOfControllers();
   Controller* getController(unsigned int pos) const;
   void setReprojection(bool flag);
   void pass(OvEye eye, unsigned int eyeTexture);
   void render();

   class Controller
   {
   public:
      Controller();
      ~Controller();

      glm::mat4 getMatrix();
      glm::vec2 getAxis(unsigned int axisId, unsigned long long int& bPressed, unsigned long long int& bTouched);
      bool isButtonPressed(unsigned long long int id, unsigned long long int mask);

      unsigned int id;
      glm::mat4 matrix;
   private:
      struct ControllerImp;
      ControllerImp* cImpl;
      
      
   };

private:
   struct Impl;
   Impl* pImpl;
   std::vector<Controller*> controllers;
};