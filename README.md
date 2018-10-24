# Cinder-SinglePassCube
Cinder example of a single-pass cubemap using gl extensions

This example requires a build of Paul Houx's reverse-z Cinder branch using GLAD.
https://github.com/paulhoux/Cinder/tree/reverse-z

This example uses #extension GL_EXT_geometry_shader4, GL_NV_viewport_array2, and GL_NV_geometry_shader_passthrough

To use these extensions, go to https://glad.dav1d.de/ and generate a zip with all available extensions and replace glad.h/glad.c in the Cinder folders.
