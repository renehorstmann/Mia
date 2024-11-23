#include "o/OPattern.h"
#include "o/OObj_builder.h"
#include "o/OArray.h"

#define O_LOG_LIB "o"
#include "o/log.h"


#define FIND_ALL_CAPACITY_JUMPS 16

#define GROUPS_CAP_JUMPS 16
#define CAPTURES_CAP_JUMPS 8


// other sets are generated in code, like dot or uppercase versions
static const char SET_c[] = "\x00\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0A\x0B\x0C\x0D\x0E\x0F"        \
              "\x10\x11\x12\x13\x14\x15\x16\x17\x18\x19\x1A\x1B\x1C\x1D\x1E\x1F\x7F";
static const char SET_p[] = "\x21\x22\x23\x24\x25\x26\x27\x28\x29\x2A\x2B\x2C\x2D\x2E\x2F\x3A"        \
              "\x3B\x3C\x3D\x3E\x3F\x40\x5B\x5C\x5D\x5E\x5F\x60\x7B\x7C\x7D\x7E";
static const char SET_s[] = "\x09\x0A\x0B\x0C\x0D\x20";


O_INLINE
void OPattern_group_set(struct OPattern_group *self, obyte position, bool set)
{
    obyte bitmask = 1 << (position % 8);
    if (set) {
        self->flags[position / 8] |= bitmask;
    } else {
        self->flags[position / 8] &= ~bitmask;
    }
}

O_STATIC
void OPattern_group_set_range(struct OPattern_group *self, obyte from, obyte to)
{
    for (obyte i = from; i <= to; i++) {
        OPattern_group_set(self, i, true);
    }
}

O_STATIC
void OPattern_group_set_set(struct OPattern_group *self, const char *set, int set_size)
{
    for (int i = 0; i < set_size; i++) {
        OPattern_group_set(self, (obyte) set[i], true);
    }
}

O_STATIC
void OPattern_group_inv(struct OPattern_group *self)
{
    for (int i = 0; i < 32; i++) {
        self->flags[i] = ~self->flags[i];
    }
}

O_STATIC
void OPattern_group_or(struct OPattern_group *self, const struct OPattern_group *or)
{
    for (int i = 0; i < 32; i++) {
        self->flags[i] |= or->flags[i];
    }
}

O_STATIC
void OPattern_group_set_dot(struct OPattern_group *self)
{
    for (int i = 0; i < 32; i++) {
        self->flags[i] = 0xFF;
    }
}

O_STATIC
void OPattern_group_set_a(struct OPattern_group *self)
{
    OPattern_group_set_range(self, 'A', 'Z');
    OPattern_group_set_range(self, 'a', 'z');
}

O_STATIC
void OPattern_group_set_c(struct OPattern_group *self)
{
    OPattern_group_set_set(self, SET_c, sizeof(SET_c));
}

O_STATIC
void OPattern_group_set_d(struct OPattern_group *self)
{
    OPattern_group_set_range(self, '0', '9');
}

O_STATIC
void OPattern_group_set_l(struct OPattern_group *self)
{
    OPattern_group_set_range(self, 'a', 'z');
}

O_STATIC
void OPattern_group_set_p(struct OPattern_group *self)
{
    OPattern_group_set_set(self, SET_p, sizeof(SET_p));
}

O_STATIC
void OPattern_group_set_s(struct OPattern_group *self)
{
    OPattern_group_set_set(self, SET_s, sizeof(SET_s));
}

O_STATIC
void OPattern_group_set_u(struct OPattern_group *self)
{
    OPattern_group_set_range(self, 'A', 'Z');
}

O_STATIC
void OPattern_group_set_w(struct OPattern_group *self)
{
    OPattern_group_set_range(self, '0', '9');
    OPattern_group_set_range(self, 'A', 'Z');
    OPattern_group_set_range(self, 'a', 'z');
}

O_STATIC
void OPattern_group_set_x(struct OPattern_group *self)
{
    OPattern_group_set_range(self, '0', '9');
    OPattern_group_set_range(self, 'A', 'F');
    OPattern_group_set_range(self, 'a', 'f');
}

