# HurtEngine
A hobby project of Chris Hurt. This 3D C++ engine features a basic render engine, physics engine, input manager, and entity-component system.

## Blender Export Instructions
* Select File->Export->Wavefront (.obj)
* Set "Forward" to "Z Forward"
* Make sure the following are selected:
  * Apply Modifiers *(optional)*
  * Write Normals
  * Include UVs *(optional)*
  * Triangulate Faces
  * Objects as OBJ Objects
* Export to project "res/" folder

## Image Format
* 32-bit PNG
* Square
* 2<sup>n</sup> side length
* All alpha values set to 0 or 1