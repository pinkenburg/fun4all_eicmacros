#ifndef MACRO_G4MAGNETBEAST_C
#define MACRO_G4MAGNETBEAST_C

#include <GlobalVariables.C>

#include <eicdetectors/BeastMagnetSubsystem.h>

#include <g4main/PHG4Reco.h>

R__LOAD_LIBRARY(libeicdetectors.so)

namespace Enable
{
  bool MAGNET = false;
  bool MAGNET_ABSORBER = false;
}  // namespace Enable

namespace G4MAGNET
{
  double magnet_outer_radius = 300.;
  double magnet_inner_radius = 135.;
  double magnet_length = 500.;
}  // namespace G4MAGNET

void MagnetFieldInit()
{
  if (!isfinite(G4MAGNET::magfield_rescale))
  {
    G4MAGNET::magfield_rescale = 1.;
  }
  if (G4MAGNET::magfield.empty())
  {
    G4MAGNET::magfield = string(getenv("CALIBRATIONROOT")) + string("/Field/Map/mfield.4col.dat");
  }
}

void MagnetInit()
{
  MagnetFieldInit();
  BlackHoleGeometry::max_radius = std::max(BlackHoleGeometry::max_radius, G4MAGNET::magnet_outer_radius);
  BlackHoleGeometry::max_z = std::max(BlackHoleGeometry::max_z, G4MAGNET::magnet_length / 2.);
  BlackHoleGeometry::min_z = std::min(BlackHoleGeometry::min_z, -G4MAGNET::magnet_length / 2.);
}

double Magnet(PHG4Reco* g4Reco, double radius)

{
  bool AbsorberActive = Enable::ABSORBER || Enable::MAGNET_ABSORBER;

  if (radius > G4MAGNET::magnet_inner_radius)
  {
    cout << "previous outer radius " << radius << " larger then magnet inner radius " << G4MAGNET::magnet_inner_radius << endl;
    gSystem->Exit(1);
  }
  BeastMagnetSubsystem* beast = new BeastMagnetSubsystem();
  beast->set_string_param("GDMPath", (string(getenv("CALIBRATIONROOT")) + string("/Magnet/BeastSolenoid.gdml")));
  beast->set_string_param("TopVolName", "SOLENOID");
  beast->SetActive(AbsorberActive);
  beast->SuperDetector("MAGNET");
  g4Reco->registerSubsystem(beast);
  return G4MAGNET::magnet_outer_radius;
}

#endif  // MACRO_G4MAGNETBEAST_C