O_STATIC
void OPattern_group_set_z(struct OPattern_group *self)
{
    OPattern_group_set(self, '\0', true);
}

O_STATIC
void OPattern_group_set_A(struct OPattern_group *self)
{
    struct OPattern_group g = {0};
    OPattern_group_set_a(&g);
    OPattern_group_inv(&g);
    OPattern_group_or(self, &g);
}

O_STATIC
void OPattern_group_set_C(struct OPattern_group *self)
{
    struct OPattern_group g = {0};
    OPattern_group_set_c(&g);
    OPattern_group_inv(&g);
    OPattern_group_or(self, &g);
}

O_STATIC
void OPattern_group_set_D(struct OPattern_group *self)
{
    struct OPattern_group g = {0};
    OPattern_group_set_d(&g);
    OPattern_group_inv(&g);
    OPattern_group_or(self, &g);
}

O_STATIC
void OPattern_group_set_L(struct OPattern_group *self)
{
    struct OPattern_group g = {0};
    OPattern_group_set_l(&g);
    OPattern_group_inv(&g);
    OPattern_group_or(self, &g);
}

O_STATIC
void OPattern_group_set_P(struct OPattern_group *self)
{
    struct OPattern_group g = {0};
    OPattern_group_set_p(&g);
    OPattern_group_inv(&g);
    OPattern_group_or(self, &g);
}

O_STATIC
void OPattern_group_set_S(struct OPattern_group *self)
{
    struct OPattern_group g = {0};
    OPattern_group_set_s(&g);
    OPattern_group_inv(&g);
    OPattern_group_or(self, &g);
}

O_STATIC
void OPattern_group_set_U(struct OPattern_group *self)
{
    struct OPattern_group g = {0};
    OPattern_group_set_u(&g);
    OPattern_group_inv(&g);
    OPattern_group_or(self, &g);
}

O_STATIC
void OPattern_group_set_W(struct OPattern_group *self)
{
    struct OPattern_group g = {0};
    OPattern_group_set_w(&g);
    OPattern_group_inv(&g);
    OPattern_group_or(self, &g);
}

O_STATIC
void OPattern_group_set_X(struct OPattern_group *self)
{
    struct OPattern_group g = {0};
    OPattern_group_set_x(&g);
    OPattern_group_inv(&g);
    OPattern_group_or(self, &g);
}

O_STATIC
void OPattern_group_set_Z(struct OPattern_group *self)
{
    struct OPattern_group g = {0};
    OPattern_group_set_z(&g);
    OPattern_group_inv(&g);
    OPattern_group_or(self, &g);
}

