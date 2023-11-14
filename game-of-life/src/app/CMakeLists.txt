#/*============================================================================
#
#  PHAS0100ASSIGNMENT1: PHAS0100 Assignment 1 Game of Life Simulation.
#
#  Copyright (c) University College London (UCL). All rights reserved.
#
#  This software is distributed WITHOUT ANY WARRANTY; without even
#  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
#  PURPOSE.
#
#  See LICENSE.txt in the top level directory for details.
#
#============================================================================*/

set(_command_line_apps
  gol_simulator
)

foreach(_app ${_command_line_apps})
  add_executable(golSimulator ${_app}.cpp) # only gol_simulator is present, edit if more files are included
  target_link_libraries(golSimulator ${PHAS0100ASSIGNMENT1_LIBRARIES} ${ALL_THIRD_PARTY_LIBRARIES})
endforeach()
