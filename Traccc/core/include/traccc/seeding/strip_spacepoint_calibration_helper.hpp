/** TRACCC library, part of the ACTS project (R&D line)
 *
 * (c) 2026 CERN for the benefit of the ACTS project
 *
 * Mozilla Public License Version 2.0
 */

#pragma once

// Project include(s).
#include "traccc/definitions/math.hpp"
#include "traccc/definitions/primitives.hpp"
#include "traccc/definitions/qualifiers.hpp"
#include "traccc/edm/spacepoint_collection.hpp"

// System include(s).
#include <array>

namespace traccc {

/// Helper functions moving strip spacepoints along their strips for a given
/// track direction, following the Acts strip spacepoint calibration
struct strip_spacepoint_calibration_helper {
  using vector3_t = std::array<float, 3>;

  /// Quantities of a strip spacepoint that do not depend on the direction
  struct derived {
    vector3_t inner_cross_outer_half_vector;
    vector3_t separation_cross_outer_half_vector;
    vector3_t separation_cross_inner_half_vector;
    vector3_t outer_center;
    vector3_t outer_half_vector;
  };

  static TRACCC_HOST_DEVICE inline float dot(const vector3_t& a,
                                             const vector3_t& b) {
    return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
  }

  static TRACCC_HOST_DEVICE inline vector3_t cross(const vector3_t& a,
                                                   const vector3_t& b) {
    return {a[1] * b[2] - a[2] * b[1], a[2] * b[0] - a[0] * b[2],
            a[0] * b[1] - a[1] * b[0]};
  }

  /// Derive the direction independent quantities of a strip spacepoint
  template <typename T>
  static TRACCC_HOST_DEVICE inline derived derive(
      const edm::spacepoint<T>& sp) {
    const vector3_t ihv = sp.strip_inner_half_vector();
    const vector3_t ohv = sp.strip_outer_half_vector();
    const vector3_t iosv = sp.strip_inner_to_outer_separation();
    return {cross(ihv, ohv), cross(iosv, ohv), cross(iosv, ihv),
            sp.strip_outer_center(), ohv};
  }

  /// Move the spacepoint along its strips to the crossing of a track with the
  /// given direction
  ///
  /// @param[in] direction the track direction at the spacepoint
  /// @param[in] sp the direction independent quantities of the spacepoint
  /// @param[out] calibrated the position of the crossing
  /// @param[in] tolerance allowed excess over the strip half length
  /// @return false if the crossing is not on the strips
  ///
  static TRACCC_HOST_DEVICE inline bool calibrate(const vector3_t& direction,
                                                  const derived& sp,
                                                  vector3_t& calibrated,
                                                  const float tolerance) {
    const float scale = dot(direction, sp.inner_cross_outer_half_vector);

    // position along the inner strip, relative to its half length
    const float s_inner = dot(direction, sp.separation_cross_outer_half_vector);
    if (math::fabs(s_inner) > math::fabs(scale) * tolerance) {
      return false;
    }

    // position along the outer strip, relative to its half length
    const float s_outer = dot(direction, sp.separation_cross_inner_half_vector);
    if (math::fabs(s_outer) > math::fabs(scale) * tolerance) {
      return false;
    }

    const float s_outer_norm = s_outer / scale;
    calibrated = {sp.outer_center[0] + sp.outer_half_vector[0] * s_outer_norm,
                  sp.outer_center[1] + sp.outer_half_vector[1] * s_outer_norm,
                  sp.outer_center[2] + sp.outer_half_vector[2] * s_outer_norm};
    return true;
  }
};

}  // namespace traccc
