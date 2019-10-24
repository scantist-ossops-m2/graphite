/*  GRAPHITE2 LICENSING

    Copyright 2010, SIL International
    All rights reserved.

    This library is free software; you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published
    by the Free Software Foundation; either version 2.1 of License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should also have received a copy of the GNU Lesser General Public
    License along with this library in the file named "LICENSE".
    If not, write to the Free Software Foundation, 51 Franklin Street,
    Suite 500, Boston, MA 02110-1335, USA or visit their web page on the
    internet at http://www.fsf.org/licenses/lgpl.html.

Alternatively, the contents of this file may be used under the terms of the
Mozilla Public License (http://mozilla.org/MPL) or the GNU General Public
License, as published by the Free Software Foundation, either version 2
of the License or (at your option) any later version.
*/
#pragma once

#include "graphite2/Segment.h"

#include "inc/Main.h"
#include "inc/Position.h"

namespace graphite2 {

typedef gr_attrCode attrCode;

class GlyphFace;
class Segment;
class Slot;
class SlotMap;
class Font;

struct SlotJustify
{
    static const int NUMJUSTPARAMS = 5;

    SlotJustify(const SlotJustify &);
    SlotJustify & operator = (const SlotJustify &);

public:
    static size_t size_of(size_t levels) { return sizeof(SlotJustify) + ((levels > 1 ? levels : 1)*NUMJUSTPARAMS - 1)*sizeof(int16); }

    void LoadSlot(const Slot &s, const Segment & seg);

    SlotJustify *next;
    int16 values[1];
};

class Slot
{
    enum Flag
    {
        DELETED     = 1,
        INSERTED    = 2,
        COPIED      = 4,
        POSITIONED  = 8,
        ATTACHEDX   = 16,
        ATTACHEDY   = 32
    };

public:
    struct iterator;

    unsigned short gid() const { return m_glyphid; }
    Position origin() const { return m_position; }
    float advance() const { return m_advance.x; }
    void advance(Position &val) { m_advance = val; }
    Position advancePos() const { return m_advance; }
    int cluster() const { return m_cluster; }
    uint32 index() const { return m_index; }
    void index(uint32 val) { m_index = val; }

    Slot(int16 *m_userAttr = NULL);
    void set(const Slot & slot, int charOffset, size_t numUserAttr, size_t justLevels);
    Slot *next() const { return m_next; }
    void next(Slot *s) { m_next = s; }
    Slot *prev() const { return m_prev; }
    void prev(Slot *s) { m_prev = s; }
    uint16 glyph() const { return m_realglyphid ? m_realglyphid : m_glyphid; }
    void setGlyph(Segment &seg, uint16 glyphid, const GlyphFace * theGlyph = NULL);
    void setRealGid(uint16 realGid) { m_realglyphid = realGid; }
    void adjKern(const Position &pos) { m_shift = m_shift + pos; m_advance = m_advance + pos; }
    void origin(const Position &pos) { m_position = pos; }
    void originate(int ind) { m_original = ind; }
    int original() const { return m_original; }
    void cluster(int ind) { m_cluster = ind; }
    bool isBase() const { return (!m_parent); }
    void resetGuard() { m_guard_shift = 0.; m_guard_adv = 0.; }
    float guardShift() const { return m_guard_shift; }
    float guardAdv() const { return m_guard_adv; }
    void update(int numSlots, int numCharInfo, Position &relpos);
    Position finalise(const Segment & seg, const Font* font, Position & base, Rect & bbox, float & clusterMin, bool rtl, bool isFinal, int depth = 0);
    void position_1(float clusterMin, float clusterMax, uint32 cluster, bool rtl, int depth);
    Position position_2(Position &base, uint32 &cluster, Position origin, const Font *font, Segment *seg, bool rtl, bool isFinal, int depth);
    bool isDeleted() const { return (m_flags & DELETED) ? true : false; }
    void markDeleted(bool state) { if (state) m_flags |= DELETED; else m_flags &= ~DELETED; }
    bool isCopied() const { return (m_flags & COPIED) ? true : false; }
    void markCopied(bool state) { if (state) m_flags |= COPIED; else m_flags &= ~COPIED; }
    bool isPositioned() const { return (m_flags & POSITIONED) ? true : false; }
    void markPositioned(bool state) { if (state) m_flags |= POSITIONED; else m_flags &= ~POSITIONED; }
    bool isAttachedX() const { return (m_flags & ATTACHEDX) ? true : false; }
    void markAttachedX(bool state) { if (state) m_flags |= ATTACHEDX; else m_flags &= ~ATTACHEDX; }
    bool isAttachedY() const { return (m_flags & ATTACHEDY) ? true : false; }
    void markAttachedY(bool state) { if (state) m_flags |= ATTACHEDY; else m_flags &= ~ATTACHEDY; }
    bool isInsertBefore() const { return !(m_flags & INSERTED); }
    int16 *userAttrs() const { return m_userAttr; }
    void userAttrs(int16 *p) { m_userAttr = p; }
    void markInsertBefore(bool state) { if (!state) m_flags |= INSERTED; else m_flags &= ~INSERTED; }
    void setAttr(Segment & seg, attrCode ind, uint8 subindex, int16 val, const SlotMap & map);
    int getAttr(const Segment &seg, attrCode ind, uint8 subindex) const;
    int getJustify(const Segment &seg, uint8 level, uint8 subindex) const;
    void setJustify(Segment &seg, uint8 level, uint8 subindex, int16 value);
    bool isLocalJustify() const { return m_justs != NULL; };
    void attachTo(Slot *ap) { m_parent = ap; }
    Slot *attachedTo() const { return m_parent; }
    Position attachOffset() const { return m_attachat; }
    Slot* firstChild() const { return m_child; }
    void firstChild(Slot *ap) { m_child = ap; }
    bool child(Slot *ap);
    Slot* nextSibling() const { return m_sibling; }
    void nextSibling(Slot *ap) { m_sibling = ap; }
    bool sibling(Slot *ap);
    bool removeChild(Slot *ap);
    int32 clusterMetric(const Segment & seg, uint8 metric, bool rtl);
    void positionShift(Position a) { m_position += a; }
    void floodShift(Position adj, int depth = 0);
    float just() const { return m_just; }
    void just(float j) { m_just = j; }
    Slot *nextInCluster(const Slot *s) const;
    bool isChildOf(const Slot *base) const;

    CLASS_NEW_DELETE

private:
    Slot *m_next;           // linked list of slots
    Slot *m_prev;
    unsigned short m_glyphid;        // glyph id
    uint16 m_realglyphid;
    uint32 m_original;      // charinfo that originated this slot (e.g. for feature values)
    uint32 m_cluster;       // charinfo index for this slot
    uint32 m_index;         // slot index given to this slot during finalising
    Slot *m_parent;         // index to parent we are attached to
    Slot *m_child;          // index to first child slot that attaches to us
    Slot *m_sibling;        // index to next child that attaches to our parent
    Position m_position;    // absolute position of glyph
    Position m_shift;       // .shift slot attribute
    Position m_advance;     // .advance slot attribute
    Position m_attachat;    // position relative to base
    float    m_guard_adv;   // Extra guard advance
    float    m_guard_shift; // Extra guard shift
    float    m_just;        // Justification inserted space
    uint8    m_flags;       // holds bit flags
    int16   *m_userAttr;    // pointer to user attributes
    SlotJustify *m_justs;   // pointer to justification parameters

    friend class Segment;
};

} // namespace graphite2

struct gr_slot : public graphite2::Slot {};
