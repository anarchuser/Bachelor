#include "Action.h"

namespace bt {
    Action::Action (port_t who, ActionType what)
            : when {get_timestamp()}
            , who {who}
            , what {what}
            {}

    Action::Action (port_t who, std::int32_t change)
            : when {get_timestamp()}
            , who {who}
            , what {ADD}
            , value {change}
            {}

    Action::Action (port_t who, PosChange move)
            : when {get_timestamp()}
            , who {who}
            , what {MOVE}
            , value {move}
            {}

    Position Position::operator + (Position other) const {
        return Position (x + other.x, y + other.y);
    }
    Position & Position::operator += (Position other) {
        x += other.x;
        y += other.y;
        return * this;
    }
    bool Position::operator == (Position other) const {
        return x == other.x && y == other.y;
    }

    std::ostream & operator << (std::ostream & os, Position pos) {
        return os << pos.x << ";" << pos.y;
    }
    std::ostream & operator << (std::ostream & os, PosChange move) {
        Position init (move.reference.x - move.delta.x, move.reference.y - move.delta.y);
        return os << "(" << init.x << "->" << move.reference.x << "|" << init.y << "->" << move.reference.y << ")";
    }

    std::ostream & operator << (std::ostream & os, ActionType type) {
        switch (type) {
            case NOOP:      return os << "NOOP";
            case FORBIDDEN: return os << "FORBIDDEN";
            case ADD:       return os << "ADD";
            case MOVE:      return os << "MOVE";
            default:        return os << "[UNKNOWN]";
        }
    }

    std::ostream & operator << (std::ostream & os, Action const & action) {
        os << "[" << action.who;
        os << "|" << action.what;
        if (action.what == ADD) {
            os << "(" << std::right << std::setfill (' ') << std::setw(3) << action.value.change << ")";
        } else if (action.what == MOVE) {
            os << "(" << std::right << std::setfill (' ') << std::setw(3) << action.value.move << ")";
        }
        os << "|@" << action.when;
        return os << "]";
    }

    std::int32_t operator + (std::int32_t state, Action const & action) {
        switch (action.what) {
            case FORBIDDEN:
                LOG (WARNING) << "\tTrying to apply a forbidden action!";
            case NOOP:
            case MOVE:
                return state;
            case ADD:
                return state + action.value.change;
            default:
                LOG (WARNING) << "\tTrying to apply an unrecognisable action!";
                return state;
        }
    }

    Position operator + (Position state, Action const & action) {
        switch (action.what) {
            case FORBIDDEN:
                LOG (WARNING) << "\tTrying to apply a forbidden action!";
            case NOOP:
            case ADD:
                return state;
            case MOVE:
                return state + action.value.move.delta;
            default:
                LOG (WARNING) << "\tTrying to apply an unrecognisable action!";
                return state;
        }
    }
}

/* Copyright © 2022 Aaron Alef */