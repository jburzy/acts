/** TRACCC library, part of the ACTS project (R&D line)
 *
 * (c) 2023 CERN for the benefit of the ACTS project
 *
 * Mozilla Public License Version 2.0
 */

#pragma once

// Project include(s).
#include "traccc/definitions/primitives.hpp"
#include "traccc/edm/container.hpp"
#include "traccc/edm/device/triplet_counter.hpp"

namespace traccc::device {

/// Triplets of bottom, middle and top spacepoints
struct device_triplet {
  // top spacepoint location in internal spacepoint container
  unsigned int spB, spM, spT;

  using link_type = device::triplet_counter_collection_types::host::size_type;
  /// Link to triplet counter where the middle and bottom spacepoints are
  /// stored
  link_type counter_link;

  /// curvature of circle estimated from triplet
  scalar curvature;
  /// weight of triplet
  scalar weight;
  /// z origin of triplet
  scalar z_vertex;
  /// transverse impact parameter of the triplet
  scalar impact_parameter;
  /// cot(theta) of the middle-bottom doublet
  scalar cot_theta;
  /// number of compatible top spacepoints
  unsigned int n_compat;
  /// quality seed flag of the seed confirmation
  bool quality;
};

/// Declare all device triplet collection types
using device_triplet_collection_types = collection_types<device_triplet>;

}  // namespace traccc::device
