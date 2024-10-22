#include "duneanaobj/StandardRecord/SREnums.h"

// But this also gives us an opportunity to instantiate the template for
// various DUNE-specific enums that would otherwise be missing symbols.
namespace caf {
template class Proxy<Detector>;
template class Proxy<Generator>;
template class Proxy<PartEMethod>;
template class Proxy<ScatteringMode>;

template class Proxy<NDLAR_RECO_STACK>;
template class Proxy<FD_RECO_STACK>;

template class Proxy<TrueParticleID::PartType>;
} // namespace caf
