# Olive - Non-Linear Video Editor
# Copyright (C) 2019 Olive Team
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

set(OLIVE_SOURCES
  ${OLIVE_SOURCES}
  render/backend/opengl/openglbackend.h
  render/backend/opengl/openglbackend.cpp
  render/backend/opengl/openglcolorprocessor.h
  render/backend/opengl/openglcolorprocessor.cpp
  render/backend/opengl/openglexporter.h
  render/backend/opengl/openglexporter.cpp
  render/backend/opengl/openglframebuffer.h
  render/backend/opengl/openglframebuffer.cpp
  render/backend/opengl/openglproxy.h
  render/backend/opengl/openglproxy.cpp
  render/backend/opengl/openglrenderfunctions.h
  render/backend/opengl/openglrenderfunctions.cpp
  render/backend/opengl/openglshader.h
  render/backend/opengl/openglshader.cpp
  render/backend/opengl/openglshadercache.h
  render/backend/opengl/opengltexture.h
  render/backend/opengl/opengltexture.cpp
  render/backend/opengl/opengltexturecache.h
  render/backend/opengl/opengltexturecache.cpp
  render/backend/opengl/openglworker.h
  render/backend/opengl/openglworker.cpp
  PARENT_SCOPE
)
