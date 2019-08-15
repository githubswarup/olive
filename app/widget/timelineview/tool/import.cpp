/***

  Olive - Non-Linear Video Editor
  Copyright (C) 2019 Olive Team

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.

***/

#include "widget/timelineview/timelineview.h"

#include <QMimeData>

#include "node/input/media/media.h"

TimelineView::ImportTool::ImportTool(TimelineView *parent) :
  Tool(parent)
{
}

void TimelineView::ImportTool::DragEnter(QDragEnterEvent *event)
{
  QStringList mime_formats = event->mimeData()->formats();

  // Listen for MIME data from a ProjectViewModel
  if (mime_formats.contains("application/x-oliveprojectitemdata")) {
    // FIXME: Implement audio insertion

    // Data is drag/drop data from a ProjectViewModel
    QByteArray model_data = event->mimeData()->data("application/x-oliveprojectitemdata");

    // Use QDataStream to deserialize the data
    QDataStream stream(&model_data, QIODevice::ReadOnly);

    // Variables to deserialize into
    quintptr item_ptr;
    int r;

    // Set ghosts to start where the cursor entered
    rational ghost_start = parent()->ScreenToTime(event->pos().x());

    while (!stream.atEnd()) {
      stream >> r >> item_ptr;

      // Get Item object
      Item* item = reinterpret_cast<Item*>(item_ptr);

      // Check if Item is Footage
      if (item->type() == Item::kFootage) {
        // If the Item is Footage, we can create a Ghost from it
        Footage* footage = static_cast<Footage*>(item);

        StreamPtr stream = footage->stream(0);

        TimelineViewGhostItem* ghost = new TimelineViewGhostItem();

        rational footage_duration(stream->timebase().numerator() * stream->duration(),
                                  stream->timebase().denominator());

        ghost->SetIn(ghost_start);
        ghost->SetOut(ghost_start + footage_duration);
        ghost->SetData(QVariant::fromValue(stream));

        parent()->AddGhost(ghost);

        // Stack each ghost one after the other
        ghost_start += footage_duration;
      }
    }

    drag_start_ = event->pos();

    event->accept();
  } else {
    // FIXME: Implement dropping from file
    event->ignore();
  }
}

void TimelineView::ImportTool::DragMove(QDragMoveEvent *event)
{
  if (parent()->HasGhosts()) {
    // Move ghosts to the mouse cursor
    foreach (TimelineViewGhostItem* ghost, parent()->ghost_items_) {
      QPoint movement = event->pos() - drag_start_;

      rational time_movement = parent()->ScreenToTime(movement.x());

      ghost->SetInAdjustment(time_movement);
      ghost->SetOutAdjustment(time_movement);
    }

    event->accept();
  } else {
    event->ignore();
  }
}

void TimelineView::ImportTool::DragLeave(QDragLeaveEvent *event)
{
  if (parent()->HasGhosts()) {
    parent()->ClearGhosts();

    event->accept();
  } else {
    event->ignore();
  }
}

void TimelineView::ImportTool::DragDrop(QDropEvent *event)
{
  if (parent()->HasGhosts()) {
    // We use QObject as the parent for the nodes we create. If there is no TimelineOutput node, this object going out
    // of scope will delete the nodes. If there is, they'll become parents of the NodeGraph instead
    QObject node_memory_manager;

    foreach (TimelineViewGhostItem* ghost, parent()->ghost_items_) {
      ClipBlock* clip = new ClipBlock();
      MediaInput* media = new MediaInput();

      // Set parents to node_memory_manager in case no TimelineOutput receives this signal
      // FIXME: Moving nodes to shared_ptrs might be a better idea, except they all use the QObject system for hierarchy
      //        already...
      clip->setParent(&node_memory_manager);
      media->setParent(&node_memory_manager);

      clip->set_length(ghost->Length());
      media->SetFootage(ghost->data().value<StreamPtr>()->footage());

      NodeParam::ConnectEdge(media->texture_output(), clip->texture_input());

      if (event->keyboardModifiers() & Qt::ControlModifier) {
        emit parent()->RequestInsertBlockAtTime(clip, ghost->GetAdjustedIn());
      } else {
        emit parent()->RequestPlaceBlock(clip, ghost->GetAdjustedIn());
      }
    }

    parent()->ClearGhosts();

    event->accept();
  } else {
    event->ignore();
  }
}