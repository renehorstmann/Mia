#ifndef MIA_DISCR_H
#define MIA_DISCR_H

/**
 * @file discr.h
 *
 * discretize primitives like 2d lines, curves, circles, rect borders into points
 *
 */

#include "o/common.h"
#include "m/types/flt.h"
#include "m/sca/flt.h"
#include "r/rect.h"
#include "r/quad.h"


/**
 * Creates an OArray of r_rect's from an OArray of vec2's
 * @param points_array OArray of vec2
 * @param init besides xy, copies this into all created rects
 * @return OArray of struct r_rect
 */
O_EXTERN
oobj u_discr_as_rects(oobj points_array, struct r_rect init);

/**
 * Creates an OArray of r_quad's from an OArray of vec2's
 * @param points_array OArray of vec2
 * @param init besides xy, copies this into all created quad
 * @return OArray of struct r_quad
 */
O_EXTERN
oobj u_discr_as_quads(oobj points_array, struct r_quad init);

/**
 * Appends a single point to the points array ; just calls OArray_push(points_array, &pt)
 * @param points_array OArray of vec2
 * @note useful as starting point before calling u_discr_line for example
 */
O_EXTERN
void u_discr_point(oobj points_array, vec2 pt);

/**
 * Appends a line segment to the points array
 * @param points_array OArray of vec2
 * @param a start point (not included)
 * @param b end point (included)
 * @param dist distance between each sampled point, may be reduced to create equal density
 * @return number of sampled points
 * @note does >NOT< include the starting point
 */
O_EXTERN
int u_discr_line(oobj points_array, vec2 a, vec2 b, float dist);

/**
 * Appends an axis aligned rectangle to the points array
 * @param points_array OArray of vec2
 * @param a edge
 * @param b edge on the opposite site
 * @param dist distance between each sampled point, may be reduced to create equal density
 * @return number of sampled points
 * @note all edge points are included
 */
O_EXTERN
int u_discr_rect(oobj points_array, vec2 a, vec2 b, float dist);

/**
 * Appends a circle bow to the points array
 * @param points_array OArray of vec2
 * @param center of the circle
 * @param radius of the circle
 * @param a_rad start angle in radiant (not included)
 * @param b_rad end angle in radiant (included)
 * @param dist distance between each sampled point, may be reduced to create equal density
 * @return number of sampled points
 * @note does >NOT< include the starting point
 */
O_EXTERN
int u_discr_circle_bow(oobj points_array, vec2 center, float radius, float a_rad, float b_rad, float dist);


/**
 * Appends a full circle to the points array
 * @param points_array OArray of vec2
 * @param center of the circle
 * @param radius of the circle
 * @param dist distance between each sampled point, may be reduced to create equal density
 * @return number of sampled points
 */
O_INLINE
int u_discr_circle(oobj points_array, vec2 center, float radius, float dist)
{
    return u_discr_circle_bow(points_array, center, radius, 0, 2*m_PI, dist);
}

/**
 * Appends a quadratic Bezier curve to the points array.
 * @param points_array OArray of vec2
 * @param p0 First control point
 * @param p1 Second control point
 * @param p2 Third control point
 * @param dist Desired distance between sampled points
 * @return Number of sampled points
 * @note Does NOT include the starting point p0 by default.
 */
O_EXTERN
int u_discr_bezier(oobj points_array, vec2 p0, vec2 p1, vec2 p2, float dist);


#endif //MIA_DISCR_H