O_STATIC
void parse(OPattern *self, const char *pattern)
{
    osize pattern_len = o_strlen(pattern);

    osize groups_cap = GROUPS_CAP_JUMPS;
    self->groups = o_new0(self, struct OPattern_group, groups_cap);
    self->groups_size = 0;

    osize captures_cap = CAPTURES_CAP_JUMPS;
    self->captures = o_new(self, struct o_str_range, captures_cap);
    self->captures_size = 0;

    bool ctrl = false;

    bool in_group = false;
    bool group_inv = false;

    struct OPattern_group w_check = {0};
    OPattern_group_set_w(&w_check);

    for (osize p_idx = 0; p_idx < pattern_len; p_idx++) {
        obyte p = (obyte) pattern[p_idx];
        bool is_ctrl = ctrl;
        ctrl = false;

        struct OPattern_group *current = &self->groups[self->groups_size];

        if (is_ctrl) {
            // prev was %

            if (p == 'a') {
                OPattern_group_set_a(current);
            } else if (p == 'A') {
                OPattern_group_set_A(current);
            } else if (p == 'c') {
                OPattern_group_set_c(current);
            } else if (p == 'C') {
                OPattern_group_set_C(current);
            } else if (p == 'd') {
                OPattern_group_set_d(current);
            } else if (p == 'D') {
                OPattern_group_set_D(current);
            } else if (p == 'l') {
                OPattern_group_set_l(current);
            } else if (p == 'L') {
                OPattern_group_set_L(current);
            } else if (p == 'p') {
                OPattern_group_set_p(current);
            } else if (p == 'P') {
                OPattern_group_set_P(current);
            } else if (p == 's') {
                OPattern_group_set_s(current);
            } else if (p == 'S') {
                OPattern_group_set_S(current);
            } else if (p == 'u') {
                OPattern_group_set_u(current);
            } else if (p == 'U') {
                OPattern_group_set_U(current);
            } else if (p == 'w') {
                OPattern_group_set_w(current);
            } else if (p == 'W') {
                OPattern_group_set_W(current);
            } else if (p == 'x') {
                OPattern_group_set_x(current);
            } else if (p == 'X') {
                OPattern_group_set_X(current);
            } else {
                // just an escaped character
                OPattern_group_set(current, p, true);
            }
        } else {
            // not is_ctrl

            if (p == '.') {
                OPattern_group_set_dot(current);
            }

                // anchors
            else if (p_idx == 0 && p == '^') {
                self->anchored_start = true;
                continue;
            } else if (p_idx == pattern_len - 1 && p == '$') {
                self->anchored_end = true;
                continue;
            }

                // groups
            else if (!in_group && p == '[') {
                in_group = true;
                obyte next = (obyte) pattern[p_idx + 1];
                if (next == '^') {
                    group_inv = true;
                    p_idx++;
                } else {
                    group_inv = false;
                }
                continue;
            } else if (in_group && p == ']') {
                if (group_inv) {
                    OPattern_group_inv(current);
                }
                in_group = false;
                group_inv = false;
            }

                // ctrl
            else if (p == '%') {
                ctrl = true;
                continue;
            }

                // captures
            else if (!in_group && p == '(') {
                self->captures[self->captures_size].start = self->groups_size;
                self->captures[self->captures_size].len = 0;
                self->captures_size++;
                if (self->captures_size >= captures_cap) {
                    captures_cap += CAPTURES_CAP_JUMPS;
                    self->captures = o_renew(self, self->captures, struct o_str_range, captures_cap);
                }
                continue;
            } else if (!in_group && p == ')') {
                for (osize cap = self->captures_size - 1; cap >= 0; cap--) {
                    if (self->captures[cap].len == 0) {
                        self->captures[cap].len = self->groups_size - self->captures[cap].start;
                        break;
                    }
                }
                continue;
            }

                // character...
            else {
                bool range = false;
                if (OPattern_group_check(&w_check, p) && p_idx < pattern_len - 2) {
                    // check if it's a range
                    obyte range_minus = (obyte) pattern[p_idx + 1];
                    obyte range_to = (obyte) pattern[p_idx + 2];

                    if (range_minus == '-' && OPattern_group_check(&w_check, range_to)) {
                        OPattern_group_set_range(current, p, range_to);
                        range = true;
                        p_idx += 2;
                    }
                }

                if (!range) {
                    // just a single character
                    OPattern_group_set(current, p, true);
                }
            }
        }

        if (!in_group) {
            // check group modifier
            obyte next = (obyte) pattern[p_idx + 1];
            if (next == '+') {
                current->mode = OPattern_group_MODE_PLUS;
                p_idx++;
            } else if (next == '*' || next == '-') {
                current->mode = OPattern_group_MODE_MUL;
                p_idx++;
            } else if (next == '?') {
                current->mode = OPattern_group_MODE_OPT;
                p_idx++;
            } else {
                current->mode = OPattern_group_MODE_SINGLE;
            }

            // resize groups
            self->groups_size++;
            if (self->groups_size >= groups_cap) {
                groups_cap += GROUPS_CAP_JUMPS;
                self->groups = o_renew(self, self->groups, struct OPattern_group, groups_cap);
                o_clear(&self->groups[self->groups_size], sizeof(struct OPattern_group), GROUPS_CAP_JUMPS);
            }
        }

    }

    // close group if ']' was missing...
    if (in_group) {
        o_log_warn_s("OPattern_init",
                   "pattern finished with an opened group, \']\' was missing!");
        if (group_inv) {
            OPattern_group_inv(&self->groups[self->groups_size]);
        }
        self->groups_size++;
    }

    // close captures if ')' were missing...
    for (osize i = 0; i < self->captures_size; i++) {
        int missed = 0;
        if (self->captures[i].len == 0) {
            missed++;
            self->captures[i].len = self->groups_size - self->captures[i].start;
        }
        if (missed > 0) {
            o_log_warn_s("OPattern_init",
                       "pattern finished with (%i) opened capture, \')\' was missing!", missed);
        }
    }
}

