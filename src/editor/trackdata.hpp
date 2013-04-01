// This file is part of Dust Racing 2D.
// Copyright (C) 2011 Jussi Lind <jussi.lind@iki.fi>
//
// Dust Racing 2D is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// Dust Racing 2D is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Dust Racing 2D. If not, see <http://www.gnu.org/licenses/>.

#ifndef TRACKDATA_HPP
#define TRACKDATA_HPP

#include <QString>

#include "../common/trackdatabase.hpp"
#include "../common/mapbase.hpp"
#include "../common/route.hpp"
#include "../common/objects.hpp"

#include "map.hpp"

class ObjectModelLoader;
class UndoStackItemBase;

class TrackData : public TrackDataBase
{
public:

    //! Constructor.
    TrackData(QString name, unsigned int cols, unsigned int rows);

    //! Destructor.
    virtual ~TrackData();

    //! Get name.
    QString name() const;

    //! Get file name.
    QString fileName() const;

    //! Set file name.
    void setFileName(QString fileName);

    //! Get map object.
    MapBase & map();

    //! Get map object.
    const MapBase & map() const;

    //! Get route object.
    Route & route();

    //! Get route object.
    const Route & route() const;

    //! Get objects object.
    Objects & objects();

    //! Get objects object.
    const Objects & objects() const;

    //! Enlarge horizontal size.
    void enlargeHorSize();

    //! Enlarge vertical size.
    void enlargeVerSize();

    //! Add item to undo stack.
    void addItemToUndoStack(UndoStackItemBase * item);

    //! Returns true if there are more undoable operations left after the undo, false otherwise.
    bool undo(const ObjectModelLoader & loader);

    //! Returns true if there are more redoable operations left after the redo, false otherwise.
    bool redo(const ObjectModelLoader & loader);

private:

    typedef std::vector< std::shared_ptr< UndoStackItemBase > > UndoStack;

    QString      m_name;
    QString      m_fileName;
    Map          m_map;
    Objects      m_objects;
    Route        m_route;
    UndoStack    m_undoStack;
    UndoStack::iterator m_undoStackPosition;
};

#endif // TRACKDATA_HPP
