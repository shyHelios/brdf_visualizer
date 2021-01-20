/*
* This is a personal academic project. Dear PVS-Studio, please check it.
* PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
*/
#include "linevertexbufferobject.h"
#include "vbos/gizmo.h"

//std::shared_ptr<LineVertexBufferObject> LineVertexBufferObject::gizmo = nullptr;

LineVertexBufferObject::LineVertexBufferObject(const std::vector<Vertex> &vertices, const std::vector<unsigned int> &indices, int lineWidth)
    : VertexBufferObject(vertices, indices), lineWidth(lineWidth) {}

void LineVertexBufferObject::draw() {
  glCall(glBindVertexArray(VAO));
  glCall(glLineWidth(lineWidth));
  glCall(glDrawElements(GL_LINES, indices_.size(), GL_UNSIGNED_INT, NULL));
  glCall(glBindVertexArray(0));
  glCall(glLineWidth(1));
}

//void LineVertexBufferObject::setupStaticObjects() {
//  LineVertexBufferObject::gizmo = std::make_shared<LineVertexBufferObject>(
//      std::vector<Vertex>(std::begin(vbo::gizmo::vertices), std::end(vbo::gizmo::vertices)),
//      std::vector<unsigned int>(std::begin(vbo::gizmo::indices), std::end(vbo::gizmo::indices)),
//      5);
//
//}

void LineVertexBufferObject::drawLines() {
  glCall(glBindVertexArray(VAO));
  glCall(glLineWidth(1));
  glCall(glDrawElements(GL_LINES, indices_.size(), GL_UNSIGNED_INT, NULL));
  glCall(glBindVertexArray(0));
}

//void LineVertexBufferObject::deleteStaticObjects() {
//  auto gizmoRaw = gizmo.get();
//  gizmo.unique()
//  delete gizmoRaw;
//}