//
// public
//

OPattern *OPattern_init(oobj obj, oobj parent, const char *pattern)
{
    OPattern *self = obj;
    o_clear(self, sizeof *self, 1);

    OObj_init(self, parent);
    OObj_id_set(self, OPattern_ID);

    parse(self, pattern);

    return self;
}

//
// object functions
//

O_STATIC
struct o_str_range find_at(OPattern *self, const void *data, osize data_size,
                           struct o_str_range *captures, osize start_idx)
{

    for (osize i = 0; i < self->captures_size; i++) {
        captures[i] = (struct o_str_range) {0};
    }

    struct o_str_range range = {0};
    osize group_idx = 0;
    osize group_cnt = 0;
    const char *data_buf = data;

    struct reppoint {
        osize pos;
        osize group;
        osize group_cnt;
    } reppoint;
    reppoint.pos = -1;
    oobj reps = OArray_new_dyn(self, NULL, sizeof(struct reppoint), 0, 16);

    osize pos = start_idx;

    for (;;) {

        // if a rep point was used, always ignore the character
        // default is to use it, so "Hello World", "(%w+)" will return "Hello", not "H"
        bool used_repstuff = reppoint.pos >= 0;
        if (used_repstuff) {
            pos = reppoint.pos;
            group_idx = reppoint.group;
            group_cnt = reppoint.group_cnt;
        }
        reppoint.pos = -1;

        // check captures on new group (or group end, which is marked with a group "overflow")
        if (group_cnt == 0) {
            for (osize cap = 0; cap < self->captures_size; cap++) {
                if (self->captures[cap].start == group_idx) {
                    captures[cap].start = pos;
                }
                if ((self->captures[cap].start + self->captures[cap].len) == group_idx) {
                    captures[cap].len = pos - captures[cap].start;
                }
            }
        }

        // check if the current pattern would be valid, even if end of data is reached
        if (pos >= data_size && group_idx < self->groups_size) {
            // valid if remaining groups are OPT or MUL or PLUS (which has a cnt of at least 1)
            bool valid = true;
            for (osize gi = group_idx; gi < self->groups_size; gi++) {
                if (self->groups[gi].mode != OPattern_group_MODE_MUL
                    && self->groups[gi].mode != OPattern_group_MODE_OPT
                    && !(self->groups[gi].mode == OPattern_group_MODE_PLUS && group_cnt > 0)) {
                    valid = false;
                    break;
                }
            }

            if (valid) {
                // set to end
                group_idx = self->groups_size;
                group_cnt = 0;

                // close opened captures
                for (osize cap = 0; cap < self->captures_size; cap++) {
                    if (captures[cap].start > 0 && captures[cap].len == 0) {
                        captures[cap].len = pos - captures[cap].start;
                    }
                }
            }
        }

        if (group_idx >= self->groups_size) {
            // valid!
            range.start = start_idx;
            range.len = pos - start_idx;
            goto CLEAN_UP;
        }

        if (pos >= data_size) {

            // do other rep points exist?
            if (OArray_num(reps) > 0) {
                // return to first of rep buffer
                OArray_pop(reps, &reppoint);
                continue;
            }

            // invalid!
            range = (struct o_str_range) {0};
            goto CLEAN_UP;
        }

        char test = data_buf[pos];
        bool valid = OPattern_group_check(&self->groups[group_idx], test);

        if (!valid) {
            if ((group_cnt > 0 && self->groups[group_idx].mode == OPattern_group_MODE_PLUS)
                || self->groups[group_idx].mode == OPattern_group_MODE_MUL
                || self->groups[group_idx].mode == OPattern_group_MODE_OPT) {
                // not valid was possible, retry with next group
                group_idx++;
                group_cnt = 0;
                continue;
            }
            // return to first of rep buffer
            if (OArray_num(reps) > 0) {
                OArray_pop(reps, &reppoint);
                continue;
            }

            // invalid
            range = (struct o_str_range) {0};
            goto CLEAN_UP;
        }

        if (self->groups[group_idx].mode == OPattern_group_MODE_SINGLE) {
            // just a single valid, next character, next group
            group_idx++;
            group_cnt = 0;
            pos++;
            continue;
        }

        // if possible, always try to use multiple at first run and add a rep point

        if (self->groups[group_idx].mode == OPattern_group_MODE_PLUS) {
            if (!used_repstuff && group_cnt > 0) {
                // add rep and check next character, same group
                reppoint.pos = pos;
                reppoint.group = group_idx;
                reppoint.group_cnt = group_cnt;
                OArray_push(reps, &reppoint);
                reppoint.pos = -1;
                group_cnt++;
                pos++;
                continue;
            }

            if (group_cnt == 0) {
                // first in group
                group_cnt++;
                pos++;
                continue;
            }
        }

        if (self->groups[group_idx].mode == OPattern_group_MODE_MUL) {
            if (!used_repstuff) {
                // add rep and check next character, same group
                reppoint.pos = pos;
                reppoint.group = group_idx;
                reppoint.group_cnt = group_cnt;
                OArray_push(reps, &reppoint);
                reppoint.pos = -1;
                group_cnt++;
                pos++;
                continue;
            }
        }

        if (self->groups[group_idx].mode == OPattern_group_MODE_OPT) {
            if (!used_repstuff) {
                // add rep and check next character, next group
                reppoint.pos = pos;
                reppoint.group = group_idx;
                reppoint.group_cnt = 0;
                OArray_push(reps, &reppoint);
                reppoint.pos = -1;
                group_idx++;
                group_cnt = 0;
                pos++;
                continue;
            }
        }

        // rep already used, test next group
        group_idx++;
        group_cnt = 0;
    }

