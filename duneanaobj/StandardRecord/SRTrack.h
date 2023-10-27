////////////////////////////////////////////////////////////////////////
/// \file    SRTrack.h
/// \brief   Reconstructed track object
/// \author  J. Wolcott <jwolcott@fnal.gov>
/// \date    Sept. 2021
////////////////////////////////////////////////////////////////////////
#ifndef DUNEANAOBJ_SRTRACK_H
#define DUNEANAOBJ_SRTRACK_H

#include "duneanaobj/StandardRecord/SREnums.h"
#include "duneanaobj/StandardRecord/SRVector3D.h"
#include "duneanaobj/StandardRecord/SRTrueParticle.h"

namespace caf
{
  class SRTrack
  {
    public:
      // less typing further below
      static constexpr float NaN = std::numeric_limits<float>::signaling_NaN();

      SRVector3D start;      ///< Track 3D start point [cm]
      SRVector3D end;        ///< Track 3D end point [cm]
      SRVector3D dir;        ///< Unit vector representing estimate of track direction *taken from start point*
      SRVector3D enddir;     ///< Unit vector representing estimate of track direction *taken from endpoint*

      float Evis     = NaN;  ///< Visible energy in voxels corresponding to this track

      // Track characteristics
      float qual = NaN;      ///< Reco-specific quality metric (in TMS, equivalent to "hits in track"/"total hits in event"

      float len_gcm2 = NaN;  ///< Track length in g/cm2
      float len_cm   = NaN;  ///< Track length in centimeter (actual physical distance)

      float E = NaN;         ///< Track energy estimate in MeV

      TrueParticleID truth;  ///< Best-match GEANT truth particle for this track.  Use SRTruthBranch::Particle() to obtain this particle
  };

}

std::ostream & operator<<(std::ostream & stream, const caf::SRTrack & tr);

#endif //DUNEANAOBJ_SRTRACK_H