    CLEAN_UP:

    // clear unfinished captures
    for (osize i = 0; i < self->captures_size; i++) {
        if (captures[i].len == 0) {
            captures[i].start = 0;
        }
    }

    o_del(reps);
    return range;
}

struct o_str_range OPattern_find_data(oobj obj, const void *data, osize data_size,
                                      struct o_str_range *out_opt_captures)
{
    OObj_assert(obj, OPattern);
    OPattern *self = obj;

    struct o_str_range range = {0};

    if (!data || data_size <= 0) {
        o_opt_set(out_opt_captures, range);
        return range;
    }

    struct o_str_range *captures = o_new0(self, struct o_str_range, self->captures_size);

    for (osize i = 0; i < data_size - self->min_groups; i++) {
        range = find_at(self, data, data_size, captures, i);
        if (range.len != 0 || self->anchored_start) {
            break;
        }
    }

    if (self->anchored_end) {
        if ((range.start + range.len) != data_size) {
            range = (struct o_str_range) {0};
        }
    }

    if (out_opt_captures) {
        for (osize i = 0; i < self->captures_size; i++) {
            out_opt_captures[i] = range.len >= 1 ? captures[i] : (struct o_str_range) {0};
        }
    }
    o_free(self, captures);
    return range;
}


osize OPattern_find_all_data(oobj obj,
                             struct o_str_range_list *out_opt_matches,
                             struct o_str_range_list_list *out_opt_captures_list,
                             const void *data, osize data_size)
{
    OObj_assert(obj, OPattern);
    OPattern *self = obj;

    if (!data || data_size <= 0) {
        o_opt_set(out_opt_matches, (struct o_str_range_list) {0});
        o_opt_set(out_opt_captures_list, (struct o_str_range_list_list) {0});
        return 0;
    }

    osize cnt = 0;
    osize capacity = FIND_ALL_CAPACITY_JUMPS;
    struct o_str_range_list matches = {
            o_new(obj, struct o_str_range, capacity),
            0,
            obj
    };
    struct o_str_range_list_list captures_list = {
            o_new(obj, struct o_str_range *, capacity),
            0,
            obj
    };

    struct o_str_range_list captures = {
            o_new0(obj, struct o_str_range, OPattern_captures(obj) + 1),
            OPattern_captures(obj),
            obj
    };

    const char *data_buf = data;

    osize start = 0;
    for (;;) {
        struct o_str_range next = OPattern_find_data(self, &data_buf[start], data_size - start, NULL);
        next.start += start;

        start = (next.start + next.len);
        matches.list[cnt] = next;
        captures_list.list[cnt] = captures;

        // create a capture list for the next find
        captures.list = o_new0(obj, struct o_str_range, OPattern_captures(obj) + 1);

        if (next.len == 0) {
            break;
        }

        cnt++;

        if (start >= data_size || self->anchored_start) {
            break;
        }

        if (cnt >= capacity) {
            // resize lists
            capacity += FIND_ALL_CAPACITY_JUMPS;
            matches.list = o_renew(obj, matches.list, struct o_str_range, capacity);
            captures_list.list = o_renew(obj, captures_list.list, struct o_str_range *, capacity);
        }
    }

    if (self->anchored_start) {
        // free all other capture lists
        for (osize i = 1; i < cnt; i++) {
            o_str_range_list_free(&captures_list.list[i]);
        }
        // check anchors (also if both are set)
        if (cnt > 0 && (matches.list[0].start != 0
                        || (self->anchored_end && (matches.list[0].start + matches.list[0].len != data_size)))) {
            // invalid anchor, also free the capture list
            o_str_range_list_free(&captures_list.list[0]);
            cnt = 0;
        }
        cnt = o_min(1, cnt);
    } else if (self->anchored_end) {
        // free all other capture lists
        for (osize i = 0; i < cnt - 1; i++) {
            o_str_range_list_free(&captures_list.list[i]);
        }
        if (cnt > 0 && (matches.list[cnt - 1].start + matches.list[cnt - 1].len != data_size)) {
            // invalid anchor, also free the capture list
            o_str_range_list_free(&captures_list.list[cnt - 1]);
            cnt = 0;
        }
        if (cnt > 1) {
            // not aliasing...
            o_memcpy(&matches.list[0], &matches.list[cnt - 1], sizeof(struct o_str_range), 1);
            captures_list.list[0] = captures_list.list[cnt - 1];
        }
        cnt = o_min(1, cnt);
    }

    matches.len = cnt;
    captures_list.len = cnt;

    // clean up and return:
    if (out_opt_matches) {
        *out_opt_matches = matches;
    } else {
        o_str_range_list_free(&matches);
    }
    if (out_opt_captures_list) {
        *out_opt_captures_list = captures_list;
    } else {
        o_str_range_list_list_free(&captures_list);
    }
    // free the remaining tmp captures list
    o_str_range_list_free(&captures);

    return cnt;
}


osize OPattern_replace_data(oobj obj, void **out_data, const void *data, osize data_size,
                            const void *replace, osize replace_size, osize *out_opt_replaced)
{
    assert(out_data != NULL);

    if (!data || data_size <= 0) {
        o_opt_set(out_data, NULL);
        o_opt_set(out_opt_replaced, 0);
        return 0;
    }

    if (!replace || replace_size < 0) {
        replace = "";
        replace_size = 0;
    }

    struct o_str_range_list matches;
    osize cnt = OPattern_find_all_data(obj, &matches, NULL, data, data_size);

    // safe for cnt==0...

    // get size
    osize new_size = data_size;
    for (osize i = 0; i < cnt; i++) {
        new_size -= matches.list[i].len;
    }
    new_size += replace_size * cnt;

    char *res = o_new(obj, char, new_size + 1);
    const char *data_buf = data;

    osize start = 0;
    osize idx = 0;
    for (osize i = 0; i < cnt; i++) {
        // copy until start
        o_memcpy(&res[idx], &data_buf[start], 1, matches.list[i].start - start);
        idx += matches.list[i].start - start;
        start = matches.list[i].start + matches.list[i].len;

        // copy replacement
        o_memcpy(&res[idx], replace, 1, replace_size);
        idx += replace_size;
    }

    // copy remaining
    o_memcpy(&res[idx], &data_buf[start], 1, data_size - start);
    idx += data_size - start;

    assert(idx == new_size);

    // add null terminator
    res[new_size] = '\0';

    // set result
    *out_data = res;
    o_opt_set(out_opt_replaced, cnt);

    o_str_range_list_free(&matches);
    return new_size;
}
